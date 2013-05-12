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
* $FileName: ti_delay.c$
* $Version : 3.8.6.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the function for delaying a task for the 
*   specified number of milliseconds.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _time_delay
 * Returned Value   : void
 * Comments         :
 *   This function puts a task on the timeout queue for the specified number
 * of milliseconds, or until removed by another task.
 *
 *END*----------------------------------------------------------------------*/

void _time_delay
   (
      /* [IN] the number of milliseconds to delay */
      register uint_32 milliseconds
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td_ptr;
   /* Start CR 330 */
   /*         TIME_STRUCT             time; */
   /* End CR 330 */

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        _usr_time_delay(milliseconds);
        return;
    }
#endif
    
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_time_delay, milliseconds);

   if ( ! milliseconds ) {
      _KLOGX1(KLOG_time_delay);
      return;
   } /* Endif */

   /* Start CR 330 */
   /* MQX_TIME_NORMALIZE(0, milliseconds, time.SECONDS, time.MILLISECONDS); */
   /* End CR 330 */

   td_ptr = kernel_data->ACTIVE_PTR;

   /* Convert milliseconds to ticks */
   /* Start CR 330 */
   /* _INT_DISABLE(); */
   
   /* PSP_TIME_TO_TICKS(&time, &td_ptr->TIMEOUT); */
   PSP_MILLISECONDS_TO_TICKS_QUICK(milliseconds, &td_ptr->TIMEOUT);

   _INT_DISABLE();
   /* End CR 330 */

   /* Calculate time to wake up the task */
   PSP_ADD_TICKS(&td_ptr->TIMEOUT, &kernel_data->TIME, &td_ptr->TIMEOUT);

   _time_delay_internal(td_ptr);

   _INT_ENABLE();

   _KLOGX1(KLOG_time_delay);

} /* Endbody */

#if MQX_ENABLE_USER_MODE
/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _usr_time_delay
 * Returned Value   : void
 * Comments         :
 *   This function puts a task on the timeout queue for the specified number
 * of milliseconds, or until removed by another task.
 *
 *END*----------------------------------------------------------------------*/

void _usr_time_delay
   (
      /* [IN] the number of ticks to delay */
      register uint_32 milliseconds
   )
{
    MQX_API_CALL_PARAMS params = {milliseconds, 0, 0, 0, 0};
    _mqx_api_call(MQX_API_TIME_DELAY, &params);
}

#endif // MQX_ENABLE_USER_MODE
#endif // MQX_HAS_TICK
