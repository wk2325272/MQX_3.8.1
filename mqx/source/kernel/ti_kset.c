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
* $FileName: ti_kset.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for setting the current time.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_set
* Returned Value   : void
* Comments         :
*   This function sets the current time on the system.  The input time is
* the UCT time.
*
*END*----------------------------------------------------------------------*/

void _time_set
   (
      /* [IN] the address where the new time is to be taken from */
      register TIME_STRUCT_PTR time_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   MQX_TICK_STRUCT                 ticks;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_time_set, time_ptr, time_ptr->SECONDS, time_ptr->MILLISECONDS);

   /* Normalize time */
   MQX_NORMALIZE_TIME_STRUCT(time_ptr);

   /* First convert old time struct into the tick struct */
   PSP_TIME_TO_TICKS(time_ptr, &ticks);

   _INT_DISABLE();

   /* Calculate offset */
   PSP_SUB_TICKS(&ticks, &kernel_data->TIME, &kernel_data->TIME_OFFSET);

   _INT_ENABLE();

   _KLOGX1(KLOG_time_set);

} /* Endbody */
#endif
/* EOF */
