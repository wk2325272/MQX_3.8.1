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
* $FileName: se_util.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains utility functions for the Semaphore component.
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
* Function Name    : _sem_get_value
* Returned Value   : _mqx_uint current sem value or MAX_MQX_UINT on error
*                    If error the task error code is set to one of:
*                       SEM_INVALID_SEMAPHORE_HANDLE
* Comments         :
*   this function returns the current value of the semaphore.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_get_value
   (
      /* [IN] -  The semaphore handle returned by _sem_open. */
      pointer users_sem_ptr
   )
{ /* Body */
   register SEM_CONNECTION_STRUCT_PTR sem_connection_ptr;
   register SEM_STRUCT_PTR            sem_ptr;
   
   sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)users_sem_ptr;
#if MQX_CHECK_VALIDITY
   if (sem_connection_ptr->VALID != SEM_VALID) {
      _task_set_error(SEM_INVALID_SEMAPHORE_HANDLE);
      return(MAX_MQX_UINT); 
   } /* Endif */
#endif

   sem_ptr = sem_connection_ptr->SEM_PTR;
#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != SEM_VALID) {
      _task_set_error(SEM_INVALID_SEMAPHORE);
      return(MAX_MQX_UINT); 
   } /* Endif */
#endif

   return(sem_ptr->COUNT);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sem_get_wait_count
* Returned Value   : _mqx_uint current num tasks on waiting list or MAX_MQX_UINT
*                    on error.
*                    If error the task error code is set to one of:
*                       SEM_INVALID_SEMAPHORE_HANDLE
* Comments         :
*   This function returns the number of tasks waiting for the specified
* semaphore
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_get_wait_count
   (
      /* [IN] -  The semaphore handle returned by _sem_open. */
      pointer users_sem_ptr
   )
{ /* Body */
   register SEM_CONNECTION_STRUCT_PTR sem_connection_ptr;
   register SEM_STRUCT_PTR            sem_ptr;
   
   sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)users_sem_ptr;
#if MQX_CHECK_VALIDITY
   if (sem_connection_ptr->VALID != SEM_VALID) {
      _task_set_error(SEM_INVALID_SEMAPHORE_HANDLE);
      return(MAX_MQX_UINT); 
   } /* Endif */
#endif

   sem_ptr = sem_connection_ptr->SEM_PTR;
#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != SEM_VALID) {
      _task_set_error(SEM_INVALID_SEMAPHORE);
      return(MAX_MQX_UINT); 
   } /* Endif */
#endif

   return(_QUEUE_GET_SIZE(&(sem_ptr->WAITING_TASKS)));

} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
