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
* $FileName: boot.c$
* $Version : 3.8.8.3$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example using RTCS Library.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <shell.h>
#include <sh_rtcs.h>
#include "config.h"
#include "ipcfg.h"
#include "bootloader.h"

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if BOOTLOADER_NAND_FLASH_SUPPORT
   #if ! BSPCFG_ENABLE_NANDFLASH
   #error This application requires BSPCFG_ENABLE_NANDFLASH defined non-zero in user_config.h. Please recompile BSP with this option.
   #endif
#endif

#if BOOTLOADER_NOR_FLASH_SUPPORT 
   #if BSP_M5329EVB || BSP_M53015EVB
      #if ! BSPCFG_ENABLE_EXT_FLASH
      #error This application requires BSPCFG_ENABLE_EXT_FLASHX defined non-zero in user_config.h. Please recompile BSP with this option.
      #endif
   #endif
#endif

#define MAIN_TASK 1

extern const SHELL_COMMAND_STRUCT Shell_commands[];
extern void Main_task (uint_32);
void Auto_boot_startup(MQX_FILE_PTR);
extern bootloader_boot(void);
extern void _bootloader_exit(void);

/* Global variables */
uint_32          _boot_addr;

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,   Function,   Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    { MAIN_TASK,    Main_task,  8000,   9,          "Main", MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/*FUNCTION*------------------------------------------------
* 
* Function Name: Auto_boot_startup
* Comments     :
*    
*
*END*-----------------------------------------------------*/
void Auto_boot_startup(MQX_FILE_PTR flash_hdl) 
{
   uint_32  timeout;
   boolean autoboot;
   uint_32 image_index = 0;
   
   if (BOOTLOADER_OK != _bootloader_get_autoboot(flash_hdl,&timeout,&image_index)){
      printf("\nError can't get kernel image!");
      printf("\nPlease setup your network to take image from TFTP server");
      printf("\n");
      return;
   }
   if (image_index) {
      printf("\nBooting from image %d...",image_index);
      printf("\nHit any key to stop autoboot:");
      
      while(1) {
         printf("%2d",timeout);
         if(status()) {
            autoboot = FALSE;
            fflush(stdin);
            break;
         }
         _time_delay(1000);
         
         autoboot = TRUE;
         timeout--;
         if(0 == timeout)
            break;
      }
      
      if(autoboot) {
        if(MQX_OK == _bootloader_load(flash_hdl, image_index)){
            printf("\n");
            /* Exit bootloader app and execute the new image */
            _mqx_exit(0);
        }
        else {
           printf("\nError");
        }      
      }
   }
   return;
}

/*TASK*-----------------------------------------------------------------
*
* Task Name      : Main_task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

void Main_task(uint_32 temp)
{
    uint_32           error;
    MQX_FILE_PTR      flash_hdl;
    
    /* Sets the address of the MQX exit handler, which MQX calls when it exits */
    _mqx_set_exit_handler(_bootloader_exit);
    
    error = RTCS_create();
    if (error != RTCS_OK) 
    {
        printf("\nRTCS failed to initialize, error = %X", error);
        _task_block();
    }

    /* Enable IP forwarding */
    _IP_forward = TRUE;

#ifdef ENET_DEVICE
    IPCFG_default_enet_device = ENET_DEVICE;
    IPCFG_default_ip_address = ENET_IPADDR;
    IPCFG_default_ip_mask = ENET_IPMASK;
    IPCFG_default_ip_gateway = 0;
    ENET_get_mac_address (IPCFG_default_enet_device, IPCFG_default_ip_address, IPCFG_default_enet_address);
#endif

   printf("\nMQX version %s",_mqx_version);
   printf("\nBoot loader application (build: %s)", __DATE__);
   printf("\n");
 
   /* Open the flash device */
   flash_hdl = fopen(FLASH_NAME_INIT, NULL);
   if (flash_hdl == NULL) {
        printf("\nUnable to open flash device");
        _task_block();
   }

   /* Initialize Image Table if not exist image */
   if (BOOTLOADER_OK == _bootloader_init_table(flash_hdl))
   {
      Auto_boot_startup(flash_hdl); 
   }
   
   fclose(flash_hdl);
   printf("\n");
   /* Run the shell */
   Shell(Shell_commands, NULL);
   for(;;) 
   {
       printf("Shell exited, restarting...\n");
       Shell(Shell_commands, NULL);
   }
}

/* EOF */
