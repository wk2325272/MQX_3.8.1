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
* $FileName: ms_recvi.c$
* $Version : 3.6.7.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the function for reading a message from a message queue.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

#define DEQUEUE_TOP_MSG_ENTRY(mq, im, m, td) \
               --(mq->NO_OF_ENTRIES); \
               im = mq->FIRST_MSG_PTR; \
               mq->FIRST_MSG_PTR = im->NEXT; \
               if ( mq->FIRST_MSG_PTR == NULL ) { \
                  mq->LAST_MSG_PTR = NULL; \
               } else { \
                  (mq->FIRST_MSG_PTR)->PREV = NULL; \
               } \
               if ( mq->TD_PTR ) { \
                  --(mq->TD_PTR->MESSAGES_AVAILABLE); \
               } \
               m = &im->MESSAGE; \
               im->QUEUED = FALSE; \
               im->TD_PTR = td;

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_receive_internal
* Returned Value  : pointer to a message structure, or NULL if timeout occurred.
* Comments        : Dequeue the first item from
*   the specified queue.  
*   
*
*END*------------------------------------------------------------------*/

MESSAGE_HEADER_STRUCT_PTR _msgq_receive_internal
   (
      /* [IN]  id of the queue from which a message is to be received */
     _queue_id             queue_id,

      /* [IN]  indication of the number of ticks which can expire before
      **       this request times out
      */
      MQX_TICK_STRUCT_PTR  timeout_tick_ptr,

      /* [IN]  relative or absolute time specified in tick_ptr */
      _mqx_uint            mode,

      /* [OUT] where the error code is to be stored */
      _mqx_uint_ptr        error_ptr
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
            MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
   register TD_STRUCT_PTR               td_ptr;
            MESSAGE_HEADER_STRUCT_PTR   message_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   register MSGQ_STRUCT_PTR             msgq_ptr;
            _queue_number               queue;

   *error_ptr = MQX_OK;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _task_set_error(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      *error_ptr = MQX_CANNOT_CALL_FUNCTION_FROM_ISR;
      return(NULL);
   }/* Endif */
#endif
 
   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      *error_ptr = MQX_COMPONENT_DOES_NOT_EXIST;
      return(NULL);
   } /* Endif */
#endif

   message_ptr    = NULL;
   td_ptr         = kernel_data->ACTIVE_PTR;

   if (queue_id == MSGQ_ANY_QUEUE){
      /* if queue_id is 0 than a receive from any queue is performed */

      /* Does the task own a queue */
      if (td_ptr->MSG_QUEUE_HEAD == NULL){
         /* Does NOT */
         _task_set_error(MSGQ_QUEUE_IS_NOT_OPEN);
         *error_ptr = MSGQ_QUEUE_IS_NOT_OPEN;
         return NULL;
      } /* Endif */

      _INT_DISABLE();
      if (td_ptr->MESSAGES_AVAILABLE == 0){
         td_ptr->STATE = RCV_ANY_BLOCKED;
         td_ptr->INFO = queue_id;
         td_ptr->MESSAGE = NULL;

         if (mode == MSG_TIMEOUT_NONE) {
            _task_block();
         } else if (mode == MSG_TIMEOUT_RELATIVE) {
            _time_delay_for(timeout_tick_ptr);
         } else {
            _time_delay_until(timeout_tick_ptr);
         } /* Endif */

         /* 
         ** SHORT CUT...
         ** The message send routine does not queue up a message in this case.
         ** the message is deposited directly into the task descriptor
         */
         message_ptr = (MESSAGE_HEADER_STRUCT_PTR)td_ptr->MESSAGE;
         if (message_ptr == NULL){
            /* A timeout has occurred */
            #if MQXCFG_ENABLE_MSG_TIMEOUT_ERROR
            _task_set_error(MSGQ_MESSAGE_NOT_AVAILABLE);
            #endif
         } /* Endif */
         td_ptr->MESSAGE = NULL;

      } else {

         /*  Check all queues for an available entry .. There must be at least
         **  one entry available
         */
         msgq_ptr = (MSGQ_STRUCT_PTR)td_ptr->MSG_QUEUE_HEAD;
         while (msgq_ptr != NULL){

            if (msgq_ptr->NO_OF_ENTRIES){
               /* dequeue the top entry */
               DEQUEUE_TOP_MSG_ENTRY(msgq_ptr, imsg_ptr, message_ptr, td_ptr);
               break;
            } /* Endif */
            msgq_ptr = msgq_ptr->NEXT_MSGQ_PTR;

         } /* Endwhile */
      } /* Endif */

   } else {

      /* RECEIVE from a specific qid */
      queue = QUEUE_FROM_QID(queue_id);
#if MQX_CHECK_ERRORS
      if ( (PROC_NUMBER_FROM_QID(queue_id) != kernel_data->INIT.PROCESSOR_NUMBER) ||
         (! VALID_QUEUE(queue)) )
      {
         _task_set_error(MSGQ_INVALID_QUEUE_ID);
         *error_ptr = MSGQ_INVALID_QUEUE_ID;
         return (pointer)message_ptr;
      } /* Endif */
#endif

      msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];
