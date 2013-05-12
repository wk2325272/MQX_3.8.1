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
*END************************************************************************/
#include "main.h"

/* allocate RAM DISK storage memory */
#if defined(RAM_DISK_SIZE) && !defined(RAM_DISK_BASE)
static uchar ram_disk[RAM_DISK_SIZE];
#define RAM_DISK_BASE ram_disk
#endif

/*FUNCTION*-----------------------------------------------------
* 
* Task Name    : Ram_disk_start
* Comments     :
*    This function installs and initializes ramdisk.
*
*END*-----------------------------------------------------*/

void Ram_disk_start(void)
{
   MQX_FILE_PTR               dev_handle1, a_fd_ptr;
   int_32                     error_code;
   _mqx_uint                  mqx_status;

   a_fd_ptr = 0;

   /* Install device */
   mqx_status = _io_mem_install("mfs_ramdisk:", (uchar_ptr)RAM_DISK_BASE, (_file_size)RAM_DISK_SIZE);
   if ( mqx_status != MQX_OK ) 
   {
      printf("Error installing memory device (0x%x)\n", mqx_status);
      _task_block();
   }

   /* Open the device which MFS will be installed on */
   dev_handle1 = fopen("mfs_ramdisk:", 0);
   if ( dev_handle1 == NULL ) 
   {
      printf("Unable to open Ramdisk device\n");
      _task_block();
   }

   /* Install MFS  */
   mqx_status = _io_mfs_install(dev_handle1, "a:", (_file_size)0);
   if (mqx_status != MFS_NO_ERROR) 
   {
      printf("Error initializing a:\n");
      _task_block();
   } 
   else 
   {
      printf("Ramdisk initialized to a:\\\n");
   }

   /* Open the filesystem and detect, if format is required */
   a_fd_ptr = fopen("a:", NULL);
   error_code = ferror(a_fd_ptr);
   if ((error_code != MFS_NO_ERROR) && (error_code != MFS_NOT_A_DOS_DISK))
   {
      printf("Error while opening a:\\ (%s)\n", MFS_Error_text((uint_32)(uint_32)error_code));
      _task_block();
   }
   if (error_code == MFS_NOT_A_DOS_DISK) 
   {
      printf("NOT A DOS DISK! You must format to continue.\n");
   }
}
