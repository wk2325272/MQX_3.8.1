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
* $FileName: mu_init.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for initializing a mutex.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_init
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*    This function creates an instance of a mutex.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_init
   (
      /* [IN] - the address where the mutex is to be initialized */
      register MUTEX_STRUCT_PTR      mutex_ptr,

      /* [IN]  - Initialization parameters for the mutex  */
      register MUTEX_ATTR_STRUCT_PTR attr_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   MUTEX_COMPONENT_STRUCT_PTR mutex_component_ptr;
   MUTEX_ATTR_STRUCT          default_attr;
#if MQX_CHECK_ERRORS
   MUTEX_STRUCT_PTR           mutex_chk_ptr;
#endif
   _mqx_uint                   result;

   _GET_KERNEL_DATA(kernel_data);
   if (attr_ptr == NULL) {
      attr_ptr = &default_attr;
      _mutatr_init(attr_ptr);
      _KLOGE3(KLOG_mutex_init, mutex_ptr, NULL);
   } else {
      _KLOGE3(KLOG_mutex_init, mutex_ptr, attr_ptr);
   } /* Endif */
   
#if MQX_CHECK_ERRORS
   if (mutex_ptr == NULL) {
      _KLOGX2(KLOG_mutex_init, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (attr_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_init, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   mutex_component_ptr = (MUTEX_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES];
   if (mutex_component_ptr == NULL) {
      result = _mutex_create_component();
      mutex_component_ptr = (MUTEX_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES];
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (mutex_component_ptr == NULL){
         _KLOGX2(KLOG_mutex_init, result);
         return(result);
      } /* Endif */
#endif
   } /* Endif */

#if MQX_CHECK_VALIDITY
   if (mutex_component_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_init, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   _int_disable();
#if MQX_CHECK_ERRORS
   /* Check if mutex is already initialized */
   mutex_chk_ptr = (MUTEX_STRUCT_PTR)
      ((pointer)mutex_component_ptr->MUTEXES.NEXT);
   while (mutex_chk_ptr != (MUTEX_STRUCT_PTR)
      ((pointer)&mutex_component_ptr->MUTEXES))
   {
      if (mutex_chk_ptr == mutex_ptr) {
         _int_enable();
         _KLOGX2(KLOG_mutex_init, MQX_EINVAL);
         return(MQX_EINVAL);
      } /* Endif */
      mutex_chk_ptr = (MUTEX_STRUCT_PTR)((pointer)mutex_chk_ptr->LINK.NEXT);
   } /* Endif */
#endif
   
   mutex_ptr->PROTOCOLS        = 
      attr_ptr->SCHED_PROTOCOL | attr_ptr->WAIT_PROTOCOL;
   mutex_ptr->VALID            = MUTEX_VALID;
   mutex_ptr->COUNT            = attr_ptr->COUNT;
   mutex_ptr->PRIORITY_CEILING = attr_ptr->PRIORITY_CEILING;
   mutex_ptr->LOCK             = 0;
   mutex_ptr->BOOSTED          = 0;
   mutex_ptr->OWNER_TD         = NULL;
   _QUEUE_INIT(&mutex_ptr->WAITING_TASKS, 0);

   _QUEUE_ENQUEUE(&mutex_component_ptr->MUTEXES, mutex_ptr);
   _int_enable();

   _KLOGX2(KLOG_mutex_init, MQX_EOK);
   return(MQX_EOK);
   
} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
