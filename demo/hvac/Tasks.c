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
* $FileName: Tasks.c$
* $Version : 3.4.8.0$
* $Date    : Sep-17-2009$
*
* Comments:
*
*   
*
*END************************************************************************/


#include "HVAC.h"


/*
** MQX initialization information
*/

const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
   /* Task Index,   Function,         Stack,  Priority, Name,       Attributes,             Param,  Time Slice */
    { HVAC_TASK,    HVAC_Task,         1400,    9,      "HVAC",     MQX_AUTO_START_TASK,    0,      0           },
#if DEMOCFG_ENABLE_SWITCH_TASK
    { SWITCH_TASK,  Switch_Task,        800,   10,      "Switch",   MQX_AUTO_START_TASK,    0,      0           },
#endif
#if DEMOCFG_ENABLE_SERIAL_SHELL
    { SHELL_TASK,   Shell_Task,        2500,   12,      "Shell",    MQX_AUTO_START_TASK,    0,      0           },
#endif
#if DEMOCFG_ENABLE_AUTO_LOGGING
    { LOGGING_TASK, Logging_task,      2500,   11,      "Logging",                    0,    0,      0           },
#endif
#if DEMOCFG_ENABLE_USB_FILESYSTEM
    { USB_TASK,     USB_task,         2200L,   8L,      "USB",      MQX_AUTO_START_TASK,    0,      0           },
#endif
    { ALIVE_TASK,   HeartBeat_Task,    1500,   10,      "HeartBeat",                  0,    0,      0           },
    {0}
};



 
/* EOF */
