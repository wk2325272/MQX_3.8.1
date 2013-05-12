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
* $FileName: lo_write.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for writing to a log.
*
*END************************************************************************/

#include <stdarg.h>
#include "mqx_inc.h"
#if MQX_USE_LOGS
#include "log.h"
#include "log_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _log_write
* Returned Value   : _mqx_uint MQX_OK, or an MQX error code.
* Comments         :
*   This function writes data into the log
*
*END*----------------------------------------------------------------------*/

_mqx_uint _log_write
   (

      /* [IN] the log number to be used */
      _mqx_uint  log_number,

      /* [IN] the number of parameters to follow */
      _mqx_uint  number_of_parameters,

      ...

   )
{ /* Body */
   MQX_TICK_STRUCT          ticks;
   TIME_STRUCT              time;
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   LOG_COMPONENT_STRUCT_PTR log_component_ptr;
   LOG_HEADER_STRUCT_PTR    log_header_ptr;
   _mqx_uint_ptr            data_ptr;
   va_list                  param_ptr;
   _mqx_uint                us;
   _mqx_uint                i;
   _mqx_uint                size;
   _mqx_uint                old_size;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_LOG] == NULL) {
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif

   log_component_ptr = (LOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_LOG];

#if MQX_CHECK_VALIDITY
   if (log_component_ptr->VALID != LOG_VALID) {
      return(MQX_INVALID_COMPONENT_HANDLE);
   } /* Endif */   
#endif

#if MQX_CHECK_ERRORS
   if (log_number >= LOG_MAXIMUM_NUMBER) {
      return(LOG_INVALID);
   } /* Endif */
#endif

   log_header_ptr = log_component_ptr->LOGS[log_number];

#if MQX_CHECK_ERRORS
   if (log_header_ptr == NULL) {
      return(LOG_DOES_NOT_EXIST);
   } /* Endif */
#endif

   if (! (log_header_ptr->FLAGS & LOG_ENABLED)) {
      return(LOG_DISABLED);
   } /* Endif */
   

   size = sizeof(LOG_ENTRY_STRUCT)/sizeof(_mqx_uint) + number_of_parameters;
#if MQX_CHECK_ERRORS
   if (size > log_header_ptr->MAX) {
      /* 
      ** The request is to log an entry larger than the
      ** entire log. Reject this.
      */
      return(LOG_FULL);
   } /* Endif */
#endif

   va_start(param_ptr, number_of_parameters);

   i = number_of_parameters + 1;
   
   _INT_DISABLE();
   if ((log_header_ptr->SIZE + size) > log_header_ptr->MAX) {
      if (log_header_ptr->FLAGS & LOG_OVERWRITE) {
         /* Remove leading elements until it can fit */
         while ((log_header_ptr->SIZE + size) > log_header_ptr->MAX) {
            data_ptr = log_header_ptr->LOG_READ;
            old_size = *data_ptr;
            log_header_ptr->SIZE -= old_size;
            data_ptr += old_size;
            if (data_ptr >= log_header_ptr->LOG_END) {
               data_ptr = log_header_ptr->LOG_START +
                  (data_ptr - log_header_ptr->LOG_END);
            } /* Endif */
            if (log_header_ptr->LOG_NEXT == log_header_ptr->LOG_READ) {
               /* Move up the next read pointer */
               log_header_ptr->LOG_NEXT = data_ptr;
            } /* Endif */
            log_header_ptr->LOG_READ = data_ptr;
         } /* Endwhile */
      } else {
         _INT_ENABLE();
         va_end(param_ptr);
         return(LOG_FULL);
      } /* Endif */
   } /* Endif */
   
   log_header_ptr->SIZE += size;

   data_ptr = log_header_ptr->LOG_WRITE;
   log_header_ptr->LAST_LOG = data_ptr;

   *data_ptr++ = size;
   if (data_ptr == log_header_ptr->LOG_END) {
      data_ptr = log_header_ptr->LOG_START;
   } /* Endif */

   *data_ptr++ = log_header_ptr->NUMBER++;
   if (data_ptr == log_header_ptr->LOG_END) {
      data_ptr = log_header_ptr->LOG_START;
   } /* Endif */

   us = (_mqx_uint)_time_get_microseconds();   
   PSP_ADD_TICKS(&kernel_data->TIME, &kernel_data->TIME_OFFSET, &ticks);
   PSP_TICKS_TO_TIME(&ticks, &time);

#if MQX_INT_SIZE_IN_BITS >= 32
   *data_ptr++  = (_mqx_uint)time.SECONDS;
#else
   #if PSP_ENDIAN == MQX_BIG_ENDIAN
      *data_ptr++ = (_mqx_uint)((time.SECONDS >> MQX_INT_SIZE_IN_BITS) & 
         MAX_MQX_UINT);
      if (data_ptr == log_header_ptr->LOG_END) {
         data_ptr = log_header_ptr->LOG_START;
      } /* Endif */
      *data_ptr++ = (_mqx_uint)(time.SECONDS & MAX_MQX_UINT);
   #else
      *data_ptr++ = (_mqx_uint)(time.SECONDS & MAX_MQX_UINT);
      if (data_ptr == log_header_ptr->LOG_END) {
         data_ptr = log_header_ptr->LOG_START;
      } /* Endif */
      *data_ptr++ = (_mqx_uint)((time.SECONDS >> MQX_INT_SIZE_IN_BITS) & 
         MAX_MQX_UINT);
   #endif
#endif

   if (data_ptr >= log_header_ptr->LOG_END) {
      data_ptr = log_header_ptr->LOG_START;
   } /* Endif */
   
#if PSP_MEMORY_ADDRESSING_CAPABILITY >= 32
   *data_ptr++ = time.MILLISECONDS;
   if (data_ptr == log_header_ptr->LOG_END) {
      data_ptr = log_header_ptr->LOG_START;
   } /* Endif */
   *data_ptr++ = us;
#elif MQX_INT_SIZE_IN_BITS == 32
   #if PSP_ENDIAN == MQX_LITTLE_ENDIAN
      *data_ptr++ = time.MILLISECONDS | (us << 16);
   #else
      *data_ptr++ = us | (time.MILLISECONDS << 16);
   #endif
#else
   *data_ptr++ = (_mqx_uint)time.MILLISECONDS;
   if (data_ptr == log_header_ptr->LOG_END) {
      data_ptr = log_header_ptr->LOG_START;
   } /* Endif */
   *data_ptr++ = us;
#endif

   if (data_ptr >= log_header_ptr->LOG_END) {
      data_ptr = log_header_ptr->LOG_START;
   } /* Endif */

   while (--i) {
      *data_ptr++ = (_mqx_uint)va_arg(param_ptr, _mqx_uint);
      if (data_ptr == log_header_ptr->LOG_END) {
         data_ptr = log_header_ptr->LOG_START;
      } /* Endif */
   } /* Endwhile */

   log_header_ptr->LOG_WRITE = data_ptr;
   _INT_ENABLE();

   va_end(param_ptr);

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LOGS */

/* EOF */
