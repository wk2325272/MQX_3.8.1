/**HEADER********************************************************************
* 
* Copyright (c) 2012 Freescale Semiconductor;
* All Rights Reserved                       
*
* Copyright (c) 1989-2012 ARC International;
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
* $FileName: ipc.c$
* $Version : 3.8.0.1$
* $Date    : Feb-21-2012$
*
* Comments:
*
*   This file contains the source for one of the IPC program examples.
*   This program should be started second.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include <ipc.h>
#include <ipc_pcb.h>
#include <io_pcb.h>
#include <pcb_shm.h>
#include "ipc_ex.h"


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



extern void main_task(uint_32);
extern void responder_task(uint_32);



const IPC_ROUTING_STRUCT core0_routing_table[] =
{
   { BSP_CORE_1_PROCESSOR_NUMBER, BSP_CORE_1_PROCESSOR_NUMBER, QUEUE_TO_REMOTE },
   { 0, 0, 0 }
};

const IPC_ROUTING_STRUCT core1_routing_table[] =
{
   { BSP_CORE_0_PROCESSOR_NUMBER, BSP_CORE_0_PROCESSOR_NUMBER, QUEUE_TO_REMOTE },
   { 0, 0, 0 }
};

const IO_PCB_SHM_INIT_STRUCT pcb_shm_init = 
{
    /* TX_BD_ADDR       */ BSP_SHARED_RAM_START,
    /* TX_LIMIT_ADDR    */ (uchar_ptr)(BSP_SHARED_RAM_START)+1024,
    /* RX_BD_ADDR       */ BSP_REMOTE_SHARED_RAM_START,
    /* RX_LIMIT_ADDR    */ (uchar_ptr)(BSP_REMOTE_SHARED_RAM_START)+1024,
    /* INPUT_MAX_LENGTH */ 128,
    /* RX_VECTOR        */ MPXS30_INTC_SSCIR0_VECTOR,
    /* TX_VECTOR        */ MPXS30_INTC_SSCIR1_VECTOR,
    /* REMOTE_RX_VECTOR */ MPXS30_INTC_SSCIR0_VECTOR,
    /* REMOTE_TX_VECTOR */ MPXS30_INTC_SSCIR1_VECTOR,
    /* INT_TRIGGER      */ _bsp_io_pcb_shm_int_trigger
};

const IPC_PCB_INIT_STRUCT pcb_init =
{
   /* IO_PORT_NAME */             "pcb_shmem:",
   /* DEVICE_INSTALL? */          _io_pcb_shm_install,
   /* DEVICE_INSTALL_PARAMETER*/  (pointer)&pcb_shm_init,
   /* IN_MESSAGES_MAX_SIZE */     sizeof(THE_MESSAGE),
   /* IN MESSAGES_TO_ALLOCATE */  8,
   /* IN MESSAGES_TO_GROW */      8,
   /* IN_MESSAGES_MAX_ALLOCATE */ 16,
   /* OUT_PCBS_INITIAL */         8,
   /* OUT_PCBS_TO_GROW */         8,
   /* OUT_PCBS_MAX */             16
};

const IPC_PROTOCOL_INIT_STRUCT ipc_init_table[] =
{
   { _ipc_pcb_init, (pointer)&pcb_init, "core_ipc_pcb", QUEUE_TO_REMOTE },
   { NULL, NULL, NULL, 0}
};
const IPC_INIT_STRUCT core0_ipc_init = {
    core0_routing_table,
    ipc_init_table
};

const IPC_INIT_STRUCT core1_ipc_init = {
    core1_routing_table,
    ipc_init_table
};

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,    Function,       Stack,                  Priority, Name,        Attributes,          Param, Time Slice */
    { IPC_TTN,       _ipc_task,      IPC_DEFAULT_STACK_SIZE, 8,        "_ipc_task", MQX_AUTO_START_TASK, (uint_32) &core0_ipc_init,     0 },
    { MAIN_TTN,      main_task,      2000,                   9,        "Main",      MQX_AUTO_START_TASK, BSP_CORE_1_PROCESSOR_NUMBER,   0 },
    { 0 }
};

