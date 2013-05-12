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
#include <ipcfg.h>

#if RTCS_PPP
#include <ppp.h>

#if ! RTCSCFG_ENABLE_VIRTUAL_ROUTES
#error This application requires RTCSCFG_ENABLE_VIRTUAL_ROUTES defined non-zero in user_config.h. Please recompile libraries (BSP, RTCS) with this option.
#endif

#if ! RTCSCFG_ENABLE_GATEWAYS
#error This application requires RTCSCFG_ENABLE_GATEWAYS defined non-zero in user_config.h. Please recompile libraries (BSP, RTCS) with this option.
#endif

#warning This application requires PPP device to be defined manually and being different from the default IO channel (BSP_DEFAULT_IO_CHANNEL). See PPP_DEVICE in config.h.

LWSEM_STRUCT ppp_sem;
static void PPP_linkup (pointer lwsem) {_lwsem_post(lwsem);}
extern void PPP_start(void);

char localname[] = "psti";
char localsecret[] = "secret";
char remotename[] = "guest";
char remotesecret[] = "anonymous";

PPP_SECRET lsecret =
   {sizeof(localname)-1,  sizeof(localsecret)-1,  localname,  localsecret};

PPP_SECRET lsecrets[] = {
   {sizeof(remotename)-1, sizeof(localsecret)-1,  remotename, localsecret},
   {0, 0, NULL, NULL}};

PPP_SECRET rsecrets[] = {
   {sizeof(remotename)-1, sizeof(remotesecret)-1, remotename, remotesecret},
   {0, 0, NULL, NULL}};

extern PPP_SECRET_PTR _PPP_PAP_LSECRET;
extern PPP_SECRET_PTR _PPP_PAP_RSECRETS;
extern char_ptr       _PPP_CHAP_LNAME;
extern PPP_SECRET_PTR _PPP_CHAP_LSECRETS;
extern PPP_SECRET_PTR _PPP_CHAP_RSECRETS;

/*FUNCTION*------------------------------------------------
* 
* Function Name: PPP_start
* Comments     :
*    
*
*END*-----------------------------------------------------*/
void PPP_start(void)
{
    uint_32           error;

    _rtcs_if_handle   ihandle;
    MQX_FILE_PTR      pfile;
    _iopcb_handle     pio;
    _ppp_handle       phandle;
    IPCP_DATA_STRUCT  ipcp_data;

    _PPP_PAP_RSECRETS = rsecrets;    /* Require authentication, allow PAP */
    _PPP_PAP_LSECRET = &lsecret;     /* Allow peer to request PAP */
    _PPP_CHAP_LNAME = localname;     /* Needed by CHAP */
    _PPP_CHAP_RSECRETS = rsecrets;   /* Require authentication, allow CHAP */
    _PPP_CHAP_LSECRETS = lsecrets;   /* Allow peer to request CHAP */

    /* Install a route for a default gateway */
    RTCS_gate_add(GATE_ADDR, INADDR_ANY, INADDR_ANY);

    pfile = fopen(PPP_DEVICE, NULL);
#ifdef PPP_DEVICE_DUN
    _io_dun_install("dun:");
    pfile = fopen("dun:", (char_ptr)pfile);
#endif
 
    pio = _iopcb_ppphdlc_init(pfile);
    _PPP_ACCM = 0;
    error = PPP_initialize(pio, &phandle);
    if (error) {
       printf("\nPPP initialize: %lx", error);
       _task_block();
    } /* Endif */

    _iopcb_open(pio, PPP_lowerup, PPP_lowerdown, phandle);
    error = RTCS_if_add(phandle, RTCS_IF_PPP, &ihandle);
    if (error) {
       printf("\nIF add failed, error = %lx", error);
       _task_block();
    } /* Endif */
    _lwsem_create(&ppp_sem, 0);
    _mem_zero(&ipcp_data, sizeof(ipcp_data));
    ipcp_data.IP_UP              = PPP_linkup;
    ipcp_data.IP_DOWN            = NULL;
    ipcp_data.IP_PARAM           = &ppp_sem;
    ipcp_data.ACCEPT_LOCAL_ADDR  = FALSE;
    ipcp_data.ACCEPT_REMOTE_ADDR = FALSE;
    ipcp_data.LOCAL_ADDR         = PPP_LOCADDR;
    ipcp_data.REMOTE_ADDR        = PPP_PEERADDR;
    ipcp_data.DEFAULT_NETMASK    = TRUE;
    ipcp_data.DEFAULT_ROUTE      = TRUE;
    error = RTCS_if_bind_IPCP(ihandle, &ipcp_data);
    if (error) {
       printf("\nIF bind failed, error = %lx", error);
       _task_block();
    } /* Endif */
    printf("\nPlease initiate PPP connection.  Waiting...");
    _lwsem_wait(&ppp_sem);
    printf("\nPPP device %s bound to %d.%d.%d.%d", PPP_DEVICE, IPBYTES(ipcp_data.LOCAL_ADDR));
}
#endif /* RTCS_PPP */

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : rtcs_init
* Comments     :
*    This function installs and initializes RTCS
*
*END*-----------------------------------------------------*/

