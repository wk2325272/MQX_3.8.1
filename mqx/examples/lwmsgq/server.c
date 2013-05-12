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
* $Version : 3.6.7.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the code for the server_task
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <lwmsgq.h>
#include "server.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/*TASK*--------------------------------------------------------
*
* Task Name : server_task
* Comments  : This task initializes the message queues,
*  creates three client tasks, and then waits for a message.
*  After recieving a message, the task returns the message to 
*  the sender.
*END*--------------------------------------------------------*/

void server_task 
   (
      uint_32 param
   )
{
   _mqx_uint          msg[MSG_SIZE];
   _mqx_uint          i;
   _mqx_uint          result;

   result = _lwmsgq_init((pointer)server_queue, NUM_MESSAGES, MSG_SIZE);
   if (result != MQX_OK) {
      // lwmsgq_init failed
   } /* Endif */
   result = _lwmsgq_init((pointer)client_queue, NUM_MESSAGES, MSG_SIZE);
   if (result != MQX_OK) {
      // lwmsgq_init failed
   } /* Endif */
   
   /* create the client tasks */
   for (i = 0; i < NUM_CLIENTS; i++) {
      _task_create(0, CLIENT_TASK, (uint_32)i);
   }
      
   while (TRUE) {
      _lwmsgq_receive((pointer)server_queue, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
      printf(" %c \n", msg[0]);
      
      _lwmsgq_send((pointer)client_queue, msg, LWMSGQ_SEND_BLOCK_ON_FULL);
   }

}
      
/* EOF */
