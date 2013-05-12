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
* $FileName: demo.c$
* $Version : 3.8.19.1$
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
#include <enet.h>
#include <ipcfg.h>
#include "lw_telnet.h"
#include "config.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/*
** MQX initialization information
*/
void Main_task (uint_32);


const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {1,             Main_task,  2500,     9,        "Bridge",   MQX_AUTO_START_TASK,    0,      0           },
    {0}
};

static void user_fn(void)
{
   int_32 c;
   uint_32 flags=0;
   
   MQX_FILE_PTR serial_fd =  fopen(BSP_DEFAULT_IO_CHANNEL, 0);
   
   ioctl(serial_fd, IO_IOCTL_SERIAL_SET_FLAGS, &flags);
   
   printf("\nEthernet <-> Serial Bridge\n");
   fprintf(serial_fd,"\r\nSerial <-> Ethernet Bridge\r\n");

   while (TRUE) {
      if (fstatus( serial_fd )) {
         c = fgetc( serial_fd );
         if (c==IO_ERROR)
            break;
         //secure correct mapping of EOL between application and telnet
         if (c == '\r') {
             putchar ('\n');
         } else {
             putchar((char)c);
         }
      }

      if (status()) {
         c = getchar();
         if (c==IO_ERROR)
            break;
         //secure correct mapping of EOL between telnet and application
         if (c == '\n') {
             fputc('\r', serial_fd);
         }
         fputc((char)c,serial_fd);
      }
   }
   fclose(serial_fd);
}




void Main_task( uint_32 temp)
{
   IPCFG_IP_ADDRESS_DATA    ip_data;
   uint_32                  error;
   _enet_address            enet_address;

   _RTCSPCB_init          = 3;
   _RTCS_msgpool_init     = 3;
   _RTCS_socket_part_init = 3;
    

   error = RTCS_create();
   if (error== RTCS_OK) {

        ip_data.ip = ENET_IPADDR;
        ip_data.mask = ENET_IPMASK;
        ip_data.gateway = ENET_IPGATEWAY;
        
        ENET_get_mac_address (BSP_DEFAULT_ENET_DEVICE, ENET_IPADDR, enet_address);
        error = ipcfg_init_device (BSP_DEFAULT_ENET_DEVICE, enet_address);
        error = ipcfg_bind_staticip (BSP_DEFAULT_ENET_DEVICE, &ip_data);
    
   }
   if (error== 0) {
      // Direct call to lw Telnet Server so that we become the Telnet Server
      while (1) 
         lw_telnet_server(user_fn);
   }
}

