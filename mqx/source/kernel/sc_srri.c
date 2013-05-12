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
* $FileName: sc_srri.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the functions for setting the 
*   scheduling rr_interval of a task.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TIME_SLICE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_set_rr_interval_internal
* Returned Value   : old_scheduling rr_interval in ticks
* Comments         :
*   This function sets various the scheduling rr_interval 
*   for a task or the system
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_set_rr_interval_internal
   (
      /* [IN] the task whose rr_interval is to change:
      ** NULL_TASK_ID => the current task
      ** DEFAULT_TASK_ID => the kernel defaults for task creation
      ** any other    => the specified task
      */
      _task_id             task_id,

      /* [IN]  the new scheduling rr_interval (ticks) */
      MQX_TICK_STRUCT_PTR  new_rr_tick_ptr,

      /* [OUT] the old scheduling rr_interval (ticks) */
      MQX_TICK_STRUCT_PTR  old_rr_tick_ptr

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   /* Handle default case */
   if (task_id == MQX_DEFAULT_TASK_ID) {
      *old_rr_tick_ptr = kernel_data->SCHED_TIME_SLICE;
      _int_disable();
      kernel_data->SCHED_TIME_SLICE = *new_rr_tick_ptr;
      _int_enable();
   } else {
      td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
      if (td_ptr == NULL) {
         return(MQX_SCHED_INVALID_TASK_ID);
      } /* Endif */
      *old_rr_tick_ptr = td_ptr->TIME_SLICE;
      _int_disable();
      td_ptr->TIME_SLICE = *new_rr_tick_ptr;
      _int_enable();
   } /* Endif */

   return(MQX_OK);

} /* Endbody */

#endif

/* EOF */
