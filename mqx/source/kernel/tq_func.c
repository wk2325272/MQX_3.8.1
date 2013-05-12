/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: tq_func.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for suspending a task on 
*   a task queue, and for resuming a task(s) from a task queue.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _taskq_suspend
* Returned Value   : MQX_OK or a task error code
* Comments         :
*   This function suspends the current task onto the specified
* task queue.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _taskq_suspend
   (
      /* [IN] the task queue handle */
      pointer users_task_queue_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   register TD_STRUCT_PTR          td_ptr;
   register TASK_QUEUE_STRUCT_PTR  task_queue_ptr = 
      (TASK_QUEUE_STRUCT_PTR)users_task_queue_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_taskq_suspend, users_task_queue_ptr);

#if MQX_CHECK_ERRORS
   if (task_queue_ptr == NULL){
      _KLOGX2(KLOG_taskq_suspend, MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   } /* Endif */
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_taskq_suspend, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   }/* Endif */
#endif

   td_ptr      = kernel_data->ACTIVE_PTR;

   _INT_DISABLE();

#if MQX_CHECK_VALIDITY
   if (task_queue_ptr->VALID != TASK_QUEUE_VALID) {
      _int_enable();
      _KLOGX2(KLOG_taskq_suspend, MQX_INVALID_TASK_QUEUE);
      return(MQX_INVALID_TASK_QUEUE);
   } /* Endif */
#endif

   td_ptr->STATE = TASK_QUEUE_BLOCKED;
   _QUEUE_UNLINK(td_ptr); /* Remove task from ready to run queue */
   td_ptr->INFO = (_mqx_uint)&task_queue_ptr->TD_QUEUE;
   if (task_queue_ptr->POLICY & MQX_TASK_QUEUE_BY_PRIORITY)  {
      _sched_insert_priorityq_internal(&task_queue_ptr->TD_QUEUE, td_ptr);
   } else {
      _QUEUE_ENQUEUE(&task_queue_ptr->TD_QUEUE, td_ptr);
   } /* Endif */

   _sched_execute_scheduler_internal(); /* Let the other tasks run */

   _INT_ENABLE();

   _KLOGX2(KLOG_taskq_suspend, MQX_OK);
   return( MQX_OK );
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _taskq_resume
* Returned Value   : MQX_OK or a task error code
* Comments         :
*   This function removes a task (or all tasks) from the task queue
* and readies them for running again.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _taskq_resume
   (
      /* [IN] the task queue handle */
      pointer users_task_queue_ptr,
      
      /* [IN] TRUE if all tasks on the queue to be resumed */
      boolean all_tasks
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   register TD_STRUCT_PTR          td_ptr;
   register TASK_QUEUE_STRUCT_PTR  task_queue_ptr = 
      (TASK_QUEUE_STRUCT_PTR)users_task_queue_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_taskq_resume, users_task_queue_ptr, all_tasks);

#if MQX_CHECK_ERRORS
   if (task_queue_ptr == NULL){
      _KLOGX2(KLOG_taskq_resume, MQX_INVALID_TASK_QUEUE);
      return(MQX_INVALID_TASK_QUEUE);
   } /* Endif */
#endif

   _INT_DISABLE();

#if MQX_CHECK_VALIDITY
   if (task_queue_ptr->VALID != TASK_QUEUE_VALID) {
      _int_enable();
      _KLOGX2(KLOG_taskq_resume, MQX_INVALID_TASK_QUEUE);
      return(MQX_INVALID_TASK_QUEUE);
   } /* Endif */
#endif

   if (_QUEUE_GET_SIZE(&task_queue_ptr->TD_QUEUE) == 0) {
      /* Task queue is empty */
      _int_enable();
      _KLOGX2(KLOG_taskq_resume, MQX_TASK_QUEUE_EMPTY);
      return(MQX_TASK_QUEUE_EMPTY);
   } /* Endif */

   if (all_tasks) {
      while (_QUEUE_GET_SIZE(&task_queue_ptr->TD_QUEUE)) {
         _QUEUE_DEQUEUE(&task_queue_ptr->TD_QUEUE, td_ptr);
         _TASK_READY(td_ptr, kernel_data);
      } /* Endwhile */
   } else {
      _QUEUE_DEQUEUE(&task_queue_ptr->TD_QUEUE, td_ptr);
      _TASK_READY(td_ptr, kernel_data);
   } /* Endif */

   _INT_ENABLE();      

   _CHECK_RUN_SCHEDULER();/* Let higher priority task run */

   _KLOGX2(KLOG_taskq_resume, MQX_OK);
   return( MQX_OK );
   
} /* Endbody */

/* EOF */
