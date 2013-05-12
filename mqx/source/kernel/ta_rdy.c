/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: ta_rdy.c$
* $Version : 3.8.6.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the function for making a task ready to run.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_ready
* Returned Value   : void
* Comments         :
*   Adds a task to the appropriate ready queue, preparing it to run again.
*
*END*----------------------------------------------------------------------*/

void _task_ready
   (
      /* [IN] the address of the task descriptor to add */
      pointer td
   )
{
    KERNEL_DATA_STRUCT_PTR  kernel_data;
    TD_STRUCT_PTR           td_ptr = (TD_STRUCT_PTR)td;

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        _usr_task_ready(td);
        return;
    }
#endif

    _GET_KERNEL_DATA(kernel_data);

    /* Perform a validity check on the td */
#if MQX_CHECK_ERRORS
    if (PROC_NUMBER_FROM_TASKID(td_ptr->TASK_ID) != kernel_data->INIT.PROCESSOR_NUMBER)
    {
        _KLOGE2(KLOG_task_ready, td_ptr);
        _task_set_error(MQX_INVALID_TASK_ID);
        _KLOGX1(KLOG_task_ready);
        return;
    }
#endif

    _INT_DISABLE();
    
    if (td_ptr->STATE == READY) {
        /* The task is already on the ready to run Queue! */
        _int_enable();
        _KLOGE2(KLOG_task_ready, td_ptr);
        _task_set_error(MQX_INVALID_TASK_STATE);
        _KLOGX1(KLOG_task_ready);
        return;
    }/* Endif */
    
    _task_ready_internal(td_ptr);
    
    _INT_ENABLE();
    
    _CHECK_RUN_SCHEDULER(); /* Let higher priority task run */
}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_task_ready
* Returned Value   : void
* Comments         :
*   Adds a task to the appropriate ready queue, preparing it to run again.
*
*END*----------------------------------------------------------------------*/
void _usr_task_ready
    (
        /* [IN] the address of the task descriptor to add */
        pointer td
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)td, 0, 0, 0, 0};
    _mqx_api_call(MQX_API_TASK_READY, &params);
}
    
#endif // MQX_ENABLE_USER_MODE
