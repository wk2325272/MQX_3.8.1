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
* $FileName: ti_asec.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for adding seconds to a tick
*   struct
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _time_add_sec_to_ticks
 * Returned Value   : MQX_TICK_STRUCT_PTR - first parameter to function
 * Comments         :
 *   This function adds a quantity in seconds to a tick struct
 *
 *END*----------------------------------------------------------------------*/

MQX_TICK_STRUCT_PTR _time_add_sec_to_ticks
   (
       /* [IN] The structure to add to */
       MQX_TICK_STRUCT_PTR tick_ptr,

       /* [IN] The number of seconds to add */
       _mqx_uint           secs
   )
{ /* Body */
   MQX_TICK_STRUCT tmp;

   if (secs) {
      PSP_SECONDS_TO_TICKS(secs, &tmp);
      PSP_ADD_TICKS(tick_ptr, &tmp, tick_ptr);
   } /* Endif */

   return tick_ptr;

} /* Endbody */

/* EOF */
