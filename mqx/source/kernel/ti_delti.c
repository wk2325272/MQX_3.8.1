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
* $FileName: ti_delti.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for delaying a task for the 
*   specified number of ticks.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _time_delay_ticks
 * Returned Value   : void
 * Comments         :
 *   This function puts a task on the timeout queue for the specified number
 * of ticks, or until removed by another task.
 *
 *END*----------------------------------------------------------------------*/

void _time_delay_ticks
   (
      /* [IN] the number of ticks to delay */
      register _mqx_uint time_in_ticks
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td_ptr;

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        _usr_time_delay_ticks(time_in_ticks);
        return;
    }
#endif
    
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_time_delay_ticks, time_in_ticks);

   if ( !time_in_ticks ) {
      _KLOGX1(KLOG_time_delay_ticks);
      return;
   } /* Endif */

   td_ptr = kernel_data->ACTIVE_PTR;

   _INT_DISABLE();

   PSP_ADD_TICKS_TO_TICK_STRUCT(&kernel_data->TIME, time_in_ticks, &td_ptr->TIMEOUT);

   _time_delay_internal(td_ptr);

   _INT_ENABLE();
   _KLOGX1(KLOG_time_delay_ticks);

}

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _usr_time_delay_ticks
 * Returned Value   : void
 * Comments         :
 *   This function puts a task on the timeout queue for the specified number
 * of ticks, or until removed by another task.
 *
 *END*----------------------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

void _usr_time_delay_ticks
   (
      /* [IN] the number of ticks to delay */
      register _mqx_uint time_in_ticks
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)time_in_ticks, 0, 0, 0, 0};
    _mqx_api_call(MQX_API_TIME_DELAY_TICKS, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_HAS_TICK */
