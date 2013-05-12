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
* $FileName: write.c$
* $Version : 3.8.3.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the write task code.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "read.h"

/*TASK*--------------------------------------------------------
*
* Task Name : write_task
* Comments  : This task waits for the write semaphore, 
*             then writes a character to "data" and posts a
*             read semaphore. 
*END*--------------------------------------------------------*/

void write_task 
   (
      uint_32 initial_data
   )
{

   //printf("\nWrite task created: 0x%lX", initial_data);
   while (TRUE) {
      if (_lwsem_wait(&fifo.WRITE_SEM) != MQX_OK) {
         //printf("\n_lwsem_wait failed");
         _task_block();
      }
      fifo_data = (uchar)initial_data;
      _lwsem_post(&fifo.READ_SEM);
   }

}

/* EOF */
