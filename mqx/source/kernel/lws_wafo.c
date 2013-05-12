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
* $FileName: lws_wafo.c$
* $Version : 3.8.7.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   light weight semaphores.  These semaphores have low memory
*   requirements, and no extra features.  Tasks are suspended
*   in fifo order while waiting for a post.  No limits on values
*   are imposed.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwsem_wait_for
* Returned Value   : an error code
*   queue
* Comments         :
*   This function obtains a semaphore from the lwsem.  If one is not
* available, the task waits (in a FIFO order) for the
* specified number of ticks.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwsem_wait_for
   (
      /* [IN] the semaphore address */
      LWSEM_STRUCT_PTR sem_ptr,

      /* [IN] the number of ticks to delay */
      MQX_TICK_STRUCT_PTR ticks
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   _mqx_uint              result;
   
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwsem_wait_for(sem_ptr, ticks);
    }
#endif

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_lwsem_wait_for, sem_ptr, ticks);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_lwsem_wait_for, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != LWSEM_VALID) {
      _KLOGX2(KLOG_lwsem_wait_for, MQX_INVALID_LWSEM);
      return(MQX_INVALID_LWSEM);
   } /* Endif */
#endif

   _INT_DISABLE();
   if (sem_ptr->VALUE <= 0) {
      td_ptr = kernel_data->ACTIVE_PTR;
      /* Calculate time to wake up the task */
      PSP_ADD_TICKS(ticks, &kernel_data->TIME, &td_ptr->TIMEOUT);
      result = _lwsem_wait_timed_internal(sem_ptr, td_ptr);
   } else {
      --sem_ptr->VALUE;
      /* Start CR 788 */
      result = MQX_OK;
      /* End  CR 788 */
   } /* Endif */

//#if MQX_COMPONENT_DESTRUCTION
   /* We must check for component destruction */
   if (sem_ptr->VALID != LWSEM_VALID) {
      _int_enable();
      /* The semaphore has been deleted */
      _KLOGX2(KLOG_lwsem_wait_for, MQX_INVALID_LWSEM);
      return(MQX_INVALID_LWSEM);
   } /* Endif */
//#endif

   _INT_ENABLE();

   _KLOGX2(KLOG_lwsem_wait_for, result);

   return(result);
   
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_lwsem_wait_for
* Returned Value   : an error code
*   queue
* Comments         :
*   This function obtains a semaphore from the lwsem.  If one is not
* available, the task waits (in a FIFO order) for the
* specified number of ticks.
*
*END*----------------------------------------------------------------------*/
#if MQX_ENABLE_USER_MODE

_mqx_uint _usr_lwsem_wait_for
   (
      /* [IN] the semaphore address */
      LWSEM_STRUCT_PTR sem_ptr,

      /* [IN] the number of ticks to delay */
      MQX_TICK_STRUCT_PTR ticks
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)sem_ptr, (uint_32)ticks, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWSEM_WAIT_FOR, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_HAS_TICK */
