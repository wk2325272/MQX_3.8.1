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
* $FileName: taskat.c$
* $Version : 3.8.10.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains a demo for creating a task AT a specified
*   address (using the specified memory for the task stack and TD
*   structure).
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <fio.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#define MAIN_TASK               10
#define TEST_TASK1              11
#define TEST_TASK1_PARAM        0x12345678

extern void main_task(uint_32);
extern void test_task(uint_32);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,   Stack, Priority,  Name,   Attributes,          Param, Time Slice */
    { MAIN_TASK,  main_task,  4000,  9,         "Main", MQX_AUTO_START_TASK, 0,     0 },
    { TEST_TASK1, test_task,  0,     8,         "test", 0,                   0,     0 },
    { 0 }
};

#define TEST_STACK_SIZE 2000
// Should be ABI aligned ... cannot do so portably....
double test_task_stack[TEST_STACK_SIZE/sizeof(double)];

volatile uint_32 test_task_val = 0;

/*TASK*-------------------------------------------------------------------
*
* Task Name    : test_task
* Comments     :
*   created task
*
*END*----------------------------------------------------------------------*/

void test_task
   (
      uint_32 param
   )
{/* Body */
   char buf[8];

   test_task_val = 1;
   printf("test_task: Local var at 0x%08x and param 0x%08x\n", buf, param);
}/* Endbody */

/*TASK*-------------------------------------------------------------------
*
* Task Name    : main_task
* Comments     :
*   starts up the task at given memory area.
*
*END*----------------------------------------------------------------------*/

void main_task
   (
      uint_32 param
   )
{/* Body */
   _task_id tid;

   printf("main_task: Task create at 0x%08x..0x%08x\n",
      test_task_stack, (uchar_ptr)test_task_stack + TEST_STACK_SIZE);

   tid = _task_create_at(0, TEST_TASK1, TEST_TASK1_PARAM, test_task_stack,
      TEST_STACK_SIZE);

   if (tid == MQX_NULL_TASK_ID) {
      printf("task create at failed");
      _mqx_fatal_error(MQX_INVALID_TASK_ID);
   } /* Endif */

   if (test_task_val != 1) {
      printf("ERROR: test_task_val != 1\n");
   } else {
      printf("PASSED\n");
   }/* Endif */

   _task_block();

} /* Endbody */

/* EOF */
