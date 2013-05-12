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
* $FileName: sc_spol.c$
* $Version : 3.0.6.0$
* $Date    : Apr-14-2009$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   scheduling policy of a task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_set_policy
* Returned Value   : old_scheduling policy
* Comments         :
*   This function sets various the scheduling policy for a task or the system
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_set_policy
   (
      /* [IN] the task whose policy is to change:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id task_id,

     /* [IN] the new scheduling policy
     */
      _mqx_uint policy
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   _mqx_uint               old_policy = MQX_SCHED_FIFO;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_sched_set_policy, (_mqx_uint)task_id, policy);
#if MQX_HAS_TIME_SLICE

#if MQX_CHECK_ERRORS
   if (! ((policy == MQX_SCHED_FIFO) || (policy == MQX_SCHED_RR))) {
      _task_set_error(MQX_SCHED_INVALID_POLICY);
      _KLOGX3(KLOG_sched_set_policy, MAX_MQX_UINT, MQX_SCHED_INVALID_POLICY);
      return(MAX_MQX_UINT);
   } /* Endif */
#endif
   
   /* Handle default case */
   if (task_id == MQX_DEFAULT_TASK_ID) {
      old_policy = kernel_data->SCHED_POLICY;
      kernel_data->SCHED_POLICY = policy;
   } else {
      td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
      if (td_ptr == NULL) {
         _task_set_error(MQX_SCHED_INVALID_TASK_ID);
         _KLOGX3(KLOG_sched_set_policy, MAX_MQX_UINT, MQX_SCHED_INVALID_TASK_ID);
         return(MAX_MQX_UINT);
      } /* Endif */
      if (td_ptr->FLAGS & MQX_TIME_SLICE_TASK) {
         old_policy = MQX_SCHED_RR;
      } else {
         old_policy = MQX_SCHED_FIFO;
      } /* Endif */
      _int_disable();
      if (policy == MQX_SCHED_RR) {
         td_ptr->FLAGS |= MQX_TIME_SLICE_TASK;
      } else {
         td_ptr->FLAGS &= ~MQX_TIME_SLICE_TASK;
      } /* Endif */
      _int_enable();
   } /* Endif */
#else

#if MQX_CHECK_ERRORS
   if (policy != MQX_SCHED_FIFO)  {
      _task_set_error(MQX_SCHED_INVALID_POLICY);
      _KLOGX3(KLOG_sched_set_policy, MAX_MQX_UINT, MQX_SCHED_INVALID_POLICY);
      return(MAX_MQX_UINT);
   } /* Endif */
   old_policy = MQX_SCHED_FIFO;
#endif

#endif

   _KLOGX3(KLOG_sched_set_policy, old_policy, 0L);
   return(old_policy);
      
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_get_policy
* Returned Value   : scheduling policy
* Comments         :
*   This function gets the scheduling policy for a task or the system
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_get_policy
   (
      /* [IN] the task whose policy is to be obtained
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id task_id,

      /* [IN] the location where to write the policy
      */
      _mqx_uint_ptr policy_ptr
   )
{ /* Body */
#if MQX_HAS_TIME_SLICE
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   _mqx_uint                old_policy;
   
   _GET_KERNEL_DATA(kernel_data);

   /* Handle default case */
   if (task_id == MQX_DEFAULT_TASK_ID) {
      old_policy = kernel_data->SCHED_POLICY;
   } else {
      td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
      if (td_ptr == NULL) {
         return(MQX_SCHED_INVALID_TASK_ID);
      } /* Endif */
      if (td_ptr->FLAGS & MQX_TIME_SLICE_TASK) {
         old_policy = MQX_SCHED_RR;
      } else {
         old_policy = MQX_SCHED_FIFO;
      } /* Endif */
   } /* Endif */

   *policy_ptr = old_policy;
#else
   *policy_ptr = MQX_SCHED_FIFO;
#endif

   return(MQX_OK);
      
} /* Endbody */

/* EOF */
