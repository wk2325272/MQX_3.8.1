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
* $FileName: ti_ti2xd.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for converting from a tick struct
*   to an xdate struct.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _time_ticks_to_xdate
* Returned Value  : boolean
* Comments        : converts ticks into a date and time from
*                   Jan.1 1970
*
*END*------------------------------------------------------------------*/

boolean _time_ticks_to_xdate
   (

      /*  [IN]  pointer to tick structure  */
      MQX_TICK_STRUCT_PTR  tick_ptr,

      /*  [OUT]  pointer to a xdate structure  */
      MQX_XDATE_STRUCT_PTR xdate_ptr

   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if ((tick_ptr == NULL) || (xdate_ptr == NULL)) {
      return (FALSE);
   } /* Endif */
#endif

   return PSP_TICKS_TO_XDATE(tick_ptr, xdate_ptr);

} /* Endbody */

/* EOF */
