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
* $FileName: sc_srr.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for setting the 
*   scheduling rr_interval of a task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_set_rr_interval
* Returned Value   : old_scheduling rr_interval
* Comments         :
*   This function sets various the scheduling rr_interval 
*   for a task or the system
*
*END*----------------------------------------------------------------------*/

uint_32 _sched_set_rr_interval
   (
      /* [IN] the task whose rr_interval is to change:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id task_id,

      /* [IN] the new scheduling rr_interval (ms) */
      uint_32  rr_interval
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)
   uint_32                old_rr_interval;
   /* Start CR 330 */
#if 0
   TIME_STRUCT            time;
#endif
   /* End CR 330 */
   MQX_TICK_STRUCT        ticks;
   MQX_TICK_STRUCT        old_ticks;
   _mqx_uint              result;
   
   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE3(KLOG_sched_set_rr_interval, (_mqx_uint)task_id, rr_interval);

   /* Start CR 330 */
   /* time.MILLISECONDS = rr_interval;  */
   /* time.SECONDS      = 0;            */
   /*                                   */
   /* PSP_TIME_TO_TICKS(&time, &ticks); */
   PSP_MILLISECONDS_TO_TICKS_QUICK(rr_interval, &ticks);
   /* End CR 330 */

   result = _sched_set_rr_interval_internal(task_id, &ticks, &old_ticks);

   if (result != MQX_OK) {
      _task_set_error(result);
      _KLOGX2(KLOG_sched_set_rr_interval, MAX_UINT_32);
      return(MAX_UINT_32);
   } /* Endif */

   /* Start CR 330 */
#if 0
   PSP_TICKS_TO_TIME(&old_ticks, &time);

   if (time.SECONDS >= (MAX_UINT_32/1000)) {
      _KLOGX2(KLOG_sched_set_rr_interval, MAX_UINT_32);
      return(MAX_UINT_32);
   } /* Endif */

   old_rr_interval = time.SECONDS * 1000;
   if (old_rr_interval >= (MAX_UINT_32 - time.MILLISECONDS)) {
      _KLOGX2(KLOG_sched_set_rr_interval, MAX_UINT_32);
      return(MAX_UINT_32);
   } /* Endif */

   old_rr_interval += time.MILLISECONDS;
#endif
   old_rr_interval = PSP_TICKS_TO_MILLISECONDS(&old_ticks, &result);
   /* End CR 330 */

   _KLOGX2(KLOG_sched_set_rr_interval, old_rr_interval);

   return(old_rr_interval);
      
} /* Endbody */

/* EOF */
