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
* $FileName: int_seh.c$
* $Version : 3.7.8.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the function for setting an exception handler
*   for an ISR.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_INTERRUPTS

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _int_set_exception_handler
* Returned Value   : _CODE_PTR_ - address of old exception handler
* Comments         :
*  sets the address of the current ISR exception handler,
* and returns the old one.
*
*END*-------------------------------------------------------------------------*/
#if !MQX_SPARSE_ISR_TABLE

void (_CODE_PTR_ _int_set_exception_handler
   (
      /* [IN] the interrupt vector that this exception handler is for */
      _mqx_uint vector,

      /* [IN] the exception handler function address */
      void (_CODE_PTR_ error_handler_address)(_mqx_uint, _mqx_uint, pointer, pointer)

   ))(_mqx_uint, _mqx_uint, pointer, pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   void (_CODE_PTR_ old_handler)(_mqx_uint, _mqx_uint, pointer, pointer) = NULL;
 
   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_int_set_exception_handler, vector, error_handler_address);

#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX2(KLOG_int_set_exception_handler, NULL);
      return(NULL);
   } /* Endif */
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) || (vector > kernel_data->LAST_USER_ISR_VECTOR)) {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      _KLOGX2(KLOG_int_set_exception_handler, NULL);
      return(NULL);
   } /* Endif */
#endif

   vector -= kernel_data->FIRST_USER_ISR_VECTOR;

   old_handler = kernel_data->INTERRUPT_TABLE_PTR[vector].APP_ISR_EXCEPTION_HANDLER;
   kernel_data->INTERRUPT_TABLE_PTR[vector].APP_ISR_EXCEPTION_HANDLER = error_handler_address;

   _KLOGX2(KLOG_int_set_exception_handler, old_handler);
   return(old_handler);

} /* Endbody */

#else

void (_CODE_PTR_ _int_set_exception_handler
    (
        /* [IN] the interrupt vector that this exception handler is for */
        _mqx_uint vector,
        
        /* [IN] the exception handler function address */
        void (_CODE_PTR_ error_handler_address)(_mqx_uint, _mqx_uint, pointer, pointer)
        
    ))(_mqx_uint, _mqx_uint, pointer, pointer)
{
    KERNEL_DATA_STRUCT_PTR kernel_data;
    void (_CODE_PTR_ old_handler)(_mqx_uint, _mqx_uint, pointer, pointer) = NULL;
    INTERRUPT_SPARSE_REC_STRUCT_PTR int_ptr;
   
    _GET_KERNEL_DATA(kernel_data);
    _KLOGE3(KLOG_int_set_exception_handler, vector, error_handler_address);
    
#if MQX_CHECK_ERRORS
    if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
        _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
        _KLOGX2(KLOG_int_set_exception_handler, NULL);
        return (NULL);
    }
    
    if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) || (vector > kernel_data->LAST_USER_ISR_VECTOR)) {
        _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
        _KLOGX2(KLOG_int_set_exception_handler, NULL);
        return (NULL);
    }
#endif
    
    _int_disable();

    int_ptr = kernel_data->INTERRUPT_TABLE_PTR[(vector - kernel_data->FIRST_USER_ISR_VECTOR) >> MQX_SPARSE_ISR_SHIFT];
   
    while (int_ptr && int_ptr->VEC_NUM != vector) {
        int_ptr = int_ptr->NEXT;
    }
    
    if (!int_ptr) {
        int_ptr = _mem_alloc_system(sizeof(INTERRUPT_SPARSE_REC_STRUCT));
        _mem_set_type(int_ptr, MEM_TYPE_INTERRUPT_VECTOR);

        int_ptr->NEXT = kernel_data->INTERRUPT_TABLE_PTR[(vector - kernel_data->FIRST_USER_ISR_VECTOR) >> MQX_SPARSE_ISR_SHIFT];
        kernel_data->INTERRUPT_TABLE_PTR[(vector - kernel_data->FIRST_USER_ISR_VECTOR) >> MQX_SPARSE_ISR_SHIFT] = int_ptr;
      
        old_handler = NULL;
    }
    else
        old_handler = int_ptr->APP_ISR_EXCEPTION_HANDLER;

    
    int_ptr->VEC_NUM = vector;
    int_ptr->APP_ISR_EXCEPTION_HANDLER  = error_handler_address;

    _int_enable();    
   
    _KLOGX2(KLOG_int_set_exception_handler, old_handler);
    return (old_handler);
}

#endif

#endif
