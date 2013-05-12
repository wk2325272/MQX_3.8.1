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
* $FileName: ipc_task.c$
* $Version : 3.8.8.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains the functions for the ipc task.
*
*END************************************************************************/

#include <stdarg.h>
#include <string.h>
#include "mqx_inc.h"
#if MQX_USE_IPC
#include "message.h"
#include "msg_prv.h"
#include "ipc.h"
#include "ipc_prv.h"
#include "ipcrtprv.h"

/*
** Definition of the field sizes in the IPC message data.
** So that the ENDIAN-ness can be swapped by _msg_swap_endian_data
*/
static const uchar _ipc_msg_type_def[] =  {
   sizeof(_mqx_uint),
   0};

static const uchar _ipc_msg_def[] = {
   sizeof(_mqx_uint),  sizeof(_task_id),
  /* Parameters... number of these must equal IPC_MAX_PARAMETERS */
   sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint),
   sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint),
   sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint),
   sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint), sizeof(_mqx_uint),
   0};

static const uchar _ipc_mqx_msg_def[] = {
   sizeof(_mqx_uint), sizeof(_task_id), sizeof(_task_id), sizeof(_mqx_uint),
   sizeof(_mqx_uint), sizeof(void (_CODE_PTR_)(_mqx_uint)),
   sizeof(_mqx_uint), sizeof(_mqx_uint),  sizeof(char _PTR_),
   sizeof(_mqx_uint), sizeof(_mqx_uint),  sizeof(_mqx_uint),
   0};


const IPC_PROTOCOL_INIT_STRUCT _ipc_init_table[] =
{
   { NULL, NULL, NULL, 0}
};

static const IPC_INIT_STRUCT _default_ipc_init = {
    _ipc_routing_table,
    _ipc_init_table
};

/*TASK*-----------------------------------------------------------------------
*
* Task Name        : _ipc_task
* Comments         :
*    The IPC task is responsible for handling IPC messages that arrive from
* another processor.
*
*END*----------------------------------------------------------------------*/

