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
* $Version : 3.8.11.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the mutex example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <mutex.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if ! MQX_HAS_TIME_SLICE
#error This application requires MQX_HAS_TIME_SLICE defined non-zero in user_config.h. Please recompile kernel with this option.
#endif


/* Task IDs */
#define MAIN_TASK     5
#define PRINT_TASK    6

extern void main_task(uint_32 initial_data);
extern void print_task(uint_32 initial_data);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,   Function,   Stack,  Priority,   Name,       Attributes,          Param, Time Slice */
    { MAIN_TASK,    main_task,  1000,   8,          "main",     MQX_AUTO_START_TASK, 0,     0 },
    { PRINT_TASK,   print_task, 1000,   9,          "print",    MQX_TIME_SLICE_TASK, 0,     3 },
    { 0 }
};

MUTEX_STRUCT   print_mutex;

/*TASK*--------------------------------------------------------
*
* Task Name : main_task
* Comments  : This task creates a mutex and then two 
*             instances of the print task.
*END*--------------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
   MUTEX_ATTR_STRUCT mutexattr;
   char*             string1 = "Hello from Print task 1\n";
   char*             string2 = "Print task 2 is alive\n";

   /* Initialize mutex attributes */
   if (_mutatr_init(&mutexattr) != MQX_OK) {
      printf("Initialize mutex attributes failed.\n");
      _task_block();
   }
   
   /* Initialize the mutex */ 
   if (_mutex_init(&print_mutex, &mutexattr) != MQX_OK) {
      printf("Initialize print mutex failed.\n");
      _task_block();
   }
   /* Create the print tasks */
   _task_create(0, PRINT_TASK, (uint_32)string1);
   _task_create(0, PRINT_TASK, (uint_32)string2);

   _task_block();
}   

/*TASK*--------------------------------------------------------
*
* Task Name : print_task
* Comments  : This task prints a message. It uses a mutex to 
*             ensure I/O is not interleaved.
*END*--------------------------------------------------------*/

void print_task
   (
      uint_32 initial_data
   )
{

   while(TRUE) {
      if (_mutex_lock(&print_mutex) != MQX_OK) {
         printf("Mutex lock failed.\n");
         _task_block();
      }
      _io_puts((char *)initial_data);
      _mutex_unlock(&print_mutex);
   }
}

/* EOF */
