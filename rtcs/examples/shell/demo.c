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
* $FileName: demo.c$
* $Version : 3.8.B.9$
* $Date    : Jun-25-2012$
*
* Comments:
*
*   Example using RTCS Library.
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <mfs.h>
#include <rtcs.h>
#include <shell.h>
#include "config.h"  


#if DEMOCFG_ENABLE_PPP
#include <ppp.h>

#if PPP_DEVICE_RAS
#include <fio.h>
#endif /* PPP_DEVICE_RAS */

#if ! RTCSCFG_ENABLE_VIRTUAL_ROUTES
#error This application requires RTCSCFG_ENABLE_VIRTUAL_ROUTES defined non-zero in user_config.h. Please recompile libraries (BSP, RTCS) with this option.
#endif

#if ! RTCSCFG_ENABLE_GATEWAYS
#error This application requires RTCSCFG_ENABLE_GATEWAYS defined non-zero in user_config.h. Please recompile libraries (BSP, RTCS) with this option.
#endif

#if (PPP_DEVICE_DUN+PPP_DEVICE_RAS) == 2
#error PPP server and PPP client can not run in parallel. Please correct PPP_DEVICE_DUN and PPP_DEVICE_RAS macros in config.h based on the requested functionality.
#endif

#warning This application requires PPP device to be defined manually and being different from the default IO channel (BSP_DEFAULT_IO_CHANNEL). See PPP_DEVICE in config.h.

#warning This application requires PPP device QUEUE_SIZE  to be more than size of input LCP packet (set BSPCFG_UARTX_QUEUE_SIZE to 128 in user_config.h) If no, you can lose LCP packets.

#if PPP_SECRETS_NOT_SHARED
#error This application requires PPP_SECRETS_NOT_SHARED defined to zero in /src/rtcs/source/include/ppp.h. Please recompile RTCS with this option.
#endif

#endif /* DEMOCFG_ENABLE_PPP */


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif



#define MAIN_TASK 1

/* PPP block */
#if DEMOCFG_ENABLE_PPP

    _ppp_handle     PPP_handle      = NULL;
    _rtcs_if_handle ihandle         = 0;
    _iopcb_handle   pio;
    _ppp_handle     phandle         = 0;
     MQX_FILE_PTR   pfile;
    _ip_address     local_address   = 0; //address will be assigned by remote host
    LWSEM_STRUCT   ppp_sem;


    extern _ppp_handle PPP_start(MQX_FILE_PTR ppp_handler);

int_32 shell_ppp_start(int_32 argc, char_ptr argv[] )
{
    uint_32           error;
    boolean           print_usage, shorthelp = FALSE; 


    print_usage = Shell_check_help_request(argc, argv, &shorthelp );

    if (print_usage)  
    {
        if (shorthelp)  
        {
            printf("%s - Start PPP connection. \n", argv[0]);
            return RTCS_OK;
        } 
        else  
        {
            printf("Usage: %s \n", argv[0]);
            printf("   Any parameters requered.\n");
            return RTCS_OK;
        }
    }
    
    printf("Start PPP...\n");
     _io_fputs("CLIENTCLIENT", pfile);
    fflush(pfile);
    PPP_handle = PPP_start(pfile);
    if(PPP_handle)
    {
        return RTCS_OK;
    }
    else
    {
        printf("\n Error: The connection attemp failed because the modem (or other connecting device) on the remote computer is out of order \n");
        error = PPP_close(phandle);                     /* send connection terminate request to remote server and close PPP connection */
        if(phandle)
        {
            error = PPP_shutdown(phandle);              /* clean up all PPP structure */
        }
        error = _lwsem_destroy(&ppp_sem);               /* destroy semafor */
        if(ihandle)
        {
            error = RTCS_if_remove(ihandle);            /* unregister PPP interface */
        }  
        if(pio)
        {  
            error = _iopcb_close(pio);                  /* close PPP driver */
        }
        printf("\nPPP connection closed\n");        
        return RTCS_ERROR;
    }
}

int_32 shell_ppp_stop(int_32 argc, char_ptr argv[] )
{
    uint_32           error;
    boolean           print_usage, shorthelp = FALSE; 


    print_usage = Shell_check_help_request(argc, argv, &shorthelp );

    if (print_usage)  
    {
        if (shorthelp)  
        {
            printf("%s - Stop PPP connection. \n", argv[0]);
            return RTCS_OK;
        } 
        else  
        {
            printf("Usage: %s \n", argv[0]);
            printf("   Any parameters requered.\n");
            return RTCS_OK;
        }
    }

    error = RTCS_if_unbind(ihandle,local_address);  /* unbind interface */
    error = PPP_close(phandle);                     /* send connection terminate request to remote server and close PPP connection */
    if(phandle)
    {
        error = PPP_shutdown(phandle);              /* clean up all PPP structure */
    }
    error = _lwsem_destroy(&ppp_sem);               /* destroy semafor */
    if(ihandle)
    {
        error = RTCS_if_remove(ihandle);            /* unregister PPP interface */
    }  
    if(pio)
    {  
        error = _iopcb_close(pio);                  /* close PPP driver */
    }
    printf("PPP connection closed\n");      
}

