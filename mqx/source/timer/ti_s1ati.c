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
* $FileName: ti_s1ati.c$
* $Version : 3.8.6.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for starting a oneshot timer
*   at a certain time.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_TIMER
#include "timer.h"
#include "tim_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _timer_start_oneshot_at_internal
* Returned Value  : _timer_id -  id of created timer or null on error
* Comments        : initialize a timer to fire once at a specified time.
*
*END*------------------------------------------------------------------*/

_timer_id _timer_start_oneshot_at_internal
   ( 

      /* [IN] the function to call when the timer expires */
      void (_CODE_PTR_    notification_function_ptr)(),

      /* [IN] the data to pass to the function when the timer expires */
      pointer             notification_data_ptr,
      
      /* 
      ** [IN] which time to use when calculating time to fire 
      **   TIMER_ElAPSED_TIME_MODE
      **   TIMER_KERNEL_TIME_MODE
      */
      _mqx_uint           mode, 
      
      /* 
      ** [IN] the time at which to call the notification
      ** function, and then cancel the timer.
      */
      MQX_TICK_STRUCT_PTR tick_ptr,

      /* [IN] Set to true if noti. function uses ticks */
      boolean             uses_ticks
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   TIMER_COMPONENT_STRUCT_PTR timer_component_ptr;
   TIMER_ENTRY_STRUCT_PTR     timer_entry_ptr;
   _mqx_uint                  error;
   
   _GET_KERNEL_DATA(kernel_data);

   timer_component_ptr = (TIMER_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
   if (timer_component_ptr == NULL) {
      error = _timer_create_component(TIMER_DEFAULT_TASK_PRIORITY,
         TIMER_DEFAULT_STACK_SIZE);
      timer_component_ptr = (TIMER_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
#if MQX_CHECK_ERRORS
      if (timer_component_ptr == NULL) {
        _task_set_error(error);
        return TIMER_NULL_ID;
      } /* Endif */
#endif
   }/* Endif */

#if MQX_CHECK_VALIDITY
   if (timer_component_ptr->VALID != TIMER_VALID) {
      _task_set_error(MQX_INVALID_COMPONENT_BASE);
      return TIMER_NULL_ID;
   } /* Endif */
#endif
#if MQX_CHECK_ERRORS
   if ((notification_function_ptr == NULL) || 
     ! ((mode == TIMER_ELAPSED_TIME_MODE) || (mode == TIMER_KERNEL_TIME_MODE))
      )
   {
      _task_set_error(MQX_INVALID_PARAMETER);
      return TIMER_NULL_ID;
   } /* Endif */

   error = (_mqx_uint)PSP_CMP_TICKS(&_mqx_zero_tick_struct, tick_ptr);
   if (error == 0) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return TIMER_NULL_ID;
   } /* Endif */
#endif

   timer_entry_ptr = (TIMER_ENTRY_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(TIMER_ENTRY_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (timer_entry_ptr == NULL) {
      _task_set_error(MQX_OUT_OF_MEMORY);
      return TIMER_NULL_ID;
   } /* Endif */
#endif
   _mem_set_type(timer_component_ptr, MEM_TYPE_TIMER_ENTRY);

   timer_entry_ptr->NOTIFICATION_FUNCTION = notification_function_ptr;
   timer_entry_ptr->NOTIFICATION_DATA_PTR = notification_data_ptr;
   timer_entry_ptr->MODE                  = (uint_16)mode;
   timer_entry_ptr->TIMER_TYPE            = TIMER_TYPE_ONESHOT_AT;
   timer_entry_ptr->TD_PTR                = kernel_data->ACTIVE_PTR;
   timer_entry_ptr->USES_TICKS            = uses_ticks;

   timer_entry_ptr->EXPIRATION_TIME = *tick_ptr;

   /* Gain exclusive access to the timer queues */
   if (kernel_data->ACTIVE_PTR != timer_component_ptr->TIMER_TD_PTR) {
      if (_lwsem_wait(&timer_component_ptr->TIMER_ENTRIES_LWSEM) != MQX_OK) {
         _task_set_error(MQX_INVALID_LWSEM);
         return(TIMER_NULL_ID);
      } /* Endif */
   } /* Endif */

/* START CR 308 */
   timer_entry_ptr->VALID = TIMER_VALID;
/* END CR 308 */

   timer_entry_ptr->ID    = _timer_alloc_id_internal(timer_component_ptr);

   if (mode == TIMER_ELAPSED_TIME_MODE)  {
      _timer_insert_queue_internal(&timer_component_ptr->ELAPSED_TIMER_ENTRIES,
         timer_entry_ptr);
   } else {
      _timer_insert_queue_internal(&timer_component_ptr->KERNEL_TIMER_ENTRIES,
         timer_entry_ptr);
   } /* Endif */
   if (kernel_data->ACTIVE_PTR != timer_component_ptr->TIMER_TD_PTR) {
      _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
   } /* Endif */
   
   return(timer_entry_ptr->ID);
   
} /* Endbody */
#endif /* MQX_USE_TIMER */

/* EOF */
