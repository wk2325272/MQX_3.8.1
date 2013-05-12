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
* $FileName: lwdemo.c$
* $Version : 3.0.3.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains source for the Lightweight MQX demo test.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
//#include <message.h>
#include <errno.h>
#include <lwevent.h>
#include <lwmsgq.h>
#include "lwdemo.h"

/*
** Global variable definitions
*/
#if 0 // removed in LWDEMO
/* Message Queue Ids */
_queue_id      Sender_Queue_qid;

/* Message Queue Ids */
_queue_id      Responder_Queue_qid;

/* Message Queue Ids */
_queue_id      Main_Queue_qid;

/* Message Pool Ids */
_pool_id       MsgPool_pool_id;
#endif 
/* Use light weight events */
LWEVENT_STRUCT lwevent;

/* Use light weight semaphores */
LWSEM_STRUCT lwsem;

/* Use light weight message queues */
uint_32 main_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32) +
   NUM_MESSAGES * MSG_SIZE];
uint_32 sender_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32) +
   NUM_MESSAGES * MSG_SIZE];
uint_32 responder_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32) +
   NUM_MESSAGES * MSG_SIZE];

/*   Function Code       */


/* End of File */
