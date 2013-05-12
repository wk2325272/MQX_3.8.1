/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved                       
*
* Copyright (c) 2004-2010 Embedded Access Inc.;
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
* $FileName: ipc_udp_2.c$
* $Version : 3.8.11.1$
* $Date    : May-28-2012$
*
* Comments:
*
*   This file contains the source for one of the IPC_UDP program examples.
*   This program should be started first.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include <ipc.h>
#include <ipc_udp.h>
#include <ipcfg.h>
#include "..\ipc_udp_ex.h"


#if ! MQX_USE_IPC
#error This application requires MQX_USE_IPC defined non-zero in user_config.h. Please recompile libraries with this option.
#endif


#if ! MQX_IS_MULTI_PROCESSOR
#error This application requires MQX_IS_MULTI_PROCESSOR defined non-zero in user_config.h. Please recompile libraries with this option.
#endif


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if ! RTCSCFG_ENABLE_UDP
#error This application requires RTCSCFG_ENABLE_UDP defined non-zero in user_config.h. Please recompile libraries with this option.
#endif


#define ENET_DEVICE BSP_DEFAULT_ENET_DEVICE

/*
** The target's IP address and netmask and gateway
*/

#ifndef ENET_IPADDR
    #define ENET_IPADDR  IPADDR(192,168,2,21) 
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,254,0) 
#endif

#ifndef ENET_IPGATEWAY
    #define ENET_IPGATEWAY  IPADDR(192,168,2,1) 
#endif



extern void responder_task(uint_32);
void Initialize_RTCS_and_IPC(void);


IPC_UDP_INIT ipc_udp_init_io_processor = 
{
   /* NAME */         "IPC_UDP", 
   /* STACK_SIZE */   2000, 
   /* PRIORITY */     7,
   /* MAX_MSG_SIZE */ 1024, 
   /* SOURCE_PORT */  4000, 
   /* DEST_PORT */    4000, 
   /* DEST */         IPADDR(192,168,2,20) 
};

const IPC_ROUTING_STRUCT ipc_routing_table[] =
{
   { TEST_ID, TEST_ID, QUEUE_TO_TEST },
   { 0, 0, 0 }
};

const IPC_PROTOCOL_INIT_STRUCT ipc_init_table[] =
{
   { IPC_UDP_Init, &ipc_udp_init_io_processor, "IPC_UDP -> I/O#1", QUEUE_TO_TEST },
   { NULL, NULL, NULL, 0}
};

static const IPC_INIT_STRUCT ipc_init = {
    ipc_routing_table,
    ipc_init_table
};

TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,    Function,       Stack,                  Priority,  Name,        Attributes,          Param, Time Slice */
    { IPC_TTN,       _ipc_task,      IPC_DEFAULT_STACK_SIZE, 8,         "_ipc_task", 0,                   (uint_32)&ipc_init,     0 },
    { RESPONDER_TTN, responder_task, 2000,                   9,         "Responder", MQX_AUTO_START_TASK, 0,                      0 },
    { 0 }
};

MQX_INITIALIZATION_STRUCT  MQX_init_struct =
{
   /* PROCESSOR_NUMBER */                 TEST2_ID,
   /* START_OF_KERNEL_MEMORY */           BSP_DEFAULT_START_OF_KERNEL_MEMORY,
   /* END_OF_KERNEL_MEMORY */             BSP_DEFAULT_END_OF_KERNEL_MEMORY,
   /* INTERRUPT_STACK_SIZE */             BSP_DEFAULT_INTERRUPT_STACK_SIZE,
   /* TASK_TEMPLATE_LIST */               (pointer)MQX_template_list,
   /* MQX_HARDWARE_INTERRUPT_LEVEL_MAX */ BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX,
   /* MAX_MSGPOOLS */                     8,
   /* MAX_MSGQS */                        16,
   /* IO_CHANNEL */                       BSP_DEFAULT_IO_CHANNEL,
   /* IO_OPEN_MODE */                     BSP_DEFAULT_IO_OPEN_MODE
};

uchar global_endian_swap_def[2] = {4, 0};

/*TASK*----------------------------------------------------------
*
* Task Name : responder_task
* Comments  : 
*     This task creates a message queue then waits for a message.
*     Upon receiving the message the data is incremented before
*     the message is returned to the sender.
*END*-----------------------------------------------------------*/

void responder_task
   (
      uint_32 dummy
   ) 
{
   THE_MESSAGE_PTR    msg_ptr;
   _queue_id          qid;
   _queue_id          my_qid;

   Initialize_RTCS_and_IPC();
   
   puts("Receiver running...\n");
   my_qid = _msgq_open(RESPONDER_QUEUE,0);
   while (TRUE) {
      msg_ptr = _msgq_receive(MSGQ_ANY_QUEUE,0);
      if (msg_ptr != NULL) {
         qid      = msg_ptr->HEADER.SOURCE_QID;
         msg_ptr->HEADER.SOURCE_QID = my_qid;
         msg_ptr->HEADER.TARGET_QID = qid;
         if (MSG_MUST_CONVERT_DATA_ENDIAN(msg_ptr->HEADER.CONTROL)) {
            _mem_swap_endian(global_endian_swap_def, &msg_ptr->DATA);
            MSG_SET_DATA_ENDIAN(msg_ptr->HEADER.CONTROL);
         } /* Endif */                               
         msg_ptr->DATA++;
         putchar('+');
         _msgq_send(msg_ptr);
      } else {
         puts("RESPONDER RECEIVE ERROR\n");
         _task_block();
      }
   }
} 

void Initialize_RTCS_and_IPC(void) 
{
    uint_32                  error;
    IPCFG_IP_ADDRESS_DATA    ip_data;
    _enet_address            enet_address = BSP_DEFAULT_ENET_OUI;
   
    error = RTCS_create();
    if (error != RTCS_OK) 
    {
        printf("\nRTCS failed to initialize, error = %X", error);
        _task_block();
    }

#ifdef ENET_DEVICE
    ENET_get_mac_address (ENET_DEVICE, ENET_IPADDR, enet_address);
#endif

    ip_data.ip = ENET_IPADDR;
    ip_data.mask = ENET_IPMASK;
    ip_data.gateway = ENET_IPGATEWAY;

    if ((ipcfg_init_device (ENET_DEVICE, enet_address)) != IPCFG_OK)
    {
        printf("\nRTCS error: ipcfg_init_device\n");
    }
    if ((ipcfg_bind_staticip (ENET_DEVICE, &ip_data)) != IPCFG_OK)
    {
        printf("\nRTCS error: ipcfg_bind_staticip\n");
    }


    /* Create the IPC task. Must be done after RTCS is created. */
    _task_create(0,IPC_TTN,(uint_32)&ipc_init);

}
/* EOF */
