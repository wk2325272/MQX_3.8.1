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
* $FileName: ms_open.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for creating message queues.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_open
* Returned Value  : _queue_id - indicating successful queue creation,
*   a NULL indicates error
* Comments        : This function is called by the application,
*   to open a specific queue.  If Queue is non-zero, then it must not be
*   currently open.  IF Queue is zero, then a free queue entry is
*   found and the queue_id returned to the requester.
*
*END*------------------------------------------------------------------*/

_queue_id _msgq_open
   (
      /* [IN] the queue being opened */
      _queue_number queue,

      /* [IN] the maximum number of entries allowed in this queue */
      uint_16       max_queue_size
   )
{ /* Body */
   _queue_id result;
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE3(KLOG_msgq_open, queue, max_queue_size);

   result =  _msgq_open_internal(queue, max_queue_size, MSG_QUEUE,
      (void (_CODE_PTR_)(pointer))NULL, NULL);

   _KLOGX2(KLOG_msgq_open, result);

   return result;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
