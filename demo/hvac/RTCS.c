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
* $FileName: RTCS.c$
* $Version : 3.8.19.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example of HVAC using RTCS.
*
*END************************************************************************/

#include "HVAC.h"

#if DEMOCFG_USE_WIFI
#include "string.h"
#endif

#if DEMOCFG_ENABLE_RTCS

#include <ipcfg.h>

#if (DEMOCFG_ENABLE_FTP_SERVER+DEMOCFG_ENABLE_TELNET_SERVER) != 1
#warning Please enable one of the network services. The restriction is only for RAM size limited devices.
#endif

extern const SHELL_COMMAND_STRUCT Telnet_commands[];

static void Telnetd_shell_fn (pointer dummy) 
{  
   Shell(Telnet_commands,NULL);
}

const RTCS_TASK Telnetd_shell_template = {"Telnet_shell", 8, 2000, Telnetd_shell_fn, NULL};


void HVAC_initialize_networking(void) 
{
   int_32                  error;
   IPCFG_IP_ADDRESS_DATA   ip_data;
   _enet_address           enet_address;

#if DEMOCFG_USE_POOLS && defined(DEMOCFG_RTCS_POOL_ADDR) && defined(DEMOCFG_RTCS_POOL_SIZE)
    /* use external RAM for RTCS buffers */
    _RTCS_mem_pool = _mem_create_pool((pointer)DEMOCFG_RTCS_POOL_ADDR, DEMOCFG_RTCS_POOL_SIZE);
#endif

#if RTCS_MINIMUM_FOOTPRINT
   /* runtime RTCS configuration for devices with small RAM, for others the default BSP setting is used */
   _RTCSPCB_init = 4;
   _RTCSPCB_grow = 2;
   _RTCSPCB_max = 20;
   _RTCS_msgpool_init = 4;
   _RTCS_msgpool_grow = 2;
   _RTCS_msgpool_max  = 20;
   _RTCS_socket_part_init = 4;
   _RTCS_socket_part_grow = 2;
   _RTCS_socket_part_max  = 20;
#endif

    error = RTCS_create();

    LWDNS_server_ipaddr = ENET_IPGATEWAY;

    ip_data.ip = ENET_IPADDR;
    ip_data.mask = ENET_IPMASK;
    ip_data.gateway = ENET_IPGATEWAY;
    
    ENET_get_mac_address (DEMOCFG_DEFAULT_DEVICE, ENET_IPADDR, enet_address);
    error = ipcfg_init_device (DEMOCFG_DEFAULT_DEVICE, enet_address);
#if DEMOCFG_USE_WIFI
    iwcfg_set_essid (DEMOCFG_DEFAULT_DEVICE,DEMOCFG_SSID);
    if ((strcmp(DEMOCFG_SECURITY,"wpa") == 0)||strcmp(DEMOCFG_SECURITY,"wpa2") == 0)
    {
        iwcfg_set_passphrase (DEMOCFG_DEFAULT_DEVICE,DEMOCFG_PASSPHRASE);

    }
    if (strcmp(DEMOCFG_SECURITY,"wep") == 0)
    {
      iwcfg_set_wep_key (DEMOCFG_DEFAULT_DEVICE,DEMOCFG_WEP_KEY,strlen(DEMOCFG_WEP_KEY),DEMOCFG_WEP_KEY_INDEX);
    }
    iwcfg_set_sec_type (DEMOCFG_DEFAULT_DEVICE,DEMOCFG_SECURITY);
    iwcfg_set_mode (DEMOCFG_DEFAULT_DEVICE,DEMOCFG_NW_MODE);
#endif  
    error = ipcfg_bind_staticip (DEMOCFG_DEFAULT_DEVICE, &ip_data);

#if DEMOCFG_ENABLE_FTP_SERVER
   FTPd_init("FTP_server", 7, 3000 );
#endif

#if DEMOCFG_ENABLE_TELNET_SERVER
   TELNETSRV_init("Telnet_server", 7, 2000, (RTCS_TASK_PTR) &Telnetd_shell_template );
#endif

#if DEMOCFG_ENABLE_KLOG && MQX_KERNEL_LOGGING
   RTCSLOG_enable(RTCSLOG_TYPE_FNENTRY);
   RTCSLOG_enable(RTCSLOG_TYPE_PCB);
#endif

}

#else /* DEMOCFG_ENABLE_RTCS */

#if (DEMOCFG_ENABLE_FTP_SERVER+DEMOCFG_ENABLE_TELNET_SERVER) > 0
#warning RTCS network stack is disabled, the selected service will not be available.
#endif

#endif /* DEMOCFG_ENABLE_RTCS */

/* EOF */
