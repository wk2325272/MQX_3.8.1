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
* $FileName: na_fni.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the internal function for finding a name
*   in the name component, given the number.
*
*END************************************************************************/

#include <string.h>
#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_find_name_internal
* Returned Value   : _mqx_uint MQX error code
* Comments         :
*   This function looks up a number in the name component, and returns the
* name associated with the number.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_find_name_internal
   (
      /* [IN] the handle returned by _name_internal_create */
      pointer       name_handle,

      /* [IN] the number to be looked up */
      _mqx_max_type number,

      /* [OUT] the location where the name is to be written to */
      char_ptr      name_string_ptr
   )
{ /* Body */
   register NAME_COMPONENT_STRUCT_PTR name_manager_ptr;
   register NAME_STRUCT_PTR           name_ptr;
   register _mqx_uint                 i;

   name_manager_ptr = (NAME_COMPONENT_STRUCT_PTR) name_handle;
#if MQX_CHECK_ERRORS
   if (name_manager_ptr == NULL)  {
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (name_manager_ptr->VALID != NAME_VALID) {
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   /* Scan to end of table, if found, return number */
   while (TRUE) {
      i        = name_manager_ptr->NUMBER_IN_BLOCK + 1;
      name_ptr = (NAME_STRUCT_PTR)&name_manager_ptr->NAMES[0];
      while (--i) {
         if (name_ptr->NAME[0] != '\0') {
            if (number == name_ptr->NUMBER) {  /* MATCH */
               strncpy(name_string_ptr, name_ptr->NAME, 
                  (_mqx_uint)NAME_MAX_NAME_SIZE-1);
               name_string_ptr[NAME_MAX_NAME_SIZE-1] = '\0';
               return(MQX_OK);
            } /* Endif */
         } /* Endif */
         name_ptr++;
      } /* Endwhile */

      if (name_manager_ptr->NEXT_TABLE == NULL) {
         return(NAME_NOT_FOUND);
      } else {
         name_manager_ptr = name_manager_ptr->NEXT_TABLE;
      } /* Endif */
#if MQX_CHECK_VALIDITY
      if (name_manager_ptr->VALID != NAME_VALID) {
         return(MQX_INVALID_COMPONENT_BASE);
      } /* Endif */
#endif
   } /* Endwhile */
   
} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
