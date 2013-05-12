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
* $FileName: mu_lock.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for locking a mutex.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_lock
* Returned Value   : _mqx_uint MQX_EOK on success or POSIX error code
* Comments         :
*   This function locks a mutex.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_lock
   (
      /* [IN] -  The address of the mutex */
      register MUTEX_STRUCT_PTR mutex_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   register TD_STRUCT_PTR          td_ptr;
   volatile uchar _PTR_            lock_ptr;
            _mqx_uint              priority;
            _mqx_uint              count;
#if MQX_MUTEX_HAS_POLLING
            _mqx_uint              limited;
#endif

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_mutex_lock, mutex_ptr);

   td_ptr = kernel_data->ACTIVE_PTR;

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_mutex_lock, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
   if (mutex_ptr == NULL) {
      _KLOGX2(KLOG_mutex_lock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (mutex_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_lock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   if (mutex_ptr->OWNER_TD == (pointer)td_ptr) {
      _KLOGX2(KLOG_mutex_lock, MQX_EDEADLK);
      return(MQX_EDEADLK);
   } /* Endif */

   priority = td_ptr->MY_QUEUE->PRIORITY;
   lock_ptr = &mutex_ptr->LOCK;   

#if MQX_MUTEX_HAS_POLLING
   if ((mutex_ptr->PROTOCOLS & MUTEX_SPIN_ONLY) ||
       (mutex_ptr->PROTOCOLS & MUTEX_LIMITED_SPIN))
   {

      if (mutex_ptr->PROTOCOLS & MUTEX_SPIN_ONLY) {
         count   = 1;
         limited = 0;
      } else {
         count   = mutex_ptr->COUNT;
         limited = 1;
      } /* Endif */
      if (mutex_ptr->PROTOCOLS & MUTEX_PRIO_INHERIT) {

         _INT_DISABLE();
         while (count && *lock_ptr) {
            count -= limited;
            if (((TD_STRUCT_PTR)(mutex_ptr->OWNER_TD))->HOME_QUEUE->PRIORITY > 
               priority) 
            {
               if (((TD_STRUCT_PTR)(mutex_ptr->OWNER_TD))->MY_QUEUE->PRIORITY > 
                  priority) 
               {
                  _sched_boost_priority_internal(
                     (TD_STRUCT_PTR)mutex_ptr->OWNER_TD, priority);
               } /* Endif */
               mutex_ptr->BOOSTED++;
            } /* Endif */
            _INT_ENABLE(); /* Allow interruption and tasks of same prio a kick*/
            _sched_yield();
            _INT_DISABLE();
#if MQX_COMPONENT_DESTRUCTION
            if (mutex_ptr->VALID != MUTEX_VALID) {
               _int_enable();
               _KLOGX2(KLOG_mutex_lock, MQX_EINVAL);
               return(MQX_EINVAL);
            } /* Endif */
#endif
         } /* Endwhile */
         if (count) {
            /* Test and set sets the high bit. */
            mutex_ptr->LOCK = MQX_TEST_AND_SET_VALUE; 
         } /* Endif */
         _INT_ENABLE();

      } else {

         while (count && _mem_test_and_set((uchar_ptr)lock_ptr)) {
            count -= limited;
            _sched_yield();
#if MQX_COMPONENT_DESTRUCTION
            if (mutex_ptr->VALID != MUTEX_VALID) {
               _KLOGX2(KLOG_mutex_lock, MQX_EINVAL);
               return(MQX_EINVAL);
            } /* Endif */
#endif
         } /* Endwhile */

      } /* Endif */

      if (count == 0) {
         _KLOGX2(KLOG_mutex_lock, MQX_EBUSY);
         return(MQX_EBUSY);
      } /* Endif */
      mutex_ptr->OWNER_TD  = td_ptr;

   } else {

#endif

      _INT_DISABLE();
      if (*lock_ptr) {
         /* Must wait for a mutex to become available */

         /* Boost the priority of the owner task if necessary */
         if (mutex_ptr->PROTOCOLS & MUTEX_PRIO_INHERIT) {
            if (((TD_STRUCT_PTR)(mutex_ptr->OWNER_TD))->HOME_QUEUE->PRIORITY > 
               priority) 
            {
               if (((TD_STRUCT_PTR)(mutex_ptr->OWNER_TD))->MY_QUEUE->PRIORITY > 
                  priority) 
               {
                  _sched_boost_priority_internal(
                     (TD_STRUCT_PTR)mutex_ptr->OWNER_TD, priority);
               } /* Endif */
               mutex_ptr->BOOSTED++;
            } /* Endif */
         } /* Endif */

         _QUEUE_UNLINK(td_ptr); /* Remove from ready to run queue */
         td_ptr->INFO = (_mqx_uint)&mutex_ptr->WAITING_TASKS;
         if (mutex_ptr->PROTOCOLS & MUTEX_PRIORITY_QUEUEING) {
            /* 
            ** The waiting task must be inserted into the waiting list in order
            ** of the priority of the waiting task.
            */
            _sched_insert_priorityq_internal(&mutex_ptr->WAITING_TASKS, td_ptr);
         } else {
            /* Enqueue at end */   
            _QUEUE_ENQUEUE(&mutex_ptr->WAITING_TASKS, td_ptr);
         } /* Endif */

        /* Now put the task to sleep */
        td_ptr->STATE = MUTEX_BLOCKED;
        _sched_execute_scheduler_internal();

        /* If mutex alive, then active task owns it now, (done by unlock) */
#if MQX_COMPONENT_DESTRUCTION
        if (mutex_ptr->VALID != MUTEX_VALID) {
           _int_enable();
           _KLOGX2(KLOG_mutex_lock, MQX_EINVAL);
           return(MQX_EINVAL);
        } /* Endif */
#endif

      } else {
         /* Test and set sets the high bit. */
         mutex_ptr->LOCK      = MQX_TEST_AND_SET_VALUE; 
         mutex_ptr->OWNER_TD  = td_ptr;
      } /* Endif */

      /*
      ** This task now owns the mutex, if priority inheritance is in effect
      ** the queue of waiting tasks must be checked for any task which may
      ** have a higher priority than mine ONLY if priority queueing is NOT
      ** in effect
      */
      if ((mutex_ptr->PROTOCOLS & MUTEX_PRIO_INHERIT) &&
         !(mutex_ptr->PROTOCOLS & MUTEX_PRIORITY_QUEUEING))
      {
         count = 
            _sched_get_max_priority_on_q_internal(&mutex_ptr->WAITING_TASKS);
         if (count < td_ptr->HOME_QUEUE->PRIORITY) {
            if (count < td_ptr->MY_QUEUE->PRIORITY) {
               _sched_boost_priority_internal(td_ptr, count);
            } /* Endif */
            mutex_ptr->BOOSTED++;
         } /* Endif */
      } /* Endif */
      _INT_ENABLE();

#if MQX_MUTEX_HAS_POLLING
   } /* Endif */
#endif

   if (mutex_ptr->PROTOCOLS & MUTEX_PRIO_PROTECT) {
      /* Switch my priority to the higher one if necessary */
      _INT_DISABLE();
      if (mutex_ptr->PRIORITY_CEILING < td_ptr->HOME_QUEUE->PRIORITY) {
         if (mutex_ptr->PRIORITY_CEILING < td_ptr->MY_QUEUE->PRIORITY) {
            _sched_boost_priority_internal(td_ptr, mutex_ptr->PRIORITY_CEILING);
         } /* Endif */
         mutex_ptr->BOOSTED++;
      } /* Endif */
      _INT_ENABLE();
   } /* Endif */

   _KLOGX2(KLOG_mutex_lock, MQX_EOK);
   return(MQX_EOK);

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
