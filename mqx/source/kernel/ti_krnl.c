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
* $FileName: ti_krnl.c$
* $Version : 3.5.6.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function called by the BSP whenever
*   a timer interrupt occurs.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
 *
 * Function Name    : _time_notify_kernel
 * Returned Value   : void
 * Comments         :
 *   This function is called by the bsp timer ISR.
 * It manages the timeout queue, the current task's time slice and
 * the system time.
 *
 *END*----------------------------------------------------------------------*/

void _time_notify_kernel
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   register TD_STRUCT_PTR          td_ptr;
   register TD_STRUCT_PTR          next_td_ptr;
   register _mqx_uint              count;
   register _mqx_int               result;

   _GET_KERNEL_DATA(kernel_data);

   /*
   ** Update the current time.
   */
   PSP_INC_TICKS(&kernel_data->TIME);

   _INT_DISABLE();

   if (kernel_data->GET_HWTICKS) {
      // The hardware clock may have counted passed it's reference
      // and have an interrupt pending.  Thus, HW_TICKS may exceed
      // kernel_data->HW_TICKS_PER_TICK and this tick_ptr may need
      // normalizing.  This is done in a moment.
      kernel_data->TIME.HW_TICKS = (*kernel_data->GET_HWTICKS)
         (kernel_data->GET_HWTICKS_PARAM);
   } /* Endif */

   // The tick_ptr->HW_TICKS value might exceed the
   // kernel_data->HW_TICKS_PER_TICK and need to be
   // normalized for the PSP.
   PSP_NORMALIZE_TICKS(&kernel_data->TIME);

   /*
   ** Check for tasks on the timeout queue, and wake the appropriate
   ** ones up.  The timeout queue is a time-priority queue.
   */
   count = _QUEUE_GET_SIZE(&kernel_data->TIMEOUT_QUEUE);
   if (count) {
      td_ptr = (TD_STRUCT_PTR)((pointer)kernel_data->TIMEOUT_QUEUE.NEXT);
      ++count;
      while ( --count ) {
         next_td_ptr = td_ptr->TD_NEXT;
         result = PSP_CMP_TICKS(&kernel_data->TIME, &td_ptr->TIMEOUT);
         if (result >= 0) {
            --kernel_data->TIMEOUT_QUEUE.SIZE;
            _QUEUE_UNLINK(td_ptr);
            td_ptr->STATE &= ~IS_ON_TIMEOUT_Q;
            if (td_ptr->STATE & TD_IS_ON_AUX_QUEUE) {
               td_ptr->STATE &= ~TD_IS_ON_AUX_QUEUE;
               _QUEUE_REMOVE(td_ptr->INFO, &td_ptr->AUX_QUEUE);
            } /* Endif */
            _TASK_READY(td_ptr, kernel_data);
         } else {
            break;  /* No more to do */
         } /* Endif */
         td_ptr = next_td_ptr;
      } /* Endwhile */
   } /* Endif */

#if MQX_HAS_TIME_SLICE
   /*
   ** Check if the currently running task is a time slice task
   ** and if its time has expired, put it at the end of its queue
   */
   td_ptr = kernel_data->ACTIVE_PTR;
   if ( td_ptr->FLAGS & MQX_TIME_SLICE_TASK ) {
      PSP_INC_TICKS(&td_ptr->CURRENT_TIME_SLICE);
      if (! (td_ptr->FLAGS & TASK_PREEMPTION_DISABLED) ) {
         result = PSP_CMP_TICKS(&td_ptr->CURRENT_TIME_SLICE, &td_ptr->TIME_SLICE);
         if ( result >= 0 ) {
            _QUEUE_UNLINK(td_ptr);
            _TASK_READY(td_ptr,kernel_data);
         } /* Endif */
      } /* Endif */
   } /* Endif */
#endif

   _INT_ENABLE();
#if MQX_USE_TIMER
   /* If the timer component needs servicing, call its ISR function */
   if (kernel_data->TIMER_COMPONENT_ISR != NULL) {
      (*kernel_data->TIMER_COMPONENT_ISR)();
   }/* Endif */
#endif

#if MQX_USE_LWTIMER
   /* If the lwtimer needs servicing, call its ISR function */
   if (kernel_data->LWTIMER_ISR != NULL) {
      (*kernel_data->LWTIMER_ISR)();
   }/* Endif */
#endif

} /* Endbody */

#endif
/* EOF */
