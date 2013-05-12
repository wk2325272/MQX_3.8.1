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
* $FileName: ti_delfo.c$
* $Version : 3.5.3.0$
* $Date    : Dec-8-2009$
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
 * Function Name    : _time_delay_for
 * Returned Value   : void
 * Comments         :
 *   This function puts a task on the timeout queue for the specified number
 * of ticks, or until removed by another task.
 *
 *END*----------------------------------------------------------------------*/

void _time_delay_for
   (
      /* [IN] the number of ticks to delay */
      register MQX_TICK_STRUCT_PTR ticks
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_time_delay_for, ticks);

#if MQX_CHECK_ERRORS
   if (ticks == NULL) {
      _task_set_error(MQX_INVALID_PARAMETER);
      _KLOGX2(KLOG_time_delay_for, MQX_INVALID_PARAMETER);
      return;
   } /* Endif */
#endif

   td_ptr = kernel_data->ACTIVE_PTR;

   _INT_DISABLE();

   /* Calculate time to wake up the task */
   PSP_ADD_TICKS(ticks, &kernel_data->TIME, &td_ptr->TIMEOUT);

   _time_delay_internal(td_ptr);

   _INT_ENABLE();

   _KLOGX1(KLOG_time_delay_for);

} /* Endbody */

#endif

/* EOF */
