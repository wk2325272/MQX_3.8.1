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
* $FileName: int_idef.c$
* $Version : 3.8.7.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the function for installing an application
*   provided default ISR, called when an unexpected interrupt occurs.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_INTERRUPTS

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_install_default_isr
* Returned Value   : _CODE_PTR_ address or NULL on error
* Comments         :
*    This routine installs the provided function as the default ISR,
* called whenever an unhandled interrupt occurs.
*
*END*----------------------------------------------------------------------*/

#if !MQX_SPARSE_ISR_TABLE

void (_CODE_PTR_ _int_install_default_isr
   ( 
      /* [IN] the new default ISR function */
      void (_CODE_PTR_ default_isr)(pointer)

   ))(pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   void           (_CODE_PTR_ old_default_isr)(pointer);
   INTERRUPT_TABLE_STRUCT_PTR int_table_ptr;
   _mqx_uint                    number;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_int_install_default_isr, default_isr);

   old_default_isr = kernel_data->DEFAULT_ISR;
   kernel_data->DEFAULT_ISR = default_isr;

   int_table_ptr = kernel_data->INTERRUPT_TABLE_PTR;
   if (int_table_ptr != NULL) {
      number = (kernel_data->LAST_USER_ISR_VECTOR -
         kernel_data->FIRST_USER_ISR_VECTOR) + 1 + 1;

      while (--number) {
         if (int_table_ptr->APP_ISR == old_default_isr) {
            int_table_ptr->APP_ISR = default_isr;
         } /* Endif */
         ++int_table_ptr;
      } /* Endwhile */
   } /* Endif */

   _KLOGX2(KLOG_int_install_default_isr, old_default_isr);

   return(old_default_isr);

} /* Endbody */

#else

void (_CODE_PTR_ _int_install_default_isr
   ( 
      /* [IN] the new default ISR function */
      void (_CODE_PTR_ default_isr)(pointer)

   ))(pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   void           (_CODE_PTR_ old_default_isr)(pointer);

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_int_install_default_isr, default_isr);

   old_default_isr = kernel_data->DEFAULT_ISR;
   kernel_data->DEFAULT_ISR = default_isr;

   _KLOGX2(KLOG_int_install_default_isr, old_default_isr);

   return(old_default_isr);

} /* Endbody */

#endif
#endif
/* EOF */
