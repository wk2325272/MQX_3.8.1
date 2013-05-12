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
* $FileName: stack_bu.c$
* $Version : 3.8.17.1$
* $Date    : Apr-2-2012$
*
* Comments:
*
*   This file contains the functions for manipulating the user
*   context on the stack.
*   $Header:stack_bu.c, 5, 3/29/2004 12:03:44 PM, $
*   $NoKeywords$
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_build_stack_frame
* Returned Value   : TRUE if successful
* Comments         :
*
*   This function sets up the stack frame of a new task descriptor.
*
*END*----------------------------------------------------------------------*/

boolean _psp_build_stack_frame
   (
      /* [IN] the address of the task descriptor */
      TD_STRUCT_PTR    td_ptr,

      /* [IN] the address of the stack memory block */
      pointer          stack_ptr,

      /* [IN] the size of the stack */
      uint_32          stack_size,

      /* [IN] the task template address */
      TASK_TEMPLATE_STRUCT_PTR template_ptr,

      /* [IN] the status register to use in creating the task */
      uint_32          status_register,

      /* [IN] the task creation parameter */
      uint_32          create_parameter
   )
{ /* Body */
   uchar_ptr stack_base_ptr;
   PSP_STACK_START_STRUCT_PTR stack_start_ptr;
   uint_32 reg_val;
   boolean res = TRUE;

   stack_base_ptr  = (uchar_ptr)_GET_STACK_BASE(stack_ptr, stack_size);
   stack_start_ptr = (pointer)(stack_base_ptr - sizeof(PSP_STACK_START_STRUCT));

   td_ptr->STACK_BASE  = (pointer)stack_base_ptr;
#if MQX_TD_HAS_STACK_LIMIT
   td_ptr->STACK_LIMIT = _GET_STACK_LIMIT(stack_ptr, stack_size);
#endif
   td_ptr->STACK_PTR   = stack_start_ptr;

   /*
   ** Build the task's initial stack frame. This contains the initialized
   ** registers, and an exception frame which will cause the task to
   ** "return" to the start of the task when it is dispatched.
   */
   _mem_zero(stack_start_ptr, (uint_32)sizeof(PSP_STACK_START_STRUCT));

   stack_start_ptr->INITIAL_CONTEXT.FRAME.STATUS_REGISTER =
      (uint_16)status_register;

   stack_start_ptr->INITIAL_CONTEXT.FRAME.FORMAT_AND_VECTOR =
      (uint_16)PSP_NORMAL_STACK_FRAME;

   stack_start_ptr->INITIAL_CONTEXT.FRAME.RETURN_ADDRESS =
      (void (_CODE_PTR_)(void))  template_ptr->TASK_ADDRESS;

   stack_start_ptr->EXIT_ADDRESS    = _task_exit_function_internal;

#if PSP_ABI == PSP_ABI_REG
   stack_start_ptr->INITIAL_CONTEXT.REGISTERS.D0 = create_parameter;
#endif

   stack_start_ptr->PARAMETER       = create_parameter;

   _PSP_GET_A5(reg_val);
   stack_start_ptr->INITIAL_CONTEXT.REGISTERS.A5    = (pointer)reg_val;

   /* Mark the bottom of the stack for debuggers*/
   stack_start_ptr->INITIAL_CONTEXT.REGISTERS.A6    =
      &stack_start_ptr->ZERO_LINK_ADDRESS;

#if PSP_HAS_FPU
   if ((td_ptr->FLAGS & MQX_FLOATING_POINT_TASK) != 0) {
      res = _psp_build_float_context(td_ptr);
   } /* Endif */
#endif

   return res;	
} /* Endbody */


#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_build_float_context
* Returned Value   : none
* Comments         :
*   This function sets up the FLOATING POINT context of a
*   new task descriptor.
*
*END*----------------------------------------------------------------------*/
boolean _psp_build_float_context
   (
      /* [IN] the address of the task descriptor */
      TD_STRUCT_PTR    td_ptr
   )
{ /* Body */
   PSP_BLOCKED_FP_STRUCT_PTR fp_ptr;

   /* Allocate space for saving/restoring the DSP registers */
   fp_ptr = (PSP_BLOCKED_FP_STRUCT_PTR)_mem_alloc_zero(
      (_mem_size)sizeof(PSP_BLOCKED_FP_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (!fp_ptr) {
      /* Couldn't allocate memory for the DSP register context */
      _task_set_error_td_internal(td_ptr, MQX_OUT_OF_MEMORY);
      return FALSE;
   } /* Endif */
#endif

   _mem_set_type(fp_ptr, MEM_TYPE_FP_CONTEXT);
   /*
   ** Transfer the block to the task being created. This will ensure the
   ** float context will be freed if the task is destroyed.
   */
   _mem_transfer_internal((pointer)fp_ptr, td_ptr);

   /* This field should never be overwitten */
   fp_ptr->TID = td_ptr->TASK_ID;

   td_ptr->FLOAT_CONTEXT_PTR = (pointer)fp_ptr;
   
   return TRUE;
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_get_float_context
* Returned Value   : pointer to task's FLOATING POINT context
* Comments         :
*   This function returns the FLOAT context of a the specified task.
*
*END*----------------------------------------------------------------------*/
pointer _psp_get_float_context
   (
      /* [IN] the address of the task descriptor */
      pointer    in_td_ptr
   )
{ /* Body */
   TD_STRUCT_PTR td_ptr;

#if MQX_CHECK_ERRORS
   /* This field should never be overwitten */
   if (!in_td_ptr) {
      return NULL;
   } /* Endif */
#endif

   td_ptr = (TD_STRUCT_PTR)in_td_ptr;

   return td_ptr->FLOAT_CONTEXT_PTR;

} /* Endbody */
#endif
/* EOF */
