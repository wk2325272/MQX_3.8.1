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
* $Version : 3.8.12.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include <errno.h>
#include <mutex.h>
#include <sem.h>
#include <event.h>
#include <log.h>
#include "demo.h"

#if PSP_MQX_CPU_IS_MCF51
   #define MAIN_STACK 900
   #define STACK_SIZE 590
   #define SEMA_STACK 380    
#else
   #define STACK_SIZE 1024
   #define MAIN_STACK STACK_SIZE
   #define SEMA_STACK STACK_SIZE
#endif   


/*   Task Templates  */ 
const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{   
  /* Task Index,    Function,       Stack,       Priority, Name,             Attributes,                                Param,  Time Slice */
   { MUTEXB,        MutexB,         STACK_SIZE,   9,      "MutexB",         0,                                         0,      0 },
   { MUTEXA,        MutexA,         STACK_SIZE,   9,      "MutexA",         0,                                         0,      0 },
   { SEMB,          SemB,           STACK_SIZE,  10,      "SemB",           0,                                         0,      0 },
   { SEMA,          SemA,           SEMA_STACK,   9,      "SemA",           0,                                         0,      0 },
   { EVENTB,        EventB,         STACK_SIZE,   9,      "EventB",         0,                                         0,      0 },
   { EVENTA,        EventA,         STACK_SIZE,   9,      "EventA",         0,                                         0,      0 },
   { BTIMESLICETASK,BTimeSliceTask, STACK_SIZE,   9,      "BTimeSliceTask", MQX_AUTO_START_TASK,                       0,      0 },
   { ATIMESLICETASK,ATimeSliceTask, STACK_SIZE,   9,      "ATimeSliceTask", (MQX_TIME_SLICE_TASK|MQX_AUTO_START_TASK), 0,      0 },
   { SENDER,        Sender,         STACK_SIZE,  10,      "Sender",         0,                                         0,      0 },
   { RESPONDER,     Responder,      STACK_SIZE,   9,      "Responder",      0,                                         0,      0 },
   { MAIN_TASK,     main_task,      MAIN_STACK,  11,      "main_task",      MQX_AUTO_START_TASK,                       0,      0 },
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
