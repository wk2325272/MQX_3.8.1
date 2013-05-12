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
* $FileName: na_close.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the internal function for deleting a name.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_destroy_handle_internal
* Returned Value   : _mqx_uint MQX error code
* Comments         :
*   This function completely removes the specified name table.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_destroy_handle_internal
   (
      /* [IN] the name data structure pointer  */
      pointer name_handle
   )
{ /* Body */
   register NAME_COMPONENT_STRUCT_PTR name_manager_ptr;
   register NAME_COMPONENT_STRUCT_PTR next_ptr;

   name_manager_ptr = (NAME_COMPONENT_STRUCT_PTR) name_handle;
#if MQX_CHECK_ERRORS
   if (name_manager_ptr == NULL){
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif

   _int_disable();
#if MQX_CHECK_VALIDITY
   if (name_manager_ptr->VALID != NAME_VALID){
      _int_enable();
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif
   _lwsem_wait((LWSEM_STRUCT_PTR)(&name_manager_ptr->SEM));
   name_manager_ptr->VALID = 0;
   _int_enable();

   _lwsem_destroy((LWSEM_STRUCT_PTR)(&name_manager_ptr->SEM));

   while (name_manager_ptr) {
      next_ptr = name_manager_ptr->NEXT_TABLE;
      name_manager_ptr->VALID = 0;
      _mem_free(name_manager_ptr);
      name_manager_ptr = next_ptr;
   } /* Endwhile */

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
