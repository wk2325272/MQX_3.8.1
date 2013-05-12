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
* $FileName: na_findn.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for finding a name, given
*   the number associated with the name.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _name_find_by_number
* Returned Value   : _mqx_uint MQX error code
* Comments         :
*   This function finds a name in the name component, given the number.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _name_find_by_number
   (
      /* [IN] the number to find */
      _mqx_max_type number,

      /* [OUT] the location where the name is to be written */
      char_ptr      name_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   pointer                handle;
   _mqx_uint              result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_name_find_name, number, name_ptr);

   handle = kernel_data->KERNEL_COMPONENTS[KERNEL_NAME_MANAGEMENT];
   if (handle == NULL) {
      _KLOGX2(KLOG_name_find_name, MQX_COMPONENT_DOES_NOT_EXIST);
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */

   result = _name_find_name_internal(handle, number, name_ptr);

   _KLOGX2(KLOG_name_find_name, result);
   return(result);

} /* Endbody */
#endif /* MQX_USE_NAME */

/* EOF */
