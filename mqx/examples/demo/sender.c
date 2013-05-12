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
* $FileName: sender.c$
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

/*   Task Code -  Sender     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  Sender
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void Sender
   (
      uint_32   parameter
   )
{
   MESSAGE_HEADER_STRUCT_PTR   msg_ptr;
   _task_id                    created_task;

   Sender_Queue_qid = _msgq_open( MSGQ_FREE_QUEUE, SIZE_UNLIMITED);
   if (Sender_Queue_qid == (_queue_id)0){
         /* queue could not be opened */
   }
   created_task = _task_create(0, RESPONDER, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
   }

   /*
   * Service the message queue - Sender_Queue
   */
   msg_ptr = _msgq_receive_ticks(Sender_Queue_qid, NO_TIMEOUT);
   /* process message Start_msg */
   msg_ptr->SIZE = sizeof(MESSAGE_HEADER_STRUCT);
   msg_ptr->SOURCE_QID = msg_ptr->TARGET_QID;
   msg_ptr->TARGET_QID = Responder_Queue_qid;
   _msgq_send(msg_ptr);

   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /*
      * Service the message queue - Sender_Queue
      */
      msg_ptr = _msgq_receive_ticks(Sender_Queue_qid, NO_TIMEOUT);
      /* process message Loop_msg */
      _time_delay_ticks(3);
      msg_ptr->SIZE = sizeof(MESSAGE_HEADER_STRUCT);
      msg_ptr->SOURCE_QID = msg_ptr->TARGET_QID;
      msg_ptr->TARGET_QID = Responder_Queue_qid;
      _msgq_send(msg_ptr);

   } /* endwhile */ 

} /*end of task*/

/* End of File */
