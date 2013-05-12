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
* $FileName: mfs_rw.c$
* $Version : 3.8.16.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the functions that are used to read and write to the 
*   device under MFS. 
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <fio.h>


#include "mfs.h"
#include "mfs_prv.h"
#include "part_mgr.h"


#if !MFSCFG_READ_ONLY

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  MFS_Write_device_sector
* Returned Value   :  error_code
* Comments  :
*     Reads or writes consecutive clusters.
*END*---------------------------------------------------------------------*/
_mfs_error MFS_Write_device_sector
    (
    MFS_DRIVE_STRUCT_PTR    drive_ptr,
    uint_32                 sector_number,  /*[IN] sector number to read/write from/to file system medium */
    char_ptr                buffer_ptr      /*[IN/OUT] address of where data is to be stored/written */
    )
{
#if MFSCFG_MAX_CLUSTER_REMAP_ATTEMPTS
    uint_32    new_cluster_number, next_cluster, bad_cluster_number;
    uint_32    i;
    uint_32    retries;
    uint_32    new_sector_number, no_of_sectors;
    char_ptr   temp_buffer_ptr;
    int_32     num, tmp;
#endif
	int_32     expect_num, seek_loc, shifter;
    _mfs_error error;

#if MFSCFG_READ_ONLY_CHECK
    if (MFS_is_read_only (NULL, drive_ptr))
    {
        return MFS_DISK_IS_WRITE_PROTECTED;
    }
#endif

    error = MFS_NO_ERROR;

    MFS_LOG(printf("MFS_Write_device_sector %d\n", sector_number));

    if ( sector_number > drive_ptr->BPB.MEGA_SECTORS )
    {
        return(MFS_SECTOR_NOT_FOUND);
    }

    /* Lock device */
    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_LOCK, &drive_ptr->DRV_NUM);

    if ( drive_ptr->BLOCK_MODE )
    {
        shifter    = 0;
        seek_loc   = sector_number;
        expect_num = 1;
    }
    else
    {
        shifter    = drive_ptr->SECTOR_POWER;
        seek_loc   = sector_number << shifter;
        expect_num = 1 << shifter;
    }  

    MFS_device_write_internal(drive_ptr, seek_loc, buffer_ptr, &expect_num,  &buffer_ptr, shifter);

    if ( expect_num > 0 )
    {
        error = drive_ptr->DEV_FILE_PTR->ERROR;
#if MFSCFG_MAX_CLUSTER_REMAP_ATTEMPTS
        /*
        ** Check to see if the write failed due to a bad sector. If so, 
        ** rewrite cluster by cluster until we find bad cluster, then move 
        ** it.
        */
        temp_buffer_ptr = buffer_ptr;
        if ( drive_ptr->BLOCK_MODE )
        {
            expect_num = drive_ptr->BPB.SECTORS_PER_CLUSTER;
        }
        else
        {
            expect_num = drive_ptr->CLUSTER_SIZE_BYTES;
        }  
        tmp = expect_num;

        for ( i = 0; i < no_of_clusters; i++ )
        {
            MFS_device_write_internal(drive_ptr, seek_loc, temp_buffer_ptr, &expect_num, &buffer_ptr, shifter);

            if ( expect_num > 0 )
            {
                error_code = drive_ptr->DEV_FILE_PTR->ERROR;
                ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_UNLOCK, &drive_ptr->DRV_NUM);
                bad_cluster_number = cluster_number + i;
                error_code = MFS_get_cluster_from_fat(drive_ptr, bad_cluster_number, &next_cluster);
                if ( error_code != MFS_NO_ERROR )
                {
                    /* 
                    ** This is done so that the unlock at end of function
                    ** works properly
                    */
                    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_LOCK, &drive_ptr->DRV_NUM);
                    break;
                }

                retries = MFSCFG_MAX_CLUSTER_REMAP_ATTEMPTS;

                while ( (error_code != MFS_NO_ERROR) && retries-- )
                {
                    /*
                    ** Mark old cluster BAD 
                    */
                    error_code = MFS_Put_fat(drive_ptr, bad_cluster_number, CLUSTER_BAD);
                    if ( error_code )
                    {
                        break;
                    }

                    /*
                    ** Find available fat, update old FAT as invalid, 
                    ** update new fat, and write new cluster.
                    */
                    new_cluster_number = MFS_Find_unused_cluster_from(drive_ptr, bad_cluster_number);
                    if ( new_cluster_number ==  CLUSTER_INVALID )
                    {
                        /*
                        ** No more clusters
                        */
                        return MFS_DISK_FULL;
                    }

                    new_sector_number = drive_ptr->DATA_START_SECTOR + ((uint_32)(new_cluster_number - CLUSTER_MIN_GOOD)) * drive_ptr->BPB.SECTORS_PER_CLUSTER;

                    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_LOCK, &drive_ptr->DRV_NUM);

                    expect_num = tmp;

                    MFS_device_write_internal(drive_ptr, new_sector_number << shifter, temp_buffer_ptr, &expect_num, &buffer_ptr, shifter);

                    if ( expect_num > 0 )
                    {
                        error_code = drive_ptr->DEV_FILE_PTR->ERROR;
                    }

                    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_UNLOCK, &(drive_ptr->DRV_NUM));
                    bad_cluster_number = new_cluster_number;
                }  

                if ( error_code != MFS_NO_ERROR )
                {
                    return MFS_WRITE_FAULT;
                }

                /*
                ** update chain, 
                */
                error_code = MFS_Put_fat(drive_ptr, new_cluster_number, next_cluster);
                if ( error_code )
                {
                    return(error_code);
                }

                if ( handle->PREVIOUS_CLUSTER )
                {
                    error_code = MFS_Put_fat(drive_ptr,handle->PREVIOUS_CLUSTER, new_cluster_number );
                    if ( error_code )
                    {
                        return(error_code);
                    }
                }
                else
                {
                    clustod(handle->DIR_ENTRY.HFIRST_CLUSTER, handle->DIR_ENTRY.LFIRST_CLUSTER, new_cluster_number);
                }  
                handle->PREVIOUS_CLUSTER = new_cluster_number;
                ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_LOCK, &drive_ptr->DRV_NUM);
            }
            else
            {
                handle->PREVIOUS_CLUSTER = cluster_number+i;
            }  
            temp_buffer_ptr += drive_ptr->CLUSTER_SIZE_BYTES;
            sector_number   += drive_ptr->BPB.SECTORS_PER_CLUSTER;
        }  
