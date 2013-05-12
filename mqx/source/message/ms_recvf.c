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
* $FileName: ms_recvf.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for reading a message from a message 
*   queue.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_receive_for
* Returned Value  : pointer to a message structure, or NULL if timeout 
*                   occurred.
* Comments        : Dequeue the first item from
*   the specified queue.  
*   ONLY tasks can call this function
*
*END*------------------------------------------------------------------*/

pointer _msgq_receive_for
   (
      /* [IN]  id of the queue from which a message is to be received */
     _queue_id              queue_id,

      /* 
      ** [IN]  the number of ticks which can expire before
      **       this request times out
      */
      MQX_TICK_STRUCT_PTR   tick_ptr
   )
{ /* Body */
#if MQX_KERNEL_LOGGING
   KERNEL_DATA_STRUCT_PTR      kernel_data;
#endif
   MESSAGE_HEADER_STRUCT_PTR   message_ptr;
   _mqx_uint                    error;


#if MQX_KERNEL_LOGGING
   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_msgq_receive_for, queue_id, tick_ptr);
#endif

   message_ptr = _msgq_receive_internal(queue_id, tick_ptr, 
      MSG_TIMEOUT_RELATIVE, &error);

#if MQX_KERNEL_LOGGING
   if ( (error ==  MQX_OK) && (message_ptr == NULL) ) {
      _KLOGX3(KLOG_msgq_receive_for, message_ptr, MSGQ_MESSAGE_NOT_AVAILABLE);
   } else if (error == MQX_OK) {
      _KLOGX5(KLOG_msgq_receive_for, message_ptr, message_ptr->TARGET_QID,
         message_ptr->SOURCE_QID, *(_mqx_uint_ptr)((uchar_ptr)message_ptr+
         sizeof(MESSAGE_HEADER_STRUCT)));
   } else {
      _KLOGX3(KLOG_msgq_receive_for, message_ptr, error);
   } /* Endif */
#endif

   return (pointer)message_ptr;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
