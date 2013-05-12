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
* $FileName: demo_cmd.c$
* $Version : 3.8.B.1$
* $Date    : Jun-22-2012$
*
* Comments:
*
*   This file contains the RTCS shell.
*
*END************************************************************************/


#include <mqx.h>
#include <fio.h>
#include <rtcs.h>
#include <ftpc.h>
#include <ftpd.h>
#include <shell.h>
#include <sh_rtcs.h>
#if SHELLCFG_USES_MFS
#include <sh_mfs.h>
#endif
#include "config.h"

#if ! SHELLCFG_USES_RTCS
#error This application requires SHELLCFG_USES_RTCS defined non-zero in user_config.h. Please recompile libraries with this option if any Ethernet interface is available.
#endif



const SHELL_COMMAND_STRUCT Shell_commands[] = {
    { "arpadd",    Shell_arpadd },
    { "arpdel",    Shell_arpdel },
    { "arpdisp",   Shell_arpdisp },
#if RTCSCFG_ENABLE_NAT
    { "dnat",      Shell_dnat },
#endif
#if RTCSCFG_ENABLE_UDP
    { "exec",      Shell_exec },
#endif
    { "exit",      Shell_exit },
    { "gate",      Shell_gate },
    { "gethbn",    Shell_get_host_by_name },
    { "getrt",     Shell_getroute },
    { "help",      Shell_help },
    { "ipconfig",  Shell_ipconfig },
#if DEMOCFG_USE_WIFI
    { "iwconfig",  Shell_iwconfig },
#endif
#if RTCSCFG_ENABLE_UDP
    { "load",      Shell_load },
#endif
#if RTCSCFG_ENABLE_NAT
    { "natinit",   Shell_natinit },
    { "natinfo",   Shell_natinfo },
#endif
    { "netstat",   Shell_netstat },
#if RTCSCFG_ENABLE_ICMP
    { "ping",      Shell_ping },
#endif
#if RTCSCFG_ENABLE_UDP
    { "sendto",    Shell_sendto },
#endif
#if RTCSCFG_ENABLE_SNMP
    { "snmpd",     Shell_SNMPd },
#endif
    { "telnet",    Shell_Telnet_client },
    { "telnetd",   Shell_Telnetd },
    { "ftp",       Shell_FTP_client },
    { "ftpd",      Shell_FTPd },
#if RTCSCFG_ENABLE_UDP
    { "tftp",      Shell_TFTP_client },
#endif

#if DEMOCFG_ENABLE_PPP
    { "ppp_start", shell_ppp_start},
    { "ppp_stop",  shell_ppp_stop },
#endif

    { "walkrt",    Shell_walkroute },

/* Filesystem commands */
#if SHELLCFG_USES_MFS
#if DEMOCFG_ENABLE_RAMDISK
    { "cd",        Shell_cd },
    { "copy",      Shell_copy },
    { "create",    Shell_create },
    { "del",       Shell_del },
    { "dir",       Shell_dir },
    { "format",    Shell_format },
    { "mkdir",     Shell_mkdir },
    { "pwd",       Shell_pwd },
    { "read",      Shell_read },
    { "ren",       Shell_rename },
    { "rmdir",     Shell_rmdir },
    { "write",     Shell_write },
#endif
#endif
    { "?",         Shell_command_list },
    { NULL,        NULL }
};


const SHELL_COMMAND_STRUCT Telnetd_shell_commands[] = {
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
#if RTCSCFG_ENABLE_UDP
    { "tftp",      Shell_TFTP_client },
#endif
    { "walkrt",    Shell_walkroute },
    { "?",         Shell_command_list },
    { NULL,        NULL }
};


const FTPd_COMMAND_STRUCT FTPd_commands[] = {
   { "abor", FTPd_unimplemented },
   { "acct", FTPd_unimplemented },
   { "quit", FTPd_quit },   
   { "noop", FTPd_noop },
   { "opts", FTPd_opts },
   { "pass", FTPd_pass },
   { "pasv", FTPd_pasv },
   { "port", FTPd_port },
   { "site", FTPd_site },
   { "syst", FTPd_syst },
   { "type", FTPd_type },
   { "user", FTPd_user },  
#if FTPDCFG_USES_MFS
   { "cdup", FTPd_cdup },
   { "cwd",  FTPd_cd   },
   { "dele", FTPd_dele },
   { "feat", FTPd_feat },
   { "list", FTPd_list },
   { "mkd",  FTPd_mkdir},
   { "nlst", FTPd_nlst },
   { "pwd",  FTPd_pwd  },
   { "retr", FTPd_retr },
   { "rmd",  FTPd_rmdir},
   { "rnfr", FTPd_rnfr },
   { "rnto", FTPd_rnto },
   { "size", FTPd_size },
   { "stor", FTPd_stor },
   { "xcwd", FTPd_cd    },
   { "xmkd", FTPd_mkdir },
   { "xpwd", FTPd_pwd   },
   { "xrmd", FTPd_rmdir },
#endif
   { "help", FTPd_help },
   { NULL,   NULL }
};


const FTPc_COMMAND_STRUCT FTPc_commands[] = {
   { "ascii",        FTPc_ascii       },
   { "binary",       FTPc_binary      },
   { "bye",          FTPc_bye         },
   { "cd",           FTPc_cd          },
   { "close",        FTPc_close       },
   { "delete",       FTPc_delete      },
   { "get",          FTPc_get         },
   { "help",         FTPc_help        },
   { "ls",           FTPc_ls          },
   { "mkdir",        FTPc_mkdir       },
   { "open",         FTPc_open        },
   { "pass",         FTPc_pass        },
   { "put",          FTPc_put         },
   { "pwd",          FTPc_pwd         },
   { "remotehelp",   FTPc_remotehelp  },
   { "rename",       FTPc_rename      },
   { "rmdir",        FTPc_rmdir       },
   { "user",         FTPc_user        },
   { NULL,   NULL }
};


/* EOF */