void rtcs_init()
{
   IPCFG_IP_ADDRESS_DATA ip_data;
   _enet_address           enet_address;   

   uint_32 error = RTCS_create();
   if (error != RTCS_OK) 
   {
      printf("\nRTCS failed to initialize, error = %X", error);
      _task_block();
   }

   _IP_forward = TRUE;

#ifdef BSP_ENET_DEVICE_COUNT
#if  (BSP_ENET_DEVICE_COUNT > 0) 
   /*Following section is relevant only in case there as some enet driver available in BSP (FEC, MACNET, WIFI or other enet driver)*/
   
   ip_data.ip = ENET_IPADDR;
   ip_data.mask = ENET_IPMASK;
   ip_data.gateway = ENET_GATEWAY;  
   
   /* calcualte unique mac address from IP ADDRES */
   ENET_get_mac_address (ENET_DEVICE, ENET_IPADDR, enet_address);
   error = ipcfg_init_device (ENET_DEVICE, enet_address);
   if (error != RTCS_OK) 
   {
      printf("\nFailed to initialize ethernet device, error = %X", error);
      _task_block();
   }

#if RTCSCFG_ENABLE_LWDNS
   LWDNS_server_ipaddr = ENET_GATEWAY;   
   ipcfg_add_dns_ip(ENET_DEVICE,LWDNS_server_ipaddr);
#endif /* RTCSCFG_ENABLE_LWDNS */

   printf("\nWaiting for ethernet cable plug in ... ");
   while(!ipcfg_get_link_active(ENET_DEVICE)) {};
   printf("Cable connected\n");

#if RTCS_DHCP
   printf("Contacting DHCP server ... ");
   error = ipcfg_bind_dhcp_wait(ENET_DEVICE, FALSE, &ip_data);
#else
   printf("Setting static IP address ... ");
   error = ipcfg_bind_staticip (ENET_DEVICE, &ip_data);
#endif /* RTCS_DHCP */
   if (error != IPCFG_ERROR_OK) 
   {
      printf("\nRTCS failed to bind interface with IPv4, error = %X", error);
      _task_block();
   }
   else
   {
	  printf("OK\n");	   
   }

   ipcfg_get_ip(ENET_DEVICE, &ip_data);
   printf("\nIP Address      : %d.%d.%d.%d\n",IPBYTES(ip_data.ip));
   printf("\nSubnet Address  : %d.%d.%d.%d\n",IPBYTES(ip_data.mask));
   printf("\nGateway Address : %d.%d.%d.%d\n",IPBYTES(ip_data.gateway));
   printf("\nDNS Address     : %d.%d.%d.%d\n",IPBYTES(ipcfg_get_dns_ip(ENET_DEVICE,0)));

#endif /* BSP_ENET_DEVICE_COUNT > 0 */
#endif /* BSP_ENET_DEVICE_COUNT */   
   
#if RTCS_PPP
    PPP_start();
#endif /* RTCS_PPP */

   /* TODO: start server(s) in separate tasks */
}
