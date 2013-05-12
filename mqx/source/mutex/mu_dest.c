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
* $FileName: mu_dest.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for destroying a mutex and a mutex
*   attributes structure.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutatr_destroy
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*    Used by a task to destroy a mutex attributes structure.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mutatr_destroy
   (
      /* [IN] the address of the mutex attributes structure */
      MUTEX_ATTR_STRUCT_PTR attr_ptr
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE2(KLOG_mutatr_destroy, attr_ptr);

#if MQX_CHECK_ERRORS
   if (attr_ptr == NULL) {
      _KLOGX2(KLOG_mutatr_destroy, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (attr_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutatr_destroy, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   attr_ptr->VALID    = 0;

   _KLOGX2(KLOG_mutatr_destroy, MQX_EOK);
   return(MQX_EOK);
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_destroy
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*   This function removes an instance of a mutex from the system.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_destroy
   (
      /* [IN]  - the address of the mutex to destroy */
      register MUTEX_STRUCT_PTR mutex_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   MUTEX_COMPONENT_STRUCT_PTR mutex_component_ptr;
   TD_STRUCT_PTR              td_ptr;
   _mqx_uint                  result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_mutex_destroy, mutex_ptr);

#if MQX_CHECK_ERRORS
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES] == NULL) {
      _KLOGX2(KLOG_mutex_destroy, MQX_COMPONENT_DOES_NOT_EXIST);
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif

   mutex_component_ptr = (MUTEX_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES];

#if MQX_CHECK_VALIDITY
   if (mutex_component_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_destroy, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   td_ptr = kernel_data->ACTIVE_PTR;

   if (mutex_ptr->OWNER_TD != td_ptr) {
      result = _mutex_lock(mutex_ptr);  /* Get the mutex */
      if (result != MQX_EOK) {
         _KLOGX2(KLOG_mutex_destroy, result);
         return(result);
      } /* Endif */
   } /* Endif */
   
   mutex_ptr->VALID = 0;

   if (mutex_ptr->LINK.NEXT != NULL) {
      /* Remove mutex from the queue of mutexes in the kernel */
      _int_disable();
      _QUEUE_REMOVE(&mutex_component_ptr->MUTEXES, mutex_ptr);
      _int_enable();
   } /* Endif */
   
   /* Get rid of all waiting tasks */
   while (_QUEUE_GET_SIZE(&mutex_ptr->WAITING_TASKS)) {
      _QUEUE_DEQUEUE(&mutex_ptr->WAITING_TASKS, td_ptr);
      _task_set_error_td_internal(td_ptr, MQX_EINVAL);
      _int_disable();
      _TASK_READY(td_ptr,kernel_data);
      _int_enable();
   } /* Endwhile */

   td_ptr = kernel_data->ACTIVE_PTR;

   _int_disable();
   if (mutex_ptr->BOOSTED) {
      _sched_unboost_priority_internal(td_ptr, mutex_ptr->BOOSTED);
   } /* Endif */

   _int_enable();

   _CHECK_RUN_SCHEDULER(); /* Let higher priority task run */

   _KLOGX2(KLOG_mutex_destroy, MQX_EOK);
   return(MQX_EOK);

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
