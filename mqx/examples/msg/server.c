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
* $FileName: server.c$
* $Version : 3.8.10.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the code for the server_task
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "server.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/* declaration of a global message pool */
_pool_id   message_pool;
_pool_id fp_message_pool;

/*TASK*--------------------------------------------------------
*
* Task Name : server_task
* Comments  : This task creates a message queue for itself,
*  creates a message pool, creates three client tasks, and 
*  then waits for a message.  After recieving a message, 
*  the task returns the message to the sender. 
*END*--------------------------------------------------------*/


void server_task 
   (
      uint_32 param
   )
{
   SERVER_MESSAGE_PTR msg_ptr;
   _mqx_uint          i;
   _queue_id          server_qid;
   boolean            result;
   _task_id           task_id;

   /* open a message queue */
   server_qid = _msgq_open(SERVER_QUEUE, 0);
   
   if (server_qid == 0) {
      printf("\nCould not open the server message queue\n");
      _task_block();
   }

   /* create a message pool */   
   message_pool = _msgpool_create(sizeof(SERVER_MESSAGE), 
      NUM_CLIENTS, 0, 0);
      
   if (message_pool == MSGPOOL_NULL_POOL_ID) {
      printf("\nCount not create a message pool\n");
      _task_block();
   }

   /* create the client tasks */
   for (i = 0; i < NUM_CLIENTS; i++) {
      task_id = _task_create(0, CLIENT_TASK, (uint_32)i);
      
      if (task_id == 0) {
         printf("\nCould not create a client task\n");
         _task_block();
      }
   }
      
   while (TRUE) {
      msg_ptr = _msgq_receive(server_qid, 0);
      
      if (msg_ptr == NULL) {
         printf("\nCould not receive a message\n");
         _task_block();
      }

      printf(" %c \n", msg_ptr->DATA[0]);
  
      /* return the message */   
      msg_ptr->HEADER.TARGET_QID = msg_ptr->HEADER.SOURCE_QID;
      msg_ptr->HEADER.SOURCE_QID = server_qid;
      
      result = _msgq_send(msg_ptr);
      
      if (result != TRUE) {
         printf("\nCould not send a message\n");
         _task_block();
      }
   }
}
      
/* EOF */
