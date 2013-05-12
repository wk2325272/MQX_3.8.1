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
* $FileName: ms_spool.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for allocating system message
*   pools.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msgpool_create_system
* Returned Value  :  boolean, indicating whether or not the pool was 
*                       successfully created.
* Comments        :
*
*END*------------------------------------------------------------------*/

boolean   _msgpool_create_system
   (
      /*  [IN]  size of the messages being created  */
      uint_16  message_size,

      /*  [IN]  no. of messages in this pool  */
      uint_16  num_messages,

      /*  [IN]  no. of messages to grow pool by if empty */
      uint_16  grow_number,

      /*  [IN]  maximum number of messages allowed in pool */
      uint_16  grow_limit
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)
   _pool_id   ret_value;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE5(KLOG_msgpool_create_system, message_size, num_messages, grow_number, grow_limit);

   ret_value = _msgpool_create_internal(message_size, num_messages, grow_number,
      grow_limit, SYSTEM_MSG_POOL);

   _KLOGX2(KLOG_msgpool_create_system, ret_value == (_pool_id)0 );

   if ( ret_value == (_pool_id)0 ) {
      return FALSE;
   } else {
      return TRUE;
   } /* Endif */

} /* Endbody */

                
/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msg_alloc_system
* Returned Value  :  pointer to a message struct for an message.
* Comments        :  get a message that is closest,but larger, to the size 
*                    requested by the application.
*
*END*------------------------------------------------------------------*/

pointer _msg_alloc_system
   (
      /* [IN]  size of the message required */
      _msg_size message_size
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
            MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
   register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   register MESSAGE_HEADER_STRUCT_PTR   message_ptr;
   register MSGPOOL_STRUCT_PTR          msgpool_ptr;
            uint_16                     grow_number;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_msg_alloc_system, message_size );

   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      _task_set_error(MQX_COMPONENT_DOES_NOT_EXIST);
      _KLOGX2( KLOG_msg_alloc_system, NULL );
      return(NULL);
   }/* Endif */
#endif
 
   message_ptr = NULL;
   _INT_DISABLE();
   msgpool_ptr = msg_component_ptr->SMALLEST_MSGPOOL_PTR;
   while (msgpool_ptr != NULL) {
      if (msgpool_ptr->MESSAGE_SIZE >= message_size) {
         imsg_ptr = msgpool_ptr->MSG_FREE_LIST_PTR;
         if ( (imsg_ptr == NULL) && 
            (msgpool_ptr->GROW_NUMBER) && 
            (msgpool_ptr->MAX < msgpool_ptr->GROW_LIMIT) )
         {
            /* Attempt to add elements to the pool */
            grow_number = msgpool_ptr->GROW_NUMBER;
            /* Start CR 621 */
            if ( ((uint_16)(msgpool_ptr->MAX + grow_number) > msgpool_ptr->GROW_LIMIT)){
            /* End CR 621 */
               grow_number   = msgpool_ptr->GROW_LIMIT - msgpool_ptr->MAX;
            } /* Endif */
            _msgpool_add_internal(msgpool_ptr, grow_number);
            imsg_ptr = msgpool_ptr->MSG_FREE_LIST_PTR;
         } /* Endif */
         if ( imsg_ptr != NULL ) {
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
            message_ptr->SIZE       = message_size;
            message_ptr->CONTROL    = MSG_HDR_ENDIAN | MSG_DATA_ENDIAN;

            _KLOGX2(KLOG_msg_alloc_system, message_ptr);
            return (pointer)message_ptr;
         } /* Endif */
      } /* Endif */
      msgpool_ptr = msgpool_ptr->NEXT_MSGPOOL_PTR;
   } /* Endwhile */

   _int_enable();
   _KLOGX2(KLOG_msg_alloc_system, message_ptr);
   return (pointer)message_ptr;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
