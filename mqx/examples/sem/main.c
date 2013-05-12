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
* $FileName: main.c$
* $Version : 3.8.7.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the code for the main task.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <sem.h>
#include "main.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


SW_FIFO      fifo;

/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  : 
*     This task initializes three semaphores, creates NUM_WRITERS 
*     write_task's, and creates one read_task.
*END*----------------------------------------------------------*/

void main_task 
   (
      uint_32 initial_data
   )
{
   _task_id   task_id;
   _mqx_uint  i;

   fifo.READ_INDEX  = 0;
   fifo.WRITE_INDEX = 0;

   /* Create the semaphores */
   if (_sem_create_component(3,1,6) != MQX_OK) {
      printf("\nCreate semaphore component failed");
      _task_block();
   }
   if (_sem_create("sem.write", ARRAY_SIZE, 0) != MQX_OK) {
      printf("\nCreating write semaphore failed");
      _task_block();
   }
   if (_sem_create("sem.read", 0, 0) != MQX_OK) {
      printf("\nCreating read semaphore failed");
      _task_block();
   }
   if (_sem_create("sem.index", 1, 0) != MQX_OK) {
      printf("\nCreating index semaphore failed");
      _task_block();
   }

   /* Create the tasks */
   for (i = 0; i < NUM_WRITERS; i++) {
      task_id = _task_create(0, WRITE_TASK, (uint_32)i);
      printf("\nwrite_task created, id 0x%lx", task_id);
   }
   
   task_id = _task_create(0,READ_TASK, 0);
   printf("\nread_task created, id 0x%lX", task_id);

   _task_block();

}

/* EOF */
