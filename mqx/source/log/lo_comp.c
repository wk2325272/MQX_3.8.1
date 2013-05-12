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
* $FileName: lo_comp.c$
* $Version : 3.0.6.0$
* $Date    : Nov-27-2008$
*
* Comments:
*
*   This file contains the function for creating the log component
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LOGS
#include "log.h"
#include "log_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _log_create_component
* Returned Value   : _mqx_uint MQX_OK, MQX_OUT_OF_MEMORY
* Comments         :
*   This function creates a kernel component providing a general log 
* service for all user tasks.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _log_create_component
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   LOG_COMPONENT_STRUCT_PTR log_component_ptr;

   _GET_KERNEL_DATA(kernel_data);

   log_component_ptr = (LOG_COMPONENT_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(LOG_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (log_component_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(log_component_ptr, MEM_TYPE_LOG_COMPONENT);

   /* We must exclude all ISRs at this point */
   _int_disable();

#if MQX_CHECK_ERRORS
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_LOG] != NULL) {
      _int_enable();
      _mem_free(log_component_ptr);
      return(MQX_OK);
   } /* Endif */
#endif

   kernel_data->KERNEL_COMPONENTS[KERNEL_LOG] = log_component_ptr;
   log_component_ptr->VALID = LOG_VALID;
   _int_enable();

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LOGS */

/* EOF */
