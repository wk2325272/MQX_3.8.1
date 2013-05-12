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
* $FileName: ms_tdel.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   message queues, and queue ids.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msg_cleanup
* Returned Value  : none
* Comments        : This function is called when a task is being
*   destroyed.  It is used to close all open message queues, and then
*   to free all messages owned by this task
*
*END*------------------------------------------------------------------*/

void _msg_cleanup
   ( 
      /* [IN] the task descriptor of the task that is being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
   MSGQ_STRUCT_PTR             msgq_ptr;
   MSGQ_STRUCT_PTR             qprev_ptr;
   MSGPOOL_STRUCT_PTR          msgpool_ptr;
   MSGPOOL_BLOCK_STRUCT_PTR    msgpool_block_ptr;
   INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   INTERNAL_MESSAGE_STRUCT_PTR tmp_imsg_ptr;
   _mqx_uint                    i,j,raw_message_size;

   _GET_KERNEL_DATA(kernel_data);
   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL) {
      return;
   } /* Endif */
#endif
   
   /* Delete the message queues owned by the task, and free the messages
   ** on the queues
   */
   msgq_ptr = (MSGQ_STRUCT_PTR)td_ptr->MSG_QUEUE_HEAD;
   while (msgq_ptr != NULL){
      if (msgq_ptr->TD_PTR != NULL){
         msgq_ptr->TD_PTR->MESSAGES_AVAILABLE -= msgq_ptr->NO_OF_ENTRIES;
      } /* Endif */

      imsg_ptr = msgq_ptr->FIRST_MSG_PTR;
      while ( imsg_ptr != NULL ) {
         tmp_imsg_ptr = imsg_ptr->NEXT;
         imsg_ptr->QUEUED = FALSE;
         _msg_free((pointer)&imsg_ptr->MESSAGE);
         imsg_ptr = tmp_imsg_ptr;
      } /* Endwhile */
      qprev_ptr                = msgq_ptr;
      msgq_ptr                 = msgq_ptr->NEXT_MSGQ_PTR;
      qprev_ptr->FIRST_MSG_PTR = NULL;
      qprev_ptr->NEXT_MSGQ_PTR = NULL;
      qprev_ptr->QUEUE         = 0;
   } /* Endwhile */

   td_ptr->MSG_QUEUE_HEAD = NULL;
   td_ptr->MSG_QUEUE_TAIL = NULL;

   /* Search through all of the message pools, looking for any messages
   ** owned by this task.  If found free them.
   */
   msgpool_ptr = msg_component_ptr->MSGPOOLS_PTR;
   i = msg_component_ptr->MAX_MSGPOOLS + 1;
   while (--i){
      if (msgpool_ptr->VALID == MSG_VALID){

         /* Search through all of the message pool blocks for this pool */
         msgpool_block_ptr   = msgpool_ptr->MSGPOOL_BLOCK_PTR;
         while (msgpool_block_ptr != NULL){
            raw_message_size = msgpool_block_ptr->RAW_MESSAGE_SIZE;
            imsg_ptr  = (INTERNAL_MESSAGE_STRUCT_PTR)
               ((uchar _PTR_)msgpool_block_ptr + sizeof(MSGPOOL_BLOCK_STRUCT));

            /* if the message is not free, not queued and the
            ** owner is the task being destroyed, then Free the message
            */
            j = msgpool_block_ptr->NUM_MESSAGES + 1;
            while (--j){
               if ( (imsg_ptr->TD_PTR == td_ptr) &&
                    (imsg_ptr->FREE == FALSE) &&
                    (imsg_ptr->QUEUED == FALSE) )
               {
                  _msg_free(&imsg_ptr->MESSAGE);
               } /* Endif */
               imsg_ptr =(INTERNAL_MESSAGE_STRUCT_PTR)
                  ((uchar _PTR_)imsg_ptr + raw_message_size);
            } /* Endwhile */

            msgpool_block_ptr = msgpool_block_ptr->NEXT_BLOCK_PTR;
         } /* Endwhile */

      } /* Endif */
      msgpool_ptr++;
   } /* Endwhile */

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
