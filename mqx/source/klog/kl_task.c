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
* $FileName: kl_task.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for enabling and disabling
*   kernel logging for a particular task.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_KERNEL_LOGGING
#include "lwlog.h"
#include "klog.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _klog_disable_logging_task
* Returned Value   : none
* Comments         :
*   This function enables logging for the specified task
*
*END*----------------------------------------------------------------------*/

void _klog_disable_logging_task
   (
      /* [IN] the task which is to have kernel logging disabled */
      _task_id tid
   )
{ /* Body */
   TD_STRUCT_PTR          td_ptr;

   _int_disable();
   td_ptr = (TD_STRUCT_PTR)_task_get_td(tid);
   if (td_ptr != NULL) {
      td_ptr->FLAGS &= ~TASK_LOGGING_ENABLED;
   } /* Endif */
   _int_enable();

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _klog_enable_logging_task
* Returned Value   : none
* Comments         :
*   This function disables logging for the specified task
*
*END*----------------------------------------------------------------------*/

void _klog_enable_logging_task
   (
      /* [IN] the task which is to have kernel logging enabled */
      _task_id tid
   )
{ /* Body */
   TD_STRUCT_PTR          td_ptr;

   _int_disable();
   td_ptr = (TD_STRUCT_PTR)_task_get_td(tid);
   if (td_ptr != NULL) {
      td_ptr->FLAGS |= TASK_LOGGING_ENABLED;
   } /* Endif */
   _int_enable();

} /* Endbody */
#endif /* MQX_KERNEL_LOGGING */

/* EOF */
