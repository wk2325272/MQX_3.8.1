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
* $FileName: ta_abort.c$
* $Version : 3.8.11.2$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the function for aborting a task.
*
*END************************************************************************/

#include "mqx_inc.h"

_mqx_uint _task_abort_internal
    (
        /* [IN] the task id of the task to abort */
        _task_id task_id,
          
        /* usermode functionality */
        boolean user
    )
{
    KERNEL_DATA_STRUCT_PTR    kernel_data;
    TD_STRUCT_PTR             td_ptr;
    pointer                   stack_ptr;
    _processor_number         processor;
    
    _GET_KERNEL_DATA(kernel_data);
    _KLOGE2(KLOG_task_abort, task_id);
    
    if (task_id != MQX_NULL_TASK_ID) {
        processor = PROC_NUMBER_FROM_TASKID(task_id);
        if (processor != (_processor_number)kernel_data->INIT.PROCESSOR_NUMBER ) {
#if MQX_IS_MULTI_PROCESSOR
            if ( kernel_data->IPC != NULL ) {
                _KLOGX2(KLOG_task_abort, MQX_OK);
                return( (*kernel_data->IPC)(FALSE, processor,
                                            KERNEL_MESSAGES, IPC_TASK_ABORT, 1, (_mqx_uint)task_id) );
            } else {
#endif
                _KLOGX2(KLOG_task_abort, MQX_INVALID_TASK_ID);
                return(MQX_INVALID_TASK_ID);
#if MQX_IS_MULTI_PROCESSOR
            } /* Endif */
#endif
        }/* Endif */
    }/* Endif */
    
    td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
    
#if MQX_ENABLE_USER_MODE
    // check template for correct user task parameters
    if (user) {
        if (!(td_ptr->FLAGS & MQX_USER_TASK)) {
            return MQX_INVALID_TASK_ID;
        }
    }
#endif
    
#if MQX_CHECK_ERRORS
    if ( (td_ptr == NULL) || (td_ptr == SYSTEM_TD_PTR(kernel_data)) ) {
        _KLOGX2(KLOG_task_abort, MQX_INVALID_TASK_ID);
        return( MQX_INVALID_TASK_ID );
    } /* Endif */
#endif
    
    if (td_ptr == kernel_data->ACTIVE_PTR) {
        if (kernel_data->IN_ISR) {
            stack_ptr = (pointer)td_ptr->STACK_PTR;
            _PSP_SET_PC_OF_INTERRUPTED_TASK(stack_ptr, 
                                            _task_exit_function_internal);
        } else {
            _task_exit_function_internal();
        }/* Endif */
    } else {
        
        _int_disable();
        /* Task is not running */
        stack_ptr = (pointer)td_ptr->STACK_PTR;
        _PSP_SET_PC_OF_BLOCKED_TASK(stack_ptr, 
                                    _task_exit_function_internal);
        /* Start CR 1222 */
        if (td_ptr->STATE & IS_ON_TIMEOUT_Q){
            /* Remove from time queue (uses NEXT, PREV field) */
            _TIME_DEQUEUE(td_ptr, kernel_data);
            /* End CR 1222 */
        } else if (td_ptr->STATE & TD_IS_ON_QUEUE) {
            _QUEUE_REMOVE(td_ptr->INFO, td_ptr);
            /* Begin CR 1223 */
        } else if((td_ptr->STATE & BLOCKED_ON_AUX_QUEUE) ==BLOCKED_ON_AUX_QUEUE){
            /* We need to remove it here because _task_ready() below will
            change its state to READY */
            _QUEUE_REMOVE(td_ptr->INFO, &td_ptr->AUX_QUEUE);
        } /* Endif */
        /* End CR 1223 */
        if (td_ptr->STATE & IS_BLOCKED) {
            _task_ready(td_ptr);
        } /* Endif */
        _int_enable();
    }/* Endif */
    
    _KLOGX2(KLOG_task_abort, MQX_OK);
    return(MQX_OK);
}


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _task_abort
* Returned Value   : _mqx_uint task error code
* Comments         :
*  This function causes the task specified by task_id to execute its task
* exit handler, and then to destroy itself.
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _task_abort
    (
        /* [IN] the task id of the task to abort */
        _task_id task_id
    )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_task_abort(task_id);
    }
#endif
    
    return _task_abort_internal(task_id, FALSE);
}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _usr_task_abort
* Returned Value   : _mqx_uint task error code
* Comments         :
*  This function causes the task specified by task_id to execute its task
* exit handler, and then to destroy itself.
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _usr_task_abort
    (
        /* [IN] the task id of the task to abort */
        _task_id task_id
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)task_id, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_TASK_ABORT, &params);
}

#endif // MQX_ENABLE_USER_MODE
