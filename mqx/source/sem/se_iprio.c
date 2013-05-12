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
* $FileName: se_iprio.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the utility function for inserting a semaphore
*   connection into a list, sorted by task priority.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SEMAPHORES
#include "name.h"
#include "name_prv.h"
#include "sem.h"
#include "sem_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sem_insert_priority_internal
* Returned Value   : none
* Comments         :
*   This function inserts a connection pointer by priority onto a queue.
* 
*END*----------------------------------------------------------------------*/

void _sem_insert_priority_internal
   (  
      /* [IN] the queue to insert the member into */
      QUEUE_STRUCT_PTR queue_ptr,

      /* [IN] the connection pointer to insert into the queue */
      SEM_CONNECTION_STRUCT_PTR sem_connection_ptr
   )
{ /* Body */
   SEM_CONNECTION_STRUCT_PTR conn2_ptr;
   SEM_CONNECTION_STRUCT_PTR conn_prev_ptr;
   _mqx_uint                  count;
   _mqx_uint                  priority;

   conn_prev_ptr = (SEM_CONNECTION_STRUCT_PTR)((pointer)queue_ptr);
   conn2_ptr     = (SEM_CONNECTION_STRUCT_PTR)((pointer)queue_ptr->NEXT);
   count         = _QUEUE_GET_SIZE(queue_ptr) + 1;
   priority      = sem_connection_ptr->TD_PTR->MY_QUEUE->PRIORITY;
   while (--count) {
      if (conn2_ptr->TD_PTR->MY_QUEUE->PRIORITY > priority) {
         break;
      } /* Endif */
      conn_prev_ptr = conn2_ptr;
      conn2_ptr     = (SEM_CONNECTION_STRUCT_PTR)conn2_ptr->NEXT;
   } /* Endwhile */
   _QUEUE_INSERT(queue_ptr, conn_prev_ptr, sem_connection_ptr);

} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