#endif

/*  
**  FTP root directory can be specified here. 
*/
char FTPd_rootdir[] = "a:\\"; 
extern const SHELL_COMMAND_STRUCT Shell_commands[];
extern void Main_task (uint_32);
extern void Ram_disk_start(void);





const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,   Function,   Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    { MAIN_TASK,    Main_task,  2000,   9,          "Main", MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};


#if DEMOCFG_ENABLE_RAMDISK

#if defined BSP_M54455EVB || defined BSP_TWRMCF54418
    #define RAM_DISK_SIZE   0x01000000
    static uchar            ram_disk[RAM_DISK_SIZE];
    #define RAM_DISK_BASE   &ram_disk[0]
#elif defined BSP_TWRMPC5125
    #define RAM_DISK_SIZE   0x00500000
    static uchar            ram_disk[RAM_DISK_SIZE];
    #define RAM_DISK_BASE   &ram_disk[0]
#elif defined BSP_TWR_K60N512 || defined BSP_TWR_K70F120M
    #define RAM_DISK_SIZE   0x00010000
    static uchar            ram_disk[RAM_DISK_SIZE];
    #define RAM_DISK_BASE   &ram_disk[0]
#elif defined BSP_TWRPXS30
    #define RAM_DISK_SIZE   0x00020000
    static uchar            ram_disk[RAM_DISK_SIZE];
    #define RAM_DISK_BASE   &ram_disk[0]
/* The MRam disk base address definition taken from linker command file*/
#elif defined (BSP_EXTERNAL_MRAM_RAM_BASE) && defined (BSP_EXTERNAL_MRAM_RAM_SIZE)
    #define RAM_DISK_BASE   BSP_EXTERNAL_MRAM_RAM_BASE 
    #define RAM_DISK_SIZE   BSP_EXTERNAL_MRAM_RAM_SIZE
#else
   #error RAM disk needs space to be defined
#endif



/*FUNCTION*------------------------------------------------
* 
* Function Name: Ram_disk_start
* Comments     :
*    
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
        printf("\nError installing memory device (0x%x)", mqx_status);
        _task_block();
    }

    /* Open the device which MFS will be installed on */
    dev_handle1 = fopen("mfs_ramdisk:", 0);
    if ( dev_handle1 == NULL ) 
    {
        printf("\nUnable to open Ramdisk device");
        _task_block();
    }

    /* Install MFS  */
    mqx_status = _io_mfs_install(dev_handle1, "a:", (_file_size)0);
    if (mqx_status != MFS_NO_ERROR) 
    {
        printf("\nError initializing a:");
        _task_block();
    } 
    else 
    {
        printf("\nInitialized Ram Disk to a:\\");
    }

    /* Open the filesystem and detect, if format is required */
    a_fd_ptr = fopen("a:", NULL);
    error_code = ferror(a_fd_ptr);
    if ((error_code != MFS_NO_ERROR) && (error_code != MFS_NOT_A_DOS_DISK))
    {
        printf("\nError while opening a:\\ (%s)", MFS_Error_text((uint_32)(uint_32)error_code));
        _task_block();
    }
    if (error_code == MFS_NOT_A_DOS_DISK) 
    {
        printf("\nNOT A DOS DISK! You must format to continue.");
    }
    
} 

#endif /* DEMOCFG_ENABLE_RAMDISK */


#if DEMOCFG_ENABLE_PPP


    static boolean PPP_link = FALSE;
    static void PPP_linkup (pointer lwsem) 
    {
        PPP_link = TRUE; 
        _lwsem_post(lwsem);
    }

    static void PPP_linkdown (pointer lwsem) 
    {
        PPP_link = FALSE;
        _lwsem_post(lwsem);
    }

    char localname[]    = "ppp_user";
    char localsecret[]  = "Secret129456cz";
    char remotename[]   = "guest";
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
_ppp_handle PPP_start(MQX_FILE_PTR ppp_handler)

