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
* $FileName: kl_creaa.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a kernel log.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_KERNEL_LOGGING
#include "lwlog.h"
#include "lwlogprv.h"
#include "klog.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _klog_create_at
* Returned Value   : MQX_OK or error code
* Comments         :
*   This function creates the kernel log at a specified area of memory
*
*END*----------------------------------------------------------------------*/

_mqx_uint _klog_create_at
   (
      /* [IN] the maximum size of the data to be stored in _mqx_max_types */
      _mqx_uint  max_size,

      /* [IN] flags about the properties of the log */
      _mqx_uint  flags,

      /* [IN] where in memory the log is to start */
      pointer    where

   )
{ /* Body */

   /* 
   ** The size of the kernel log must be a multiple of the size of a
   ** light weight log entry
   */
   max_size = max_size * sizeof(_mqx_max_type) / sizeof(LWLOG_ENTRY_STRUCT);

   return _lwlog_create_at(LOG_KERNEL_LOG_NUMBER, max_size, flags, where);

} /* Endbody */
#endif /* MQX_KERNEL_LOGGING */

/* EOF */
