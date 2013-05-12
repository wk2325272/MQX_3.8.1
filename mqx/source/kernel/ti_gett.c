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
* $FileName: ti_gett.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for returning the current time in ticks.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_get_ticks
* Returned Value   : void
* Comments         :
*    This function returns a tick structure filled in with the current time.
*
*   ***  Keep in sync with ti_elapt.c  ***
*
*END*----------------------------------------------------------------------*/

void _time_get_ticks
   (
      /* [IN/OUT] the address where the time is to be written */
      register MQX_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;

#if MQX_CHECK_ERRORS
   if ( tick_ptr == NULL ) {
      return;
   } /* Endif */
#endif

   _GET_KERNEL_DATA(kernel_data);

   _INT_DISABLE();
      
   /* Start CR 2068 */
   /*PSP_ADD_TICKS(&kernel_data->TIME, &kernel_data->TIME_OFFSET, tick_ptr);*/
   *tick_ptr = kernel_data->TIME;

   // The hardware clock keeps counting...

   if (kernel_data->GET_HWTICKS) {
      // The hardware clock may have counted passed it's reference
      // and have an interrupt pending.  Thus, HW_TICKS may exceed
      // kernel_data->HW_TICKS_PER_TICK and this tick_ptr may need
      // normalizing.  This is done in a moment.
      tick_ptr->HW_TICKS = (*kernel_data->GET_HWTICKS)
         (kernel_data->GET_HWTICKS_PARAM);
   } /* Endif */
   
   PSP_ADD_TICKS(tick_ptr, &kernel_data->TIME_OFFSET, tick_ptr);
   /* End CR 2068 */

   _INT_ENABLE(); // The timer ISR may go off and increment kernel_data->TIME

   // The tick_ptr->HW_TICKS value might exceed the
   // kernel_data->HW_TICKS_PER_TICK and need to be
   // normalized for the PSP.
   PSP_NORMALIZE_TICKS(tick_ptr);

} /* Endbody */
#endif
/* EOF */
