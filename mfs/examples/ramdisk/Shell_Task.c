/**HEADER********************************************************************
* 
* Copyright (c) 2011 Freescale Semiconductor;
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
* $FileName: Shell_Task.c$
* $Version : 3.8.24.0$
* $Date    : Aug-1-2011$
*
* Comments: Provide MFS file system on external RAM
*
*   
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include "mfs.h"
#include <shell.h>

#if ! SHELLCFG_USES_MFS
#error This application requires SHELLCFG_USES_MFS defined non-zero in user_config.h. Please recompile libraries with this option.
#endif

void Shell_Task(uint_32);
void Ram_disk_start(void);

/* The MRam disk base address definition */
#if defined (BSP_EXTERNAL_MRAM_RAM_BASE) && defined (BSP_EXTERNAL_MRAM_RAM_SIZE)
    #warning Please check that the MRAM memory is available on your EVB (Tower). 
    #define RAM_DISK_BASE   BSP_EXTERNAL_MRAM_RAM_BASE 
    #define RAM_DISK_SIZE   BSP_EXTERNAL_MRAM_RAM_SIZE
#else 
    #warning Modify the RAM_DISK_SIZE if you need more or less ramdisk space. 
    #define RAM_DISK_SIZE   0x1800
    static uchar   ram_disk[RAM_DISK_SIZE];
    #define RAM_DISK_BASE   &ram_disk[0]
#endif

const SHELL_COMMAND_STRUCT Shell_commands[] = {   
   { "cd",        Shell_cd },      
   { "copy",      Shell_copy },
   { "create",    Shell_create },
   { "del",       Shell_del },       
   { "disect",    Shell_disect},      
   { "dir",       Shell_dir },      
   { "df",        Shell_df },      
   { "exit",      Shell_exit }, 
   { "format",    Shell_format },
   { "help",      Shell_help }, 
   { "mkdir",     Shell_mkdir },     
   { "pwd",       Shell_pwd },       
   { "read",      Shell_read },      
   { "ren",       Shell_rename },    
   { "rmdir",     Shell_rmdir },
   { "sh",        Shell_sh },
   { "type",      Shell_type },
   { "write",     Shell_write },     
   { "?",         Shell_command_list },     
   
   { NULL,        NULL } 
};

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_Task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/
void Shell_Task(uint_32 temp)
{ 

   /* Run the shell on the serial port */
   printf("Demo start\n\r");
   Ram_disk_start();
   for(;;)  {
      Shell(Shell_commands, NULL);
      printf("Shell exited, restarting...\n");
   }
}
 
/*TASK*-----------------------------------------------------
* 
* Task Name    : Ram_disk_start
* Comments     : Open device and install MFS on device
*    
*
*END*-----------------------------------------------------*/
void Ram_disk_start(void)
{ /* Body */
   MQX_FILE_PTR               dev_handle1,
                              a_fd_ptr;
   int_32                     error_code;
   _mqx_uint                  mqx_status;

   a_fd_ptr = 0;

   /* install device */
   mqx_status = _io_mem_install("mfs_ramdisk:", (uchar_ptr)RAM_DISK_BASE,
      (_file_size)RAM_DISK_SIZE);
   /* Number of sectors is returned by ioctl IO_IOCTL_GET_NUM_SECTORS function */
   /* If another disc structure is desired, use MFS_FORMAT_DATA structure to   */
   /* define it and call standart format function instead default_format       */   
   if ( mqx_status != MQX_OK ) {
      printf("\nError installing memory device (0x%x)", mqx_status);
      _task_block();
   } /* Endif */

   /* Open the device which MFS will be installed on */
   dev_handle1 = fopen("mfs_ramdisk:", 0);
   if ( dev_handle1 == NULL ) {
      printf("\nUnable to open Ramdisk device");
      _task_block();
   } /* Endif */

   /* Install MFS  */
   mqx_status = _io_mfs_install(dev_handle1, "a:", (_file_size)0);
   if (mqx_status != MFS_NO_ERROR) {
      printf("\nError initializing a:");
      _task_block();
   } else {
      printf("\nInitialized Ram Disk to a:\\");
   } /* Endif */

   /* Open the filesystem and format detect, if format is required */
   a_fd_ptr = fopen("a:", NULL);
   error_code    = ferror(a_fd_ptr);
   if ((error_code != MFS_NO_ERROR) && (error_code != MFS_NOT_A_DOS_DISK))
   {
      printf("\nError while opening a:\\ (%s)", MFS_Error_text((uint_32)(uint_32)error_code));
      _task_block();
   } /* Endif */
   if ( error_code == MFS_NOT_A_DOS_DISK ) {
      printf("\nNOT A DOS DISK! You must format to continue.");
   } /* Endif */
    
} /* Endbody */ 

/* EOF */
