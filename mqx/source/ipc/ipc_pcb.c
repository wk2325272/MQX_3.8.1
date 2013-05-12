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
* $FileName: ipc_pcb.c$
* $Version : 3.6.7.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains the source for the IPC drivers working over
*   IO PCB device drivers.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_IPC
#include "message.h"
#include "msg_prv.h"
#include "fio.h"
#include "io.h"
#include "io_pcb.h"
#include "ipc.h"
#include "ipc_prv.h"
#include "ipc_pcb.h"
#include "ipc_pcbv.h"

/* A list of current IPC PCB drivers that have been installed */
QUEUE_STRUCT _ipc_pcb_drivers = {NULL, NULL, 0, 0};


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _ipc_pcb_init
* Returned Value  : _mqx_uint result   MQX_OK or an error code from ipc_pcb.h
* Comments        :
*     This function is use to initialze the the inter-processor
* communications over a serial port, using the standard ASYNC_SERIAL driver.
*
*END*---------------------------------------------------------------------*/

_mqx_uint _ipc_pcb_init
   (
      /* [IN] the address of a IPC_PROTOCOL_INIT_STRUCT */
      const IPC_PROTOCOL_INIT_STRUCT * init_ptr,

      /* [IN] the address of a IPC_PROTOCOL_INFO_STRUCT */
      pointer                      data_ptr
   )
{
   IPC_PROTOCOL_INFO_STRUCT_PTR info_ptr;
   IPC_PCB_INFO_STRUCT_PTR      pcb_info_ptr;
   IPC_PCB_INIT_STRUCT_PTR      pcb_init_ptr;
   _io_pcb_pool_id              pcb_pool;
   _queue_id                    qid;
   _pool_id                     msg_pool;
   _mqx_uint                    result = MQX_OK;
   _psp_code_addr               _ipc_pcb_function_location =
      (_psp_code_addr)_ipc_pcb_input_notification;

   info_ptr = (IPC_PROTOCOL_INFO_STRUCT_PTR)data_ptr;
   pcb_info_ptr = (IPC_PCB_INFO_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(IPC_PCB_INFO_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (pcb_info_ptr == NULL) {
      return(_task_get_error());
   }
#endif
   info_ptr->IPC_PROTOCOL_INFO_PTR = pcb_info_ptr;

   pcb_init_ptr = (IPC_PCB_INIT_STRUCT_PTR)init_ptr->IPC_PROTOCOL_INIT_DATA;

   info_ptr->IPC_TYPE     = IPC_PCB_PROTOCOL;
   info_ptr->IPC_INIT_PTR = init_ptr;

   /* CREATE OUTPUT PCB POOL */
   pcb_pool = _io_pcb_create_pool(1, 0,
      pcb_init_ptr->OUT_PCBS_INITIAL,
      pcb_init_ptr->OUT_PCBS_TO_GROW,
      pcb_init_ptr->OUT_PCBS_MAX,
      0, NULL,
      0, NULL);
#if MQX_CHECK_ERRORS
   if (pcb_pool == IO_PCB_NULL_POOL_ID) {
      result = IPC_PCB_OUTPUT_PCB_POOL_CREATE_FAILED;
   } else {
#endif
      pcb_info_ptr->PCB_OUTPUT_POOL = pcb_pool;
#if MQX_CHECK_ERRORS
   }
#endif

   /* Create output message queue */
   if (result == MQX_OK ) {
      qid = _msgq_open_system(
         (_queue_number)info_ptr->IPC_INIT_PTR->IPC_OUT_QUEUE,
         0, _ipc_pcb_output_notification, info_ptr);
#if MQX_CHECK_ERRORS
      if (qid == MSGQ_NULL_QUEUE_ID) {
         result = IPC_PCB_INVALID_QUEUE;
      } else {
#endif
         pcb_info_ptr->OUT_MSG_QID = qid;
#if MQX_CHECK_ERRORS
      }
#endif
   }

   if (result == MQX_OK ) {
      /* Create input message pool */
      msg_pool = _msgpool_create(
         pcb_init_ptr->IN_MESSAGES_MAX_SIZE,
         pcb_init_ptr->IN_MESSAGES_TO_ALLOCATE,
         pcb_init_ptr->IN_MESSAGES_TO_GROW,
         pcb_init_ptr->IN_MESSAGES_MAX_ALLOCATE);
#if MQX_CHECK_ERRORS
      if (msg_pool == MSGPOOL_NULL_POOL_ID) {
         result = (IPC_PCB_PACKET_POOL_CREATE_FAILED);
      } else {
#endif
         pcb_info_ptr->MSG_INPUT_POOL = msg_pool;
#if MQX_CHECK_ERRORS
      }
#endif
   }

   if (result == MQX_OK) {
      if (pcb_init_ptr->DEVICE_INSTALL != NULL) {
         result = (*pcb_init_ptr->DEVICE_INSTALL)(
            pcb_init_ptr->IO_PCB_DEVICE_NAME,
            pcb_init_ptr->DEVICE_INSTALL_PARAMETER);
      }
   }

   if (result == MQX_OK) {
      /* Open the IO PCB device */
      pcb_info_ptr->FD = fopen(pcb_init_ptr->IO_PCB_DEVICE_NAME, (char_ptr)0);
#if MQX_CHECK_ERRORS
      if (pcb_info_ptr->FD == NULL) {
         result = IPC_PCB_DEVICE_OPEN_FAILED;
      }
#endif
   }

   if (result == MQX_OK) {
      pcb_pool = _io_pcb_create_pool(1, 0,
         pcb_init_ptr->IN_MESSAGES_TO_ALLOCATE,
         pcb_init_ptr->IN_MESSAGES_TO_GROW,
         pcb_init_ptr->IN_MESSAGES_MAX_ALLOCATE,
         _ipc_pcb_alloc, info_ptr,
         0, NULL);
#if MQX_CHECK_ERRORS
      if (pcb_pool == IO_PCB_NULL_POOL_ID) {
         result = IPC_PCB_INPUT_PCB_POOL_CREATE_FAILED;
      } else {
#endif
        pcb_info_ptr->PCB_INPUT_POOL = pcb_pool;
#if MQX_CHECK_ERRORS
      }
#endif
   }

   if (result == MQX_OK) {
      result = ioctl(pcb_info_ptr->FD, IO_PCB_IOCTL_READ_CALLBACK_SET,
        &_ipc_pcb_function_location);
   }

   if (result == MQX_OK) {
      result = ioctl(pcb_info_ptr->FD, IO_PCB_IOCTL_SET_INPUT_POOL,
        (pointer)&pcb_info_ptr->PCB_INPUT_POOL);
   }

   if (result == MQX_OK) {
      result = ioctl(pcb_info_ptr->FD, IO_PCB_IOCTL_START, NULL);
   }

#if MQX_CHECK_ERRORS
   if (result != MQX_OK ) {
      if (pcb_info_ptr->FD) {
         fclose(pcb_info_ptr->FD);
      }
      if (pcb_info_ptr->PCB_INPUT_POOL ) {
         _io_pcb_destroy_pool(pcb_info_ptr->PCB_INPUT_POOL);
      }
      if (pcb_info_ptr->MSG_INPUT_POOL) {
         _msgpool_destroy(pcb_info_ptr->MSG_INPUT_POOL);
      }
      if (pcb_info_ptr->OUT_MSG_QID) {
         _msgq_close(pcb_info_ptr->OUT_MSG_QID);
      }
      if (pcb_info_ptr->PCB_OUTPUT_POOL) {
         _io_pcb_destroy_pool(pcb_info_ptr->PCB_OUTPUT_POOL);
      }
      _mem_free(pcb_info_ptr);
   } else {
#endif
      if (_ipc_pcb_drivers.NEXT == NULL) {
         _queue_init(&_ipc_pcb_drivers, 0);
      }
      _queue_enqueue(&_ipc_pcb_drivers, &pcb_info_ptr->QUEUE);
#if MQX_CHECK_ERRORS
   }
#endif

   return(result);

}


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _ipc_pcb_output_notification
* Returned Value  : none.
* Comments        :
*     This function is called whenever a message is to be sent offboard.
* It packages the message into a PCB and fires it off to an IO PCB device
* driver.   The function is call by _msg_send (it is an activation function)
* Note that the PCB free function is set to _ipc_pcb_free..
*
*END*---------------------------------------------------------------------*/

void _ipc_pcb_output_notification
   (
      /* [IN] the address of an IPC_PROTOCOL_INFO_STRUCT */
      pointer data_ptr
   )
{ /* Body */
   IPC_PROTOCOL_INFO_STRUCT_PTR info_ptr;
   IPC_PCB_INFO_STRUCT_PTR      pcb_info_ptr;
   IO_PCB_STRUCT_PTR            pcb_ptr;
   IPC_MESSAGE_STRUCT_PTR       msg_ptr;
#if PSP_MEMORY_ADDRESSING_CAPABILITY > 8
   boolean                      must_unpack;
   boolean                      success;
   uint_32                      frag_size;
   uint_32                      result;
#endif

   info_ptr = (IPC_PROTOCOL_INFO_STRUCT_PTR)data_ptr;
   pcb_info_ptr = (IPC_PCB_INFO_STRUCT_PTR)info_ptr->IPC_PROTOCOL_INFO_PTR;

#if PSP_MEMORY_ADDRESSING_CAPABILITY > 8
   /* Determine if we must unpack the messages */
   result = ioctl(pcb_info_ptr->FD, IO_PCB_IOCTL_UNPACKED_ONLY, &must_unpack);
   if (result != MQX_OK) {
      must_unpack = FALSE;
   }
#endif

   while (_msgq_get_count(info_ptr->IPC_OUT_QID)) {
      pcb_ptr = _io_pcb_alloc(pcb_info_ptr->PCB_OUTPUT_POOL, FALSE);
      if (pcb_ptr == NULL) {
         return;
      }
      msg_ptr = (IPC_MESSAGE_STRUCT_PTR)_msgq_poll(info_ptr->IPC_OUT_QID);

#if PSP_MEMORY_ADDRESSING_CAPABILITY > 8
      if (must_unpack) {
         success = _ipc_pcb_unpack_message(msg_ptr, pcb_ptr);
         _msg_free(msg_ptr);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
         if (!success) {
            /* Toss the packet */
            _io_pcb_free_internal(pcb_ptr);
            return;
         }
#endif
      } else {
         pcb_ptr->FRAGMENTS[0].LENGTH    = (uint_32)msg_ptr->HEADER.SIZE;
         pcb_ptr->FRAGMENTS[0].FRAGMENT  = (uchar_ptr)msg_ptr;
         pcb_ptr->OWNER_PRIVATE_FLAGS    = IPC_PCB_FRAG_IS_MSG;
      }
#else
      pcb_ptr->FRAGMENTS[0].LENGTH    = (uint_32)msg_ptr->HEADER.SIZE;
      pcb_ptr->FRAGMENTS[0].FRAGMENT  = (uchar_ptr)msg_ptr;
#endif
      pcb_ptr->FREE_PCB_FUNCTION_PTR  = _ipc_pcb_free;

      _io_pcb_write(pcb_info_ptr->FD, pcb_ptr);

      pcb_info_ptr->OUTPUT_MESSAGE_COUNT++;

   }

}


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _ipc_pcb_free
* Returned Value   : _mqx_uint MQX_OK or an error code.
* Comments         :
*   this function frees a pcb sent via the ipc_pcb output task
*   It needs to free the message that is in the PCB.
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _ipc_pcb_free
   (
      /* [IN] the pcb that has completed */
      IO_PCB_STRUCT_PTR pcb_ptr
   )
{

   _msg_free(pcb_ptr->FRAGMENTS[0].FRAGMENT);
   return(_io_pcb_free_internal(pcb_ptr));

}


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _ipc_pcb_alloc
* Returned Value   : IO_PCB_STRUCT_PTR .. the pointer to the pcb (or new pcb)
*                    NULL upon error.
* Comments         :
*   this function is to be called whenever a PCB is allocation from the
* input PCB pool, by the input task.  For example, when the input task
* is preparing to set up to receive data it would obtain a PCB from the
* input pool.. At that point, this function will be called by the pcb alloc
* function.  This function obtains a message and sets up the PCB to point to it.
*
*END*-------------------------------------------------------------------------*/

IO_PCB_STRUCT_PTR _ipc_pcb_alloc
   (
      /* [IN] the pcb that has completed */
      IO_PCB_STRUCT_PTR pcb_ptr,

      /* [IN] data provided to this function */
      pointer           data_ptr
   )
{
   IPC_PROTOCOL_INFO_STRUCT_PTR info_ptr;
   IPC_PCB_INFO_STRUCT_PTR      pcb_info_ptr;
   MESSAGE_HEADER_STRUCT_PTR    msg_ptr;

   info_ptr = (IPC_PROTOCOL_INFO_STRUCT_PTR)data_ptr;
   pcb_info_ptr = (IPC_PCB_INFO_STRUCT_PTR)info_ptr->IPC_PROTOCOL_INFO_PTR;

   msg_ptr = _msg_alloc(pcb_info_ptr->MSG_INPUT_POOL);
#if MQX_CHECK_ERRORS
   if (msg_ptr == NULL) {
      return(NULL);
   }
#endif
   pcb_ptr->FRAGMENTS[0].LENGTH   = 0;
   pcb_ptr->FRAGMENTS[0].FRAGMENT = (uchar_ptr)msg_ptr;
   pcb_ptr->INSTANTIATOR_PRIVATE  = info_ptr;

   return(pcb_ptr);

}


/*FUNCTION*-----------------------------------------------------------------
*
* Task Name       : _ipc_pcb_input_notification
* Returned Value  : none.
* Comments        :
*     This function is called by the underlying IO PCB device driver
* whenever an input message has been received.
*
*END*---------------------------------------------------------------------*/

void _ipc_pcb_input_notification
   (
      /* [IN] the file descriptor used to set the notification function */
      MQX_FILE_PTR      fd_ptr,

      /* [IN] the pcb that has arrived */
      IO_PCB_STRUCT_PTR pcb_ptr
   )
{
   KERNEL_DATA_STRUCT_PTR       kernel_data;
   IPC_PROTOCOL_INFO_STRUCT_PTR info_ptr;
   IPC_PCB_INFO_STRUCT_PTR      pcb_info_ptr;
   MESSAGE_HEADER_STRUCT_PTR    msg_ptr;

   _GET_KERNEL_DATA(kernel_data);

   info_ptr = (IPC_PROTOCOL_INFO_STRUCT_PTR)pcb_ptr->INSTANTIATOR_PRIVATE;
   pcb_info_ptr = (IPC_PCB_INFO_STRUCT_PTR)info_ptr->IPC_PROTOCOL_INFO_PTR;

   msg_ptr = (MESSAGE_HEADER_STRUCT_PTR)pcb_ptr->FRAGMENTS[0].FRAGMENT;
   if (MSG_MUST_CONVERT_HDR_ENDIAN(msg_ptr->CONTROL)) {
      _msg_swap_endian_header(msg_ptr);
   }
   if (PROC_NUMBER_FROM_QID(msg_ptr->TARGET_QID) == 0) {
      /* This is a special message for this CPU */
      msg_ptr->TARGET_QID = BUILD_QID(kernel_data->INIT.PROCESSOR_NUMBER,
         QUEUE_FROM_QID(msg_ptr->TARGET_QID));
   }
   _msgq_send(msg_ptr);

   _io_pcb_free(pcb_ptr);

   pcb_info_ptr->INPUT_MESSAGE_COUNT++;

}
#endif /* MQX_USE_IPC */

/* EOF */
