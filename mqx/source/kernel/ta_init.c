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
* $FileName: ta_init.c$
* $Version : 3.8.14.2$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the function for creating a task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_init_internal
* Returned Value   : pointer to the created task descriptor or NULL on error
* Comments         : create task for given task template
* 
*END*----------------------------------------------------------------------*/

TD_STRUCT_PTR _task_init_internal
   (
      /* [IN] the task template for the new task */
      TASK_TEMPLATE_STRUCT_PTR template_ptr,
      
      /* [IN] the task id of the creating task */
      _task_id                 creator_task_id,

      /* [IN] the creation parameter for the task */
      uint_32                  create_parameter,

/* START CR 897 */
      /* [IN] is the template ptr a dynamic template (ie not from list) */
      boolean                  dynamic,
      
      /* [IN] if not NULL, the location of the stack is provided */
      pointer                  input_stack_ptr,

      /* [IN] the input stack size if provided by the application */
      _mem_size                input_stack_size
/* END CR 897 */
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   TD_STRUCT_PTR            td_ptr;
   READY_Q_STRUCT_PTR       ready_q_ptr;
   TASK_TEMPLATE_STRUCT_PTR new_template_ptr;
#if MQX_USE_LWMEM_ALLOCATOR==0
   STOREBLOCK_STRUCT_PTR    block_ptr;
#endif
   char_ptr                 stack_ptr;
   _mqx_uint                stack_size;
   _mqx_uint                td_size; // CR 1124

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
    if (template_ptr->TASK_PRIORITY > kernel_data->LOWEST_TASK_PRIORITY)  {
#if MQX_USE_IDLE_TASK
        if (template_ptr != &kernel_data->IDLE_TASK_TEMPLATE) {
#endif

        _task_set_error(MQX_INVALID_TASK_PRIORITY);      
        return (NULL);
    
#if MQX_USE_IDLE_TASK
        }
#endif
    }
#endif

   
   /*
   ** Calculate space for the stack. Note that the stack size
   ** must be no smaller than a specified minimum, and it
   ** is made to be a multiple of longwords. The space allocated
   ** for the stack includes the specified stacksize and the
   ** overhead required by the kernel.
   */
/* START CR 897 */
   if (input_stack_ptr) {
       // force stack size, ignore stack size from template 
      stack_size = input_stack_size;
#if MQX_CHECK_ERRORS
      if (stack_size <= (PSP_MINSTACKSIZE+sizeof(TD_STRUCT))){
         _task_set_error(MQX_INVALID_SIZE);
         return(NULL);
      } /* Endif */
#endif
      td_ptr = _task_alloc_td_internal(stack_size, &td_size, input_stack_ptr, template_ptr->TASK_ATTRIBUTES & MQX_USER_TASK);
      stack_size -= td_size;

      if (dynamic) stack_size -= sizeof(TASK_TEMPLATE_STRUCT);

   } else {
       // take stack size from template
      stack_size = template_ptr->TASK_STACKSIZE;
#if MQX_CHECK_ERRORS
      if ( stack_size <= PSP_MINSTACKSIZE  ) {
         stack_size = PSP_MINSTACKSIZE; 
      } /* Endif */
#endif
      /* Make the size a multiple of the memory alignment */   
      _STACK_ALIGN_VAL_LARGER(stack_size);

#if 0 // we dont need this, because we using _mem_alloc_align function in _task_alloc_td_internal
#if PSP_MEMORY_ALIGNMENT
      /* But we need to add size to allow for alignment of stack base */
      stack_size += PSP_STACK_ALIGNMENT + 1;
#endif
#endif

      /* 
      ** Allocate space for a task descriptor and stack.
      ** If there is none available, 
      ** then indicate that the create failed.
      */
      // function allocate task descriptor, stack and otionally increase stack block size by size required for task template (dynamic = nonzero)
      td_ptr = _task_alloc_td_internal(dynamic ? (stack_size + sizeof(TASK_TEMPLATE_STRUCT)) : stack_size,
         &td_size, NULL, template_ptr->TASK_ATTRIBUTES & MQX_USER_TASK);
   }/* Endif */

/* END CR 897 */

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (td_ptr == NULL) {
      _task_set_error(MQX_OUT_OF_TASK_DESCRIPTORS);
      return (NULL);
   } /* Endif */
#endif
   td_ptr->STATE = BLOCKED;

   // get begin of new stack
   if (td_ptr->STACK_ALLOC_BLOCK) {
       // (stack has been allocated)
       // stack start in new allocated block
       stack_ptr = td_ptr->STACK_ALLOC_BLOCK;
   }
   else {
       // (this will be executed if user provide input_stack_size)
       // stack start in provided block behind TD
       stack_ptr = (char_ptr)td_ptr + td_size;
   }

   if (dynamic) {
       // task is creating from temporary template, store template before stack (in stack allocated memory)
       new_template_ptr = (pointer)stack_ptr;
       stack_ptr += sizeof(TASK_TEMPLATE_STRUCT);
       *new_template_ptr = *template_ptr;
       template_ptr = new_template_ptr;
       template_ptr->TASK_TEMPLATE_INDEX |= SYSTEM_TASK_FLAG;
       // stack_ptr is moved behind template
   }
   
#if MQX_MONITOR_STACK
   // fill stack with mark for TAD stack overflow functionality
   _task_fill_stack_internal((_mqx_uint_ptr)stack_ptr, stack_size);
#endif

   /*  Find the ready_q that the new task belongs to.  */
   /* The ready queues are stored as a array as follows:
   **     lowest priority - eg 9
   **                          8
   **                        ...
   **     highest priority     0
   ** With the READY_Q_LIST pointer pointing to the highest priority queue 
   */
   ready_q_ptr = kernel_data->READY_Q_LIST - template_ptr->TASK_PRIORITY;
   td_ptr->MY_QUEUE = ready_q_ptr;
#if MQX_HAS_DYNAMIC_PRIORITIES
  td_ptr->HOME_QUEUE  = ready_q_ptr;
#endif   
   td_ptr->TASK_SR     = ready_q_ptr->ENABLE_SR;
   td_ptr->FLAGS       = template_ptr->TASK_ATTRIBUTES;
   
#if MQX_ENABLE_USER_MODE
   if (template_ptr->TASK_ATTRIBUTES & MQX_USER_TASK) {
       td_ptr->FLAGS |= TASK_USER_MODE;
   }
#endif /* MQX_ENABLE_USER_MODE */

/* START CR 897 */
   if (input_stack_ptr) {
      td_ptr->FLAGS |= TASK_STACK_PREALLOCATED;
   } /* Endif */
/* END CR 897 */

   // create stack frame - initialize stack parameters in TD and stack itself (registers required by scheduller, etc.)
   if (FALSE == _psp_build_stack_frame(td_ptr, stack_ptr, stack_size, template_ptr, (_mqx_uint)ready_q_ptr->ENABLE_SR, create_parameter)) {
   	  // build stack failed
   	  
   	  // TODO dealloc td_ptr ??? - litle bit complicate
   	  return NULL;
   }
   

   /* Initialize the task's task descriptor. */
#if MQX_TD_HAS_TASK_TEMPLATE_PTR
   td_ptr->TASK_TEMPLATE_PTR = template_ptr;
#endif
#if MQX_TD_HAS_TEMPLATE_INDEX
   td_ptr->TEMPLATE_INDEX    = template_ptr->TASK_TEMPLATE_INDEX;
#endif

#if MQX_HAS_TIME_SLICE
   /* Use the provided default time slice */
   if (template_ptr->DEFAULT_TIME_SLICE) {
#if (MQX_DEFAULT_TIME_SLICE_IN_TICKS == 0)
      uint_32 ticks;

      ticks = ((template_ptr->DEFAULT_TIME_SLICE * 2 *
         kernel_data->TICKS_PER_SECOND) / 1000) / 2  /* Rounding.. */;
      PSP_ADD_TICKS_TO_TICK_STRUCT(&td_ptr->TIME_SLICE, 
         ticks, &td_ptr->TIME_SLICE);
#else
      PSP_ADD_TICKS_TO_TICK_STRUCT(&td_ptr->TIME_SLICE, 
         template_ptr->DEFAULT_TIME_SLICE, &td_ptr->TIME_SLICE);
#endif
   } else {
      td_ptr->TIME_SLICE = kernel_data->SCHED_TIME_SLICE;
   } /* Endif */
   if (kernel_data->SCHED_POLICY == MQX_SCHED_RR) {
      td_ptr->FLAGS |= MQX_TIME_SLICE_TASK;
   } /* Endif */
#endif
   
#if MQX_USE_IO
   td_ptr->STDIN_STREAM  = kernel_data->PROCESSOR_STDIN;
   td_ptr->STDOUT_STREAM = kernel_data->PROCESSOR_STDOUT;
   td_ptr->STDERR_STREAM = kernel_data->PROCESSOR_STDERR;
#endif

#if MQX_TD_HAS_PARENT   
   td_ptr->PARENT = creator_task_id;
#endif

   /* 
   ** Move ownership of the td from the creating task, to the
   ** newly created task.
   */
#if MQX_USE_LWMEM_ALLOCATOR==0
   block_ptr = GET_MEMBLOCK_PTR(td_ptr);
   kernel_data->ACTIVE_PTR->MEMORY_RESOURCE_LIST = block_ptr->NEXTBLOCK;
#endif
/* START CR 897 */
   if (input_stack_ptr == 0) {
      _mem_transfer_internal(td_ptr, td_ptr);
      _mem_transfer_internal(td_ptr->STACK_ALLOC_BLOCK, td_ptr);
   } /* Endif */
/* END CR 897 */

   return (td_ptr);

} /* Endbody */


