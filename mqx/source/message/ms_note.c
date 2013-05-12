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
* $FileName: ms_note.c$
* $Version : 3.0.5.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the functions for getting and setting the message
*   queue notification functions.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_set_notification_function
* Returned Value  : address of the old notification function
* Comments        : 
*   This function attatches a notification function to a message queue.
*   When a message is placed onto the message queue, then
*   this function will be called.
*
*END*------------------------------------------------------------------*/

void (_CODE_PTR_ _msgq_set_notification_function
   (
      /* [IN] the queue upon which to install the function */
      _queue_id         qid,

      /* [IN] the function to be called when an entry is put on the queue */
      void (_CODE_PTR_ notification_function)(pointer),

      /* [IN] information to pass to the notification function */
      pointer          notification_data
   )) (pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   MSG_COMPONENT_STRUCT_PTR msg_component_ptr;
   MSGQ_STRUCT_PTR          msgq_ptr;
   void         (_CODE_PTR_ result)(pointer);
   _queue_number            queue;
#if MQX_CHECK_ERRORS
   _processor_number        pnum;
#endif

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_msgq_set_notification_function, qid, notification_function, notification_data);

   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      result = NULL;
      _KLOGX2(KLOG_msgq_set_notification_function, result);
      return(result);
   } /* Endif */
#endif

   queue = QUEUE_FROM_QID(qid);

#if MQX_CHECK_ERRORS
   pnum  = PROC_NUMBER_FROM_QID(qid);
   if (( pnum != kernel_data->INIT.PROCESSOR_NUMBER) ||  ( ! VALID_QUEUE(queue))) {
      _task_set_error(MSGQ_INVALID_QUEUE_ID);
      result = NULL;
      _KLOGX2(KLOG_msgq_set_notification_function, result);
      return result;
   } /* Endif */
#endif

   msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];
   if ( msgq_ptr->QUEUE != (queue) ) {
      _task_set_error(MSGQ_QUEUE_IS_NOT_OPEN);
      result = NULL;
   } else {
      result = msgq_ptr->NOTIFICATION_FUNCTION;
      msgq_ptr->NOTIFICATION_FUNCTION           = notification_function;
      msgq_ptr->NOTIFICATION_FUNCTION_PARAMETER = notification_data;
   } /* Endif */

   _KLOGX2(KLOG_msgq_set_notification_function, result);
   return result;

} /* Endbody */


/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_get_notification_function
* Returned Value  : error code, MQX_OK if no errors
* Comments        : 
*   This function returns the notification function attatched
*   to a message queue.
*
*END*------------------------------------------------------------------*/

_mqx_uint _msgq_get_notification_function
   (
      /* [IN] the queue upon which to install the function */
      _queue_id              qid,

      /* [OUT] the function to be called when an entry is put on the queue */
      void (_CODE_PTR_ _PTR_ notification_function)(pointer),

      /* [OUT] information to pass to the notification function */
      pointer _PTR_          notification_data
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   MSG_COMPONENT_STRUCT_PTR msg_component_ptr;
   MSGQ_STRUCT_PTR          msgq_ptr;
   _mqx_uint                 result = MQX_OK;
   _queue_number            queue;
#if MQX_CHECK_ERRORS
   _processor_number        pnum;
#endif

   _GET_KERNEL_DATA(kernel_data);

   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif

   queue = QUEUE_FROM_QID(qid);

#if MQX_CHECK_ERRORS
   pnum  = PROC_NUMBER_FROM_QID(qid);
   if (( pnum != kernel_data->INIT.PROCESSOR_NUMBER) || ( ! VALID_QUEUE(queue))) {
      result = MSGQ_INVALID_QUEUE_ID;
   } /* Endif */
#endif

   if (result == MQX_OK) {
      msgq_ptr = &msg_component_ptr->MSGQS_PTR[queue];
      if ( msgq_ptr->QUEUE != (queue) ) {
         result = MSGQ_QUEUE_IS_NOT_OPEN;
      } else {
         *notification_function = msgq_ptr->NOTIFICATION_FUNCTION;
         *notification_data     = msgq_ptr->NOTIFICATION_FUNCTION_PARAMETER;
      } /* Endif */
   } /* Endif */

   return result;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
