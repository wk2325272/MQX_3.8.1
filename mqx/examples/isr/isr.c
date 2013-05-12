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
* $FileName: isr.c$
* $Version : 3.8.8.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the isr example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#define MAIN_TASK 10
extern void main_task(uint_32);
extern void new_tick_isr(pointer);




const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,  Stack, Priority, Name,        Attributes,          Param, Time Slice */
    { MAIN_TASK,  main_task, 2000,  8,        "Main",      MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

typedef struct my_isr_struct
{
   pointer               OLD_ISR_DATA;
   void      (_CODE_PTR_ OLD_ISR)(pointer);
   _mqx_uint             TICK_COUNT;
} MY_ISR_STRUCT, _PTR_ MY_ISR_STRUCT_PTR;

/*ISR*-----------------------------------------------------------
*
* ISR Name : new_tick_isr
* Comments :
*   This ISR replaces the existing timer ISR, then calls the 
*   old timer ISR.
*END*-----------------------------------------------------------*/

void new_tick_isr
   (
      pointer user_isr_ptr
   )
{
   MY_ISR_STRUCT_PTR  isr_ptr;

   isr_ptr = (MY_ISR_STRUCT_PTR)user_isr_ptr;
   isr_ptr->TICK_COUNT++;

   /* Chain to the previous notifier */
   (*isr_ptr->OLD_ISR)(isr_ptr->OLD_ISR_DATA);
}

/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  : 
*   This task installs a new ISR to replace the timer ISR.
*   It then waits for some time, finally printing out the
*   number of times the ISR ran.
*END*-----------------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
   MY_ISR_STRUCT_PTR  isr_ptr;

   isr_ptr = _mem_alloc_zero((_mem_size)sizeof(MY_ISR_STRUCT));

   isr_ptr->TICK_COUNT   = 0;
   isr_ptr->OLD_ISR_DATA =
      _int_get_isr_data(BSP_TIMER_INTERRUPT_VECTOR);
   isr_ptr->OLD_ISR      =
      _int_get_isr(BSP_TIMER_INTERRUPT_VECTOR);

   _int_install_isr(BSP_TIMER_INTERRUPT_VECTOR, new_tick_isr,
      isr_ptr);

   _time_delay_ticks(200);

   printf("\nTick count = %d\n", isr_ptr->TICK_COUNT);

   _task_block();

}

/* EOF */
