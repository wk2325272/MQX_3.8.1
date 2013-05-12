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
* $FileName: ev_comp.c$
* $Version : 3.8.5.1$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for creating the event component
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
* Function Name    : _event_create_component
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to install the event component into the kernel so that
* other tasks may create and use events.
*
*END*------------------------------------------------------------------*/

_mqx_uint _event_create_component
   (
      /* [IN] the initial number of event */
      _mqx_uint initial_number, 

      /* 
      ** [IN] the number of events to add when
      ** space is no longer available
      */
      _mqx_uint grow_number,

      /* [IN] the maximum number of events allowed */
      _mqx_uint maximum_number 
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR     kernel_data;
   register EVENT_COMPONENT_STRUCT_PTR event_component_ptr;
            _mqx_uint                   result;
            
   _GET_KERNEL_DATA(kernel_data);            

   _KLOGE4(KLOG_event_create_component, initial_number, grow_number, maximum_number);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_event_create_component, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   _lwsem_wait((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));

   event_component_ptr = (EVENT_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_EVENTS];
   if (event_component_ptr != NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _KLOGX2(KLOG_event_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */
   
   event_component_ptr = (EVENT_COMPONENT_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(EVENT_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (event_component_ptr == NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _KLOGX2(KLOG_event_create_component, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(event_component_ptr, MEM_TYPE_EVENT_COMPONENT);

   result = _name_create_handle_internal(&event_component_ptr->NAME_TABLE_HANDLE,
      initial_number, grow_number, maximum_number, initial_number);
#if MQX_CHECK_ERRORS
   if (result != MQX_OK) {
      _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
      _mem_free(event_component_ptr);
      _KLOGX2(KLOG_event_create_component, result);
      return(result);
   } /* Endif */
#endif
   event_component_ptr->VALID             = EVENT_VALID;
   event_component_ptr->GROW_NUMBER       = grow_number;
   if (maximum_number == 0) {
      event_component_ptr->MAXIMUM_NUMBER = MAX_MQX_UINT;
   } else if (maximum_number < initial_number) {
      event_component_ptr->MAXIMUM_NUMBER = initial_number;
   } else {
      event_component_ptr->MAXIMUM_NUMBER = maximum_number;
   } /* Endif */

   kernel_data->KERNEL_COMPONENTS[KERNEL_EVENTS] = event_component_ptr;

#if MQX_COMPONENT_DESTRUCTION
   kernel_data->COMPONENT_CLEANUP[KERNEL_EVENTS] = _event_cleanup;
#endif

   _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->COMPONENT_CREATE_LWSEM));
   
   _KLOGX2(KLOG_event_create_component, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_EVENTS */

/* EOF */
