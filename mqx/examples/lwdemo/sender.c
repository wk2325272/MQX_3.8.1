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
* $Version : 3.7.8.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains source for the Lightweight MQX demo test.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <errno.h>
#include <lwevent.h>
#include <lwmsgq.h>
#include "lwdemo.h"

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
   _mqx_uint  msg[MSG_SIZE];
   _task_id   created_task;
   
   created_task = _task_create(0, RESPONDER, 0);
   if (created_task == MQX_NULL_TASK_ID) {
      /* task creation failed */
   }

   /*
   * Service the message queue - Sender_Queue
   */
   _lwmsgq_receive((pointer)sender_queue, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
   _lwmsgq_send((pointer)responder_queue, msg, LWMSGQ_SEND_BLOCK_ON_FULL);   

   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /*
      * Service the message queue - Sender_Queue
      */
      _lwmsgq_receive((pointer)sender_queue, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
      _time_delay_ticks(3);
      _lwmsgq_send((pointer)responder_queue, msg, LWMSGQ_SEND_BLOCK_ON_FULL);
   } /* endwhile */ 

} /*end of task*/

/* End of File */
