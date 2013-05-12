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
* $FileName: psp_psti.c$
* $Version : 3.5.3.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for converting picoseconds to a ticks
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _psp_psecs_to_ticks
 * Returned Value   : void
 * Comments         :
 *   This function converts picoseconds into ticks. Note, there is no way to
 * represent MAX_UINT_16 picoseconds in terms of ticks.
 *
 *END*----------------------------------------------------------------------*/

void _psp_psecs_to_ticks
   (
       /* [IN] The number of picoseconds to convert */
       _mqx_uint           psecs,

       /* [OUT] Pointer to tick structure where the result will be stored */
       PSP_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   uint_64                tmp;
   uint_64                hwtick_calc;

   _GET_KERNEL_DATA(kernel_data);

   tmp = (uint_64)psecs * kernel_data->TICKS_PER_SECOND;


   tick_ptr->TICKS[0] = tmp / 1000000000000ULL;
   /* Calculate the remaining nanoticks */
   tmp %= 1000000000000ULL;

   
   /* Convert to hardware ticks 
   ** (use 500000000000 which is 1000000000000/2) to avoid extra * 2 for rounding
   */
   hwtick_calc = 500000000000ULL / kernel_data->HW_TICKS_PER_TICK;
   if ((hwtick_calc * kernel_data->HW_TICKS_PER_TICK) == 500000000000ULL) {
      /* Integral number */
      tmp = tmp / hwtick_calc;
   } else {
      /* Convert to hardware ticks */
      tmp = ((tmp * kernel_data->HW_TICKS_PER_TICK ) * 2) / (1000000000000ULL);
   }/* Endif */
   tmp++;
   tmp /= 2; /* For rounding */
   tick_ptr->HW_TICKS[0] = (uint_32)tmp;

} /* Endbody */
#endif

/* EOF */
