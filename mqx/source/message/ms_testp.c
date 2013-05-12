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
* $FileName: ms_testp.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing all message pools 
*   in the system.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgpool_test
* Returned Value  : _mqx_uint MQX_OK or a MQX error code
* Comments        : This function tests all message pools in the system
* for consistency and validity.
*
*END*------------------------------------------------------------------*/

_mqx_uint _msgpool_test
   ( 
      /* [OUT] the message pool in error */
      pointer _PTR_ pool_error_ptr,

      /* [OUT] the message in error */
      pointer _PTR_ msg_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR      kernel_data;
   MSG_COMPONENT_STRUCT_PTR    msg_component_ptr;
   MSGPOOL_STRUCT_PTR          msgpool_ptr;
   MSGPOOL_BLOCK_STRUCT_PTR    msgpool_block_ptr;
   INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   _mqx_uint                    i,j,raw_message_size;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_msgpool_test, pool_error_ptr, msg_error_ptr);
   
   msg_component_ptr = _GET_MSG_COMPONENT_STRUCT_PTR(kernel_data);
#if MQX_CHECK_ERRORS
   if (msg_component_ptr == NULL){
      _KLOGX2(KLOG_msgpool_test, MQX_COMPONENT_DOES_NOT_EXIST);
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif
   
   /* Check all the message pools */
   msgpool_ptr = msg_component_ptr->MSGPOOLS_PTR;
   i = msg_component_ptr->MAX_MSGPOOLS + 1;
   while (--i){
      _int_disable();
      if (msgpool_ptr->VALID == MSG_VALID){
         /* The pool has been created */
         
         /* Search through all of the message pool blocks for this pool */
         msgpool_block_ptr   = msgpool_ptr->MSGPOOL_BLOCK_PTR;
         while (msgpool_block_ptr != NULL){
            raw_message_size = msgpool_block_ptr->RAW_MESSAGE_SIZE;
            imsg_ptr  = (INTERNAL_MESSAGE_STRUCT_PTR)
               msgpool_block_ptr->FIRST_IMSG_PTR;
            j = msgpool_block_ptr->NUM_MESSAGES + 1;
            while (--j){
               if ((imsg_ptr->VALID != MSG_VALID) || 
                   (imsg_ptr->MSGPOOL_PTR != msgpool_ptr))
               {
                  _int_enable();
                  *pool_error_ptr = msgpool_ptr;
                  *msg_error_ptr  = imsg_ptr;
                  _KLOGX4(KLOG_msgpool_test, MSGQ_INVALID_MESSAGE, msgpool_ptr,
                     imsg_ptr);
                  return(MSGQ_INVALID_MESSAGE);
               } /* Endif */
               imsg_ptr =(INTERNAL_MESSAGE_STRUCT_PTR)
                  ((uchar _PTR_)imsg_ptr + raw_message_size);
            } /* Endwhile */

            msgpool_block_ptr = msgpool_block_ptr->NEXT_BLOCK_PTR;
         } /* Endwhile */

      } /* Endif */
      _int_enable();
      msgpool_ptr++;
   } /* Endwhile */

   _KLOGX2(KLOG_msgpool_test, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
