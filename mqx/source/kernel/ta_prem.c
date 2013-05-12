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
* $FileName: ta_prem.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for stopping and re-starting
*   task pre-emption.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_stop_preemption
* Comments         :
*    This function disables preemption of the currently running task
*    unless it blocks, or calls _task_start_preemption.
*    Note that interrupts will still be handled
*
*END*----------------------------------------------------------------------*/

void _task_stop_preemption
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
            TD_STRUCT_PTR td_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE1(KLOG_task_stop_preemption);

   td_ptr      = kernel_data->ACTIVE_PTR;
   _int_disable();
   td_ptr->FLAGS |= TASK_PREEMPTION_DISABLED;
   _int_enable();
   _KLOGX1(KLOG_task_stop_preemption);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_start_preemption
* Comments         :
*    This function restores the ability of this task to be preempted.
*
*END*----------------------------------------------------------------------*/

void _task_start_preemption
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
            TD_STRUCT_PTR td_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE1(KLOG_task_start_preemption);

   td_ptr      = kernel_data->ACTIVE_PTR;
   _int_disable();
   td_ptr->FLAGS &= ~TASK_PREEMPTION_DISABLED;
   _CHECK_RUN_SCHEDULER(); /* Allow higher priority tasks to run */
   _int_enable();

   _KLOGX1(KLOG_task_start_preemption);

} /* Endbody */

/* EOF */
