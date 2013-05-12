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
* $FileName: ta_dest.c$
* $Version : 3.8.11.2$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for destroying a task.
*
*END************************************************************************/

#include "mqx_inc.h"

_mqx_uint _task_destroy_internal
    (
        /* [IN] the task id of the task to destroy */
        _task_id task_id,
      
        /* usermode functionality */
        boolean user
   )
{
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   TD_STRUCT_PTR            victim_ptr;
   TD_STRUCT_PTR            td_ptr;
   _processor_number        processor;
   uint_16                  task_num;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_task_destroy, task_id);

/* Start CR 1902 */ 
   if (task_id != MQX_NULL_TASK_ID) {
      processor = PROC_NUMBER_FROM_TASKID(task_id);
      if (processor != (_processor_number)kernel_data->INIT.PROCESSOR_NUMBER ) {
#if MQX_IS_MULTI_PROCESSOR
         if ( kernel_data->IPC != NULL ) {
            _KLOGX2(KLOG_task_destroy, MQX_OK);
            return( (*kernel_data->IPC)(FALSE, processor,
               KERNEL_MESSAGES, IPC_TASK_DESTROY, 1, (_mqx_uint)task_id) );
         } else {
#endif
            _KLOGX2(KLOG_task_destroy, MQX_INVALID_TASK_ID);
            return(MQX_INVALID_TASK_ID);
#if MQX_IS_MULTI_PROCESSOR
         } /* Endif */
#endif
      }/* Endif */
   }/* Endif */ 
   
   victim_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);

#if MQX_ENABLE_USER_MODE
    // check template for correct user task parameters
    if (user) {
        if (!(victim_ptr->FLAGS & MQX_USER_TASK)) {
            return MQX_INVALID_TASK_ID;
        }
    }
#endif
    
   /* Make sure we are not trying to destroy the system task */
#if MQX_CHECK_ERRORS
   if ((victim_ptr == NULL) || (victim_ptr == SYSTEM_TD_PTR(kernel_data))) {
      _KLOGX2(KLOG_task_destroy, MQX_INVALID_TASK_ID);
      return MQX_INVALID_TASK_ID;
   } /* Endif */
#endif
/* End CR 1902 */

   /* First, serialize task creation/destruction */
   _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   _int_disable();

   if (victim_ptr->STATE == DYING) {
      /* Task is already being destroyed */
      /* SPR P171-0021-01 re-enable interrupts */
      _int_enable();
      /* END SPR */
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);
      _KLOGX2(KLOG_task_destroy, MQX_OK);
      return MQX_OK;
   } /* Endif */

   /* remove the victim from any queues it may be in. */
   if (victim_ptr->STATE == READY) {
      if (victim_ptr != kernel_data->ACTIVE_PTR) {
         /* Remove other task from ready queue */
         _QUEUE_UNLINK(victim_ptr);
      } /* Endif */
   } else if (victim_ptr->STATE & IS_ON_TIMEOUT_Q) {
      /* Remove from time queue (uses NEXT, PREV field) */
      _TIME_DEQUEUE(victim_ptr, kernel_data);
   } else if (victim_ptr->STATE & TD_IS_ON_QUEUE) {
      /* 
      ** Remove from queue, where suspended 
      ** eg, MUTEX_BLOCKED, IO_BLOCKED, TASKQ_BLOCKED
      ** (uses NEXT, PREV field)
      */
      _QUEUE_REMOVE(victim_ptr->INFO, victim_ptr);
   } /* Endif */
               
   if (victim_ptr->STATE & TD_IS_ON_AUX_QUEUE) {
      /* 
      ** Remove from queue, where suspended 
      ** eg, LWSEM_BLOCKED
      ** (uses AUX_QUEUE field)
      */
      _QUEUE_REMOVE(victim_ptr->INFO, &victim_ptr->AUX_QUEUE);
   } /* Endif */

   victim_ptr->STATE   = DYING;

#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
   /* Stop Floating point context monitoring */
   if (kernel_data->FP_ACTIVE_PTR == victim_ptr) {
      kernel_data->FP_ACTIVE_PTR = NULL;
   }/* Endif */
#endif

#if MQXCFG_ENABLE_DSP && PSP_HAS_DSP
   /* Stop DSP context monitoring */
   if (kernel_data->DSP_ACTIVE_PTR == victim_ptr) {
      kernel_data->DSP_ACTIVE_PTR = NULL;
   }/* Endif */
#endif

   _int_enable();

#if MQX_COMPONENT_DESTRUCTION
   {
      _mqx_uint i;
      for (i = 0; i < MAX_KERNEL_COMPONENTS; ++i) {
         if (kernel_data->COMPONENT_CLEANUP[i] != NULL) {
            (*kernel_data->COMPONENT_CLEANUP[i])(victim_ptr);
         } /* Endif */
      } /* Endfor */
   }
#endif

