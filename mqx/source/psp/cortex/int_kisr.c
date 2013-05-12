/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: int_kisr.c$
* $Version : 3.7.4.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the function for installing a kernel level isr. 
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_install_kernel_isr
* Returned Value   : pointer
* Comments         :
*    This function installs a kernel level isr handler.
*
*END*----------------------------------------------------------------------*/

void (_CODE_PTR_ _int_install_kernel_isr
   (  
      /* [IN] the vector where the ISR is to be installed */
      uint_32  vector,

      /* [IN] the function to install into the vector table */
      void     (_CODE_PTR_ isr_ptr)(void)

   ))(void)
{
#if !MQX_ROM_VECTORS
    
#if MQX_KERNEL_LOGGING
   KERNEL_DATA_STRUCT_PTR kernel_data;
#endif
   void                   (_CODE_PTR_ old_isr_ptr)(void);
   uint_32                result_code;
   uint_32_ptr            loc_ptr;

#if MQX_KERNEL_LOGGING
   _GET_KERNEL_DATA(kernel_data);
#endif

   _KLOGE3(KLOG_int_install_kernel_isr, vector, isr_ptr);

#if MQX_CHECK_ERRORS
   result_code = MQX_OK;
   old_isr_ptr = NULL;

   if ( vector >= PSP_MAXIMUM_INTERRUPT_VECTORS ) {
      result_code = MQX_INVALID_VECTORED_INTERRUPT;
   } else {
#endif

   loc_ptr = (uint_32_ptr)_int_get_vector_table();
   old_isr_ptr = (void (_CODE_PTR_)(void))loc_ptr[vector];
   loc_ptr[vector] = (uint_32)isr_ptr;

#if MQX_CHECK_ERRORS
   } /* Endif */

   /* Set result code and return result. */
   _task_set_error(result_code);
#endif

   _KLOGX3(KLOG_int_install_kernel_isr, old_isr_ptr, result_code);
   return (old_isr_ptr);
#else
   
#if MQX_CHECK_ERRORS
   /* Set result code and return result. */
   _task_set_error(MQX_INVALID_CONFIGURATION);
#endif

   return NULL;
#endif
}
