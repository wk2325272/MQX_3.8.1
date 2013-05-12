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
* $FileName: lwl_crei.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a lw log.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWLOGS
#include "lwlog.h"
#include "lwlogprv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwlog_create_internal
* Returned Value   : _mqx_uint MQX_OK, or mqx error code
* Comments         :
*   This function creates a new log.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwlog_create_internal
   (

      /* [IN] the log number to be used */
      _mqx_uint               log_number, 

      /* [IN] the maximum number of entries */
      _mqx_uint               max_size,

      /* [IN] flags about the properties of the log */
      _mqx_uint               flags,

      /* [IN] where the log should be created */
      LWLOG_HEADER_STRUCT_PTR log_header_ptr

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   LWLOG_COMPONENT_STRUCT_PTR log_component_ptr;
   LWLOG_ENTRY_STRUCT_PTR     entry_ptr;
   _mqx_uint                  result, i;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (log_number >= LOG_MAXIMUM_NUMBER) {
      return(LOG_INVALID);
   } /* Endif */
#endif

   log_component_ptr = (LWLOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_LWLOG];
   if (log_component_ptr == NULL) {
      result = _lwlog_create_component();
      log_component_ptr = (LWLOG_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_LWLOG];
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (log_component_ptr == NULL) {
         return(result);
      } /* Endif */
#endif
#if MQX_CHECK_ERRORS
   } else if (log_component_ptr->LOGS[log_number] != NULL) {
      return(LOG_EXISTS);
#endif
   } /* Endif */

#if MQX_CHECK_VALIDITY
   if (log_component_ptr->VALID != LWLOG_VALID) {
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */   
#endif
   _mem_zero((pointer)log_header_ptr, (_mem_size)sizeof(LWLOG_HEADER_STRUCT));

   log_header_ptr->FLAGS       = flags;
   log_header_ptr->FLAGS      |= LWLOG_ENABLED;
   log_header_ptr->NUMBER      = 1;
   log_header_ptr->MAX_ENTRIES = max_size;

   entry_ptr = &log_header_ptr->FIRST_ENTRY;
   
   log_header_ptr->READ_PTR   = entry_ptr;
   log_header_ptr->OLDEST_PTR = entry_ptr;

   max_size--;
   for ( i = 0; i < max_size; i++ ) {
      entry_ptr->NEXT_PTR = entry_ptr + 1;
      entry_ptr++;
   } /* Endfor */

   log_header_ptr->WRITE_PTR = entry_ptr;
   entry_ptr->NEXT_PTR = log_header_ptr->READ_PTR;

   _int_disable();
#if MQX_CHECK_ERRORS
   if (log_component_ptr->LOGS[log_number] != NULL) {
      _int_enable();
      return(LOG_EXISTS);
   } /* Endif */
#endif
   log_component_ptr->LOGS[log_number] = log_header_ptr;
   _int_enable();

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LWLOGS */

/* EOF */
