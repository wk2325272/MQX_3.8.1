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
* $FileName: ta_creat.c$
* $Version : 3.8.6.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the function for creating a task.
*
*END************************************************************************/

#include "mqx_inc.h"


_task_id _task_create_internal
    (  
        /* [IN] the processor upon which to create the task */
        _processor_number processor_number,
      
        /* [IN] the task template index number for this task */
        _mqx_uint         template_index,
      
        /* [IN] the parameter to pass to the newly created task */
        uint_32           parameter,
      
        /* usermode functionality */
        boolean user
    )
{
    KERNEL_DATA_STRUCT_PTR   kernel_data;
    TD_STRUCT_PTR            td_ptr;
#if MQX_IS_MULTI_PROCESSOR
    TASK_TEMPLATE_STRUCT_PTR task_template_ptr;
#endif
    _task_id                 result = MQX_NULL_TASK_ID;
#if MQX_IS_MULTI_PROCESSOR
    boolean                  blocking;
#endif
    
    _GET_KERNEL_DATA(kernel_data);
    _KLOGE4(KLOG_task_create, processor_number, template_index, parameter);
   
#if MQX_CHECK_ERRORS
    if (template_index & SYSTEM_TASK_FLAG) {
        _task_set_error(MQX_INVALID_TEMPLATE_INDEX);
        _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, MQX_INVALID_TEMPLATE_INDEX);
        return MQX_NULL_TASK_ID;
    }
#endif

#if MQX_ENABLE_USER_MODE
    if (user) {
        if (kernel_data->INIT.MAX_USER_TASK_COUNT && kernel_data->USR_TASK_RUN_COUNT >= kernel_data->INIT.MAX_USER_TASK_COUNT) {
            _task_set_error(MQX_TOO_MANY_USER_TASKS);
            _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, MQX_TOO_MANY_USER_TASKS);
            return MQX_NULL_TASK_ID;
        }

        kernel_data->USR_TASK_RUN_COUNT++;
    }
#endif // MQX_ENABLE_USER_MODE

    if (processor_number == 0 ) {
        processor_number = (_processor_number)kernel_data->INIT.PROCESSOR_NUMBER;
#if MQX_CHECK_ERRORS
    } else if (processor_number > MQX_MAX_PROCESSOR_NUMBER) {
        _task_set_error(MQX_INVALID_PROCESSOR_NUMBER);
        _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, MQX_INVALID_PROCESSOR_NUMBER);
        return MQX_NULL_TASK_ID;
#endif 
    } else if (processor_number != kernel_data->INIT.PROCESSOR_NUMBER) {
#if MQX_ENABLE_USER_MODE
        if (user) {
            // usermode dont support multiprocessor functionality
            _task_set_error(MQX_INVALID_PROCESSOR_NUMBER);
            _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, MQX_INVALID_PROCESSOR_NUMBER);
            return MQX_NULL_TASK_ID;
        }
#endif
       
#if MQX_IS_MULTI_PROCESSOR
        if ( kernel_data->IPC != NULL ) {
#if MQX_TASK_CREATION_BLOCKS
            blocking = TRUE;
#else
            blocking = FALSE;
#endif
            if (template_index == 0) {
                /* Task template is pointed to by the parameter */
                task_template_ptr = (TASK_TEMPLATE_STRUCT_PTR)parameter;
                (*kernel_data->IPC)(blocking, processor_number, \
                    KERNEL_MESSAGES, \
                    IPC_TASK_CREATE_WITH_TEMPLATE, \
                    8L, \
                    task_template_ptr->TASK_TEMPLATE_INDEX, \
                    task_template_ptr->TASK_ADDRESS, \
                    task_template_ptr->TASK_STACKSIZE, \
                    task_template_ptr->TASK_PRIORITY, \
                    task_template_ptr->TASK_NAME, \
                    task_template_ptr->TASK_ATTRIBUTES, \
                    task_template_ptr->CREATION_PARAMETER, \
                    task_template_ptr->DEFAULT_TIME_SLICE);
            } else {
                (*kernel_data->IPC)(blocking, processor_number, \
                    KERNEL_MESSAGES, IPC_TASK_CREATE, 3, \
                    processor_number, template_index, parameter);
            } /* Endif */
            return(kernel_data->ACTIVE_PTR->INFO);
        } else {
#endif
            _task_set_error(MQX_INVALID_PROCESSOR_NUMBER);
            _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, MQX_INVALID_PROCESSOR_NUMBER);
            return MQX_NULL_TASK_ID;
#if MQX_IS_MULTI_PROCESSOR
        } /* Endif */
#endif
    } /* Endif */

    td_ptr = _task_build_internal(template_index, parameter, NULL, 0, user);

    if (td_ptr != NULL) {
        result = td_ptr->TASK_ID;
        _INT_DISABLE();
        _task_ready_internal(td_ptr);
        _INT_ENABLE();
        _CHECK_RUN_SCHEDULER(); /* Let a higher priority task run */
    }

    _KLOGX3(KLOG_task_create, result, kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);

    return(result);  
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_create
* Returned Value   : pointer to the created task descriptor or NULL on error
* Comments         :
*    This function will create a new task of the type specified by the
*    task template number.
*
*END*----------------------------------------------------------------------*/

_task_id _task_create
    (  
        /* [IN] the processor upon which to create the task */
        _processor_number processor_number,
      
        /* [IN] the task template index number for this task */
        _mqx_uint         template_index,
      
        /* [IN] the parameter to pass to the newly created task */
        uint_32           parameter
    )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_task_create(processor_number, template_index, parameter);
    }
#endif
    return _task_create_internal(processor_number, template_index, parameter, FALSE);
}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_task_create
* Returned Value   : pointer to the created task descriptor or NULL on error
* Comments         :
*    This function will create a new task of the type specified by the
*    task template number.
*
*END*----------------------------------------------------------------------*/

_task_id _usr_task_create
    (  
        /* [IN] the processor upon which to create the task */
        _processor_number processor_number,
      
        /* [IN] the task template index number for this task */
        _mqx_uint         template_index,
      
        /* [IN] the parameter to pass to the newly created task */
        uint_32           parameter
    ) 
{
    MQX_API_CALL_PARAMS params = {(uint_32)processor_number, (uint_32)template_index, (uint_32)parameter, 0, 0};
    return _mqx_api_call(MQX_API_TASK_CREATE, &params);
}

#endif // MQX_ENABLE_USER_MODE
