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
* $FileName: ms_pooli.c$
* $Version : 3.0.5.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for the creating a
*   message pool.
*   $Header:ms_pooli.c, 8, 9/29/2005 1:14:14 PM, Goutham D. R.$
*   $NoKeywords$
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msgpool_create_internal
* Returned Value  :  _pool_id, indicating whether or not the pool was
*                       successfully created.
* Comments        :
*
*END*------------------------------------------------------------------*/

_pool_id   _msgpool_create_internal
   (
      /*  [IN]  size of the messages being created  */
      uint_16  message_size,

      /*  [IN]  no. of messages in this pool  */
      uint_16  num_messages,

      /*  [IN]  no. of messages to grow pool by if empty */
      uint_16  grow_number,

      /*  [IN]  maximum number of messages allowed in pool */
      uint_16  grow_limit,

      /*  [IN]  whether this is a system pool or a regular pool */
      _mqx_uint  pool_type
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR kernel_data;
            MSG_COMPONENT_STRUCT_PTR  msg_component_ptr;
   register MSGPOOL_STRUCT_PTR     msgpool_ptr;
   register MSGPOOL_STRUCT_PTR     temp_msgpool_ptr;
   register MSGPOOL_STRUCT_PTR     prev_msgpool_ptr;
   register _mqx_uint               i;
            _mqx_uint               result;

   _GET_KERNEL_DATA(kernel_data);
   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);

#if MQX_CHECK_ERRORS
   if (message_size < sizeof(MESSAGE_HEADER_STRUCT)) {
      _task_set_error(MSGPOOL_MESSAGE_SIZE_TOO_SMALL);
      return ((_pool_id) 0);
   } /* Endif */
#endif

   /*
   ** Try to find an available slot in the array of msgpools for a new pool
   ** if MAX_MSGPOOLS_EVER has not yet reached MAX_MSGPOOLS then
   ** simply use MAX_MSGPOOLS_EVER as an index value and then increment it
   ** but if MAX_MSGPOOLS_EVER has reached MAX_MSGPOOLS then
   ** go back and search through the previously assigned headers to see
   ** if one has been deallocated and is available for use
   */
   if (msg_component_ptr == NULL) {
      result = _msg_create_component();
      msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (msg_component_ptr == NULL) {
         _task_set_error(result);
         return ((_pool_id)0);
      } /* Endif */
#endif
   } /* Endif */

   _int_disable();
   if (msg_component_ptr->MAX_MSGPOOLS_EVER >= msg_component_ptr->MAX_MSGPOOLS) {
      msgpool_ptr = &msg_component_ptr->MSGPOOLS_PTR[0];
      for ( i=0; i < msg_component_ptr->MAX_MSGPOOLS; i++ ) {
         if (msgpool_ptr->VALID != MSG_VALID) {
            break;
         } /* Endif */
         msgpool_ptr++;
      } /* Endfor */
      if (i == msg_component_ptr->MAX_MSGPOOLS) {
         _int_enable();
         _task_set_error(MSGPOOL_OUT_OF_POOLS);
         return ((_pool_id)0);
      } /* Endif */
   } else {
      msgpool_ptr = &msg_component_ptr->MSGPOOLS_PTR[
         msg_component_ptr->MAX_MSGPOOLS_EVER++];
   } /* Endif */

   msgpool_ptr->VALID        = MSG_VALID;
   msgpool_ptr->MESSAGE_SIZE = message_size;
   msgpool_ptr->GROW_NUMBER  = 0;
   _int_enable();

   msgpool_ptr->MSGPOOL_BLOCK_PTR = NULL;
   msgpool_ptr->MSG_FREE_LIST_PTR = NULL;
   msgpool_ptr->NEXT_MSGPOOL_PTR  = NULL;
   msgpool_ptr->SIZE              = 0;
   msgpool_ptr->MAX               = 0;
   if ( grow_number == 0 ) {
      msgpool_ptr->GROW_LIMIT = num_messages;
   } else if (grow_limit == 0) {
      msgpool_ptr->GROW_LIMIT = 0xFFFF;
   } else {
      msgpool_ptr->GROW_LIMIT = grow_limit;
   } /* Endif */
   msgpool_ptr->MSGPOOL_TYPE  = pool_type;

   if (num_messages) {
      _msgpool_add_internal(msgpool_ptr, num_messages);

      /* no messages could be created, so abort pool creation */
      if (msgpool_ptr->SIZE == 0) {
         msgpool_ptr->VALID = 0;
         _task_set_error(MQX_OUT_OF_MEMORY);
         return ((_pool_id)0);
      } /* Endif */
   } /* Endif */

   msgpool_ptr->GROW_NUMBER       = grow_number;

   if ( pool_type == SYSTEM_MSG_POOL ) {
      /* We must insert the pool into the link list of system message pools,
      ** by order of size, smallest first.
      */

      _int_disable();
      prev_msgpool_ptr = msg_component_ptr->SMALLEST_MSGPOOL_PTR;
      if (prev_msgpool_ptr == NULL) {
         /* first entry in list */
         msg_component_ptr->SMALLEST_MSGPOOL_PTR = msgpool_ptr;
         msg_component_ptr->LARGEST_MSGPOOL_PTR  = msgpool_ptr;
      } else if (prev_msgpool_ptr->MESSAGE_SIZE >= msgpool_ptr->MESSAGE_SIZE){
         /* The new pool is smaller than that at head of list */
         msgpool_ptr->NEXT_MSGPOOL_PTR        = prev_msgpool_ptr;
         msg_component_ptr->SMALLEST_MSGPOOL_PTR = msgpool_ptr;
      } else {
         temp_msgpool_ptr = prev_msgpool_ptr->NEXT_MSGPOOL_PTR;
         while (temp_msgpool_ptr != NULL) {
            /* check the relative message sizes */
            if (temp_msgpool_ptr->MESSAGE_SIZE < msgpool_ptr->MESSAGE_SIZE){
               /* continue to walk down linked list */
               prev_msgpool_ptr = temp_msgpool_ptr;
               temp_msgpool_ptr = prev_msgpool_ptr->NEXT_MSGPOOL_PTR;
            } else {
               /* this entry belongs between prev_ptr and temp_msgpool_ptr */
               prev_msgpool_ptr->NEXT_MSGPOOL_PTR = msgpool_ptr;
               msgpool_ptr->NEXT_MSGPOOL_PTR      = temp_msgpool_ptr;
               break;
            } /* Endif */
         } /* Endwhile */
         if (temp_msgpool_ptr == NULL) {
            /* searched the list and this entry belongs at the bottom */
            prev_msgpool_ptr->NEXT_MSGPOOL_PTR  = msgpool_ptr;
            msg_component_ptr->LARGEST_MSGPOOL_PTR = msgpool_ptr;
         }/* Endif */
      } /* Endif */
      _int_enable();

   } /* Endif */

   return ((_pool_id)msgpool_ptr);

} /* Endbody */


