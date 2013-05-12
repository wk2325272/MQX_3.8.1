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
* $FileName: ipc_udp.c$
* $Version : 3.8.4.0$
* $Date    : Oct-11-2011$
*
* Comments:
*
*   This file contains the source for the IPC drivers working over 
*   Ethernet/UDP.
*
*END************************************************************************/

#include "mqx.h"
#include "psp.h"

#if MQX_USE_IPC
#include "message.h"
#include "ipc.h"
#include "ipc_udp.h"
#include "ipc_udp_prv.h"

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : IPC_UDP_Init
* Returned Value  : _mqx_uint result   
*                   MQX_OK or MQX_OUT_OF_MEMORY or MQX_INVALID_PARAMETER
* Comments        :
*     This function is use to initialze the the inter-processor
* communications over Ethernet/UDP, using the ENET driver.
*
*END*---------------------------------------------------------------------*/
_mqx_uint IPC_UDP_Init( const IPC_PROTOCOL_INIT_STRUCT * ipc_init_ptr, pointer ipc_info_ptr)

{ /* Body */
    IPC_UDP_CONTEXT_PTR      context_ptr;
    context_ptr = _mem_alloc_system_zero(sizeof *context_ptr);
    if (context_ptr == NULL) 
    {
        return MQX_OUT_OF_MEMORY;
    }
    
    /* Fill-in the context structure */
    context_ptr->INIT_PTR              = ipc_init_ptr->IPC_PROTOCOL_INIT_DATA;
    context_ptr->IPC_INIT_PTR          = ipc_init_ptr;
    context_ptr->RADDR.sin_family      = AF_INET;
    context_ptr->RADDR.sin_port        = context_ptr->INIT_PTR->DEST_PORT;
    context_ptr->RADDR.sin_addr.s_addr = context_ptr->INIT_PTR->DEST;   
    
    /* Initialize UDP socket and create two IPC_UDP tasks, one for incomming and
       one for outcomming messages */
    if (!IPC_UDP_Init_internal( context_ptr )) 
    {
       if (context_ptr->POOL_ID) 
       {
            _msgpool_destroy(context_ptr->POOL_ID);
       }
       if (context_ptr->SOCKET) 
       {
           shutdown(context_ptr->SOCKET, FLAG_ABORT_CONNECTION);
       }
       if (context_ptr->IN_TID) 
       {
            _task_destroy(context_ptr->IN_TID);
       }
       if (context_ptr->OUT_TID) 
       {
            _task_destroy(context_ptr->OUT_TID);
       }
       _mem_free(context_ptr);
       return MQX_INVALID_PARAMETER; 
    }
    return MQX_OK;
} /* Endbody */

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : IPC_UDP_Init_internal
* Returned Value  : boolean   
*                   TRUE = initialization sucessful
*                   FALSE = initialization not sucessful
* Comments        :
*     This function is use to initialze the the inter-processor
* communications over Ethernet/UDP internally, using the ENET driver.
*
*END*---------------------------------------------------------------------*/
boolean IPC_UDP_Init_internal(IPC_UDP_CONTEXT_PTR context_ptr )

