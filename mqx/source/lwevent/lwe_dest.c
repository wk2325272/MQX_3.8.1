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
* $FileName: lwe_dest.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for destroying a light weight event.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWEVENTS
#include "lwevent.h"
#include "lwe_prv.h"

_mqx_uint _lwevent_destroy_internal
    (
        /* [IN] the location of the event */
        LWEVENT_STRUCT_PTR event_ptr,
        
        boolean user
   )
{
   KERNEL_DATA_STRUCT_PTR kernel_data;
#if MQX_COMPONENT_DESTRUCTION
   TD_STRUCT_PTR          td_ptr;
#endif

#if MQX_ENABLE_USER_MODE
   if (user && !_psp_mem_check_access_mask((uint_32)event_ptr, sizeof(LWEVENT_STRUCT), MPU_UM_R, MPU_UM_RW)) {
       return MQX_LWEVENT_INVALID;
   }
#endif
   
   _GET_KERNEL_DATA(kernel_data);                                         

   _KLOGE2(KLOG_lwevent_destroy, event_ptr);
   
#if MQX_COMPONENT_DESTRUCTION

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_lwevent_destroy, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   _int_disable();
#if MQX_CHECK_VALIDITY
   if (event_ptr->VALID != LWEVENT_VALID) {
      _int_enable();
      _KLOGX2(KLOG_lwevent_destroy, MQX_LWEVENT_INVALID);
      return(MQX_LWEVENT_INVALID);
   } /* Endif */
#endif

   /* Effectively stop all access to the event */
   event_ptr->VALID = 0;
   while (_QUEUE_GET_SIZE(&event_ptr->WAITING_TASKS)) {
      _QUEUE_DEQUEUE(&event_ptr->WAITING_TASKS, td_ptr);
      _BACKUP_POINTER(td_ptr, TD_STRUCT, AUX_QUEUE);
      _TIME_DEQUEUE(td_ptr, kernel_data);
      _TASK_READY(td_ptr, kernel_data);
   } /* Endwhile */   
 
   /* remove event from kernel LWEVENTS queue */
#if MQX_ENABLE_USER_MODE
    if (user) {
        _QUEUE_REMOVE(&kernel_data->USR_LWEVENTS, event_ptr);
    }
    else 
#endif
    {
        _QUEUE_REMOVE(&kernel_data->LWEVENTS, event_ptr);
    }

   _int_enable();

   /* May need to let higher priority task run */
   _CHECK_RUN_SCHEDULER();
#endif

   _KLOGX2(KLOG_lwevent_destroy, MQX_OK);
   return(MQX_OK);
   
}

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _lwevent_destroy
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to destroy an instance of a light weight event
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _lwevent_destroy
    (
        /* [IN] the location of the event */
        LWEVENT_STRUCT_PTR event_ptr
    )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwevent_destroy(event_ptr);
    }
#endif
    
    return _lwevent_destroy_internal(event_ptr, FALSE);
}


#if MQX_ENABLE_USER_MODE

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _usr_lwevent_destroy
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to destroy an instance of a light weight event
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _usr_lwevent_destroy
    (
        /* [IN] the location of the event */
        LWEVENT_STRUCT_PTR event_ptr
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)event_ptr, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWEVENT_DESTROY, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWEVENTS */

/* EOF */
