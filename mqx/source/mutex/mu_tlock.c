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
* $FileName: mu_tlock.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for trying to lock a mutex.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_try_lock
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*   This function trys to get a mutex.  If it is not available,
* it simply returns.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_try_lock
   (
      /* [IN] -  The address of the mutex */
      register MUTEX_STRUCT_PTR mutex_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_mutex_try_lock, mutex_ptr);

#if MQX_CHECK_ERRORS
   if (mutex_ptr == NULL) {
      _KLOGX2(KLOG_mutex_try_lock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (mutex_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_try_lock, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   /* Start CR 711 */
   _INT_DISABLE();
   /* End CR 711 */
   if (_mem_test_and_set(&mutex_ptr->LOCK)) {
      /* Start CR 711 */
      _INT_ENABLE();
      /* End CR 711 */
      /* Lock value was set, can't have it */
      if (mutex_ptr->OWNER_TD == (pointer)kernel_data->ACTIVE_PTR) {
         _KLOGX2(KLOG_mutex_try_lock, MQX_EDEADLK);
         return(MQX_EDEADLK);
      } else {
         _KLOGX2(KLOG_mutex_try_lock, MQX_EBUSY);
         return(MQX_EBUSY);
      } /* Endif */
   } else {
      /* Lock value was not set, now it is, I own it */
      mutex_ptr->OWNER_TD  = kernel_data->ACTIVE_PTR;
      /* Start CR 711 */
      _INT_ENABLE();
      /* End CR 711 */

      if (mutex_ptr->PROTOCOLS & MUTEX_PRIO_PROTECT) {
         /* Switch my priority to the higher one if necessary */
         td_ptr = kernel_data->ACTIVE_PTR;
         _INT_DISABLE();
         if (mutex_ptr->PRIORITY_CEILING < td_ptr->HOME_QUEUE->PRIORITY) {
            if (mutex_ptr->PRIORITY_CEILING < td_ptr->MY_QUEUE->PRIORITY) {
               _sched_boost_priority_internal(td_ptr, mutex_ptr->PRIORITY_CEILING);
            } /* Endif */
            mutex_ptr->BOOSTED++;
         } /* Endif */
         _INT_ENABLE();
      } /* Endif */

      _KLOGX2(KLOG_mutex_try_lock, MQX_EOK);
      return(MQX_EOK);
   } /* Endif */

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
