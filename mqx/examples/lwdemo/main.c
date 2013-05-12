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
* $FileName: main.c$
* $Version : 3.6.12.0$
* $Date    : Jun-14-2010$
*
* Comments:
*
*   This file contains source for the Lightweight MQX demo test.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include <errno.h>
#include <lwevent.h>
#include <lwmsgq.h>
#if MQX_KERNEL_LOGGING
#include <klog.h>
#endif
#if MQX_USE_LOGS
#include <log.h>
#endif
#include "lwdemo.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/*   Task Code -  Main     */

/*TASK---------------------------------------------------------------
*   
* Task Name   :  Main
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void main_task
   (
      uint_32   parameter
   )
{
   _mqx_uint                 msg[MSG_SIZE];
   _task_id                  created_task;
#if MQX_USE_LOGS || MQX_KERNEL_LOGGING
   _mqx_uint                 log_result;
#endif
   _mqx_uint                 result;

   _int_install_unexpected_isr();
   printf("\nMQX %s\n",_mqx_version);
   printf("Hello from main_task().");

#if MQX_USE_LOGS
   /* create the log component */
   log_result = _log_create_component();
   if (log_result != MQX_OK) { 
      printf("Error: log component could not be created\n");
   } /* endif */
#endif
   /* create lwevent group */
   result = _lwevent_create(&lwevent,0);
   if (result != MQX_OK) { 
      printf("Error: event component could not be created\n");
   } /* endif */
   
   /* create a lwsem */
   result = _lwsem_create(&lwsem, 10);
   if (result != MQX_OK) {
      /* semaphore component could not be created */
   } /* endif */

   _lwmsgq_init((pointer)main_queue, NUM_MESSAGES, MSG_SIZE);
   _lwmsgq_init((pointer)sender_queue, NUM_MESSAGES, MSG_SIZE);
   _lwmsgq_init((pointer)responder_queue, NUM_MESSAGES, MSG_SIZE);

   created_task = _task_create(0, SENDER, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      printf("Error: SENDER task creation failed\n");
   }
   created_task = _task_create(0, LWSEMA, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      printf("Error: LWSEMA task creation failed\n");
   }
   created_task = _task_create(0, LWSEMB, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      printf("Error: LWSEMB task creation failed\n");
   }
   created_task = _task_create(0, LWEVENTA, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      printf("Error: LWEVENTA task creation failed\n");
   }
   created_task = _task_create(0, LWEVENTB, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      printf("Error: LWEVENTB task creation failed\n");
   }

#if MQX_KERNEL_LOGGING == 1
   /* create log number 0 */
   log_result = _klog_create(200, 0);
   if (log_result != MQX_OK) { 
      printf("Error: log 0 could not be created\n");
   } /* endif */

   /* define kernel logging */
   _klog_control(0xFFFFFFFF, FALSE);
   _klog_control(
      KLOG_ENABLED                  |
      KLOG_FUNCTIONS_ENABLED        |
      KLOG_INTERRUPTS_ENABLED       |
      KLOG_SYSTEM_CLOCK_INT_ENABLED |
      KLOG_CONTEXT_ENABLED          |
      KLOG_TASKING_FUNCTIONS        |
      KLOG_ERROR_FUNCTIONS          |
      KLOG_MESSAGE_FUNCTIONS        |
      KLOG_INTERRUPT_FUNCTIONS      |
      KLOG_MEMORY_FUNCTIONS         |
      KLOG_TIME_FUNCTIONS           |
      KLOG_EVENT_FUNCTIONS          |
      KLOG_NAME_FUNCTIONS           |
      KLOG_MUTEX_FUNCTIONS          |
      KLOG_SEMAPHORE_FUNCTIONS      |
      KLOG_WATCHDOG_FUNCTIONS, 
      TRUE
      );
#endif

   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      _lwmsgq_send((pointer)sender_queue, msg, LWMSGQ_SEND_BLOCK_ON_FULL);
      _lwmsgq_receive((pointer)main_queue, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
   } /* endwhile */ 
} /*end of task*/

/* End of File */