#if MQX_IO_COMPONENT_DESTRUCTION
   /* Call I/O component cleanup functions */      
   {
      _mqx_uint i;
      for (i = 0; i < MAX_IO_COMPONENTS; ++i) {
         if (kernel_data->IO_COMPONENT_CLEANUP[i] != NULL) {
            (*kernel_data->IO_COMPONENT_CLEANUP[i])(victim_ptr);
         } /* Endif */
      } /* Endfor */
   }
#endif

   /* Save the task number */
   task_num = TASK_NUMBER_FROM_TASKID(victim_ptr->TASK_ID);

   td_ptr = kernel_data->ACTIVE_PTR;

/* START CR 897 */
   if ((victim_ptr->FLAGS & TASK_STACK_PREALLOCATED) == 0) {
      pointer block_ptr = _mem_get_next_block_internal(victim_ptr, NULL);
      while (block_ptr != NULL) {
         pointer next_block_ptr = _mem_get_next_block_internal(victim_ptr, block_ptr);
         if (td_ptr != victim_ptr) {
            _mem_transfer_internal(block_ptr, SYSTEM_TD_PTR(kernel_data));
         } /* Endif */
         if (block_ptr != victim_ptr) {
            _mem_free(block_ptr);
         } /* Endif */
         block_ptr = next_block_ptr;
      } /* Endwhile */
   } /* Endif */
/* END CR 897 */

   victim_ptr->TASK_ID = TASK_ID_DYING;

   /* Start SPR P171-0017-01                */
   /* Remove td from linked list of all tds */
   if (_QUEUE_GET_SIZE(&kernel_data->TD_LIST) == 1) {
      /* Destroying the last task */
      kernel_data->TASK_NUMBER   = 1;
      kernel_data->INSERT_TD_PTR = NULL;
   } else if (kernel_data->TASK_NUMBER > task_num) {
      kernel_data->TASK_NUMBER   = task_num;
      kernel_data->INSERT_TD_PTR = 
         (TD_STRUCT_PTR)((uchar_ptr)victim_ptr->TD_LIST_INFO.PREV -
         FIELD_OFFSET(TD_STRUCT,TD_LIST_INFO));
   } /* Endif */
   _QUEUE_REMOVE(&kernel_data->TD_LIST, &victim_ptr->TD_LIST_INFO);
   /* End SPR P171-0017-01                  */
               
   /* Free up create/destroy for other tasks */
   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   if (victim_ptr == td_ptr) {
      /* We are destroying the current task */
      _int_disable();
      _QUEUE_UNLINK(victim_ptr);  /* Remove myself from the ready queue */
      _psp_destroy_stack_frame(victim_ptr);
/* START CR 897 */
      if ((victim_ptr->FLAGS & TASK_STACK_PREALLOCATED) == 0) {
         _mem_transfer_internal(victim_ptr, SYSTEM_TD_PTR(kernel_data));
         _mem_free(victim_ptr);   /* Free the td which includes task's stack! */
      } /* Endif */
/* END CR 897 */
      /* Now we must jump to the scheduler */
      _sched_run_internal();   /* WILL NEVER RETURN FROM HERE */
   }/* Endif */

   _psp_destroy_stack_frame(victim_ptr);

    /* Free the task descriptor. */
/* START CR 897 */
   if ((victim_ptr->FLAGS & TASK_STACK_PREALLOCATED) == 0) {
      _mem_free(victim_ptr);
   } /* Endif */
/* END CR 897 */
   
#if MQX_ENABLE_USER_MODE
    if (victim_ptr->FLAGS & MQX_USER_TASK) {
        _int_disable();
        kernel_data->USR_TASK_RUN_COUNT--;
        _int_enable();
    }
#endif /* MQX_ENABLE_USER_MODE */

   _KLOGX2(KLOG_task_destroy, MQX_OK);
   return(MQX_OK);

}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_destroy
* Returned Value   : _mqx_uint error code
* Comments         : 
*   Destroy the task specified by the given task-id (the victim).  
*   All of the victim's resources are released, specifically,
*   all queues closed, and all memory is freed.
*   Component cleanup functions are called to free any component
*   resources owned by this task.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _task_destroy
    (
        /* [IN] the task id of the task to destroy */
        _task_id task_id
    )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_task_destroy(task_id);
    }
#endif
    
    return _task_destroy_internal(task_id, FALSE);
}


#if MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_usr_task_destroy
* Returned Value   : _mqx_uint error code
* Comments         : 
*   Destroy the task specified by the given task-id (the victim).  
*   All of the victim's resources are released, specifically,
*   all queues closed, and all memory is freed.
*   Component cleanup functions are called to free any component
*   resources owned by this task.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _usr_task_destroy
    (
        /* [IN] the task id of the task to destroy */
        _task_id task_id
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)task_id, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_TASK_DESTROY, &params);
}

#endif // MQX_ENABLE_USER_MODE
