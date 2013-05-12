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
* $FileName: ta_env.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for accessing the task environment.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TASK_ENVIRONMENT

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _task_set_environment
* Returned Value   : pointer
* Comments         :
*   The function sets the address of the environment data for this task
*
*END*-------------------------------------------------------------------------*/

pointer _task_set_environment
   ( 
     /* [IN] the id of the task whose environment is to be set */
     _task_id  task_id, 
     
     /* [IN] the value to set the task environment to */
     pointer   environment_ptr
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR      kernel_data;)
   TD_STRUCT_PTR               td_ptr;
   pointer                     old_env_ptr;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE3(KLOG_task_set_environment, task_id, environment_ptr);

   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if (td_ptr == NULL) {
      _task_set_error(MQX_INVALID_TASK_ID);
      _KLOGX2(KLOG_task_set_environment, NULL);
      return( NULL );
   }/* Endif */

   old_env_ptr = td_ptr->ENVIRONMENT_PTR;
   td_ptr->ENVIRONMENT_PTR = environment_ptr;

   _KLOGX2(KLOG_task_set_environment, old_env_ptr);
   return(old_env_ptr);
   
} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _task_get_environment
* Returned Value   : pointer
* Comments         :
*   The function returns the address of the environment data for this task
*
*END*-------------------------------------------------------------------------*/

pointer _task_get_environment
   (
     /* [IN] the id of the task whose environment is to be returned */
      _task_id  task_id
   )
{ /* Body */
   TD_STRUCT_PTR          td_ptr;

   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if ( td_ptr == NULL ) {
      _task_set_error(MQX_INVALID_TASK_ID);
      return( NULL );
   }/* Endif */

   return(td_ptr->ENVIRONMENT_PTR);
   
} /* Endbody */
#endif

/* EOF */