#if MQX_MONITOR_STACK 
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_fill_stack_internal
* Returned Value   : void
* Comments         :
*   initializes a stack to be filled with the MQX_MONITOR_STACK_VALUE: "stak"
*
*END*----------------------------------------------------------------------*/

void _task_fill_stack_internal
   (
      /* [IN] the location to start filling from */
      _mqx_uint_ptr addr_ptr,
      
      /* [IN] the number of bytes to fill */
      _mqx_uint     size
   )
{ /* Body */

   size = (size/sizeof(_mqx_uint)) + 1;  /* Calculate # _mqx_uints */
   while (--size) {
      *addr_ptr++ = MQX_STACK_MONITOR_VALUE;
   } /* Endwhile */

} /* Endbody */
#endif


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_ready_internal
* Returned Value   : void
* Comments         :
*   Adds a task to the ready to run queue.  
*   This function MUST BE CALLED DISABLED
*
*END*----------------------------------------------------------------------*/

void _task_ready_internal
   (
      /* [IN] the address of the task descriptor to add */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR  kernel_data;
   register READY_Q_STRUCT_PTR      ready_q_ptr;
   register TD_STRUCT_PTR           old_tail_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_task_ready, td_ptr);

   ready_q_ptr   = td_ptr->MY_QUEUE;
   old_tail_ptr  = ready_q_ptr->TAIL_READY_Q;
   /* 
   ** Ready queues are ALWAYS allocated with higher priority queues in higher
   ** memory, thus we need to update the CURRENT_READY_Q field (which
   ** always points to the highest priority ready queue with a task
   ** on it.
   */
   if ( (uchar _PTR_)ready_q_ptr >
      (uchar _PTR_)(kernel_data->CURRENT_READY_Q) )
   {
      kernel_data->CURRENT_READY_Q = ready_q_ptr;
   } /* Endif */

   td_ptr->STATE              = READY;
   td_ptr->TD_PREV            = old_tail_ptr;
   td_ptr->TD_NEXT            = old_tail_ptr->TD_NEXT;
   old_tail_ptr->TD_NEXT      = td_ptr;
   ready_q_ptr->TAIL_READY_Q  = td_ptr;
   /* SPR P171-0020-01 add macro to zero tick structure */
#if MQX_HAS_TIME_SLICE
   MQX_ZERO_TICK_STRUCT(&td_ptr->CURRENT_TIME_SLICE);
#endif
   /* END SPR */
   _KLOGX1(KLOG_task_ready);

} /* Endbody */

/* EOF */
