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
* $FileName: lwt_cncl.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for canceling a timer.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWTIMER
#include "lwtimer.h"
#include "lwtimprv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _lwtimer_cancel_timer
* Returned Value  : _mqx_uint MQX error code
* Comments        : 
*  This function will cancel an outstanding timer request                 
*
*END*------------------------------------------------------------------*/

_mqx_uint _lwtimer_cancel_timer
   (
      /* [IN] the timer to add to the period */
      LWTIMER_STRUCT_PTR        timer_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   LWTIMER_PERIOD_STRUCT_PTR period_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_lwtimer_cancel_timer, timer_ptr);

#if MQX_CHECK_VALIDITY
   if (timer_ptr->VALID != LWTIMER_VALID) {
      _KLOGX2(KLOG_lwtimer_cancel_timer, MQX_LWTIMER_INVALID);
      return MQX_LWTIMER_INVALID;
   } /* Endif */
#endif

   period_ptr = timer_ptr->PERIOD_PTR;
   _int_disable();
#if MQX_CHECK_VALIDITY
   if (period_ptr->VALID != LWTIMER_VALID) {
      _int_enable();
      _KLOGX2(KLOG_lwtimer_cancel_timer, MQX_LWTIMER_INVALID);
      return MQX_LWTIMER_INVALID;
   } /* Endif */
#endif
   timer_ptr->VALID = 0;
   if (timer_ptr == period_ptr->TIMER_PTR) {
      period_ptr->TIMER_PTR = (pointer)timer_ptr->LINK.PREV;
   } /* Endif */
   _QUEUE_REMOVE(&period_ptr->TIMERS, timer_ptr);
   _int_enable();

   _KLOGX2(KLOG_lwtimer_cancel_timer, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LWTIMER */

/* EOF */

