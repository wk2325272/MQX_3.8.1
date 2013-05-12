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
* $FileName: psp_tkti.c$
* $Version : 3.5.5.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for converting between ticks and
*   the TIME_STRUCT format.
*
*END************************************************************************/

#include "mqx_inc.h"

/* Start CR 1256 */

/*FUNCTION*-----------------------------------------------------------------
* 
* Function Name    : _psp_ticks_to_time
* Returned Value   : boolean - Returns FALSE if overflow occurs
* Comments         :
*    This function converts ticks into the time struct format
*
*END*----------------------------------------------------------------------*/

#if MQX_HAS_TICK

#if 1


boolean _psp_ticks_to_time
   (
      /* [IN] Pointer to the tick struct to store the results in */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] Pointer to the time struct to convert */
      TIME_STRUCT_PTR     time_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   
#ifdef __ICCCF__   // IAR workaround - problem with 64bit local variables (compiler problem)
   _GET_KERNEL_DATA(kernel_data);
 
   time_ptr->SECONDS = tick_ptr->TICKS[0] / kernel_data->TICKS_PER_SECOND;
   time_ptr->MILLISECONDS = ((tick_ptr->TICKS[0] * 1000) / kernel_data->TICKS_PER_SECOND) - time_ptr->SECONDS * 1000;
#else
   uint_64 ms;

   _GET_KERNEL_DATA(kernel_data);

   ms = (tick_ptr->TICKS[0] * 1000) / kernel_data->TICKS_PER_SECOND;

   time_ptr->SECONDS = ms / 1000;
   time_ptr->MILLISECONDS = ms - time_ptr->SECONDS * 1000;
#endif
   
   return TRUE;
} /* Endbody   */

#else

boolean _psp_ticks_to_time
   (
      /* [IN] Pointer to the tick struct to store the results in */
      PSP_TICK_STRUCT_PTR tick_ptr,

      /* [OUT] Pointer to the time struct to convert */
      TIME_STRUCT_PTR     time_ptr
   )
{ /* Body */
   static uint_64         max_ticks = 0;
   static uint_64         hw_ticks_per_sec;
   static uint_32         tpms;
   static boolean         evenly_divisible;

   uint_64                tmp;
   uint_32                tps;
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   tps = kernel_data->TICKS_PER_SECOND;

   if (max_ticks == 0) {
      max_ticks = (uint_64)tps * MAX_UINT_32;
      tpms = 1000 / tps;
      evenly_divisible = 1000 % tps == 0;
      hw_ticks_per_sec = (uint_64)tps * kernel_data->HW_TICKS_PER_TICK;
   } /* Endif */

   /* Saturate if ticks go out of range of time struct */
   if (tick_ptr->TICKS[0] > max_ticks) {
      time_ptr->SECONDS      = MAX_UINT_32;
      time_ptr->MILLISECONDS = 999;
      return FALSE;
   } /* Endif */

   /* This is guaranteed to work since ticks/tps is less than MAX_UINT_32 */
   if (evenly_divisible) {
      tmp = tick_ptr->TICKS[0] * tpms;
   } else {
      tmp = (tick_ptr->TICKS[0] * 1000) / tps;
   } /* Endif */

   time_ptr->SECONDS      = tmp / 1000;
   time_ptr->MILLISECONDS = tmp - time_ptr->SECONDS * 1000;

   tmp =  (uint_64)tick_ptr->HW_TICKS[0] * 1000 * 2;
   if (tmp > MAX_UINT_32) {
      tmp =  tmp / hw_ticks_per_sec;
      tmp += 1;
      tmp >>= 1;

      time_ptr->MILLISECONDS += (uint_32)tmp;
   } else {
      uint_32 tmp32 = tmp;
      tmp32 = tmp32 / tps / kernel_data->HW_TICKS_PER_TICK;
      tmp32 += 1;
      tmp32 >>= 1;
      time_ptr->MILLISECONDS += tmp32;
   } /* Endif */

   if (time_ptr->MILLISECONDS >= 1000) {
      time_ptr->SECONDS++;
      time_ptr->MILLISECONDS -= 1000;
   } /* Endif */

   return TRUE;

} /* Endbody */
#endif
#endif

/* End CR 1256 */

/* EOF */
