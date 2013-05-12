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
* $FileName: ti_deli.c$
* $Version : 3.5.7.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for delaying the specified task. It is
*   assumed that the timeout field in the TD has already been set by the API
*   functions.
*
*END************************************************************************/

#include "mqx_inc.h"
#include "timer.h"
#include "tim_prv.h"

#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _time_delay_internal
 * Returned Value   : void
 * Comments         :
 *   This function puts a task on the timeout queue for the specified number
 * of ticks, or until removed by another task. Must be called int disabled.
 *
 *END*----------------------------------------------------------------------*/

void _time_delay_internal
   (
      /* [IN] the task to delay */
      register TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register TD_STRUCT_PTR           td2_ptr;
   register TD_STRUCT_PTR           tdprev_ptr;
   register _mqx_uint               count;
   register _mqx_int                result;
   
   _GET_KERNEL_DATA(kernel_data);

   /* Remove task from ready to run queue */
   tdprev_ptr = (TD_STRUCT_PTR)((pointer)&kernel_data->TIMEOUT_QUEUE);
   if ( _QUEUE_GET_SIZE(&kernel_data->TIMEOUT_QUEUE) ) {

      /* Perform insertion sort by time */
      td2_ptr    = (TD_STRUCT_PTR)((pointer)kernel_data->TIMEOUT_QUEUE.NEXT);

      /* SPR P171-0023-01 use pre-decrement on while loop */
      count      = _QUEUE_GET_SIZE(&kernel_data->TIMEOUT_QUEUE) + 1;
      while ( --count ) {
      /* END SPR */
         result = PSP_CMP_TICKS(&td2_ptr->TIMEOUT, &td_ptr->TIMEOUT);
         if (MQX_DELAY_ENQUEUE_POLICY(result)) { /* CR171 */
            /* Enqueue before td2_ptr */
            break;
         } /* Endif */

         tdprev_ptr = td2_ptr;
         td2_ptr    = td2_ptr->TD_NEXT;
      } /* Endwhile */

   } /* Endif */

   /* Remove from ready queue */
   _QUEUE_UNLINK(td_ptr);

   /* Insert into timeout queue */
   _QUEUE_INSERT(&kernel_data->TIMEOUT_QUEUE,tdprev_ptr,td_ptr);

   td_ptr->STATE |= IS_ON_TIMEOUT_Q;

   _sched_execute_scheduler_internal();

} /* Endbody */
#endif

/* EOF */
