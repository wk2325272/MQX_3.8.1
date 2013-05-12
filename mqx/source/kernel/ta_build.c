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
* $FileName: ta_build.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for creating a task, but not letting
*   it run.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_build_internal
* Returned Value   : TD_STRUCT_PTR the td for the created task.
* Comments         :
*    This function will create a new task of the type specified by the
*    task template number, but not make it ready to run.
*
*END*----------------------------------------------------------------------*/

TD_STRUCT_PTR _task_build_internal
    (  
        /* [IN] the task template index number for this task */
        _mqx_uint     template_index,
      
        /* [IN] the parameter to pass to the newly created task */
        uint_32       parameter,

        /* [IN] if not NULL, the location of the stack is provided */
        pointer       stack_ptr,

        /* [IN] the stack size if provided by the application */
        _mqx_uint     stack_size,

        /* usermode functionality */
        boolean user
    )
{
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   TD_STRUCT_PTR            td_ptr;
   TASK_TEMPLATE_STRUCT_PTR task_template_ptr;
   boolean                  dynamic;
   uint_32                  create_parameter;
   
   _GET_KERNEL_DATA(kernel_data);

   if ( template_index ) {
      dynamic = FALSE;
      create_parameter = parameter;
         
      /* Search all the local task templates for one whose index matches */
      task_template_ptr = kernel_data->INIT.TASK_TEMPLATE_LIST;
      while ( task_template_ptr->TASK_TEMPLATE_INDEX &&
         (task_template_ptr->TASK_TEMPLATE_INDEX != template_index) )
      {
         ++task_template_ptr;
      } /* Endwhile */
      if (task_template_ptr->TASK_TEMPLATE_INDEX == 0) {
         /* Task not found */
         task_template_ptr = NULL;
      } /* Endif */
   } else {
      task_template_ptr = (TASK_TEMPLATE_STRUCT_PTR)parameter;
      create_parameter  = task_template_ptr->CREATION_PARAMETER;
      dynamic = TRUE;
   } /* Endif */

#if MQX_CHECK_ERRORS
   if (task_template_ptr == NULL) {
      _task_set_error(MQX_NO_TASK_TEMPLATE);
      return NULL;
   } /* Endif */
#endif
   

#if MQX_ENABLE_USER_MODE
    // check template for correct user task parameters
    if (user) {
        if (!(task_template_ptr->TASK_ATTRIBUTES & MQX_USER_TASK)) {
            _task_set_error(MQX_INVALID_PARAMETER);
            return NULL;
        }
        
        if (kernel_data->INIT.MAX_USER_TASK_PRIORITY && kernel_data->INIT.MAX_USER_TASK_PRIORITY > task_template_ptr->TASK_PRIORITY) {
            _task_set_error(MQX_INVALID_TASK_PRIORITY);
            return NULL;
        }
    }
#endif

   /* serialize task creation/destruction */
   _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   /* Create the task, but do not ready it */
/* START CR 897 */
   td_ptr = _task_init_internal(task_template_ptr, kernel_data->ACTIVE_PTR->TASK_ID, \
       create_parameter, dynamic, stack_ptr, stack_size);
/* END CR 897 */

   /* Allow other tasks to create */
   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   return(td_ptr);
      
} /* Endbody */

/* EOF */
