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
* $FileName: ta_creas.c$
* $Version : 3.8.6.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for creating a task with the
*   stack location specified.
*
*END************************************************************************/

#include "mqx_inc.h"

/* 
** This variable has no use to MQX. 
** Its been created for Task Aware Debug module.
*/
volatile uint_32 _tad_task_at_flag;

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _task_create_at
* Returned Value   : task ID of the created task or NULL on error
* Comments         :
*    This function will create a new task of the type specified by the
*    task template number, puting the TD and Stack at the specified location.
*
*END*----------------------------------------------------------------------*/

_task_id _task_create_at
   (
      /* [IN] the processor upon which to create the task */
      _processor_number processor_number,

      /* [IN] the task template index number for this task */
      _mqx_uint         template_index,

      /* [IN] the parameter to pass to the newly created task */
      uint_32           parameter,

      /* [IN] the location where the stack and TD are to be created */
      pointer           stack_ptr,

      /* [IN] the size of the stack */
      _mem_size         stack_size
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   TD_STRUCT_PTR            td_ptr;
   _task_id                 result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE5(KLOG_task_create_at, processor_number, template_index, parameter,
      stack_ptr);

#if MQX_CHECK_ERRORS
   if (template_index & SYSTEM_TASK_FLAG) {
      _task_set_error(MQX_INVALID_TEMPLATE_INDEX);
      _KLOGX3(KLOG_task_create_at, MQX_NULL_TASK_ID,
         MQX_INVALID_TEMPLATE_INDEX);
      return MQX_NULL_TASK_ID;
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (processor_number != 0 ) 
   {
      _task_set_error(MQX_INVALID_PROCESSOR_NUMBER);
      _KLOGX3(KLOG_task_create_at, MQX_NULL_TASK_ID,
         MQX_INVALID_PROCESSOR_NUMBER);
      return MQX_NULL_TASK_ID;
   } /* Endif */
#endif
   
   td_ptr = _task_build_internal(template_index, parameter, stack_ptr, stack_size, FALSE);
   if (td_ptr != NULL) {
      result = td_ptr->TASK_ID;
      _INT_DISABLE();
      _task_ready_internal(td_ptr);
      _INT_ENABLE();
      _CHECK_RUN_SCHEDULER(); /* Let a higher priority task run */
   } else {
      result = MQX_NULL_TASK_ID;
   }/* Endif */

   _KLOGX3(KLOG_task_create_at, result,
      kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);

   _tad_task_at_flag++;
   
   return(result);

} /* Endbody */

/* EOF */
