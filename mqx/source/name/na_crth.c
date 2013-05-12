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
* $FileName: na_crth.c$
* $Version : 3.0.5.0$
* $Date    : Nov-27-2008$
*
* Comments:
*
*   This file contains the internal function for creating a name
*   component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_create_handle_internal
* Returned Value   : a Task Error Code
* Comments         :
*   This function initializes a name component.  It provides
* for the storage and retrieval of a name associated with a number.
* The name component can be used by any other kernel components.
* Each may have a separate set of names.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_create_handle_internal
   (
      /* [OUT] address where the name data structure pointer is to be stored */
      pointer _PTR_  name_handle,

      /* [IN] the initial number of names that can be stored  */
      _mqx_uint      initial_number,

      /* [IN] the number of names to be added when table full */
      _mqx_uint      grow_number,

      /* [IN] the maximum number of names that can be stored */
      _mqx_uint      maximum_number,

      /* [IN] the total number of name spaces in all chained pools */
      _mqx_uint      total_so_far
   )
{ /* Body */
   register NAME_COMPONENT_STRUCT_PTR name_manager_ptr;

   name_manager_ptr = (NAME_COMPONENT_STRUCT_PTR)_mem_alloc_system_zero((_mem_size)
      (sizeof(NAME_COMPONENT_STRUCT) +
      ((initial_number - 1) * sizeof(NAME_STRUCT))));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (name_manager_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
  _mem_set_type(name_manager_ptr, MEM_TYPE_NAME_COMPONENT);

   /* The name component has been created, so we must create the semaphore */
   if (initial_number == total_so_far) {
      /* Initialize the semaphore for this name component */
      _lwsem_create((LWSEM_STRUCT_PTR)(&name_manager_ptr->SEM), 1);
   } /* Endif */

   name_manager_ptr->GROW_NUMBER    = grow_number;
   if (maximum_number == 0) {
      name_manager_ptr->MAX_NUMBER  = MAX_MQX_UINT;
   } else if (maximum_number < initial_number) {
      name_manager_ptr->MAX_NUMBER  = initial_number;
   } else {
      name_manager_ptr->MAX_NUMBER  = maximum_number;
   } /* Endif */
   name_manager_ptr->NUMBER_IN_BLOCK = initial_number;
   name_manager_ptr->TOTAL_NUMBER    = total_so_far;
   name_manager_ptr->VALID           = NAME_VALID;

   *(NAME_COMPONENT_STRUCT_PTR _PTR_)name_handle = name_manager_ptr;

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
