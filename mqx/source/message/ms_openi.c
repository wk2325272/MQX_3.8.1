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
* $FileName: ms_openi.c$
* $Version : 3.0.7.0$
* $Date    : Jun-26-2009$
*
* Comments:
*
*   This file contains the functions for creating message queues.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_open_internal
* Returned Value  : _queue_id - indicating successful queue creation,
*   a NULL indicates error
* Comments        : This is an internal function used
*   to open a message queue.
*
*END*------------------------------------------------------------------*/

_queue_id _msgq_open_internal
   (
      /* [IN] the queue being opened */
      _queue_number queue,

      /* [IN] the maximum number of entries allowed in this queue */
      uint_16       max_queue_size,

      /* [IN] the type of queue, NORMAL or SYSTEM */
      _mqx_uint      queue_type,

      /* [IN] the function to be called when an entry is put on the queue */
      void (_CODE_PTR_ notification_function)(pointer),

      /* [IN] information to pass to the notification function */
      pointer notification_data
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR   kernel_data;
            TD_STRUCT_PTR            td_ptr;
            MSG_COMPONENT_STRUCT_PTR msg_component_ptr;
   register MSGQ_STRUCT_PTR          temp_msgq_ptr;
   register MSGQ_STRUCT_PTR          msgq_ptr;
   register _queue_id                qid;
   register _mqx_uint                 i;
            _mqx_uint                 result;

   _GET_KERNEL_DATA(kernel_data);
   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
   qid               = (_queue_id)0;

   if (msg_component_ptr == NULL) {
      result = _msg_create_component();
      msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (msg_component_ptr == NULL) {
         _task_set_error(result);
         return(qid);
      } /* Endif */
#endif      
   } /* Endif */

   _int_disable();
   if (queue == MSGQ_FREE_QUEUE) {
      /* Lets find a free queue entry */
      msgq_ptr = &msg_component_ptr->MSGQS_PTR[MSGQ_FIRST_USER_QUEUE];
      for (i = MSGQ_FIRST_USER_QUEUE; i <= msg_component_ptr->MAX_MSGQS; ++i) {
         if (msgq_ptr->QUEUE != (_queue_number)i) {
            queue = (_queue_number)i;
            break;
            /* Found one! */
         } /* Endif */
         ++msgq_ptr;
      } /* Endfor */
      if (queue == MSGQ_FREE_QUEUE) {
         /* Did not get one */
         _int_enable();
         _task_set_error(MSGQ_QUEUE_IN_USE);
         return(qid);
      }/* Endif */
   } else {
      if (queue >= msg_component_ptr->MAX_MSGQS) {
         _int_enable();
         _task_set_error(MSGQ_INVALID_QUEUE_NUMBER);
         return(qid);
      }
   }



#if MQX_CHECK_ERRORS
   if ( !VALID_QUEUE(queue))  {
      _int_enable();
      _task_set_error(MSGQ_INVALID_QUEUE_ID);
      return(qid);
   } /* Endif */
#endif

   msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];

#if MQX_CHECK_ERRORS
   if (msgq_ptr->QUEUE == queue) {
      _int_enable();
      _task_set_error(MSGQ_QUEUE_IN_USE);
      return(qid);
   } /* Endif */
#endif

   msgq_ptr->QUEUE         = queue;
   msgq_ptr->FIRST_MSG_PTR = NULL;
   msgq_ptr->LAST_MSG_PTR  = NULL;
   msgq_ptr->NEXT_MSGQ_PTR = NULL;
   msgq_ptr->NO_OF_ENTRIES = 0;
   msgq_ptr->TYPE          = (uint_16)queue_type;
   msgq_ptr->MAX_ENTRIES   = max_queue_size;
   msgq_ptr->NOTIFICATION_FUNCTION           = notification_function;
   msgq_ptr->NOTIFICATION_FUNCTION_PARAMETER = notification_data;

   if ( queue_type == MSG_QUEUE ) {
      td_ptr = kernel_data->ACTIVE_PTR;
      msgq_ptr->TD_PTR = td_ptr;
      if ( td_ptr->MSG_QUEUE_HEAD == NULL ) {
         td_ptr->MSG_QUEUE_HEAD = (pointer)msgq_ptr;
         td_ptr->MSG_QUEUE_TAIL = (pointer)msgq_ptr;
         td_ptr->MESSAGES_AVAILABLE = 0;
      } else {
         temp_msgq_ptr = (MSGQ_STRUCT_PTR)(td_ptr->MSG_QUEUE_TAIL);
         temp_msgq_ptr->NEXT_MSGQ_PTR = msgq_ptr;
         td_ptr->MSG_QUEUE_TAIL = (pointer)msgq_ptr;
      } /* Endif */
   } else {
      msgq_ptr->TD_PTR = NULL;
   } /* Endif */

   _INT_ENABLE();
   qid = BUILD_QID(kernel_data->INIT.PROCESSOR_NUMBER, queue);
   return (qid);

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
