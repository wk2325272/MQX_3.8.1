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
* $FileName: webserver_demo.c$
* $Version : 3.8.26.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <ipcfg.h>
#include <httpd.h>
#include <httpd_types.h>
#include <tfs.h>
#include "security_private.h"
#include "security_public.h"
#include "security.h"


#if !BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if !ENABLE_ADC
#error This application requires BSPCFG_ENABLE_ADC defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

extern const HTTPD_CGI_LINK_STRUCT cgi_lnk_tbl[];
extern const TFS_DIR_ENTRY tfs_data[];

enum {
   MAIN_TASK=1,
   IO_TASK
};

void Main_Task(uint_32);
void io_task(uint_32);




#if PSP_MQX_CPU_IS_MCF51CN

const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {MAIN_TASK,     Main_Task,  1700,     8,        "Main",     MQX_AUTO_START_TASK,    0,      0           },
    {IO_TASK,       io_task,     500,     7,        "IO poll",  0,                      0,      0           },
    {0}
};

#else // PSP_MQX_CPU_IS_MCF51CN

const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {MAIN_TASK,     Main_Task,  3000,     8,        "Main",     MQX_AUTO_START_TASK,    0,      0           },
    {IO_TASK,       io_task,    1000,     7,        "IO poll",  0,                      0,      0           },
    {0}
};

#endif // PSP_MQX_CPU_IS_MCF51CN



const HTTPD_ROOT_DIR_STRUCT root_dir[] = {
    { "", "tfs:" },
    { 0, 0 }
};



void Main_Task(uint_32 initial_data)
{
   HTTPD_STRUCT *    server;
   int_32            error;

   SEC_Initialize();
 
   _task_create(0, IO_TASK, 0); // start io task

   SEC_InitializeNetworking(5, 6, 4, DEMOCFG_ENABLE_DHCP);

   /* Set up Trival File System to server webpages */            
   if ((error = _io_tfs_install("tfs:", tfs_data))) 
   {
      printf("\nTFS install returned: %08x\n", error);
   }

    server = httpd_server_init((HTTPD_ROOT_DIR_STRUCT*)root_dir, "\\mqx.html");
    HTTPD_SET_PARAM_CGI_TBL(server, (HTTPD_CGI_LINK_STRUCT*)cgi_lnk_tbl);
    
#if HTTPDCFG_POLL_MODE
    while (1) {
          /* Poll Webserver */
          httpd_server_poll(server, -1);
    }
#else
    httpd_server_run(server);
    _task_block();
#endif    
}

/* Security task */
void io_task(uint_32 temp) {
   while (1) {
      Input_Poll();
      _time_delay(200);
   }
}
