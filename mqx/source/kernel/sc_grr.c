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
* $FileName: sc_grr.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function that returns the time slice
*   interval for a task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_get_rr_interval
* Returned Value   : uint_32 MQX_OK or MAX_UINT_32
*                     If MAX_UINT_32, the task error code is set to an MQX
*                     error code.
* Comments         :
*   This function returns the current time slice in 
* milliseconds(uint_32) for the specified task.
*
*END*----------------------------------------------------------------------*/

uint_32 _sched_get_rr_interval
   (
      /* 
      ** [IN] the task id to apply this to:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel default for task creation
      ** any other    => the specified task
      */
      _task_id    task_id,

      /* 
      ** [IN/OUT] the address where the current time slice time
      **   in milliseconds (uint_32) is to be written
      */
      uint_32_ptr ms_ptr
   )
{ /* Body */
   TIME_STRUCT      time;
   MQX_TICK_STRUCT  tick;
   uint_32          slice;

   if (_sched_get_rr_interval_ticks(task_id, &tick) != MQX_OK) {
      *ms_ptr = MAX_UINT_32;
      return MAX_UINT_32;
   } /* Endif */

   PSP_TICKS_TO_TIME(&tick, &time);

   if (time.SECONDS >= (MAX_UINT_32/1000)) {
      *ms_ptr = MAX_UINT_32;
      return(MAX_UINT_32);
   } /* Endif */

   slice = time.SECONDS * 1000;
   if (slice >= (MAX_UINT_32 - time.MILLISECONDS)) {
      *ms_ptr = MAX_UINT_32;
      return(MAX_UINT_32);
   } /* Endif */

   *ms_ptr = slice + time.MILLISECONDS;

   return( MQX_OK );
   
} /* Endbody */

/* EOF */
