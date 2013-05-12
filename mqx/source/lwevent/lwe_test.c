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
* $FileName: lwe_test.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for testing the light weight events.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWEVENTS
#include "lwevent.h"
#include "lwe_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _lwevent_test
* Returned Value   :  _mqx_uint MQX_OK or a MQX error code
* Comments         :
*    This function tests the event component for validity and consistency.
* 
*END*------------------------------------------------------------------*/

_mqx_uint _lwevent_test
   (
      /* [OUT] the event in error */
      pointer _PTR_ event_error_ptr,

      /* [OUT] the td on a light weight event in error */
      pointer _PTR_ td_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   LWEVENT_STRUCT_PTR       event_ptr;
   _mqx_uint                result;
   _mqx_uint                queue_size;

   _GET_KERNEL_DATA(kernel_data);                                         

   _KLOGE2(KLOG_lwevent_test, event_error_ptr);

   *td_error_ptr    = NULL;
   *event_error_ptr = NULL;

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_lwevent_test, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   }/* Endif */
#endif

   /* 
   ** It is not considered an error if the lwevent component has not been
   ** created yet
   */
   if (kernel_data->LWEVENTS.NEXT == NULL) {
      return(MQX_OK);
   } /* Endif */


   result = _queue_test((QUEUE_STRUCT_PTR)&kernel_data->LWEVENTS, 
      event_error_ptr);
   if (result != MQX_OK) {
      _KLOGX3(KLOG_lwevent_test, result, *event_error_ptr);
      return(result);
   } /* Endif */

   event_ptr = (LWEVENT_STRUCT_PTR)((pointer)kernel_data->LWEVENTS.NEXT);
   queue_size = _QUEUE_GET_SIZE(&kernel_data->LWEVENTS);
   while (queue_size--) {
      if (event_ptr->VALID != LWEVENT_VALID) {
         result = MQX_LWEVENT_INVALID;
         break;
      } /* Endif */
      result = _queue_test(&event_ptr->WAITING_TASKS, td_error_ptr);
      if (result != MQX_OK) {
         break;
      } /* Endif */
      event_ptr = (LWEVENT_STRUCT_PTR)(pointer)event_ptr->LINK.NEXT;
   } /* Endwhile */

   _int_enable();

   if (result != MQX_OK) {
      *event_error_ptr = (pointer)event_ptr;
   } /* Endif */
   _KLOGX4(KLOG_lwevent_test, result, *event_error_ptr, *td_error_ptr);
   return(result);

}

#if MQX_ENABLE_USER_MODE

_mqx_uint _lwevent_usr_check
    (
        /* [IN] the location of the event */
        LWEVENT_STRUCT_PTR tst_event_ptr
    )
{
    KERNEL_DATA_STRUCT_PTR kernel_data;
    LWEVENT_STRUCT_PTR event_ptr;
    _mqx_uint result = MQX_LWEVENT_INVALID;
    _mqx_uint queue_size;

    _GET_KERNEL_DATA(kernel_data);                                         

    event_ptr = (LWEVENT_STRUCT_PTR)((pointer)kernel_data->USR_LWEVENTS.NEXT);
    queue_size = _QUEUE_GET_SIZE(&kernel_data->USR_LWEVENTS);
    
    while (queue_size--) {
        if (event_ptr->VALID != LWEVENT_VALID) {
            break;
        }
        
        if (tst_event_ptr == event_ptr) {
            result = MQX_OK;
            break;
        }
        
        event_ptr = (LWEVENT_STRUCT_PTR)(pointer)event_ptr->LINK.NEXT;
    }
    
    return result;
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWEVENTS */

