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
* $FileName: int_xcpt.c$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains the isr that handles exceptions.
*
*END************************************************************************/

#include "mqx_inc.h"


/*ISR*---------------------------------------------------------------------
*
* Function Name    : _int_exception_isr
* Returned Value   : none
* Comments         :
*  This function is the exception handler which may be used to replace the
* default exception handler, in order to provide support for exceptions.
*
* If an exception happens while a task is running,
* then: if a task exception handler exists, it is executed, 
*       otherwise the task is aborted.
*
* If an exception happens while an isr is running,
* then: if an isr exception handler exists, it is executed, and the isr aborted
*       otherwise the isr is aborted.
*
*END*-------------------------------------------------------------------------*/

#if !MQX_SPARSE_ISR_TABLE

void _int_exception_isr
   (
      /* [IN] the parameter passed to the default ISR, the vector number */
      pointer parameter
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR         kernel_data;
   TD_STRUCT_PTR                  td_ptr;
   PSP_INTERRUPT_FRAME_STRUCT_PTR exception_frame_ptr;
   PSP_INTERRUPT_FRAME_STRUCT_PTR isr_frame_ptr;
   INTERRUPT_TABLE_STRUCT_PTR     table_ptr;
   uint_32                        isr_vector;
   uint_32                        exception_vector;
   void                           (*exception_handler)
      (uint_32, uint_32, pointer, pointer);
   void               (_CODE_PTR_ func)(void);

   _GET_KERNEL_DATA(kernel_data);
   td_ptr = kernel_data->ACTIVE_PTR;

   /* Stop all interrupts */
   _PSP_SET_DISABLE_SR(kernel_data->DISABLE_SR);

   if ( kernel_data->IN_ISR > 1 ) {
      /* 
      ** We have entered this function from an exception that happened
      ** while an isr was running.
      */

      /* 
      ** The interrupt context pointer stored in the kernel is part
      ** of the interrupt frame structure, whose address can then be determined.
      */
      exception_frame_ptr = (pointer)kernel_data->INTERRUPT_CONTEXT_PTR;
      _BACKUP_POINTER(exception_frame_ptr,PSP_INTERRUPT_FRAME_STRUCT,
         INTERRUPT_CONTEXT);
      exception_vector    = exception_frame_ptr->EXCEPTION_NUMBER;

      /* the current context contains a pointer to the next one */
      isr_frame_ptr = (pointer)exception_frame_ptr->INTERRUPT_CONTEXT.PREV_CONTEXT;
      if (isr_frame_ptr == NULL) {
         /* This is not allowable */
         _mqx_fatal_error(MQX_CORRUPT_INTERRUPT_STACK);
      }/* Endif */
      _BACKUP_POINTER(isr_frame_ptr,PSP_INTERRUPT_FRAME_STRUCT,
         INTERRUPT_CONTEXT);
      isr_vector    = isr_frame_ptr->EXCEPTION_NUMBER;

      /* Call the isr exception handler for the ISR that WAS running */
      table_ptr = kernel_data->INTERRUPT_TABLE_PTR;
#if MQX_CHECK_ERRORS
      if ((table_ptr != NULL) &&
         (isr_vector >= kernel_data->FIRST_USER_ISR_VECTOR) &&
         (isr_vector <= kernel_data->LAST_USER_ISR_VECTOR))
      {
#endif
         /* 
         ** Call the exception handler for the isr on isr_vector,
         ** passing the isr_vector, the exception_vector, the isr_data and
         ** the basic frame pointer for the exception
         */
         table_ptr = &table_ptr[isr_vector - kernel_data->FIRST_USER_ISR_VECTOR];
         exception_handler = table_ptr->APP_ISR_EXCEPTION_HANDLER;
         if (exception_handler != NULL) {
            (*exception_handler)(isr_vector, exception_vector, 
               table_ptr->APP_ISR_DATA, exception_frame_ptr);
         } /* Endif */
#if MQX_CHECK_ERRORS
      } else {
         /* In this case, the exception occured in this handler */
         _mqx_fatal_error(MQX_INVALID_VECTORED_INTERRUPT);
      } /* Endif */
#endif
      
      /* Indicate we have popped 1 interrupt stack frame (the exception frame) */
      --kernel_data->IN_ISR;

      /* 
      ** Jump to the location in _int_kernel_isr after where the user isr
      ** has returned, and before we de-allocate the interrupt frame
      */
      func = _int_kernel_isr_return_internal;

       /* Reset the stack to point to the interrupt frame */
      _PSP_SET_SP((pointer)isr_frame_ptr);

       /* And off we go */
      _PSP_GOTO(func);

   } else {
      /* 
      ** We have entered this function from an exception that happened
      ** while a task was running.
      */

      if (td_ptr->EXCEPTION_HANDLER_PTR != NULL ) {
         (*td_ptr->EXCEPTION_HANDLER_PTR)((uint_32)parameter, td_ptr->STACK_PTR);
      } else {
         /* Abort the current task */
         _task_abort(MQX_NULL_TASK_ID);
      }/* Endif */

   }/* Endif */
   
} /* Endbody */

#endif

/* EOF */

