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
* $FileName: read.c$
* $Version : 3.8.4.0$
* $Date    : Oct-14-2011$
*
* Comments:
*
*   This file contains the read task code.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "read.h"

#if ! MQX_ENABLE_USER_MODE
#error This application requires MQX_ENABLE_USER_MODE defined non-zero in user_config.h. Please recompile BSP and PSP with this option.
#endif

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


USER_RO_ACCESS SW_FIFO fifo;
USER_RW_ACCESS uchar fifo_data;

/*TASK*--------------------------------------------------------
*
* Task Name : read_task
* Comments  : This task creates two semaphores and 
*             NUM_WRITER write_tasks. It waits
*             on the read_sem and finally outputs the 
*             "data" variable.
*END*--------------------------------------------------------*/

void read_task 
   (
      uint_32 initial_data
   )
{
   _task_id  task_id;
   _mqx_uint result;
   _mqx_uint i;

   /* Create the lightweight semaphores */
   result = _usr_lwsem_create(&fifo.READ_SEM, 0);
   if (result != MQX_OK) {
      printf("\nCreating read_sem failed: 0x%X", result);
      _task_block();
   }

   result = _usr_lwsem_create(&fifo.WRITE_SEM, 1);
   if (result != MQX_OK) {
      printf("\nCreating write_sem failed: 0x%X", result);
      _task_block();
   }

   /* Create the write tasks */
   for (i = 0; i < NUM_WRITERS; i++) {
      task_id = _task_create(0, WRITE_TASK, (uint_32)('A' + i));
      printf("\nwrite_task created, id 0x%lX", task_id);
   }

   while(TRUE) {
      result = _lwsem_wait(&fifo.READ_SEM);
      if (result != MQX_OK) {
         printf("\n_lwsem_wait failed: 0x%X", result);
         _task_block();
      }
      putchar('\n');
      putchar(fifo_data);
      _lwsem_post(&fifo.WRITE_SEM);
   }

}

/* EOF */
