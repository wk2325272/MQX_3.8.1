/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: USB_File.c$
* $Version : 3.7.5.0$
* $Date    : Jan-17-2011$
*
* Comments:
*
*   This file contains the initialization and menu source code for
*   the USB mass storage MFS test example program using USB mass storage
*   link driver.
*
*END************************************************************************/

#include "hvac.h"

#include "usb_file.h"


#if DEMOCFG_ENABLE_USB_FILESYSTEM

pointer usb_filesystem_install( 
   pointer     usb_handle,
   char_ptr    block_device_name,
   char_ptr    partition_manager_name,
   char_ptr    file_system_name )
{
   uint_32                    partition_number;
   uchar_ptr                  dev_info;
   int_32                     error_code;
   uint_32                    mfs_status;
   USB_FILESYSTEM_STRUCT_PTR  usb_fs_ptr;


   usb_fs_ptr = _mem_alloc_system_zero(sizeof(USB_FILESYSTEM_STRUCT));
   if (usb_fs_ptr==NULL) {
      return NULL;
   }


   _io_usb_mfs_install(block_device_name, 0, (pointer)usb_handle);
   usb_fs_ptr->DEV_NAME = block_device_name;


   /* Open the USB mass storage  device */
   _time_delay(500);
   usb_fs_ptr->DEV_FD_PTR = fopen(block_device_name, (char_ptr) 0);

   if (usb_fs_ptr->DEV_FD_PTR == NULL) {
      printf("\nUnable to open USB disk");
      usb_filesystem_uninstall(usb_fs_ptr);
      return NULL;
   } 
   _io_ioctl(usb_fs_ptr->DEV_FD_PTR, IO_IOCTL_SET_BLOCK_MODE, NULL);

   /* get the vendor information and display it */
   printf("\n************************************************************************");
   _io_ioctl(usb_fs_ptr->DEV_FD_PTR, IO_IOCTL_GET_VENDOR_INFO, &dev_info);
   printf("\nVendor Information:     %-1.8s Mass Storage Device",dev_info);
   _io_ioctl(usb_fs_ptr->DEV_FD_PTR, IO_IOCTL_GET_PRODUCT_ID, &dev_info);
   printf("\nProduct Identification: %-1.16s",dev_info);
   _io_ioctl(usb_fs_ptr->DEV_FD_PTR, IO_IOCTL_GET_PRODUCT_REV, &dev_info);
   printf("\nProduct Revision Level: %-1.4s",dev_info);
   printf("\n************************************************************************");

   /* Try Installing a the partition manager */
   error_code = _io_part_mgr_install(usb_fs_ptr->DEV_FD_PTR, partition_manager_name, 0);
   if (error_code != MFS_NO_ERROR) {
      printf("\nError while initializing (%s)", MFS_Error_text((uint_32)error_code));
      usb_filesystem_uninstall(usb_fs_ptr);
      return NULL;
   } 
   usb_fs_ptr->PM_NAME = partition_manager_name;

   usb_fs_ptr->PM_FD_PTR = fopen(partition_manager_name, NULL);
   if (usb_fs_ptr->PM_FD_PTR == NULL) {
      error_code = ferror(usb_fs_ptr->PM_FD_PTR);
      printf("\nError while opening partition (%s)", MFS_Error_text((uint_32)error_code));
      usb_filesystem_uninstall(usb_fs_ptr);
      return NULL;
   } 

   printf("\n--->USB Mass storage device opened");

   partition_number = 1;
   error_code = _io_ioctl(usb_fs_ptr->PM_FD_PTR, IO_IOCTL_VAL_PART, &partition_number);
   if (error_code == PMGR_INVALID_PARTITION) {
      printf("\n--->No partition available on this device");

      /* uninitialize */
      fclose(usb_fs_ptr->PM_FD_PTR);
      usb_fs_ptr->PM_FD_PTR = NULL;

      _io_part_mgr_uninstall(usb_fs_ptr->PM_NAME);
      usb_fs_ptr->PM_NAME = NULL;

      /* install MFS without partition */
      mfs_status = _io_mfs_install(usb_fs_ptr->DEV_FD_PTR, file_system_name, 0);
   } else {
      printf("\n--->Partition Manager installed");
       /* Install MFS on the partition #1 */
      mfs_status = _io_mfs_install(usb_fs_ptr->PM_FD_PTR, file_system_name, partition_number);

   } 

   if (mfs_status != MFS_NO_ERROR) {
      printf("\nError initializing MFS (%s)", MFS_Error_text((uint_32)mfs_status));
      /* uninitialize and exit */
      usb_filesystem_uninstall(usb_fs_ptr);
      return NULL;
   } 
   printf("\n--->File System installed");

   usb_fs_ptr->FS_NAME  = file_system_name;

   usb_fs_ptr->FS_FD_PTR = fopen(file_system_name, 0);
   if (usb_fs_ptr->FS_FD_PTR==NULL) {
      usb_filesystem_uninstall(usb_fs_ptr);
      return NULL;
   } 

   printf("\n--->File System opened");
   return (pointer) usb_fs_ptr;
}

