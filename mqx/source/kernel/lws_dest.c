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
* $FileName: lws_dest.c$
* $Version : 3.8.8.0$
* $Date    : Sep-19-2011$
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

_mqx_uint _lwsem_destroy_internal
   (
      /* [IN] the semaphore handle */
      LWSEM_STRUCT_PTR sem_ptr,
      boolean user
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
//#if MQX_COMPONENT_DESTRUCTION
   TD_STRUCT_PTR          td_ptr;
//#endif
   LWSEM_STRUCT_PTR  sem_chk_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_lwsem_destroy, sem_ptr);
//#if MQX_COMPONENT_DESTRUCTION

   _int_disable();
#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != LWSEM_VALID) {
      _int_enable();
      _KLOGX2(KLOG_lwsem_destroy, MQX_INVALID_LWSEM);
      return(MQX_INVALID_LWSEM);
   } /* Endif */
#endif

   sem_ptr->VALID  = 0;   /* Invalidate the semaphore */
   while (_QUEUE_GET_SIZE(&sem_ptr->TD_QUEUE)) {
      _QUEUE_DEQUEUE(&sem_ptr->TD_QUEUE, td_ptr);
      _BACKUP_POINTER(td_ptr, TD_STRUCT, AUX_QUEUE);
      _TIME_DEQUEUE(td_ptr, kernel_data);
      _TASK_READY(td_ptr, kernel_data);
   } /* Endwhile */   

   /* Check if lwsem is in kernel's semaphore list */
#if MQX_ENABLE_USER_MODE
   if (user) {
       sem_chk_ptr = (LWSEM_STRUCT_PTR)((pointer)kernel_data->USR_LWSEM.NEXT);
       while (sem_chk_ptr != (LWSEM_STRUCT_PTR)((pointer)&kernel_data->USR_LWSEM)) {
           if (sem_chk_ptr == sem_ptr) {
               /* remove semaphore from kernel LWSEM queue */
               _QUEUE_REMOVE(&kernel_data->USR_LWSEM, sem_ptr);
               break;
           }
           sem_chk_ptr = (LWSEM_STRUCT_PTR)((pointer)sem_chk_ptr->NEXT);
       }
   }
   else
#endif
   {
       sem_chk_ptr = (LWSEM_STRUCT_PTR)((pointer)kernel_data->LWSEM.NEXT);
       while (sem_chk_ptr != (LWSEM_STRUCT_PTR)((pointer)&kernel_data->LWSEM)) {
           if (sem_chk_ptr == sem_ptr) {
               /* remove semaphore from kernel LWSEM queue */
               _QUEUE_REMOVE(&kernel_data->LWSEM, sem_ptr);
               break;
           } /* Endif */
           sem_chk_ptr = (LWSEM_STRUCT_PTR)((pointer)sem_chk_ptr->NEXT);
       } /* Endwhile */
   }
   
   _int_enable();
     
   _CHECK_RUN_SCHEDULER();   /* Allow higher priority tasks to run */

//#endif

   _KLOGX2(KLOG_lwsem_destroy, MQX_OK);
   return(MQX_OK);
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwsem_destroy
* Returned Value   : error code
* Comments         :
*   This function deletes a light weight semaphore.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwsem_destroy
   (
      /* [IN] the semaphore handle */
      LWSEM_STRUCT_PTR sem_ptr
   )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwsem_destroy(sem_ptr);
    }
#endif
    
    return _lwsem_destroy_internal(sem_ptr, FALSE);
}
    
#if MQX_ENABLE_USER_MODE
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwsem_destroy
* Returned Value   : error code
* Comments         :
*   This function deletes a light weight semaphore.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _usr_lwsem_destroy
    (
        /* [IN] the semaphore handle */
        LWSEM_STRUCT_PTR sem_ptr
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)sem_ptr, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWSEM_DESTROY, &params);
}

#endif // MQX_ENABLE_USER_MODE