#endif
    }

    /* Unlock device under MFS */
    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_UNLOCK, &drive_ptr->DRV_NUM);

    switch ( error )
    {
        case IO_ERROR_WRITE_PROTECTED:
            error = MFS_DISK_IS_WRITE_PROTECTED;
            break;

        case IO_ERROR_WRITE:
            error = MFS_WRITE_FAULT;
            break;

        case IO_ERROR_WRITE_ACCESS:
            error = MFS_SECTOR_NOT_FOUND;
            break;

        case IO_ERROR_READ:
            error = MFS_READ_FAULT;
            break;

        case IO_ERROR_READ_ACCESS:
            error = MFS_SECTOR_NOT_FOUND;
            break;

        default:
            break;
    }  

    return(error);
}  

#endif



/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  MFS_Read_device_sector
* Returned Value   :  error_code
* Comments  :
*     Readsone sector into the DATA_SECTOR buffer
*END*---------------------------------------------------------------------*/
_mfs_error MFS_Read_device_sector
    (
    MFS_DRIVE_STRUCT_PTR    drive_ptr,
    uint_32                 sector_number,  /*[IN] sector number to read/write from/to file system medium */
    char_ptr               sector_ptr
    )
{
    uint_32    attempts;
    int_32     num, expect_num, shifter, seek_loc;
    _mfs_error error;

    error = MFS_NO_ERROR;

    if ( sector_number > drive_ptr->BPB.MEGA_SECTORS )
    {
        return(MFS_SECTOR_NOT_FOUND);
    }

    MFS_LOG(printf("MFS_Read_device_sector %d", sector_number));

    attempts = 0;
    /* Lock device */
    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_LOCK, &drive_ptr->DRV_NUM);

    if ( drive_ptr->BLOCK_MODE )
    {
        shifter    = 0;
        seek_loc   = sector_number;
        expect_num = 1;
    }
    else
    {
        shifter    = drive_ptr->SECTOR_POWER;
        seek_loc   = sector_number << shifter;
        expect_num = 1 << shifter;
    }  

    fseek(drive_ptr->DEV_FILE_PTR, seek_loc, IO_SEEK_SET);

    while ( expect_num > 0 && attempts++ < MFSCFG_MAX_READ_RETRIES )
    {
        num = read(drive_ptr->DEV_FILE_PTR, sector_ptr, expect_num);
        if ( num == IO_ERROR )
        {
            break;
        }
        expect_num -= num;
        sector_ptr += num << shifter;
    }  

    if ( expect_num > 0 )
    {
        error = drive_ptr->DEV_FILE_PTR->ERROR;
    }

    /* Unlock device under MFS */
    ioctl(drive_ptr->DEV_FILE_PTR, IO_IOCTL_DEV_UNLOCK, &drive_ptr->DRV_NUM);

    if ( error == IO_ERROR_READ )
    {
        error = MFS_READ_FAULT;
    }
    else if ( error == IO_ERROR_READ_ACCESS )
    {
        error = MFS_SECTOR_NOT_FOUND;
    }

    return error;
}  


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  MFS_Invalidate_directory_sector
* Returned Value   :  error_code
* Comments  :
*     Invalidates the cached sector.
*END*---------------------------------------------------------------------*/

