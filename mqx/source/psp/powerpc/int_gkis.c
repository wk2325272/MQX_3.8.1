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
* $FileName: int_gkis.c$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains the functions for returning the kernel 
*   isr for an interrupt.
*
*END************************************************************************/

#include "mqx_inc.h"

#ifdef PSP_BOOKE
// Pass it off to the bsp
extern void (_CODE_PTR_ _bsp_int_get_kernel_isr(_mqx_uint))(void);
#endif


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_get_kernel_isr
* Returned Value   : _CODE_PTR_ address or NULL on error
* Comments         : 
*   This function retrieves the address of the first level interrupt
*   handler for the specified vector
*
*END*----------------------------------------------------------------------*/

void (_CODE_PTR_ _int_get_kernel_isr
   (
      /* [IN] the vector number whose kernel ISR is wanted */
      _mqx_uint int_vector

   ))(void)
{ /* Body */

#if MQX_CHECK_ERRORS
   if ( int_vector >= PSP_MAXIMUM_INTERRUPT_VECTORS ) {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      return NULL;
   } /* Endif */
#endif

#ifdef PSP_BOOKE

   return _bsp_int_get_kernel_isr(int_vector);

#else   // !PSP_BOOKE (Original exception model)

   #ifdef PSP_MPC566
   return (void (_CODE_PTR_)(void))(_int_get_vector_table() + (int_vector * 0x100));
   #else
   return (void (_CODE_PTR_)(void))(int_vector * 0x100);
   #endif
   
#endif /* PSP_BOOKE */

} /* Endbody */

/* EOF */
