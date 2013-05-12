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
* $FileName: tq_test.c$
* $Version : 3.6.4.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the function for testing all task queues
*   in the system.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _taskq_test
* Returned Value   : MQX_OK or an error code
* Comments         :
*   This function tests all task queues for correctness
*
*END*----------------------------------------------------------------------*/

_mqx_uint _taskq_test
   ( 
      /* [OUT] the task queue in error */
      pointer _PTR_ task_queue_error_ptr,

      /* [OUT] the td on a task queue in error */
      pointer _PTR_ td_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TASK_QUEUE_STRUCT_PTR  task_queue_ptr;
   _mqx_uint                queue_size;
   _mqx_uint                result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_taskq_test, task_queue_error_ptr, td_error_ptr);

   *td_error_ptr = NULL;
   *task_queue_error_ptr = NULL;

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_taskq_test, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   }/* Endif */
#endif

   task_queue_ptr = (TASK_QUEUE_STRUCT_PTR)
      ((pointer)kernel_data->KERNEL_TASK_QUEUES.NEXT);
   if (_QUEUE_GET_SIZE(&kernel_data->KERNEL_TASK_QUEUES) == 0) {
      _KLOGX2(KLOG_taskq_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   _int_disable();

   result = _queue_test((QUEUE_STRUCT_PTR)&kernel_data->KERNEL_TASK_QUEUES,
      task_queue_error_ptr);
   if (result != MQX_OK) {
      _int_enable();
      _KLOGX3(KLOG_taskq_test, result, *task_queue_error_ptr);
      return(result);
   } /* Endif */

   queue_size = _QUEUE_GET_SIZE(&kernel_data->KERNEL_TASK_QUEUES);
   while (queue_size--) {
      if (task_queue_ptr->VALID != TASK_QUEUE_VALID) {
         result = MQX_INVALID_TASK_QUEUE;
         break;
      } /* Endif */

      result = _queue_test(&task_queue_ptr->TD_QUEUE, td_error_ptr);
      if (result != MQX_OK) {
         break;
      } /* Endif */
      
      task_queue_ptr = task_queue_ptr->NEXT;
   } /* Endwhile */

   _int_enable();

   if (result != MQX_OK) {
      *task_queue_error_ptr = (pointer)task_queue_ptr;
   } /* Endif */
   _KLOGX4(KLOG_taskq_test, result, *task_queue_error_ptr, *td_error_ptr);

   return(result);
   
} /* Endbody */

/* EOF */
