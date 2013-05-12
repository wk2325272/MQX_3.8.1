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
* $FileName: slcd.c$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the source for the hello example program.
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


/* Task IDs */
#define SLCD_TASK 5
#define TEXT_LENGH 9
extern void slcd_task(uint_32);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,          Param, Time Slice */
    { SLCD_TASK,   slcd_task, 1500,   8,        "slcd",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/*TASK*-----------------------------------------------------
* 
* Task Name    : SLCD_TASK
* Comments     :
*    This task prints  string dedined on SLCD
*
*END*-----------------------------------------------------*/
void slcd_task
   (
      uint_32 initial_data
   )
{
   uint_8 i,j;
   char_ptr string ;
   _SLCDModule_Init();
   _SLCDModule_ClearLCD(1);
  for(;;){ 
    for(i=30;i>0;i--){      
    string = _mem_alloc_system_zero(sizeof(string));
    strcpy(string,"SLCD DEMO ") ;
   _SLCDModule_PrintString(string, i);
   _time_delay(200);
   _SLCDModule_ClearLCD(0);
    }
    //size = sizeof(string);
   for(i=0;i< TEXT_LENGH ;i++){
      for(j=0;j< TEXT_LENGH;j++)
      {
        string[j] = string[j+1]  ;    
      } 
      _SLCDModule_PrintString(string, 0);
      _time_delay(200);
   }
  
   
   }

  _task_block();

}

/* EOF */
