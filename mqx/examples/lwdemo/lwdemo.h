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
* $FileName: lwdemo.h$
* $Version : 3.6.4.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   
*
*END************************************************************************/

/* Defines and data structures for application */

#define LWSEMB            7
#define LWSEMA            8
#define LWEVENTB          9
#define LWEVENTA          10
#define SENDER           13
#define RESPONDER        14
#define MAIN_TASK        15

#define NO_TIMEOUT        0
#define SIZE_UNLIMITED    0

/* Definitions for LW Message Queue Component */
#define NUM_MESSAGES          4
#define MSG_SIZE              1

/*
**   Externs for global data
*/
/* LW Event Definitions */
extern   LWEVENT_STRUCT lwevent;

/* LW Sem Definitions */
extern   LWSEM_STRUCT lwsem;

/* LW Message Queue Definitions */
extern   uint_32 main_queue[];
extern   uint_32 sender_queue[];
extern   uint_32 responder_queue[];

/*
** Externs for Tasks and ISRs
*/
extern void LWSemB(uint_32);
extern void LWSemA(uint_32);
extern void LWEventB(uint_32);
extern void LWEventA(uint_32);
extern void Sender(uint_32);
extern void Responder(uint_32);
extern void main_task(uint_32);


/* EOF */
