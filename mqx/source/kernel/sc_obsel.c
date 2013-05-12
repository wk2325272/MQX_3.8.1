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
* $FileName: sc_obsel.c$
* $Version : 3.5.5.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   scheduling policy of a task.  These functions are obsolete,
*   and are provided as compatiblity functions only.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_DYNAMIC_PRIORITIES || MQX_HAS_TIME_SLICE

/*--------------------------------------------------------------------------*/
/*                   OBSOLETE FUNCTION DEFINITIONS 
*/

/* To be used in the parameter structure to indicate that the field is
** to be ignored, or as the policy to be applied.
*/
#define SCHED_NOT_CHANGED (_mqx_uint)(-1)

/*--------------------------------------------------------------------------*/
/* 
** SCHEDULER PARAMETERS STRUCTURE
**
** This structure defines the parameters structure use to 
** set/modify/get the scheduling method for a task
*/
typedef struct sched_param_struct {

   /* The software priority level the task is to run at */
   _mqx_uint     PRIORITY;
   
   /* The time slice interval (milliseconds) the task is to use */
   uint_32     ROUND_ROBIN_INTERVAL;

   /* The policy of the requested item (used in get parameters) */
   _mqx_uint     POLICY;

} SCHED_PARAM_STRUCT, _PTR_ SCHED_PARAM_STRUCT_PTR;

extern _mqx_uint _sched_getparam(_task_id, SCHED_PARAM_STRUCT_PTR);
extern _mqx_uint _sched_setparam(_task_id, SCHED_PARAM_STRUCT_PTR);
extern _mqx_uint _sched_setparam_td(pointer, SCHED_PARAM_STRUCT_PTR);
extern _mqx_uint _sched_setscheduler(_task_id, _mqx_uint, SCHED_PARAM_STRUCT_PTR);
extern void     _sched_set_param_internal(TD_STRUCT_PTR, SCHED_PARAM_STRUCT_PTR);


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_set_param_internal
* Returned Value   : none
* Comments         :
*   This function sets various parameters for task scheduling.
* Values of 0 are ignored.
*
*END*----------------------------------------------------------------------*/

