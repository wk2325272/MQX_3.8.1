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
* $FileName: lwe_set.c$
* $Version : 3.8.7.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for setting the event.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWEVENTS
#include "lwevent.h"
#include "lwe_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _lwevent_set
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to set the specified event bits in an event.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _lwevent_set
   (
      /* [IN] - The address of the light weight event */
      LWEVENT_STRUCT_PTR   event_ptr, 

      /* [IN] - bit mask, each bit of which represents an event. */
      _mqx_uint bit_mask
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   QUEUE_ELEMENT_STRUCT_PTR    q_ptr;
   QUEUE_ELEMENT_STRUCT_PTR    next_q_ptr;
   TD_STRUCT_PTR               td_ptr;
   _mqx_uint                   set_bits;
   
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwevent_set(event_ptr, bit_mask);
    }
#endif
            
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_lwevent_set, event_ptr, bit_mask);

   _INT_DISABLE();
#if MQX_CHECK_VALIDITY
   if (event_ptr->VALID != LWEVENT_VALID) {
      _int_enable();
      _KLOGX2(KLOG_lwevent_set, MQX_LWEVENT_INVALID);
      return(MQX_LWEVENT_INVALID);
   } /* Endif */
#endif

   set_bits = event_ptr->VALUE | bit_mask;

   if (_QUEUE_GET_SIZE(&event_ptr->WAITING_TASKS)) {
      /* Schedule waiting task(s) to run if bits ok */

      q_ptr = event_ptr->WAITING_TASKS.NEXT;
      while (q_ptr != (QUEUE_ELEMENT_STRUCT_PTR)
         ((pointer)&event_ptr->WAITING_TASKS))
      {
         td_ptr = (pointer)q_ptr;
         _BACKUP_POINTER(td_ptr, TD_STRUCT, AUX_QUEUE);
         next_q_ptr = q_ptr->NEXT;
         if (((td_ptr->FLAGS & TASK_LWEVENT_ALL_BITS_WANTED) && 
            ((td_ptr->LWEVENT_BITS & set_bits) == td_ptr->LWEVENT_BITS)) ||
            ((!(td_ptr->FLAGS & TASK_LWEVENT_ALL_BITS_WANTED)) && 
            (td_ptr->LWEVENT_BITS & set_bits)))
         {  
/* Start CR 406 */
#if 0
            _QUEUE_DEQUEUE(&event_ptr->WAITING_TASKS, q_ptr);
#endif
            _QUEUE_REMOVE(&event_ptr->WAITING_TASKS, q_ptr);
/* End CR 406 */
            _TIME_DEQUEUE(td_ptr, kernel_data);
            td_ptr->INFO = 0;
            _TASK_READY(td_ptr, kernel_data);

            /* store information about which bits caused task to be unblocked */
            td_ptr->LWEVENT_BITS &= set_bits;
            set_bits &= ~(event_ptr->AUTO & td_ptr->LWEVENT_BITS);
            
         } /* Endif */
         q_ptr = next_q_ptr;
      } /* Endwhile */
   } /* Endif */

   event_ptr->VALUE = set_bits;
   _INT_ENABLE();

   /* May need to let higher priority task run */
   _CHECK_RUN_SCHEDULER();

   _KLOGX2(KLOG_lwevent_set, MQX_OK);
   return(MQX_OK);
         
}

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _usr_lwevent_set
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to set the specified event bits in an event.
*
* 
*END*------------------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

_mqx_uint _usr_lwevent_set
   (
      /* [IN] - The address of the light weight event */
      LWEVENT_STRUCT_PTR   event_ptr, 

      /* [IN] - bit mask, each bit of which represents an event. */
      _mqx_uint bit_mask
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)event_ptr, (uint_32)bit_mask, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWEVENT_SET, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWEVENTS */

/* EOF */
