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
* $FileName: lwl_read.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for reading an entry from a log.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWLOGS
#include "lwlog.h"
#include "lwlogprv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwlog_read
* Returned Value   : _mqx_uint MQX_OK, or a MQX error code.
* Comments         :
*   This function reads data from the log
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwlog_read
   (

      /* [IN] the log number to be used */
      _mqx_uint log_number,

      /* [IN] what type of read to perform */
      _mqx_uint read_type,
      
      /* [IN] the address where the log entry information inforamtion should go */
      LWLOG_ENTRY_STRUCT_PTR entry_ptr

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   LWLOG_COMPONENT_STRUCT_PTR log_component_ptr;
   LWLOG_HEADER_STRUCT_PTR    log_header_ptr;
   LWLOG_ENTRY_STRUCT_PTR     log_ptr;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (log_number >= LOG_MAXIMUM_NUMBER) {
      return(LOG_INVALID);
   } /* Endif */
   if (entry_ptr == NULL) {
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif

   log_component_ptr = (LWLOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_LWLOG];

#if MQX_CHECK_ERRORS
   if (log_component_ptr == NULL) {
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif

   _int_disable();
#if MQX_CHECK_VALIDITY
   if (log_component_ptr->VALID != LWLOG_VALID) {
      _int_enable();
      return(MQX_INVALID_COMPONENT_HANDLE);
   } /* Endif */   
#endif

   log_header_ptr = log_component_ptr->LOGS[log_number];

#if MQX_CHECK_ERRORS
   if (log_header_ptr == NULL) {
      _int_enable();
      return(LOG_DOES_NOT_EXIST);
   } /* Endif */
#endif

   if (!log_header_ptr->CURRENT_ENTRIES) {
      /* No data available */
      _int_enable();
      return(LOG_ENTRY_NOT_AVAILABLE);
   } /* Endif */

   if (read_type == LOG_READ_OLDEST_AND_DELETE) {
      log_header_ptr->CURRENT_ENTRIES--;
      log_ptr = log_header_ptr->OLDEST_PTR;
      log_header_ptr->OLDEST_PTR = log_ptr->NEXT_PTR;
      log_header_ptr->READ_PTR   = log_ptr->NEXT_PTR;
      log_header_ptr->READS = 0;
   } else if (read_type == LOG_READ_OLDEST) {
      log_ptr = log_header_ptr->OLDEST_PTR;
      log_header_ptr->READ_PTR = log_ptr->NEXT_PTR;
      log_header_ptr->READS    = 1;
   } else if (read_type == LOG_READ_NEXT) {
      log_ptr = log_header_ptr->READ_PTR;
      if ((log_ptr == log_header_ptr->WRITE_PTR->NEXT_PTR) &&
         (log_header_ptr->READS >= log_header_ptr->CURRENT_ENTRIES)) 
      {
         _int_enable();
         return(LOG_ENTRY_NOT_AVAILABLE);
      } /* Endif */
      log_header_ptr->READ_PTR = log_ptr->NEXT_PTR;
      log_header_ptr->READS++;
   } else if (read_type == LOG_READ_NEWEST) {
      log_header_ptr->READS = log_header_ptr->CURRENT_ENTRIES;
      log_ptr = log_header_ptr->WRITE_PTR;
#if MQX_CHECK_ERRORS
   } else {
      _int_enable();
      return(LOG_INVALID_READ_TYPE);
#endif
   } /* Endif */

   *entry_ptr = *log_ptr;

   _int_enable();

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LWLOGS */

/* EOF */
