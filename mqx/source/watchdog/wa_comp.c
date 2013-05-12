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
* $FileName: wa_comp.c$
* $Version : 3.8.7.1$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for creating the watchdog
*   component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SW_WATCHDOGS
#include "watchdog.h"
#include "wdog_prv.h"

/*FUNCTION*-----------------------------------------------------------
*
* Function Name   : _watchdog_create_component
* Return Value    : _mqx_uint MQX_OK or an error code
* Comments        : 
*   This is the code that installs the watchdog component into the kernel.
*   Note. Cannot be called from an ISR
*
*END*------------------------------------------------------------------*/

_mqx_uint _watchdog_create_component
   (
      /* [IN] the vector upon which timer interrupts on */
      _mqx_uint        timer_interrupt_vector,
      
      /* [IN] the function to call when a watchdog timer expires */
      void (_CODE_PTR_ error_function)(pointer td_ptr)
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   WATCHDOG_COMPONENT_STRUCT_PTR watchdog_component_ptr;
   pointer                       interrupt_data;

   _GET_KERNEL_DATA(kernel_data);
   
   _KLOGE3(KLOG_watchdog_create_component, timer_interrupt_vector, error_function);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_watchdog_create_component, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   _lwsem_wait((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_WATCHDOG] != NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _KLOGX2(KLOG_watchdog_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */

#if MQX_CHECK_ERRORS
   if (!error_function) {
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _KLOGX2(KLOG_watchdog_create_component, WATCHDOG_INVALID_ERROR_FUNCTION);
      return(WATCHDOG_INVALID_ERROR_FUNCTION);
   } /* Endif */
   if ((timer_interrupt_vector < kernel_data->FIRST_USER_ISR_VECTOR) ||
       (timer_interrupt_vector > kernel_data->LAST_USER_ISR_VECTOR))
   {
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _KLOGX2(KLOG_watchdog_create_component, WATCHDOG_INVALID_INTERRUPT_VECTOR);
      return(WATCHDOG_INVALID_INTERRUPT_VECTOR);
   } /* Endif */
#endif
   
   /* Get the watchdog component data structure */
   watchdog_component_ptr = (WATCHDOG_COMPONENT_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(WATCHDOG_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (watchdog_component_ptr == NULL){
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _KLOGX2(KLOG_watchdog_create_component, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif   
   _mem_set_type(watchdog_component_ptr, MEM_TYPE_WATCHDOG_COMPONENT);

   watchdog_component_ptr->ERROR_FUNCTION   =
      (void (_CODE_PTR_)(TD_STRUCT_PTR))error_function;
   watchdog_component_ptr->VALID            = WATCHDOG_VALID;
   watchdog_component_ptr->INTERRUPT_VECTOR = timer_interrupt_vector;

   interrupt_data = _int_get_isr_data(timer_interrupt_vector);
   _INT_DISABLE();
   watchdog_component_ptr->TIMER_INTERRUPT_HANDLER = _int_install_isr(
      timer_interrupt_vector, _watchdog_isr, interrupt_data);
#if MQX_CHECK_ERRORS
   if (!watchdog_component_ptr->TIMER_INTERRUPT_HANDLER) {
      _int_enable();
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _mem_free(watchdog_component_ptr);
      _KLOGX2(KLOG_watchdog_create_component, WATCHDOG_INVALID_INTERRUPT_VECTOR);
      return(WATCHDOG_INVALID_INTERRUPT_VECTOR);
   } /* Endif */
#endif

   kernel_data->KERNEL_COMPONENTS[KERNEL_WATCHDOG] = watchdog_component_ptr;
#if MQX_COMPONENT_DESTRUCTION
   kernel_data->COMPONENT_CLEANUP[KERNEL_WATCHDOG] = _watchdog_cleanup;
#endif
   _INT_ENABLE();
   _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));

   _KLOGX2(KLOG_watchdog_create_component, MQX_OK);

   return(MQX_OK);

} /* Endbody */


/*ISR*------------------------------------------------------------------
*
* Interrupt Handler : _watchdog_isr
* Comments        : This function takes over from the timer interrupt
*  handler in order to process watchdog timeouts.
*
*END*------------------------------------------------------------------*/

void _watchdog_isr
   (
      /*  [IN]  the parameter passed by the kernel */
      pointer parameter
   )
{ /* Body */
   MQX_TICK_STRUCT                 kd_time;
   KERNEL_DATA_STRUCT_PTR          kernel_data;
   TD_STRUCT_PTR                   td_ptr;
   WATCHDOG_COMPONENT_STRUCT_PTR   watchdog_component_ptr;
   WATCHDOG_ALARM_TABLE_STRUCT_PTR table_ptr;
   _mqx_uint                       i;
   _mqx_int                        result;

   

   _GET_KERNEL_DATA(kernel_data);
   watchdog_component_ptr = (WATCHDOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_WATCHDOG];
   
   /* execute standard timer routine (in case of BSP timer, the 
      _time_notify_kernel() may get invoked now) */ 
   (*watchdog_component_ptr->TIMER_INTERRUPT_HANDLER)(parameter);

#if MQX_CHECK_ERRORS
   if (watchdog_component_ptr == NULL) {
      (*kernel_data->DEFAULT_ISR)((pointer)kernel_data->SYSTEM_CLOCK_INT_NUMBER);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (watchdog_component_ptr->VALID != WATCHDOG_VALID) {
      (*kernel_data->DEFAULT_ISR)((pointer)kernel_data->SYSTEM_CLOCK_INT_NUMBER);
   } /* Endif */
#endif

   table_ptr = &watchdog_component_ptr->ALARM_ENTRIES;
   while (table_ptr != NULL) {
      for (i = 0; i < WATCHDOG_TABLE_SIZE; ++i) {
         td_ptr = table_ptr->TD_PTRS[i];
         if (td_ptr != NULL) {
#if MQX_TASK_DESTRUCTION
            if (! (td_ptr->FLAGS & TASK_WATCHDOG_STARTED)) {
               /* The td has been destroyed and re-created */
               table_ptr->TD_PTRS[i] = NULL;
            } else if (td_ptr->TASK_ID == 0) {
               /* The task has been destroyed */
               table_ptr->TD_PTRS[i] = NULL;
            } else
#endif
            if (td_ptr->FLAGS & TASK_WATCHDOG_RUNNING) {
               /* An enabled watchdog */

               /* A timer interrupt is about to fire so increment kernel time */
               kd_time = kernel_data->TIME;
               result = PSP_CMP_TICKS(&kd_time, &td_ptr->WATCHDOG_TIME);
               if (result >= 0) {
                  /* EXPIRED */
                  td_ptr->FLAGS &= ~TASK_WATCHDOG_RUNNING;
                  (*watchdog_component_ptr->ERROR_FUNCTION)(td_ptr);
               } /* Endif */
            } /* Endif */
         } /* Endif */
      } /* Endfor */
      table_ptr = table_ptr->NEXT_TABLE_PTR;

   } /* Endwhile */
   
} /* Endbody */
#endif /* MQX_USE_SW_WATCHDOGS */

/* EOF */
