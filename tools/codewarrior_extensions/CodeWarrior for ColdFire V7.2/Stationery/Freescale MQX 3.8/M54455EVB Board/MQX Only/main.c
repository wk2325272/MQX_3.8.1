/****************************************************************************
* 
*   This file contains MQX only stationery code.
*
****************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include "main.h"


#if !BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


TASK_TEMPLATE_STRUCT MQX_template_list[] = 
{ 
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   1500,  9,   "main", MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0,                 }
};


/*TASK*-----------------------------------------------------
* 
* Task Name    : Main_task
* Comments     :
*    This task prints " Hello World "
*
*END*-----------------------------------------------------*/

void Main_task(uint_32 initial_data)
{
   printf("\n Hello World \n"); 
   
   _mqx_exit(0);
}

/* EOF */