void _ipc_task
   (
      uint_32 parameter
   )
{
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   IPC_COMPONENT_STRUCT_PTR      ipc_component_ptr;
   const IPC_INIT_STRUCT *       ipc_init_ptr = (IPC_INIT_STRUCT_PTR)parameter;
   IPC_MESSAGE_STRUCT_PTR        msg_ptr;
   IPC_MQX_MESSAGE_STRUCT_PTR    ipc_msg_ptr;
   const IPC_PROTOCOL_INIT_STRUCT *  protocol_init_ptr;
   IPC_PROTOCOL_INFO_STRUCT_PTR  info_ptr;
   TD_STRUCT_PTR                 td_ptr;
   _mqx_uint         (_CODE_PTR_ handler)(IPC_MESSAGE_STRUCT_PTR);
   _queue_id                     queue;
   _mqx_uint                     result;
   _mqx_uint                     component;

   _GET_KERNEL_DATA(kernel_data);

   if (ipc_init_ptr==NULL) {
       ipc_init_ptr = &_default_ipc_init;
   }

   /* Install the message routing handlers */
   result = _ipc_msg_route_init_internal(ipc_init_ptr->ROUTING_LIST_PTR);
#if MQX_CHECK_ERRORS
   if (result != MQX_OK) {
      _task_set_error(result);
      _task_block();
   }
#endif

   kernel_data->MY_IPC_TD_PTR = kernel_data->ACTIVE_PTR;
   kernel_data->MY_IPC_ID     = kernel_data->ACTIVE_PTR->TASK_ID;

   /* Create the IPC component structure */
   ipc_component_ptr = (IPC_COMPONENT_STRUCT_PTR)
      _mem_alloc_zero((_mem_size)sizeof(IPC_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (ipc_component_ptr == NULL) {
      _task_block();
   }
#endif
   kernel_data->IPC_COMPONENT_PTR = ipc_component_ptr;

   /* Create a pool of messages for use by the ipc task. */
   kernel_data->IPC_NAMED_POOL = _msgpool_create(
      sizeof(IPC_MESSAGE_STRUCT), IPC_NUM_MESSAGES, IPC_GROW_MESSAGES, IPC_LIMIT_MESSAGES);
#if MQX_CHECK_ERRORS || MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (kernel_data->IPC_NAMED_POOL == MSGPOOL_NULL_POOL_ID) {
      _task_block();
   }
#endif

   /* Open the ipc message queue */
   queue = _msgq_open(IPC_MESSAGE_QUEUE_NUMBER, 0);
#if MQX_CHECK_ERRORS
      if (queue == MSGQ_NULL_QUEUE_ID) {
         _task_block();
      }
#endif

   kernel_data->IPC = _ipc_send_internal;

   /* Initialize inter-processor communications protocols */
   protocol_init_ptr = ipc_init_ptr->PROTOCOL_LIST_PTR;
   while (protocol_init_ptr->IPC_PROTOCOL_INIT != NULL) {
#if MQX_CHECK_ERRORS
      if (((_queue_number)protocol_init_ptr->IPC_OUT_QUEUE) == 0 ||
         ((_queue_number)protocol_init_ptr->IPC_OUT_QUEUE > MQX_MAX_PROCESSOR_NUMBER))
      {
         _task_block();
      }
#endif
      info_ptr = (IPC_PROTOCOL_INFO_STRUCT_PTR)
         _mem_alloc_zero((_mem_size)sizeof(IPC_PROTOCOL_INFO_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (info_ptr == NULL) {
         _task_block();
      }
#endif
      info_ptr->IPC_OUT_QUEUE = protocol_init_ptr->IPC_OUT_QUEUE;
      info_ptr->IPC_NAME      = protocol_init_ptr->IPC_NAME;
      info_ptr->IPC_OUT_QID   = _msgq_get_id(0, (_queue_number)protocol_init_ptr->IPC_OUT_QUEUE);
      info_ptr->IPC_INIT_PTR  = protocol_init_ptr;

      (*protocol_init_ptr->IPC_PROTOCOL_INIT)(protocol_init_ptr, info_ptr);

      protocol_init_ptr++;
   }

   /*
   ** The IPC Initialization is complete. Wait for messages to come in
   ** and service them.
   */
   while ( TRUE ) {

      _task_set_error(MQX_OK);
      msg_ptr = (IPC_MESSAGE_STRUCT_PTR)_msgq_receive(MSGQ_ANY_QUEUE, 0L);
      if ( MSG_MUST_CONVERT_DATA_ENDIAN(msg_ptr->HEADER.CONTROL) ) {
         _msg_swap_endian_data( (uchar _PTR_)((pointer)_ipc_msg_type_def),
            (MESSAGE_HEADER_STRUCT_PTR)((pointer)msg_ptr));
         if (msg_ptr->MESSAGE_TYPE & IPC_MQX_TYPE_MASK) {
            _mem_swap_endian( (uchar _PTR_)_ipc_mqx_msg_def,
               &msg_ptr->NUMBER_OF_PARAMETERS);
         } else {
            _mem_swap_endian_len( (uchar _PTR_)_ipc_msg_def,
               &msg_ptr->NUMBER_OF_PARAMETERS, msg_ptr->NUMBER_OF_PARAMETERS + 2);
         }
      }

      if (msg_ptr->MESSAGE_TYPE & IPC_MQX_TYPE_MASK) {

         ipc_msg_ptr = (IPC_MQX_MESSAGE_STRUCT_PTR)((pointer)msg_ptr);
         switch (ipc_msg_ptr->MESSAGE_TYPE & IPC_MQX_TYPE_MASK) {

            case IPC_MQX_CREATE:
               _KLOGE4(KLOG_task_create, 0, ipc_msg_ptr->TEMPLATE_INDEX,ipc_msg_ptr->CREATE_PARAMETER);
               if (ipc_msg_ptr->TEMPLATE_INDEX == 0) {
                  td_ptr = _task_build_internal(0, (uint_32)&ipc_msg_ptr->TEMPLATE, NULL, 0, FALSE);
               } else {
                  td_ptr = _task_build_internal(ipc_msg_ptr->TEMPLATE_INDEX, (uint_32)ipc_msg_ptr->CREATE_PARAMETER, NULL, 0, FALSE);
               }

               _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, MQX_INVALID_PROCESSOR_NUMBER);
               if (td_ptr != NULL) {
                  td_ptr->PARENT = ipc_msg_ptr->REQUESTOR_ID;
                  _task_ready(td_ptr);
                  _KLOGX3(KLOG_task_create, td_ptr->TASK_ID, MQX_OK);
               } else {
                  _KLOGX3(KLOG_task_create, MQX_NULL_TASK_ID, _task_get_error());
               }
               result = (_mqx_uint)td_ptr->TASK_ID;
               _ipc_send_internal(FALSE,
                  PROC_NUMBER_FROM_TASKID(msg_ptr->REQUESTOR_ID),
                  KERNEL_MESSAGES, IPC_ACTIVATE,
                  (_mqx_uint)3, result, msg_ptr->REQUESTOR_ID,
                  kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
               break;

            case IPC_MQX_ABORT:
               result = (_mqx_uint)_task_abort(ipc_msg_ptr->VICTIM_ID);
               break;
            case IPC_MQX_RESTART:
               result = (_mqx_uint)_task_restart(ipc_msg_ptr->VICTIM_ID, NULL, TRUE);
               break;
            case IPC_MQX_DESTROY:
               result = (_mqx_uint)_task_destroy(ipc_msg_ptr->VICTIM_ID);
               break;

            case IPC_MQX_ACTIVATE:
               /* Start the specified task, setting the INFO
               ** to the returned result.
               */
               td_ptr = (TD_STRUCT_PTR)_task_get_td(ipc_msg_ptr->REQUESTOR_ID);
               if (td_ptr != NULL) {
                  if (td_ptr->STATE == SEND_BLOCKED) {
                     td_ptr->INFO = ipc_msg_ptr->VICTIM_ID;
                     _task_set_error_td_internal(td_ptr, ipc_msg_ptr->CREATE_PARAMETER);
                     _task_ready(td_ptr);
                  }
               }
               result = MQX_OK;
               break;

         }

         _msg_free((pointer)msg_ptr);

      } else {

         component = IPC_GET_COMPONENT(msg_ptr->MESSAGE_TYPE);

         if (component < MAX_KERNEL_COMPONENTS) {
            handler = ipc_component_ptr->IPC_COMPONENT_HANDLER[component];
#if MQX_CHECK_ERRORS
            if (handler) {
#endif
               _task_set_error(MQX_OK);
               result = (*handler)((pointer)msg_ptr);
#if MQX_CHECK_ERRORS
            } else {
               result = MQX_IPC_SERVICE_NOT_AVAILABLE;
            }
#endif
         } else if (component < (MAX_KERNEL_COMPONENTS + MAX_IO_COMPONENTS)) {
            handler = ipc_component_ptr->IPC_IO_COMPONENT_HANDLER[component - MAX_KERNEL_COMPONENTS];
#if MQX_CHECK_ERRORS
            if (handler) {
#endif
               _task_set_error(MQX_OK);
               result = (*handler)((pointer)msg_ptr);
#if MQX_CHECK_ERRORS
            } else {
               result = MQX_IPC_SERVICE_NOT_AVAILABLE;
               _msg_free((pointer)msg_ptr);
            }
#endif
         } else {
            result = MQX_IPC_INVALID_MESSAGE;
            _msg_free((pointer)msg_ptr);
         }
      }
   }
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _ipc_send_internal
* Returned Value   : _mqx_uint a mqx task error code
* Comments         :
*   this function creates an IPC message and sends it.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _ipc_send_internal
   (
      /* [IN] blocking... is the call to blcok the current task */
      boolean blocking,

      /* [IN] the processor to send to */
      _processor_number processor_number,

      /* [IN] the component to receive the message */
      _mqx_uint component,

      /* [IN] the type of component message */
      _mqx_uint type,

      /* [IN] the number of parameters in the message */
      _mqx_uint num_parameters,

       ...
   )
{
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   IPC_MESSAGE_STRUCT_PTR     msg_ptr;
   IPC_MQX_MESSAGE_STRUCT_PTR ipc_msg_ptr;
   va_list                    ap;
   _mqx_uint                  i;

   _GET_KERNEL_DATA(kernel_data);

   kernel_data->ACTIVE_PTR->TASK_ERROR_CODE = MQX_OK;

   msg_ptr = (IPC_MESSAGE_STRUCT_PTR) _msg_alloc((_pool_id)kernel_data->IPC_NAMED_POOL);
   while (msg_ptr == NULL) {
      _time_delay(kernel_data->KERNEL_ALARM_RESOLUTION*2);
      msg_ptr = (IPC_MESSAGE_STRUCT_PTR) _msg_alloc((_pool_id)kernel_data->IPC_NAMED_POOL);
   }

   msg_ptr->HEADER.TARGET_QID = BUILD_QID(processor_number, IPC_MESSAGE_QUEUE_NUMBER);
   msg_ptr->HEADER.SOURCE_QID = BUILD_QID((_processor_number) kernel_data->INIT.PROCESSOR_NUMBER, IPC_MESSAGE_QUEUE_NUMBER);

   va_start(ap, num_parameters);

   if (component == KERNEL_MESSAGES) {
      ipc_msg_ptr = (IPC_MQX_MESSAGE_STRUCT_PTR)((pointer)msg_ptr);
      ipc_msg_ptr->REQUESTOR_ID = kernel_data->ACTIVE_PTR->TASK_ID;
      switch (type) {
         case IPC_TASK_CREATE:
         case IPC_TASK_CREATE_WITH_TEMPLATE:
            ipc_msg_ptr->MESSAGE_TYPE = IPC_MQX_CREATE;
            if (type == IPC_TASK_CREATE_WITH_TEMPLATE) {
               ipc_msg_ptr->TEMPLATE_INDEX = 0;
               ipc_msg_ptr->TEMPLATE.TASK_TEMPLATE_INDEX = va_arg(ap, uint_32);
               ipc_msg_ptr->TEMPLATE.TASK_ADDRESS    =
                  (void (_CODE_PTR_)(uint_32))va_arg(ap, _psp_code_addr);
               ipc_msg_ptr->TEMPLATE.TASK_STACKSIZE  = va_arg(ap, _mem_size);
               ipc_msg_ptr->TEMPLATE.TASK_PRIORITY   = va_arg(ap, _mqx_uint);
               ipc_msg_ptr->TEMPLATE.TASK_NAME       = va_arg(ap, char_ptr);
               ipc_msg_ptr->TEMPLATE.TASK_ATTRIBUTES = va_arg(ap, _mqx_uint);
               ipc_msg_ptr->TEMPLATE.CREATION_PARAMETER = va_arg(ap, uint_32);
               ipc_msg_ptr->CREATE_PARAMETER = ipc_msg_ptr->TEMPLATE.CREATION_PARAMETER;
               ipc_msg_ptr->TEMPLATE.DEFAULT_TIME_SLICE = va_arg(ap, uint_32);
               ipc_msg_ptr->HEADER.SIZE  = sizeof(IPC_MQX_MESSAGE_STRUCT);
            } else {
               i = va_arg(ap, _mqx_uint);  /* Toss processor number */
               ipc_msg_ptr->TEMPLATE_INDEX   = va_arg(ap, _mqx_uint);
               ipc_msg_ptr->CREATE_PARAMETER = (uint_32)va_arg(ap, uint_32);
               ipc_msg_ptr->HEADER.SIZE  = sizeof(IPC_MQX_MESSAGE_STRUCT) -
                  sizeof(TASK_TEMPLATE_STRUCT);
            }
            break;
         case IPC_TASK_DESTROY:
            ipc_msg_ptr->MESSAGE_TYPE = IPC_MQX_DESTROY;
            ipc_msg_ptr->VICTIM_ID    = va_arg(ap, _mqx_uint);
            ipc_msg_ptr->HEADER.SIZE  = sizeof(IPC_MQX_MESSAGE_STRUCT) -
               sizeof(TASK_TEMPLATE_STRUCT);
            break;
         case IPC_TASK_ABORT:
            ipc_msg_ptr->MESSAGE_TYPE = IPC_MQX_ABORT;
            ipc_msg_ptr->VICTIM_ID    = va_arg(ap, _mqx_uint);
            ipc_msg_ptr->HEADER.SIZE  = sizeof(IPC_MQX_MESSAGE_STRUCT) -
               sizeof(TASK_TEMPLATE_STRUCT);
            break;
        case IPC_TASK_RESTART:
            ipc_msg_ptr->MESSAGE_TYPE = IPC_MQX_RESTART;
            ipc_msg_ptr->VICTIM_ID    = va_arg(ap, _mqx_uint);
            ipc_msg_ptr->HEADER.SIZE  = sizeof(IPC_MQX_MESSAGE_STRUCT) -
               sizeof(TASK_TEMPLATE_STRUCT);
            break;
         case IPC_ACTIVATE:
            ipc_msg_ptr->MESSAGE_TYPE     = IPC_MQX_ACTIVATE;
            ipc_msg_ptr->VICTIM_ID        = va_arg(ap, _mqx_uint);
            ipc_msg_ptr->REQUESTOR_ID     = va_arg(ap, _mqx_uint);
            ipc_msg_ptr->CREATE_PARAMETER = (uint_32)va_arg(ap, uint_32);
            ipc_msg_ptr->HEADER.SIZE  = sizeof(IPC_MQX_MESSAGE_STRUCT) -
               sizeof(TASK_TEMPLATE_STRUCT);
            break;
      }

   } else {

      msg_ptr->MESSAGE_TYPE =   IPC_SET_COMPONENT(0, component);
      msg_ptr->MESSAGE_TYPE = IPC_SET_TYPE(msg_ptr->MESSAGE_TYPE, type);
      if (! blocking) {
         msg_ptr->MESSAGE_TYPE = IPC_SET_NON_BLOCKING(msg_ptr->MESSAGE_TYPE, 1);
      }

      msg_ptr->REQUESTOR_ID         = kernel_data->ACTIVE_PTR->TASK_ID;

      if (num_parameters > IPC_MAX_PARAMETERS) {
         num_parameters = IPC_MAX_PARAMETERS;
      }

      if (type & IPC_STRING_PARAMETER) {
         --num_parameters;
         msg_ptr->HEADER.SIZE = (_msg_size)sizeof(IPC_MESSAGE_STRUCT);
      } else {
         msg_ptr->HEADER.SIZE = (_msg_size)(sizeof(IPC_MESSAGE_STRUCT) -
            (IPC_MAX_PARAMETERS * sizeof(uint_32)) +
            (num_parameters * sizeof(uint_32)));
      }

      msg_ptr->NUMBER_OF_PARAMETERS = (uint_32)num_parameters;

      for (i = 0; (i < num_parameters) && (i < IPC_MAX_PARAMETERS); ++i) {
         msg_ptr->PARAMETERS[i] = (uint_32)(va_arg(ap, uint_32));
      }

      if (type & IPC_STRING_PARAMETER) {
         char_ptr to_ptr   = (char_ptr)&msg_ptr->PARAMETERS[i];
         char_ptr from_ptr = (char_ptr)va_arg(ap, pointer);
         uint_32  size     = (IPC_MAX_PARAMETERS - i) * sizeof(uint_32);
         strncpy((pointer)to_ptr, (pointer)from_ptr, size);
         to_ptr[size-1] = '\0';
      }
   }

   va_end(ap);

   if (blocking) {
      if (_msgq_send_blocked_internal(msg_ptr)) {
         return MQX_OK;
      }
   } else {
      if (_msgq_send(msg_ptr)) {
         return MQX_OK;
      }
   }

   return kernel_data->ACTIVE_PTR->TASK_ERROR_CODE;

}
#endif /* MQX_USE_IPC */

/* EOF */
