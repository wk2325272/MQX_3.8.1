/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: part_mgr.c$
* $Version : 3.8.12.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the functions for a partition manager device that is 
*   is built to work under MFS1.40. These functions can also create or 
*   remove partitions from a disk.
*
*END************************************************************************/

#include <mqx.h>
#include <fio.h>


#include "mfs.h"
#include "part_mgr.h"

extern _mfs_error _mfs_validate_device(MQX_FILE_PTR dev_fd, uint_32_ptr sector_size_ptr, boolean _PTR_ block_mode_ptr);
static int_32 _io_part_mgr_uninstall_internal(IO_DEVICE_STRUCT_PTR dev_ptr);

/*FUNCTION*---------------------------------------------------------------
*
* Function Name  : _io_part_mgr_install
* Returned Value : int_32 error code
* Comments       : installs the partition manager device
*                  
*
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_install           
    (
    MQX_FILE_PTR dev_fd,     /*[IN] Handle of the device on which to install the partition manager */
    char_ptr    identifier, /*[IN] The new name of the partition manager device */  
    uint_32     sector_size /*[IN] The size of sectors in bytes on the lower level device */     
    )                                      
{                         
    PART_MGR_STRUCT_PTR           pm_struct_ptr;
    int_32                        error_code;

    pm_struct_ptr = MFS_mem_alloc_system_zero( sizeof(PART_MGR_STRUCT));
    if ( pm_struct_ptr == NULL )
    {
        return( PMGR_INSUF_MEMORY );
    } 

    _mem_set_type(pm_struct_ptr, MEM_TYPE_PART_MGR_STRUCT);

    /*
    ** Create a light weight semaphore
    */
    _lwsem_create(&(pm_struct_ptr->SEM), 0);

    /* Store the handle of the device in the lower layer */
    pm_struct_ptr->DEV_FILE_PTR = dev_fd;
    pm_struct_ptr->STATE = PMGR_INVALID_STATE;

    error_code = _io_dev_install_ext( identifier, 
        _io_part_mgr_open,              
        _io_part_mgr_close,             
        _io_part_mgr_read,              
        _io_part_mgr_write,             
        (_mqx_int(_CODE_PTR_)(MQX_FILE_PTR,_mqx_uint,pointer))_io_part_mgr_ioctl,
        _io_part_mgr_uninstall_internal,
        (pointer) pm_struct_ptr );                         

    if ( error_code )
    {
        _lwsem_destroy(&pm_struct_ptr->SEM);
        _mem_free(pm_struct_ptr);
    }
    else
    {
        _lwsem_post(&(pm_struct_ptr->SEM));   
    } 

    return error_code;

}                          


/*FUNCTION*---------------------------------------------------------------
*
* Function Name  : _io_part_mgr_uninstall_internal
* Returned Value : int_32 error code
* Comments       : uninstalls the partition manager device
*                  
*
*END*--------------------------------------------------------------------*/

static int_32 _io_part_mgr_uninstall_internal
    (
    IO_DEVICE_STRUCT_PTR        dev_ptr     /*[IN] The identifier of the device */
    )                                      
{ 
    PART_MGR_STRUCT_PTR pt_mgr_ptr;

    pt_mgr_ptr = (PART_MGR_STRUCT_PTR) dev_ptr->DRIVER_INIT_PTR; 
    _lwsem_destroy(&pt_mgr_ptr->SEM);  
    _mem_free(pt_mgr_ptr);
    return(IO_OK);
} 

#if MFSCFG_DEPRICATED_UNINSTALL
/*FUNCTION*---------------------------------------------------------------
*
* Function Name  : _io_part_mgr_uninstall
* Returned Value : int_32 error code
* Comments       : uninstalls the partition manager device
*                  
*
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_uninstall           
    (
    char_ptr          identifier    /*[IN] The identifier string of the device */
    )                                      
{  
    return _io_dev_uninstall(identifier);
}  
#endif


