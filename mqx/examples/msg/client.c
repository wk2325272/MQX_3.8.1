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
* $FileName: client.c$
* $Version : 3.8.4.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the code for the client_task
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include "server.h"


/*TASK*--------------------------------------------------------
*
* Task Name : client_task
* Comments  : This task opens a message queue and
*   allocates a message from the message pool.
*   The message is sent to the server_task and
*   a reply is waited for. The reply message is then freed.
*END*--------------------------------------------------------*/

void client_task 
   (
      uint_32 index
   )
{
   SERVER_MESSAGE_PTR msg_ptr;
   _queue_id          client_qid;
   boolean            result;

   client_qid  = _msgq_open((_queue_number)(CLIENT_QUEUE_BASE +
      index), 0);

   if (client_qid == 0) {
      printf("\nCould not open a client message queue\n");
      _task_block();
   }
   
   while (TRUE) {
      /*allocate a message*/
      msg_ptr = (SERVER_MESSAGE_PTR)_msg_alloc(message_pool);

      if (msg_ptr == NULL) {
         printf("\nCould not allocate a message\n");
         _task_block();
      }

      msg_ptr->HEADER.SOURCE_QID = client_qid;      
      msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, SERVER_QUEUE);
      msg_ptr->HEADER.SIZE = sizeof(MESSAGE_HEADER_STRUCT) + 
         strlen((char_ptr)msg_ptr->DATA) + 1;
      msg_ptr->DATA[0] = ('A'+ index);
     
      printf("Client Task %ld\n", index);  
      
      result = _msgq_send(msg_ptr);
      
      if (result != TRUE) {
         printf("\nCould not send a message\n");
         _task_block();
      }
   
      /* wait for a return message */
      msg_ptr = _msgq_receive(client_qid, 0);
      
      if (msg_ptr == NULL) {
         printf("\nCould not receive a message\n");
         _task_block();
      }
    
       /* free the message */
      _msg_free(msg_ptr);
   }

}

/* EOF */
