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
* $FileName: na_comp.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating the Name component.
*   This component can be installed as a generic name
*   service for user tasks, associating a string name with a  _mqx_max_type
*   quantity.  The component is also used by other kernel components to
*   manage names.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_create_component
* Returned Value   : _mqx_uint MQX error code
* Comments         :
*   This function creates the name component.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_create_component
   (
      /* [IN] the initial number of names that can be stored  */
      _mqx_uint initial_number,

      /* [IN] the number of names to be added when table full */
      _mqx_uint grow_number,

      /* [IN] the maximum number of names that can be stored  */
      _mqx_uint maximum_number
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   _mqx_uint                 result;
   pointer                   handle;
   NAME_COMPONENT_STRUCT_PTR component_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_name_create_component, initial_number, grow_number, maximum_number);

   component_ptr = (NAME_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_NAME_MANAGEMENT];
   if (component_ptr != NULL){
      if (maximum_number > component_ptr->MAX_NUMBER) {
         component_ptr->MAX_NUMBER = maximum_number;
      } /* Endif */
      _KLOGX2(KLOG_name_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   result = _name_create_handle_internal(&handle,
      initial_number, grow_number, maximum_number, initial_number);

   /* We must exclude all ISRs at this point */
   if (result == MQX_OK) {
      _int_disable();
      if (kernel_data->KERNEL_COMPONENTS[KERNEL_NAME_MANAGEMENT] != NULL){
         _int_enable();
         _name_destroy_handle_internal(handle);
         component_ptr = (NAME_COMPONENT_STRUCT_PTR)
            kernel_data->KERNEL_COMPONENTS[KERNEL_NAME_MANAGEMENT];
         if (maximum_number > component_ptr->MAX_NUMBER) {
            component_ptr->MAX_NUMBER = maximum_number;
         } /* Endif */
         _KLOGX2(KLOG_name_create_component, MQX_OK);
         return(MQX_OK);
      } /* Endif */
      kernel_data->KERNEL_COMPONENTS[KERNEL_NAME_MANAGEMENT] = handle;
      _int_enable();
   } /* Endif */

   _KLOGX2(KLOG_name_create_component, result);
   return(result);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
