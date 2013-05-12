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
* $FileName: ti_tdel.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function called when a task is destroyed
*   in order to cleanup any connections for the timer component
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_TIMER
#include "timer.h"
#include "tim_prv.h"

/*FUNCTION*-----------------------------------------------------------
*
* Function Name   : _timer_cleanup
* Return Value    : none
* Comments        : This is the function that is called by a task during
*   task destruction, in order to cleanup any timers
*
*END*------------------------------------------------------------------*/

void _timer_cleanup
   (
      /* [IN] the task being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   TIMER_COMPONENT_STRUCT_PTR timer_component_ptr;
   QUEUE_STRUCT_PTR           queue_ptr;
   TIMER_ENTRY_STRUCT_PTR     element_ptr;
   TIMER_ENTRY_STRUCT_PTR     next_element_ptr;

   _GET_KERNEL_DATA(kernel_data);
   
   timer_component_ptr = kernel_data->KERNEL_COMPONENTS[KERNEL_TIMER];
   if (timer_component_ptr == NULL) {
      return;
   } /* Endif */

   /* Gain exclusive access to the timer queues */
   _lwsem_wait(&timer_component_ptr->TIMER_ENTRIES_LWSEM);

   queue_ptr   = (QUEUE_STRUCT_PTR)&timer_component_ptr->ELAPSED_TIMER_ENTRIES;
   element_ptr = (TIMER_ENTRY_STRUCT_PTR)((pointer)queue_ptr->NEXT);
   while ((pointer)element_ptr != (pointer)queue_ptr) {
      next_element_ptr = (TIMER_ENTRY_STRUCT_PTR)
         ((pointer)element_ptr->QUEUE_ELEMENT.NEXT);
      if (element_ptr->TD_PTR == td_ptr) {
         _QUEUE_REMOVE(queue_ptr, element_ptr);
         element_ptr->VALID = 0;
         _mem_free(element_ptr);
      } /* Endif */
      element_ptr = next_element_ptr;
   } /* Endwhile */

   queue_ptr   = (QUEUE_STRUCT_PTR)
      ((pointer)&timer_component_ptr->KERNEL_TIMER_ENTRIES);
   element_ptr = (TIMER_ENTRY_STRUCT_PTR)((pointer)queue_ptr->NEXT);
   while ((pointer)element_ptr != (pointer)queue_ptr) {
      next_element_ptr = (TIMER_ENTRY_STRUCT_PTR)
         ((pointer)element_ptr->QUEUE_ELEMENT.NEXT);
      if (element_ptr->TD_PTR == td_ptr) {
         _QUEUE_REMOVE(queue_ptr, element_ptr);
         element_ptr->VALID = 0;
         _mem_free(element_ptr);
      } /* Endif */
      element_ptr = next_element_ptr;
   } /* Endwhile */

   _lwsem_post(&timer_component_ptr->TIMER_ENTRIES_LWSEM);

} /* Endbody */
#endif /* MQX_USE_TIMER */

/* EOF */
