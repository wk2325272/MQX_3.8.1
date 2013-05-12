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
* $FileName: ti_nxd.c$
* $Version : 3.6.5.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the function for normalizing an xdate struct
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _time_normalize_xdate
* Returned Value  : boolean
* Comments        : Normalizes an xdate struct. Will return FALSE if
*   illegal values are encountered.
*                   
*
*END*------------------------------------------------------------------*/

boolean _time_normalize_xdate
   (

     /*  [IN]  pointer to date and time structure */
     MQX_XDATE_STRUCT_PTR  xdate_ptr

   )
{ /* Body */
   _mqx_uint leap;

   if (xdate_ptr->YEAR < (_mqx_uint)XCLK_FIRST_YEAR) {
      return FALSE;
   } /* Endif */

   if (xdate_ptr->PSEC >= PICOSECS_IN_NANOSECOND) {
      xdate_ptr->NSEC += xdate_ptr->PSEC / PICOSECS_IN_NANOSECOND;
      xdate_ptr->PSEC  = xdate_ptr->PSEC % PICOSECS_IN_NANOSECOND;
   } /* Endif */

   if (xdate_ptr->NSEC >= NANOSECS_IN_MICROSECOND) {
      xdate_ptr->USEC += xdate_ptr->NSEC / NANOSECS_IN_MICROSECOND;
      xdate_ptr->NSEC  = xdate_ptr->NSEC % NANOSECS_IN_MICROSECOND;
   } /* Endif */

   if (xdate_ptr->USEC >= MICROSECS_IN_MILLISECOND) {
      xdate_ptr->MSEC += xdate_ptr->USEC / MICROSECS_IN_MILLISECOND;
      xdate_ptr->USEC  = xdate_ptr->USEC % MICROSECS_IN_MILLISECOND;
   } /* Endif */

   if (xdate_ptr->MSEC >= MILLISECS_IN_SECOND) {
      xdate_ptr->SEC  += xdate_ptr->MSEC / MILLISECS_IN_SECOND;
      xdate_ptr->MSEC  = xdate_ptr->MSEC % MILLISECS_IN_SECOND;
   } /* Endif */

   if (xdate_ptr->SEC >= SECS_IN_MINUTE) {
      xdate_ptr->MIN += xdate_ptr->SEC / SECS_IN_MINUTE;
      xdate_ptr->SEC  = xdate_ptr->SEC % SECS_IN_MINUTE;
   } /* Endif */

   if (xdate_ptr->MIN >= MINUTES_IN_HOUR) {
      xdate_ptr->HOUR += xdate_ptr->MIN / MINUTES_IN_HOUR;
      xdate_ptr->MIN   = xdate_ptr->MIN % MINUTES_IN_HOUR;
   } /* Endif */

   if (xdate_ptr->HOUR >= HOURS_IN_DAY) {
      xdate_ptr->MDAY += xdate_ptr->HOUR / HOURS_IN_DAY;
      xdate_ptr->HOUR  = xdate_ptr->HOUR % HOURS_IN_DAY;
   } /* Endif */

   if (xdate_ptr->MONTH > MONTHS_IN_YEAR) {
      /* Months range from 1 to 12 */
      xdate_ptr->YEAR  += (xdate_ptr->MONTH-1) / MONTHS_IN_YEAR;
      xdate_ptr->MONTH  = (xdate_ptr->MONTH-1) % MONTHS_IN_YEAR + 1;
   } /* Endif */

   while (1) {

      leap = (_mqx_uint)_time_check_if_leap(xdate_ptr->YEAR);

      if (xdate_ptr->MDAY > _time_days_in_month_internal[leap][xdate_ptr->MONTH]) {        
         xdate_ptr->MDAY -= _time_days_in_month_internal[leap][xdate_ptr->MONTH];
         xdate_ptr->MONTH++;
      } else {
         break;
      } /* Endif */

      if (xdate_ptr->MONTH > MONTHS_IN_YEAR) {
         xdate_ptr->YEAR  +=  (xdate_ptr->MONTH / MONTHS_IN_YEAR);
         xdate_ptr->MONTH  =  (xdate_ptr->MONTH % MONTHS_IN_YEAR);
      } /* Endif */

   } /* Endwhile */

   if (xdate_ptr->YEAR > XCLK_LAST_YEAR) {
      return( FALSE );
   } /* Endif */
   
   return TRUE;

} /* Endbody */

/* EOF */
