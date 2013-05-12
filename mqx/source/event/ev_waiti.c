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
* $FileName: ev_waiti.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for waiting for all or any requested bits
*   to be set in the event.
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
* Function Name    : _event_wait_internal
* Returned Value   : Returns MQX_OK upon success an error code:
* Comments         : Used by a task to wait for all or any specified events
*   according to bit mask
* 
*END*------------------------------------------------------------------*/
 
_mqx_uint _event_wait_internal
   (
      /* 
      ** [IN] - An event handle returned from a call to _event_open() or
      ** _event_open_fast()
      */
      pointer              users_event_ptr, 

      /* [IN] - bit mask - each bit of which represents an event. */
      _mqx_uint            bit_mask, 

      /* [IN] - The maximum number of ticks to wait for or time to until */
      MQX_TICK_STRUCT_PTR  tick_ptr,

      /* [IN] - if wait is specified in ticks is it absolute or relative time */
      boolean              ticks_are_absolute,

      /* [IN] - if wait is for all bits */
      boolean              all

   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
            TD_STRUCT_PTR               td_ptr;
   register EVENT_STRUCT_PTR            event_ptr;
   register EVENT_CONNECTION_STRUCT_PTR event_connection_ptr;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   event_connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)users_event_ptr;

#if MQX_CHECK_VALIDITY
   if (event_connection_ptr->VALID != EVENT_VALID){
      return(EVENT_INVALID_EVENT_HANDLE); 
   } /* Endif */
#endif

   if (event_connection_ptr->REMOTE_CPU) {
      _KLOGX2(KLOG_event_wait_all, EVENT_CANNOT_WAIT_ON_REMOTE_EVENT);
      return(EVENT_CANNOT_WAIT_ON_REMOTE_EVENT); 
   }/* Endif */

   td_ptr = kernel_data->ACTIVE_PTR;

#if MQX_CHECK_ERRORS
   if (event_connection_ptr->TD_PTR != td_ptr) {
      return(EVENT_INVALID_EVENT_HANDLE); 
   } /* Endif */
#endif

   _INT_DISABLE();
   event_ptr = event_connection_ptr->EVENT_PTR;

#if MQX_CHECK_VALIDITY
   if (event_ptr->VALID != EVENT_VALID) {
      _int_enable();
      return(EVENT_INVALID_EVENT); 
   } /* Endif */
#endif

   if ((all && ((event_ptr->EVENT & bit_mask) != bit_mask)) ||
      (!all && ((event_ptr->EVENT & bit_mask) == 0)))
   {
      /* Must wait for a event to become available */
      if (all) {
         event_connection_ptr->FLAGS  = EVENT_WANTS_ALL;
      } else {
         event_connection_ptr->FLAGS  = 0;
      } /* Endif */

      event_connection_ptr->MASK = bit_mask;

      /* Enqueue at end */   
      _QUEUE_ENQUEUE(&event_ptr->WAITING_TASKS, event_connection_ptr);

      /* Now put the task to sleep */
      td_ptr->STATE = EVENT_BLOCKED;
         
      if (tick_ptr) {
         if (ticks_are_absolute) {
            _time_delay_until(tick_ptr);
         } else {
            _time_delay_for(tick_ptr);
         } /* Endif */
      } else {
         _task_block();
      } /* Endif */

#if MQX_COMPONENT_DESTRUCTION
      if (event_ptr->VALID == 0) {  /* We've been deleted */
         _int_enable();
         return(EVENT_DELETED);
      } /* Endif */
#endif

      _QUEUE_REMOVE(&event_ptr->WAITING_TASKS, event_connection_ptr);

      if (tick_ptr && !(event_connection_ptr->FLAGS & EVENT_OCCURRED)) {
         /* MUST have timed out */
         _INT_ENABLE();
         return(EVENT_WAIT_TIMEOUT);
      } /* Endif */
   } /* Endif */

      /* START CR 1832 */
      if (event_ptr->AUTO_CLEAR) {
         event_ptr->EVENT = event_ptr->EVENT & ~bit_mask;
      } /* Endif */
      /* END CR 1832 */

   /* GOT IT */
   _INT_ENABLE();

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_EVENTS */

/* EOF */
