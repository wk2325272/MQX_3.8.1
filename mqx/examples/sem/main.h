#ifndef __main_h__
#define __main_h__
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
* $FileName: main.h$
* $Version : 3.0.3.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains definitions for the semaphore example.
*
*END************************************************************************/

#define MAIN_TASK     5
#define WRITE_TASK    6
#define READ_TASK     7
#define ARRAY_SIZE    5
#define NUM_WRITERS   2

/* 
** Global data structure that is accessible by read and write tasks.
** Contains a data array that simulates a fifo. The read_index
** and write_index mark the location in the array that the read
** and write tasks are accessing. All data is protected by
** semaphores.
*/ 

typedef struct sw_fifo
{
   _task_id  DATA[ARRAY_SIZE];
   uint_32   READ_INDEX;
   uint_32   WRITE_INDEX; 
} SW_FIFO, _PTR_ SW_FIFO_PTR;

/* Funtion prototypes */
extern void main_task(uint_32 initial_data);
extern void write_task(uint_32 initial_data);
extern void read_task(uint_32 initial_data);

extern    SW_FIFO fifo;

#endif
/* EOF */
