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
* $FileName: ti_stpat.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for starting a periodic timer
*   at a certain time.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_TIMER
#include "timer.h"
#include "tim_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _timer_start_periodic_at
* Returned Value  : _timer_id - Returns id of timer or null on error
* Comments        : initialize a timer to fire periodically at a
*    specific time.
*
*END*------------------------------------------------------------------*/

_timer_id _timer_start_periodic_at
   ( 

      /* [IN] the function to call when the timer expires */
      void (_CODE_PTR_ notification_function_ptr)
         (_timer_id, pointer, uint_32, uint_32), 

      /* [IN] the data to pass to the function when the timer expires */
      pointer          notification_data_ptr,
      
      /* 
      ** [IN] which time to use when calculating time to fire 
      **   TIMER_ELAPSED_TIME_MODE
      **   TIMER_KERNEL_TIME_MODE
      */
      _mqx_uint        mode, 
      
      /* 
      ** [IN] the time at which to call the notification
      ** function, and then cancel the timer.
      */
      TIME_STRUCT_PTR  time_ptr,

      /* 
      ** [IN] the number of milliseconds to wait between calls to the
      ** notification function
      */
      uint_32          milliseconds

   )
{
   _KLOGM(KERNEL_DATA_STRUCT_PTR   kernel_data;)
   MQX_TICK_STRUCT                 stick;
   MQX_TICK_STRUCT                 wtick;
   /* Start CR 330 */
   /* TIME_STRUCT                     wtime; */
   /* End CR 330 */
   _timer_id                       result;
   
   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE4(KLOG_timer_start_periodic_at, notification_function_ptr, mode, 
      time_ptr);

#if MQX_CHECK_ERRORS
   if ((notification_function_ptr == NULL) || 
       (time_ptr == NULL) ||
       (milliseconds == 0))
   {
      _task_set_error(MQX_INVALID_PARAMETER);
      _KLOGX2(KLOG_timer_start_periodic_at, TIMER_NULL_ID);
      return TIMER_NULL_ID;
   } /* Endif */
#endif

   PSP_TIME_TO_TICKS(time_ptr, &stick);

   /* Start CR 330 */
   /* wtime.SECONDS      = 0;            */
   /* wtime.MILLISECONDS = milliseconds; */
   /*                                    */
   /* PSP_TIME_TO_TICKS(&wtime, &wtick); */
   PSP_MILLISECONDS_TO_TICKS_QUICK(milliseconds, &wtick);
   /* End CR 330 */

   result = _timer_start_periodic_at_internal(notification_function_ptr, 
      notification_data_ptr, mode, (MQX_TICK_STRUCT_PTR)&stick,
      (MQX_TICK_STRUCT_PTR)&wtick, FALSE);

   _KLOGX2(KLOG_timer_start_periodic_at, result);

   return(result);
   
} /* Endbody */
#endif /* MQX_USE_TIMER */

/* EOF */

