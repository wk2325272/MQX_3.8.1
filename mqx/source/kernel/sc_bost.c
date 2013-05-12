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
* $FileName: sc_bost.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for use with priority
*   boosting and unboosting of tasks.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_DYNAMIC_PRIORITIES
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_boost_priority_internal
* Returned Value   : none
* Comments         :
*   This function boosts a task to a temporarily higher priority
*
*   This function MUST BE CALLED DISABLED
*
*END*----------------------------------------------------------------------*/

void _sched_boost_priority_internal
   (
      /* [IN] the address of the task descriptor whose priority is to change */
      register TD_STRUCT_PTR td_ptr,

      /* [IN] the new task priority,  It MUST be a valid priority */
      register _mqx_uint      priority
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_sched_boost_priority, td_ptr, priority, td_ptr->MY_QUEUE->PRIORITY);

   td_ptr->FLAGS |= TASK_PRIORITY_BOOSTED;
   ++td_ptr->BOOSTED;

   _sched_set_priority_internal(td_ptr, priority);

   /* Allow higher priority tasks to run */
   _CHECK_RUN_SCHEDULER(); 

   _KLOGX1(KLOG_sched_boost_priority);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_unboost_priority_internal
* Returned Value   : none
* Comments         :
*   This function returns a task from a temporarily higher priority
*
*   This function MUST BE CALLED DISABLED
*
*END*----------------------------------------------------------------------*/

void _sched_unboost_priority_internal
   (
      /* [IN] the address of the task descriptor whose priority is to change */
      register TD_STRUCT_PTR td_ptr,

      /* [IN] the number of times to 'unboost' the task */
      register _mqx_uint      number_of_boosts
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE5(KLOG_sched_unboost_priority, td_ptr, number_of_boosts, td_ptr->BOOSTED, td_ptr->HOME_QUEUE->PRIORITY);

   if (td_ptr->FLAGS & TASK_PRIORITY_BOOSTED) {
      if (td_ptr->BOOSTED > number_of_boosts) {
         td_ptr->BOOSTED = td_ptr->BOOSTED - number_of_boosts;
      } else {
         td_ptr->BOOSTED = 0;
         td_ptr->FLAGS &= ~TASK_PRIORITY_BOOSTED;
         _sched_set_priority_internal(td_ptr, (_mqx_uint)td_ptr->HOME_QUEUE->PRIORITY);
         /* Allow higher priority tasks to run */
         _CHECK_RUN_SCHEDULER(); 
      } /* Endif */
   } /* Endif */
   
   _KLOGX1(KLOG_sched_unboost_priority);

} /* Endbody */
#endif

/* EOF */
