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
* $FileName: ms_free.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for freeing a message
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msg_free
* Returned Value  : None
* Comments        : mark the specified message as 'free'
*
*END*------------------------------------------------------------------*/

void  _msg_free
   (
      /*  [IN]   pointer to a message struct which is to be released  */
      pointer msg_ptr
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
   register INTERNAL_MESSAGE_STRUCT_PTR imsg_ptr;
   register MSGPOOL_STRUCT_PTR          msgpool_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_msg_free, msg_ptr);

   imsg_ptr = GET_INTERNAL_MESSAGE_PTR(msg_ptr);
#if MQX_CHECK_VALIDITY
   if ( imsg_ptr->VALID != MSG_VALID ) {
      _KLOGX2(KLOG_msg_free, MQX_INVALID_POINTER);
      _task_set_error(MQX_INVALID_POINTER);
      return;
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (imsg_ptr->FREE){
      _KLOGX2(KLOG_msg_free, MQX_NOT_RESOURCE_OWNER);
      _task_set_error(MQX_NOT_RESOURCE_OWNER);
      return;
   } /* Endif */
   if (imsg_ptr->QUEUED){
      _KLOGX2(KLOG_msg_free, MSGQ_MESSAGE_IS_QUEUED);
      _task_set_error(MSGQ_MESSAGE_IS_QUEUED);
      return;
   } /* Endif */
#endif

   msgpool_ptr = imsg_ptr->MSGPOOL_PTR;
   imsg_ptr->FREE   = TRUE;
   imsg_ptr->QUEUED = FALSE;

   _INT_DISABLE();
   /* Link onto the free list */
   imsg_ptr->NEXT = msgpool_ptr->MSG_FREE_LIST_PTR;
   msgpool_ptr->MSG_FREE_LIST_PTR   = imsg_ptr;
   ++msgpool_ptr->SIZE;
   _INT_ENABLE();

   _KLOGX2(KLOG_msg_free, MQX_OK);

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
