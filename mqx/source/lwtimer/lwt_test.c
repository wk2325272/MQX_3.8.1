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
* $FileName: lwt_test.c$
* $Version : 3.6.5.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the function that tests the light weight timers.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWTIMER
#include "lwtimer.h"
#include "lwtimprv.h"

/*FUNCTION*-----------------------------------------------------------
*
* Function Name   : _lwtimer_test
* Return Value    : _mqx_uint - MQX_OK or error code
* Comments        : This function tests the timer component for validity
*   and consistency.
*
*END*------------------------------------------------------------------*/

_mqx_uint _lwtimer_test
   (
      /* [OUT] the timer period in error */
      pointer _PTR_ period_error_ptr,

      /* [OUT] the timer element in error */
      pointer _PTR_ timer_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   LWTIMER_STRUCT_PTR        timer_ptr;
   LWTIMER_PERIOD_STRUCT_PTR period_ptr;
   _mqx_uint                 result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_lwtimer_test, period_error_ptr, timer_error_ptr);

   *period_error_ptr = NULL;
   *timer_error_ptr  = NULL;

   /* 
   ** It is not considered an error if the lwtimer component has not been
   ** created yet
   */
   if (kernel_data->LWTIMERS.NEXT == NULL) {
      return(MQX_OK);
   } /* Endif */
  
   result = _queue_test(&kernel_data->LWTIMERS, period_error_ptr);
   if (result != MQX_OK) {
      _KLOGX3(KLOG_lwtimer_test, result, *period_error_ptr);
      return(result);
   } /* Endif */
   
   _int_disable();
   period_ptr = (pointer)kernel_data->LWTIMERS.NEXT;
   while ((pointer)period_ptr != (pointer)&kernel_data->LWTIMERS) {
      if (period_ptr->VALID != LWTIMER_VALID) {
         _int_enable();
         *period_error_ptr = period_ptr;
         _KLOGX3(KLOG_lwtimer_test, MQX_LWTIMER_INVALID, period_ptr);
         return(MQX_LWTIMER_INVALID);
      } /* Endif */
      result = _queue_test(&period_ptr->TIMERS, timer_error_ptr);
      if (result != MQX_OK) {
         /* START CR 2066 */
         _int_enable();
         /* END CR 2066 */
         *period_error_ptr = period_ptr;
         _KLOGX4(KLOG_lwtimer_test, result, *period_error_ptr, *timer_error_ptr);
         return(result);
      } /* Endif */
      timer_ptr = (pointer)period_ptr->TIMERS.NEXT;
      while (timer_ptr != (pointer)&period_ptr->TIMERS) {
         if (timer_ptr->VALID != LWTIMER_VALID) {
            *period_error_ptr = period_ptr;
            *timer_error_ptr  = timer_ptr;
            _KLOGX4(KLOG_lwtimer_test, MQX_LWTIMER_INVALID, period_ptr,
               timer_ptr);
            return(MQX_LWTIMER_INVALID);
         } /* Endif */
         timer_ptr = (pointer)timer_ptr->LINK.NEXT;
      } /* Endwhile */
      period_ptr = (pointer)period_ptr->LINK.NEXT;
   } /* Endwhile */

   _int_enable();
   _KLOGX2(KLOG_lwtimer_test, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LWTIMER */

/* EOF */
