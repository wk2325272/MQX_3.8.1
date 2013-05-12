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
* $FileName: watchdog.c$
* $Version : 3.8.10.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the watchdog example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <watchdog.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#define MAIN_TASK       10
#define TEST_TASK       11

extern void main_task(uint_32);
extern void test_task(uint_32);
extern void handle_watchdog_expiry(pointer);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,   Stack, Priority,  Name,       Attributes,          Param, Time Slice */
    { MAIN_TASK,  main_task,  4000,  8,         "Main",     MQX_AUTO_START_TASK, 0,     0 },
    { TEST_TASK,  test_task,  4000,  9,         "Test",     0,                   0,     0 },
    { 0 }
};

LWSEM_STRUCT shutdown_sem;



/*FUNCTION*------------------------------------------------------
*
* Function Name  : handle_watchdog_expiry
* Returned Value : none
* Comments       :
*     This function is called when a watchdog has expired.
*END*-----------------------------------------------------------*/

void handle_watchdog_expiry
   (
      pointer td_ptr
   )
{
  _lwsem_post(&shutdown_sem);
}

/*FUNCTION*------------------------------------------------------
*
* Function Name  : waste_time
* Returned Value : input value times 10
* Comments       :
*     This function loops the specified number of times,
*     essentially wasting time.
*END*-----------------------------------------------------------*/

static _mqx_uint waste_time
   (
      _mqx_uint n
   )
{
   _mqx_uint        i;
   volatile uint_32 result;

   result = 0;
   for (i = 0; i < n; i++) {
      result += 1;
   }
   return result * 10;
}


/*TASK*----------------------------------------------------------
*
* Task Name : test_task
* Comments  : 
*   This task creates a watchdog, then loops, performing
*   work for longer and longer periods until the watchdog fires.
*END*-----------------------------------------------------------*/

void test_task
   (
      uint_32 initial_data
   )
{
   MQX_TICK_STRUCT ticks;
   _mqx_uint       result;
   _mqx_uint       n;

   _time_init_ticks(&ticks, 10);

   result = _watchdog_create_component(BSP_TIMER_INTERRUPT_VECTOR, 
      handle_watchdog_expiry);
   if (result != MQX_OK) {
      printf("\nError creating watchdog component.");
      _task_block();
   }

   n = 100;
   while (TRUE) {
      result = _watchdog_start_ticks(&ticks);
      n = waste_time(n);
      _watchdog_stop();
      printf("\n %d", n);
   }

}

/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  : 
*   This task creates test task and waits until on semaphore from watchdog.
*   In case the watchdog fires, it performs action:
*          (prints "Watchdog expired" and shutdown mqx)
*   
*END*-----------------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
  _task_id test_task;
  
   _lwsem_create(&shutdown_sem, 0);
   test_task = _task_create(0, TEST_TASK,0);
   
   
   _lwsem_wait(&shutdown_sem);
   _task_destroy(test_task);
   
   printf("\nWatchdog expired");

   _task_block();
}
/* EOF */
