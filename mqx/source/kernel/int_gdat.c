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
* $FileName: int_gdat.c$
* $Version : 3.5.7.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for returning the data pointer
*   for an ISR.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_get_isr_data
* Returned Value   : pointer address or NULL on error
* Comments         : 
*   This function retrieves the address of the interrupt handler data
*   for the specified vector
*
*END*----------------------------------------------------------------------*/
#if !MQX_SPARSE_ISR_TABLE

pointer _int_get_isr_data
   (
      /* [IN] the vector number whose ISR data is to be returned */
      _mqx_uint vector
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);
#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      return(NULL);
   } /* Endif */
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (vector > kernel_data->LAST_USER_ISR_VECTOR))  
   {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      return(NULL);
   }/* Endif */
#endif

   vector -= kernel_data->FIRST_USER_ISR_VECTOR;
   return(kernel_data->INTERRUPT_TABLE_PTR[vector].APP_ISR_DATA);
      
} /* Endbody */

#else

pointer _int_get_isr_data
   (
      /* [IN] the vector number whose ISR data is to be returned */
      _mqx_uint vector
   )
{
   KERNEL_DATA_STRUCT_PTR kernel_data;
   INTERRUPT_SPARSE_REC_STRUCT_PTR int_ptr;

   _GET_KERNEL_DATA(kernel_data);
#if MQX_CHECK_ERRORS
   if ( kernel_data->INTERRUPT_TABLE_PTR == NULL ) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      return(NULL);
   } /* Endif */
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (vector > kernel_data->LAST_USER_ISR_VECTOR))  
   {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      return(NULL);
   }/* Endif */
#endif

   _int_disable();
   
   int_ptr = kernel_data->INTERRUPT_TABLE_PTR[(vector - kernel_data->FIRST_USER_ISR_VECTOR) >> MQX_SPARSE_ISR_SHIFT];
   
   while (int_ptr && int_ptr->VEC_NUM != vector) {
         int_ptr = int_ptr->NEXT;
   }
   
   _int_enable();
   
   return (int_ptr) ? int_ptr->APP_ISR_DATA : NULL;
}

#endif

#endif

/* EOF */
