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
* $FileName: lwe_waii.c$
* $Version : 3.7.7.0$
* $Date    : Jan-3-2011$
*
* Comments:
*
*   This file contains the function for waiting for a light weight event
*   $Header:lwe_waii.c, 9, 9/29/2005 1:12:00 PM, Goutham D. R.$
*   $NoKeywords$
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWEVENTS
#include "lwevent.h"
#include "lwe_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _lwevent_wait_internal
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to for any specified event. If all time parameters are 0
*    then the timeout is infinite
* 
*END*------------------------------------------------------------------*/

_mqx_uint _lwevent_wait_internal
   (
      /* [IN] - The address of the light weight event */
      LWEVENT_STRUCT_PTR   event_ptr, 

      /* [IN] - bit mask, each bit of which represents an event. */
      _mqx_uint            bit_mask,
      
      /* [IN] - waiting for all bits or just any */
      boolean              all,

      /* [IN] - The maximum number of ticks to wait for or time to until */
      MQX_TICK_STRUCT_PTR  tick_ptr,

      /* [IN] - if wait is specified in ticks is it absolute or relative time */
      boolean              ticks_are_absolute

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   _mqx_uint              result;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   result = MQX_OK;
   td_ptr = kernel_data->ACTIVE_PTR;
   _INT_DISABLE();

#if MQX_CHECK_VALIDITY
   if (event_ptr->VALID != LWEVENT_VALID) {
      _int_enable();
      return(MQX_LWEVENT_INVALID);
   } /* Endif */
#endif

   if ( ( all && (event_ptr->VALUE & bit_mask) == bit_mask) ||
        (!all && (event_ptr->VALUE & bit_mask)))
   {
      /* store information about which bits caused task to be unblocked */
      td_ptr->LWEVENT_BITS = event_ptr->VALUE & bit_mask;
      /* clear used automatic events */
      event_ptr->VALUE &= ~(event_ptr->AUTO & bit_mask);

      _INT_ENABLE();
      return(result);
   } /* Endif */

   /* Must wait for a event to become available */

   td_ptr->LWEVENT_BITS = bit_mask;
   if (all) {
      td_ptr->FLAGS |= TASK_LWEVENT_ALL_BITS_WANTED;
/* Start CR 1166 */
   } else {
      td_ptr->FLAGS &= ~TASK_LWEVENT_ALL_BITS_WANTED;
/* End CR   1166 */
   } /* Endif */

   /* Enqueue at end */   
   _QUEUE_ENQUEUE(&event_ptr->WAITING_TASKS, &td_ptr->AUX_QUEUE);

   /* Now put the task to sleep */
   td_ptr->STATE = LWEVENT_BLOCKED;
   td_ptr->INFO  = (_mqx_uint)&event_ptr->WAITING_TASKS;      
   if (tick_ptr) {
      if (ticks_are_absolute) {
         _time_delay_until(tick_ptr);
      } else {
         _time_delay_for(tick_ptr);
      } /* Endif */
      if (td_ptr->INFO) {
         /* Must have timed out */
/* Start CR 544 */
         /*_QUEUE_REMOVE(&event_ptr->WAITING_TASKS, &td_ptr->AUX_QUEUE);*/
/* End CR 544 */
         result = LWEVENT_WAIT_TIMEOUT;
      } /* Endif */
   } else {
      _task_block();
   } /* Endif */

#if MQX_COMPONENT_DESTRUCTION
   if (event_ptr->VALID == 0) {  /* We've been deleted */
      result = MQX_LWEVENT_INVALID;
   } /* Endif */
#endif

   _INT_ENABLE();
   return(result);

} /* Endbody */
#endif /* MQX_USE_LWEVENTS */

/* EOF */
