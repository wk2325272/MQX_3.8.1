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
* $FileName: na_abi.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the internal function for adding a new name
*   into the component, using the index into the component, rather than the
*   name.
*
*END************************************************************************/

#include <string.h>
#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_add_internal_by_index
* Returned Value   : _mqx_uint MQX error code
* Comments         :
*   This function adds a name to the name component along with the _mqx_max_type
* number to be associate with the name.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_add_internal_by_index
   (
      /* [IN] the handle returned by _name_internal_create */
      pointer        name_handle,

      /* [IN] the name to be associated with the number */
      char _PTR_     name, 

      /* [IN] the number to be associated with the name */
      _mqx_max_type  number,

      /* [IN] the index in the name table */
      _mqx_uint      index
   )
{ /* Body */
            NAME_COMPONENT_STRUCT_PTR base_name_manager_ptr;
   register NAME_COMPONENT_STRUCT_PTR name_manager_ptr;
   register NAME_STRUCT_PTR           name_ptr;
            _mqx_uint                 result = MQX_OK;

   name_manager_ptr = (NAME_COMPONENT_STRUCT_PTR) name_handle;
#if MQX_CHECK_ERRORS
   if (name_manager_ptr == NULL) {
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (name_manager_ptr->VALID != NAME_VALID){
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif
   base_name_manager_ptr = name_manager_ptr;

   /* We are modifying the table, so lets get exclusive access */
   _int_disable();
#if MQX_CHECK_VALIDITY
   if (base_name_manager_ptr->VALID != NAME_VALID) {
      _int_enable();
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   _lwsem_wait((LWSEM_STRUCT_PTR)(&base_name_manager_ptr->SEM));
   _int_enable();

   /* Scan to correct location in table, and if not found, insert it */
   while (index >= name_manager_ptr->NUMBER_IN_BLOCK) {
      index -= name_manager_ptr->NUMBER_IN_BLOCK;
      if (name_manager_ptr->NEXT_TABLE == NULL) {
         if (name_manager_ptr->GROW_NUMBER &&
            ((name_manager_ptr->TOTAL_NUMBER + name_manager_ptr->GROW_NUMBER) <=
            name_manager_ptr->MAX_NUMBER))
         {
            result = _name_create_handle_internal(
               (pointer _PTR_)&name_manager_ptr->NEXT_TABLE,
               name_manager_ptr->GROW_NUMBER, 
               name_manager_ptr->GROW_NUMBER,
               name_manager_ptr->MAX_NUMBER,
               (name_manager_ptr->TOTAL_NUMBER +
                 name_manager_ptr->GROW_NUMBER));
            if (result != MQX_OK) {
               break;
            }/* Endif */
         } else {
            result = NAME_TABLE_FULL;
            break;
         } /* Endif */
      } /* Endif */
      name_manager_ptr = name_manager_ptr->NEXT_TABLE;
#if MQX_CHECK_VALIDITY
      if (name_manager_ptr->VALID != NAME_VALID){
         result = MQX_INVALID_COMPONENT_BASE;
         break;
      } /* Endif */
#endif
   } /* Endwhile */

   if (result == MQX_OK) {
      name_ptr = &name_manager_ptr->NAMES[index];
      if (name_ptr->NAME[0] == '\0'  ) {
         strncpy(name_ptr->NAME, name, (_mqx_uint)NAME_MAX_NAME_SIZE-1);
         name_ptr->NAME[NAME_MAX_NAME_SIZE-1] = '\0';
         name_ptr->NUMBER = number;
         base_name_manager_ptr->NUMBER++;
      } else {
         result = NAME_EXISTS;
      } /* Endif */
   } /* Endif */

   _lwsem_post((LWSEM_STRUCT_PTR)(&base_name_manager_ptr->SEM));

   return(result);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
