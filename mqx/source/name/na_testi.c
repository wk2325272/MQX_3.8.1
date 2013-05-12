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
* $FileName: na_testi.c$
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
* Function Name    : _name_test_internal
* Returned Value   : a MQX Error Code
* Comments         :
*   This function tests the specified name component for consistency.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_test_internal
   (
      /* [IN] the name component to test */
      NAME_COMPONENT_STRUCT_PTR base_name_manager_ptr,

      /* [OUT] the address of the base name component in error */
      pointer _PTR_             name_error_ptr,

      /* [OUT] the address of the name component extension in error */
      pointer _PTR_             name_extension_error_ptr
   )
{ /* Body */
   NAME_COMPONENT_STRUCT_PTR name_manager_ptr;
   pointer                   sem_in_err_ptr;
   pointer                   td_in_err_ptr;
   _mqx_uint                 result;
   _mqx_uint                 total;
   _mqx_uint                 i;

   if (base_name_manager_ptr == NULL) {
      return(MQX_OK);
   } /* Endif */

   *name_error_ptr = base_name_manager_ptr;
   if (base_name_manager_ptr->VALID != NAME_VALID) {
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */

   result = _lwsem_test(&sem_in_err_ptr, &td_in_err_ptr);
   if (result != MQX_OK) {
      return(result);
   } /* Endif */

   name_manager_ptr = base_name_manager_ptr;
   total = 0;
   _int_disable();
   while (name_manager_ptr != NULL) {
#if MQX_CHECK_VALIDITY
      if (name_manager_ptr->VALID != NAME_VALID) {
         _int_enable();
         *name_extension_error_ptr = name_manager_ptr;
         return(MQX_INVALID_COMPONENT_BASE);
      } /* Endif */
#endif
      for (i = 0; i < name_manager_ptr->NUMBER_IN_BLOCK; ++i) {
         if (name_manager_ptr->NAMES[i].NAME[0] != '\0') {
            ++total;
         } /* Endif */
      } /* Endfor */
      name_manager_ptr = name_manager_ptr->NEXT_TABLE;
   } /* Endwhile */
   _int_enable();

   if (total != base_name_manager_ptr->NUMBER) {
      *name_extension_error_ptr = name_manager_ptr;
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
