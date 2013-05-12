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
* $FileName: ta_util.c$
* $Version : 3.8.10.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file contains utility functions dealing with tasks.
*
*END************************************************************************/

#include <string.h>
#include "mqx_inc.h"

#if MQX_TD_HAS_TASK_TEMPLATE_PTR
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_id_from_name
* Returned Value   : _task_id (or NULL_TASK_ID if not found)
* Comments         :
*    This function uses a task name (from its task template)
*    to find a task id.  Only the first task found with
*    the provided name is found.
*
*END*----------------------------------------------------------------------*/

_task_id _task_get_id_from_name
   (
      /* [IN] the name to look up */
      char_ptr name_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   TD_STRUCT_PTR           td_ptr;
   _mqx_uint                size;

   _GET_KERNEL_DATA(kernel_data);
   
   _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   td_ptr = (TD_STRUCT_PTR)((uchar_ptr)kernel_data->TD_LIST.NEXT -
      FIELD_OFFSET(TD_STRUCT,TD_LIST_INFO));
   size   = _QUEUE_GET_SIZE(&kernel_data->TD_LIST);

   while (size && td_ptr) {
      if (strncmp( td_ptr->TASK_TEMPLATE_PTR->TASK_NAME, name_ptr, 
         MQX_MAX_TASK_NAME_SIZE) == 0) 
      {
         _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);
         return td_ptr->TASK_ID;
      } /* Endif */
      size--;
      td_ptr = (TD_STRUCT_PTR)((uchar_ptr)(td_ptr->TD_LIST_INFO.NEXT) -
         FIELD_OFFSET(TD_STRUCT,TD_LIST_INFO));
   } /* Endwhile */
   
   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   return MQX_NULL_TASK_ID;

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_template_ptr
* Returned Value   : TASK_TEMPLATE_STRUCT_PTR pointer to a task's template
* Comments         :
*    This function obtains a task template from the task id provided. 
*    It returns NULL if an invalid task_id is presented.
*
*END*----------------------------------------------------------------------*/

TASK_TEMPLATE_STRUCT_PTR  _task_get_template_ptr
   (
      /* [IN] the task id */
      _task_id  task_id
   )
{ /* Body */
   TD_STRUCT_PTR  td_ptr;

   td_ptr = _task_get_td(task_id);

   return (td_ptr ? td_ptr->TASK_TEMPLATE_PTR : NULL);

} /* Endbody */

#endif

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_template_index
* Returned Value   : _mqx_uint template number (0 if not found)
* Comments         :
*    This function obtains a task template index (from its task template)
*    using the string name of the task provided
*
*END*----------------------------------------------------------------------*/

_mqx_uint  _task_get_template_index
   (
      /* [IN] the name to look up */
      char_ptr name_ptr 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   TASK_TEMPLATE_STRUCT_PTR task_template_ptr;

   _GET_KERNEL_DATA(kernel_data);
   
   _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   /** Search all the local task templates for one whose index matches */
   task_template_ptr = kernel_data->INIT.TASK_TEMPLATE_LIST;
   while (task_template_ptr->TASK_TEMPLATE_INDEX){
      if (task_template_ptr->TASK_NAME != NULL)  {
         if (strncmp( task_template_ptr->TASK_NAME, name_ptr, 
            MQX_MAX_TASK_NAME_SIZE) == 0) 
         {
            break;
         } /* Endif */
      } /* Endif */
      ++task_template_ptr;
   } /* Endwhile */
   
   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->TASK_CREATE_LWSEM);

   return task_template_ptr->TASK_TEMPLATE_INDEX;

} /* Endwhile */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_id_from_td
* Returned Value   : _task_id (or NULL_TASK_ID if error)
* Comments         :
*    This function uses a task descriptor pointer
*    to return the task's id.  
*
*END*----------------------------------------------------------------------*/

_task_id _task_get_id_from_td
   (
      pointer td_ptr
   )
{
   if (NULL == td_ptr) 
   {
      return MQX_NULL_TASK_ID;
   }
   return ((TD_STRUCT_PTR)td_ptr)->TASK_ID;
}
/* EOF */
