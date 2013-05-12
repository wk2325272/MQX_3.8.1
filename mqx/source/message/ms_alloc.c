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
* $FileName: ms_alloc.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for allocating a message.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msg_alloc
* Returned Value  :  pointer to a message 
* Comments        :  get a message from the named pool
*
*END*------------------------------------------------------------------*/

pointer _msg_alloc
   (
      /* [IN]  the pool from which to get the message */
      _pool_id      pool
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
#if MQX_CHECK_ERRORS
            MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
#endif
   register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   register MESSAGE_HEADER_STRUCT_PTR   message_ptr;
   register MSGPOOL_STRUCT_PTR          msgpool_ptr;
            uint_16                     grow_number;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_msg_alloc, pool);

#if MQX_CHECK_ERRORS
   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
   if (msg_component_ptr == NULL){
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX3(KLOG_msg_alloc, NULL, MQX_COMPONENT_DOES_NOT_EXIST);
      return(NULL);
   } /* Endif */
#endif

   message_ptr = NULL;
   msgpool_ptr = (MSGPOOL_STRUCT_PTR)pool;

#if MQX_CHECK_VALIDITY || MQX_CHECK_ERRORS
   if ( 
#if MQX_CHECK_VALIDITY
      (msgpool_ptr->VALID != MSG_VALID) 
#if MQX_CHECK_ERRORS
      || 
#endif
#endif
#if MQX_CHECK_ERRORS
      (msgpool_ptr->MSGPOOL_TYPE != MSG_POOL) 
#endif
   ) {  
      _task_set_error(MSGPOOL_INVALID_POOL_ID);
      _KLOGX3(KLOG_msg_alloc, message_ptr, MSGPOOL_INVALID_POOL_ID);
      return (message_ptr);
   } /* Endif */
#endif

   /* Start CR 2055 */
   if ( (msgpool_ptr->SIZE == 0) && (msgpool_ptr->GROW_NUMBER) && 
      ( (msgpool_ptr->MAX < msgpool_ptr->GROW_LIMIT) || 
      (msgpool_ptr->GROW_LIMIT == 0) ) )   
   /* End CR 2055 */     
   {
      /* Attempt to add elements to the pool */
      grow_number = msgpool_ptr->GROW_NUMBER;
      /* Start CR 621 */
      if (grow_number > (uint_16)(msgpool_ptr->GROW_LIMIT - msgpool_ptr->MAX)) {
      /* End CR 621 */
         grow_number = msgpool_ptr->GROW_LIMIT - msgpool_ptr->MAX;
      } /* Endif */
      _msgpool_add_internal(msgpool_ptr, grow_number);
   } /* Endif */

   _INT_DISABLE();
   imsg_ptr = msgpool_ptr->MSG_FREE_LIST_PTR;
   if (imsg_ptr == NULL) {
      _int_enable();
      _task_set_error(MSGPOOL_OUT_OF_MESSAGES);
      _KLOGX3(KLOG_msg_alloc, message_ptr, MSGPOOL_OUT_OF_MESSAGES);
      return((pointer)message_ptr);
   } /* Endif */
   
   msgpool_ptr->MSG_FREE_LIST_PTR = imsg_ptr->NEXT;
   --msgpool_ptr->SIZE;
   _INT_ENABLE();
   imsg_ptr->FREE   = FALSE;
   imsg_ptr->QUEUED = FALSE;
   if (kernel_data->IN_ISR) {
      imsg_ptr->TD_PTR = NULL;
   } else {
      imsg_ptr->TD_PTR = kernel_data->ACTIVE_PTR;
   } /* Endif */
   message_ptr = (MESSAGE_HEADER_STRUCT_PTR)&imsg_ptr->MESSAGE;
   message_ptr->TARGET_QID = MSGQ_NULL_QUEUE_ID;
   message_ptr->SOURCE_QID = MSGQ_NULL_QUEUE_ID;
   message_ptr->SIZE       = msgpool_ptr->MESSAGE_SIZE;
   message_ptr->CONTROL    = MSG_HDR_ENDIAN | MSG_DATA_ENDIAN;
   _KLOGX3(KLOG_msg_alloc, message_ptr, MQX_OK);
   return (pointer)message_ptr;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
