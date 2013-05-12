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
* $FileName: demo.h$
* $Version : 3.0.3.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   
*
*END************************************************************************/

/*   Defines and data structures for application       */

#define MUTEXB            5
#define MUTEXA            6
#define SEMB              7
#define SEMA              8
#define EVENTB            9
#define EVENTA           10
#define BTIMESLICETASK   11
#define ATIMESLICETASK   12
#define SENDER           13
#define RESPONDER        14
#define MAIN_TASK        15

#define NO_TIMEOUT        0
#define SIZE_UNLIMITED    0


/* Defines for Semaphore Component */
#define SEM_INITIAL_NUMBER    10
#define SEM_GROWTH            10
#define SEM_MAXIMUM           20

/* Defines for Event Component */
#define EVENT_INITIAL_NUMBER  10
#define EVENT_GROWTH          10
#define EVENT_MAXIMUM         20

/*
**   Externs for global data
*/
/*  Message Queue Ids */
extern   _queue_id     Sender_Queue_qid;

/*  Message Queue Ids */
extern   _queue_id     Responder_Queue_qid;

/*  Message Queue Ids */
extern   _queue_id     Main_Queue_qid;

/*  Message Pool Ids */
extern   _pool_id      MsgPool_pool_id;

/*  Mutex Definitions */
extern   MUTEX_STRUCT  Mutex1;

/*
** Externs for Tasks and ISRs
*/
extern void MutexB(uint_32);
extern void MutexA(uint_32);
extern void SemB(uint_32);
extern void SemA(uint_32);
extern void EventB(uint_32);
extern void EventA(uint_32);
extern void BTimeSliceTask(uint_32);
extern void ATimeSliceTask(uint_32);
extern void Sender(uint_32);
extern void Responder(uint_32);
extern void main_task(uint_32);


/* EOF */
