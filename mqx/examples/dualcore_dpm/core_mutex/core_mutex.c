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
* $FileName: core_mutex.c$
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
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,          Param, Time Slice */
    { 1,   test_task, 1500,   8,        "test_task",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};


const TASK_TEMPLATE_STRUCT  MQX_template_list_1[] =
{
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,          Param, Time Slice */
    { 1,   test_task, 1500,   8,        "test_task",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

// Declare some shared data
typedef struct shared_data_struct {
    uint_32 count;
    uint_32 core_count[2];
} SHARED_DATA_STRUCT, * SHARED_DATA_STRUCT_PTR;



/*TASK*-----------------------------------------------------
*
* Task Name    : test_task
* Comments     :
*    This task tests the core mutex access
*
*END*-----------------------------------------------------*/
void test_task(uint_32 initial_data)
{
    CORE_MUTEX_PTR cm_ptr;
    uint_32        iteration=0;
    SHARED_DATA_STRUCT_PTR shared_data_ptr;
    boolean                 corenum = _psp_core_num();

    _time_delay(1000);

    cm_ptr = _core_mutex_create( 0, 1, MQX_TASK_QUEUE_FIFO );

    _core_mutex_lock(cm_ptr);
    // place shared memory in core 1's shared memory area.
    // A more complex application would want a managed pool there
    shared_data_ptr = corenum ? BSP_SHARED_RAM_START : BSP_REMOTE_SHARED_RAM_START;
    _mem_zero(shared_data_ptr, sizeof(*shared_data_ptr));
    _core_mutex_unlock(cm_ptr);

    while (1) {
        _core_mutex_lock(cm_ptr);
        shared_data_ptr->count++;
        shared_data_ptr->core_count[corenum]++;
        //_time_delay((uint_32)rand() % 20 );
        _core_mutex_unlock(cm_ptr);
        //_time_delay((uint_32)rand() % 20 );

        iteration++;
        if (((iteration % 1000) == 0) && (corenum==0)) {
            _core_mutex_lock(cm_ptr);
            printf("Count = %d, core0_count=%d, core1_count=%d\n",
                shared_data_ptr->count,
                shared_data_ptr->core_count[0],
                shared_data_ptr->core_count[1]);
            if (shared_data_ptr->count != shared_data_ptr->core_count[0] + shared_data_ptr->core_count[1]) {
                /* the total count got overwritten because of a broken lock */
                printf("Core mutex failed!\n");
                _task_block();
                break;
            }
            _core_mutex_unlock(cm_ptr);
        }
    }
}
