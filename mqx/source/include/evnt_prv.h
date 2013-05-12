#ifndef __evnt_prv_h__
#define __evnt_prv_h__ 1
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
* $FileName: evnt_prv.h$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This include file is used to define constants and data types private
*   to the event component.
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*                        CONSTANT DEFINITIONS                              */

/* Used to mark a block of memory as belonging to an event group */
#define EVENT_VALID                    ((_mqx_uint)(0x65766E74))   /* "evnt" */

/* Used to indicate that an event occurred */
#define EVENT_OCCURRED                 (2)
#define EVENT_WANTS_ALL                (1)

#define EVENT_MAX_WAITING_TASKS        ((uint_16)0)

/* IPC Message types for remote event access */
#define IPC_EVENT_OPEN                 (1)
#define IPC_EVENT_SET                  (2)

/*--------------------------------------------------------------------------*/
/*                      DATA STRUCTURE DEFINITIONS                          */

/* 
** EVENT COMPONENT STRUCTURE
**
** This is the base structure pointed to by the kernel data
** structure COMPONENT field for events
*/
typedef struct event_component_struct
{

   /* The maximum number of event instances allowed */
   _mqx_uint        MAXIMUM_NUMBER;

   /* The number of event instances to grow by when table full */
   _mqx_uint        GROW_NUMBER;

   /* A validation stamp allowing for checking of memory overwrite. */
   _mqx_uint        VALID;

   /* The handle to the naming table for events */
   pointer         NAME_TABLE_HANDLE;

} EVENT_COMPONENT_STRUCT, _PTR_ EVENT_COMPONENT_STRUCT_PTR;


/* 
** EVENT STRUCTURE
**
** This is the structure of an instance of an event.  The address
** is kept in the event name table, associated with the name.
*/
typedef struct event_struct
{

   /* This is the queue of waiting tasks.
   ** What is queued is the address  of the handle provided 
   ** to the user (EVENT_COMPONENT_STRUCT)
   */
   QUEUE_STRUCT    WAITING_TASKS;

   /* This is a validation stamp for the event */
   _mqx_uint       VALID;
   
   /* the actual event bits */
   _mqx_uint       EVENT;

   /* event type */
   boolean         AUTO_CLEAR;
   
   /* The string name of the event, includes null */
   char            NAME[NAME_MAX_NAME_SIZE];

} EVENT_STRUCT, _PTR_ EVENT_STRUCT_PTR;


/* 
** EVENT CONNECTION STRUCTURE
**    This is the structure whose address is returned to the user
** as an event handle.
*/
typedef struct event_connection_struct
{

   /* 
   ** These pointers are used to link the connection struct onto
   ** the WAITING TASK queue of the event
   */
   pointer           NEXT;
   pointer           PREV;

   /* This is a validation stamp for the data structure */
   _mqx_uint         VALID;

   /* 
   ** Is this event on a remote procssor, if non-zero it is the processor 
   ** number of the remote processor.
   */
   _mqx_uint         REMOTE_CPU;

   /* The bit mask of bits to wait for */
   _mqx_uint         MASK;
   
   /* A flag indicating whether all bits are required, or whether
   ** an event has been set
   */
   _mqx_uint         FLAGS;
   
   /* The address of the task descriptor that owns this connection */
   TD_STRUCT_PTR     TD_PTR;

   /* The address of the event structure associated with this connection */
   EVENT_STRUCT_PTR  EVENT_PTR;

    
} EVENT_CONNECTION_STRUCT, _PTR_ EVENT_CONNECTION_STRUCT_PTR;

/* ANSI c prototypes */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TAD_COMPILE__
extern void      _event_cleanup(TD_STRUCT_PTR);
extern _mqx_uint _event_create_internal(char _PTR_, EVENT_STRUCT_PTR _PTR_);
extern _mqx_uint _event_create_fast_internal(_mqx_uint, EVENT_STRUCT_PTR _PTR_);
extern _mqx_uint _event_wait_internal(pointer, _mqx_uint, MQX_TICK_STRUCT_PTR, 
   boolean, boolean);
/* Start CR 1947 */
extern _mqx_uint _event_wait_any_internal(pointer, _mqx_uint, MQX_TICK_STRUCT_PTR, 
   boolean);
/* End CR 1947 */   
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
