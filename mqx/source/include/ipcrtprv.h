#ifndef __ipcrtprv_h__
#define __ipcrtprv_h__ 1
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
* $FileName: ipcrtprv.h$
* $Version : 3.0.4.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains the definitions private to the router used to route
*   IPC messages.
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*
**                          CONSTANT DECLARATIONS
*/

/*--------------------------------------------------------------------------*/
/*
**                          DATATYPE DECLARATIONS
*/

/*
** IPC MSG ROUTING STRUCT
**
** This structure contains info for a particular route
*/
typedef struct ipc_msg_routing_struct
{

   /* Used to link all routing structures together */
   QUEUE_ELEMENT_STRUCT  LINK;

   /* The minimum processor number in the range */
   _processor_number     MIN_PROC_NUMBER;

   /* The maximum processor number in the range */
   _processor_number     MAX_PROC_NUMBER;

   /* The queue to use if the processor number is in the above range */
   _queue_number         QUEUE;

} IPC_MSG_ROUTING_STRUCT, _PTR_ IPC_MSG_ROUTING_STRUCT_PTR;

/*
** IPC MSG ROUTING COMPONENT STRUCT
**
** The structure kept in the kernel data by the message routing component
*/
typedef struct ipc_msg_routing_component_struct 
{
   /* Linked list of routes installed */
   QUEUE_STRUCT         ROUTING_LIST;

   boolean  (_CODE_PTR_ MSG_ROUTER)(_processor_number, pointer, boolean);
   
} IPC_MSG_ROUTING_COMPONENT_STRUCT, _PTR_ IPC_MSG_ROUTING_COMPONENT_STRUCT_PTR;

/*--------------------------------------------------------------------------*/
/*
**                          C PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TAD_COMPILE__
extern _mqx_uint _ipc_msg_route_init_internal( const IPC_ROUTING_STRUCT * route_ptr );
extern _mqx_uint _ipc_msg_route_internal(_processor_number, pointer, boolean);
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
