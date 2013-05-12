/**HEADER********************************************************************
*
* Copyright (c) 2012 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2012 ARC International;
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
* $FileName: hello_dc.c$
* $Version : 3.8.0.1$
* $Date    : Feb-21-2012$
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

extern void test_task(uint_32);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
/* Task Index,   Function,   Stack,  Priority, Name,     Attributes,    Param,            Time Slice */
    { 1,   test_task, 1500,   8,        "test_task",  MQX_AUTO_START_TASK, (uint_32)"core 0", 0 },
    { 0 }
};


const TASK_TEMPLATE_STRUCT  MQX_template_list_1[] =
{
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,    Param,            Time Slice */
    { 1,   test_task, 1500,   8,        "test_task",  MQX_AUTO_START_TASK, (uint_32)"core 1", 0 },
    { 0 }
};



/*TASK*-----------------------------------------------------
*
* Task Name    : test_task
* Comments     :
*    This task tests the core mutex access
*
*END*-----------------------------------------------------*/
void test_task(uint_32 initial_data)
{
    char *          name = (char *)initial_data;

    while (TRUE) {
        printf("Hello from %s\n", name);
        _sched_yield();
    }
}
