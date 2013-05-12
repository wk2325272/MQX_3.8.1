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
* $FileName: ttl.c$
* $Version : 3.7.7.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains source for the Lightweight MQX demo test.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <errno.h>
#include <lwevent.h>
#include "lwdemo.h"

/*   Task Templates  */ 
const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,  Stack, Priority,  Name,        Attributes,          Param, Time Slice */
    { LWSEMB,     LWSemB,    0x300,    10,     "LWSemB",    0,                   0,     0 },
    { LWSEMA,     LWSemA,    0x300,     9,     "LWSemA",    0,                   0,     0 },
    { LWEVENTB,   LWEventB,  0x300,     9,     "LWEventB",  0,                   0,     0 },
    { LWEVENTA,   LWEventA,  0x300,     9,     "LWEventA",  0,                   0,     0 },
    { SENDER,     Sender,    0x300,    10,     "Sender",    0,                   0,     0 },
    { RESPONDER,  Responder, 0x300,     9,     "Responder", 0,                   0,     0 },
    { MAIN_TASK,  main_task, 0xC00,    11,     "main_task", MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/*   MQX Initialization  */ 
MQX_INITIALIZATION_STRUCT  MQX_init_struct = 
{ 
   /* PROCESSOR_NUMBER                */  BSP_DEFAULT_PROCESSOR_NUMBER,
   /* START_OF_KERNEL_MEMORY          */  BSP_DEFAULT_START_OF_KERNEL_MEMORY,
   /* END_OF_KERNEL_MEMORY            */  BSP_DEFAULT_END_OF_KERNEL_MEMORY,
   /* INTERRUPT_STACK_SIZE            */  BSP_DEFAULT_INTERRUPT_STACK_SIZE,
   /* TASK_TEMPLATE_LIST              */  (pointer)MQX_template_list,
   /* MQX_HARDWARE_INTERRUPT_LEVEL_MAX*/  BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX,
   /* MAX_BUFFER_POOLS                */  2, 
   /* MAX_QUEUE_NUMBER                */  12, 
   /* IO_CHANNEL                      */  BSP_DEFAULT_IO_CHANNEL,
   /* IO_OPEN_MODE                    */  BSP_DEFAULT_IO_OPEN_MODE
};


/* End of File */
