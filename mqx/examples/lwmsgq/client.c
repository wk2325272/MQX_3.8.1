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
* $Version : 3.6.4.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the code for the client_task
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <lwmsgq.h>
#include "server.h"


/*TASK*--------------------------------------------------------
*
* Task Name : client_task
* Comments  : This task sends a message to the server_task and
*   then waits for a reply.
*END*--------------------------------------------------------*/

void client_task
   (
      uint_32 index
   )
{
   _mqx_uint          msg[MSG_SIZE];
  
   while (TRUE) {
      msg[0] = ('A'+ index);
     
      printf("Client Task %ld\n", index);
      _lwmsgq_send((pointer)server_queue, msg, LWMSGQ_SEND_BLOCK_ON_FULL);

      _time_delay_ticks(1);
      
      /* wait for a return message */
      _lwmsgq_receive((pointer)client_queue, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
   }

}

/* EOF */
