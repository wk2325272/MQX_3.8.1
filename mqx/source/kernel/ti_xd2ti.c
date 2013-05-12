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
* $FileName: ti_xd2ti.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for converting from a xdate struct
*   to a tick struct.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TICK

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _time_xdate_to_ticks
* Returned Value  : boolean
* Comments        : converts from the xdate structure into ticks
*                   since 1970.
*
*END*------------------------------------------------------------------*/


boolean _time_xdate_to_ticks
   (

     /* [IN]  pointer to date and time structure */
     MQX_XDATE_STRUCT_PTR  xdate_ptr,

     /* [OUT]  pointer to structure where num of ticks is to be written */   
     MQX_TICK_STRUCT_PTR   tick_ptr

   )
{ /* Body */
   uint_32                time;
   _mqx_uint              tmp;
   MQX_TICK_STRUCT        t2;
   KERNEL_DATA_STRUCT_PTR kernel_data;

#if MQX_CHECK_ERRORS
   if ((xdate_ptr == NULL) || (tick_ptr == NULL)) {
      return FALSE;
   } /* Endif */
#endif

   if (!_time_normalize_xdate(xdate_ptr)) {
      return FALSE;
   } /* Endif */

   /*
   ** Determine the number of days since Jan 1, 1970 at 00:00:00
   */
   time = _time_days_before_year_internal[(xdate_ptr->YEAR - 
      (uint_16)XCLK_FIRST_YEAR)];

   tmp = (_mqx_uint)_time_check_if_leap(xdate_ptr->YEAR);

   /*
   ** Add the number of days since 0000 hours, Jan 1, to the first
   ** day of month.
   */
   time += _time_days_before_month_internal[tmp][xdate_ptr->MONTH-1];

   /*
   ** Add the number of days since the beginning of the
   ** month
   */
   time += (uint_32)(xdate_ptr->MDAY - 1);

   /* Convert everything so far into hours */
   time = time * HOURS_IN_DAY + xdate_ptr->HOUR;

   /* Convert to seconds */
   tick_ptr->HW_TICKS = 0;
   t2 = _mqx_zero_tick_struct;
   PSP_ADD_TICKS_TO_TICK_STRUCT(&t2, SECS_IN_HOUR, &t2);
   PSP_MULTIPLY_TICKS_BY_UINT_32(&t2, time, tick_ptr);

   /* Add in seconds and minutes from XDATE struct */
   tmp = xdate_ptr->SEC + xdate_ptr->MIN * SECS_IN_MINUTE;
   PSP_ADD_TICKS_TO_TICK_STRUCT(tick_ptr, tmp, tick_ptr);

   /* Convert everything into ticks */
   _GET_KERNEL_DATA(kernel_data);
   time = (uint_32)kernel_data->TICKS_PER_SECOND;
   PSP_MULTIPLY_TICKS_BY_UINT_32(tick_ptr, time, tick_ptr);

   if (xdate_ptr->MSEC) {
      PSP_MILLISECONDS_TO_TICKS(xdate_ptr->MSEC, &t2);
      PSP_ADD_TICKS(tick_ptr, &t2, tick_ptr);
   } /* Endif */

   if (xdate_ptr->USEC) {
      PSP_MICROSECONDS_TO_TICKS(xdate_ptr->USEC, &t2);
      PSP_ADD_TICKS(tick_ptr, &t2, tick_ptr);
   } /* Endif */

   if (xdate_ptr->NSEC) {
      PSP_NANOSECONDS_TO_TICKS(xdate_ptr->NSEC, &t2);
      PSP_ADD_TICKS(tick_ptr, &t2, tick_ptr);
   } /* Endif */

   if (xdate_ptr->PSEC) {
      PSP_PICOSECONDS_TO_TICKS(xdate_ptr->PSEC, &t2);
      PSP_ADD_TICKS(tick_ptr, &t2, tick_ptr);
   } /* Endif */

   return TRUE;

} /* Endbody */
#endif

/* EOF */
