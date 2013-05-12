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
* $FileName: lwe_waiu.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the functions for waiting for a light weight event.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWEVENTS
#include "lwevent.h"
#include "lwe_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _lwevent_wait_until
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to wait for a specified event.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _lwevent_wait_until
   (
      /* [IN] - The address of the light weight event */
      LWEVENT_STRUCT_PTR   event_ptr, 

      /* [IN] - bit mask, each bit of which represents an event. */
      _mqx_uint            bit_mask, 

      /* [IN] - boolean, wait for all bits or just any bits */
      boolean              all,

      /* [IN] - The time in ticks to wait for the events 
      **        to be set.  If the value is NULL, then the timeout will be 
      **        infinite.
      */
      MQX_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR  kernel_data;)
   _mqx_uint                      result;

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwevent_wait_until(event_ptr, bit_mask, all, tick_ptr);
    }
#endif
    
   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE5(KLOG_lwevent_wait_until, event_ptr, bit_mask, all, tick_ptr);

   result = _lwevent_wait_internal(event_ptr, bit_mask, all, 
      tick_ptr, TRUE);

   _KLOGX2(KLOG_lwevent_wait_until, result);
   return(result);

} /* Endbody */

#if MQX_ENABLE_USER_MODE

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _usr_lwevent_wait_until
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to wait for a specified event.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _usr_lwevent_wait_until
    (
        /* [IN] - The address of the light weight event */
        LWEVENT_STRUCT_PTR   event_ptr, 

        /* [IN] - bit mask, each bit of which represents an event. */
        _mqx_uint            bit_mask, 

        /* [IN] - boolean, wait for all bits or just any bits */
        boolean              all,

        /* [IN] - The time in ticks to wait for the events 
        **        to be set.  If the value is NULL, then the timeout will be 
        **        infinite.
        */
        MQX_TICK_STRUCT_PTR tick_ptr
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)event_ptr, (uint_32)bit_mask, (uint_32)all, (uint_32)tick_ptr, 0};
    return _mqx_api_call(MQX_API_LWEVENT_WAIT_UNTIL, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWEVENTS */

/* EOF */
