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
* $FileName: lwlog.c$
* $Version : 3.8.8.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the lightweight log example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <lwlog.h>


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
   /* Task Index,   Function,   Stack,  Priority,   Name,    Attributes,          Param, Time Slice */
    { MAIN_TASK,    main_task,  2000,   8,          "Main",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  :
*   This task logs 10 keystroke entries in a lightweight log,
*   then prints out the log.
*END*-----------------------------------------------------------*/

void main_task 
   (
      uint_32 initial_data
   )
{
   LWLOG_ENTRY_STRUCT entry;
   _mqx_uint          result;
   _mqx_uint          i;
   uchar              c;

   /* Create the lightweight log component */
   result = _lwlog_create_component();
   if (result != MQX_OK) {
      printf("Main task: _lwlog_create_component failed.");
      _task_block(); 
   }

   /* Create a log */
   result = _lwlog_create(MY_LOG, 10, 0);
   if (result != MQX_OK) {
      printf("Main task: _lwlog_create failed.");   
      _task_block();  
   }

   /* Write data to the log */   
   printf("Enter 10 characters:\n");
   for (i = 0; i < 10; i++) {
      c = getchar();
      result = _lwlog_write(MY_LOG, (_mqx_max_type)c,
         (_mqx_max_type)i, 0, 0, 0, 0, 0);
      if (result != MQX_OK) {
         printf("Main task: _lwlog_write failed.");   
      }
   }

   /* Read data from the log */
   printf("\nLog contains:\n");
   while (_lwlog_read(MY_LOG, LOG_READ_OLDEST_AND_DELETE,
      &entry) == MQX_OK)
   {
      printf("Time: ");
#if MQX_LWLOG_TIME_STAMP_IN_TICKS
      _psp_print_ticks((PSP_TICK_STRUCT_PTR)&entry.TIMESTAMP);
#else
      printf("%ld.%03ld%03ld", entry.SECONDS, entry.MILLISECONDS,
         entry.MICROSECONDS);
#endif
      printf(", c = %c, i=%d\n", (uchar)entry.DATA[0] & 0xff,
         (_mqx_uint)entry.DATA[1]);
   }
   
   /* Destroy the lightweight log */
   _lwlog_destroy(MY_LOG);

   _task_block();

}

/* EOF */
