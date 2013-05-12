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
* $FileName: int_pc.c$
* $Version : 3.7.3.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the function for resetting the pc of interrupted task.
*END************************************************************************/

#include "mqx_inc.h"

/* Saved the stack pointers for the various interrupt modes */
extern uint_32 _psp_irq_sp;
extern uint_32 _psp_fiq_sp;
extern uint_32 _psp_undef_sp;
extern uint_32 _psp_abort_sp;

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _psp_set_pc_of_interrupted_task
* Returned Value   : void
* Comments         :
*    This function sets the pc of an interrupted task.
*
*END*----------------------------------------------------------------------*/

void _psp_set_pc_of_interrupted_task
   (
      PSP_BASIC_INT_FRAME_STRUCT_PTR   td_stack_ptr,
      /* [IN] the function to set the pc to */
      void     (_CODE_PTR_ function)(uint_32)
   )
{ /* Body */
   uint_32                         tmp;
   PSP_BASIC_INT_FRAME_STRUCT_PTR  stack_ptr;

   /* Determine current mode */
   // TODO remake
#if 0   
   _PSP_GET_CPSR(tmp);
   tmp &= PSP_PSR_MODE_MASK;

   /* Get stack pointer for the current mode */
   switch( tmp ) {
      case PSP_PSR_MODE_FIQ:
         stack_ptr = (PSP_BASIC_INT_FRAME_STRUCT_PTR)_psp_fiq_sp;
         break;
      case PSP_PSR_MODE_IRQ:
         stack_ptr = (PSP_BASIC_INT_FRAME_STRUCT_PTR)_psp_irq_sp;
         break;
      case PSP_PSR_MODE_UNDEF:
         stack_ptr = (PSP_BASIC_INT_FRAME_STRUCT_PTR)_psp_undef_sp;
         break;
      case PSP_PSR_MODE_ABORT:
         stack_ptr = (PSP_BASIC_INT_FRAME_STRUCT_PTR)_psp_abort_sp;
         break;
      default:
         stack_ptr = td_stack_ptr;
         break;
   } /* Endswitch */
#endif   

   tmp = (uint_32)function;
   /* 
   ** Stack pointer is actually on the top. We need to add the stack
   ** frame to it
   */
   stack_ptr--;


//jm   stack_ptr->STACK.RETURN_ADDRESS = function;
  
   stack_ptr->STACK.LR             = tmp;

   if (tmp | 0x1) {
      /* Fix SPSR to return to thumb mode */
      tmp = _psp_get_spsr();
      tmp |= 0x20;
      _psp_set_spsr(tmp);
   } /* Endif */
} /* Endbody */

/* EOF */