/*FUNCTION*---------------------------------------------------------------
*
* Function Name  : _io_part_mgr_open
* Returned Value : MQX_OK
* Comments       : Does nothing. All the initialization is done in
*                  the io_part_mgr_install function
*                  
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_open
    (
    MQX_FILE_PTR   fd_ptr,          /* [IN] the file pointer of the partition manager to open */
    char_ptr       open_name_ptr,   /* [IN] the name of the device that is being opened */
    char_ptr       open_mode_ptr    /* [IN] the flags that determine the open mode */
    )
{   
    PART_MGR_STRUCT_PTR   pm_struct_ptr;
    PMGR_PART_INFO_STRUCT entry_info;
    MQX_FILE_PTR          dev_fd;
    char_ptr              part_table_disk;
    char_ptr              tmp_ptr;
    uint_32               i;
    uint_32               sector_size;
    int_32                error, size;

    pm_struct_ptr = (PART_MGR_STRUCT_PTR)fd_ptr->DEV_PTR->DRIVER_INIT_PTR; 
    dev_fd = pm_struct_ptr->DEV_FILE_PTR;

    error = _mfs_validate_device(dev_fd, &sector_size,&pm_struct_ptr->BLOCK_MODE);
    if ( error )
    {
        return error;
    }

    tmp_ptr = MFS_mem_alloc_system(sector_size);

    if ( !tmp_ptr )
    {
        return( PMGR_INSUF_MEMORY );
    } 
     
    _mem_set_type(tmp_ptr,MEM_TYPE_PART_MGR_SECTOR);

    // Don't save allocated sector buffer. Most of the time, we will never use it again.
    // We only need it if we want to re-write the partition info, and can allocate it
    // again if required.
  
    part_table_disk = tmp_ptr + PMGR_PARTITION_TABLE_OFFSET;

    _lwsem_wait(&(pm_struct_ptr->SEM));
    if ( pm_struct_ptr->BLOCK_MODE )
    {
        size = 1;
    }
    else
    {
        size = sector_size;
    }  


    /*
    ** Just in case there is stale data in the memory block
    ** and the read fails but doesn't report that it did
    */
    part_table_disk[PMGR_PART_TABLE_SIZE -2] = 0;
    part_table_disk[PMGR_PART_TABLE_SIZE -1] = 0;

    /* Read in the partition table entries and the 2 signature bytes */
    fseek(dev_fd, 0, IO_SEEK_SET);
    if ( read(dev_fd, tmp_ptr, size) != size )
    {
        _lwsem_post(&(pm_struct_ptr->SEM));
        _mem_free(tmp_ptr);
        if ( dev_fd->ERROR != IO_OK )
        {
            error = dev_fd->ERROR;
        }
        else
        {
            error = IO_ERROR;
        }  
        return error;
    }  

    /* The last two bytes of the table must have the right signature */
    if ( (part_table_disk[PMGR_PART_TABLE_SIZE -2] != 0x55) 
        || ((uchar) part_table_disk[PMGR_PART_TABLE_SIZE -1] != 0xAA) )
    {
        pm_struct_ptr->STATE = PMGR_INVALID_STATE;
    }
    else
    {
        /* 
        ** The partition manager will not be in a valid state unless there is at 
        ** least one valid partition. This for loop checks for a valid partition,
        ** which must have a length > 0, and a defined type.
        */ 
        for ( i = 0; i < PMGR_MAX_PARTITIONS ; i++ )
        {
            _pmgr_disk_to_host((PMGR_PART_ENTRY_STRUCT_PTR) part_table_disk + i, &entry_info);
            /* Check to make sure the entry is valid */
            if ( (entry_info.TYPE == PMGR_PARTITION_FAT_12_BIT ||
                entry_info.TYPE == PMGR_PARTITION_FAT_16_BIT ||
                entry_info.TYPE == PMGR_PARTITION_HUGE ||   
                entry_info.TYPE == PMGR_PARTITION_HUGE_LBA ||
                entry_info.TYPE == PMGR_PARTITION_FAT32 ||
                entry_info.TYPE == PMGR_PARTITION_FAT32_LBA) &&
                entry_info.LENGTH > 0 )
            {
                pm_struct_ptr->PART_START_SECTOR[i] = entry_info.START_SECTOR;
                pm_struct_ptr->VALID[i] = PMGR_VALID_STATE;
                pm_struct_ptr->STATE = PMGR_VALID_STATE;            
                pm_struct_ptr->LENGTH[i] = entry_info.LENGTH;            
            }  
        }  
    }  

    pm_struct_ptr->DEV_SECTOR_SIZE = sector_size;
    _mem_free(tmp_ptr);

    _lwsem_post(&(pm_struct_ptr->SEM));   

    return IO_OK;
}  