/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msgpool_add_internal
* Returned Value  :  Boolean all elements added returns TRUE
* Comments        :
*   Creates a new block of messages, and adds the block to the msgpool.
*   the messages in the block are linked together onto the free list.
*
*END*------------------------------------------------------------------*/

void _msgpool_add_internal
   (
      /* [IN] a pointer to the message pool */
      MSGPOOL_STRUCT_PTR  msgpool_ptr,

      /* [IN] the number of messages to attempt to add */
      uint_16 num_messages
   )
{ /* Body */
   INTERNAL_MESSAGE_STRUCT_PTR  first_imsg_ptr;
   INTERNAL_MESSAGE_STRUCT_PTR  imsg_ptr;
   MSGPOOL_BLOCK_STRUCT_PTR     msgpool_block_ptr;
   _mqx_int                      raw_message_size;
   _mqx_uint                     count;

   raw_message_size = sizeof(INTERNAL_MESSAGE_STRUCT) -
      sizeof(MESSAGE_HEADER_STRUCT) +  msgpool_ptr->MESSAGE_SIZE;
   _MEMORY_ALIGN_VAL_LARGER(raw_message_size);

   msgpool_block_ptr = (MSGPOOL_BLOCK_STRUCT_PTR)_mem_alloc_system((_mem_size)
      ((raw_message_size*num_messages) + sizeof(MSGPOOL_BLOCK_STRUCT) +
      PSP_MEMORY_ALIGNMENT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (msgpool_block_ptr == NULL) {
      return;
   } /* Endif */
#endif
  _mem_set_type(msgpool_block_ptr, MEM_TYPE_MESSAGE_POOL_BLOCKS);

   msgpool_block_ptr->NEXT_BLOCK_PTR   = NULL;
   msgpool_block_ptr->RAW_MESSAGE_SIZE = raw_message_size;
   msgpool_block_ptr->NUM_MESSAGES     = num_messages;

   first_imsg_ptr =(INTERNAL_MESSAGE_STRUCT_PTR)
      ((uchar _PTR_)msgpool_block_ptr +  sizeof(MSGPOOL_BLOCK_STRUCT));
   first_imsg_ptr = (INTERNAL_MESSAGE_STRUCT_PTR)
      _ALIGN_ADDR_TO_HIGHER_MEM(first_imsg_ptr);

   msgpool_block_ptr->FIRST_IMSG_PTR = first_imsg_ptr;

   imsg_ptr = first_imsg_ptr;
   count    = num_messages + 1;
   while (--count){
      imsg_ptr->NEXT           = NULL;   /* Free list pointer */
      imsg_ptr->VALID          = MSG_VALID;
      imsg_ptr->FREE           = TRUE;
      imsg_ptr->QUEUED         = FALSE;
      imsg_ptr->MSGPOOL_PTR    = msgpool_ptr;
      imsg_ptr->TD_PTR         = NULL;
      if (count > 1) {
         /* imsg is in the middle of the list */
         imsg_ptr->NEXT           = (INTERNAL_MESSAGE_STRUCT_PTR)
            ((uchar _PTR_)imsg_ptr + raw_message_size);
         imsg_ptr                 = imsg_ptr->NEXT;
      }/* Endif */
   } /* Endwhile */

   /* Now make the pool entries available */
   _int_disable();
   msgpool_block_ptr->NEXT_BLOCK_PTR = msgpool_ptr->MSGPOOL_BLOCK_PTR;
   msgpool_ptr->MSGPOOL_BLOCK_PTR    = msgpool_block_ptr;

   /* imsg_ptr here is the last message on the list */
   imsg_ptr->NEXT                    = msgpool_ptr->MSG_FREE_LIST_PTR;
   msgpool_ptr->MSG_FREE_LIST_PTR    = first_imsg_ptr;
   msgpool_ptr->SIZE += num_messages;
   msgpool_ptr->MAX  += num_messages;
   _int_enable();

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
