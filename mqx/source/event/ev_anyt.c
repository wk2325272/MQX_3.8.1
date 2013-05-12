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
* $FileName: ev_anyt.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for waiting for any event in a set.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_EVENTS
#include "name.h"
#include "name_prv.h"
#include "event.h"
#include "evnt_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _event_wait_any_ticks
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to for any specified event.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _event_wait_any_ticks
   (
      /* [IN] - An event handle returned from a call to _event_open() */
      pointer  users_event_ptr, 

      /* [IN] - bit mask, each bit of which represents an event. */
      _mqx_uint bit_mask, 

      /* 
      ** [IN] - The maximum number of ticks to wait for the events 
      **        to be set.  If the value is 0, then the timeout will be 
      **        infinite.
      */
      _mqx_uint timeout_in_ticks
   )
{ /* Body */
   MQX_TICK_STRUCT                  ticks;
   _KLOGM(KERNEL_DATA_STRUCT_PTR    kernel_data;)
   _mqx_uint                        result;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE4(KLOG_event_wait_any_ticks, users_event_ptr, bit_mask, 
      timeout_in_ticks);

   if (timeout_in_ticks) {
      ticks = _mqx_zero_tick_struct;

      PSP_ADD_TICKS_TO_TICK_STRUCT(&ticks, timeout_in_ticks, &ticks);
      result = _event_wait_internal(users_event_ptr, bit_mask, &ticks,
         FALSE, FALSE);
   } else {
      result = _event_wait_internal(users_event_ptr, bit_mask, NULL,
         FALSE, FALSE);
   } /* Endif */

   _KLOGX2(KLOG_event_wait_any_ticks, result);
   return(result);

} /* Endbody */
#endif /* MQX_USE_EVENTS */

/* EOF */
