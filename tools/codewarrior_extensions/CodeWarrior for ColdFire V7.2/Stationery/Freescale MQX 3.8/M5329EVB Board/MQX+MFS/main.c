/****************************************************************************
* 
*   This file contains MQX+MFS stationery code.
*
****************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include <mfs.h>
#include <shell.h>
#include <sh_mfs.h>
#include "main.h"


#if !BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if !SHELLCFG_USES_MFS
#error This application requires SHELLCFG_USES_MFS defined non-zero in user_config.h. Please recompile libraries with this option.
#endif


#if !defined(RAM_DISK_SIZE)
#error Please specify RAM_DISK_SIZE
#endif


/* allocate RAM DISK storage memory */
#if defined(RAM_DISK_SIZE) && !defined(RAM_DISK_BASE)
static uchar ram_disk[RAM_DISK_SIZE];
#define RAM_DISK_BASE ram_disk
#endif


const SHELL_COMMAND_STRUCT Shell_commands[] = {   
   /* MFS commands */ 
   { "cd",        Shell_cd },      
   { "copy",      Shell_copy },
   { "create",    Shell_create },
   { "del",       Shell_del },       
   { "disect",    Shell_disect},      
   { "dir",       Shell_dir },      
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


TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   2000,  9,   "main", MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0,                 }
};


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


/*TASK*-----------------------------------------------------------------
*
* Function Name  : Main_task
* Comments       :
*    This task initializes MFS and starts SHELL.
*
*END------------------------------------------------------------------*/

void Main_task(uint_32 initial_data)
{

   /* Install ramdisk */
   Ram_disk_start();

   /*******************************
   * 
   * START YOUR CODING HERE
   *
   ********************************/
      
   for (;;)  
   {
      /* Run the shell */
      Shell(Shell_commands, NULL);
      printf("Shell exited, restarting...\n");
   }
}

/* EOF */
