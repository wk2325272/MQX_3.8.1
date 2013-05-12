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
* $FileName: lwe_crea.c$
* $Version : 3.8.6.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the functions for creating a light weight event.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWEVENTS
#include "lwevent.h"
#include "lwe_prv.h"


_mqx_uint _lwevent_create_internal
   (
      /* [IN] the location of the event */
      LWEVENT_STRUCT_PTR event_ptr,
      
      /* [IN] flags for the light weight event */
      _mqx_uint          flags,
      
      boolean           user
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
/* START CR 1896 */
   LWEVENT_STRUCT_PTR     event_chk_ptr;
/* END CR 1896 */
   
#if MQX_ENABLE_USER_MODE
   if (user && !_psp_mem_check_access_mask((uint_32)event_ptr, sizeof(LWEVENT_STRUCT), MPU_UM_R, MPU_UM_RW)) {
       return MQX_LWEVENT_INVALID;
   }
#endif
   
   _GET_KERNEL_DATA(kernel_data);                                         

   _KLOGE2(KLOG_lwevent_create, event_ptr);

   _QUEUE_INIT(&event_ptr->WAITING_TASKS, 0);
   event_ptr->VALUE = 0;
   event_ptr->FLAGS = flags;

   if (flags & LWEVENT_AUTO_CLEAR)
      event_ptr->AUTO = ~0;
   else
      event_ptr->AUTO = 0;

   _int_disable();
   
#if MQX_ENABLE_USER_MODE
   if (user) {
      if (kernel_data->USR_LWEVENTS.NEXT == NULL) {
         /* Initialize the light weight event queue */
         _QUEUE_INIT(&kernel_data->USR_LWEVENTS, 0);
      }
   }
   else 
#endif
   {
      if (kernel_data->LWEVENTS.NEXT == NULL) {
         /* Initialize the light weight event queue */
         _QUEUE_INIT(&kernel_data->LWEVENTS, 0);
      }
   }
   
   event_ptr->VALID = LWEVENT_VALID;

/* START CR 1896 */
#if MQX_CHECK_ERRORS
   /* Check if lwevent is already initialized */
#if MQX_ENABLE_USER_MODE
    if (user) {
        event_chk_ptr = (LWEVENT_STRUCT_PTR)((pointer)kernel_data->USR_LWEVENTS.NEXT);
        while (event_chk_ptr != (LWEVENT_STRUCT_PTR)((pointer)&kernel_data->USR_LWEVENTS)) {
            if (event_chk_ptr == event_ptr) {
                _int_enable();
                _KLOGX2(KLOG_lwevent_create, MQX_EINVAL);
                return(MQX_EINVAL);
            }
            event_chk_ptr = (LWEVENT_STRUCT_PTR)((pointer)event_chk_ptr->LINK.NEXT);
        }
    }
    else
#endif
    {
        event_chk_ptr = (LWEVENT_STRUCT_PTR)((pointer)kernel_data->LWEVENTS.NEXT);
        while (event_chk_ptr != (LWEVENT_STRUCT_PTR)((pointer)&kernel_data->LWEVENTS)) {
            if (event_chk_ptr == event_ptr) {
                _int_enable();
                _KLOGX2(KLOG_lwevent_create, MQX_EINVAL);
                return(MQX_EINVAL);
            }
            event_chk_ptr = (LWEVENT_STRUCT_PTR)((pointer)event_chk_ptr->LINK.NEXT);
        }
    }
#endif
    
#if MQX_ENABLE_USER_MODE
    if (user) {
        _QUEUE_ENQUEUE(&kernel_data->USR_LWEVENTS, &event_ptr->LINK);
    }
    else 
#endif
    {
        _QUEUE_ENQUEUE(&kernel_data->LWEVENTS, &event_ptr->LINK);
    }
    
   _int_enable();

   _KLOGX2(KLOG_lwevent_create, MQX_OK);
   return(MQX_OK);
}

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _lwevent_create
* Returned Value   : 
*   Returns MQX_OK upon success, or an error code
* Comments         :
*    Used by a task to create an instance of an light weight event.
*
* 
*END*------------------------------------------------------------------*/

_mqx_uint _lwevent_create
   (
      /* [IN] the location of the event */
      LWEVENT_STRUCT_PTR event_ptr,
      
      /* [IN] flags for the light weight event */
      _mqx_uint          flags
   )
{
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwevent_create(event_ptr, flags);
    }
#endif
    
    return _lwevent_create_internal(event_ptr, flags, FALSE);
}


/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _usr_lwevent_create
* Returned Value   : 
*   Returns MQX_OK upon success, or an error code
* Comments         :
*    Used by a task to create an instance of an light weight event.
*
* 
*END*------------------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

_mqx_uint _usr_lwevent_create
   (
      /* [IN] the location of the event */
      LWEVENT_STRUCT_PTR event_ptr,
      
      /* [IN] flags for the light weight event */
      _mqx_uint          flags
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)event_ptr, (uint_32)flags, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWEVENT_CREATE, &params);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWEVENTS */

/* EOF */
