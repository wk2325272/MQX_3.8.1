#ifndef __mutex_h__
#define __mutex_h__ 1
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
* $FileName: mutex.h$
* $Version : 3.0.5.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This include file is used to define constants and data types for the
*   mutex component
*
*END************************************************************************/

#include <mqx_cnfg.h>
#if (! MQX_USE_MUTEXES) && (! defined (MQX_DISABLE_CONFIG_CHECK))
#error MUTEX component is currently disabled in MQX kernel. Please set MQX_USE_MUTEXES to 1 in user_config.h and recompile kernel.
#endif

/*--------------------------------------------------------------------------*/
/* 
**                      CONSTANT DECLARATIONS
*/

/*
** Mutex waiting policies, only ONE may be selected
**   SPIN means the task does NOT Block
**   QUEUEING means the task blocks
*/
#define MUTEX_SPIN_ONLY            (0x01)
#define MUTEX_LIMITED_SPIN         (0x02)
#define MUTEX_QUEUEING             (0x04)
#define MUTEX_PRIORITY_QUEUEING    (0x08)

/* 
** Mutex priority policies, only ONE may be selected
**   INHERITANCE means the task owning the MUTEX runs at the priority of
**     the task with the highest priority waiting for the mutex
**   PROTECT means the task runs at the priority specified in the MUTEX
**     (unless it is of higher priority already) (priority ceiling)
**                                             
*/
#define MUTEX_NO_PRIO_INHERIT            (0x0000)
#define MUTEX_PRIO_INHERIT               (0x0100)
#define MUTEX_PRIO_PROTECT               (0x0200)

/* What a mutex valid field should be */
#define MUTEX_VALID                      (_mqx_uint)(0x6d757478)   /* "mutx" */

/* 
** The static initializer for a mutex, 
** NOTE: This implies that this mutex will be "anonymous" and
** the kernel will not know about this mutex
*/
#define MUTEX_INITIALIZER \
{ \
   NULL, NULL, MUTEX_QUEUEING, MUTEX_VALID, 0, 0, 0, 0, 0, \
   {NULL, NULL, 0, -1}, NULL \
}

/*--------------------------------------------------------------------------*/
/* 
**                      DATATYPE DECLARATIONS
*/

/*
**
** MUTEX ATTR STRUCTURE
**
** This structure defines a mutex attributes structure.
*/
typedef struct mutex_attr_struct 
{

   /* 
   ** The scheduling protocol a task using this mutex should follow when
   ** it owns the mutex.
   */
   _mqx_uint SCHED_PROTOCOL;

   /* A validation field for mutexes */
   _mqx_uint VALID;
     
   /* 
   ** The software priority of the mutex if the sched protocol is 
   ** priority ceiling 
   */
   _mqx_uint PRIORITY_CEILING;
   
   /* the limited spin count for spin wait mutexes */
   _mqx_uint COUNT;

   /* 
   ** The waiting protocol a task using this mutex should follow when
   ** a mutex is not available
   */
   _mqx_uint WAIT_PROTOCOL;

} MUTEX_ATTR_STRUCT, _PTR_ MUTEX_ATTR_STRUCT_PTR;


/*
**
** MUTEX STRUCTURE
**
** This structure defines the mutual exclusion (MUTEX) data structure.
*/
typedef struct mutex_struct 
{

   /* Link pointers to maintain a list of mutexes in the kernel */
   QUEUE_ELEMENT_STRUCT  LINK;
      
   /* The protocols that this should mutex follow */
   _mqx_uint             PROTOCOLS;

   /* A validation field for mutexes */
   _mqx_uint             VALID;
     
   /* the software priority of the mutex if the policy is priority ceiling */
   _mqx_uint             PRIORITY_CEILING;
   
   /* the limited spin count for spin wait mutexes */
   _mqx_uint             COUNT;

   /* is the mutex being destroyed? */
   uint_16               DELAYED_DESTROY;
   
   /* The actual mutex */
   uchar                 LOCK;
   
   /* An alignment filler */
   uchar                 FILLER;
   
   /* A queue of tasks waiting for the mutex */
   QUEUE_STRUCT          WAITING_TASKS;

   /* What task owns this mutex */
   pointer               OWNER_TD;
   
   /* Has the owner task had it's priority boosted? and how many times */
   _mqx_uint             BOOSTED;

} MUTEX_STRUCT, _PTR_ MUTEX_STRUCT_PTR;


/*--------------------------------------------------------------------------*/
/* 
**                      'C' FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TAD_COMPILE__
extern _mqx_uint _mutatr_destroy(MUTEX_ATTR_STRUCT_PTR);
extern _mqx_uint _mutatr_get_priority_ceiling(MUTEX_ATTR_STRUCT_PTR, _mqx_uint_ptr);
extern _mqx_uint _mutatr_get_sched_protocol(MUTEX_ATTR_STRUCT_PTR, _mqx_uint_ptr);
extern _mqx_uint _mutatr_get_spin_limit(MUTEX_ATTR_STRUCT_PTR, _mqx_uint_ptr);
extern _mqx_uint _mutatr_get_wait_protocol(MUTEX_ATTR_STRUCT_PTR, _mqx_uint_ptr);
extern _mqx_uint _mutatr_set_priority_ceiling(MUTEX_ATTR_STRUCT_PTR, _mqx_uint);
extern _mqx_uint _mutatr_set_sched_protocol(MUTEX_ATTR_STRUCT_PTR, _mqx_uint);
extern _mqx_uint _mutatr_set_spin_limit(MUTEX_ATTR_STRUCT_PTR, _mqx_uint);
extern _mqx_uint _mutatr_set_wait_protocol(MUTEX_ATTR_STRUCT_PTR, _mqx_uint);
extern _mqx_uint _mutatr_init(MUTEX_ATTR_STRUCT_PTR);

/* Mutex functions */
extern _mqx_uint _mutex_create_component( void );
extern _mqx_uint _mutex_destroy(MUTEX_STRUCT_PTR);
extern _mqx_uint _mutex_get_priority_ceiling(MUTEX_STRUCT_PTR, _mqx_uint_ptr);
extern _mqx_uint _mutex_get_wait_count(MUTEX_STRUCT_PTR);
extern _mqx_uint _mutex_init(MUTEX_STRUCT_PTR, MUTEX_ATTR_STRUCT_PTR);
extern _mqx_uint _mutex_lock(MUTEX_STRUCT_PTR);
extern _mqx_uint _mutex_set_priority_ceiling(MUTEX_STRUCT_PTR, _mqx_uint, _mqx_uint_ptr);
extern _mqx_uint _mutex_test(pointer _PTR_);
extern _mqx_uint _mutex_try_lock(MUTEX_STRUCT_PTR);
extern _mqx_uint _mutex_unlock(MUTEX_STRUCT_PTR);
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
