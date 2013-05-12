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
* $FileName: ms_comp.c$
* $Version : 3.8.5.1$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for creating the message component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msg_create_component
* Returned Value  : MQX error code
* Comments        :
*
*END*------------------------------------------------------------------*/

_mqx_uint  _msg_create_component
   (
      void
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR   kernel_data;
   register MSG_COMPONENT_STRUCT_PTR msg_component_ptr;
            pointer                  pools_ptr;
            pointer                  msgqs_ptr;
            _mqx_uint                error;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE1(KLOG_msg_create_component);

   error = _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
#if MQX_CHECK_ERRORS
   if (error != MQX_OK) {
      _KLOGX2(KLOG_msg_create_component, error);
      return(error);
   } /* Endif */
#endif

   if (kernel_data->KERNEL_COMPONENTS[KERNEL_MESSAGES] != NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
      _KLOGX2(KLOG_msg_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   msg_component_ptr = (MSG_COMPONENT_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(MSG_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (msg_component_ptr == NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
      _KLOGX2(KLOG_msg_create_component, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
  _mem_set_type(msg_component_ptr, MEM_TYPE_MESSAGE_COMPONENT);

   if (kernel_data->INIT.MAX_MSGPOOLS == 0) {
      kernel_data->INIT.MAX_MSGPOOLS = 1;
   } /* Endif */
   pools_ptr = _mem_alloc_system_zero((_mem_size)(kernel_data->INIT.MAX_MSGPOOLS * 
      sizeof(MSGPOOL_STRUCT)));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (pools_ptr == NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
      _KLOGX2(KLOG_msg_create_component, MSGPOOL_POOL_NOT_CREATED);
      return MSGPOOL_POOL_NOT_CREATED;
   }/* Endif */
#endif
  _mem_set_type(pools_ptr, MEM_TYPE_MESSAGE_POOLS);

   if (kernel_data->INIT.MAX_MSGQS >= MAX_UINT_16) {
      kernel_data->INIT.MAX_MSGQS = MAX_UINT_16 - 1;
   } else if (kernel_data->INIT.MAX_MSGQS < 1) {
      kernel_data->INIT.MAX_MSGQS = 1;
   } /* Endif */
   msgqs_ptr = _mem_alloc_system_zero( (_mem_size)((kernel_data->INIT.MAX_MSGQS + 1) *
      sizeof(MSGQ_STRUCT)));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (msgqs_ptr == NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
      _mem_free(pools_ptr);
      _KLOGX2(KLOG_msg_create_component, MSGQ_TOO_MANY_QUEUES);
      return MSGQ_TOO_MANY_QUEUES;
   } /* Endif */
#endif
  _mem_set_type(msgqs_ptr, MEM_TYPE_MESSAGE_QUEUES);

   if (msg_component_ptr->MSGPOOLS_PTR == NULL) {
      msg_component_ptr->MAX_MSGPOOLS_EVER    = 0;
      msg_component_ptr->SMALLEST_MSGPOOL_PTR = NULL;
      msg_component_ptr->LARGEST_MSGPOOL_PTR  = NULL;
      msg_component_ptr->MAX_MSGPOOLS = kernel_data->INIT.MAX_MSGPOOLS;
      msg_component_ptr->MAX_MSGQS    = kernel_data->INIT.MAX_MSGQS;
      msg_component_ptr->MSGPOOLS_PTR = (MSGPOOL_STRUCT_PTR)pools_ptr;
      pools_ptr = NULL;
      msg_component_ptr->MSGQS_PTR    = (MSGQ_STRUCT_PTR)msgqs_ptr;
      msgqs_ptr = NULL;
   }/* Endif */

   msg_component_ptr->VALID = MESSAGE_VALID;

   kernel_data->KERNEL_COMPONENTS[KERNEL_MESSAGES] = msg_component_ptr;

#if MQX_COMPONENT_DESTRUCTION
   kernel_data->COMPONENT_CLEANUP[KERNEL_MESSAGES] = _msg_cleanup;
#endif

   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);

   if (pools_ptr) {
      _mem_free(pools_ptr);
   }/* Endif */
   if (msgqs_ptr) {
      _mem_free(msgqs_ptr);
   }/* Endif */

   _KLOGX2(KLOG_msg_create_component, MQX_OK);
   return MQX_OK;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
