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
* $FileName: int_sdat.c$
* $Version : 3.5.5.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for setting the data parameter
*   for an ISR
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_set_isr_data
* Returned Value   : pointer address or NULL on error
* Comments         : 
*   This function sets the address of the interrupt handler data
*   for the specified vector, and returns the old value
*
*END*----------------------------------------------------------------------*/
#if !MQX_SPARSE_ISR_TABLE

pointer _int_set_isr_data
   ( 
      /* [IN] the interrupt vector that this data is for */
      _mqx_uint vector,
      
      /* [IN] the data passed to the ISR by the kernel */
      pointer data
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   pointer                old_data;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_int_set_isr_data,vector,data);

#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX2(KLOG_int_set_isr_data,NULL);
      return(NULL);
   } /* Endif */
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (vector > kernel_data->LAST_USER_ISR_VECTOR))
   {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      _KLOGX2(KLOG_int_set_isr_data,NULL);
      return(NULL);
   } /* Endif */
#endif

   vector -= (kernel_data->FIRST_USER_ISR_VECTOR);

   old_data = kernel_data->INTERRUPT_TABLE_PTR[vector].APP_ISR_DATA;
   kernel_data->INTERRUPT_TABLE_PTR[vector].APP_ISR_DATA = data;

   _KLOGX2(KLOG_int_set_isr_data,old_data);
   return old_data;

} /* Endbody */

#else

pointer _int_set_isr_data
   ( 
      /* [IN] the interrupt vector that this data is for */
      _mqx_uint vector,
      
      /* [IN] the data passed to the ISR by the kernel */
      pointer data
   )
{
   KERNEL_DATA_STRUCT_PTR kernel_data;
   pointer                old_data = NULL;
   INTERRUPT_SPARSE_REC_STRUCT_PTR int_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_int_set_isr_data,vector,data);

#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX2(KLOG_int_set_isr_data,NULL);
      return(NULL);
   } /* Endif */
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (vector > kernel_data->LAST_USER_ISR_VECTOR))
   {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      _KLOGX2(KLOG_int_set_isr_data,NULL);
      return(NULL);
   } /* Endif */
#endif

   _int_disable();
   
   int_ptr = kernel_data->INTERRUPT_TABLE_PTR[(vector - kernel_data->FIRST_USER_ISR_VECTOR) >> MQX_SPARSE_ISR_SHIFT];
   
   while (int_ptr && int_ptr->VEC_NUM != vector) {
      int_ptr = int_ptr->NEXT;
   }
   
   if (int_ptr) {
      old_data = int_ptr->APP_ISR_DATA;
      int_ptr->APP_ISR_DATA = data;
   }
   
   _int_enable();

   _KLOGX2(KLOG_int_set_isr_data,old_data);
   return old_data;
}

#endif
#endif

/* EOF */