TASK_TEMPLATE_STRUCT  MQX_template_list_1[] = 
{
   /* Task Index,    Function,       Stack,                  Priority,  Name,        Attributes,          Param, Time Slice */
    { IPC_TTN,       _ipc_task,      IPC_DEFAULT_STACK_SIZE, 8,         "_ipc_task", MQX_AUTO_START_TASK, (uint_32) &core1_ipc_init,    0 },
    { RESPONDER_TTN, responder_task, 2000,                   9,         "Responder", MQX_AUTO_START_TASK, 0,                            0 },
    { 0 }
};


void log_message(char_ptr message)
{
   static uint_32 msg_count = 0;
   msg_count++;
   printf(message);
}
void log_error(char_ptr error)
{
    static uint_32 err_count = 0;
    err_count++;
    printf(error);
}
/*TASK*----------------------------------------------------------
*
* Task Name : main_task
* Comments  : 
*     This task creates a message pool and a message queue then 
*     sends a message to a queue on the second CPU.
*     It waits for a return message, validating the message before
*     sending a new message.
*END*-----------------------------------------------------------*/

void main_task(uint_32 dest_core) 
{
    _pool_id        msgpool;
    THE_MESSAGE_PTR msg_ptr;
    _queue_id       my_qid,temp_qid;
    uint_32         expected;
    
    printf("\n\n\nMain task started\n");
    
    // wait for P1 to boot - 
    _time_delay(1000);
    
    my_qid   = _msgq_open(MAIN_QUEUE,0);
    msgpool = _msgpool_create(sizeof(THE_MESSAGE), 8, 8, 0);
    msg_ptr = (THE_MESSAGE_PTR)_msg_alloc(msgpool);
    
    if (msg_ptr != NULL) {
        msg_ptr->HEADER.TARGET_QID = _msgq_get_id((_processor_number) dest_core,RESPONDER_QUEUE);
        msg_ptr->HEADER.SOURCE_QID = my_qid;
        msg_ptr->DATA = 0;
    }
    
    while (msg_ptr != NULL) {
        expected = msg_ptr->DATA+1;
        printf("Main task sending\n");
        _msgq_send(msg_ptr);
        printf("Main task receiving...");
        msg_ptr = _msgq_receive(MSGQ_ANY_QUEUE, 0);
        printf("done\n");
        if (msg_ptr != NULL) {
           printf("Message: Size=%x, SQID= %x, TQID=%x, DATA = %x\n", msg_ptr->HEADER.SIZE, msg_ptr->HEADER.SOURCE_QID,
               msg_ptr->HEADER.TARGET_QID, msg_ptr->DATA );
           if (msg_ptr->HEADER.SIZE != sizeof(THE_MESSAGE)) {
                log_error("Message wrong size\n");
           } else if (msg_ptr->DATA != expected) {
                log_error("Message data incorrect\n");
           }
           temp_qid                   = msg_ptr->HEADER.SOURCE_QID;
           msg_ptr->HEADER.SOURCE_QID = msg_ptr->HEADER.TARGET_QID;
           msg_ptr->HEADER.TARGET_QID = temp_qid;
           msg_ptr->DATA++;
        }
    }
    log_error("Message alloc/receive failed\n");
} 

/*TASK*----------------------------------------------------------
*
* Task Name : responder_task
* Comments  : 
*     This task creates a message queue then waits for a message.
*     Upon receiving the message the data is incremented before
*     the message is returned to the sender.
*END*-----------------------------------------------------------*/

void responder_task(uint_32 param) 
{
    THE_MESSAGE_PTR    msg_ptr;
    _queue_id          my_qid,temp_qid;

    printf("\n\n\nResponder task started\n");
    my_qid = _msgq_open(RESPONDER_QUEUE,0);
    while (TRUE) {
        printf("Responder task receiving...");
        msg_ptr = _msgq_receive(MSGQ_ANY_QUEUE,0);
        printf("done\n");
        if (msg_ptr != NULL) {
            printf("Message: Size=%x, SQID= %x, TQID=%x, DATA = %x\n", msg_ptr->HEADER.SIZE, msg_ptr->HEADER.SOURCE_QID,
                msg_ptr->HEADER.TARGET_QID, msg_ptr->DATA );
            // Swap source & Destination
            temp_qid                   = msg_ptr->HEADER.SOURCE_QID;
            msg_ptr->HEADER.SOURCE_QID = msg_ptr->HEADER.TARGET_QID;
            msg_ptr->HEADER.TARGET_QID = temp_qid;
            msg_ptr->DATA++;
            _msgq_send(msg_ptr);
        } else {
            log_error("Responder task receive error\n");
        }
    }
} 