{

    uint_32           error;


    _ip_address       peer_address=0; //address will be assigned by remote host
    IPCP_DATA_STRUCT  ipcp_data;

 
    _PPP_PAP_LSECRET = &lsecret;
    _PPP_PAP_RSECRETS  = NULL; /* rsecrets  */
    _PPP_CHAP_LNAME    = NULL; /* localname */
    _PPP_CHAP_LSECRETS = NULL; /* lsecrets  */
    _PPP_CHAP_RSECRETS = NULL; /* rsecrets  */

    /* Install a route for a default gateway */
    RTCS_gate_add(GATE_ADDR, INADDR_ANY, INADDR_ANY);

    pio = _iopcb_ppphdlc_init(ppp_handler);

    _PPP_ACCM = 0;
    error = PPP_initialize(pio, &phandle);
    
    if (error) 
    {
       printf("\nPPP initialize: %lx", error);
       _task_block();
    } /* Endif */

    _iopcb_open(pio, PPP_lowerup, PPP_lowerdown, phandle);
    
    error = RTCS_if_add(phandle, RTCS_IF_PPP, &ihandle);
    
    if (error) 
    {
       printf("\nIF add failed, error = %lx", error);
       _task_block();
    } /* Endif */
    _lwsem_create(&ppp_sem, 0);
    _mem_zero(&ipcp_data, sizeof(ipcp_data));
    ipcp_data.IP_UP              = PPP_linkup;
    ipcp_data.IP_DOWN            = PPP_linkdown;
    ipcp_data.IP_PARAM           = &ppp_sem;
    ipcp_data.ACCEPT_LOCAL_ADDR  = TRUE;/* FALSE */
    ipcp_data.ACCEPT_REMOTE_ADDR = TRUE;/* FALSE */
    ipcp_data.LOCAL_ADDR         = 0;   /* PPP_LOCADDR  */
    ipcp_data.REMOTE_ADDR        = 0;   /* PPP_PEERADDR */
    ipcp_data.DEFAULT_NETMASK    = TRUE;
    ipcp_data.DEFAULT_ROUTE      = TRUE;
    
    _time_delay(1500);
    
    error = RTCS_if_bind_IPCP(ihandle, &ipcp_data);
    _time_delay(1000);

    if (error) 
    {
       printf("\nIF bind failed, error = %lx", error);
       _task_block();
    } /* Endif */
    printf("\nPlease initiate PPP connection.  Waiting...\n");    

/*Handshake with RAS server with NULL-modem*/    
#if PPP_DEVICE_RAS
    error = 0; /* 10 attempts to connect */
    while (error < 10)
    {
        if (PPP_link == FALSE)
        {
          _time_delay(1000);
          printf("time = %d\n",error);	
        }
        else
        {
            printf("\nPPP_link = %d, time = %d\n",PPP_link,error);
            break;
        }
        error ++;
    }
    if (PPP_link == TRUE) 
    {
        local_address = IPCP_get_local_addr(ihandle);
        peer_address = IPCP_get_peer_addr(ihandle);
        printf("\nPPP device on %s is bound on.\n", PPP_DEVICE);
        printf(" PPP local address is   : %d.%d.%d.%d\n", IPBYTES(local_address));
        printf(" PPP remote address is : %d.%d.%d.%d\n", IPBYTES(peer_address));
        printf(" Now PPP connection is established on %d.%d.%d.%d\n", IPBYTES(peer_address));
    }
    else
    {
        _lwsem_post(&ppp_sem);    	
    }
#endif /* PPP_DEVICE_RAS */

    _lwsem_wait(&ppp_sem);

#if PPP_DEVICE_DUN
    printf("\nPPP device %s bound to %d.%d.%d.%d", PPP_DEVICE, IPBYTES(ipcp_data.LOCAL_ADDR));    
#endif /* PPP_DEVICE_DUN */

    if (PPP_link == TRUE) 
    {
        return (phandle);
    }
    else
    {
        phandle = NULL;
        return(phandle);
    }

}
#endif /* DEMOCFG_ENABLE_PPP */

/*TASK*-----------------------------------------------------------------
*
* Task Name      : Main_task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

void Main_task(uint_32 temp)
{
    uint_32     error;

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
    if (error != RTCS_OK) 
    {
        printf("\nRTCS failed to initialize, error = %X", error);
        _task_block();
    }
    /* Enable IP forwarding */
    _IP_forward = TRUE;

#if DEMOCFG_ENABLE_PPP

    pfile = fopen(PPP_DEVICE, NULL);

    #if PPP_DEVICE_DUN /* Dial-Up Network */
        _io_dun_install("dun:");
    #elif PPP_DEVICE_RAS /* Remote Access Service */
        _io_ras_install("dun:");
    #endif
    
    pfile = fopen("dun:", (char_ptr)pfile);
    _time_delay(1000); 
    
#endif /* DEMOCFG_ENABLE_PPP */

#if DEMOCFG_ENABLE_RAMDISK  
    Ram_disk_start();
    /*If root directory is not specified, use root from first valid filesystem */
    if(FTPd_rootdir[0] == '\0')
    {
        _io_get_fs_name(_io_get_first_valid_fs(), FTPd_rootdir, sizeof(FTPd_rootdir));
        FTPd_rootdir[strlen(FTPd_rootdir)] = '\\';
        FTPd_rootdir[strlen(FTPd_rootdir)+1] = '\0';
    }
#endif /* DEMOCFG_ENABLE_RAMDISK */


    /* Run the shell */
    Shell(Shell_commands, NULL);
    for(;;) 
    {
        printf("Shell exited, restarting...\n");
        Shell(Shell_commands, NULL);
    }
}

/* EOF */
