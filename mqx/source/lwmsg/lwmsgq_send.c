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
* $FileName: lwmsgq_send.c$
* $Version : 3.8.2.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file provides a simple light weight message queue.
*   A message in the message pool has a fixed size, a multiple of 32 bits.
*   Blocking reads, and Blocking writes are provided.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMSGQ
#include "lwmsgq.h"
#include "lwmsgqpr.h"


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _lwmsgq_send
* Returned Value   : error code
* Comments         :
*   This function puts the word into the queue if possible
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwmsgq_send
   (
      /* Handle to the queue */
      pointer           handle,

      /* location of message to copy in */
      _mqx_max_type_ptr message,

      /* flags for blocking on full, blocking on send */
      _mqx_uint         flags
   )
{/* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   LWMSGQ_STRUCT_PTR      q_ptr = (LWMSGQ_STRUCT_PTR)handle;
   _mqx_uint              i;
   _mqx_max_type_ptr      from_ptr;
   _mqx_max_type_ptr      to_ptr;

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwmsgq_send(handle, message, flags);
    }
#endif
    
   /* Start CR 1944 */
   _GET_KERNEL_DATA(kernel_data);
   _KLOGE4(KLOG_lwmsgq_send, handle, message, flags);
   /* End CR 1944 */
   
   _int_disable();
#if MQX_CHECK_VALIDITY
   if (q_ptr->VALID != LWMSGQ_VALID){
      _int_enable();
      /* Start CR 1944 */
      _KLOGX2(KLOG_lwmsgq_send, LWMSGQ_INVALID);
      /* End CR 1944 */
      return LWMSGQ_INVALID;
   } /* Endif */
#endif
   if (LWMSGQ_IS_FULL(q_ptr)) {
      if (flags & LWMSGQ_SEND_BLOCK_ON_FULL) {
         td_ptr = kernel_data->ACTIVE_PTR;
         while (LWMSGQ_IS_FULL(q_ptr)) {
            td_ptr->STATE = LWMSGQ_WRITE_BLOCKED;
            td_ptr->INFO  = (_mqx_uint)&q_ptr->WAITING_WRITERS;
            _QUEUE_UNLINK(td_ptr);
            _QUEUE_ENQUEUE(&q_ptr->WAITING_WRITERS, &td_ptr->AUX_QUEUE);
            _sched_execute_scheduler_internal(); /* Let other tasks run */
         } /* Endwhile */
      } else {
         _int_enable();
         /* Start CR 1944 */
         _KLOGX2(KLOG_lwmsgq_send, LWMSGQ_FULL);
         /* End CR 1944 */
         return LWMSGQ_FULL;
      } /* Endif */
   }/* Endif */
   to_ptr = q_ptr->MSG_WRITE_LOC;
   from_ptr = message;
   i = q_ptr->MSG_SIZE+1;
   while (--i) {
      *to_ptr++ = *from_ptr++;
   } /* Endwhile */
   q_ptr->MSG_WRITE_LOC += q_ptr->MSG_SIZE;
   if (q_ptr->MSG_WRITE_LOC >= q_ptr->MSG_END_LOC) {
      q_ptr->MSG_WRITE_LOC = q_ptr->MSG_START_LOC;
   } /* Endif */
   q_ptr->CURRENT_SIZE++;
   if (! _QUEUE_IS_EMPTY(&q_ptr->WAITING_READERS)) {
      _QUEUE_DEQUEUE(&q_ptr->WAITING_READERS, td_ptr);
      _BACKUP_POINTER(td_ptr, TD_STRUCT, AUX_QUEUE);
      _TIME_DEQUEUE(td_ptr, kernel_data);
      td_ptr->INFO = 0;  /* Signal that post is activating the task */
      _TASK_READY(td_ptr, kernel_data);
      if (flags & LWMSGQ_SEND_BLOCK_ON_SEND) {
         _task_block();
      } else {
         _CHECK_RUN_SCHEDULER(); /* Let higher priority task run */
      }/* Endif */
   } else {
      if (flags & LWMSGQ_SEND_BLOCK_ON_SEND) {
         _task_block();
      }/* Endif */
   } /* Endif */
   _int_enable();
   /* Start CR 1944 */
   _KLOGX2(KLOG_lwmsgq_send, MQX_OK);
   /* End CR 1944 */
   return MQX_OK;

}/* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _usr_lwmsgq_send
* Returned Value   : error code
* Comments         :
*   This function puts the word into the queue if possible
*
*END*----------------------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

_mqx_uint _usr_lwmsgq_send
   (
      /* Handle to the queue */
      pointer           handle,

      /* location of message to copy in */
      _mqx_max_type_ptr message,

      /* flags for blocking on full, blocking on send */
      _mqx_uint         flags
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)handle, (uint_32)message, (uint_32)flags, 0, 0};
    return _mqx_api_call(MQX_API_LWMSGQ_SEND, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWMSGQ */
