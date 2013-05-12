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
* $FileName: se_tdel.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function which cleans up any connections for
*   a task.  This function is called when a task is destroyed,
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SEMAPHORES
#include "name.h"
#include "name_prv.h"
#include "sem.h"
#include "sem_prv.h"

/*FUNCTION****************************************************************
* 
* Function Name    : _sem_cleanup
* Returned Value   : none
* Comments         :
*    Used during task destruction to free up any semaphore resources
* owned by this task.
*
* 
*END**********************************************************************/

void _sem_cleanup
   (
      /* [IN] the task being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   SEM_CONNECTION_STRUCT_PTR sem_connection_ptr;
   SEM_CONNECTION_STRUCT_PTR connection_ptr;
   SEM_STRUCT_PTR            sem_ptr;

   connection_ptr = _mem_get_next_block_internal(td_ptr, NULL);
   while (connection_ptr) {
      if ((connection_ptr->VALID == SEM_VALID) &&
         (connection_ptr->TD_PTR == td_ptr) )
      {
         sem_ptr = connection_ptr->SEM_PTR;
         if (sem_ptr->VALID == SEM_VALID) {
             /* Check if the connection is on the queue */
             _int_disable();
             sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)
                ((pointer)sem_ptr->WAITING_TASKS.NEXT);
             while (sem_connection_ptr != (pointer)&sem_ptr->WAITING_TASKS.NEXT){
                if (sem_connection_ptr == connection_ptr) {
                   /* Connection is queued, so dequeue it */
                   _QUEUE_REMOVE(&sem_ptr->WAITING_TASKS, connection_ptr);
                   break;
                }/* Endif */
                sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)
                   sem_connection_ptr->NEXT;
             } /* Endwhile */
             if (sem_ptr->POLICY & SEM_STRICT) {
                while (connection_ptr->POST_STATE) {
                   _sem_post(connection_ptr);
                } /* Endwhile */
             }/* Endif */
             _int_enable();
         }/* Endif */
      } /* Endif */
      connection_ptr = (SEM_CONNECTION_STRUCT_PTR)
         _mem_get_next_block_internal(td_ptr, connection_ptr);
   } /* Endwhile */

} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
