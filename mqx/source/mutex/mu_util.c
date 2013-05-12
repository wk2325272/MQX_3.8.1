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
* $FileName: mu_util.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains utility functions for the mutex component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_set_priority_ceiling
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*    Used by a task to change the priority ceiling of a mutex structure.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_set_priority_ceiling
   (
      /* [IN] the mutex address */
      MUTEX_STRUCT_PTR mutex_ptr,

      /* [IN] the ceiling to use */
      _mqx_uint        ceiling,

      /* [OUT] where the old ceiling is to be stored */
      _mqx_uint_ptr    old_ceiling_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   
   _GET_KERNEL_DATA(kernel_data);
   _KLOGE4(KLOG_mutex_set_priority_ceiling, mutex_ptr, ceiling, old_ceiling_ptr);

#if MQX_CHECK_ERRORS
   if (ceiling > kernel_data->LOWEST_TASK_PRIORITY) {
      _KLOGX2(KLOG_mutex_set_priority_ceiling, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   _int_disable();

#if MQX_CHECK_ERRORS
   if ((mutex_ptr == NULL) || (old_ceiling_ptr == NULL)) {
      _int_enable();
      _KLOGX2(KLOG_mutex_set_priority_ceiling, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (mutex_ptr->VALID != MUTEX_VALID) {
      _int_enable();
      _KLOGX2(KLOG_mutex_set_priority_ceiling, MQX_EINVAL);
      return(MQX_EINVAL);
   } /* Endif */
#endif

   *old_ceiling_ptr            = mutex_ptr->PRIORITY_CEILING;
   mutex_ptr->PRIORITY_CEILING = ceiling;
   _int_enable();
   
   _KLOGX2(KLOG_mutex_set_priority_ceiling, MQX_EOK);
   return(MQX_EOK);
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_get_priority_ceiling
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*    Used by a task to get the priority ceiling of a mutex structure.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_get_priority_ceiling
   (
      /* [IN] the mutex address */
      MUTEX_STRUCT_PTR mutex_ptr,

      /* [IN] the protocol address */
      _mqx_uint_ptr     ceiling_ptr
   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if ((mutex_ptr == NULL) || (ceiling_ptr == NULL)) {
      return(MQX_EINVAL);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (mutex_ptr->VALID != MUTEX_VALID) {
      return(MQX_EINVAL);
   } /* Endif */
#endif

   *ceiling_ptr = mutex_ptr->PRIORITY_CEILING;
   return(MQX_EOK);
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_get_wait_count
* Returned Value   : _mqx_uint current num tasks on waiting list or -1 on error
* Comments         :
*   This function returns the number of tasks waiting for the specified
* mutex
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_get_wait_count
   (
      /* [IN] -  The address of the mutex */
      register MUTEX_STRUCT_PTR mutex_ptr
   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if (mutex_ptr == NULL) {
      _task_set_error(MQX_EINVAL);
      return(MAX_MQX_UINT); 
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (mutex_ptr->VALID != MUTEX_VALID) {
      _task_set_error(MQX_EINVAL);
      return(MAX_MQX_UINT); 
   } /* Endif */
#endif
   
   return(_QUEUE_GET_SIZE(&mutex_ptr->WAITING_TASKS));

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
