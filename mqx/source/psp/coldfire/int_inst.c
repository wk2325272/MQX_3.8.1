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
* $FileName: int_inst.c$
* $Version : 3.5.5.0$
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
* Function Name    : _psp_int_install
* Returned Value   : void
* Comments         :
*    This function initializes the 1st level interrupt tables.
*
*END*----------------------------------------------------------------------*/

void _psp_int_install
   (
      void
   )
{ /* Body */

#if !MQX_ROM_VECTORS
   uint_32                    i;
   uint_32_ptr                vbr_ptr;
   
   vbr_ptr = (uint_32_ptr)_int_get_vector_table();
      
   /* Initialize the hardware interrupt vector table */   
   // 0: Initial stack pointer
   // 1: Initial program counter
   for (i = 2; i < PSP_MAXIMUM_INTERRUPT_VECTORS; i++ ) {
      vbr_ptr[i] = (uint_32)_int_kernel_isr;
   } /* Endfor */
#endif
} /* Endbody */

#endif
/* EOF */
