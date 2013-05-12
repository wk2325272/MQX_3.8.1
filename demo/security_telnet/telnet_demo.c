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
* $FileName: telnet_demo.c$
* $Version : 3.8.24.0$
* $Date    : Sep-20-2011$
*
* Comments:
*
*   Security telnet example using RTCS Library.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <enet.h>
#include <ipcfg.h>
#include <shell.h> 

#include "lw_telnet.h"
#include "security.h"
    
#include "security_private.h"
#include "security_public.h"
#include "Security_Shell_Commands.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#if ! ENABLE_ADC
#error This application requires BSPCFG_ENABLE_ADC or BSPCFG_ENABLE_ADC1 defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

/*
** MQX initialization information
*/
void main_task(uint_32);
void io_task(uint_32);

enum {
   MAIN_TASK=1,
   IO_TASK
};


#if PSP_MQX_CPU_IS_MCF51CN
const TASK_TEMPLATE_STRUCT MQX_template_list[] = {
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {MAIN_TASK,     main_task,  1000,     8,        "Telnet",   MQX_AUTO_START_TASK,    0,      0           },
    {IO_TASK,       io_task,     500,     7,        "IO poll",  0,                      0,      0           },
    {0}
};
#else
const TASK_TEMPLATE_STRUCT MQX_template_list[] = {
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {MAIN_TASK,     main_task,  3000,     8,        "Telnet",   MQX_AUTO_START_TASK,    0,      0           },
    {IO_TASK,       io_task,    1000,     7,        "IO poll",  0,                      0,      0           },
    {0}
};
#endif
     

void main_task( uint_32 temp) {
      
   // Initialize Security application
   SEC_Initialize(); 

    /* Start io task */
    _task_create(0, IO_TASK, 0); 

   // Initialize TCP/IP
   SEC_InitializeNetworking(3, 3, 3, DEMOCFG_ENABLE_DHCP);
      
   // call lw_telnet_server to become the telnet server 
   while (1) {  
        lw_telnet_server(Telnetd_shell_fn);
    }
}

void io_task(uint_32 temp) {
    while (1) {
        //Read in switch, potentiometer, and accelerometer inputs
        Input_Poll();
        _time_delay(200);
    }
}
