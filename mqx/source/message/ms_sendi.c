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
* $FileName: ms_sendi.c$
* $Version : 3.6.8.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains the functions for sending messages.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"
#include "ipc.h"
#include "ipcrtprv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msgq_send_internal
* Returned Value  :  boolean, indicating validity of queue_id
* Comments        :  Verify the input queue_id and try to send the
*                    message directly.  
*
*END*------------------------------------------------------------------*/

boolean  _msgq_send_internal
   (
      /* [IN]  pointer to the  message being sent by application */
      MESSAGE_HEADER_STRUCT_PTR msg_ptr,

      /* [IN]  is the calling task to be blocked after the call */
      boolean                   blocking,

      /* [IN]  the queue to put the message onto */
      _queue_id                 target_qid
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR       kernel_data;
            MSG_COMPONENT_STRUCT_PTR     msg_component_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR  imsg_ptr;
   register MSGQ_STRUCT_PTR              msgq_ptr;
   register TD_STRUCT_PTR                td_ptr;
            MESSAGE_HEADER_STRUCT_PTR    tmp_msg_ptr;
   register _mqx_uint                     state;
   register _queue_number                queue;
   register _processor_number            pnum;
/* Start CR 2191 */   
            boolean                      swapped_msg;
/* End CR 2191 */            
   
   _GET_KERNEL_DATA(kernel_data);

   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      return(FALSE);
   } /* Endif */
   if (msg_ptr == NULL) {
      _task_set_error(MSGQ_INVALID_MESSAGE);
      return(FALSE);
   } /* Endif */
#endif

   imsg_ptr = GET_INTERNAL_MESSAGE_PTR(msg_ptr);

#if MQX_CHECK_ERRORS
   if (imsg_ptr->VALID != MSG_VALID){
      /* An invalid message was input by the application. */
      _task_set_error(MSGQ_INVALID_MESSAGE);
      return FALSE;
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (imsg_ptr->FREE || imsg_ptr->QUEUED){
      /* Trying to send a free message, or one on a message queue. */
      _task_set_error(MSGQ_INVALID_MESSAGE);
      return FALSE;
   } /* Endif */
#endif

   pnum  = PROC_NUMBER_FROM_QID(target_qid);

   /* If processor number is zero then the message is for this processor */
   if (pnum == 0) {
      /* Fix up the target QID in the message header */
      msg_ptr->TARGET_QID = BUILD_QID(kernel_data->INIT.PROCESSOR_NUMBER, 
         msg_ptr->TARGET_QID);
   } else if (pnum != kernel_data->INIT.PROCESSOR_NUMBER) {
#if MQX_IS_MULTI_PROCESSOR
      IPC_MSG_ROUTING_COMPONENT_STRUCT_PTR ipc_msg_comp_ptr;
      ipc_msg_comp_ptr = (IPC_MSG_ROUTING_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_IPC_MSG_ROUTING];
      if (ipc_msg_comp_ptr && ipc_msg_comp_ptr->MSG_ROUTER) {
         return( (*ipc_msg_comp_ptr->MSG_ROUTER)(pnum, msg_ptr, blocking));
      } else {
#endif
         _msg_free(msg_ptr);
         _task_set_error(MSGQ_INVALID_QUEUE_ID);
         return FALSE;
#if MQX_IS_MULTI_PROCESSOR
      }/* Endif */
#endif
   } /* Endif */
      
   queue = QUEUE_FROM_QID(target_qid);

#if MQX_CHECK_ERRORS
   if ( ! VALID_QUEUE(queue)) {
      _msg_free(msg_ptr);
      _task_set_error(MSGQ_INVALID_QUEUE_ID);
      return FALSE;
   } /* Endif */
#endif

   msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];
   if (msgq_ptr->QUEUE != queue) {
      msgq_ptr = NULL;
   } /* Endif */

#if MQX_CHECK_ERRORS
   if (msgq_ptr == NULL) {
      _msg_free(msg_ptr);
      _task_set_error(MSGQ_QUEUE_IS_NOT_OPEN);
      return FALSE;
   } /* Endif */
