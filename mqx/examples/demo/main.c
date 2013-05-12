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
* $Version : 3.8.13.0$
* $Date    : Oct-4-2011$
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
#if MQX_KERNEL_LOGGING
#include <klog.h>
#endif
#include "demo.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if ! MQX_HAS_TIME_SLICE
#error This application requires MQX_HAS_TIME_SLICE defined non-zero in user_config.h. Please recompile PSP and BSP with this option.
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
   MESSAGE_HEADER_STRUCT_PTR msg_ptr;
   _task_id                  created_task;
   _mqx_uint                 log_result;
   _mqx_uint                 event_result;
   _mqx_uint                 sem_result;

_int_install_unexpected_isr();
   printf("\nMQX %s\n",_mqx_version);
   printf("Hello from main_task().\n");

   /* create the log component */
   log_result = _log_create_component();
   if (log_result != MQX_OK) { 
      /* log component could not be created */
   } /* endif */
   /* Create the mutex component */
   if (_mutex_create_component() != MQX_OK) {
      /* an error has been detected */
   }

   /* create the event component */
   event_result = _event_create_component(EVENT_INITIAL_NUMBER, EVENT_GROWTH, 
      EVENT_MAXIMUM);
   if (event_result != MQX_OK) { 
      /* event component could not be created */
      printf("Error: Cannot create event component\n");
      _task_block();
   } /* endif */

   /* create the semaphore component */
   sem_result = _sem_create_component(SEM_INITIAL_NUMBER, SEM_GROWTH, 
      SEM_MAXIMUM);
   if (sem_result != MQX_OK) { 
      /* semaphore component could not be created */
      printf("Error: Cannot create semaphore component\n");
      _task_block();
   } /* endif */
   MsgPool_pool_id = _msgpool_create ( 8, 10, 0, 0);
   if (MsgPool_pool_id == MSGPOOL_NULL_POOL_ID) { 
      /* _msgpool_create did not succeed */ 
      printf("Error: Cannot create message pool\n");
      _task_block();
   } 
   Main_Queue_qid = _msgq_open( MSGQ_FREE_QUEUE, SIZE_UNLIMITED);
   if (Main_Queue_qid == (_queue_id)0){
         /* queue could not be opened */
      printf("Error: Cannot open message pool\n");
      _task_block();
   }
   created_task = _task_create(0, SENDER, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }
   created_task = _task_create(0, MUTEXA, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }
   created_task = _task_create(0, MUTEXB, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }
   created_task = _task_create(0, SEMA, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }
   created_task = _task_create(0, SEMB, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }
   created_task = _task_create(0, EVENTA, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }
   created_task = _task_create(0, EVENTB, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
      printf("Error: Cannot create task\n");
      _task_block();
   }

#if MQX_KERNEL_LOGGING
   /* create log number 0 */
   log_result = _klog_create(200, 0);
   if (log_result != MQX_OK) { 
      /* log 0 could not be created */
      printf("Error: Cannot create kernel log\n");
      _task_block();
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
      msg_ptr = _msg_alloc((_pool_id) MsgPool_pool_id );
      if (msg_ptr == NULL) { 
         /* No available message buffer */ 
      } 
      msg_ptr->SIZE = sizeof(MESSAGE_HEADER_STRUCT);
      msg_ptr->SOURCE_QID = msg_ptr->TARGET_QID;
      msg_ptr->TARGET_QID = Sender_Queue_qid;
      _msgq_send(msg_ptr);

      /*
      * Service the message queue - Main_Queue
      */
      msg_ptr = _msgq_receive_ticks(Main_Queue_qid, NO_TIMEOUT);
      /* process message End_msg */
      _msg_free(msg_ptr);

   } /* endwhile */ 
} /*end of task*/

/* End of File */
