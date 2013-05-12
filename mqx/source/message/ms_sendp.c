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
* $FileName: ms_sendp.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for sending a prioritized message.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   :  _msgq_send_priority
* Returned Value  :  boolean, indicating validity of queue_id
* Comments        :  
*    This function sends a prioritized message that is to be inserted
* into the message queue.
*
*END*------------------------------------------------------------------*/

boolean  _msgq_send_priority
   (
      /* [IN]  pointer to the  message being sent by application */
      pointer input_msg_ptr,

      /* [IN]  the priority of the message (0 - 15)
      ** priority 0 is the lowest priority (the default).
      ** priority 15 is the highest.
      */
      _mqx_uint priority
         
   )
{ /* Body */
   boolean result;
   MESSAGE_HEADER_STRUCT_PTR msg_ptr = (MESSAGE_HEADER_STRUCT_PTR)
      input_msg_ptr;
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE6(KLOG_msgq_send_priority, msg_ptr, ((MESSAGE_HEADER_STRUCT_PTR)msg_ptr)->TARGET_QID, ((MESSAGE_HEADER_STRUCT_PTR)msg_ptr)->SOURCE_QID, priority, urgent);

   if (priority > MSG_MAX_PRIORITY) {
      _task_set_error(MSGQ_INVALID_MESSAGE_PRIORITY);
      _KLOGX2(KLOG_msgq_send_priority, FALSE);
      return FALSE;
   } /* Endif */

   msg_ptr->CONTROL &= 0xF0;
   msg_ptr->CONTROL |= (priority & 0xF);

   result = _msgq_send_internal(msg_ptr, FALSE, msg_ptr->TARGET_QID);

   _KLOGX2(KLOG_msgq_send_priority, result);
   return(result);
   
} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
