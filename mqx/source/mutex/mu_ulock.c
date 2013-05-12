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
* $FileName: mu_ulock.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for unlocking a mutex.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_unlock
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*   This function releases the mutex, so another task may use it.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_unlock
   (
      /* [IN] - the address of a mutex */
      register MUTEX_STRUCT_PTR mutex_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   register TD_STRUCT_PTR          td_ptr;
            _mqx_uint               boosted;
            
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_mutex_unlock, mutex_ptr);

   td_ptr = kernel_data->ACTIVE_PTR;
   
#if MQX_CHECK_ERRORS
   if (mutex_ptr == NULL) {
      _KLOGX2(KLOG_mutex_unlock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (mutex_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_unlock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (mutex_ptr->LOCK != MQX_TEST_AND_SET_VALUE) {
      _KLOGX2(KLOG_mutex_unlock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */      
   if (mutex_ptr->OWNER_TD != (pointer)td_ptr)  {
      _KLOGX2(KLOG_mutex_unlock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   _INT_DISABLE();
   boosted = mutex_ptr->BOOSTED;
   mutex_ptr->BOOSTED = 0;   

   if (mutex_ptr->PROTOCOLS & (MUTEX_SPIN_ONLY | MUTEX_LIMITED_SPIN)) {
      mutex_ptr->OWNER_TD = NULL;
      mutex_ptr->LOCK = 0;
   } else {

      if (_QUEUE_GET_SIZE(&mutex_ptr->WAITING_TASKS)) {

         /* Schedule a waiting task to run */
         _QUEUE_DEQUEUE(&mutex_ptr->WAITING_TASKS, td_ptr);
         mutex_ptr->OWNER_TD = td_ptr;
         _TASK_READY(td_ptr, kernel_data);
         if (mutex_ptr->PROTOCOLS & MUTEX_PRIO_PROTECT) {
            /* Switch priority to the higher one if necessary */
            if (mutex_ptr->PRIORITY_CEILING < td_ptr->HOME_QUEUE->PRIORITY) {
               if (mutex_ptr->PRIORITY_CEILING < td_ptr->MY_QUEUE->PRIORITY) {
                  _sched_boost_priority_internal(td_ptr,
                     mutex_ptr->PRIORITY_CEILING);
               } /* Endif */
               mutex_ptr->BOOSTED++;
            } /* Endif */
         } /* Endif */

         _CHECK_RUN_SCHEDULER();/* Let higher priority task run */

      } else {
         mutex_ptr->LOCK = 0;
         mutex_ptr->OWNER_TD = NULL;
      } /* Endif */

   } /* Endif */

   if (boosted) {
      _sched_unboost_priority_internal(kernel_data->ACTIVE_PTR, boosted);
   } /* Endif */
   _INT_ENABLE();      

   _KLOGX2(KLOG_mutex_unlock, MQX_EOK);
   return(MQX_EOK);

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
