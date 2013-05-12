#ifndef __ipc_ex_h__
#define __ipc_ex_h__
/**HEADER********************************************************************
* 
* Copyright (c) 2012 Freescale Semiconductor;
* All Rights Reserved                       
*
* Copyright (c) 1989-2012 ARC International;
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
* $FileName: ipc_ex.h$
* $Version : 3.8.0.1$
* $Date    : Feb-21-2012$
*
* Comments:
*
*   This file contains the source for the common definitions for the
*   IPC example
*
*END************************************************************************/

#define IPC_TTN            9
#define MAIN_TTN           10
#define RESPONDER_TTN      11

#define EXAMPLE_QUEUE_BASE 8
#define QUEUE_TO_REMOTE    EXAMPLE_QUEUE_BASE
#define MAIN_QUEUE         EXAMPLE_QUEUE_BASE+1
#define RESPONDER_QUEUE    EXAMPLE_QUEUE_BASE+2

typedef struct the_message
{
   MESSAGE_HEADER_STRUCT  HEADER;
   uint_32                DATA;
} THE_MESSAGE, _PTR_ THE_MESSAGE_PTR;

#endif
