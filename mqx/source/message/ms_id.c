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
* $FileName: ms_id.c$
* $Version : 3.0.5.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function for creating a queue id.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msgq_get_id
* Returned Value  : _queue_id - the constructed QUEUE ID
* Comments        : This is an interface primitive is called by an 
*   application to get a queue id of a queue number.
*
*END*------------------------------------------------------------------*/

_queue_id _msgq_get_id
   (
      /* [IN] the processor on which the queue exists */
      _processor_number  processor_number,

      /* [IN] the queue number */
      _queue_number      queue_number
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (processor_number > MQX_MAX_PROCESSOR_NUMBER) {
      return(MSGQ_NULL_QUEUE_ID);
   }/* Endif */
#endif   
   if ( processor_number == 0 ) {
      processor_number = (_processor_number)kernel_data->INIT.PROCESSOR_NUMBER;
   } /* Endif */
   return BUILD_QID(processor_number, queue_number);

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
