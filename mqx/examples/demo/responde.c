/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: responde.c$
* $Version : 3.8.4.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include <errno.h>
#include <mutex.h>
#include <sem.h>
#include <event.h>
#include <log.h>
#include "demo.h"

/*   Task Code -  Responder     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  Responder
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void Responder
   (
      uint_32   parameter
   )
{
   MESSAGE_HEADER_STRUCT_PTR   msg_ptr;

   Responder_Queue_qid = _msgq_open( MSGQ_FREE_QUEUE, SIZE_UNLIMITED);
   if (Responder_Queue_qid == (_queue_id)0){
         /* queue could not be opened */
   }
   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /*
      * Service the message queue - Responder_Queue
      */
      msg_ptr = _msgq_receive_ticks(Responder_Queue_qid, NO_TIMEOUT);
      /* process message Respond_msg */
      msg_ptr->SIZE = sizeof(MESSAGE_HEADER_STRUCT);
      msg_ptr->SOURCE_QID = msg_ptr->TARGET_QID;
      msg_ptr->TARGET_QID = Sender_Queue_qid;
      _msgq_send(msg_ptr);

      putchar('.');
   } /* endwhile */ 
} /*end of task*/

/* End of File */
