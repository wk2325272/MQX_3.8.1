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
* $FileName: ti_test.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function that tests the timer component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_TIMER
#include "timer.h"
#include "tim_prv.h"

/*FUNCTION*-----------------------------------------------------------
*
* Function Name   : _timer_test
* Return Value    : _mqx_uint - MQX_OK or error code
* Comments        : This function tests the timer component for validity
*   and consistency.
*
*END*------------------------------------------------------------------*/

_mqx_uint _timer_test
   (
      /* [OUT] the timer element in error */
      pointer _PTR_ timer_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   TIMER_COMPONENT_STRUCT_PTR timer_component_ptr;
   QUEUE_STRUCT_PTR           queue_ptr;
   TIMER_ENTRY_STRUCT_PTR     element_ptr;
   _mqx_uint                  result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_timer_test, timer_error_ptr);

   *timer_error_ptr = NULL;
  
   timer_component_ptr = kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
   if (timer_component_ptr == NULL) {
      _KLOGX2(KLOG_timer_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   /* Gain exclusive access to the timer queues */
   _lwsem_wait(&timer_component_ptr->TIMER_ENTRIES_LWSEM);

   result = _queue_test(&timer_component_ptr->ELAPSED_TIMER_ENTRIES,
      timer_error_ptr);
   if (result != MQX_OK) {
      _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
      _KLOGX3(KLOG_timer_test, result, *timer_error_ptr);
      return(result);
   } /* Endif */
   
   result = _queue_test(&timer_component_ptr->KERNEL_TIMER_ENTRIES,
      timer_error_ptr);
   if (result != MQX_OK) {
      _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
      _KLOGX3(KLOG_timer_test, result, *timer_error_ptr);
      return(result);
   } /* Endif */

   queue_ptr   = (pointer)&timer_component_ptr->ELAPSED_TIMER_ENTRIES;
   element_ptr = (pointer)queue_ptr->NEXT;
   while (element_ptr != (pointer)queue_ptr) {
#if MQX_CHECK_VALIDITY
      if (element_ptr->VALID != TIMER_VALID) {
         *timer_error_ptr = element_ptr;
         _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
         _KLOGX3(KLOG_timer_test, MQX_INVALID_COMPONENT_HANDLE,
            *timer_error_ptr);
         return(MQX_INVALID_COMPONENT_HANDLE);
      } /* Endif */
#endif
      element_ptr = (pointer)element_ptr->QUEUE_ELEMENT.NEXT;
   } /* Endwhile */

   queue_ptr   = (pointer)&timer_component_ptr->KERNEL_TIMER_ENTRIES;
   element_ptr = (pointer)queue_ptr->NEXT;
   while (element_ptr != (pointer)queue_ptr) {
#if MQX_CHECK_VALIDITY
      if (element_ptr->VALID != TIMER_VALID) {
         *timer_error_ptr = element_ptr;
         _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
         _KLOGX3(KLOG_timer_test, MQX_INVALID_COMPONENT_HANDLE,
            *timer_error_ptr);
         return(MQX_INVALID_COMPONENT_HANDLE);
      } /* Endif */
#endif
      element_ptr = (pointer)element_ptr->QUEUE_ELEMENT.NEXT;
   } /* Endwhile */

   _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);
   _KLOGX2(KLOG_timer_test, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_TIMER */

/* EOF */