#if MQX_CHECK_ERRORS
      if ( msgq_ptr->QUEUE != queue ) {
         _task_set_error(MSGQ_QUEUE_IS_NOT_OPEN);
         *error_ptr =  MSGQ_QUEUE_IS_NOT_OPEN;
         return message_ptr;
      } /* Endif */
      if ( (msgq_ptr->TD_PTR != NULL) && (msgq_ptr->TD_PTR != td_ptr) ) {
         _task_set_error(MSGQ_NOT_QUEUE_OWNER);
         *error_ptr = MSGQ_NOT_QUEUE_OWNER;
         return message_ptr;
      } /* Endif */
#endif

      /*
      ** check the specified queue for an entry
      ** if not entry, then block until an entry is received or
      ** timeout occurs
      */
      _INT_DISABLE();
      if (msgq_ptr->NO_OF_ENTRIES == 0) {
         if (msgq_ptr->TD_PTR == NULL) {
            /* A system message queue, indicate none available */
            message_ptr = NULL;
         } else {
            td_ptr->STATE   = RCV_SPECIFIC_BLOCKED;
            td_ptr->INFO    = queue;
            td_ptr->MESSAGE = NULL;

            if (mode == MSG_TIMEOUT_NONE) {
               _task_block();
            } else if (mode == MSG_TIMEOUT_RELATIVE) {
               _time_delay_for(timeout_tick_ptr);
            } else {
               _time_delay_until(timeout_tick_ptr);
            } /* Endif */

            message_ptr = (MESSAGE_HEADER_STRUCT_PTR)td_ptr->MESSAGE;
            if ( message_ptr == NULL ) {
               #if MQXCFG_ENABLE_MSG_TIMEOUT_ERROR
               _task_set_error(MSGQ_MESSAGE_NOT_AVAILABLE);
               #endif
            } else if ((message_ptr->TARGET_QID != queue_id) &&
                       (msgq_ptr->NO_OF_ENTRIES > 0)) {
               /* The original msg was swapped out in msgq_sendi() for 
                  a higher priority msg with a different target_qid.
                  Enqueue this msg, and then dequeue the msg we need.
                */
               register MSGQ_STRUCT_PTR tmp_msgq_ptr;
               register _queue_number   tmp_queue;
              
               /* Get the msg's queue */
               tmp_queue = QUEUE_FROM_QID(message_ptr->TARGET_QID);
               tmp_msgq_ptr = &msg_component_ptr->MSGQS_PTR[tmp_queue];

               if ((tmp_msgq_ptr->MAX_ENTRIES == 0) ||
                   (tmp_msgq_ptr->NO_OF_ENTRIES < tmp_msgq_ptr->MAX_ENTRIES))
               {
                   /* the msg's queue has room */
                   imsg_ptr = GET_INTERNAL_MESSAGE_PTR(message_ptr);

#if MQX_CHECK_ERRORS
                   if (imsg_ptr->VALID != MSG_VALID){
                      /* An invalid message was input by the application. */
                      _task_set_error(MSGQ_INVALID_MESSAGE);
                      message_ptr = NULL;
                   } else
#endif
                   {
                      /* enqueue the msg */
                      _msgq_insert_message_internal(tmp_msgq_ptr, imsg_ptr, TRUE);
                      
                      if (tmp_msgq_ptr->TD_PTR) {
                         ++(tmp_msgq_ptr->TD_PTR->MESSAGES_AVAILABLE);
                      } /* Endif */

                      /* now dequeue our queue's top entry */
                      DEQUEUE_TOP_MSG_ENTRY(msgq_ptr, imsg_ptr, message_ptr, td_ptr);
                   }
               } else {
                  /* Queue full, error - this should not happen 
                     since msgq_sendi() checks for room on the queue 
                     for all msgs, including short-cut msgs. */
                  _task_set_error(MSGQ_QUEUE_FULL);
                  message_ptr = NULL;
               }
            } /* Endif */
            td_ptr->MESSAGE = NULL;
         } /* Endif */
      } else {   
         /* dequeue the top entry */
         DEQUEUE_TOP_MSG_ENTRY(msgq_ptr, imsg_ptr, message_ptr, td_ptr);
      } /* Endif */
   } /* Endif */
      
   _INT_ENABLE();

   return message_ptr;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
