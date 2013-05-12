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
* $FileName: int_iisr.c$
* $Version : 3.8.10.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the function for installing an ISR.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_INTERRUPTS

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_install_isr
* Returned Value   : _CODE_PTR_ address or NULL on error
* Comments         :
*    This function provides a user interface for dynamic isr
*    installation.
*
*END*----------------------------------------------------------------------*/

#if !MQX_SPARSE_ISR_TABLE

void (_CODE_PTR_ _int_install_isr
   (
      /* [IN] the interrupt vector number (NOT OFFSET) */
      _mqx_uint  vector,

      /* [IN] the address of the function to be executed */
      void (_CODE_PTR_ isr_ptr)(pointer),

      /* 
      ** [IN] the value to be provided to the function as it's
      ** first parameter when an interrupt occurs.
      */
      pointer  isr_data

   ))(pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   INTERRUPT_TABLE_STRUCT_PTR table_ptr;
   void         (_CODE_PTR_   old_isr_ptr)(pointer);

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_int_install_isr, vector, isr_ptr, isr_data);

#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      return(NULL);
   } /* Endif */      
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (vector > kernel_data->LAST_USER_ISR_VECTOR))
   {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      _KLOGX2(KLOG_int_install_isr, NULL);
      return(NULL);
   }/* Endif */
#endif

   table_ptr = &kernel_data->INTERRUPT_TABLE_PTR[vector -
      kernel_data->FIRST_USER_ISR_VECTOR];

   _int_disable();
   old_isr_ptr = table_ptr->APP_ISR;
   table_ptr->APP_ISR  = isr_ptr;
   table_ptr->APP_ISR_DATA = isr_data;
   _int_enable();

   _KLOGX2(KLOG_int_install_isr, old_isr_ptr);

   return (old_isr_ptr);

} /* Endbody */

#else

void (_CODE_PTR_ _int_install_isr
   (
      /* [IN] the interrupt vector number (NOT OFFSET) */
      _mqx_uint  vector,

      /* [IN] the address of the function to be executed */
      void (_CODE_PTR_ isr_ptr)(pointer),

      /* 
      ** [IN] the value to be provided to the function as it's
      ** first parameter when an interrupt occurs.
      */
      pointer  isr_data

   ))(pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   INTERRUPT_SPARSE_REC_STRUCT_PTR int_ptr;
   void         (_CODE_PTR_   old_isr_ptr)(pointer);

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_int_install_isr, vector, isr_ptr, isr_data);

#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      return(NULL);
   } /* Endif */      
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (vector > kernel_data->LAST_USER_ISR_VECTOR))
   {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      _KLOGX2(KLOG_int_install_isr, NULL);
      return(NULL);
   }/* Endif */
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
      
      old_isr_ptr = _int_default_isr;
   }
   else
      old_isr_ptr = int_ptr->APP_ISR;
   
   int_ptr->VEC_NUM = vector;
   int_ptr->APP_ISR  = isr_ptr;
   int_ptr->APP_ISR_DATA = isr_data;

   _int_enable();

   _KLOGX2(KLOG_int_install_isr, old_isr_ptr);

   return (old_isr_ptr);

} /* Endbody */

#endif
#endif

/* EOF */
