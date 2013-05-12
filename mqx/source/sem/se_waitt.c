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
* $FileName: se_waitt.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for waiting on a semaphore.
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
* Function Name    : _sem_wait_ticks
* Returned Value   : _mqx_uint MQX_OK, or SEM_INVALID_SEMAPHORE_HANDLE,
*                       SEM_INVALID_SEMAPHORE_COUNT, SEM_WAIT_TIMEOUT
* Comments         :
*   This function waits for a semaphore to become available.  If one is not
* the task is queued according to the queueing policy for this semaphore.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_wait_ticks
   (
      /* [IN] -  The semaphore handle returned by _sem_open. */
      pointer  users_sem_ptr,

      /* 
      ** [IN] - the number of ticks to wait for a semaphore.
      **        If the value is 0, then the timeout will be infinite.
      */
      _mqx_uint timeout_in_ticks
   )
{ /* Body */
   MQX_TICK_STRUCT                ticks;
   _KLOGM(KERNEL_DATA_STRUCT_PTR  kernel_data;)
   _mqx_uint                      result;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE3(KLOG_sem_wait_ticks, users_sem_ptr, timeout_in_ticks);

   if (timeout_in_ticks) {
      ticks = _mqx_zero_tick_struct;

      ticks.TICKS[0] = timeout_in_ticks;
      result = _sem_wait_internal(users_sem_ptr, &ticks, FALSE);
   } else {
      result = _sem_wait_internal(users_sem_ptr, NULL, FALSE);
   } /* Endif */

   _KLOGX2(KLOG_sem_wait_ticks, result);
   return(result);

} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
