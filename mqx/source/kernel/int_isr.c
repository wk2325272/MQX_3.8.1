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
* $FileName: int_isr.c$
* $Version : 3.0.4.0$
* $Date    : Apr-14-2009$
*
* Comments:
*
*   This file contains the function for the default ISR called
*   by MQX if an unhandled interrupt/exception occurs.
*   This function can be replaced by the user as the default ISR
*   by calling:
*   _int_install_unexpected_isr or _int_install_exception_isr
*   which install MQX provided default ISRs.
*   or by calling _int_install_default_isr 
*   to install an application provided default ISR.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_default_isr
* Returned Value   : void
* Comments         :
*    The second level handler for all unhandled interrupts.
*
*END*----------------------------------------------------------------------*/

void _int_default_isr
   (
      /* [IN] the parameter passed to the ISR by the kernel */
      pointer vector_number
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;

   _GET_KERNEL_DATA(kernel_data);

   td_ptr = kernel_data->ACTIVE_PTR;
   _KLOGE5(KLOG_int_default_isr, td_ptr, vector_number, 
      &vector_number, vector_number);

   _int_disable();
   if (td_ptr->STATE != UNHANDLED_INT_BLOCKED) {
      td_ptr->STATE = UNHANDLED_INT_BLOCKED;
      td_ptr->INFO  = (_mqx_uint)vector_number;
      _task_set_error_td_internal(td_ptr, MQX_UNHANDLED_INTERRUPT);
      _QUEUE_UNLINK(td_ptr);
   } /* Endif */
   _int_enable();

} /* Endbody */

/* EOF */
