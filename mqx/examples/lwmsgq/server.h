#ifndef __server_h__
#define __server_h__
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
* $FileName: server.h$
* $Version : 3.0.3.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains definitions for this application
*
*END************************************************************************/

#include <mqx.h>

/* Number of clients */
#define NUM_CLIENTS 3

/* Task IDs */
#define SERVER_TASK       5
#define CLIENT_TASK       6

/* This structure contains a data field and a message header structure */
#define NUM_MESSAGES  3
#define MSG_SIZE      1
extern uint_32 server_queue[];
extern uint_32 client_queue[];

/* Function prototypes */
extern void server_task (uint_32 initial_data);
extern void client_task (uint_32 initial_data);

#endif
/* EOF */
