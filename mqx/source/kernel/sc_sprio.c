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
* $FileName: sc_sprio.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   task priority of a task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_set_priority_internal
* Returned Value   : none
* Comments         :
*   This function puts the specified td onto the specified priority queue.
*   This function MUST BE CALLED DISABLED 
*
*END*----------------------------------------------------------------------*/

void _sched_set_priority_internal
   (
      /* [IN] the address of the task descriptor whose priority is to change */
      register TD_STRUCT_PTR          td_ptr,

      /* [IN] the new task priority,  It MUST be a valid priority */
      register _mqx_uint               new_priority
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register READY_Q_STRUCT_PTR      ready_q_ptr;
   register _mqx_uint                old_priority;

   _GET_KERNEL_DATA(kernel_data);

   /* Find the new queue */
   ready_q_ptr  = kernel_data->READY_Q_LIST;
   old_priority = td_ptr->MY_QUEUE->PRIORITY;

   ready_q_ptr  = ready_q_ptr - new_priority;

   /*
   ** Remove the current task from the head of the queue, and
   ** place the task at the back of the specified queue.
   */
   td_ptr->TASK_SR  = ready_q_ptr->ENABLE_SR;
   td_ptr->MY_QUEUE = ready_q_ptr;
   if (td_ptr->STATE == READY) {
      _QUEUE_UNLINK(td_ptr);
      _TASK_READY(td_ptr,kernel_data);
      if (kernel_data->ACTIVE_PTR == td_ptr) {
         /* Changing the current task's priority */
         kernel_data->ACTIVE_SR = td_ptr->TASK_SR;
         if (new_priority > old_priority) {
            /* 
            ** Save state, and re-run the scheduler 
            ** since the actice task's priority has been lowered, 
            ** and now a higher priority task may be ready to run
            */
            _sched_execute_scheduler_internal();
         } /* Endif */
      } else {
         /* May need to mod sr value of task on stack */
         if (td_ptr->DISABLED_LEVEL == 0) {
            _task_sync_priority_internal(td_ptr);
         } /* Endif */
      } /* Endif */   
   } else {
      /* May need to mod sr value of task on stack */
      if (td_ptr->DISABLED_LEVEL == 0) {
         _task_sync_priority_internal(td_ptr);
      } /* Endif */
   } /* Endif */
      
} /* Endbody */

/* EOF */
