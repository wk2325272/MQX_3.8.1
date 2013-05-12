/**HEADER********************************************************************
*
* Copyright (c) 2011 Freescale Semiconductor;
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
* $FileName: usermode.c$
* $Version : 3.8.3.2$
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

#include <lwmsgq.h>

#include <string.h>

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if !MQX_ENABLE_USER_MODE
#error This application requires MQX_ENABLE_USER_MODE defined non-zero in user_config.h. Please recompile BSP and PSP with this option.
#endif

/* Task IDs */
#define PRIV_TASK       5
#define USR_MAIN_TASK   6
#define USR_TASK        7

#define MSG_SIZE        1
#define NUM_MESSAGES    5

#define USR_TASK_CNT    6


static void privilege_task(uint_32);
static void usr_task(uint_32);
static void usr_main_task(uint_32);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,       Function,       Stack,  Priority,   Name,         Attributes,          Param,  Time Slice */
    { PRIV_TASK,        privilege_task, 1500,   8,          "priv_task",  MQX_AUTO_START_TASK, 0,      0 },
    { USR_MAIN_TASK,    usr_main_task,  1500,   8,          "usr_main_task", MQX_USER_TASK,    0,      0 },
    { USR_TASK,         usr_task,       1500,   9,          "usr_task",   MQX_USER_TASK,       0,      0 },
    { 0 }
};

USER_RW_ACCESS uint_32 *que;

USER_RW_ACCESS _lwmem_pool_id usr_pool_id;


/*TASK*-----------------------------------------------------
* 
* Task Name    : privilege_task
* Comments     : the main auto-start task
*
*END*-----------------------------------------------------*/

static void privilege_task(uint_32 initial_data) 
{
    _mqx_uint msg[MSG_SIZE];
    _lwmem_pool_id mem_pool_id;
    LWMEM_POOL_STRUCT mem_pool;
    pointer mem_pool_start;
    
    LWMEM_POOL_STRUCT_PTR usr_pool_ptr;
    pointer usr_pool_start;
    
    /* memory pool: Read-Only for User tasks */
    mem_pool_start = _mem_alloc(1024);
    mem_pool_id = _lwmem_create_pool(&mem_pool, mem_pool_start, 1024);
    _mem_set_pool_access(mem_pool_id, POOL_USER_RO_ACCESS);

    /* message queue to communicate between this task and User tasks */
    que = (uint_32*)_mem_alloc_from(mem_pool_id, sizeof(LWMSGQ_STRUCT) + NUM_MESSAGES * MSG_SIZE * sizeof(_mqx_uint));
    _usr_lwmsgq_init((pointer)que, NUM_MESSAGES, MSG_SIZE);

    /* memory pool: Read-Write for user tasks */
    usr_pool_ptr = _usr_mem_alloc(sizeof(LWMEM_POOL_STRUCT));
    usr_pool_start = _mem_alloc(1024);
    usr_pool_id = _lwmem_create_pool(usr_pool_ptr, usr_pool_start, 1024);
    _mem_set_pool_access(usr_pool_id, POOL_USER_RW_ACCESS);

    /* create the user "main" task, whcih then creates the others */
    _task_create(0, USR_MAIN_TASK, USR_TASK_CNT);

    /* receive messages from user tasks and print */
    while (1) {
        _lwmsgq_receive((pointer)que, msg, LWMSGQ_RECEIVE_BLOCK_ON_EMPTY, 0, 0);
        printf(" %c \n", msg[0]);
    }
}

/*TASK*-----------------------------------------------------
* 
* Task Name    : usr_main_task
* Comments     : user task which demonstrates creating 
*                other user tasks
*
*END*-----------------------------------------------------*/

static void usr_main_task(uint_32 initial_data) 
{
    int i;
    
    /* each user task receives its indes as a parameter */
    for (i = 0; i < initial_data; i++) {
        _usr_task_create(0, USR_TASK, i);
    }

    _task_block();
}

/*TASK*-----------------------------------------------------
* 
* Task Name    : usr_task
* Comments     : worker user task
*
*END*-----------------------------------------------------*/

static void usr_task(uint_32 initial_data) 
{
    _mqx_uint *msg;
    pointer ptr;
    
    /* allocate message object from default User memory pool */
    msg = (_mqx_uint*)_usr_mem_alloc(MSG_SIZE * sizeof(_mqx_uint));
  
    while (1) 
    {
        /* build the message with our signature */
        msg[0] = ('A'+ initial_data);
        
        /* demonstrate allocation from user pool created dynamically */
        ptr = _usr_mem_alloc_from(usr_pool_id, 16);

        /* send message to the privilege task and wait 'random' number of time */
        _usr_lwmsgq_send((pointer)que, msg, LWMSGQ_SEND_BLOCK_ON_FULL);
        _time_delay_ticks((initial_data + 1)* 10);

        /* this memory was not really needed, just for a demo purpose */
        _usr_mem_free(ptr);
    }
}
