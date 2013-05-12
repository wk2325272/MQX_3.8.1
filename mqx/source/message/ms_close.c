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
* $FileName: ms_close.c$
* $Version : 3.0.5.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function for closing a message queue.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_close
* Returned Value  : boolean, indicating validity of queue_id
* Comments        : Informs the kernel that a queue is being closed.
*                   A task can only close a queue that it owns.
*
*END*------------------------------------------------------------------*/

boolean  _msgq_close
   (
      /* [IN]  id of the queue to be closed  */
      _queue_id queue_id
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
   register TD_STRUCT_PTR               td_ptr;
            MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
   register MSGQ_STRUCT_PTR             msgq_ptr;
   register MSGQ_STRUCT_PTR             td_msgq_ptr;
   register MSGQ_STRUCT_PTR             qprev_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR tmp_imsg_ptr;
   register uint_16                     queue;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_msgq_close, queue_id);

   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL) {
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX3(KLOG_msgq_close, FALSE, MQX_COMPONENT_DOES_NOT_EXIST);
      return(FALSE);
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if ( PROC_NUMBER_FROM_QID(queue_id) != kernel_data->INIT.PROCESSOR_NUMBER ) {
      _task_set_error(MSGQ_INVALID_QUEUE_ID);
      _KLOGX3(KLOG_msgq_close, FALSE, MSGQ_INVALID_QUEUE_ID);
      return FALSE;
   } /* Endif */
#endif

   queue = QUEUE_FROM_QID(queue_id);

#if MQX_CHECK_ERRORS
   if ( ! VALID_QUEUE(queue) ) {
      _task_set_error(MSGQ_INVALID_QUEUE_ID);
      _KLOGX3(KLOG_msgq_close, FALSE, MSGQ_INVALID_QUEUE_ID);
      return FALSE;
   } /* Endif */
#endif
   
   msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];

#if MQX_CHECK_ERRORS
   if ( msgq_ptr->QUEUE != (queue) ) {
      _task_set_error(MSGQ_QUEUE_IS_NOT_OPEN);
      _KLOGX3(KLOG_msgq_close, FALSE, MSGQ_QUEUE_IS_NOT_OPEN);
      return FALSE;
   } /* Endif */
#endif

   td_ptr = kernel_data->ACTIVE_PTR;
   if ( ((msgq_ptr->TD_PTR == NULL) || (msgq_ptr->TD_PTR == td_ptr))) {
      /*
      **  this queue is currently active and is owned by
      **  the requesting task so DELETE the queue.
      */
      _int_disable();
      if ( msgq_ptr->TD_PTR != NULL ) {
         td_ptr->MESSAGES_AVAILABLE -= msgq_ptr->NO_OF_ENTRIES;
      } /* Endif */
      imsg_ptr = msgq_ptr->FIRST_MSG_PTR;
      while ( imsg_ptr != NULL ) {
         tmp_imsg_ptr = imsg_ptr->NEXT;
         imsg_ptr->QUEUED = FALSE;
         _msg_free((pointer)&imsg_ptr->MESSAGE);
         imsg_ptr = tmp_imsg_ptr;
      } /* Endwhile */
      msgq_ptr->FIRST_MSG_PTR = NULL;

      if ( msgq_ptr->TD_PTR ) {
         /* Remove the queue from the Task descriptors linked list */
         td_msgq_ptr = (MSGQ_STRUCT_PTR)td_ptr->MSG_QUEUE_HEAD;
         if ( td_msgq_ptr == msgq_ptr ) {
            td_ptr->MSG_QUEUE_HEAD = (pointer)td_msgq_ptr->NEXT_MSGQ_PTR;
            if ( td_ptr->MSG_QUEUE_HEAD == NULL ) {
               td_ptr->MSG_QUEUE_TAIL = NULL;
            } /* Endif */
        } else {
            qprev_ptr = td_msgq_ptr;
            td_msgq_ptr  = qprev_ptr->NEXT_MSGQ_PTR;
            while ( td_msgq_ptr != msgq_ptr ) {
               qprev_ptr = td_msgq_ptr;
               td_msgq_ptr = qprev_ptr->NEXT_MSGQ_PTR;
            } /* Endwhile */
            qprev_ptr->NEXT_MSGQ_PTR = td_msgq_ptr->NEXT_MSGQ_PTR;
            if ( td_ptr->MSG_QUEUE_TAIL == (pointer)td_msgq_ptr ) {
               td_ptr->MSG_QUEUE_TAIL = (pointer)qprev_ptr;
            } /* Endif */
         } /* Endif */
      } /* Endif */
      msgq_ptr->QUEUE = 0;
      _int_enable();
      _KLOGX3(KLOG_msgq_close, TRUE, 0);
      return TRUE;
   } else {
      _task_set_error(MSGQ_NOT_QUEUE_OWNER);
      _KLOGX3(KLOG_msgq_close, FALSE, MSGQ_NOT_QUEUE_OWNER);
      return FALSE;
   } /* Endif */

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
