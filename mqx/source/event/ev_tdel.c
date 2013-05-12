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
* $FileName: ev_tdel.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for cleaning up events when a task
*   is deleted.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_EVENTS
#include "name.h"
#include "name_prv.h"
#include "event.h"
#include "evnt_prv.h"

/*FUNCTION*------------------------------------------------------------
* 
* Function Name    : _event_cleanup
* Returned Value   : none
* Comments         :
*    Used during task destruction to free up any events resources
* owned by this task.
* The function walks the resource list of the task looking for 
* event connection struct.  It can detect one by looking for EVENT_VALID
* in the correct location.
* 
*END*------------------------------------------------------------------*/

void _event_cleanup
   ( 
      /* [IN] the task descriptor of the task being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   EVENT_CONNECTION_STRUCT_PTR event_connection_ptr;
   EVENT_CONNECTION_STRUCT_PTR connection_ptr;
   EVENT_STRUCT_PTR            event_ptr;

   connection_ptr = _mem_get_next_block_internal(td_ptr, NULL);
   while (connection_ptr) {
      if ((connection_ptr->VALID == EVENT_VALID) &&
         (connection_ptr->TD_PTR == td_ptr) )
      {
         event_ptr = connection_ptr->EVENT_PTR;
         if (event_ptr->VALID == EVENT_VALID) {
             /* Check if the connection is on the queue */
             _int_disable();
             event_connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)
                ((pointer)event_ptr->WAITING_TASKS.NEXT);
             while (event_connection_ptr != 
                (pointer)&event_ptr->WAITING_TASKS.NEXT) 
             {
                if (event_connection_ptr == connection_ptr) {
                   /* Connection is queued, so dequeue it */
                   _QUEUE_REMOVE(&event_ptr->WAITING_TASKS, connection_ptr);
                   break;
                }/* Endif */
                event_connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)
                   event_connection_ptr->NEXT;
             } /* Endwhile */
             _int_enable();
         }/* Endif */
      } /* Endif */
      connection_ptr = (EVENT_CONNECTION_STRUCT_PTR)
         _mem_get_next_block_internal(td_ptr, connection_ptr);
   } /* Endwhile */

} /* Endbody */
#endif /* MQX_USE_EVENTS */

/* EOF */
