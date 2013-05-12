/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: CFCARD_File.c$
* $Version : 3.8.7.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file contains the initialization  for Compact Flash card
*
*END************************************************************************/

#include "Shell_Task.h"
#include "CFCard_File.h"


/*TASK*-----------------------------------------------------------------
*
* Function Name  : CF_filesystem_install
* Returned Value : void
* Comments       : Install filesystem on given pccard file pointer
*
*END------------------------------------------------------------------*/
FS_STRUCT_PTR CF_filesystem_install( 
   MQX_FILE_PTR pccard_fp,
   char_ptr    partition_manager_name,
   char_ptr    file_system_name )
{
   uint_32                    partition_number;
   int_32                     error_code;
   uint_32                    mfs_status;
   FS_STRUCT_PTR  fs_ptr;

                              
   fs_ptr = _mem_alloc_system_zero(sizeof(FS_STRUCT));
   if (fs_ptr==NULL) {
      return NULL;
   }
    
    /* Open flash device */
    fs_ptr->DEV_FD_PTR = fopen("pcflasha:", (char_ptr) pccard_fp);
    if ( fs_ptr->DEV_FD_PTR == NULL ) {
      printf("\nOpen flash device failed\n");
      return NULL;
    } /* Endif */
  
    error_code = ioctl(fs_ptr->DEV_FD_PTR, PCFLASH_IOCTL_SET_BLOCK_MODE, NULL);
    if (error_code == IO_OK) {
//       printf("\nBlock mode enabled.");
    } else {
       printf("\nBlock mode not enabled. Running in byte mode.\n");
    } /* Endif */
  
  
   /* Try Installing a the partition manager */
   error_code = _io_part_mgr_install(fs_ptr->DEV_FD_PTR, partition_manager_name, 0);
   if (error_code != MFS_NO_ERROR) {
      printf("\nError while initializing (%s)", MFS_Error_text((uint_32)error_code));
      CF_filesystem_uninstall(fs_ptr);
      return NULL;
   } 
   fs_ptr->PM_NAME = partition_manager_name;

   fs_ptr->PM_FD_PTR = fopen(partition_manager_name, NULL);
   if (fs_ptr->PM_FD_PTR == NULL) {
      error_code = ferror(fs_ptr->PM_FD_PTR);
      printf("\nError while opening partition (%s)", MFS_Error_text((uint_32)error_code));
      CF_filesystem_uninstall(fs_ptr);
      return NULL;
   } 

   printf("\n--->Compact Flash device opened");

   partition_number = 1;
   error_code = _io_ioctl(fs_ptr->PM_FD_PTR, IO_IOCTL_VAL_PART, &partition_number);
   if (error_code == PMGR_INVALID_PARTITION) {
      printf("\n--->No partition available on this device");

      /* uninitialize */
      fclose(fs_ptr->PM_FD_PTR);
      fs_ptr->PM_FD_PTR = NULL;

      _io_part_mgr_uninstall(fs_ptr->PM_NAME);
      fs_ptr->PM_NAME = NULL;

      /* install MFS without partition */
      mfs_status = _io_mfs_install(fs_ptr->DEV_FD_PTR, file_system_name, 0);
   } else {
      printf("\n--->Partition Manager installed");
       /* Install MFS on the partition #1 */
      mfs_status = _io_mfs_install(fs_ptr->PM_FD_PTR, file_system_name, partition_number);

   } 

   if (mfs_status != MFS_NO_ERROR) {
      printf("\nError initializing MFS (%s)", MFS_Error_text((uint_32)mfs_status));
      /* uninitialize and exit */
      CF_filesystem_uninstall(fs_ptr);
      return NULL;
   } 

   fs_ptr->FS_NAME  = file_system_name;

   fs_ptr->FS_FD_PTR = fopen(file_system_name, 0);
   if (fs_ptr->FS_FD_PTR==NULL) {
      CF_filesystem_uninstall(fs_ptr);
      return NULL;
   } 

   printf("\n--->File System opened as : %s\n",fs_ptr->FS_NAME);
   return (pointer) fs_ptr;
}



/*TASK*-----------------------------------------------------------------
*
* Function Name  : CF_filesystem_uninstall
* Returned Value : void
* Comments       : Uninstall filesystem
*
*END------------------------------------------------------------------*/
void CF_filesystem_uninstall( FS_STRUCT_PTR  fs_ptr)
{
   int_32                     error_code;
   uint_32                    mfs_status;
//   boolean                    partition_found = FALSE;

   if (fs_ptr == NULL) {
      return;
   }
   printf("\nDevice removed, starting device cleanup");
   if (fs_ptr->FS_FD_PTR) {
      printf("\n--->Closing MFS");
      error_code = fclose(fs_ptr->FS_FD_PTR);
      if ( error_code != IO_OK ) {
         printf("\nError closing MFS device: 0x%X.", error_code);
      }
      else
      {
         fs_ptr->FS_FD_PTR = NULL;
      }
   }

   
   if (fs_ptr->FS_NAME) {
      printf("\n--->Uninstalling %s",fs_ptr->FS_NAME);
      mfs_status = _io_mfs_uninstall(fs_ptr->FS_NAME);
      if ((mfs_status != MFS_NO_ERROR)  && (mfs_status !=IO_DEVICE_DOES_NOT_EXIST)) {
         printf("\nError while uninstalling MFS on device");
      }
      fs_ptr->FS_NAME = '\0';
   }

   if (fs_ptr->PM_FD_PTR != NULL) {
      /* Close partition device */
      printf("\n--->Closing partition");
      error_code = fclose(fs_ptr->PM_FD_PTR);
      if ( error_code != IO_OK ) {
         printf("\nError closing Partition device %s: 0x%X.", fs_ptr->PM_NAME, error_code);
      }
      else
      {
         fs_ptr->PM_FD_PTR = NULL;
      }
      
   }

    if (fs_ptr->PM_NAME) {
     /* Uninstall the partition manager */
      printf("\n--->Uninstalling PM");
      error_code = _io_part_mgr_uninstall(fs_ptr->PM_NAME);
      if (( error_code != IO_OK ) && (mfs_status !=IO_DEVICE_DOES_NOT_EXIST)) {
         printf("\nError uninstalling Partition device %s: 0x%X.", fs_ptr->PM_NAME, error_code);
      }
      else
      {
         fs_ptr->PM_NAME = '\0';
      }
   } 

   /* PCFLASH device */
   if (fs_ptr->DEV_FD_PTR) {
      printf("\n--->Closing pcflash\n");
      error_code = fclose(fs_ptr->DEV_FD_PTR);
      if ( error_code != IO_OK ) {
         printf("\nError closing disk (0x%X)\n", error_code);
      }
      else
      {
         fs_ptr->DEV_FD_PTR = NULL; 
      }
   }
   _mem_free(fs_ptr);
}



/* EOF */
