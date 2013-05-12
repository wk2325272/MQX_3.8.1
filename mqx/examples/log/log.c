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
* $FileName: log.c$
* $Version : 3.8.9.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the source for the isr example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <log.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#define MAIN_TASK  10
#define MY_LOG     1
extern void main_task(uint_32 initial_data);
 
const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,  Stack, Priority,   Name,        Attributes,          Param, Time Slice */
    { MAIN_TASK,  main_task, 2000,  8,          "Main",      MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

typedef struct entry_struct
{
   LOG_ENTRY_STRUCT   HEADER;
   _mqx_uint          C;
   _mqx_uint          I;
} ENTRY_STRUCT, _PTR_ ENTRY_STRUCT_PTR;


/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  :
*   This task logs 10 keystroke entries then prints out the log.
*END*-----------------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
   ENTRY_STRUCT entry;
   _mqx_uint    result;
   _mqx_uint    i;
   uchar        c;

   /* Create the log component */
   result = _log_create_component();
   if (result != MQX_OK) {
      printf("Main task: _log_create_component failed");
      _task_block();  
   }

   /* Create a log */
   result = _log_create(MY_LOG,
      10 * (sizeof(ENTRY_STRUCT)/sizeof(_mqx_uint)), 0);
   if (result != MQX_OK) {
      printf("Main task: _log_create failed");   
      _task_block();  
   }

   /* Write the data to the log */   
   printf("Please type in 10 characters:\n");
   for (i = 0; i < 10; i++) {
      c = getchar();
      result = _log_write(MY_LOG, 2, (_mqx_uint)c, i);
      if (result != MQX_OK) {
         printf("Main task: _log_write failed");   
      }
   }

   /* Read data from the log */
   printf("\nLog contains:\n");
   while (_log_read(MY_LOG, LOG_READ_OLDEST_AND_DELETE, 2,
      (LOG_ENTRY_STRUCT_PTR)&entry) == MQX_OK)
   {
      printf("Time: %lu.%03u%03u, c=%c, i=%u\n",
         entry.HEADER.SECONDS,
         (_mqx_uint)entry.HEADER.MILLISECONDS,
         (_mqx_uint)entry.HEADER.MICROSECONDS,
         (uchar)entry.C & 0xff,
         entry.I);
   }

   /* Destroy the log */
   _log_destroy(MY_LOG);

   _task_block();

}

/* EOF */