_mfs_error MFS_Invalidate_directory_sector
    (
    MFS_DRIVE_STRUCT_PTR    drive_ptr   /*[IN] the drive on which to operate */
    )
{
#if MFSCFG_READ_ONLY
    return MFS_NO_ERROR;
#else

#if MFSCFG_READ_ONLY_CHECK
    if (MFS_is_read_only (NULL, drive_ptr))
    {
        return MFS_NO_ERROR;
    }
#endif

    drive_ptr->DIR_SECTOR_NUMBER = MAX_UINT_32;
    drive_ptr->DIR_SECTOR_DIRTY=FALSE;
    return MFS_NO_ERROR;
#endif
}  


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  MFS_Flush_directory_sector_buffer
* Returned Value   :  MFS error code
* Comments  :
*   Write the sector buffer back to the disk.
*   Assumes the semaphore is already obtained.
*END*---------------------------------------------------------------------*/
_mfs_error MFS_Flush_directory_sector_buffer
    (
    MFS_DRIVE_STRUCT_PTR  drive_ptr
    )
{
#if MFSCFG_READ_ONLY
    return MFS_NO_ERROR;
#else
    _mfs_error   error_code= MFS_NO_ERROR;

#if MFSCFG_READ_ONLY_CHECK
    if (MFS_is_read_only (NULL, drive_ptr))
    {
        return error_code;
    }
#endif

    if ( drive_ptr->DIR_SECTOR_DIRTY )
    {
        error_code = MFS_Write_device_sector(drive_ptr,drive_ptr->DIR_SECTOR_NUMBER,drive_ptr->DIR_SECTOR_PTR);
        drive_ptr->DIR_SECTOR_DIRTY = FALSE;
    }

    return(error_code);
#endif
}  


#if 0
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  MFS_Write_back_directory_sector_buffer
* Returned Value   :  MFS error code
* Comments  :
*   Write the sector buffer back to the disk.
*   Assumes the semaphore is already obtained.
*END*---------------------------------------------------------------------*/
_mfs_error MFS_Write_back_directory_sector_buffer
    (
    MFS_DRIVE_STRUCT_PTR  drive_ptr
    )
{
#if MFSCFG_READ_ONLY
    return MFS_NO_ERROR;
#else
    _mfs_error   error_code = MFS_NO_ERROR;

#if MFSCFG_READ_ONLY_CHECK
    if (MFS_is_read_only (NULL, drive_ptr))
    {
        return error_code;
    }
#endif

    MFS_LOG(printf("MFS_Write_back_directory_sector_buffer"));
    drive_ptr->DIR_SECTOR_DIRTY = TRUE;
    if ( drive_ptr->WRITE_CACHE_POLICY==MFS_WRITE_THROUGH_CACHE )
    {
        error_code = MFS_Flush_directory_sector_buffer(drive_ptr);
    }

    return(error_code);
#endif

}  
#endif


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :  MFS_Read_directory_sector
* Returned Value   :  MFS pointer
* Comments  :
*   Reads ONE sector in the sector_buffer.
*   This function cannot read any sectors before the root directory.
*   The semaphore is assumed to be obtained.
*
*END*---------------------------------------------------------------------*/

