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
* $FileName: ms_poll.c$
* $Version : 3.0.6.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function for returning a message from
*   a message queues without waiting.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_poll
* Returned Value  : pointer to a message structure
*                   NULL if the message queue is empty.
* Comments        : Poll the message queue for a message.
*
*END*------------------------------------------------------------------*/

pointer _msgq_poll
   (
      /* [IN] queue from which a message is to be dequeued*/
      _queue_id queue_id
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
            MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
   register MESSAGE_HEADER_STRUCT_PTR   message_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   register MSGQ_STRUCT_PTR             msgq_ptr;
            _queue_number               queue;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_msgq_poll, queue_id);

    msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX3(KLOG_msgq_poll, NULL, MQX_COMPONENT_DOES_NOT_EXIST);
      return(NULL);
   } /* Endif */
#endif

   message_ptr = NULL;
   queue = QUEUE_FROM_QID(queue_id);

#if MQX_CHECK_ERRORS
   if ((PROC_NUMBER_FROM_QID(queue_id) != kernel_data->INIT.PROCESSOR_NUMBER) ||
      (! VALID_QUEUE(queue)) )
   {
      _task_set_error(MSGQ_INVALID_QUEUE_ID);
      _KLOGX3(KLOG_msgq_poll, NULL, MSGQ_INVALID_QUEUE_ID);
      return (pointer)message_ptr;
   } /* Endif */
 #endif

   msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];
   if (msgq_ptr->QUEUE != (queue)) {
      _task_set_error(MSGQ_QUEUE_IS_NOT_OPEN);
      _KLOGX3(KLOG_msgq_poll, NULL, MSGQ_QUEUE_IS_NOT_OPEN);
      return((pointer)message_ptr);
   } /* Endif */
      
#if MQX_CHECK_ERRORS
   if (msgq_ptr->TD_PTR != NULL) {
      if (msgq_ptr->TD_PTR != kernel_data->ACTIVE_PTR) {
         _task_set_error(MSGQ_NOT_QUEUE_OWNER);
         _KLOGX3(KLOG_msgq_poll, NULL, MSGQ_NOT_QUEUE_OWNER);
         return((pointer)message_ptr);
      } /* Endif */
   } /* Endif */
#endif

   /* check the specified queue for an entry */
   _INT_DISABLE();
   if ( msgq_ptr->NO_OF_ENTRIES == 0 ) {
      _INT_ENABLE();
      #if MQXCFG_ENABLE_MSG_TIMEOUT_ERROR
         _task_set_error(MSGQ_MESSAGE_NOT_AVAILABLE);
      #endif
      _KLOGX3(KLOG_msgq_poll, NULL, MSGQ_MESSAGE_NOT_AVAILABLE);
   } else {

      --(msgq_ptr->NO_OF_ENTRIES);
      imsg_ptr = msgq_ptr->FIRST_MSG_PTR;
      msgq_ptr->FIRST_MSG_PTR = imsg_ptr->NEXT;
      if ( msgq_ptr->FIRST_MSG_PTR == NULL ) {
         msgq_ptr->LAST_MSG_PTR = NULL;
      } else {
         (msgq_ptr->FIRST_MSG_PTR)->PREV = NULL;
      } /* Endif */

      if ( msgq_ptr->TD_PTR ) {
         --(msgq_ptr->TD_PTR->MESSAGES_AVAILABLE);
      } /* Endif */
      message_ptr = &imsg_ptr->MESSAGE;
      imsg_ptr->QUEUED = FALSE;
      imsg_ptr->TD_PTR = NULL;

      _INT_ENABLE();
      _KLOGX2(KLOG_msgq_poll, message_ptr);
   } /* Endif */
   return (pointer)message_ptr;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