/*FUNCTION*--------------------------------------------------------------
*
* Function Name  : _io_part_mgr_close
* Returned Value : MQX_OK
* Comments       : Frees the memory allocated to hold the partition info
*                  
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_close
    (

    MQX_FILE_PTR   fd_ptr   /* [IN] the file pointer of the partition manager to close */
    ) 
{   
    PART_MGR_STRUCT_PTR   pm_struct_ptr;

    pm_struct_ptr = (PART_MGR_STRUCT_PTR)fd_ptr->DEV_PTR->DRIVER_INIT_PTR; 

    _lwsem_wait(&(pm_struct_ptr->SEM));

    pm_struct_ptr->STATE = PMGR_INVALID_STATE;

    _lwsem_post(&(pm_struct_ptr->SEM));

    return IO_OK;
}  


/*FUNCTION*--------------------------------------------------------------
*
* Function Name  : _io_part_mgr_read
* Returned Value : The number of bytes read from device
* Comments       : Calls the read function of the next layer
*                  
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_read
    (
    MQX_FILE_PTR   fd_ptr,  /* [IN] the file pointer of the partition manager to read from */
    char_ptr       data_ptr,    /* [IN] the data location to read to */
    int_32         num          /* [IN] the number of bytes to read */
    ) 
{   
    int_32     bytes_read;

    bytes_read = read( ((PART_MGR_STRUCT_PTR) fd_ptr->DEV_PTR->DRIVER_INIT_PTR)->DEV_FILE_PTR, data_ptr, num);
    fd_ptr->ERROR = ((PART_MGR_STRUCT_PTR)fd_ptr->DEV_PTR->DRIVER_INIT_PTR)->DEV_FILE_PTR->ERROR;

    return( bytes_read );
}          


/*FUNCTION*--------------------------------------------------------------
*
* Function Name  : _io_part_mgr_write
* Returned Value : the number of bytes writen to device
* Comments       : calls the write function of the next layer
*                  
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_write
    (
    MQX_FILE_PTR   fd_ptr,      /* [IN] the file pointer of the partition manager to read from */
    char_ptr       data_ptr,    /* [IN] the data location to read from */
    int_32         num          /* [IN] the number of bytes to write */
    ) 
{   
    int_32      bytes_writen;

    bytes_writen = write( ((PART_MGR_STRUCT_PTR) fd_ptr->DEV_PTR->DRIVER_INIT_PTR)->DEV_FILE_PTR, data_ptr, num);
    fd_ptr->ERROR = ((PART_MGR_STRUCT_PTR)fd_ptr->DEV_PTR->DRIVER_INIT_PTR)->DEV_FILE_PTR->ERROR;

    return( bytes_writen);
}  


/*FUNCTION*--------------------------------------------------------------
*
* Function Name  : _io_part_mgr_ioctl
* Returned Value : int_32 or error code
* Comments       : contains the different ioctl commands
*                  
*END*--------------------------------------------------------------------*/

