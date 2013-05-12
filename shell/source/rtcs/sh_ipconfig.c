/**HEADER*******************************************************************
*
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: sh_ipconfig.c$
* $Version : 3.8.18.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example using RTCS Library.
*
*END************************************************************************/


#include <mqx.h>
#include "shell.h"
#if SHELLCFG_USES_RTCS

#include <bsp.h>
#include <rtcs.h>
#include <enet.h>
#include <ipcfg.h>
#include "sh_rtcs.h"
#include "sh_enet.h"
#include "string.h"

#ifdef BSP_ENET_DEVICE_COUNT
#if  (BSP_ENET_DEVICE_COUNT > 0)

static int_32 Shell_ipconfig_status (uint_32 enet_device)
{
    IPCFG_STATE             state;
    boolean                 link;
    boolean                 task;
    _enet_address           mac = {0};
    IPCFG_IP_ADDRESS_DATA   ip_data;
#if RTCSCFG_IPCFG_ENABLE_DNS
    _ip_address             dns;
#endif
#if RTCSCFG_IPCFG_ENABLE_BOOT
    _ip_address             tftp_serveraddress;
    uchar_ptr               tftp_servername;
    uchar_ptr               boot_filename;
#endif

    state = ipcfg_get_state (enet_device);
    if (state != IPCFG_STATE_INIT)
    {
        link = ipcfg_get_link_active (enet_device);
        task = ipcfg_task_status ();
        ipcfg_get_mac (enet_device, mac);
        ipcfg_get_ip (enet_device, &ip_data);
#if RTCSCFG_IPCFG_ENABLE_DNS
        dns = ipcfg_get_dns_ip (enet_device, 0);
#endif
#if RTCSCFG_IPCFG_ENABLE_BOOT
        tftp_serveraddress = ipcfg_get_tftp_serveraddress (enet_device);
        tftp_servername = ipcfg_get_tftp_servername (enet_device);
        boot_filename = ipcfg_get_boot_filename (enet_device);
#endif
        printf ("Eth#: %d\n", enet_device);
        printf ("Link: %s\n", link ? "on" : "off");
        printf ("Bind: %s\n", ipcfg_get_state_string (state));
        printf ("MAC : %02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        printf ("IP  : %d.%d.%d.%d\n", IPBYTES(ip_data.ip));
        printf ("MASK: %d.%d.%d.%d\n", IPBYTES(ip_data.mask));
#if RTCSCFG_ENABLE_GATEWAYS
        printf ("GATE: %d.%d.%d.%d\n", IPBYTES(ip_data.gateway));
#endif
#if RTCSCFG_IPCFG_ENABLE_DNS
        printf ("DNS1: %d.%d.%d.%d\n", IPBYTES(dns));
#endif
#if RTCSCFG_IPCFG_ENABLE_BOOT
        printf ("TFTP: %d.%d.%d.%d '%s'\n", IPBYTES(tftp_serveraddress), tftp_servername);
        printf ("BOOT: '%s'\n", boot_filename);
#endif
        printf ("Link status task %s\n", task ? "running" : "stopped");
    }
    else
    {
        printf ("Ethernet device %d not yet initialized.\n", enet_device);
    }
    return SHELL_EXIT_SUCCESS;
}



static int_32 Shell_ipconfig_init (uint_32 enet_device, uint_32 index, int_32 argc, char_ptr argv[])
{
    uint_32         error;
    _enet_address   mac;

    ipcfg_get_mac(enet_device, mac);
    mac[5] += enet_device;

    if (argc > ++index)
    {
        if (! Shell_parse_enet_address(argv[index], mac))
        {
            printf ("Error init ethernet device %d, invalid MAC address!\n", enet_device);
            return SHELL_EXIT_ERROR;
        }
    }

    error = ipcfg_init_device (enet_device, mac);
    if (error != 0)
    {
        printf ("Error init ethernet device %d, interface error %08x!\n", enet_device, error);
        return SHELL_EXIT_ERROR;
    }

    printf ("Ethernet device %d initialization successful.\n", enet_device);
    return SHELL_EXIT_SUCCESS;
}



static int_32 Shell_ipconfig_task (uint_32 enet_device, uint_32 index, int_32 argc, char_ptr argv[])
{
    uint_32 error;
    uint_32 priority;
    uint_32 period;

    if (argc > ++index)
    {
        if (strcmp (argv[index], "start") == 0)
        {
            if (argc > ++index)
            {
                if (! Shell_parse_number(argv[index], &priority))
                {
                    printf ("Error in task command, invalid priority parameter!\n");
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                printf ("Error in task command, missing priority parameter!\n");
                return SHELL_EXIT_ERROR;
            }

            if (argc > ++index)
            {
                if (! Shell_parse_number(argv[index], &period))
                {
                    printf ("Error in task command, invalid priority parameter!\n");
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                printf ("Error in task command, missing period parameter!\n");
                return SHELL_EXIT_ERROR;
            }

            error = ipcfg_task_create (priority, period);
            if (error != 0)
            {
                printf ("Error in task command, create failed!\n", enet_device, error);
                return SHELL_EXIT_ERROR;
            }
        }
        else if (strcmp (argv[index], "stop") == 0)
        {
            ipcfg_task_destroy (TRUE);
        }
        else
        {
            printf ("Error in task command, invalid parameter!\n");
            return SHELL_EXIT_ERROR;
        }
    }

    printf ("Link status task %s\n", ipcfg_task_status () ? "running" : "stopped");
    return SHELL_EXIT_SUCCESS;
}



#if RTCSCFG_IPCFG_ENABLE_DNS
static int_32 Shell_ipconfig_dns (uint_32 enet_device, uint_32 index, int_32 argc, char_ptr argv[])
{
    uint_32     i;
    _ip_address dns = 0;

    if (argc > ++index)
    {
        if (strcmp (argv[index], "add") == 0)
        {
            if (argc > ++index)
            {
                if (! Shell_parse_ip_address (argv[index], &dns))
                {
                    printf ("Error in dns command, invalid ip address!\n");
                    return SHELL_EXIT_ERROR;
                }
                if (ipcfg_add_dns_ip (enet_device, dns))
                {
                    printf ("Add dns ip successful.\n");
                }
                else
                {
                    printf ("Add dns ip failed!\n");
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                printf ("Error in dns command, missing ip parameter!\n");
                return SHELL_EXIT_ERROR;
            }
        }
        else if (strcmp (argv[index], "del") == 0)
        {
            if (argc > ++index)
            {
                if (! Shell_parse_ip_address (argv[index], &dns))
                {
                    printf ("Error in dns command, invalid ip address!\n");
                    return SHELL_EXIT_ERROR;
                }
                if (ipcfg_del_dns_ip (enet_device, dns))
                {
                    printf ("Del dns ip successful.\n");
                }
                else
                {
                    printf ("Del dns ip failed!\n");
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                printf ("Error in dns command, missing ip parameter!\n");
                return SHELL_EXIT_ERROR;
            }
        }
        else
        {
            printf ("Error in dns command, unknown parameter!\n");
            return SHELL_EXIT_ERROR;
        }

    }
    else
    {
        for (i = 0, dns = ipcfg_get_dns_ip (enet_device, i); dns != 0 ; i++, dns = ipcfg_get_dns_ip (enet_device, i))
        {
            printf ("%d: %d.%d.%d.%d\n", i + 1, IPBYTES(dns));
        }
    }

    return SHELL_EXIT_SUCCESS;
}
#endif



static int_32 Shell_ipconfig_staticip (uint_32 enet_device, uint_32 index, int_32 argc, char_ptr argv[])
{
    uint_32                 error;
    IPCFG_IP_ADDRESS_DATA   ip_data;

    if (argc > ++index)
    {
        if (! Shell_parse_ip_address (argv[index], &ip_data.ip))
        {
            printf ("Error in static ip command, invalid ip address!\n");
            return SHELL_EXIT_ERROR;
        }
    }
    else
    {
        printf ("Error in static ip command, missing ip address parameter!\n");
        return SHELL_EXIT_ERROR;
    }

    if (argc > ++index)
    {
        if (! Shell_parse_ip_address (argv[index], &ip_data.mask))
        {
            printf ("Error in static ip command, invalid mask!\n");
            return SHELL_EXIT_ERROR;
        }
    }
    else
    {
        printf ("Error in static ip command, missing mask parameter!\n");
        return SHELL_EXIT_ERROR;
    }

#if RTCSCFG_ENABLE_GATEWAYS
    if (argc > ++index)
    {
        if (! Shell_parse_ip_address (argv[index], &ip_data.gateway))
        {
            printf ("Error in static ip command, invalid gateway!\n");
            return SHELL_EXIT_ERROR;
        }
    }
#endif

    error = ipcfg_bind_staticip (enet_device, &ip_data);
    if (error != 0)
    {
        printf ("Error during static ip bind %08x!\n", error);
        return SHELL_EXIT_ERROR;
    }

    printf ("Static ip bind successful.\n");
    return SHELL_EXIT_SUCCESS;
}



#if RTCSCFG_IPCFG_ENABLE_DHCP
static int_32 Shell_ipconfig_dhcp (uint_32 enet_device, uint_32 index, int_32 argc, char_ptr argv[])
{
    uint_32                     error;
    IPCFG_IP_ADDRESS_DATA       auto_ip_data;
    boolean                     auto_ip = FALSE ;

    if (argc > ++index)
    {
        if (strcmp (argv[index], "noauto") == 0)
        {
            auto_ip = FALSE;
        }
        else
        {
            if (! Shell_parse_ip_address (argv[index], &auto_ip_data.ip))
            {
                printf ("Error in dhcp command, invalid ip address!\n");
                return SHELL_EXIT_ERROR;
            }

            if (argc > ++index)
            {
                if (! Shell_parse_ip_address (argv[index], &auto_ip_data.mask))
                {
                    printf ("Error in dhcp command, invalid mask!\n");
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                printf ("Error in dhcp command, missing mask parameter!\n");
                return SHELL_EXIT_ERROR;
            }

#if RTCSCFG_ENABLE_GATEWAYS
            if (argc > ++index)
            {
                if (! Shell_parse_ip_address (argv[index], &auto_ip_data.gateway))
                {
                    printf ("Error dhcp command, invalid gateway!\n");
                    return SHELL_EXIT_ERROR;
                }
            }
            else
            {
                printf ("Error in dhcp command, missing gateway parameter!\n");
                return SHELL_EXIT_ERROR;
            }
#endif
            auto_ip = TRUE;
        }
    }

    error = ipcfg_bind_dhcp_wait (enet_device, auto_ip, &auto_ip_data);
    if (error != IPCFG_OK)
    {
        printf ("Error during dhcp bind %08x!\n", error);
        return SHELL_EXIT_ERROR;
    }

    printf ("Bind via dhcp successful.\n");
    return SHELL_EXIT_SUCCESS;
}
#endif



static int_32 Shell_ipconfig_autoip (uint_32 enet_device, uint_32 index, int_32 argc, char_ptr argv[])
{
   printf ("This command has been removed, please use ipconfig staticip commnad instead");
   return SHELL_EXIT_SUCCESS;
}



static int_32 Shell_ipconfig_unbind (uint_32 enet_device)
{
    uint_32 error;

    error = ipcfg_unbind (enet_device);
    if (error != 0)
    {
        printf ("Error during unbind %08x!\n", error);
        return SHELL_EXIT_ERROR;
    }

    printf ("Unbind successful.\n");
    return SHELL_EXIT_SUCCESS;
}



static int_32 Shell_ipcfg_phy_registers(uint_32 enet_device)
{
    uint_32  i,regs[32];

    if (ipcfg_phy_registers (enet_device,32,regs))
    {
    printf ("Phy registers.\n");
    for (i=0;i<32;i++) {
       printf("Register %2d (0x%02x) = 0x%04x\n",i,regs[i]);
    }
   } else {
        printf ("Error reading phy registers!\n");
    }
    return SHELL_EXIT_SUCCESS;
}






/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_ipconfig
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

int_32 Shell_ipconfig(int_32 argc, char_ptr argv[] )
{ /* Body */
    boolean                 print_usage, shorthelp = FALSE;
    int_32                  return_code = SHELL_EXIT_SUCCESS;
    uint_32                 enet_device = BSP_DEFAULT_ENET_DEVICE, index = 1;

    print_usage = Shell_check_help_request (argc, argv, &shorthelp);

    if (!print_usage)
    {
        if (argc > index)
        {
            if (Shell_parse_number (argv[index], &enet_device))
            {
                index++;
            }
        }

        if (enet_device >= BSP_ENET_DEVICE_COUNT)
        {
            printf ("Wrong number of ethernet device (%d)!\n", enet_device);
            return_code = SHELL_EXIT_ERROR;
        }
        else
        {
            if (argc > index)
            {
                if (strcmp (argv[index], "init") == 0)
                {
                    return_code = Shell_ipconfig_init (enet_device, index, argc, argv);
                }
                else if (strcmp (argv[index], "task") == 0)
                {
                    return_code = Shell_ipconfig_task (enet_device, index, argc, argv);
                }
#if RTCSCFG_IPCFG_ENABLE_DNS
                else if (strcmp (argv[index], "dns") == 0)
                {
                    return_code = Shell_ipconfig_dns (enet_device, index, argc, argv);
                }
#endif
                else if (strcmp (argv[index], "staticip") == 0)
                {
                    return_code = Shell_ipconfig_staticip (enet_device, index, argc, argv);
                }
#if RTCSCFG_IPCFG_ENABLE_DHCP
                else if (strcmp (argv[index], "dhcp") == 0)
                {
                    return_code = Shell_ipconfig_dhcp (enet_device, index, argc, argv);
                }
#endif
                else if (strcmp (argv[index], "autoip") == 0)
                {
                    return_code = Shell_ipconfig_autoip (enet_device, index, argc, argv);
                }
#if RTCSCFG_IPCFG_ENABLE_BOOT
                else if (strcmp (argv[index], "boot") == 0)
                {
                    return_code = Shell_ipconfig_boot (enet_device);
                }
#endif
                else if (strcmp (argv[index], "unbind") == 0)
                {
                    return_code = Shell_ipconfig_unbind (enet_device);
                }
                else if (strcmp (argv[index], "phyinfo") == 0)
                {
                    return_code = Shell_ipcfg_phy_registers (enet_device);
                }
                else
                {
                    printf ("Unknown ipconfig command!\n");
                    return_code = SHELL_EXIT_ERROR;
                }
            }
            else
            {
                return_code = Shell_ipconfig_status (enet_device);
            }
        }
    }

    if ((print_usage) || (return_code != SHELL_EXIT_SUCCESS))
    {
        if (shorthelp)
        {
            printf ("%s [<device>] [<command>]\n", argv[0]);
        }
        else
        {
            printf ("Usage: %s [<device>] [<command>]\n", argv[0]);
            printf ("  Commands:\n");
            printf ("    init [<mac>]                            = Initialize ethernet device (once)\n");
            printf ("    task [start <priority> <period> | stop] = Manage link status checking task\n");
#if RTCSCFG_IPCFG_ENABLE_DNS
            printf ("    dns [add <ip> | del <ip>]               = Manage dns ip list\n");
#endif
            printf ("    staticip <ip> <mask> [<gateway>]        = Bind with static ip\n");
#if RTCSCFG_IPCFG_ENABLE_DHCP
            printf ("    dhcp [<ip> <mask> [<gateway>]]          = Bind with dhcp [use <ip> & <mask> in case dhcp fails]\n");
#endif
            printf ("    autoip [<ip> <mask> [<gateway>]]        = Obsolete, use staticip instead\n");
#if RTCSCFG_IPCFG_ENABLE_BOOT
            printf ("    boot                                    = Bind with boot protocol\n");
#endif
            printf ("    unbind                                  = Unbind network interface\n");
            printf ("  Parameters:\n");
            printf ("    <device>   = Ethernet device number (default %d)\n", BSP_DEFAULT_ENET_DEVICE);
            printf ("    <mac>      = Ethernet MAC address\n"  );
            printf ("    <priority> = Link status task MQX priority\n");
            printf ("    <period>   = Link status task check period (ms)\n");
            printf ("    <ip>       = IP address to use \n");
            printf ("    <mask>     = Network mask to use \n");
            printf ("    <gateway>  = Network gateway to use\n");
        }
    }

    return return_code;
} /* Endbody */
#else /* (BSP_ENET_DEVICE_COUNT > 0) */
int_32 Shell_ipconfig(int_32 argc, char_ptr argv[] )
{
   printf ("Cannot use this command, no enet device driver available in this BSP.");
   return SHELL_EXIT_ERROR;
}
#endif /* (BSP_ENET_DEVICE_COUNT > 0) */
#else /* BSP_ENET_DEVICE_COUNT */
int_32 Shell_ipconfig(int_32 argc, char_ptr argv[] )
{
   printf ("Cannot use this command, no enet device driver available in this BSP.");
   return SHELL_EXIT_ERROR;
}
#endif /* BSP_ENET_DEVICE_COUNT */
#endif /* SHELLCFG_USES_RTCS */
/* EOF */