#endif

   _INT_DISABLE();
   
   if ((msgq_ptr->MAX_ENTRIES == 0) ||
       (msgq_ptr->NO_OF_ENTRIES < msgq_ptr->MAX_ENTRIES))
   {
   /* End CR 2265 */
   
      /* There is room on the queue, so add the msg.  We 
         need to check for room here even if the msg ends up
         being short-cutted to the receiver (via td_ptr->MESSAGE)
         in case msg_receive needs to enqueue the msg.
      */

      if (msgq_ptr->TYPE == MSG_QUEUE) {
         /* THIS MESSAGE QUEUE IS ATTACHED TO A TASK */

         /* check for pending receive
         ** if a receive is pending then satisfy the request
         ** and add the receiving task onto the ready-to-run queue
         */
      
         td_ptr = msgq_ptr->TD_PTR;
         state  = td_ptr->STATE & STATE_MASK;
         if ( (state == RCV_ANY_BLOCKED) ||
             ((state == RCV_SPECIFIC_BLOCKED) && (td_ptr->INFO == queue)))
         {
            /* The task is blocked, waiting for a message */
            td_ptr->MESSAGE = &imsg_ptr->MESSAGE;
            imsg_ptr->TD_PTR = td_ptr;
            _TIME_DEQUEUE(td_ptr,kernel_data);
            _TASK_READY(td_ptr,kernel_data);

            /* Now run the notification function */
            if (msgq_ptr->NOTIFICATION_FUNCTION != NULL) {
               (*msgq_ptr->NOTIFICATION_FUNCTION)(msgq_ptr->NOTIFICATION_FUNCTION_PARAMETER);
            } /* Endif */

            if (blocking) {
               if ( ! kernel_data->IN_ISR) {
                  td_ptr = kernel_data->ACTIVE_PTR;
                  td_ptr->STATE = SEND_BLOCKED;
                  _task_block();
               } /* Endif */
            } else {
               /*
               **  if the highest priority ready task is not the
               **  same priority as the sending task, then a higher
               **  priority task was made ready and it has to be allowed
               **  to run.
               */
               _CHECK_RUN_SCHEDULER(); /* Let a higher priority task run */
            } /* Endif */

         } else {
            /* The task is ready to run and pre-empted OR blocked and
            ** on a different queue.
            */

/* Start CR 2191 */
            swapped_msg = FALSE;
/* End CR 2191 */            
            if ((msg_ptr->CONTROL & MSG_PRIORITY_MASK) &&
                (td_ptr->MESSAGE != NULL))
            {
               /* Check the message in the TD */
               tmp_msg_ptr = (MESSAGE_HEADER_STRUCT_PTR)td_ptr->MESSAGE;
               if ( (msg_ptr->CONTROL & MSG_HDR_URGENT) ||
                  /* Urgent messages first */
                    ( (! (tmp_msg_ptr->CONTROL & MSG_HDR_URGENT)) &&
                      /* Start CR 621 */
                      ( (_mqx_uint)(tmp_msg_ptr->CONTROL & MSG_HDR_PRIORITY_MASK) < 
                        (_mqx_uint)(msg_ptr->CONTROL & MSG_HDR_PRIORITY_MASK)) 
                      /* End CR 621 */
                    )
                  )
                  /* Higher priority messages first */
               {
                  /* Put new message into TD */
                  td_ptr->MESSAGE = msg_ptr;
 
/* Start CR 2193 */                  
                  /* Set the new message's ownership to the receiving queue's TD */
                  imsg_ptr = GET_INTERNAL_MESSAGE_PTR(msg_ptr);
                  imsg_ptr->TD_PTR = td_ptr; 

                  /* Old message which we pulled from TD, need to add to queue, below */  
/* End CR 2193 */                                   
                  msg_ptr  = tmp_msg_ptr;  
                  imsg_ptr = GET_INTERNAL_MESSAGE_PTR(msg_ptr);

                  /* Don't know the sender's TD for the swapped out msg,
                     so set it to NULL; */ 
                  imsg_ptr->TD_PTR = NULL;
/* Start CR 2191 */                  
                  /* Indicate that a swap occurred */
                  swapped_msg = TRUE;
                  /* Set the queue to the swapped msg's queue. */
                  if (target_qid != msg_ptr->TARGET_QID)
                  {
                     queue = QUEUE_FROM_QID(msg_ptr->TARGET_QID);
                     msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];
                     
                     /* This msg's queue was not full when it was short-cut,
                        so we should not get here.  Check anyway. */
                     if ((msgq_ptr->MAX_ENTRIES != 0) &&
                         (msgq_ptr->NO_OF_ENTRIES >= msgq_ptr->MAX_ENTRIES))
                     {
                        /* Queue full, error */
                        _INT_ENABLE();
                        _msg_free(msg_ptr);
                        _task_set_error(MSGQ_QUEUE_FULL);
                        return FALSE;
                     } /* Endif */
                  } /* Endif */
               } /* Endif */
            } /* Endif */

            /* add the message */
            _msgq_insert_message_internal(msgq_ptr, imsg_ptr, swapped_msg);

            if (msgq_ptr->TD_PTR){
               ++(msgq_ptr->TD_PTR->MESSAGES_AVAILABLE);
            } /* Endif */

            /* Now run the notification function */
            if (msgq_ptr->NOTIFICATION_FUNCTION != NULL) {
               (*msgq_ptr->NOTIFICATION_FUNCTION)(msgq_ptr->NOTIFICATION_FUNCTION_PARAMETER);
            } /* Endif */

            if (blocking && ! kernel_data->IN_ISR ) {
               td_ptr = kernel_data->ACTIVE_PTR;
               td_ptr->STATE = SEND_BLOCKED;
               _task_block();
            } /* Endif */

      } /* Endif */
            
   } else {
      /* THIS IS A SYSTEM QUEUE NOT ATTACHED TO A TASK */


         /* add the message to the queue */
         _msgq_insert_message_internal(msgq_ptr, imsg_ptr, FALSE);


         /* Run the notification function. */
         if ( msgq_ptr->NOTIFICATION_FUNCTION != NULL ) {
            (*msgq_ptr->NOTIFICATION_FUNCTION)(msgq_ptr->NOTIFICATION_FUNCTION_PARAMETER);
         } /* Endif */

      } /* Endif */

   } else {
      /* Queue full, error */
      _INT_ENABLE();
      _task_set_error(MSGQ_QUEUE_FULL);
      _msg_free(&imsg_ptr->MESSAGE);
      return FALSE;
   } /* Endif */

   _INT_ENABLE();
   return TRUE;  /* Message sent MQX_OK */

} /* Endbody */