int_32 _io_part_mgr_ioctl
    (
    MQX_FILE_PTR   fd_ptr,      /* [IN] the stream to perform the operation on */
    uint_32        cmd,         /* [IN] the ioctl command */
    uint_32_ptr    param_ptr    /* [IN] the ioctl parameters */
    ) 
{   
    PART_MGR_STRUCT_PTR           pt_mgr_ptr;
    uint_32                       result = MQX_OK;

    pt_mgr_ptr = (PART_MGR_STRUCT_PTR)fd_ptr->DEV_PTR->DRIVER_INIT_PTR;

    switch ( cmd )
    {
        case IO_IOCTL_SEEK:
            if ( pt_mgr_ptr->BLOCK_MODE )
            {
                pt_mgr_ptr->DEV_FILE_PTR->LOCATION = fd_ptr->LOCATION + 
                    pt_mgr_ptr->PART_START_SECTOR[pt_mgr_ptr->ACTIVE_PART - 1];
            }
            else
            {
                pt_mgr_ptr->DEV_FILE_PTR->LOCATION = fd_ptr->LOCATION + 
                    pt_mgr_ptr->DEV_SECTOR_SIZE * 
                    pt_mgr_ptr->PART_START_SECTOR[pt_mgr_ptr->ACTIVE_PART - 1];
            }  
            break;

        case IO_IOCTL_DEV_LOCK:
            _lwsem_wait(&pt_mgr_ptr->SEM);
            pt_mgr_ptr->ACTIVE_PART = *param_ptr; 
            break;

        case IO_IOCTL_DEV_UNLOCK:
            _lwsem_post(&pt_mgr_ptr->SEM);
            break;

        case IO_IOCTL_GET_BLOCK_SIZE:
            *param_ptr = pt_mgr_ptr->DEV_SECTOR_SIZE;
            break;

        case IO_IOCTL_GET_NUM_SECTORS:
            *param_ptr = pt_mgr_ptr->LENGTH[pt_mgr_ptr->ACTIVE_PART - 1];
            break;

        case IO_IOCTL_VAL_PART:
            if ( pt_mgr_ptr->VALID[*param_ptr -1] == PMGR_VALID_STATE )
            {
                result = MQX_OK;
            }
            else
            {
                result = PMGR_INVALID_PARTITION;
            }  
            break;

        case IO_IOCTL_SET_PARTITION:
            _lwsem_wait(&pt_mgr_ptr->SEM);
            result = _pmgr_set_part_info(pt_mgr_ptr, (PMGR_PART_INFO_STRUCT_PTR) param_ptr, 0);
            _lwsem_post(&pt_mgr_ptr->SEM);         
            break;

        case IO_IOCTL_GET_PARTITION:
            _lwsem_wait(&pt_mgr_ptr->SEM);      
            result = _pmgr_get_part_info(pt_mgr_ptr, (PMGR_PART_INFO_STRUCT_PTR) param_ptr);
            _lwsem_post(&pt_mgr_ptr->SEM);
            break;

        case IO_IOCTL_CLEAR_PARTITION:
            _lwsem_wait(&pt_mgr_ptr->SEM);
            result = _pmgr_set_part_info(pt_mgr_ptr, NULL, *param_ptr);
            _lwsem_post(&pt_mgr_ptr->SEM);
            break;

        default:
            /* Pass IOCTL command to lower layer */
            result = _io_ioctl(pt_mgr_ptr->DEV_FILE_PTR, cmd, param_ptr);
            break;                   
    }  

    fd_ptr->ERROR = pt_mgr_ptr->DEV_FILE_PTR->ERROR;

    return( result );
}  


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  _pmgr_set_part_info
* Returned Value   :  int_32 error code
* Comments  :
*     Will overwrite the partition table entry with the new information
*     received (if opcode == 0). If the opcode is a partition number, this 
*     function will clear that partition. Will return error code upon error.
*
*END*---------------------------------------------------------------------*/

