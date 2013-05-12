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
* $FileName: ta_fp.c$
* $Version : 3.5.5.0$
* $Date    : Jan-22-2010$
*
* Comments:
*
*   This file contains the functions for modifying the floating point
*   attribute of a task.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQXCFG_ENABLE_FP && PSP_HAS_FPU

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_enable_fp
* Comments         :
*    This function enables floating point context switching for the current
*    task.
*
*END*----------------------------------------------------------------------*/

void _task_enable_fp
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
            TD_STRUCT_PTR td_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE1(KLOG_task_enable_fp);

   td_ptr      = kernel_data->ACTIVE_PTR;
   _int_disable();
   td_ptr->FLAGS |= MQX_FLOATING_POINT_TASK;
   if (kernel_data->FP_ACTIVE_PTR != NULL) {
      if (kernel_data->FP_ACTIVE_PTR != kernel_data->ACTIVE_PTR) {
         _psp_save_fp_context_internal();
      }/* Endif */
   }/* Endif */
   kernel_data->FP_ACTIVE_PTR = kernel_data->ACTIVE_PTR;
   _int_enable();
   _KLOGX1(KLOG_task_enable_fp);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_disable_fp
* Comments         :
*    This function disables floating point context switching for the current
*    task.
*
*END*----------------------------------------------------------------------*/

void _task_disable_fp
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
            TD_STRUCT_PTR td_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE1(KLOG_task_disable_fp);

   td_ptr = kernel_data->ACTIVE_PTR;
   _int_disable();
   if (td_ptr->FLAGS & MQX_FLOATING_POINT_TASK)  {
      td_ptr->FLAGS &= ~MQX_FLOATING_POINT_TASK;
      kernel_data->FP_ACTIVE_PTR = NULL;
   } /* Endif */
   _int_enable();
   _KLOGX1(KLOG_task_disable_fp);

} /* Endbody */
#endif

/* EOF */
