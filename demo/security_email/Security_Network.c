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
* $FileName: Security_Network.c$
* $Version : 3.7.16.0$
* $Date    : Jan-17-2011$
*
* Comments:
*
*
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include "enet.h"
#include <ipcfg.h>
#include <string.h>
#include "security_private.h"
#include "security_public.h"
#include "security.h"

void SEC_InitializeNetworking(uint_32 pcbs, uint_32 msgs, uint_32 sockets, boolean dhcp) 
{
  int_32                 error;
  IPCFG_IP_ADDRESS_DATA  ip_data;
  _enet_address          enet_address;

    /* runtime RTCS configuration */
   _RTCSPCB_init = pcbs;
   _RTCS_msgpool_init = msgs;
   _RTCS_socket_part_init = sockets;
    
   error = RTCS_create();
   if (error == RTCS_OK) {

      ip_data.ip = ENET_IPADDR;
      ip_data.mask = ENET_IPMASK;
      ip_data.gateway = ENET_IPGATEWAY;

      ENET_get_mac_address(BSP_DEFAULT_ENET_DEVICE, ENET_IPADDR, enet_address);
      ipcfg_init_device(BSP_DEFAULT_ENET_DEVICE, enet_address);
      ipcfg_add_dns_ip(BSP_DEFAULT_ENET_DEVICE,LWDNS_server_ipaddr);

      // check link status
      printf("\nWaiting for ethernet cable plug in ... ");
      while(!ipcfg_get_link_active(BSP_DEFAULT_ENET_DEVICE)) {};
      printf("Cable connected\n");

       /* If DHCP Enabled, get IP address from DHCP server */
      if (dhcp) {
         printf("\nDHCP bind ... ");
         error = ipcfg_bind_dhcp_wait(BSP_DEFAULT_ENET_DEVICE, 1, &ip_data);
            if (error != IPCFG_ERROR_OK) {
               printf("Error %08x!\n", error);
            }
            else {
               printf("Successful!\n");
            }
      } else {
         /* Else bind with static IP */
         printf ("\nStatic IP bind ... ");
         error = ipcfg_bind_staticip(BSP_DEFAULT_ENET_DEVICE, &ip_data);

         if (error != IPCFG_ERROR_OK) {
            printf("Error %08x!\n",error);
         }
         else {
            printf("Successful!\n");
         }
       }

       if (error == IPCFG_ERROR_OK) {
          ipcfg_get_ip(BSP_DEFAULT_ENET_DEVICE, &ip_data);
          printf("\nIP Address      : %d.%d.%d.%d\n",IPBYTES(ip_data.ip));
          printf("\nSubnet Address  : %d.%d.%d.%d\n",IPBYTES(ip_data.mask));
          printf("\nGateway Address : %d.%d.%d.%d\n",IPBYTES(ip_data.gateway));
          printf("\nDNS Address     : %d.%d.%d.%d\n",IPBYTES(ipcfg_get_dns_ip(BSP_DEFAULT_ENET_DEVICE,0)));
       }
   } else {
       printf("\nRTCS_Create failed !\n");
       _task_block();
   }
}


boolean SEC_GetTime()
{
    boolean res = FALSE;
    
#if DEMOCFG_ENABLE_SNTP
   _ip_address  ipaddr;
   TIME_STRUCT time;
   DATE_STRUCT date;
   char tries = 0;

   /* Try three times to get time */
   while(tries<3)
   {
     _time_delay(1000);
      printf("\nGetting time from time server ... ");

      if (RTCS_resolve_ip_address(SNTP_SERVER,&ipaddr,NULL,0)) {
         /* Contact SNTP server and update time */
         if(SNTP_oneshot(ipaddr,1000)==RTCS_OK)
         {
            printf("Succeeded\n");
            res = TRUE;
            break;
         }
         else
         {
            printf("Failed\n");
         }

      }
      else
      {
         printf("Failed - address not resolved\n");
         break;
      }
    tries++;
   }
   /* Get current time */
   _time_get(&time);
   _time_to_date(&time,&date);
   printf("\nCurrent Time: %02d/%02d/%02d %02d:%02d:%02d\n",
      date.YEAR,date.MONTH,date.DAY,date.HOUR,date.MINUTE,date.SECOND);
#endif

   return res;
}

