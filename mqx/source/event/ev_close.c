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
* $FileName: ev_close.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for closing an event.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_EVENTS
#include "name.h"
#include "name_prv.h"
#include "event.h"
#include "evnt_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _event_close
* Returned Value   : 
*   Returns MQX_OK upon success, or a MQX error code:
* Comments         :
*    Used by a task to close (relinquish access to) an event instance.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _event_close
   (
      /* [IN] - An event handle returned from a call to _event_open() or
      ** _event_open_fast()
      */
      pointer users_event_ptr 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   register EVENT_CONNECTION_STRUCT_PTR event_connection_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_event_close, users_event_ptr);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_event_close, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   event_connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)users_event_ptr;
#if MQX_CHECK_VALIDITY
   if (event_connection_ptr->VALID != EVENT_VALID) {
      _KLOGX2(KLOG_event_close, EVENT_INVALID_EVENT_HANDLE);
      return(EVENT_INVALID_EVENT_HANDLE); 
   } /* Endif */

#endif

   event_connection_ptr->VALID = 0;

   if (_mem_free(event_connection_ptr) != MQX_OK) {
      _KLOGX2(KLOG_event_close, kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
      return(kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
   } else {
      _KLOGX2(KLOG_event_close, MQX_OK);
      return(MQX_OK);
   } /* Endif */

} /* Endbody */
#endif /* MQX_USE_EVENTS */

/* EOF */
