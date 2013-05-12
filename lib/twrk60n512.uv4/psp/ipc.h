#ifndef __ipc_h__
#define __ipc_h__ 1
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
* $FileName: ipc.h$
* $Version : 3.8.8.0$
* $Date    : Oct-13-2011$
*
* Comments:
*
*   This file contains the definitions for the interprocessor 
*   communications component.
*
*END************************************************************************/

#include <mqx_cnfg.h>
#if (!MQX_USE_IPC) && (! defined (MQX_DISABLE_CONFIG_CHECK))
#error IPC component is currently disabled in MQX kernel. Please set MQX_USE_IPC to 1 in user_config.h and recompile kernel.
#endif

/*--------------------------------------------------------------------------*/
/*
**                          CONSTANT DECLARATIONS
*/

/*
** Error messages
*/
#define IPC_ROUTE_EXISTS            (IPC_ERROR_BASE|0x01)

/*
** Define the stack size for the IPC
*/
#define IPC_DEFAULT_STACK_SIZE \
   ((384*sizeof(_mem_size))+PSP_STACK_KLOG_OVERHEAD)

/* The size of the data portion of the IPC message in uint_32s */
#define IPC_DATA_SIZE              (19)

/* Maximum packet size sent between IPC tasks */
#define IPC_MAX_PACKET_SIZE \
   ((IPC_DATA_SIZE * sizeof(_mqx_uint)) + /* Data bytes */ \
   (4 * sizeof(_mqx_uint)))  /* Maximum size of message header */

/*
** Types of Inter-processor low level protocols
*/
#define IPC_LOOPBACK_PROTOCOL       (1)
#define IPC_ASYNC_SERIAL_PROTOCOL   (2)
#define IPC_SYNC_SERIAL_PROTOCOL    (3)
#define IPC_SHARED_MEM_PROTOCOL     (4)
#define IPC_TI_COMPORT_PROTOCOL     (5)
#define IPC_SHARC_LINKPORT_PROTOCOL (6)
#define IPC_LAPB_PROTOCOL           (7)
#define IPC_RTCS_PROTOCOL           (8)
#define IPC_PCB_PROTOCOL            (9)

/*--------------------------------------------------------------------------*/
/*
**                          DATATYPE DECLARATIONS
*/

/*
** IPC PROTOCOL INIT STRUCT
** The standard inter-processor initialization information
*/

typedef struct ipc_protocol_init_struct
{

   /* The function to call to initialize the inter-processor protocol */
   _mqx_uint (_CODE_PTR_ IPC_PROTOCOL_INIT)(
      const struct ipc_protocol_init_struct _PTR_ ipc_init_ptr,
      pointer ipc_info_ptr);

   /* The address of the protocol specific initialization data. */
   pointer               IPC_PROTOCOL_INIT_DATA;

   /* A string name to identify this driver. */
   char _PTR_            IPC_NAME;

   /* This is the output queue number to which the kernel will send messages
   ** that are to be sent to the remote processor
   */
   _queue_number         IPC_OUT_QUEUE;

} IPC_PROTOCOL_INIT_STRUCT, _PTR_ IPC_PROTOCOL_INIT_STRUCT_PTR;


/*
** IPC_ROUTING_STRUCT
**
** The routing table for inter-processor communications
** When a request to a processor that is OFF board,
** The processor number is checked versus the minimum and maximum
** processor number in the _IP_routing_table, which is composed of
** these structures.  If a match is found, then the message is 
** in fact placed onto the specified QUEUE number.
*/

typedef struct ipc_routing_struct
{
   /* The minimum processor number in the range */
   _processor_number MIN_PROC_NUMBER;

   /* The maximum processor number in the range */
   _processor_number MAX_PROC_NUMBER;

   /* The queue to use if the processor number is in the above range */
   _queue_number     QUEUE;

} IPC_ROUTING_STRUCT, _PTR_ IPC_ROUTING_STRUCT_PTR;


/*
** IPC PROTOCOL INFO STRUCT
** This structure contains standard Inter-Processor protocol information,
** kept with the inter-processor driver.
*/
typedef struct ipc_protocol_info_struct
{

   /* What type of inter-processor communications protocol is this? */
   _mqx_uint           IPC_TYPE;

   /* This is the output queue number to which MQX+M will send messages
   ** that are to be sent to the remote processor
   */
   _queue_number       IPC_OUT_QUEUE;

   /* A string name used to identify this IPC, from the init record */
   char _PTR_          IPC_NAME;

   /* The queue ID of the output queue */
   _queue_id           IPC_OUT_QID;

   /* The address of protocol specific information */
   pointer             IPC_PROTOCOL_INFO_PTR;
   
   /* The address of the initialization record used to initialize this IPC */
   const IPC_PROTOCOL_INIT_STRUCT * IPC_INIT_PTR;

} IPC_PROTOCOL_INFO_STRUCT, _PTR_ IPC_PROTOCOL_INFO_STRUCT_PTR;

typedef struct ipc_init_struct {
    const IPC_ROUTING_STRUCT *          ROUTING_LIST_PTR;
    const IPC_PROTOCOL_INIT_STRUCT *   PROTOCOL_LIST_PTR;
} IPC_INIT_STRUCT, * IPC_INIT_STRUCT_PTR
;
/*--------------------------------------------------------------------------*/
/*
**                          C PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

extern const IPC_PROTOCOL_INIT_STRUCT _ipc_init_table[];
extern const IPC_ROUTING_STRUCT       _ipc_routing_table[];

/* Component IPC handlers */
#ifndef __TAD_COMPILE__
extern void _event_install_ipc_handler(void);

extern _mqx_uint _ipc_add_io_ipc_handler(_mqx_uint (_CODE_PTR_ handler)(pointer),
   _mqx_uint);
extern _mqx_uint _ipc_add_ipc_handler(_mqx_uint (_CODE_PTR_ handler)(pointer),
   _mqx_uint);
extern _mqx_uint _ipc_msg_route_add(_processor_number, _processor_number,
   _queue_number);
extern pointer   _ipc_msg_processor_route_exists(_processor_number);
extern _mqx_uint _ipc_msg_route_remove(_processor_number, _processor_number,
   _queue_number);
extern void      _ipc_task(uint_32);
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
