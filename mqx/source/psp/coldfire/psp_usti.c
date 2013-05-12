/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: psp_usti.c$
* $Version : 3.5.3.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for converting microseconds to a ticks
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_HW_TICKS

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _psp_usecs_to_ticks
 * Returned Value   : void
 * Comments         :
 *   This function converts microseconds into ticks
 *
 *END*----------------------------------------------------------------------*/

void _psp_usecs_to_ticks
   (
       /* [IN] The number of microseconds to convert */
       _mqx_uint           usecs,

       /* [OUT] Pointer to tick structure where the result will be stored */
       PSP_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   uint_64                tmp;
   uint_32                hwticks_calc;
   
   _GET_KERNEL_DATA(kernel_data);

   tmp = (uint_64)usecs * kernel_data->TICKS_PER_SECOND;

   tick_ptr->TICKS[0] = tmp / (1000 * 1000);
   /* Calculate the remaining microticks */
   tmp %= (1000 * 1000);

   
   /* Convert to hardware ticks 
   ** (use 500000 which is 1000000/2) to avoid extra * 2 for rounding
   */
   hwticks_calc = 500000 / kernel_data->HW_TICKS_PER_TICK;
   if ((hwticks_calc * kernel_data->HW_TICKS_PER_TICK) == 500000) {
      /* Integral calculation */
      tmp = tmp / hwticks_calc;
   } else {
      /* Convert to hardware ticks */
      tmp = ((tmp * kernel_data->HW_TICKS_PER_TICK ) * 2) / (1000 * 1000);
   }/* Endif */
   tmp++;
   tmp /= 2; /* For rounding */
   tick_ptr->HW_TICKS[0] = (uint_32)tmp;

} /* Endbody */

#endif

/* EOF */
