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
* $FileName: ti_leap.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for determining if the given
*   year is a leap year.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _time_check_if_leap
* Returned Value  : boolean - TRUE if a leap year
* Comments        : Determines if the given year is a leap year
*
*END*------------------------------------------------------------------*/

boolean _time_check_if_leap
   (

     /*  [IN]  The year to check */
     uint_16   year

   )
{ /* Body */
   boolean  leap;

   /*
   ** If the year is a century year not divisible by 400
   ** then it is not a leap year, otherwise if year divisible by
   ** four then it is a leap year
   */
   if (year % (uint_16)100 == (uint_16)0) {

      if (year % (uint_16)400 == (uint_16)0) {
         leap = TRUE;
      } else {
         leap = FALSE;
      } /* Endif */
         
   } else {

      if (year % (uint_16)4 == (uint_16)0) {
         leap = TRUE;
      } else {
         leap = FALSE;
      } /* Endif */

   } /* Endif */

   return leap;
} /* Endbody */

/* EOF */