void _sched_set_param_internal
   (
      /* [IN] the address of the task descriptor to which the
      **    scheduling parameters apply.
      */
      register TD_STRUCT_PTR          td_ptr,

     /* [IN] the address of a parameter structure used
     **    to define other tasking parameters (task priority level,
     **    and round robin time slice size)
     */
      register SCHED_PARAM_STRUCT_PTR param_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
            READY_Q_STRUCT_PTR     ready_q_ptr;
   
   _GET_KERNEL_DATA(kernel_data);
#if MQX_HAS_TIME_SLICE
   if (param_ptr->ROUND_ROBIN_INTERVAL != SCHED_NOT_CHANGED) {
      _int_disable();
      PSP_MILLISECONDS_TO_TICKS_QUICK(param_ptr->ROUND_ROBIN_INTERVAL, &td_ptr->TIME_SLICE);
      _int_enable();
   } /* Endif */
#endif
#if MQX_HAS_DYNAMIC_PRIORITIES
   if ((param_ptr->PRIORITY == SCHED_NOT_CHANGED) ||
        (param_ptr->PRIORITY == td_ptr->MY_QUEUE->PRIORITY))
   {
      return;
   }

   _int_disable();

   /* Make the change permanent */
   ready_q_ptr = kernel_data->READY_Q_LIST;
   td_ptr->HOME_QUEUE = ready_q_ptr - param_ptr->PRIORITY;
   if (td_ptr->BOOSTED) {
      /* Can only change priority to a higher (lower value) */
      if (param_ptr->PRIORITY < td_ptr->MY_QUEUE->PRIORITY) {
         /* Move the task to the correct priority level */
         _sched_set_priority_internal(td_ptr, param_ptr->PRIORITY);
        /* Allow higher priority tasks to run */
        _CHECK_RUN_SCHEDULER(); 
      } /* Endif */
   } else {
      _sched_set_priority_internal(td_ptr, param_ptr->PRIORITY);
      /* Allow higher priority tasks to run */
      _CHECK_RUN_SCHEDULER(); 
   } /* Endif */
   _int_enable();
#endif      
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_setparam
* Returned Value   : _mqx_uint MQX_OK or MAX_MQX_UINT
*                     If MAX_MQX_UINT then the task error code is set to one of:
*                     SCHED_INVALID_PARAMETER_PTR, SCHED_INVALID_PARAMETER
*                     SCHED_INVALID_TASK_ID
* Comments         :
*   This function sets the current tasking parameters for the specified
* task.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_setparam
   (
      /* [IN] the task id to apply this policy to:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id task_id,

      /* [IN] the address of a parameter structure used
      **    to define other tasking parameters (task priority level,
      **    and round robin time slice size)
      */
      SCHED_PARAM_STRUCT_PTR param_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td_ptr;
   /* Start CR 330 */
   /*         TIME_STRUCT            time; */
   /* End CR 330 */

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE6(KLOG_sched_setparam, task_id, param_ptr, param_ptr->PRIORITY, param_ptr->ROUND_ROBIN_INTERVAL, param_ptr->POLICY);

#if MQX_CHECK_ERRORS
   /* Validate parameters */
   if (param_ptr == NULL) {
      _task_set_error(MQX_SCHED_INVALID_PARAMETER_PTR);
      _KLOGX3(KLOG_sched_setparam, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER_PTR);
      return(MAX_MQX_UINT);
   } /* Endif */
   
   /* Check for a RR interval that may wrap! */
   if (param_ptr->ROUND_ROBIN_INTERVAL != SCHED_NOT_CHANGED) {
      if ((param_ptr->ROUND_ROBIN_INTERVAL + 1000) < 
         param_ptr->ROUND_ROBIN_INTERVAL)
      {
         /* new time slice is too big */
         _task_set_error(MQX_SCHED_INVALID_PARAMETER);
         _KLOGX3(KLOG_sched_setparam, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER);
         return(MAX_MQX_UINT);
      } /* Endif */
   } /* Endif */
#endif
   
   /* Handle default case */
   if (task_id == MQX_DEFAULT_TASK_ID) {
#if MQX_HAS_TIME_SLICE
      if (param_ptr->ROUND_ROBIN_INTERVAL != SCHED_NOT_CHANGED) {
         /* time.MILLISECONDS = param_ptr->ROUND_ROBIN_INTERVAL;      */
         /* PSP_TIME_TO_TICKS(&time, &kernel_data->SCHED_TIME_SLICE); */
         PSP_MILLISECONDS_TO_TICKS_QUICK(param_ptr->ROUND_ROBIN_INTERVAL,
            &kernel_data->SCHED_TIME_SLICE);
      } /* Endif */
      _KLOGX3(KLOG_sched_setparam, MQX_OK, (_mqx_uint)0L);
      return(MQX_OK);
#else
      _task_set_error(MQX_SCHED_INVALID_PARAMETER);
      _KLOGX3(KLOG_sched_setparam, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER);
      return(MAX_MQX_UINT);
#endif

   } /* Endif */
   
   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if (td_ptr == NULL) {
      _task_set_error(MQX_SCHED_INVALID_TASK_ID);
      _KLOGX3(KLOG_sched_setparam, MAX_MQX_UINT, MQX_SCHED_INVALID_TASK_ID);
      return(MAX_MQX_UINT);
   } /* Endif */

   if ((param_ptr->PRIORITY != SCHED_NOT_CHANGED) &&
      (param_ptr->PRIORITY > kernel_data->LOWEST_TASK_PRIORITY))
   {
      _task_set_error(MQX_SCHED_INVALID_PARAMETER);
      _KLOGX3(KLOG_sched_setparam, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER);
      return(MAX_MQX_UINT);
   } /* Endif */

   _sched_set_param_internal(td_ptr, param_ptr);

   _KLOGX3(KLOG_sched_setparam, MQX_OK, (_mqx_uint)0L);
   return(MQX_OK);
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_setparam_td
* Returned Value   : _mqx_uint MQX_OK or MAX_MQX_UINT
*                     If MAX_MQX_UINT then the task error code is set to one of:
*                     SCHED_INVALID_PARAMETER_PTR, SCHED_INVALID_PARAMETER
*                     SCHED_INVALID_TASK_ID
* Comments         :
*   This function sets the current tasking parameters for the specified
* task.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_setparam_td
   (
      /* [IN] the task to apply this policy to:
      ** This must be a correct Task Descriptor address
      */
      pointer input_td_ptr,

      /* [IN] the address of a parameter structure used
      **    to define other tasking parameters (task priority level,
      **    and round robin time slice size)
      */
      SCHED_PARAM_STRUCT_PTR param_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE6(KLOG_sched_setparam_td, input_td_ptr, param_ptr, param_ptr->PRIORITY, param_ptr->ROUND_ROBIN_INTERVAL, param_ptr->POLICY);

   td_ptr = (TD_STRUCT_PTR)input_td_ptr;

#if MQX_CHECK_ERRORS
   /* Validate parameters */
   if (param_ptr == NULL) {
      _task_set_error(MQX_SCHED_INVALID_PARAMETER_PTR);
      _KLOGX3(KLOG_sched_setparam_td, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER_PTR);
      return(MAX_MQX_UINT);
   } /* Endif */
   
   /* Check for a RR interval that may wrap! */
   if (param_ptr->ROUND_ROBIN_INTERVAL != SCHED_NOT_CHANGED) {
      if ((param_ptr->ROUND_ROBIN_INTERVAL + 1000) < 
         param_ptr->ROUND_ROBIN_INTERVAL)
      {
         /* New time slice is too big */
         _task_set_error(MQX_SCHED_INVALID_PARAMETER);
         _KLOGX3(KLOG_sched_setparam_td, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER);
         return(MAX_MQX_UINT);
      } /* Endif */
   } /* Endif */
#endif
   
#if MQX_CHECK_ERRORS
   if ((param_ptr->PRIORITY != SCHED_NOT_CHANGED) &&
     (param_ptr->PRIORITY > kernel_data->LOWEST_TASK_PRIORITY))
   {
      _task_set_error(MQX_SCHED_INVALID_PARAMETER);
      _KLOGX3(KLOG_sched_setparam_td, MAX_MQX_UINT, MQX_SCHED_INVALID_PARAMETER);
      return(MAX_MQX_UINT);
   } /* Endif */
#endif

   _sched_set_param_internal(td_ptr, param_ptr);

   _KLOGX3(KLOG_sched_setparam_td, MQX_OK, 0L);
   return(MQX_OK);
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_getparam
* Returned Value   : MQX_OK or MAX_MQX_UINT
*                     IF MAX_MQX_UINT, the task error code is set to one of:
*                     SCHED_INVALID_PARAMETER_PTR, SCHED_INVALID_TASK_ID,
* Comments         :
*   This function returns the current scheduling parameters for the
* specified task.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_getparam
   (
      /* [IN] the task id to apply this policy to:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id               task_id,

      /* [IN] the address of a parameter structure where
      **    the tasking parameters (task priority level,
      **    and round robin time slice size) are to be written
      */
      SCHED_PARAM_STRUCT_PTR param_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
            TD_STRUCT_PTR           td_ptr;
            TIME_STRUCT             time;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   /* Validate parameters */
   if ( param_ptr == NULL ) {
      _task_set_error( MQX_SCHED_INVALID_PARAMETER_PTR );
      return( MAX_MQX_UINT );
   } /* Endif */
#endif
   
   /* Handle default case */
   if ( task_id == MQX_DEFAULT_TASK_ID ) {
#if MQX_HAS_TIME_SLICE
      PSP_TICKS_TO_TIME(&kernel_data->SCHED_TIME_SLICE, &time);
      param_ptr->ROUND_ROBIN_INTERVAL = time.MILLISECONDS + 1000 * time.SECONDS;
      param_ptr->POLICY               = kernel_data->SCHED_POLICY;
#else
      param_ptr->ROUND_ROBIN_INTERVAL = 0;
      param_ptr->POLICY               = MQX_SCHED_FIFO;
#endif

      param_ptr->PRIORITY             = 0;
      return( MQX_OK );
   } /* Endif */

   /* Handle the current task */
   td_ptr = (TD_STRUCT_PTR)_task_get_td( task_id );
   if ( td_ptr == NULL ) {
      _task_set_error( MQX_SCHED_INVALID_TASK_ID );
      return( MAX_MQX_UINT );
   } /* Endif */
   
   param_ptr->PRIORITY             = td_ptr->MY_QUEUE->PRIORITY;

#if MQX_HAS_TIME_SLICE
   PSP_TICKS_TO_TIME(&td_ptr->TIME_SLICE, &time);
   param_ptr->ROUND_ROBIN_INTERVAL = time.MILLISECONDS + 1000 * time.SECONDS;
   param_ptr->POLICY               =
      (td_ptr->FLAGS & MQX_TIME_SLICE_TASK ? MQX_SCHED_RR : MQX_SCHED_FIFO);
#else
   param_ptr->ROUND_ROBIN_INTERVAL = 0;
   param_ptr->POLICY               = MQX_SCHED_FIFO;
#endif

   return( MQX_OK );
   
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_setscheduler
* Returned Value   : _mqx_uint MQX_OK,  MAX_MQX_UINT
*                    if MAX_MQX_UINT, the task error code is set to one of:
*                       MQX_SCHED_INVALID_POLICY, MQX_SCHED_INVALID_PARAMETER_PTR
*                       MQX_SCHED_INVALID_PARAMETER, MQX_SCHED_INVALID_TASK_ID
* Comments         :
*   This function sets the scheduler policy.  This allows for changing the
* task priority, changing the scheduling policy between FIFO and Round Robin,
* and setting the round robin time slice time.  This may be set for the
* currently running task, another task on this processor, or the default
* values assigned to each new task created.
*   Values of SCHED_NOT_CHANGED in the parameter fields,
* means to ignore any assignments for that field.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_setscheduler
   (
      /* [IN] the task id to apply this policy to:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id task_id,

      /* [IN] what scheduling policy to apply:
      **    MQX_SCHED_RR or MQX_SCHED_FIFO(the default)
      */
      _mqx_uint  policy,

      /* [IN] the address of a parameter structure used
      **    to define other tasking parameters (task priority level,
      **    and round robin time slice size)
      */
      SCHED_PARAM_STRUCT_PTR param_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td_ptr;
            _mqx_uint                result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE6(KLOG_sched_setscheduler, task_id, policy, param_ptr, param_ptr->PRIORITY, param_ptr->ROUND_ROBIN_INTERVAL);

   /* Validate policy parameters */
   if (policy != SCHED_NOT_CHANGED) {

#if MQX_CHECK_ERRORS
#if MQX_HAS_TIME_SLICE
      if (! ((policy == MQX_SCHED_FIFO) || (policy == MQX_SCHED_RR))) {
#else
      if (policy != MQX_SCHED_FIFO) {
#endif
         _task_set_error(MQX_SCHED_INVALID_POLICY);
         _KLOGX3(KLOG_sched_setscheduler, MAX_MQX_UINT, MQX_SCHED_INVALID_POLICY);
         return(MAX_MQX_UINT);
      } /* Endif */
#endif

#if MQX_HAS_TIME_SLICE
      /* Handle default policy case */
      if (task_id == MQX_DEFAULT_TASK_ID) {
         kernel_data->SCHED_POLICY = policy;
      } /* Endif */
#endif
   } /* Endif */

   result = _sched_setparam(task_id, param_ptr);
   if (result != MQX_OK) {
      _KLOGX3(KLOG_sched_setscheduler, MAX_MQX_UINT, 0);
      return(result);
   }

   /* Set the scheduler policy of the specified task. */
   if ((policy != SCHED_NOT_CHANGED) &&
      (task_id != MQX_DEFAULT_TASK_ID))
   {
      td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
      if (td_ptr == NULL) {
         _task_set_error(MQX_SCHED_INVALID_TASK_ID);
         _KLOGX3(KLOG_sched_setscheduler, MAX_MQX_UINT, MQX_SCHED_INVALID_TASK_ID);
         return(MAX_MQX_UINT);
      } /* Endif */

#if MQX_HAS_TIME_SLICE
      if (policy == MQX_SCHED_RR) {
         _int_disable();
         td_ptr->FLAGS |= MQX_TIME_SLICE_TASK;
         _int_enable();
      } else if (policy == MQX_SCHED_FIFO) {
         _int_disable();
         td_ptr->FLAGS &= ~MQX_TIME_SLICE_TASK;
         _int_enable();
      } /* Endif */
#endif

   } /* Endif */

   _KLOGX3(KLOG_sched_setscheduler, MQX_OK, 0);

   return(MQX_OK);
   
} /* Endbody */

#endif
/* EOF */
