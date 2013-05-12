#ifndef __read_h__
#define __read_h__
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
* $FileName: read.h$
* $Version : 3.0.3.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains the definitions for this example.
*
*END************************************************************************/


/* Number of Writer Tasks */
#define NUM_WRITERS  3

/* Task IDs */
#define WRITE_TASK   5
#define READ_TASK    6

/* 
** Global data structure accessible by read and write tasks. 
** It contains two lightweight semaphores that govern access to 
** the data variable.
*/  
typedef struct sw_fifo
{
   LWSEM_STRUCT   READ_SEM;
   LWSEM_STRUCT   WRITE_SEM;
   uchar          DATA;
} SW_FIFO, _PTR_ SW_FIFO_PTR;


/* Function prototypes */
extern void write_task(uint_32 initial_data);
extern void read_task(uint_32 initial_data);

extern SW_FIFO fifo;

#endif
/* EOF */
