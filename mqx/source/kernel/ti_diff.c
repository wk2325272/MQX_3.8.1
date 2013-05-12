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
* $FileName: ti_diff.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for calculating the difference
*   between two times.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_diff
* Returned Value   : void
* Comments         :
*    This function calculates the difference between two times.
*
*END*----------------------------------------------------------------------*/

void _time_diff
   (
      /* [IN] the starting time */
      TIME_STRUCT_PTR start_time_ptr,

      /* [IN] the ending time */
      TIME_STRUCT_PTR end_time_ptr,

      /* [OUT] the difference in time */
      TIME_STRUCT_PTR diff_time_ptr
   )
{ /* Body */
   TIME_STRUCT temp;

   /* 
   ** Use temporary variable in case diff_time_ptr is the
   ** same as either start or end pointers
   */
   temp.SECONDS      = end_time_ptr->SECONDS;
   temp.MILLISECONDS = end_time_ptr->MILLISECONDS;
   if (temp.MILLISECONDS < start_time_ptr->MILLISECONDS) {
      temp.MILLISECONDS += 1000;
      temp.SECONDS--;
   } /* Endif */
   temp.SECONDS      -= start_time_ptr->SECONDS;
   temp.MILLISECONDS -= start_time_ptr->MILLISECONDS;

   if (temp.MILLISECONDS > 1000) {
      temp.SECONDS      += (temp.MILLISECONDS / 1000);
      temp.MILLISECONDS = temp.MILLISECONDS % 1000;
   } /* Endif */

   *diff_time_ptr = temp;

} /* Endbody */

/* EOF */