{ /* Body */
    sockaddr_in             laddr;
    TASK_TEMPLATE_STRUCT    task_template = { 0 };
    
    /* Create a message pool */
    context_ptr->POOL_ID = _msgpool_create(context_ptr->INIT_PTR->MAX_MSG_SIZE, IPC_UDP_POOL_INIT, IPC_UDP_POOL_GROW, IPC_UDP_POOL_MAX );
    if (context_ptr->POOL_ID == NULL)
        return FALSE;
    
    /* Create socket and bind */
    context_ptr->SOCKET  = socket(PF_INET, SOCK_DGRAM, 0);
    if (context_ptr->SOCKET == RTCS_SOCKET_ERROR)
        return FALSE;
    laddr.sin_family       = AF_INET;
    laddr.sin_port         = context_ptr->INIT_PTR->SOURCE_PORT;
    laddr.sin_addr.s_addr  = INADDR_ANY;   
    if (bind(context_ptr->SOCKET, &laddr, sizeof(laddr))!= RTCS_OK)
    {
        return FALSE;
    }
    
    /* Create two IPC_UDP tasks, one for incomming and
       one for outcomming messages  */
    task_template.TASK_STACKSIZE = context_ptr->INIT_PTR->STACK_SIZE;
    task_template.TASK_PRIORITY = context_ptr->INIT_PTR->PRIORITY;
    task_template.TASK_NAME = context_ptr->INIT_PTR->NAME;
    task_template.CREATION_PARAMETER = (uint_32) context_ptr;
    task_template.TASK_ADDRESS =  IPC_UDP_Out;
    context_ptr->OUT_TID = _task_create(0, 0, (uint_32) &task_template );
    if (context_ptr->OUT_TID == MQX_NULL_TASK_ID)
        return FALSE;
    task_template.TASK_ADDRESS =  IPC_UDP_In;
    context_ptr->IN_TID = _task_create(0, 0, (uint_32) &task_template );
    if (context_ptr->IN_TID == MQX_NULL_TASK_ID)
        return FALSE;
    
    return TRUE;
} /* Endbody */


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : IPC_UDP_In
* Returned Value  : none.
* Comments        :
*     This function of the IPC_UDP task handles input messages incomming 
* from the UDP socket.
*
*END*---------------------------------------------------------------------*/
void IPC_UDP_In( uint_32 param ) 

{ /* Body */
    IPC_UDP_CONTEXT_PTR context_ptr = (IPC_UDP_CONTEXT_PTR) param;
    MESSAGE_HEADER_STRUCT_PTR  msg_ptr = NULL;
    uint_32                    msg_size;
    uint_16                    raddr_size;
    sockaddr_in                raddr;

    RTCS_attachsock( context_ptr->SOCKET);
    while (1) 
    {
        while (msg_ptr == NULL) 
        {
            msg_ptr = _msg_alloc( context_ptr->POOL_ID);
            if (msg_ptr == NULL) 
            {
                _time_delay(10);
            }
        }
        raddr_size = sizeof(raddr);
        msg_size = recvfrom(context_ptr->SOCKET, (char_ptr)msg_ptr, context_ptr->INIT_PTR->MAX_MSG_SIZE, 0, &raddr, &raddr_size ); 
        if (msg_size == 0) 
        {
            /* Socket is closed */
            _msg_free(msg_ptr);
            return;
        }
        else if (msg_size == msg_ptr->SIZE) 
        {
            /* Send the message to the message queue */
            if (_msgq_send(msg_ptr)) 
            {
                msg_ptr = NULL;
            }
        }
    }
} /* Endbody */

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : IPC_UDP_Out
* Returned Value  : none.
* Comments        :
*     This function of the IPC_UDP task handles output messages that must be 
* sent via Ethernet/UDP.
*
*END*---------------------------------------------------------------------*/
void IPC_UDP_Out( uint_32 param ) 

{ /* Body */
    IPC_UDP_CONTEXT_PTR context_ptr = (IPC_UDP_CONTEXT_PTR) param;
    MESSAGE_HEADER_STRUCT_PTR  msg_ptr;
    _queue_id                  out_queue;
    uint_32                    bytes_sent;
    
    /* Open message queue */
    out_queue = _msgq_open(context_ptr->IPC_INIT_PTR->IPC_OUT_QUEUE, 0);
    if (out_queue == MSGQ_NULL_QUEUE_ID) 
    {
        return;
    }
    
    RTCS_attachsock( context_ptr->SOCKET);
    while (TRUE) 
    {
        /* Wait for a message to be sent */
        msg_ptr = _msgq_receive( out_queue, 0);
        if (msg_ptr == NULL) 
        {
            _msgq_close(out_queue);
            return;
        }
        bytes_sent = sendto( context_ptr->SOCKET, msg_ptr, msg_ptr->SIZE, 0, &context_ptr->RADDR, sizeof(context_ptr->RADDR));    
        _msg_free(msg_ptr);
    }
} /* Endbody */

#endif /* MQX_USE_IPC */

/* EOF */
