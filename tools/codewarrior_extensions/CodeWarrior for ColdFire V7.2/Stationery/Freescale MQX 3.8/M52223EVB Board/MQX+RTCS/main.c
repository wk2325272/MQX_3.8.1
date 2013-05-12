/****************************************************************************
* 
*   This file contains MQX+RTCS stationery code.
*
****************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <shell.h>
#include <sh_rtcs.h>
#include <ipcfg.h>
#include "main.h"


#ifdef PPP_DEVICE
#include <ppp.h>

#if ! RTCSCFG_ENABLE_VIRTUAL_ROUTES
#error This application requires RTCSCFG_ENABLE_VIRTUAL_ROUTES defined non-zero in user_config.h. Please recompile libraries (BSP, RTCS) with this option.
#endif

#if ! RTCSCFG_ENABLE_GATEWAYS
#error This application requires RTCSCFG_ENABLE_GATEWAYS defined non-zero in user_config.h. Please recompile libraries (BSP, RTCS) with this option.
#endif

#warning This application requires PPP device to be defined manually and being different from the default IO channel (BSP_DEFAULT_IO_CHANNEL). See PPP_DEVICE in main.h.
#endif


#if !BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


const SHELL_COMMAND_STRUCT Shell_commands[] = {
   /* RTCS commands */ 
   { "arpadd",    Shell_arpadd },
   { "arpdel",    Shell_arpdel },
   { "arpdisp",   Shell_arpdisp },
   { "exit",      Shell_exit },      
   { "gate",      Shell_gate },
   { "gethbn",    Shell_get_host_by_name }, 
   { "getrt",     Shell_getroute },
   { "help",      Shell_help }, 
   { "ipconfig",  Shell_ipconfig },      
   { "netstat",   Shell_netstat },   
#if RTCSCFG_ENABLE_ICMP      
   { "ping",      Shell_ping },      
#endif
#if ENET_DEVICE
   { "telnet",    Shell_Telnet_client },
   { "telnetd",   Shell_Telnetd },
#endif
   { "walkrt",    Shell_walkroute },
   { "?",         Shell_command_list },     
   { NULL,        NULL } 
};


const SHELL_COMMAND_STRUCT Telnetd_shell_commands[] = {
   /* RTCS commands */ 
   { "exit",      Shell_exit },      
   { "gethbn",    Shell_get_host_by_name }, 
   { "getrt",     Shell_getroute },
   { "help",      Shell_help }, 
   { "ipconfig",  Shell_ipconfig },      
   { "netstat",   Shell_netstat },
   { "pause",     Shell_pause },
#if RTCSCFG_ENABLE_ICMP      
   { "ping",      Shell_ping },
#endif
   { "telnet",    Shell_Telnet_client },
   { "walkrt",    Shell_walkroute },
   { "?",         Shell_command_list },     
   { NULL,        NULL } 
};


TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   3000,  9,   "main", MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0}
};


#ifdef PPP_DEVICE

LWSEM_STRUCT ppp_sem;
extern void PPP_start(void);
static void PPP_linkup (pointer lwsem) {_lwsem_post(lwsem);}

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

#endif


/*TASK*-----------------------------------------------------------------
*
* Function Name  : Main_task
* Comments       :
*    This task initializes RTCS and starts SHELL.
*
*END------------------------------------------------------------------*/

void Main_task(uint_32 initial_data)
{
   uint_32 error;

   error = RTCS_create();
   if (error != RTCS_OK) 
   {
      printf("\nRTCS failed to initialize, error = %X", error);
      _task_block();
   }

#ifdef PPP_DEVICE
   PPP_start();
#else
   _IP_forward = TRUE;
   IPCFG_default_enet_device = ENET_DEVICE;
   IPCFG_default_ip_address = ENET_IPADDR;
   IPCFG_default_ip_mask = ENET_IPMASK;
   IPCFG_default_ip_gateway = 0;
   ENET_get_mac_address (IPCFG_default_enet_device, IPCFG_default_ip_address, IPCFG_default_enet_address);

   /* IPCFG can be initialized from shell or by calling ipcfg_init_device() */ 
   printf("Start by initializing Ethernet device with 'ipconfig init'...\n");
#endif

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
