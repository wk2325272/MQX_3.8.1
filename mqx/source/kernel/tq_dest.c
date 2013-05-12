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
* $FileName: tq_dest.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for destroying a task queue.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _taskq_destroy
* Returned Value   : MQX_OK or a task error code
* Comments         :
*   This function destroys the task queue
*
*END*----------------------------------------------------------------------*/

_mqx_uint _taskq_destroy
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

   _KLOGE2(KLOG_taskq_destroy, users_task_queue_ptr);

#if MQX_CHECK_ERRORS
   if (task_queue_ptr == NULL){
      _int_enable();
      _KLOGX2(KLOG_taskq_destroy, MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   } /* Endif */
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_taskq_destroy, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   }/* Endif */
#endif

   _int_disable();
#if MQX_CHECK_VALIDITY
   if (task_queue_ptr->VALID != TASK_QUEUE_VALID)  {
      _int_enable();
      _KLOGX2(KLOG_taskq_destroy, MQX_INVALID_TASK_QUEUE);
      return(MQX_INVALID_TASK_QUEUE);
   } /* Endif */
#endif

   task_queue_ptr->VALID = 0;
   while (_QUEUE_GET_SIZE(&task_queue_ptr->TD_QUEUE)) {
      _QUEUE_DEQUEUE(&task_queue_ptr->TD_QUEUE, td_ptr);
      _TASK_READY(td_ptr, kernel_data);
      _int_enable();
      _int_disable();
   } /* Endwhile */

   _QUEUE_REMOVE(&kernel_data->KERNEL_TASK_QUEUES,task_queue_ptr);
   
   _int_enable();

   _CHECK_RUN_SCHEDULER(); /* Let higher priority task run */

   _mem_free(task_queue_ptr);

   _KLOGX2(KLOG_taskq_destroy, MQX_OK);
   return(MQX_OK);
      
} /* Endbody */

/* EOF */