int_32 _pmgr_set_part_info
    (
    PART_MGR_STRUCT_PTR        pm_struct_ptr,   /*[IN] the partitionn manager infor */
    PMGR_PART_INFO_STRUCT_PTR  host_entry,      /*[IN] the entry to write to disk */
    uint_32                    opcode           /*[IN] Opcode: 0 to set a partition, or 1 to 4 to clear that partition */
    )  
{  
    char_ptr                         part_table_disk;
    char_ptr                         tmp_ptr;
    int_32                           error_code = MQX_OK;
    int_32                           size;

    if ( opcode == 0 )
    {
        /* Check to make sure the entry is valid */
        if ( !( (host_entry->TYPE == PMGR_PARTITION_FAT_12_BIT ||
            host_entry->TYPE == PMGR_PARTITION_FAT_16_BIT ||
            host_entry->TYPE == PMGR_PARTITION_HUGE ||   
            host_entry->TYPE == PMGR_PARTITION_HUGE_LBA ||
            host_entry->TYPE == PMGR_PARTITION_FAT32 ||
            host_entry->TYPE == PMGR_PARTITION_FAT32_LBA) && 
            host_entry->LENGTH > 0 && host_entry->SLOT < 5 && host_entry->SLOT > 0) )
        {
            return( PMGR_INVALID_PARTITION );
        }  
    }  

    tmp_ptr =  MFS_mem_alloc_system(pm_struct_ptr->DEV_SECTOR_SIZE);
    if ( tmp_ptr == NULL )
    {
        return MQX_OUT_OF_MEMORY;
    }

    _mem_set_type(tmp_ptr,MEM_TYPE_PART_MGR_SECTOR);

    if ( pm_struct_ptr->BLOCK_MODE )
    {
        size = 1;
    }
    else
    {
        size = pm_struct_ptr->DEV_SECTOR_SIZE;
    }  

    part_table_disk = tmp_ptr + PMGR_PARTITION_TABLE_OFFSET;

    fseek(pm_struct_ptr->DEV_FILE_PTR, 0, IO_SEEK_SET);
    if ( read(pm_struct_ptr->DEV_FILE_PTR, tmp_ptr, size) == size )
    {
        /* If setting a partition, extract the settings */
        if ( opcode == 0 )
        {
            _pmgr_host_to_disk(host_entry, (PMGR_PART_ENTRY_STRUCT_PTR) part_table_disk + host_entry->SLOT - 1);
        }
        else
        {
            /* If we are clearing a partition, write all zeros to it */
            _mem_zero((PMGR_PART_ENTRY_STRUCT_PTR) part_table_disk + opcode - 1, sizeof(PMGR_PART_ENTRY_STRUCT));
        }  
        part_table_disk[PMGR_PART_TABLE_SIZE - 2] = 0x55;
        part_table_disk[PMGR_PART_TABLE_SIZE - 1] = 0xAA;
        fseek(pm_struct_ptr->DEV_FILE_PTR, 0, IO_SEEK_SET);
        if ( write(pm_struct_ptr->DEV_FILE_PTR, tmp_ptr, size) != size )
        {
            error_code = pm_struct_ptr->DEV_FILE_PTR->ERROR;   
        }
        else if ( opcode == 0 )
        {
            pm_struct_ptr->STATE = PMGR_VALID_STATE;
            pm_struct_ptr->PART_START_SECTOR[host_entry->SLOT - 1] = host_entry->START_SECTOR;
            pm_struct_ptr->VALID[host_entry->SLOT-1] = PMGR_VALID_STATE;
        }  
    }
    else
    {
        error_code = pm_struct_ptr->DEV_FILE_PTR->ERROR;
    }  

    _mem_free(tmp_ptr);

    return( error_code );
}    


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  _pmgr_get_part_info
* Returned Value   :  int_32 error code
* Comments  :
*     Will read the partition table entry from disk  
*
*END*---------------------------------------------------------------------*/

int_32 _pmgr_get_part_info 
    (
    PART_MGR_STRUCT_PTR        pm_struct_ptr,   /*[IN] the partitionn manager information */
    PMGR_PART_INFO_STRUCT_PTR  host_entry       /*[IN]/[OUT] the entry to read from disk */
    )   
{   
    int_32                       error_code = MQX_OK;
    char_ptr                     part_table_disk;
    char_ptr                     tmp_ptr;
    int_32                       size;

    if ( pm_struct_ptr->BLOCK_MODE )
    {
        size = 1;
    }
    else
    {
        size = pm_struct_ptr->DEV_SECTOR_SIZE;
    }  

    tmp_ptr =  MFS_mem_alloc_system(pm_struct_ptr->DEV_SECTOR_SIZE);
    if ( tmp_ptr == NULL )
    {
        return MQX_OUT_OF_MEMORY;
    }

    _mem_set_type(tmp_ptr,MEM_TYPE_PART_MGR_SECTOR);

    part_table_disk = tmp_ptr + PMGR_PARTITION_TABLE_OFFSET;

    fseek(pm_struct_ptr->DEV_FILE_PTR, 0, IO_SEEK_SET);
    if ( read(pm_struct_ptr->DEV_FILE_PTR, tmp_ptr, size) == size )
    {
        _pmgr_disk_to_host((PMGR_PART_ENTRY_STRUCT_PTR) part_table_disk + host_entry->SLOT - 1, host_entry);
    }
    else
    {
        error_code = pm_struct_ptr->DEV_FILE_PTR->ERROR;
    }  

    _mem_free(tmp_ptr); 
    return( error_code );
}    


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  _pmgr_disk_to_host
* Returned Value   :  void
* Comments  :
*     Will copy a partition table entry from its disk format to a host format
*
*END*---------------------------------------------------------------------*/


