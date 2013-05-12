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
* $FileName: ti_task.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for the timer task.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_TIMER
#include "timer.h"
#include "tim_prv.h"

/*TASK*------------------------------------------------------------
*
* Task Name       : _timer_task
* Comments        : This task runs the timer queues, calling
*    the notification functions as required.
*
*END*------------------------------------------------------------------*/

void _timer_task
   (
      uint_32 fake_parameter
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   TIMER_COMPONENT_STRUCT_PTR  timer_component_ptr;
   TIMER_ENTRY_STRUCT_PTR      timer_entry_ptr;
   QUEUE_STRUCT_PTR            queue_ptrs[2];
   QUEUE_STRUCT_PTR            queue_ptr;
#if MQX_TIMER_USES_TICKS_ONLY==0
   TIME_STRUCT                 time;
#endif
   MQX_TICK_STRUCT             ticks;
   _mqx_uint                   i;
   _mqx_int                    result;

   _GET_KERNEL_DATA(kernel_data);
   
   timer_component_ptr = kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
#if MQX_CHECK_ERRORS
   if (timer_component_ptr == NULL){
      _task_block();
   } /* Endif */
#endif

   timer_component_ptr->TIMER_TD_PTR = kernel_data->ACTIVE_PTR;
   _QUEUE_INIT(&timer_component_ptr->ELAPSED_TIMER_ENTRIES, 0);
   _QUEUE_INIT(&timer_component_ptr->KERNEL_TIMER_ENTRIES, 0);
   _lwsem_create(&timer_component_ptr->TIMER_ISR_LWSEM, 0);
   _lwsem_create(&timer_component_ptr->TIMER_ENTRIES_LWSEM, 1);
    timer_component_ptr->VALID = TIMER_VALID;

   kernel_data->TIMER_COMPONENT_ISR  = _timer_isr;
   queue_ptrs[0] = &timer_component_ptr->ELAPSED_TIMER_ENTRIES;
   queue_ptrs[1] = &timer_component_ptr->KERNEL_TIMER_ENTRIES;

   while (TRUE) {

      for (i = 0; i < 2; ++i) {

         queue_ptr = queue_ptrs[i];

         /* Stop tasks from accessing the timer entries table */
         if (_lwsem_wait(&timer_component_ptr->TIMER_ENTRIES_LWSEM) != MQX_OK) {
            _task_set_error(MQX_INVALID_LWSEM);
            _task_block();
         } /* Endif */
 
         while (queue_ptr->SIZE > 0) {

           /* 
           ** Check the first entry on the queue to see if it has expired 
           ** Note that entries are always put into the list in sorted order.
           */
            timer_entry_ptr = (TIMER_ENTRY_STRUCT_PTR)
               ((pointer)queue_ptr->NEXT);
            if (i == 0) {
               _time_get_elapsed_ticks(&ticks);
            } else {
               _time_get_ticks(&ticks);
            } /* Endif */

            result = PSP_CMP_TICKS(&timer_entry_ptr->EXPIRATION_TIME, &ticks);

            if (result <= 0) {
               /* This entry has expired, remove it from the queue */
               _QUEUE_REMOVE(queue_ptr, timer_entry_ptr);
   
               timer_component_ptr->ENTRY_PTR = timer_entry_ptr;

               /* fire its notification function */

              /* START CR 1773 */
              if (timer_entry_ptr->VALID) { 
              /* END CR 1773 */

#if MQX_TIMER_USES_TICKS_ONLY==0
                 if (!timer_entry_ptr->USES_TICKS) {
                    PSP_TICKS_TO_TIME(&ticks, &time);
                    (*timer_entry_ptr->NOTIFICATION_FUNCTION)(
                       /* Start CR 1187 */
                       timer_entry_ptr->ID, 
                       /* End CR 1187 */
                       timer_entry_ptr->NOTIFICATION_DATA_PTR, time.SECONDS, 
                       time.MILLISECONDS);
                 } else {
#endif
                    (*timer_entry_ptr->NOTIFICATION_FUNCTION)(
                       /* Start CR 1187 */
                       timer_entry_ptr->ID, 
                       /* End CR 1187 */
                       timer_entry_ptr->NOTIFICATION_DATA_PTR, 
                       (pointer)&ticks);
#if MQX_TIMER_USES_TICKS_ONLY==0
                 } /* Endif */
#endif
              /* START CR 1773 */
              }
              /* END CR 1773 */ 
               
               /* This entry may be a one-shot, in which case it is to be deleted */
               if (IS_ONESHOT(timer_entry_ptr->TIMER_TYPE) ||
                  (timer_entry_ptr->VALID==0)) 
               {
                  timer_entry_ptr->VALID = 0;
                  _mem_free(timer_entry_ptr);
               } else {
                  /* Calculate next firing period */
                  PSP_ADD_TICKS(&timer_entry_ptr->EXPIRATION_TIME, 
                     &timer_entry_ptr->CYCLE, 
                     &timer_entry_ptr->EXPIRATION_TIME);
                  _timer_insert_queue_internal(queue_ptr, timer_entry_ptr);
               }/* Endif */

            } else {
               /* This queue does not require work */
               break;
            }/* Endif */

         } /* Endwhile */

         if (_lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM) !=
            MQX_OK)
         {
             _task_set_error(MQX_INVALID_LWSEM);
             _task_block();
         } /* Endif */

      } /* Endfor */

       /* wait until activation from the kernel timer isr */
       if (_lwsem_wait(&timer_component_ptr->TIMER_ISR_LWSEM) != MQX_OK) {
          _task_set_error(MQX_INVALID_LWSEM);
          _task_block();
       } /* Endif */

   } /* Endwhile */
   
} /* Endbody */


