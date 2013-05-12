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
* $FileName: int_gkis.c$
* $Version : 3.8.4.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the functions for returning the kernel isr for an interrupt.
*
*END************************************************************************/

#include "mqx_inc.h"

extern uint_32 __ICFEDIT_region_ROM_start__;
extern uint_32 __ICFEDIT_region_RAM_start__;

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
   uint_32     vtor = _PSP_GET_VTOR(); //value of Vector Table Offset Register

#if MQX_CHECK_ERRORS
   if ( vector >= PSP_MAXIMUM_INTERRUPT_VECTORS ) {
      _task_set_error(MQX_INVALID_VECTORED_INTERRUPT);
      return NULL;
   } /* Endif */
#endif

   /* Note that VTOR bit indicating SRAM / ROM location is just a SRAM / ROM
   ** base address within Cortex memory map
   */
   return (void (_CODE_PTR_)(void)) (vtor + 4 * vector);

} /* Endbody */

/* EOF */
