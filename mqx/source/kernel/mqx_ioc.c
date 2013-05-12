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
* $FileName: mqx_ioc.c$
* $Version : 3.5.6.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the support functions for the MQX I/O
*   components.
*
*END************************************************************************/


#include "mqx_inc.h"
#if MQX_USE_IO_COMPONENTS


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_get_io_component_handle
* Returned Value   : pointer handle
* Comments         : 
*    This function returns the I/O Component handle for the specified
*    I/O component
*
*END*----------------------------------------------------------------------*/

pointer _mqx_get_io_component_handle
   (
      /* [IN] the component number */
      _mqx_uint component
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;

   _GET_KERNEL_DATA(kernel_data);
 
#if MQX_CHECK_ERRORS
   if (component >= MAX_IO_COMPONENTS) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(NULL);
   } /* Endif */
#endif

   return kernel_data->IO_COMPONENTS[component];

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_set_io_component_handle
* Returned Value   : pointer handle
* Comments         : 
*    This function sets the I/O Component handle for the specified
*    I/O component, and returns the previous value.
*
*END*----------------------------------------------------------------------*/

pointer _mqx_set_io_component_handle
   (
      /* [IN] the component number */
      _mqx_uint component,

      /* [IN] the new handle */
      pointer handle
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   pointer  old_handle;

   _GET_KERNEL_DATA(kernel_data);
 
#if MQX_CHECK_ERRORS
   if (component >= MAX_IO_COMPONENTS) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(NULL);
   } /* Endif */
#endif

   _int_disable();
   old_handle = kernel_data->IO_COMPONENTS[component];
   if (old_handle == NULL) {
      kernel_data->IO_COMPONENTS[component] = handle;
   } else {
      if (handle == NULL) {
         kernel_data->IO_COMPONENTS[component] = handle;
      } /* Endif */
   } /* Endif */
   _int_enable();
   return old_handle;

} /* Endbody */

#if MQX_IO_COMPONENT_DESTRUCTION
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_get_io_component_cleanup
* Returned Value   : pointer cleanup
* Comments         : 
*    This function returns the I/O Component cleanup for the specified
*    I/O component
*
*END*----------------------------------------------------------------------*/

void (_CODE_PTR_ _mqx_get_io_component_cleanup
   (
      /* [IN] the component number */
      _mqx_uint component
   ))(pointer)
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;

   _GET_KERNEL_DATA(kernel_data);
 
#if MQX_CHECK_ERRORS
   if (component >= MAX_IO_COMPONENTS) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(NULL);
   } /* Endif */
#endif

   return (void (_CODE_PTR_)(pointer))
      kernel_data->IO_COMPONENT_CLEANUP[component];

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_set_io_component_cleanup
* Returned Value   : pointer cleanup
* Comments         : 
*    This function sets the I/O Component cleanup for the specified
*    I/O component, and returns the previous value.
*
*END*----------------------------------------------------------------------*/

void (_CODE_PTR_ _mqx_set_io_component_cleanup
   (
      /* [IN] the component number */
      _mqx_uint component,

      /* [IN] the new cleanup */
      void (_CODE_PTR_ cleanup)(pointer)
   ))(pointer)
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   void (_CODE_PTR_ old_cleanup)(TD_STRUCT_PTR);

   _GET_KERNEL_DATA(kernel_data);
 
#if MQX_CHECK_ERRORS
   if (component >= MAX_IO_COMPONENTS) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(NULL);
   } /* Endif */
#endif

   _int_disable();
   old_cleanup = kernel_data->IO_COMPONENT_CLEANUP[component];
   kernel_data->IO_COMPONENT_CLEANUP[component] =
      (void (_CODE_PTR_)(TD_STRUCT_PTR))cleanup;
   _int_enable();
   return (void (_CODE_PTR_)(pointer))old_cleanup;

} /* Endbody */
#endif /* MQX_COMPONENT_DESTRUCTION */



/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_link_io_component_handle
* Returned Value   :  
* Comments         : 
*     
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mqx_link_io_component_handle
   (
      /* [IN] the component number */
      _mqx_uint component,

      /* [IN] the handle */
     pointer    handle,

      /* [IN] the pointer to the next field */
     pointer *  next_field_ptr)
{
   pointer                 old_handle;
   KERNEL_DATA_STRUCT_PTR  kernel_data;
#if MQX_CHECK_ERRORS
   pointer *               prev_ptr,next_ptr;
   uint_32                 offset = (uint_32) next_field_ptr - (uint_32) handle;
#endif

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if ((component >= MAX_IO_COMPONENTS) || (handle==NULL) || (next_field_ptr==NULL)) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   } /* Endif */
#endif

   _int_disable();
   
#if MQX_CHECK_ERRORS
   if (kernel_data->IO_COMPONENTS[component] == handle) {
       _int_enable();
      return MQX_INVALID_COMPONENT_HANDLE;
   } else {
      
      prev_ptr = kernel_data->IO_COMPONENTS[component];
   
      while (prev_ptr) {
         next_ptr = *((pointer *)((uint_32)prev_ptr + offset)); 
         if (next_ptr == handle) {
             _int_enable();
            return MQX_INVALID_COMPONENT_HANDLE;
         } else {
            prev_ptr  = next_ptr;   
         }
      }
   }

#endif


   old_handle = kernel_data->IO_COMPONENTS[component];
   kernel_data->IO_COMPONENTS[component] = handle;
   *next_field_ptr = old_handle;
   _int_enable();
   return MQX_OK;
}


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_unlink_io_component_handle
* Returned Value   :  
* Comments         : 
*     
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mqx_unlink_io_component_handle
   (
      /* [IN] the component number */
      _mqx_uint component,

      /* [IN] the handle */
     pointer    handle,

      /* [IN] the pointer to the next field */
     pointer *  next_field_ptr)
{
   uint_32                 offset = (uint_32) next_field_ptr - (uint_32) handle;
   pointer                 prev_ptr,next_ptr;
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   _mqx_uint               result = MQX_INVALID_COMPONENT_HANDLE;

   _GET_KERNEL_DATA(kernel_data);
   

#if MQX_CHECK_ERRORS
   if ((component >= MAX_IO_COMPONENTS) || (handle==NULL) || (next_field_ptr==NULL)) {
      _task_set_error(MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   } /* Endif */
#endif

   _int_disable();
   
   if (kernel_data->IO_COMPONENTS[component] == handle) {
      kernel_data->IO_COMPONENTS[component] = *((pointer *)((uint_32)handle + offset)); 
      *((pointer *) ((uint_32)handle + offset)) = NULL;
      result = MQX_OK; 
   } else {
      
      prev_ptr = kernel_data->IO_COMPONENTS[component];
   
      while (prev_ptr) {
         next_ptr = *((pointer *)((uint_32)prev_ptr + offset)); 
         if (next_ptr == handle) {
             *((pointer *) ((uint_32)prev_ptr + offset))  =  *((pointer *)((uint_32)next_ptr + offset)); 
              *((pointer *) ((uint_32)next_ptr + offset)) = NULL;
            result = MQX_OK; 
            break;
         } else {
            prev_ptr  = next_ptr;   
         }
      }
   }
   
      
   _int_enable();
   return result;
}
#endif

/* EOF */
