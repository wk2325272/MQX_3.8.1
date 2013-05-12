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
* $FileName: lwl_size.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for calculating the amount of
*   memory needed to create a LW log given the number of entries.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWLOGS
#include "lwlog.h"
#include "lwlogprv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwlog_calculate_size
* Returned Value   : _mem_size - size needed for lw log
* Comments         :
*   This function calculates the size necessary to contain a LW log
*
*END*----------------------------------------------------------------------*/

_mem_size _lwlog_calculate_size
   (

      /* [IN] the maximum number of entries */
      _mqx_uint entries

   )
{ /* Body */
   _mem_size result;

#if MQX_CHECK_ERRORS
   if (entries == 0) {
      return 0;
   } /* Endif */
#endif

   result = (_mem_size)sizeof(LWLOG_HEADER_STRUCT) +
      (_mem_size)(entries-1) * (_mem_size)sizeof(LWLOG_ENTRY_STRUCT);

   return result;

} /* Endbody */
#endif /* MQX_USE_LWLOGS */

/* EOF */
