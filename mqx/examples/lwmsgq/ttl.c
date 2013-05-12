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
* $FileName: ttl.c$
* $Version : 3.7.6.0$
* $Date    : Mar-23-2011$
*
* Comments:
*
*   This file contains the task template list for this processor.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <lwmsgq.h>
#include "server.h"

uint_32 server_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32) +
   NUM_MESSAGES * MSG_SIZE];
uint_32 client_queue[sizeof(LWMSGQ_STRUCT)/sizeof(uint_32) +
   NUM_MESSAGES * MSG_SIZE];


const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,   Function,    Stack,  Priority,  Name,       Attributes,          Param, Time Slice */
    { SERVER_TASK,  server_task, 2000,   8,         "server",   MQX_AUTO_START_TASK, 0,     0 },
    { CLIENT_TASK,  client_task, 1000,   8,         "client",   0,                   0,     0 },    
    { 0 }
};

/* EOF */
