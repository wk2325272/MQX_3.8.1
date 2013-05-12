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
* $FileName: tq_util.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for returning the number
*   of tasks on a task queue.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _taskq_get_value
* Returned Value   : size of task queue, or MAX_MQX_UINT on error
* Comments         :
*   This function returns the size of the task queue.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _taskq_get_value
   (
      /* [IN] the task queue handle */
      pointer users_task_queue_ptr
   )
{ /* Body */
   register TASK_QUEUE_STRUCT_PTR  task_queue_ptr = 
      (TASK_QUEUE_STRUCT_PTR)users_task_queue_ptr;

#if MQX_CHECK_ERRORS
   if (task_queue_ptr == NULL)  {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(MAX_MQX_UINT);
   } /* Endif */
#endif

#if MQX_CHECK_VALIDITY   
   if (task_queue_ptr->VALID != TASK_QUEUE_VALID){
      _task_set_error(MQX_INVALID_TASK_QUEUE);
      return(MAX_MQX_UINT);
   } /* Endif */
#endif

   return(task_queue_ptr->TD_QUEUE.SIZE);
   
} /* Endbody */

/* EOF */