void _pmgr_disk_to_host
    (
    PMGR_PART_ENTRY_STRUCT_PTR   disk_entry,      /*[IN] ptr to a partition table entry in disk format */
    PMGR_PART_INFO_STRUCT_PTR    part_entry /*[OUT] ptr to a partition table entry in a more usable format */
    )   
{  
    part_entry->HEADS = 0;
    part_entry->SECTORS = 0;
    part_entry->CYLINDERS = 0;
    part_entry->TYPE = disk_entry->TYPE;
    part_entry->START_SECTOR = pmgr_dtohl(disk_entry->START_SECTOR);
    part_entry->LENGTH = pmgr_dtohl(disk_entry->LENGTH);   
}    


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  _pmgr_host_to_disk
* Returned Value   :  void
* Comments  :
*     Will copy a partition table entry from its host format to the disk format
*
*END*---------------------------------------------------------------------*/

void _pmgr_host_to_disk
    (
    PMGR_PART_INFO_STRUCT_PTR              part_entry,  /*[IN] ptr to a partition table entry in a usable format */ 
    PMGR_PART_ENTRY_STRUCT_PTR             disk_entry   /*[OUT] ptr to a partition table entry in disk format */
    )   
{  
    uint_32     temp,cyl,head,sec,hds_cyl,sct_trk,lba;

    disk_entry->ACTIVE_FLAG = 0;
    disk_entry->TYPE = part_entry->TYPE;
    pmgr_htodl(disk_entry->START_SECTOR, part_entry->START_SECTOR);
    pmgr_htodl(disk_entry->LENGTH, part_entry->LENGTH);   

    /* Check if CHS is present */
    if ( !part_entry->CYLINDERS || !part_entry->HEADS || !part_entry->SECTORS )
    {
        disk_entry->START_HEAD = 0;
        disk_entry->START_CYLINDER[0] = 0;
        disk_entry->START_CYLINDER[1] = 0;
        disk_entry->ENDING_HEAD = 0;
        disk_entry->ENDING_CYLINDER[0] = 0;
        disk_entry->ENDING_CYLINDER[1] = 0;    
    }
    else
    {
        /* setup info used for calculations */
        //tot_cyl = part_entry->CYLINDERS;
        sct_trk = part_entry->SECTORS;
        hds_cyl = part_entry->HEADS;

        /* Calculate starting CHS */
        lba = part_entry->START_SECTOR;
        cyl = lba / (hds_cyl * sct_trk);
        temp = lba % (hds_cyl * sct_trk);
        head = temp / sct_trk;
        sec = temp % sct_trk + 1;

        disk_entry->START_HEAD = (uchar) head;  
        disk_entry->START_CYLINDER[0] = (uchar) ((sec & 0x3f) + ((cyl & 0x0300) >> 2)); // CR TBD
        disk_entry->START_CYLINDER[1] = (uchar) (cyl & 0x0FF);

        /* Calculate the ending CHS */
        lba = part_entry->LENGTH + part_entry->START_SECTOR;
        cyl = lba / (hds_cyl * sct_trk);
        temp = lba % (hds_cyl * sct_trk);
        head = temp / sct_trk;
        sec = temp % sct_trk + 1;
        disk_entry->ENDING_HEAD = (uchar) head;
        disk_entry->ENDING_CYLINDER[0] = (uchar) ((sec & 0x3f) + ((cyl & 0x0300) >> 2)); // CR TBD
        disk_entry->ENDING_CYLINDER[1] = (uchar) (cyl & 0x0FF);
    }  
}
