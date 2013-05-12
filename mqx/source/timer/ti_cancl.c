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
* $FileName: ti_cancl.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for canceling a timer alarm.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_TIMER
#include "timer.h"
#include "tim_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _timer_cancel   
* Returned Value  : _mqx_uint MQX error code
* Comments        : 
*  This function will cancel an outstanding timer request                 
*
*END*------------------------------------------------------------------*/

_mqx_uint _timer_cancel
   (

      /* [IN]  id of the alarm to be cancelled  */
     _timer_id id

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   TIMER_COMPONENT_STRUCT_PTR  timer_component_ptr;
   TIMER_ENTRY_STRUCT_PTR      timer_entry_ptr;
   QUEUE_STRUCT_PTR            queue_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_timer_cancel, id);

   timer_component_ptr = kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
#if MQX_CHECK_ERRORS
   if (timer_component_ptr == NULL) {
      _KLOGX2(KLOG_timer_cancel, MQX_COMPONENT_DOES_NOT_EXIST);
      return MQX_COMPONENT_DOES_NOT_EXIST; 
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (timer_component_ptr->VALID != TIMER_VALID) {
      _KLOGX2(KLOG_timer_cancel, MQX_INVALID_COMPONENT_BASE);
      return MQX_INVALID_COMPONENT_BASE;
   } /* Endif */
#endif
#if MQX_CHECK_ERRORS
   if (id == 0) {
      _KLOGX2(KLOG_timer_cancel, MQX_INVALID_PARAMETER);
      return MQX_INVALID_PARAMETER;
   } /* Endif */

   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_timer_cancel, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return MQX_CANNOT_CALL_FUNCTION_FROM_ISR;
   } /* Endif */
#endif

   /* Gain exclusive access to the timer queues */
   /* Start CR 1332 */
   if (kernel_data->ACTIVE_PTR != timer_component_ptr->TIMER_TD_PTR) {
      if (_lwsem_wait(&timer_component_ptr->TIMER_ENTRIES_LWSEM) != MQX_OK) {
         _KLOGX2(KLOG_timer_cancel, MQX_INVALID_LWSEM);
         return(MQX_INVALID_LWSEM);
      } /* Endif */
   } /*Endif */
   timer_entry_ptr = _timer_find_entry_internal(timer_component_ptr, id);
   /* End CR 1332 */

   if ((timer_entry_ptr == NULL) || (timer_entry_ptr->VALID != TIMER_VALID)) {
      if (kernel_data->ACTIVE_PTR != timer_component_ptr->TIMER_TD_PTR) {
         _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
      } /* Endif */
      _KLOGX2(KLOG_timer_cancel, MQX_INVALID_PARAMETER);
      return MQX_INVALID_PARAMETER;
    } /* Endif */

   if (timer_entry_ptr->MODE == TIMER_ELAPSED_TIME_MODE) {
      queue_ptr = &timer_component_ptr->ELAPSED_TIMER_ENTRIES;
   } else {
      queue_ptr = &timer_component_ptr->KERNEL_TIMER_ENTRIES;
   } /* Endif */

   timer_entry_ptr->VALID = 0;
   timer_entry_ptr->ID = 0;
   if (kernel_data->ACTIVE_PTR != timer_component_ptr->TIMER_TD_PTR) {
      _QUEUE_REMOVE(queue_ptr, timer_entry_ptr);
      _mem_free(timer_entry_ptr);
      _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
   } /* Endif */

   _KLOGX2(KLOG_timer_cancel, MQX_OK);
   return(MQX_OK);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _timer_find_entry_internal
* Returned Value   : TIMER_ENTRY_STRUCT_PTR entry associated with timer ID
* Comments         :
*   locate a timer via its ID
*
*END*-------------------------------------------------------------------------*/

TIMER_ENTRY_STRUCT_PTR _timer_find_entry_internal
   (
      /* [IN] the address of the timer component structure */
      TIMER_COMPONENT_STRUCT_PTR timer_component_ptr,

      /* [IN] the timer id */
      _timer_id                  id
   )
{ /* Body */
   QUEUE_STRUCT_PTR       queue_ptr;
   TIMER_ENTRY_STRUCT_PTR timer_entry_ptr;
   _mqx_uint              i;
   _mqx_uint              size;

   for (i = 0; i < 2; i++) {
      if (i == 0) {      
         queue_ptr = &timer_component_ptr->ELAPSED_TIMER_ENTRIES;
      } else {
         queue_ptr = &timer_component_ptr->KERNEL_TIMER_ENTRIES;
      } /* Endif */
      timer_entry_ptr = (TIMER_ENTRY_STRUCT_PTR)((pointer)queue_ptr->NEXT);
      /* SPR P171-0023-01 use pre-decrement on while loop */
      size = queue_ptr->SIZE + 1;
      while (--size) {
      /* END SPR */
         if (timer_entry_ptr->ID == id) {
            return(timer_entry_ptr);
         } /* Endif */
         timer_entry_ptr = (TIMER_ENTRY_STRUCT_PTR)
            ((pointer)timer_entry_ptr->QUEUE_ELEMENT.NEXT);
      } /* Endwhile */
   } /* Endfor */

   return(NULL);

} /* Endbody */
#endif /* MQX_USE_TIMER */

/* EOF */
