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
* $FileName: lws_post.c$
* $Version : 3.8.4.0$
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


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwsem_post
* Returned Value   : an error code
*   queue
* Comments         :
*   This function posts a semaphore to the lwsem.  If any tasks are waiting
* for semaphores, the first available task is made ready.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwsem_post
   (
      /* [IN] the semaphore address */
      LWSEM_STRUCT_PTR  sem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwsem_post(sem_ptr);
    }
#endif

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_lwsem_post, sem_ptr);

#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != LWSEM_VALID) {
      _KLOGX2(KLOG_lwsem_post, MQX_INVALID_LWSEM);
      return(MQX_INVALID_LWSEM);
   } /* Endif */
#endif

   _INT_DISABLE();
   if ((sem_ptr->VALUE >= 0) && (_QUEUE_GET_SIZE(&sem_ptr->TD_QUEUE))) {
      _QUEUE_DEQUEUE(&sem_ptr->TD_QUEUE, td_ptr);
      _BACKUP_POINTER(td_ptr, TD_STRUCT, AUX_QUEUE);
      _TIME_DEQUEUE(td_ptr, kernel_data);
      td_ptr->INFO = 0;  /* Signal that post is activating the task */
      _TASK_READY(td_ptr, kernel_data);
      _CHECK_RUN_SCHEDULER(); /* Let higher priority task run */
   } else {
      ++sem_ptr->VALUE;      
   } /* Endif */
   _INT_ENABLE();

   _KLOGX2(KLOG_lwsem_post, MQX_OK);

   return(MQX_OK);
   
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_lwsem_post
* Returned Value   : an error code
*   queue
* Comments         :
*   This function posts a semaphore to the lwsem.  If any tasks are waiting
* for semaphores, the first available task is made ready.
*
*END*----------------------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

_mqx_uint _usr_lwsem_post
   (
      /* [IN] the semaphore address */
      LWSEM_STRUCT_PTR  sem_ptr
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)sem_ptr, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWSEM_POST, &params);
}
#endif /* MQX_ENABLE_USER_MODE */