/*FUNCTION*--------------------------------------------------------------------
*
* Function Name    : _timer_isr
* Returned Value   : none
* Comments         :
*   check the timer queues, and if a timer is pending
* schedule the timer task to run.   This function is called from the
* kernel timer isr.
*
*END*-------------------------------------------------------------------------*/

void _timer_isr
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   TIMER_COMPONENT_STRUCT_PTR  timer_component_ptr;
   TIMER_ENTRY_STRUCT_PTR      timer_entry_ptr;
   MQX_TICK_STRUCT             kernel_time;
   _mqx_int                    result;
   
   _GET_KERNEL_DATA(kernel_data);
   
   timer_component_ptr = kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
#if MQX_CHECK_ERRORS
   if (timer_component_ptr == NULL){
      return;
   } /* Endif */
#endif
 
   if (timer_component_ptr->TIMER_TD_PTR->STATE == READY) {
      return;
   }/* Endif */

   _time_get_elapsed_ticks(&kernel_time);
   if (_QUEUE_GET_SIZE(&timer_component_ptr->ELAPSED_TIMER_ENTRIES)) {
      timer_entry_ptr = (pointer)timer_component_ptr->ELAPSED_TIMER_ENTRIES.NEXT;
      result = PSP_CMP_TICKS(&kernel_time, &timer_entry_ptr->EXPIRATION_TIME);
      if (result >= 0) {
         _lwsem_post(&timer_component_ptr->TIMER_ISR_LWSEM);
         return;
      }/* Endif */
   }/* Endif */

   if (_QUEUE_GET_SIZE(&timer_component_ptr->KERNEL_TIMER_ENTRIES)) {
      PSP_ADD_TICKS(&kernel_time, &kernel_data->TIME_OFFSET, &kernel_time);
      timer_entry_ptr = (pointer)timer_component_ptr->KERNEL_TIMER_ENTRIES.NEXT;
      result = PSP_CMP_TICKS(&kernel_time, &timer_entry_ptr->EXPIRATION_TIME);
      if (result >= 0) {
         _lwsem_post(&timer_component_ptr->TIMER_ISR_LWSEM);
         return;
      }/* Endif */
   }/* Endif */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _timer_insert_queue_internal
* Returned Value   : none
* Comments         :
*   insert timer_entry_ptr into the queue by time, with lowest time first
*
*END*-------------------------------------------------------------------------*/

void _timer_insert_queue_internal
   (
      /* The queue to insert onto */
      register QUEUE_STRUCT_PTR         queue_ptr,
      
      /* The entry to insert into the queue */
      register TIMER_ENTRY_STRUCT_PTR   timer_entry_ptr
   )      
{ /* Body */
   /* The element on the queue to start inserting from */
   register TIMER_ENTRY_STRUCT_PTR   element_ptr;
   register TIMER_ENTRY_STRUCT_PTR   prev_ptr;
   register _mqx_int                 result;
           
   prev_ptr = NULL;
   element_ptr = (pointer)queue_ptr->NEXT;
   while (element_ptr != (pointer)queue_ptr) {
      result = PSP_CMP_TICKS(&element_ptr->EXPIRATION_TIME, 
         &timer_entry_ptr->EXPIRATION_TIME);
      if (MQX_DELAY_ENQUEUE_POLICY(result)) { /* CR172 */
         /* Found the spot to insert it */
         break;
      }/* Endif */
      prev_ptr = element_ptr;
      element_ptr = (pointer)element_ptr->QUEUE_ELEMENT.NEXT;
   } /* Endwhile */

   /* Start CR 622 */
   _queue_insert(queue_ptr, (QUEUE_ELEMENT_STRUCT_PTR)((pointer)prev_ptr),
      (QUEUE_ELEMENT_STRUCT_PTR)((pointer)timer_entry_ptr));
   /* End CR 622 */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _timer_alloc_id_internal
* Returned Value  : _timer_id - Returns id of for a timer
* Comments         :
*   Create a new timer ID
*
*END*-------------------------------------------------------------------------*/

_timer_id _timer_alloc_id_internal
   (
      /* [IN] the address of the timer component structure */
      TIMER_COMPONENT_STRUCT_PTR timer_component_ptr
   )
{ /* Body */
   QUEUE_STRUCT_PTR       queue_ptr;
   TIMER_ENTRY_STRUCT_PTR timer_entry_ptr;
   _timer_id              id;
   boolean                ok;
   _mqx_uint              i;
   _mqx_uint              size;

   if (timer_component_ptr->ID == 0) {
      timer_component_ptr->ID = 1;
   } /* Endif */
   id = timer_component_ptr->ID;

   /* Make sure ID not in use */
   ok = FALSE;
   while (!ok) {
      ok = TRUE;
      for (i = 0; (i < 2) && ok; i++) {
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
               ok = FALSE;
               id++;
               break;
            } /* Endif */
            timer_entry_ptr = (TIMER_ENTRY_STRUCT_PTR)
               ((pointer)timer_entry_ptr->QUEUE_ELEMENT.NEXT);
         } /* Endwhile */
      } /* Endfor */
   } /* Endwhile */

   timer_component_ptr->ID = id + 1;

   return(id);

} /* Endbody */
#endif /* MQX_USE_TIMER */

/* EOF */
