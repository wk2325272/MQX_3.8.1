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
* $Version : 3.8.4.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the code for the write task.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <sem.h>
#include "main.h"

/*TASK*----------------------------------------------------------
*
* Task Name : write_task
* Comments  : 
*     This task opens a connection to all three semaphores then 
*     waits for sem.write and sem.index. It writes one entry
*     in the data array and posts sem.index and sem.read.
*END*-----------------------------------------------------------*/
     
void write_task 
   (
      uint_32 initial_data
   )
{
   pointer write_sem;
   pointer read_sem;
   pointer index_sem;

   /* open connections to all the semaphores */
   if (_sem_open("sem.write", &write_sem) != MQX_OK) {
      printf("\nOpening write semaphore failed.");
      _task_block();
   }                  
   if (_sem_open("sem.index", &index_sem) != MQX_OK) {
      printf("\nOpening index semaphore failed.");
      _task_block();
   }
   if (_sem_open("sem.read", &read_sem) != MQX_OK) {
      printf("\nOpening read semaphore failed.");
      _task_block();
   }

   while (TRUE) {
      /* wait for the semphores */
      if (_sem_wait(write_sem, 0) != MQX_OK) {
         printf("\nWwaiting for Write semaphore failed");
         _task_block();
      }                  
      if (_sem_wait(index_sem, 0) != MQX_OK) {
         printf("\nWaiting for index semaphore failed");
         _task_block();
      }
      
      fifo.DATA[fifo.WRITE_INDEX++] = _task_get_id();
      if (fifo.WRITE_INDEX >= ARRAY_SIZE) {
         fifo.WRITE_INDEX = 0;
      }
      /* Post the semaphores */
      _sem_post(index_sem);
      _sem_post(read_sem);
   }

}

/* EOF */
