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
* $FileName: lwl_writ.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for writing to a log.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWLOGS
#include "lwlog.h"
#include "lwlogprv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwlog_write
* Returned Value   : _mqx_uint MQX_OK, or an MQX error code.
* Comments         :
*   This wrapper function to write data into the log
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwlog_write
   (

      /* [IN] the log number to be used */
      _mqx_uint      log_number,

      /* [IN] The data to be written into the log entry */
      _mqx_max_type  p1,
      _mqx_max_type  p2,
      _mqx_max_type  p3,
      _mqx_max_type  p4,
      _mqx_max_type  p5,
      _mqx_max_type  p6,
      _mqx_max_type  p7
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   _mqx_uint               result;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (log_number >= LOG_MAXIMUM_NUMBER) {
      return(LOG_INVALID);
   } /* Endif */

   if (kernel_data->KERNEL_COMPONENTS[KERNEL_LWLOG] == NULL) {
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif

   _INT_DISABLE();

   result = _lwlog_write_internal(log_number, p1, p2, p3, p4, p5, p6, p7);

   _INT_ENABLE();

   return(result);

} /* Endbody */
#endif /* MQX_USE_LWLOGS */

/* EOF */
