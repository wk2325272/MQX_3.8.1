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
* $FileName: eds_init.c$
* $Version : 3.0.5.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating the EDS IPC component
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_IPC
#include "pcb.h"
#include "eds.h"
#include "eds_prv.h"

/* Global variable controling the timout for multi-processor EDS */
_mqx_uint _eds_timeout_value = EDS_MSG_WAIT_TIMEOUT;

/*FUNCTION****************************************************************
* 
* Function Name    : _eds_create_component
* Returned Value   : 
*   Returns MQX_OK upon success, a Task Error code or an error code:
* Comments         :
*    Used by a task to install the EDS IPC component into the kernel
*
*END**********************************************************************/

_mqx_uint _eds_create_component
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   EDS_COMPONENT_STRUCT_PTR  eds_component_ptr;
   _mqx_uint                 result = MQX_OK;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE1(KLOG_eds_create_component);

#if MQX_CHECK_ERRORS
   if (kernel_data->IO_COMPONENTS[IO_EDS_COMPONENT] != NULL) {
      _KLOGX2(KLOG_eds_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (kernel_data->IPC_COMPONENT_PTR == NULL) {
      _KLOGX2(KLOG_eds_create_component, MQX_INVALID_COMPONENT_HANDLE);
      return(MQX_INVALID_COMPONENT_HANDLE);
   } /* Endif */
#endif

   eds_component_ptr = _mem_alloc_system_zero(
      (_mem_size)sizeof(EDS_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (eds_component_ptr == NULL) {
      _KLOGX2(KLOG_eds_create_component, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif

   _time_init_ticks(&eds_component_ptr->TIMEOUT, EDS_MSG_WAIT_TIMEOUT);

   _int_disable();

#if MQX_CHECK_ERRORS
   if (kernel_data->IO_COMPONENTS[IO_EDS_COMPONENT] != NULL) {
      _int_enable();
      _mem_free(eds_component_ptr);
      _KLOGX2(KLOG_eds_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */
#endif

   result = _lwsem_create(&eds_component_ptr->SEM, 1);

#if MQX_CHECK_ERRORS
   if (result != MQX_OK) {
      _int_enable();
      _mem_free(eds_component_ptr);
      _KLOGX2(KLOG_eds_create_component, result);
      return(result);
   } /* Endif */
#endif
   
   kernel_data->IO_COMPONENTS[IO_EDS_COMPONENT] = eds_component_ptr;

   eds_component_ptr->MY_QID =
      BUILD_QID(kernel_data->PROCESSOR_NUMBER,IPC_MESSAGE_QUEUE_NUMBER);
   eds_component_ptr->VALID  = EDS_VALID;

   _lwsem_wait(&eds_component_ptr->SEM);

   _int_enable();

   /* Install the EDS IPC handler */
   _ipc_add_io_ipc_handler(_eds_ipc_handler, IO_EDS_COMPONENT);

   _lwsem_post(&eds_component_ptr->SEM);

   _KLOGX2(KLOG_eds_create_component, MQX_OK);
   return(MQX_OK);

} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _eds_ipc_handler
* Returned Value   : Returns MQX_OK upon success, a Task Error code 
*                    or an error code:
* Comments         : Used by the IPC task to handle EDS messages
*
*END**********************************************************************/

_mqx_uint _eds_ipc_handler
   (
      pointer imsg_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   EDS_COMPONENT_STRUCT_PTR  eds_component_ptr;
   IPC_MESSAGE_STRUCT_PTR    ipc_msg_ptr;
   EDS_MSG_STRUCT_PTR        eds_msg_ptr;
   EDS_OP_STRUCT_PTR         op_ptr;
   IPC_MESSAGE_STRUCT_PTR    ipc_msg2_ptr;
   EDS_MSG_STRUCT_PTR        eds_msg2_ptr;
   _mqx_uint                 size;
   _queue_number             qnum;
   _processor_number         i;
   _processor_number         pcount;
   /* Start CR 327 */
   boolean                   remote_is_little_endian;
   /* End CR 327 */

   _GET_KERNEL_DATA(kernel_data);

   ipc_msg_ptr = imsg_ptr;
   eds_msg_ptr = (EDS_MSG_STRUCT_PTR)((pointer)ipc_msg_ptr->PARAMETERS);
   op_ptr      = &eds_msg_ptr->OP_COMMAND;

   /* Start CR 327 */
   remote_is_little_endian = MSG_IS_DATA_LITTLE_ENDIAN(ipc_msg_ptr->HEADER.CONTROL);
   /* End CR 327 */
   eds_component_ptr = kernel_data->IO_COMPONENTS[IO_EDS_COMPONENT];

#if MQX_CHECK_ERRORS
   if (!eds_component_ptr) {
      return MQX_INVALID_COMPONENT_HANDLE;
   } /* Endif */
#endif

#if MQX_CHECK_VALIDITY
   if (eds_component_ptr->VALID != EDS_VALID) {
      return MQX_INVALID_COMPONENT_HANDLE;
   } /* Endif */
#endif

   _lwsem_wait(&eds_component_ptr->SEM);

   switch (op_ptr->OPERATION) {

      case EDS_IDENTIFY:
      case EDS_LITTLE_ENDIAN_IDENTIFY:
         /* Save the host's processor number for future reference */
         eds_component_ptr->HOST_PNUM = 
            PROC_NUMBER_FROM_QID(ipc_msg_ptr->HEADER.SOURCE_QID);
         eds_component_ptr->HOST_QNUM = 
            QUEUE_FROM_QID(ipc_msg_ptr->HEADER.SOURCE_QID);
         eds_component_ptr->HOST_QID = ipc_msg_ptr->HEADER.SOURCE_QID;
         op_ptr->ADDRESS   = (uint_32)kernel_data->INIT.START_OF_KERNEL_MEMORY;
         op_ptr->ADDRESS2  = (uint_32)kernel_data->INIT.END_OF_KERNEL_MEMORY;
         op_ptr->PROCESSOR = kernel_data->CPU_TYPE;
#if (PSP_ENDIAN == MQX_BIG_ENDIAN)
         op_ptr->ENDIAN    = EDS_BIG_ENDIAN;
#else
         op_ptr->ENDIAN    = EDS_LITTLE_ENDIAN;
#endif
         op_ptr->EDS_ERROR = EDS_OK;
         ipc_msg_ptr->HEADER.SIZE = EDS_COMMAND_SIZE;
         ipc_msg_ptr->MESSAGE_TYPE =
            IPC_SET_IO_MESSAGE_TYPE(IO_EDS_COMPONENT, 0,1,0);
         _eds_ipc_return_message(ipc_msg_ptr);
         break;

      case EDS_IDENTIFY_MULTIPROC:
         /* Start CR 327 */
#if 0
         htonl(&op_ptr->OPERATION,EDS_CONFIG_MULTIPROC_RESPOND);
         htonl(&op_ptr->ADDRESS,(uint_32)kernel_data->INIT.START_OF_KERNEL_MEMORY);
         htonl(&op_ptr->ADDRESS2,(uint_32)kernel_data->INIT.END_OF_KERNEL_MEMORY);
         htonl(&op_ptr->PROCESSOR,kernel_data->CPU_TYPE);
#if (PSP_ENDIAN == MQX_BIG_ENDIAN)
         op_ptr->ENDIAN    = EDS_BIG_ENDIAN;
#else
         htonl(&op_ptr->ENDIAN,EDS_LITTLE_ENDIAN);
#endif
         htonl(&op_ptr->EDS_ERROR,EDS_OK);
#endif
#if (PSP_ENDIAN == MQX_BIG_ENDIAN)
         if (remote_is_little_endian)  {
            htonl(&op_ptr->OPERATION,EDS_CONFIG_MULTIPROC_RESPOND);
            htonl(&op_ptr->ADDRESS,(uint_32)kernel_data->INIT.START_OF_KERNEL_MEMORY);
            htonl(&op_ptr->ADDRESS2,(uint_32)kernel_data->INIT.END_OF_KERNEL_MEMORY);
            htonl(&op_ptr->PROCESSOR,kernel_data->CPU_TYPE);
            htonl(&op_ptr->ENDIAN,EDS_BIG_ENDIAN);
            htonl(&op_ptr->EDS_ERROR,EDS_OK);
         } else {
            op_ptr->OPERATION  = EDS_CONFIG_MULTIPROC_RESPOND;
            op_ptr->ADDRESS    = (uint_32)kernel_data->INIT.START_OF_KERNEL_MEMORY;
            op_ptr->ADDRESS2   = (uint_32)kernel_data->INIT.END_OF_KERNEL_MEMORY;
            op_ptr->PROCESSOR  = kernel_data->CPU_TYPE;
            op_ptr->ENDIAN     = EDS_BIG_ENDIAN;
            op_ptr->EDS_ERROR  = EDS_OK;
         } /* Endif */
#else
         if (!remote_is_little_endian)  {
            htonl(&op_ptr->OPERATION,EDS_CONFIG_MULTIPROC_RESPOND);
            htonl(&op_ptr->ADDRESS,(uint_32)kernel_data->INIT.START_OF_KERNEL_MEMORY);
            htonl(&op_ptr->ADDRESS2,(uint_32)kernel_data->INIT.END_OF_KERNEL_MEMORY);
            htonl(&op_ptr->PROCESSOR,kernel_data->CPU_TYPE);
            htonl(&op_ptr->ENDIAN,EDS_LITTLE_ENDIAN);
            htonl(&op_ptr->EDS_ERROR,EDS_OK);
         } else {
            op_ptr->OPERATION  = EDS_CONFIG_MULTIPROC_RESPOND;
            op_ptr->ADDRESS    = (uint_32)kernel_data->INIT.START_OF_KERNEL_MEMORY;
            op_ptr->ADDRESS2   = (uint_32)kernel_data->INIT.END_OF_KERNEL_MEMORY;
            op_ptr->PROCESSOR  = kernel_data->CPU_TYPE;
            op_ptr->ENDIAN     = EDS_LITTLE_ENDIAN;
            op_ptr->EDS_ERROR  = EDS_OK;
         } /* Endif */
#endif         
         /* End CR 327 */
         ipc_msg_ptr->HEADER.SIZE = EDS_COMMAND_SIZE;
         ipc_msg_ptr->MESSAGE_TYPE =
            IPC_SET_IO_MESSAGE_TYPE(IO_EDS_COMPONENT, 0,1,0);
         _eds_ipc_return_message(ipc_msg_ptr);
         break;

      case EDS_CONFIG_MULTIPROC:
         _time_init_ticks(&eds_component_ptr->TIMEOUT, _eds_timeout_value);
         qnum = QUEUE_FROM_QID(ipc_msg_ptr->HEADER.TARGET_QID);
         pcount = 0;
         for (i = 1; i < (uint_32)MQX_MAX_PROCESSOR_NUMBER; i++) {
            if ((i != (uint_16)kernel_data->PROCESSOR_NUMBER) &&
               (i != eds_component_ptr->HOST_PNUM) && 
               _ipc_msg_processor_route_exists(i) ) 
            {
               ipc_msg2_ptr = (IPC_MESSAGE_STRUCT_PTR)
                  _msg_alloc(kernel_data->IPC_NAMED_POOL);
               while(!ipc_msg2_ptr) {
                  _time_delay_ticks(EDS_MSG_WAIT_TIMEOUT);
                  ipc_msg2_ptr = (IPC_MESSAGE_STRUCT_PTR)
                     _msg_alloc(kernel_data->IPC_NAMED_POOL);
               } /* Endif */
               ipc_msg2_ptr->HEADER.SOURCE_QID = ipc_msg_ptr->HEADER.TARGET_QID;
               ipc_msg2_ptr->HEADER.TARGET_QID = BUILD_QID(i, qnum);
               ipc_msg2_ptr->HEADER.SIZE       = EDS_COMMAND_SIZE;
               eds_msg2_ptr = (EDS_MSG_STRUCT_PTR)
                  ((pointer)ipc_msg2_ptr->PARAMETERS);
               eds_msg2_ptr->OP_COMMAND.OPERATION = EDS_IDENTIFY_MULTIPROC;
               ipc_msg2_ptr->MESSAGE_TYPE =
                  IPC_SET_IO_MESSAGE_TYPE(IO_EDS_COMPONENT, 0,1,0);
               _msgq_send(ipc_msg2_ptr);
               pcount++;
            } /* Endif */
         } /* Endfor */
         if (pcount) {
            eds_component_ptr->RESPONSES_EXPECTED = pcount;
            eds_component_ptr->SAVED_MSG_PTR      = ipc_msg_ptr;
            eds_component_ptr->TIMER_ID = _timer_start_oneshot_after_ticks(
               _eds_timeout, (pointer)eds_component_ptr, TIMER_KERNEL_TIME_MODE,
               &eds_component_ptr->TIMEOUT);
            eds_component_ptr->STATE = EDS_CONFIG_IN_PROGRESS;
         } else {
            op_ptr->SIZE      = 0;
            op_ptr->OPERATION = EDS_CONFIG_MULTIPROC_END;
            ipc_msg_ptr->HEADER.SIZE  = EDS_COMMAND_SIZE;
            eds_component_ptr->STATE  = EDS_IDLE;
            ipc_msg_ptr->MESSAGE_TYPE =
               IPC_SET_IO_MESSAGE_TYPE(IO_EDS_COMPONENT, 0,1,0);
            _eds_ipc_return_message(ipc_msg_ptr);
         } /* Endif */
         break;

      case EDS_CONFIG_MULTIPROC_RESPOND:
         if ((eds_component_ptr->STATE == EDS_CONFIG_IN_PROGRESS) &&
            /* If the timer is still on then the response was in time */
             (eds_component_ptr->TIMER_ID != TIMER_NULL_ID))
         {

            /* Forward the message to the host */
            ipc_msg_ptr->HEADER.TARGET_QID = eds_component_ptr->HOST_QID;
            ipc_msg_ptr->HEADER.CONTROL    = MSG_HDR_ENDIAN | MSG_DATA_ENDIAN;
            op_ptr->OPERATION              = EDS_IDENTIFY;
            _msgq_send(ipc_msg_ptr);

            /* Check to see if this is the last one */
            if (!(--eds_component_ptr->RESPONSES_EXPECTED)) {
            
               /* Cancel the timer */
               _timer_cancel(eds_component_ptr->TIMER_ID);

               eds_component_ptr->STATE    = EDS_IDLE;
               eds_component_ptr->TIMER_ID = TIMER_NULL_ID;

               ipc_msg2_ptr = eds_component_ptr->SAVED_MSG_PTR;
               ipc_msg2_ptr->HEADER.SOURCE_QID = eds_component_ptr->MY_QID;
               ipc_msg2_ptr->HEADER.TARGET_QID = eds_component_ptr->HOST_QID;
               ipc_msg2_ptr->HEADER.SIZE       = EDS_COMMAND_SIZE;
               eds_msg2_ptr = (EDS_MSG_STRUCT_PTR)
                  ((pointer)ipc_msg2_ptr->PARAMETERS);
               eds_msg2_ptr->OP_COMMAND.OPERATION = EDS_CONFIG_MULTIPROC_END;
               eds_msg2_ptr->OP_COMMAND.SIZE      = 0;
               ipc_msg2_ptr->MESSAGE_TYPE =
                  IPC_SET_IO_MESSAGE_TYPE(IO_EDS_COMPONENT, 0,1,0);
               _msgq_send(ipc_msg2_ptr);
            } /* Endif */
         } else {
            /* Response wasn't in time. Toss the message */
             eds_component_ptr->STATE = EDS_IDLE;
            _msg_free(ipc_msg_ptr);
         } /* Endif */
         break;

      case EDS_READ:
         /* Limit the size to transfer */
         if (op_ptr->SIZE > (IPC_MAX_PARAMETERS * 4)){
            op_ptr->SIZE = IPC_MAX_PARAMETERS * 4;
            ipc_msg_ptr->MESSAGE_TYPE = EDS_INVALID_SIZE;
         } else {
            ipc_msg_ptr->MESSAGE_TYPE = EDS_OK;
         } /* Endif */

         /* Calculate the number of single addressable units to read/write */
         size = op_ptr->SIZE / (4/sizeof(uint_32));

         /* Copy the data. */
         _mem_copy((pointer)op_ptr->ADDRESS, (pointer)op_ptr, size);
         ipc_msg_ptr->HEADER.SIZE = IPC_COMMAND_SIZE + size;
         _eds_ipc_return_message(ipc_msg_ptr);
         break;

      case EDS_WRITE:
         /* Limit the size to transfer */
         if (op_ptr->SIZE > (EDS_DATA_SIZE * 4)){
            op_ptr->SIZE = EDS_DATA_SIZE * 4;
            op_ptr->EDS_ERROR = EDS_INVALID_SIZE;
         } else {
            op_ptr->EDS_ERROR = EDS_OK;
         } /* Endif */

         /* Calculate the number of single addressable units to read/write */
         size = op_ptr->SIZE / (4/sizeof(uint_32));

         /* Copy the data. */
         _mem_copy((pointer)&op_ptr->ADDRESS2, (pointer)op_ptr->ADDRESS, size);
         ipc_msg_ptr->HEADER.SIZE = EDS_COMMAND_SIZE;
         _eds_ipc_return_message(ipc_msg_ptr);
         break;

      default:
         _msg_free(ipc_msg_ptr);
         break;

   } /* Endswitch */

   _lwsem_post(&eds_component_ptr->SEM);

   return(MQX_OK);

} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _eds_ipc_return_message
* Returned Value   : None
* Comments         : Used by the EDS IPC handler to return a message back
*                    to the sender
*
*END**********************************************************************/

void _eds_ipc_return_message
   (
      IPC_MESSAGE_STRUCT_PTR msg_ptr
   )
{ /* Body */
   _queue_id  tmp_qid;

   /* Reverse the QIDs */
   tmp_qid = msg_ptr->HEADER.SOURCE_QID;
   msg_ptr->HEADER.SOURCE_QID = msg_ptr->HEADER.TARGET_QID;
   msg_ptr->HEADER.TARGET_QID = tmp_qid;
   msg_ptr->HEADER.CONTROL    = MSG_HDR_ENDIAN | MSG_DATA_ENDIAN;

   _msgq_send(msg_ptr);

} /* Endbody */


/*FUNCTION****************************************************************
* 
* Function Name    : _eds_timeout
* Returned Value   : None
* Comments         : This function is called if we don't get a response 
*    in time from the config reqest command sent to the other CPUs
*
*END**********************************************************************/

void _eds_timeout
   (
      _timer_id           timer_id,
      pointer             data_ptr,
      MQX_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   EDS_COMPONENT_STRUCT_PTR  eds_component_ptr;
   IPC_MESSAGE_STRUCT_PTR    ipc_msg_ptr;
   EDS_MSG_STRUCT_PTR        eds_msg_ptr;
   EDS_OP_STRUCT_PTR         op_ptr;

   eds_component_ptr = (EDS_COMPONENT_STRUCT_PTR)data_ptr;

   _lwsem_wait(&eds_component_ptr->SEM);

   eds_component_ptr->STATE    = EDS_IDLE;
   eds_component_ptr->TIMER_ID = TIMER_NULL_ID;

   ipc_msg_ptr = eds_component_ptr->SAVED_MSG_PTR;

   ipc_msg_ptr->HEADER.SOURCE_QID = eds_component_ptr->MY_QID;
   ipc_msg_ptr->HEADER.TARGET_QID = eds_component_ptr->HOST_QID;
   ipc_msg_ptr->HEADER.SIZE       = EDS_COMMAND_SIZE;

   eds_msg_ptr = (EDS_MSG_STRUCT_PTR)((pointer)ipc_msg_ptr->PARAMETERS);
   op_ptr      = &eds_msg_ptr->OP_COMMAND;

   op_ptr->OPERATION = EDS_CONFIG_REQUEST_TIMEOUT;
   op_ptr->SIZE      = 0;

   _msgq_send(ipc_msg_ptr);

   _lwsem_post(&eds_component_ptr->SEM);

} /* Endbody */
#endif /* MQX_USE_IPC */

/* EOF */
