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
* $FileName: ta_rest.c$
* $Version : 3.8.18.3$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for restarting a task.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_TD_HAS_TASK_TEMPLATE_PTR

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _task_restart_func
* Returned Value   : _mqx_uint error code
* Comments         :
*   Restart the task specified by the given task-id (the victim).
*   All of the victim's resources are released, specifically,
*   all queues closed, and all memory is freed.
*   Component cleanup functions are called to free any component
*   resources owned by this task.
*
*END*----------------------------------------------------------------------*/

static _mqx_uint _task_restart_func
   (
      /* [IN] the task id of the task to restart */
      _task_id    task_id,

      /* [IN] pointer to new task parameter if not NULL */
      uint_32_ptr param_ptr,

      /*
      ** [IN] whether the task should be restarted in the
      ** blocked state or not
      */
      boolean     blocked

   )
{ /* Body */
   PSP_STACK_START_STRUCT_PTR  stack_start_ptr;
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   TASK_TEMPLATE_STRUCT_PTR    template_ptr;
   TD_STRUCT_PTR               victim_ptr;
   TD_STRUCT_PTR               td_ptr;
   READY_Q_STRUCT_PTR          ready_q_ptr;
   char_ptr                    stack_ptr;
   pointer                     block_ptr;
   uint_32                     create_param;
   _processor_number           processor;
   _mem_size                   stack_size;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_task_restart, task_id);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      /* Cannot be called from an ISR */
      _KLOGX2(KLOG_task_restart, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

/* Start CR 1902 */
   if (task_id != MQX_NULL_TASK_ID) {
      processor = PROC_NUMBER_FROM_TASKID(task_id);
      if (processor != (_processor_number)kernel_data->INIT.PROCESSOR_NUMBER ) {
#if MQX_IS_MULTI_PROCESSOR
         if ( kernel_data->IPC != NULL ) {
            _KLOGX2(KLOG_task_restart, MQX_OK);
            return( (*kernel_data->IPC)(FALSE, processor,
               KERNEL_MESSAGES, IPC_TASK_RESTART, 3, (_mqx_uint)task_id,
               (_mqx_uint)param_ptr, (_mqx_uint)blocked));
         } else {
#endif
            _KLOGX2(KLOG_task_restart, MQX_INVALID_TASK_ID);
            return(MQX_INVALID_TASK_ID);
#if MQX_IS_MULTI_PROCESSOR
         } /* Endif */
#endif
      }/* Endif */
   }/* Endif */

   victim_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);

#if MQX_CHECK_ERRORS
   if (victim_ptr == NULL) {
      _KLOGX2(KLOG_task_restart, MQX_INVALID_TASK_ID);
      return(MQX_INVALID_TASK_ID);
   } /* Endif */

   if (victim_ptr == SYSTEM_TD_PTR(kernel_data)) {
      _KLOGX2(KLOG_task_restart, MQX_INVALID_TASK_ID);
      return(MQX_INVALID_TASK_ID);
   } /* Endif */
#endif
/* End CR 1902 */

   /* First, serialize task creation/destruction/restart */
   _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   _int_disable();
   /* remove the victim from any queues it may be in. */
   if (victim_ptr->STATE == READY) {
      if (victim_ptr != kernel_data->ACTIVE_PTR) {
         /* Remove other task from ready queue */
         _QUEUE_UNLINK(victim_ptr);
      } /* Endif */
   } else if (victim_ptr->STATE & IS_ON_TIMEOUT_Q) {
      /* Remove from time queue */
      _TIME_DEQUEUE(victim_ptr, kernel_data);
   } else if (victim_ptr->STATE & TD_IS_ON_QUEUE) {
      /* Remove from queue, where suspended
      ** eg, MUTEX_BLOCKED, IO_BLOCKED, TASKQ_BLOCKED
      */
      _QUEUE_REMOVE(victim_ptr->INFO, victim_ptr);
   } /* Endif */

   if (victim_ptr->STATE & TD_IS_ON_AUX_QUEUE) {
      /*
      ** Remove from queue, where suspended
      ** eg, LWSEM_BLOCKED
      ** (uses AUX_QUEUE field)
      */
      _QUEUE_REMOVE(victim_ptr->INFO, &victim_ptr->AUX_QUEUE);
   } /* Endif */

   victim_ptr->STATE   = DYING;

