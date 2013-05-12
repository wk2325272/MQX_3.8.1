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
* $FileName: ti_difft.c$
* $Version : 3.6.6.0$
* $Date    : Oct-25-2010$
*
* Comments:
*
*   This file contains the function for calculating the difference
*   between two times given in ticks.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_diff_ticks
* Returned Value   : _mqx_uint - MQX_OK or error code
* Comments         :
*    This function calculates the difference between two times in ticks.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _time_diff_ticks
   (
      /* [IN] the ending time */
      MQX_TICK_STRUCT_PTR end_tick_ptr,

      /* [IN] the starting time */
      MQX_TICK_STRUCT_PTR start_tick_ptr,

      /* [OUT] the difference in time */
      MQX_TICK_STRUCT_PTR diff_tick_ptr
   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if ((end_tick_ptr == NULL) || (start_tick_ptr == NULL) || 
      (diff_tick_ptr == NULL)) 
   {
      return MQX_INVALID_PARAMETER;
   } /* Endif */
#endif

   PSP_SUB_TICKS(end_tick_ptr, start_tick_ptr, diff_tick_ptr);

   return MQX_OK;

} /* Endbody */



/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_diff_ticks_int32
* Returned Value   : ticks difference in int32 and overflow/underflow flag
* Comments         :
*    This function calculates the difference between two times in ticks
*    and clamps result into int_32 interval.
*
*END*----------------------------------------------------------------------*/

int_32 _time_diff_ticks_int32
   (
      /* [IN] the ending time */
      MQX_TICK_STRUCT_PTR end_tick_ptr,

      /* [IN] the starting time */
      MQX_TICK_STRUCT_PTR start_tick_ptr,

      /* [OUT] Set to TRUE if overflow occurs */
      boolean _PTR_       overflow_ptr
   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if ((end_tick_ptr == NULL) || (start_tick_ptr == NULL))
   {
      if (overflow_ptr != NULL) *overflow_ptr = TRUE;
      return MAX_INT_32;
   } /* Endif */
#endif

   return (PSP_SUB_TICKS_INT32(end_tick_ptr, start_tick_ptr, overflow_ptr));
} /* Endbody */

/* EOF */
