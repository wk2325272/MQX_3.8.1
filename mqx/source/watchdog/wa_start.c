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
* $FileName: wa_start.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for starting a watchdog.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SW_WATCHDOGS
#include "watchdog.h"
#include "wdog_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _watchdog_start
* Returned Value  : boolean TRUE if succeeded
* Comments        : This function starts a software watchdog for the
*  current task.  The timer will expire at the specified number of
*  milliseconds in the future.  The application should call
*  _watchdog_start periodically to re-arm the timer.
*
*END*------------------------------------------------------------------*/

boolean _watchdog_start
   (
      /*  [IN]  the time in ms at which to expire the watchdog */
      uint_32 time
   )
{
   _KLOGM(KERNEL_DATA_STRUCT_PTR   kernel_data;)
   boolean                         result;
   MQX_TICK_STRUCT                 start_time;
   /* Start CR 330 */
   /* TIME_STRUCT                     tmp; */
   /* End CR 330 */

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE2(KLOG_watchdog_start, time);

   /* Start CR 330 */
   /* tmp.MILLISECONDS = time;              */
   /* tmp.SECONDS      = 0;                 */
   /*                                       */
   /* PSP_TIME_TO_TICKS(&tmp, &start_time); */
   PSP_MILLISECONDS_TO_TICKS_QUICK(time, &start_time);
   /* End CR 330 */

   result = _watchdog_start_internal((MQX_TICK_STRUCT_PTR)&start_time);

   _KLOGX2(KLOG_watchdog_start, result);

   return(result);

} /* Endbody */
#endif /* MQX_USE_SW_WATCHDOGS */

/* EOF */