#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
   /* Stop Floating point context monitoring */
   if (kernel_data->FP_ACTIVE_PTR == victim_ptr) {
      kernel_data->FP_ACTIVE_PTR = NULL;
   }/* Endif */
#endif

   _int_enable();

#if MQX_COMPONENT_DESTRUCTION
   {
      _mqx_uint i;
      for (i = 0; i < MAX_KERNEL_COMPONENTS; ++i) {
         if (kernel_data->COMPONENT_CLEANUP[i] != NULL) {
            (*kernel_data->COMPONENT_CLEANUP[i])(victim_ptr);
         } /* Endif */
      } /* Endfor */
   }
#endif

#if MQX_IO_COMPONENT_DESTRUCTION
   /* Call I/O component cleanup functions */
   {
      _mqx_uint i;
	  for (i = 0; i < MAX_IO_COMPONENTS; ++i) {
         if (kernel_data->IO_COMPONENT_CLEANUP[i] != NULL) {
            (*kernel_data->IO_COMPONENT_CLEANUP[i])(victim_ptr);
         } /* Endif */
      } /* Endfor */
   }
#endif

   td_ptr = kernel_data->ACTIVE_PTR;
   block_ptr = _mem_get_next_block_internal(victim_ptr, NULL);
   while (block_ptr != NULL) {
       if (block_ptr == victim_ptr ||
           MEM_TYPE_TD == GET_MEMBLOCK_TYPE(block_ptr) ||
           MEM_TYPE_TASK_STACK == GET_MEMBLOCK_TYPE(block_ptr))
       {

           block_ptr = _mem_get_next_block_internal (victim_ptr, block_ptr);
           if (block_ptr == victim_ptr)
               break;
       } else {
           _mem_transfer_td_internal(block_ptr, victim_ptr, td_ptr);
           _mem_free(block_ptr);
           block_ptr = _mem_get_next_block_internal (victim_ptr, NULL);
       }
   }

   /* Free up create/destroy/restart for other tasks */
   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   template_ptr    = victim_ptr->TASK_TEMPLATE_PTR;
   stack_start_ptr = _psp_get_stack_start(victim_ptr);
   if (param_ptr == NULL) {
      create_param = stack_start_ptr->PARAMETER;
   } else {
      create_param = *param_ptr;
   } /* Endif */

   stack_ptr = victim_ptr->STACK_ALLOC_BLOCK; // get mem allocated block for stack
   
   if (stack_ptr == (pointer)template_ptr) {
       // task has been created with temporary template, copy of template is before stack in allocated block for stack
       stack_ptr += sizeof(TASK_TEMPLATE_STRUCT);       
   }
   
   stack_size = template_ptr->TASK_STACKSIZE;
   if ( stack_size <= PSP_MINSTACKSIZE  ) {
      stack_size = PSP_MINSTACKSIZE;
   } /* Endif */

   /* Make the size a multiple of the memory alignment */
   _MEMORY_ALIGN_VAL_LARGER(stack_size);
#if 0 // we dont need this, because we using _mem_alloc_align function in _task_alloc_td_internal
#if PSP_MEMORY_ALIGNMENT
   /* But we need to add size to allow for alignment of stack base */
   stack_size += PSP_MEMORY_ALIGNMENT + 1;
#endif
#endif //0

   ready_q_ptr = kernel_data->READY_Q_LIST - template_ptr->TASK_PRIORITY;
   victim_ptr->MY_QUEUE = ready_q_ptr;
#if MQX_HAS_DYNAMIC_PRIORITIES
   victim_ptr->HOME_QUEUE  = ready_q_ptr;
#endif
   victim_ptr->TASK_SR     = ready_q_ptr->ENABLE_SR;
   victim_ptr->FLAGS       = template_ptr->TASK_ATTRIBUTES;

