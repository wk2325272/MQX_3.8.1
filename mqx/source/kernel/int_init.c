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
* $FileName: int_init.c$
* $Version : 3.5.9.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for initializing the handling of 
*   interrupts.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_init
* Returned Value   : _mqx_uint - MQX OK or an error code
* Comments         :
*    This function initializes the kernel interrupt table.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _int_init
   (
      /* [IN] the first (lower) user ISR vector number */
      _mqx_uint          first_user_isr_vector_number,

      /* [IN] the last user ISR vector number */
      _mqx_uint          last_user_isr_vector_number

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   INTERRUPT_TABLE_STRUCT_PTR int_table_ptr;
   _mqx_uint                  number;

#if MQX_CHECK_ERRORS
   if (last_user_isr_vector_number < first_user_isr_vector_number) {
      return MQX_INVALID_PARAMETER;
   } /* Endif */
#endif

   _GET_KERNEL_DATA(kernel_data);
   kernel_data->INT_KERNEL_ISR_ADDR = _int_kernel_isr;

   /* Set the current default ISR for MQX that is called whenever an 
   ** unhandled interrupt occurs
   */
   kernel_data->DEFAULT_ISR = _int_default_isr;

#if !MQX_SPARSE_ISR_TABLE
   number = last_user_isr_vector_number - first_user_isr_vector_number + 1;
#else
   number = ((last_user_isr_vector_number - first_user_isr_vector_number + 1) >> MQX_SPARSE_ISR_SHIFT) + 1;
#endif

   int_table_ptr = _mem_alloc_system_zero((_mem_size)(sizeof(INTERRUPT_TABLE_STRUCT) * number));   
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if ( int_table_ptr == NULL ) {
      return(MQX_OUT_OF_MEMORY);
   }/* Endif */
#endif
   _mem_set_type(int_table_ptr, MEM_TYPE_INTERRUPT_TABLE);

   kernel_data->INTERRUPT_TABLE_PTR   = int_table_ptr;
   kernel_data->FIRST_USER_ISR_VECTOR = first_user_isr_vector_number;
   kernel_data->LAST_USER_ISR_VECTOR  = last_user_isr_vector_number;
   
#if !MQX_SPARSE_ISR_TABLE
   while (number--) {
      int_table_ptr->APP_ISR      = _int_default_isr;
      int_table_ptr->APP_ISR_DATA = (pointer)(first_user_isr_vector_number++);
      ++int_table_ptr;
   } /* Endwhile */
#else

   kernel_data->SPARSE_ISR_COUNT = number;
   kernel_data->SPARSE_ISR_SHIFT = MQX_SPARSE_ISR_SHIFT;

   while (number--) {
      int_table_ptr[number] = NULL;
   }

#endif   

   return MQX_OK;
   
} /* Endbody */
#endif

/* EOF */
