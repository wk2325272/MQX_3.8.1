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
* $FileName: ev_open.c$
* $Version : 3.0.7.0$
* $Date    : Nov-27-2008$
*
* Comments:
*
*   This file contains the function for opening an event.
*
*END************************************************************************/

#include <string.h>
#include "mqx_inc.h"
#if MQX_USE_EVENTS
#include "name.h"
#include "name_prv.h"
#include "event.h"
#include "evnt_prv.h"
#include "message.h"
#include "msg_prv.h"
#include "ipc.h"
#include "ipc_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _event_open
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to open (gain access to) an event instance.
*
* 
*END*------------------------------------------------------------------*/
 
_mqx_uint _event_open
   (
      /* [IN] the string name of the event */
      char _PTR_ name_ptr,

      /* [IN/OUT] - the address where the event handle is to be 
      ** written
      */
      pointer _PTR_ returned_event_ptr
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
   register EVENT_COMPONENT_STRUCT_PTR  event_component_ptr;
            EVENT_STRUCT_PTR            event_ptr;
   register EVENT_CONNECTION_STRUCT_PTR event_connection_ptr;
            _mqx_uint                   result;
            char _PTR_                  parse_ptr;
            uint_32                     processor;
            _mqx_max_type               tmp;

   _GET_KERNEL_DATA(kernel_data);                                         

   _KLOGE3(KLOG_event_open, name_ptr, returned_event_ptr);
   
   *returned_event_ptr = NULL;
   
   event_component_ptr = kernel_data->KERNEL_COMPONENTS[KERNEL_EVENTS];
#if MQX_CHECK_ERRORS
   if (event_component_ptr == NULL){
      _KLOGX2(KLOG_event_open, MQX_COMPONENT_DOES_NOT_EXIST);
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (event_component_ptr->VALID != EVENT_VALID) {
      _KLOGX2(KLOG_event_open, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   /* If lead by a number, this is a processor number */
   processor = 0;
   parse_ptr = name_ptr;
   while (*parse_ptr && (*parse_ptr >= '0') && (*parse_ptr <= '9')) {
      processor = (processor * 10) + *parse_ptr - (char)'0';
      parse_ptr++;
   } /* Endwhile */
   if (processor) {
      /* Processor number must be followed by ':' */
      if (*parse_ptr == ':') {
         parse_ptr++;
      } else {
         processor = 0;
      } /* Endif */
   } /* Endif */

   event_connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)
      _mem_alloc_zero((_mem_size)sizeof(EVENT_CONNECTION_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (event_connection_ptr == NULL) {
      _KLOGX2(KLOG_event_open, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(event_connection_ptr, MEM_TYPE_EVENT_CONNECTION);
  
   if (processor != 0) {

#if MQX_IS_MULTI_PROCESSOR
      if (kernel_data->IPC) {
         /* This open is for a remote processor */
         result = (*kernel_data->IPC)(TRUE, processor, KERNEL_EVENTS, 
            IPC_EVENT_OPEN | IPC_STRING_PARAMETER, 1, parse_ptr);
         if (result != MQX_OK) {
            _mem_free(event_connection_ptr);
            _KLOGX2(KLOG_event_open, result);
            return(result);
         } /* Endif */
         event_connection_ptr->EVENT_PTR = (pointer)
            kernel_data->ACTIVE_PTR->INFO;
         event_connection_ptr->REMOTE_CPU = processor;
      } else {
#endif
         _mem_free(event_connection_ptr);
         _KLOGX2(KLOG_event_open, EVENT_NOT_FOUND);
         return(EVENT_NOT_FOUND);
#if MQX_IS_MULTI_PROCESSOR
      }/* Endif */
#endif
      
   } else {
   
      result = _name_find_internal(event_component_ptr->NAME_TABLE_HANDLE, 
         name_ptr, &tmp);

#if MQX_CHECK_ERRORS
      if (result != MQX_OK) {
         _mem_free(event_connection_ptr);
         if (result == NAME_NOT_FOUND) {
            _KLOGX2(KLOG_event_open, EVENT_NOT_FOUND);
            return(EVENT_NOT_FOUND);
         } /* Endif */
         _KLOGX2(KLOG_event_open, result);
         return(result);
      } /* Endif */
#endif

      event_ptr = (EVENT_STRUCT_PTR)tmp;

#if MQX_CHECK_VALIDITY
      if (event_ptr->VALID != EVENT_VALID) {
         /* Event not valid */
         _mem_free(event_connection_ptr);
         _KLOGX2(KLOG_event_open, EVENT_INVALID_EVENT);
         return(EVENT_INVALID_EVENT);
      } /* Endif */
#endif

      event_connection_ptr->EVENT_PTR = event_ptr;

   }/* Endif */

   event_connection_ptr->VALID     = EVENT_VALID;
   event_connection_ptr->TD_PTR    = kernel_data->ACTIVE_PTR;
   *returned_event_ptr = (pointer)event_connection_ptr;

   _KLOGX3(KLOG_event_open, MQX_OK, event_connection_ptr);
   return(MQX_OK);
   
} /* Endbody */   
#endif /* MQX_USE_EVENTS */

/* EOF */
