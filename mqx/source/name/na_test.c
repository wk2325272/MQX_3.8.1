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
* $FileName: na_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing the name component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_test
* Returned Value   : an MQX Error Code
* Comments         :
*   This function tests the name component for consistency and validity.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_test
   (
      /* [OUT] the address of the base name component in error */
      pointer _PTR_ name_error_ptr,

      /* [OUT] the address of the name component extension in error */
      pointer _PTR_ name_extension_error_ptr
   )
{ /* Body */              
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   NAME_COMPONENT_STRUCT_PTR name_component_ptr;
   _mqx_uint                 result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_name_test, name_error_ptr, name_extension_error_ptr);

   *name_error_ptr = NULL;
   *name_extension_error_ptr = NULL;

   name_component_ptr = (NAME_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_NAME_MANAGEMENT];
   if (name_component_ptr == NULL) {
      _KLOGX2(KLOG_name_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   result = _name_test_internal(
      name_component_ptr,
      name_error_ptr, name_extension_error_ptr);

   _KLOGX2(KLOG_name_test, result);
   return(result);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
