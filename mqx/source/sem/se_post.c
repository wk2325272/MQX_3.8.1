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
* $FileName: se_post.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for posting a sempahore.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SEMAPHORES
#include "name.h"
#include "name_prv.h"
#include "sem.h"
#include "sem_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sem_post
* Returned Value   : _mqx_uint MQX_OK, or MQX_INVALID_COMPONENT_BASE,
*                       SEM_INVALID_SEMAPHORE_HANDLE, SEM_CANNOT_POST,
*                       SEM_INVALID_SEMAPHORE_COUNT, SEM_SEMAPHORE_NOT_FOUND
* Comments         :
*   This function returns a semaphore to the semaphore, so another
* task may use it.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_post
   (
      /* [IN] -  The semaphore handle returned by _sem_open. */
      pointer users_sem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   SEM_STRUCT_PTR            sem_ptr;
#if MQX_COMPONENT_DESTRUCTION
   SEM_COMPONENT_STRUCT_PTR  sem_component_ptr;
#endif
   SEM_CONNECTION_STRUCT_PTR new_sem_connection_ptr;
   SEM_CONNECTION_STRUCT_PTR sem_connection_ptr;
   TD_STRUCT_PTR             new_td_ptr;
   boolean                   task_added = FALSE;
   boolean                   destroying_semaphore = FALSE;
            
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_sem_post, users_sem_ptr);

   sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)users_sem_ptr;

#if MQX_CHECK_VALIDITY
   if (sem_connection_ptr->VALID != SEM_VALID) {
      _KLOGX2(KLOG_sem_post, SEM_INVALID_SEMAPHORE_HANDLE);
      return(SEM_INVALID_SEMAPHORE_HANDLE); 
   } /* Endif */
#endif

   sem_ptr = sem_connection_ptr->SEM_PTR;

#if MQX_CHECK_ERRORS
   if (sem_ptr->POLICY & SEM_STRICT) {
      if (kernel_data->IN_ISR) {
         _KLOGX2(KLOG_sem_post, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
         return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      } /* Endif */

      if (sem_connection_ptr->TD_PTR != kernel_data->ACTIVE_PTR) {
         /* ONLY OPENING task can use the semaphore */
         _KLOGX2(KLOG_sem_post, SEM_INVALID_SEMAPHORE_HANDLE);
         return(SEM_INVALID_SEMAPHORE_HANDLE); 
      } /* Endif */

      if (sem_connection_ptr->POST_STATE == 0) {
         _KLOGX2(KLOG_sem_post, SEM_CANNOT_POST);
         return(SEM_CANNOT_POST);
      } /* Endif */
   } /* Endif */
#endif

   _INT_DISABLE();
   
#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != SEM_VALID) {
      _int_enable();
      _KLOGX2(KLOG_sem_post, SEM_INVALID_SEMAPHORE);
      return(SEM_INVALID_SEMAPHORE);
   } /* Endif */
#endif

   if (sem_ptr->POLICY & SEM_STRICT) {
#if MQX_CHECK_ERRORS
      if (sem_ptr->COUNT > sem_ptr->MAX_COUNT) {
         /* Corruption somewhere */
         _int_enable();
         _KLOGX2(KLOG_sem_post, SEM_INVALID_SEMAPHORE_COUNT);
         return(SEM_INVALID_SEMAPHORE_COUNT);
      } /* Endif */
#endif

      --sem_connection_ptr->POST_STATE;
      if (sem_connection_ptr->POST_STATE == 0) {
         _QUEUE_REMOVE(&sem_ptr->OWNING_TASKS, sem_connection_ptr);
      } /* Endif */
   } /* Endif */


   if (_QUEUE_GET_SIZE(&sem_ptr->WAITING_TASKS)) {
      /* Schedule a waiting task to run */

      new_sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)
         ((pointer)sem_ptr->WAITING_TASKS.NEXT);
      while (new_sem_connection_ptr != (pointer)&sem_ptr->WAITING_TASKS) {
         new_td_ptr = new_sem_connection_ptr->TD_PTR;
         if ((new_td_ptr->STATE & STATE_MASK) == SEM_BLOCKED) {
            _TIME_DEQUEUE(new_td_ptr, kernel_data);
            _TASK_READY(new_td_ptr, kernel_data);
            new_td_ptr->INFO = SEM_AVAILABLE;
            task_added = TRUE;
            break;
         } /* Endif */
         new_sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)
            new_sem_connection_ptr->NEXT;
      } /* Endwhile */

   } /* Endif */
   
   if (!task_added) {

      ++sem_ptr->COUNT;

#if MQX_COMPONENT_DESTRUCTION
     if ( sem_ptr->DELAYED_DESTROY ) {
        if ( ( (sem_ptr->POLICY & SEM_STRICT) &&
               (sem_ptr->COUNT == sem_ptr->MAX_COUNT) ) ||
             ( !(sem_ptr->POLICY & SEM_STRICT) ) ) 
        {
           /* Destroy the semaphore name */
           sem_ptr->VALID = 0;
           destroying_semaphore = TRUE;
        } /* Endif */
      } /* Endif */
#endif
   } /* Endif */

   if (sem_connection_ptr->BOOSTED && (sem_connection_ptr->POST_STATE == 0)) {
      /* This task was boosted by a waiting task */
      _sched_unboost_priority_internal(kernel_data->ACTIVE_PTR, 
         sem_connection_ptr->BOOSTED);
      sem_connection_ptr->BOOSTED = 0;
   } /* Endif */

   _INT_ENABLE();

   /* Let higher priority task run */
   _CHECK_RUN_SCHEDULER();

#if MQX_COMPONENT_DESTRUCTION
   if (destroying_semaphore) {
      sem_component_ptr = (SEM_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_SEMAPHORES];
      if (sem_component_ptr != NULL) {
         _name_delete_internal(sem_component_ptr->NAME_TABLE_HANDLE,
            sem_ptr->NAME);
      } /* Endif */
      _mem_free(sem_ptr);
   } /* Endif */
#endif

   _KLOGX2(KLOG_sem_post, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
