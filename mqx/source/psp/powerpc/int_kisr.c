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
* $FileName: int_kisr.c$
* $Version : 3.8.4.0$
* $Date    : Sep-20-2011$
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
      _mqx_uint            int_vector,

      /* [IN] the function to install into the vector table */
      void     (_CODE_PTR_ isr_ptr)(void)

   ))(void)
{ /* Body */
#if !MQX_ROM_VECTORS
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)
   uint_32                       result  = MQX_OK;
   pointer                       loc;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE3(KLOG_int_install_kernel_isr, int_vector, isr_ptr);
   

  
#if MQX_CHECK_ERRORS
   loc = NULL;

   if ( int_vector >= PSP_MAXIMUM_INTERRUPT_VECTORS ) {
      result = MQX_INVALID_VECTORED_INTERRUPT;
      /* Set result code and return result. */
      _task_set_error(result);
   } else {
#endif

   #ifdef PSP_MPC566
     loc = (pointer)(_int_get_vector_table() + (int_vector * 0x100));
   #else
     loc = (pointer)(int_vector * 0x100);
   #endif

   _int_disable();
   _mem_copy((pointer)isr_ptr, loc, 0x100);
   _dcache_flush_mlines(loc, 0x100);
   _icache_invalidate_mlines(loc, 0x100);
   _int_enable();

#if MQX_CHECK_ERRORS
   } /* Endif */
#endif

   _KLOGX3(KLOG_int_install_kernel_isr, loc, result);

   return (void (_CODE_PTR_)(void))(loc);
#else
    
#if MQX_CHECK_ERRORS
   /* Set result code and return result. */
   _task_set_error(MQX_INVALID_CONFIGURATION);
#endif

   return NULL;
#endif


} /* Endbody */

/* EOF */
