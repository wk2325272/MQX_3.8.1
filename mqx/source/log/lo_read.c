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
* $FileName: lo_read.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for reading an entry from a log.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LOGS
#include "log.h"
#include "log_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _log_read
* Returned Value   : _mqx_uint MQX_OK, or a MQX error code.
* Comments         :
*   This function reads data from the log
*
*END*----------------------------------------------------------------------*/

_mqx_uint _log_read
   (

      /* [IN] the log number to be used */
      _mqx_uint            log_number,

      /* [IN] what type of read to perform */
      _mqx_uint            read_type,
      
      /* 
      ** [IN] the maximum number of _mqx_uints to be read from this entry,
      ** NOT including header. Header will always be transferred.
      */
      _mqx_uint            size_to_read,
      
      /* [IN] the address of the log entry information */
      LOG_ENTRY_STRUCT_PTR entry_ptr

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   LOG_COMPONENT_STRUCT_PTR log_component_ptr;
   LOG_HEADER_STRUCT_PTR    log_header_ptr;
   LOG_ENTRY_STRUCT_PTR     log_ptr;
   _mqx_uint_ptr            data_ptr;
   _mqx_uint_ptr            next_ptr;
   _mqx_uint_ptr            user_ptr;
   _mqx_uint                size;
   uint_16                  milliseconds;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (log_number >= LOG_MAXIMUM_NUMBER) {
      return(LOG_INVALID);
   } /* Endif */
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_LOG] == NULL) {
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
   if (entry_ptr == NULL) {
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif

   log_component_ptr = (LOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_LOG];

#if MQX_CHECK_VALIDITY
   if (log_component_ptr->VALID != LOG_VALID) {
      return(MQX_INVALID_COMPONENT_HANDLE);
   } /* Endif */   
#endif

   _INT_DISABLE();
   log_header_ptr = log_component_ptr->LOGS[log_number];
#if MQX_CHECK_ERRORS
   if (log_header_ptr == NULL) {
      _int_enable();
      return(LOG_DOES_NOT_EXIST);
   } /* Endif */
#endif

   if (log_header_ptr->SIZE == 0) {
      /* No data available */
      _INT_ENABLE();
      return(LOG_ENTRY_NOT_AVAILABLE);
   } /* Endif */

   if (read_type == LOG_READ_OLDEST_AND_DELETE) {
      data_ptr = log_header_ptr->LOG_READ;
   } else if (read_type == LOG_READ_NEXT) {
      data_ptr = log_header_ptr->LOG_NEXT;
      if (data_ptr == NULL) {
         _INT_ENABLE();
         return(LOG_ENTRY_NOT_AVAILABLE);
      } /* Endif */
   } else if (read_type == LOG_READ_OLDEST) {
      data_ptr = log_header_ptr->LOG_READ;
   } else if (read_type == LOG_READ_NEWEST) {
      data_ptr = log_header_ptr->LAST_LOG;
#if MQX_CHECK_ERRORS
   } else {
      _int_enable();
      return(LOG_INVALID_READ_TYPE);
#endif
   } /* Endif */

   size = *data_ptr;
   next_ptr = data_ptr + size;

/* Start CR 570 */
#if 0
   if (next_ptr >= log_header_ptr->LOG_END) {
      next_ptr = log_header_ptr->LOG_START +
         (next_ptr - log_header_ptr->LOG_END);
   } /* Endif */
#else
   if (log_header_ptr->LOG_READ < log_header_ptr->LOG_WRITE) {
      /* R < W */
      if (next_ptr >= log_header_ptr->LOG_WRITE) {
         next_ptr = log_header_ptr->LOG_READ;
      } /* Endif */
   } else {
      /* R > W */
      if (next_ptr >= log_header_ptr->LOG_END) {
         next_ptr = log_header_ptr->LOG_START +
            (next_ptr - log_header_ptr->LOG_END);
      } /* Endif */
      if (next_ptr == log_header_ptr->LOG_WRITE) {
         next_ptr = log_header_ptr->LOG_READ;
      } /* Endif */
   } /* Endif */
#endif
/* End CR 570 */

   if (read_type == LOG_READ_OLDEST_AND_DELETE) {
      log_header_ptr->LOG_READ += size;
      log_header_ptr->SIZE -= size;
      if (log_header_ptr->LOG_READ >= log_header_ptr->LOG_END) {
         log_header_ptr->LOG_READ = log_header_ptr->LOG_START +
            (log_header_ptr->LOG_READ - log_header_ptr->LOG_END);
      } /* Endif */
   } /* Endif */

   if (size > (size_to_read + (_mqx_uint)sizeof(LOG_ENTRY_STRUCT)/sizeof(_mqx_uint))) {
      size = (size_to_read + (_mqx_uint)sizeof(LOG_ENTRY_STRUCT)/sizeof(_mqx_uint));
   } /* Endif */
   
   ++size;
   user_ptr = (_mqx_uint_ptr)((pointer)entry_ptr);
   while (--size) {
      *user_ptr++ = *data_ptr++;
      if (data_ptr == log_header_ptr->LOG_END) {
         data_ptr = log_header_ptr->LOG_START;
      } /* Endif */
   } /* Endwhile */ 
   log_ptr = entry_ptr;
   if (log_ptr->MICROSECONDS > MICROSECS_IN_MILLISECOND) {
      milliseconds = log_ptr->MICROSECONDS / (uint_16)MICROSECS_IN_MILLISECOND;
      log_ptr->MILLISECONDS += milliseconds;
      log_ptr->MICROSECONDS -= (milliseconds * (uint_16)MICROSECS_IN_MILLISECOND);
   } /* Endif */

   if (read_type == LOG_READ_NEXT) {
      log_header_ptr->LOG_NEXT = next_ptr;
   } else if (read_type == LOG_READ_OLDEST) {
      log_header_ptr->LOG_NEXT = next_ptr;
   } else {
      log_header_ptr->LOG_NEXT = NULL;
   } /* Endif */
   _INT_ENABLE();

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LOGS */

/* EOF */
