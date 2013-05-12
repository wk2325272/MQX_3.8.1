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
* $FileName: se_close.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for closing a connection to a semaphore.
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
* Function Name    : _sem_close
* Returned Value   : _mqx_uint MQX_OK or SEM_INVALID_SEMAPHORE_HANDLE,
*                       SEM_INVALID_SEMAPHORE_HANDLE
* Comments         :
*   This function closes a connection to an instance of a semaphore
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_close
   (
      /* [IN/OUT] -  The semaphore handle returned by _sem_open. */
      pointer users_sem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;
   SEM_CONNECTION_STRUCT_PTR sem_connection_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_sem_close, users_sem_ptr);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_sem_close, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)users_sem_ptr;
#if MQX_CHECK_ERRORS
   if (sem_connection_ptr->TD_PTR != kernel_data->ACTIVE_PTR) {
      _KLOGX2(KLOG_sem_close, SEM_INVALID_SEMAPHORE_HANDLE);
      return(SEM_INVALID_SEMAPHORE_HANDLE);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (sem_connection_ptr->VALID != SEM_VALID) {
      _KLOGX2(KLOG_sem_close, SEM_INVALID_SEMAPHORE_HANDLE);
      return(SEM_INVALID_SEMAPHORE_HANDLE); 
   } /* Endif */
#endif

   if (sem_connection_ptr->SEM_PTR->VALID) {
      /* Release all held semaphores */
      while (sem_connection_ptr->POST_STATE > 0) {
         _sem_post(sem_connection_ptr);
      } /* Endwhile */
   } /* Endif */

   sem_connection_ptr->VALID = 0;

   if (_mem_free(sem_connection_ptr) != MQX_OK) {
      _KLOGX2(KLOG_sem_close, kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
      return(kernel_data->ACTIVE_PTR->TASK_ERROR_CODE);
   } else {
      _KLOGX2(KLOG_sem_close, MQX_OK);
      return(MQX_OK);
   } /* Endif */

} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
