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
* $FileName: ta_prio.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   task priority of a task.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_DYNAMIC_PRIORITIES
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_priority
* Returned Value   : none
* Comments         :
*   This function returns the priority of the specified task
*
*END*----------------------------------------------------------------------*/

_mqx_uint _task_get_priority
   (
      /* [IN] the task id to use */
      _task_id     task_id,

      /* [OUT] the location where the task priority is to be placed */
      _mqx_uint_ptr priority_ptr
   )
{ /* Body */
   TD_STRUCT_PTR           td_ptr;
   
   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if (td_ptr == NULL) {
      return(MQX_INVALID_TASK_ID);
   } /* Endif */
   *priority_ptr = td_ptr->HOME_QUEUE->PRIORITY;
   return MQX_OK;
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_set_priority
* Returned Value   : none
* Comments         :
*   This function sets the priority of the specified task
*
*END*----------------------------------------------------------------------*/

_mqx_uint _task_set_priority
   (
      /* [IN] the task id to use */
      _task_id     task_id,

      /* [IN] the new task priority */
      _mqx_uint     new_priority,

      /* [OUT] the location where the old task priority is to be placed */
      _mqx_uint_ptr priority_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   READY_Q_STRUCT_PTR      ready_q_ptr;
   TD_STRUCT_PTR           td_ptr;
   TASK_QUEUE_STRUCT_PTR   task_queue_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_task_set_priority, task_id, new_priority);

#if MQX_CHECK_ERRORS
   if (new_priority > kernel_data->LOWEST_TASK_PRIORITY) {
      _KLOGX2(KLOG_task_set_priority, MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   }/* Endif */
#endif

   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if (td_ptr == NULL) {
      _KLOGX2(KLOG_task_set_priority, MQX_INVALID_TASK_ID);
      return(MQX_INVALID_TASK_ID);
   } /* Endif */

   _int_disable();
   /* Return old priority */
   *priority_ptr = td_ptr->HOME_QUEUE->PRIORITY;

   /* Make the change permanent */
   ready_q_ptr = kernel_data->READY_Q_LIST;
   td_ptr->HOME_QUEUE = ready_q_ptr - new_priority;

   if (td_ptr->BOOSTED) {
      /* Can only change priority to a higher (lower value) */
      if (new_priority < td_ptr->MY_QUEUE->PRIORITY) {
         /* Move the task to the correct priority level */
         _sched_set_priority_internal(td_ptr, new_priority);
      } /* Endif */
   } else {
      /* Move the task to the correct priority level */
      _sched_set_priority_internal(td_ptr, new_priority);
   } /* Endif */

   if (td_ptr->STATE == TASK_QUEUE_BLOCKED) {
      task_queue_ptr = (TASK_QUEUE_STRUCT_PTR)
         ((uchar_ptr)td_ptr->INFO - FIELD_OFFSET(TASK_QUEUE_STRUCT, TD_QUEUE));
      if (task_queue_ptr->POLICY & MQX_TASK_QUEUE_BY_PRIORITY) {
         /* Requeue the td by priority */
         _QUEUE_REMOVE(&task_queue_ptr->TD_QUEUE, td_ptr);
         _sched_insert_priorityq_internal(&task_queue_ptr->TD_QUEUE, 
            td_ptr);
      }/* Endif */
   }/* Endif */
   
   /* Allow higher priority tasks to run */
   _CHECK_RUN_SCHEDULER(); 

   _int_enable();

   _KLOGX2(KLOG_task_set_priority, MQX_OK);
   return MQX_OK;
   
} /* Endbody */

#endif
/* EOF */
