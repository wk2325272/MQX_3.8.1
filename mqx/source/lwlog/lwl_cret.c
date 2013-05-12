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
* $FileName: lwl_cret.c$
* $Version : 3.0.5.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a lw log in kernel memory.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWLOGS
#include "lwlog.h"
#include "lwlogprv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwlog_create
* Returned Value   : _mqx_uint MQX_OK, or mqx error code
* Comments         :
*   This function creates a new log.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwlog_create
   (

      /* [IN] the log number to be used */
      _mqx_uint log_number, 

      /* [IN] the maximum number of entries */
      _mqx_uint max_size,

      /* [IN] flags about the properties of the log */
      _mqx_uint flags

   )
{ /* Body */
   LWLOG_HEADER_STRUCT_PTR  log_header_ptr;
   _mqx_uint                result;

#if MQX_CHECK_ERRORS
   if (max_size == 0) {
      return MQX_INVALID_SIZE;
   } /* Endif */
#endif

   log_header_ptr = _mem_alloc_system_zero( (_mem_size)sizeof(LWLOG_HEADER_STRUCT) +
      (_mem_size)(max_size-1) * (_mem_size)sizeof(LWLOG_ENTRY_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (log_header_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(log_header_ptr, MEM_TYPE_LWLOG);
   
   result = _lwlog_create_internal(log_number, max_size, flags, log_header_ptr);
   if (result == MQX_OK) {
      log_header_ptr->TYPE = LWLOG_DYNAMIC;
   } else {
      _mem_free(log_header_ptr);
   } /* Endif */

   return(result);

} /* Endbody */
#endif /* MQX_USE_LWLOGS */

/* EOF */