MQX_FILE_PTR usb_filesystem_handle( USB_FILESYSTEM_STRUCT_PTR  usb_fs_ptr)
{
   return usb_fs_ptr->FS_FD_PTR ;
}

void usb_filesystem_uninstall( USB_FILESYSTEM_STRUCT_PTR  usb_fs_ptr)
{
   int_32                     error_code;
   uint_32                    mfs_status;

   if (usb_fs_ptr == NULL) {
      return;
   }
    _int_disable();
   printf("\nDevice removed, starting device cleanup");
   printf("\n--->Closing MFS");
   if (usb_fs_ptr->FS_FD_PTR) {
      error_code = fclose(usb_fs_ptr->FS_FD_PTR);
      if ( error_code != IO_OK ) {
         printf("\nError closing MFS device: 0x%X.", error_code);
      } 
   }

   printf("\n--->Uninstalling %s",usb_fs_ptr->FS_NAME);
   if (usb_fs_ptr->FS_NAME) {
      mfs_status = _io_mfs_uninstall(usb_fs_ptr->FS_NAME);
      if ((mfs_status != MFS_NO_ERROR)  && (mfs_status !=IO_DEVICE_DOES_NOT_EXIST)) {
         printf("\nError while uninstalling MFS on device");
      } 
   }

   if (usb_fs_ptr->PM_FD_PTR != NULL) {
      /* Close partition device */
      printf("\n--->Closing partition");
      error_code = fclose(usb_fs_ptr->PM_FD_PTR);
      if ( error_code != IO_OK ) {
         printf("\nError closing Partition device %s: 0x%X.", usb_fs_ptr->PM_NAME, error_code);
      } 
   }

    if (usb_fs_ptr->PM_NAME) {
     /* Uninstall the partition manager */
      printf("\n--->Uninstalling PM");
      error_code = _io_part_mgr_uninstall(usb_fs_ptr->PM_NAME);
      if (( error_code != IO_OK ) && (mfs_status !=IO_DEVICE_DOES_NOT_EXIST)) {
         printf("\nError uninstalling Partition device %s: 0x%X.", usb_fs_ptr->PM_NAME, error_code);
      } 
   } 

   /* USB mass storage link device */
   if (usb_fs_ptr->DEV_FD_PTR) {
      printf("\n--->Closing disk");
      error_code = fclose(usb_fs_ptr->DEV_FD_PTR);
      if ( error_code != IO_OK ) {
         printf("\nError closing disk (0x%X)", error_code);
      } 
   }
   _int_enable();
   
   if (usb_fs_ptr->DEV_NAME) {
      printf("\n--->Closing USB MFS");
      _io_dev_uninstall(usb_fs_ptr->DEV_NAME);
   }

   _mem_free(usb_fs_ptr);
}

#endif


/* EOF */
