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
* $FileName: klog.c$
* $Version : 3.8.12.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the kernel log example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <log.h>
#include <klog.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


extern void main_task(uint_32 initial_data);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,   Stack, Priority,  Name,     Attributes,          Param, Time Slice */
    { 10,         main_task,  1500,  8,         "Main",   MQX_AUTO_START_TASK, 0,     0},
    { 0 }
};

/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  :
*   This task logs timer interrupts to the kernel log,
*   then prints out the log.
*END*-----------------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
   _mqx_uint result;
   _mqx_uint i;

   /* Create the kernel log */
   result = _klog_create(2*1024, 0);
   if (result != MQX_OK) {
      printf("Main task: _klog_create failed %08x", result);
      _task_block();
   }

   /* Enable kernel logging */
   _klog_control(KLOG_ENABLED | KLOG_CONTEXT_ENABLED |
      KLOG_INTERRUPTS_ENABLED| KLOG_SYSTEM_CLOCK_INT_ENABLED |
      KLOG_FUNCTIONS_ENABLED | KLOG_TIME_FUNCTIONS |
      KLOG_INTERRUPT_FUNCTIONS, TRUE);

   /* Write data into kernel log */   
   for (i = 0; i < 10; i++) {
      _time_delay_ticks(5 * i);
   }

   /* Disable kernel logging */
   _klog_control(0xFFFFFFFF, FALSE);

   /* Read data from the kernel log */
   printf("\nKernel log contains:\n");
   while (_klog_display()){
   }

   _task_block();

}

/* EOF */
