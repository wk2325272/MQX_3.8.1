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
* $FileName: ti_init.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for initializing a tick struct with
*   a quantity of ticks
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _time_init_ticks
 * Returned Value   : _mqx_uint - MQX_OK or error code
 * Comments         :
 *   This function initializes a tick struct
 *
 *END*----------------------------------------------------------------------*/

_mqx_uint _time_init_ticks
   (
       /* [IN/OUT] The structure to initialize */
       MQX_TICK_STRUCT_PTR tick_ptr,

       /* [IN] The number of ticks to initialize with */
       _mqx_uint           ticks
   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if (tick_ptr == NULL) {
      return MQX_INVALID_PARAMETER;
   } /* Endif */
#endif

   if (ticks) {
      /* Start CR 195 */
      tick_ptr->HW_TICKS = 0;
      /* End CR 195 */
      PSP_ADD_TICKS_TO_TICK_STRUCT(&_mqx_zero_tick_struct, ticks, tick_ptr);
   } else {
      *tick_ptr = _mqx_zero_tick_struct;
   } /* Endif */

   return MQX_OK;

} /* Endbody */

/* EOF */