/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msgq_insert_message_internal
* Returned Value  :  none
* Comments        :  
*   insert the message into the message queue according to its 
* priority and urgency
*
*END*------------------------------------------------------------------*/

void _msgq_insert_message_internal
   (
      /* [IN] The message queue to put the message onto */
      register MSGQ_STRUCT_PTR             msgq_ptr,

      /* [IN] The message which is to be inserted */
      register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr,

      /* [IN] Was this message swapped out from the TD? */
      register boolean swapped_msg
   )
{ /* Body */
   register INTERNAL_MESSAGE_STRUCT_PTR  prev_imsg_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR  next_imsg_ptr;
            _mqx_uint                     control;
            _mqx_uint                     priority;
   if (msgq_ptr->FIRST_MSG_PTR == NULL) {
      imsg_ptr->PREV          = NULL;
      imsg_ptr->NEXT          = NULL;
      msgq_ptr->FIRST_MSG_PTR = imsg_ptr;
      msgq_ptr->LAST_MSG_PTR  = imsg_ptr;
   } else {
/* Start CR 2191 */
      if (imsg_ptr->MESSAGE.CONTROL & MSG_HDR_URGENT || swapped_msg) {
         /* Insert at front if urgent 
            OR if we are dealing with message previously in TD that was pulled out
            by a higher priority message */
/* End CR 2191 */
         imsg_ptr->NEXT                  = msgq_ptr->FIRST_MSG_PTR;
         imsg_ptr->PREV                  = NULL;
         (msgq_ptr->FIRST_MSG_PTR)->PREV = imsg_ptr;
          msgq_ptr->FIRST_MSG_PTR        = imsg_ptr;
      } else if (imsg_ptr->MESSAGE.CONTROL & MSG_HDR_PRIORITY_MASK) {
         /* We must insert the message into the queue */
         priority = imsg_ptr->MESSAGE.CONTROL & MSG_HDR_PRIORITY_MASK;
         prev_imsg_ptr = NULL;
         next_imsg_ptr = msgq_ptr->FIRST_MSG_PTR;
         while (next_imsg_ptr) {
            control  = next_imsg_ptr->MESSAGE.CONTROL & MSG_HDR_PRIORITY_MASK;
            if (priority > control) {
               break;
            } /* Endif */
            prev_imsg_ptr = next_imsg_ptr;
            next_imsg_ptr = next_imsg_ptr->NEXT;
         } /* Endwhile */

         if (prev_imsg_ptr == NULL) {
            /* Insert at front */
            imsg_ptr->NEXT                  = msgq_ptr->FIRST_MSG_PTR;
            imsg_ptr->PREV                  = NULL;
            (msgq_ptr->FIRST_MSG_PTR)->PREV = imsg_ptr;
            msgq_ptr->FIRST_MSG_PTR         = imsg_ptr;
         } else  if (next_imsg_ptr == NULL) {
            /* Insert at end */
            imsg_ptr->PREV                 = msgq_ptr->LAST_MSG_PTR;
            imsg_ptr->NEXT                 = NULL;
            (msgq_ptr->LAST_MSG_PTR)->NEXT = imsg_ptr;
            msgq_ptr->LAST_MSG_PTR         = imsg_ptr;
         } else {
            /* Insert in middle */
            imsg_ptr->PREV      = prev_imsg_ptr;
            imsg_ptr->NEXT      = next_imsg_ptr;
            prev_imsg_ptr->NEXT = imsg_ptr;
            next_imsg_ptr->PREV = imsg_ptr;
         } /* Endif */
      } else {
         /* Tack on to end */
         imsg_ptr->PREV                 = msgq_ptr->LAST_MSG_PTR;
         imsg_ptr->NEXT                 = NULL;
         (msgq_ptr->LAST_MSG_PTR)->NEXT = imsg_ptr;
         msgq_ptr->LAST_MSG_PTR         = imsg_ptr;
      } /* Endif */
   } /* Endif */
   
   ++(msgq_ptr->NO_OF_ENTRIES);
   imsg_ptr->QUEUED = TRUE;
   
} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
