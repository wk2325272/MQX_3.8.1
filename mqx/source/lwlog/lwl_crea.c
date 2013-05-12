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
* $FileName: lwl_crea.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a LW log at a specified 
*   location.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWLOGS
#include "lwlog.h"
#include "lwlogprv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwlog_create_at
* Returned Value   : _mqx_uint MQX_OK, or mqx error code
* Comments         :
*   This function creates a new log.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwlog_create_at
   (

      /* [IN] the log number to be used */
      _mqx_uint log_number, 

      /* [IN] the maximum number of entries */
      _mqx_uint max_size,

      /* [IN] flags about the properties of the log */
      _mqx_uint flags,

      /* [IN] where the log should be created */
      pointer   where

   )
{ /* Body */
   LWLOG_HEADER_STRUCT_PTR  log_header_ptr = (LWLOG_HEADER_STRUCT_PTR)where;
   _mqx_uint                result;

#if MQX_CHECK_ERRORS
   if (max_size == 0) {
      return(LOG_INVALID_SIZE);
   } /* Endif */

   if (where == NULL) {
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif

   result = _lwlog_create_internal(log_number, max_size, flags, log_header_ptr);
   if (result == MQX_OK) {
      log_header_ptr->TYPE = LWLOG_STATIC;
   } /* Endif */

   return(result);

} /* Endbody */
#endif /* MQX_USE_LWLOGS */

/* EOF */
