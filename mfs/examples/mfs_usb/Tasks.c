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
* $FileName: Tasks.c$
* $Version : 3.6.8.0$
* $Date    : Jun-3-2010$
*
* Comments:
*
*   
*
*END************************************************************************/


#include "MFS_USB.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if PSP_MQX_CPU_IS_MCF51JM
   /* small ram taget */
   #define SHELL_TASK_STACK_SIZE 1000
   #define USB_TASK_STACK_SIZE   1000
#else
   #define SHELL_TASK_STACK_SIZE 4000
   #define USB_TASK_STACK_SIZE   2500   
#endif   

/*
** MQX initialization information
*/

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
  /* Task Index, Function,   Stack,                   Priority,  Name,      Attributes,          Param, Time Slice */
   { 1,          Shell_Task, SHELL_TASK_STACK_SIZE,   10,        "Shell",   MQX_AUTO_START_TASK, 0,     0 },
   { 2,          USB_task,   USB_TASK_STACK_SIZE,     8,         "USB",     MQX_AUTO_START_TASK, 0,     0 },
   { 0 }
};



 
/* EOF */
