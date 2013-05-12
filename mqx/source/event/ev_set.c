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
* $FileName: ev_set.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for setting the event.
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
* Function Name    : _event_set
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to set the specified event bits in an event.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _event_set
   (
      /* [IN] - An event handle returned from _event_open or _event_open_fast */
      pointer users_event_ptr, 

      /* [IN] - bit mask, each bit of which represents an event. */
      _mqx_uint bit_mask
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   EVENT_STRUCT_PTR            event_ptr;
   EVENT_COMPONENT_STRUCT_PTR  event_component_ptr;
   EVENT_CONNECTION_STRUCT_PTR conn_ptr;
   EVENT_CONNECTION_STRUCT_PTR next_conn_ptr;
   EVENT_CONNECTION_STRUCT_PTR event_connection_ptr;
   TD_STRUCT_PTR               new_td_ptr;
   _mqx_uint                    set_bits;
            
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_event_set, users_event_ptr, bit_mask);

   event_connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)users_event_ptr;

#if MQX_CHECK_VALIDITY
   if (event_connection_ptr->VALID != EVENT_VALID){
      _KLOGX2(KLOG_event_set, EVENT_INVALID_EVENT_HANDLE);
      return(EVENT_INVALID_EVENT_HANDLE); 
   } /* Endif */
#endif

   event_component_ptr = (EVENT_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_EVENTS];

#if MQX_CHECK_ERRORS
   if (event_component_ptr == NULL){
      _KLOGX2(KLOG_event_set, MQX_COMPONENT_DOES_NOT_EXIST);
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (event_component_ptr->VALID != EVENT_VALID){
      _KLOGX2(KLOG_event_set, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

#if MQX_IS_MULTI_PROCESSOR
   if (event_connection_ptr->REMOTE_CPU) {
      if (kernel_data->IPC) {
         /* This open is for a remote processor */
         (*kernel_data->IPC)(TRUE, event_connection_ptr->REMOTE_CPU, 
             KERNEL_EVENTS, IPC_EVENT_SET, 2, event_connection_ptr->EVENT_PTR,
             bit_mask);
         _KLOGX2(KLOG_event_set, kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
         return(kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
      } else {
         _KLOGX2(KLOG_event_set, EVENT_NOT_FOUND);
         return(EVENT_NOT_FOUND);
      }/* Endif */
   }/* Endif */
#endif

   _INT_DISABLE();
   event_ptr = event_connection_ptr->EVENT_PTR;

#if MQX_CHECK_VALIDITY
   if (event_ptr->VALID != EVENT_VALID) {
      _INT_ENABLE();
      _KLOGX2(KLOG_event_set, EVENT_INVALID_EVENT);
      return(EVENT_INVALID_EVENT);
   } /* Endif */
#endif

   set_bits = event_ptr->EVENT | bit_mask;

   if (_QUEUE_GET_SIZE(&event_ptr->WAITING_TASKS)) {
      /* Schedule waiting task(s) to run if bits ok */

      conn_ptr = (EVENT_CONNECTION_STRUCT_PTR)
         ((pointer)event_ptr->WAITING_TASKS.NEXT);
      while (conn_ptr != (EVENT_CONNECTION_STRUCT_PTR)
         ((pointer)&event_ptr->WAITING_TASKS))
      {
         next_conn_ptr = (EVENT_CONNECTION_STRUCT_PTR)conn_ptr->NEXT;
         if (((conn_ptr->FLAGS & EVENT_WANTS_ALL) && 
            ((conn_ptr->MASK & set_bits) == conn_ptr->MASK)) ||
            ((!(conn_ptr->FLAGS & EVENT_WANTS_ALL)) && 
            (conn_ptr->MASK & set_bits)))
         {  
            new_td_ptr = conn_ptr->TD_PTR;
            if ((new_td_ptr->STATE & STATE_MASK) == EVENT_BLOCKED) {
               /* He may have timed out */
               conn_ptr->FLAGS |= EVENT_OCCURRED;
               _TIME_DEQUEUE(new_td_ptr, kernel_data);
               _TASK_READY(new_td_ptr, kernel_data);
               /* Only ready one task if event is an auto clear event */
               if (event_ptr->AUTO_CLEAR) {
                  set_bits &= ~conn_ptr->MASK;
                  break;
               } /* Endif */
            } /* Endif */
         } /* Endif */
         conn_ptr = next_conn_ptr;
      } /* Endwhile */
   } /* Endif */

   event_ptr->EVENT = set_bits;

   _INT_ENABLE();

   /* May need to let higher priority task run */
   _CHECK_RUN_SCHEDULER();

   _KLOGX2(KLOG_event_set, MQX_OK);
   return(MQX_OK);
         
} /* Endbody */
#endif /* MQX_USE_EVENTS */

/* EOF */