pointer MFS_Read_directory_sector
    (
    MFS_DRIVE_STRUCT_PTR  drive_ptr,
    uint_32         cluster,        /*[IN] number of the cluster containing the sector to be read */
    uint_16         sector,         /*[IN] index of the sector within the cluster */
    _mfs_error_ptr  error_ptr
    )
{
    uint_32   abs_sector;

    if ( cluster == 0 )
    {
        /*
        ** Reading the root directory
        */
        abs_sector = drive_ptr->ROOT_START_SECTOR+sector;
        /*
        ** Overflow...
        */
        if ( abs_sector >= drive_ptr->DATA_START_SECTOR )
        {
            *error_ptr = MFS_Flush_directory_sector_buffer(drive_ptr);
            drive_ptr->DIR_SECTOR_NUMBER = 0;
            return(NULL);
        }
    }
    else
    {
        abs_sector = CLUSTER_TO_SECTOR(cluster) + sector;
    }  

    if ( abs_sector > drive_ptr->BPB.MEGA_SECTORS )
    {
        *error_ptr = MFS_Flush_directory_sector_buffer(drive_ptr);
        return(NULL);
    }

    if ( abs_sector != drive_ptr->DIR_SECTOR_NUMBER )
    {
        *error_ptr = MFS_Flush_directory_sector_buffer(drive_ptr);
        if ( *error_ptr )
        {
            return(NULL);
        }
        *error_ptr = MFS_Read_device_sector(drive_ptr, abs_sector, drive_ptr->DIR_SECTOR_PTR);
        if ( *error_ptr )
        {
            MFS_Invalidate_directory_sector(drive_ptr);
            return( NULL );
        }
        drive_ptr->DIR_SECTOR_NUMBER = abs_sector;
    }
    else
    {
        *error_ptr = MFS_NO_ERROR;
    }  

    return(drive_ptr->DIR_SECTOR_PTR);
}  


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : MFS_Flush_caches
* Returned Value   : error code
* Comments  :
*     Turns the write cache on or off.
*
*END*---------------------------------------------------------------------*/

_mfs_error MFS_Flush_caches
    (
    MFS_DRIVE_STRUCT_PTR  drive_ptr
    )
{
#if MFSCFG_READ_ONLY
    return MFS_NO_ERROR;
#else
    _mfs_error        error_code, return_code = MFS_NO_ERROR;

#if MFSCFG_READ_ONLY_CHECK
    if (MFS_is_read_only (NULL, drive_ptr))
    {
        return return_code;
    }
#endif

    return_code = MFS_Flush_directory_sector_buffer(drive_ptr);
    error_code = MFS_Flush_data_sector_buffer(drive_ptr);
    if ( return_code == MFS_NO_ERROR ) return_code = error_code;
    error_code = MFS_Flush_fat_cache(drive_ptr);
    if ( return_code == MFS_NO_ERROR ) return_code = error_code;

    return(return_code);
#endif   
}  

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : MFS_device_write_internal
* Returned Value   : int_32. Number of chars written or IO_ERROR
* Comments  :
*     Writes a block to disk.
*
*END*---------------------------------------------------------------------*/

int_32   MFS_device_write_internal
    (
    MFS_DRIVE_STRUCT_PTR       drive_ptr,       /*[IN] the drive on which to operate */
    uint_32                    location,        /*[IN] Where to write to */
    char_ptr                   temp_buffer_ptr, /*[IN] the buffer containing the data */
    int_32_ptr                 expected_num_ptr,/*[IN] pointer to the expect num of bytes/sectors to be written */
    char_ptr _PTR_             temp_buf_ptr,    /*[IN] pointer to a temporary buffer pointer to be incremented */
    int_32                     shifter          /*[IN] indicates if bytes or sectors are being written */
    )
{
    uint_32        retries;
    int_32         num_read;
    int_32         num = 0;

    fseek(drive_ptr->DEV_FILE_PTR, location, IO_SEEK_SET);

    retries = 0;
    while ( *expected_num_ptr > 0 && retries++ < MFSCFG_MAX_WRITE_RETRIES )
    {
        num = write(drive_ptr->DEV_FILE_PTR, temp_buffer_ptr, *expected_num_ptr);
        if ( num == IO_ERROR )
        {
            break;
        }
        if (drive_ptr->READBACK_SECTOR_PTR) 
        {
           fseek(drive_ptr->DEV_FILE_PTR, location, IO_SEEK_SET);

           num_read = read(drive_ptr->DEV_FILE_PTR, drive_ptr->READBACK_SECTOR_PTR, num);
           if (num==num_read) 
           {
               if (memcmp(temp_buffer_ptr,drive_ptr->READBACK_SECTOR_PTR,num)!=0) 
               {
                  num=IO_ERROR;
                  break;
               }
           }
        }
        
        
        *expected_num_ptr -= num;
        *temp_buf_ptr += num << shifter;
    }  
    return( num );
}
