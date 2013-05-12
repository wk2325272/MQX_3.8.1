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
*
* Comments:
*
*   Example using RTCS Library.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <ftpd.h>
#include "demo.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if ! FTPDCFG_USES_MFS
#error This application requires FTPDCFG_USES_MFS defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


void Main_task (uint_32);
extern void USB_task(uint_32 param);

/*
** MQX initialization information
*/

const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    { 1,            Main_task,  2000,   9L,         "FTP",  MQX_AUTO_START_TASK, 0,     0 },
    { 5,            USB_task,   5200L,  8L,         "USB",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

void Main_task( uint_32 temp)
{
   _ip_address       enet_ipaddr = ENET_IPADDR;
   _ip_address       enet_ipmask = ENET_IPMASK;
   uint_32           enet_device = 0;
   _enet_address     address;
   _enet_handle      ehandle;
   _rtcs_if_handle   ihandle;
   uint_32           error;

#if DEMO_USE_POOLS && defined(DEMO_RTCS_POOL_ADDR) && defined(DEMO_RTCS_POOL_SIZE)
    /* use external RAM for RTCS buffers */
    _RTCS_mem_pool = _mem_create_pool((pointer)DEMO_RTCS_POOL_ADDR, DEMO_RTCS_POOL_SIZE);
#endif

   /* runtime RTCS configuration */
   _RTCSPCB_init = 4;
   _RTCSPCB_grow = 2;
   _RTCSPCB_max = 20;
   _RTCS_msgpool_init = 4;
   _RTCS_msgpool_grow = 2;
   _RTCS_msgpool_max  = 20;
   _RTCS_socket_part_init = 4;
   _RTCS_socket_part_grow = 2;
   _RTCS_socket_part_max  = 20;

   if (RTCS_create()== RTCS_OK) {
                         
   ENET_get_mac_address(BSP_DEFAULT_ENET_DEVICE,ENET_IPADDR,address);
   error = ENET_initialize(BSP_DEFAULT_ENET_DEVICE, address, 0, &ehandle);

   error = RTCS_if_add(ehandle, RTCS_IF_ENET, &ihandle);
   error = RTCS_if_bind(ihandle, ENET_IPADDR, ENET_IPMASK);
 
   // Direct call to FTP Server so that we become the FTP Server
   FTPd_init("FTP_server", 7, 5000 ); //creates new task
   }
}

char FTPd_rootdir[] = {"c:\\"}; 
const FTPd_COMMAND_STRUCT FTPd_commands[] = {


   { "abor", FTPd_unimplemented },
   { "acct", FTPd_unimplemented },
   { "cdup", FTPd_cdup },        

   { "cwd",  FTPd_cd   },        
   { "feat", FTPd_feat },       
   { "help", FTPd_help },       
   { "dele", FTPd_dele },       
   { "list", FTPd_list },       
   { "mkd",  FTPd_mkdir},       
   { "noop", FTPd_noop },
   { "nlst", FTPd_nlst },       
   { "opts", FTPd_opts },
   { "pass", FTPd_pass },
   { "pasv", FTPd_pasv },
   { "port", FTPd_port },
   { "pwd",  FTPd_pwd  },       
   { "quit", FTPd_quit },
   { "rnfr", FTPd_rnfr },
   { "rnto", FTPd_rnto },
   { "retr", FTPd_retr },
   { "stor", FTPd_stor },
   { "rmd",  FTPd_rmdir},       
   { "site", FTPd_site },
   { "size", FTPd_size },
   { "syst", FTPd_syst },
   { "type", FTPd_type },
   { "user", FTPd_user },
   { "xcwd", FTPd_cd    },        
   { "xmkd", FTPd_mkdir },       
   { "xpwd", FTPd_pwd   },       
   { "xrmd", FTPd_rmdir },       
   { NULL,   NULL } 
};

