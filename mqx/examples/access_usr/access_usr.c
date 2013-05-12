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
* $FileName: access_usr.c$
* $Version : 3.8.7.1$
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

#include <string.h>

#if ! MQX_ENABLE_USER_MODE
#error This application requires MQX_ENABLE_USER_MODE defined non-zero in user_config.h. Please recompile BSP and PSP with this option.
#endif

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/* Task IDs */
#define HELLO_TASK      5
#define USR_TASK_DEF    6
#define USR_TASK_RW     7
#define USR_TASK_RO     8
#define USR_TASK_NO_R   9
#define USR_TASK_NO_W   10


static void hello_task(uint_32);
static void usr_task(uint_32);
static void usr_task_no_w(uint_32);


USER_RW_ACCESS static int g_urw;
USER_RO_ACCESS static int g_uro;
USER_NO_ACCESS static int g_uno;
static int g_udef;

USER_RW_ACCESS static char str[200];

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,          Param, Time Slice */
    { HELLO_TASK,   hello_task, 1500,   9,  "hello",    MQX_AUTO_START_TASK,    0,  0 },
    { USR_TASK_DEF,     usr_task,   1500,   8,  "usr_task def", MQX_USER_TASK,          0,  0 },
    { USR_TASK_RW,     usr_task,   1500,   8,  "usr_task rw", MQX_USER_TASK,          0,  0 },
    { USR_TASK_RO,     usr_task,   1500,   8,  "usr_task ro", MQX_USER_TASK,          0,  0 },
    { USR_TASK_NO_R,     usr_task,   1500,   8,  "usr_task no_r", MQX_USER_TASK,          0,  0 },
    { USR_TASK_NO_W,     usr_task_no_w,   1500,   8,  "usr_task no_w", MQX_USER_TASK,     0,  0 },
    { 0 }
};


/*TASK*-----------------------------------------------------
* 
* Task Name    : hello_task
* Comments     :
*    This task prints " Hello World "
*
*END*-----------------------------------------------------*/
static void hello_task
   (
      uint_32 initial_data
   )
{
    printf("\n\nUSER PRIVILEGE MODE TEST\n"); 
        
    str[0] = 0;
    _task_create(0, USR_TASK_DEF, (uint_32)&g_udef);
    printf("global user default access task: %s\n", str);

    
    str[0] = 0;
    _task_create(0, USR_TASK_RW, (uint_32)&g_urw);
    printf("global user rw task: %s\n", str);
    
    
    str[0] = 0;
    _task_create(0, USR_TASK_RO, (uint_32)&g_uro);
    printf("global user read only task: %s\n", str);

    
    str[0] = 0;
    _task_create(0, USR_TASK_NO_R, (uint_32)&g_uno);
    printf("global user no access - read task: %s\n", str);

    
    str[0] = 0;
    _task_create(0, USR_TASK_NO_W, (uint_32)&g_uno);
    printf("global user no access - write task: %s\n", str);
    
    printf("\n\nEND\n"); 
    
    _task_block();
}

static void usr_task(uint_32 initial_data) {
    uint_32 val;
    uint_32 *test = (uint_32*)initial_data;
    
    strcat(str, "READING ");
  
    val = *test;
    
    strcat(str, "READ_DONE ");
    
    val++;
    
    strcat(str, "WRITING ");
    *test = val;

    strcat(str, "WRITE_DONE");

    _task_block();
}

static void usr_task_no_w(uint_32 initial_data) {
    uint_32 val = 0;
    
    strcat(str, "WRITING ");
    g_uno = val;
    strcat(str, "WRITE_DONE");

    _task_block();
}