#if MQX_ENABLE_USER_MODE
   if (template_ptr->TASK_ATTRIBUTES & MQX_USER_TASK) {
       victim_ptr->FLAGS |= TASK_USER_MODE;
   }
#endif // MQX_ENABLE_USER_MODE
   
#if MQX_HAS_TIME_SLICE
   /* Reset the time slice back to the default */
   if (template_ptr->DEFAULT_TIME_SLICE) {
#if (MQX_DEFAULT_TIME_SLICE_IN_TICKS == 0)
      uint_32 ticks;

      ticks = ((template_ptr->DEFAULT_TIME_SLICE * 2 *
         kernel_data->TICKS_PER_SECOND) / 1000) / 2  /* Rounding.. */;
      PSP_ADD_TICKS_TO_TICK_STRUCT(&td_ptr->TIME_SLICE,
         ticks, &td_ptr->TIME_SLICE);
#else
      PSP_ADD_TICKS_TO_TICK_STRUCT(&victim_ptr->TIME_SLICE,
         template_ptr->DEFAULT_TIME_SLICE, &victim_ptr->TIME_SLICE);
#endif
   } else {
      victim_ptr->TIME_SLICE = kernel_data->SCHED_TIME_SLICE;
   } /* Endif */
   /* Reset scheduling policy for task */
   if (kernel_data->SCHED_POLICY == MQX_SCHED_RR) {
      victim_ptr->FLAGS |= MQX_TIME_SLICE_TASK;
   } /* Endif */
#endif

#if MQX_USE_IO
   /* Reset the io streams back to the default */
   victim_ptr->STDIN_STREAM  = kernel_data->PROCESSOR_STDIN;
   victim_ptr->STDOUT_STREAM = kernel_data->PROCESSOR_STDOUT;
   victim_ptr->STDERR_STREAM = kernel_data->PROCESSOR_STDERR;
#endif

   _int_disable();

   if (victim_ptr == td_ptr) {
      _QUEUE_UNLINK(victim_ptr);  /* Remove victim from the queue */
   } /* Endif */

   /* Rebuild the task's initial context */
   if (FALSE == _psp_build_stack_frame(victim_ptr, stack_ptr, stack_size, template_ptr, victim_ptr->TASK_SR, create_param)) {
      // build stack failed
      return MQX_OUT_OF_MEMORY;
   }

   if (blocked) {
      victim_ptr->STATE = BLOCKED;
   } else {
      _task_ready_internal(victim_ptr);
   } /* Endif */

   victim_ptr->DISABLED_LEVEL = 0;

   if (victim_ptr == td_ptr) {
      /*
      ** The active task is to be restarted.
      ** Will not return from here
      */
      _sched_run_internal();
   } /* Endif */

   /* The victim has been added to the ready Q. Preemption check must be made */
   if (kernel_data->CURRENT_READY_Q != td_ptr->MY_QUEUE) {
      _sched_execute_scheduler_internal();
   } /* Endif */

   _int_enable();

   _KLOGX2(KLOG_task_restart, MQX_OK);
   return(MQX_OK);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _task_restart
* Returned Value   : _mqx_uint error code
* Comments         :
*   Restart the task specified by the given task-id (the victim).
*   Reserve stack for Stack Start Structure and call the _task_restart_func().
*
*END*----------------------------------------------------------------------*/
_mqx_uint _task_restart
   (
      /* [IN] the task id of the task to restart */
      _task_id    task_id,

      /* [IN] pointer to new task parameter if not NULL */
      uint_32_ptr param_ptr,

      /*
      ** [IN] whether the task should be restarted in the
      ** blocked state or not
      */
      boolean     blocked

   )
{ /* Body */
   /* reserve memory in stack */
   volatile char dummy_stack_start_struct[sizeof(PSP_STACK_START_STRUCT)];
   dummy_stack_start_struct[0] = 0x00;

   /* call the right _task_restart function */
   return(_task_restart_func(task_id, param_ptr, blocked));
}

#endif
/* EOF */
