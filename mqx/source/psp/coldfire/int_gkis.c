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
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for returning the kernel 
*   isr for an interrupt.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_USE_INTERRUPTS

/* uint_32 _psp_saved_vbr = 0; */
extern uchar __VECTOR_TABLE_ROM_START[];
volatile uint_32 _psp_saved_vbr = (uint_32)__VECTOR_TABLE_ROM_START;


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
      uint_32 vector

   ))(void)
{ /* Body */
   uint_32_ptr vbr_ptr;
   _mqx_uint     tmp;

#if MQX_CHECK_ERRORS
   if ( vector >= PSP_MAXIMUM_INTERRUPT_VECTORS ) {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      return NULL;
   } /* Endif */
#endif

   _PSP_GET_VBR(tmp);

   vbr_ptr = (uint_32_ptr)tmp;

   return (void (_CODE_PTR_)(void))vbr_ptr[vector];

} /* Endbody */
#endif

/* EOF */
