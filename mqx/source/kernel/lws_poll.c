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
* $FileName: lws_poll.c$
* $Version : 3.8.5.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   light weight semaphores.  These semaphores have low memory
*   requirements, and no extra features.  Tasks are suspended
*   in fifo order while waiting for a post.  No limits on values
*   are imposed.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwsem_poll
* Returned Value   : TRUE is semaphore obtained, otherwise FALSE if not 
*   available
* Comments         :
*   This function obtains a semaphore from the lwsem if one is
* available returning TRUE, otherwise it returns FALSE.  This call does not
* block.
*
*END*----------------------------------------------------------------------*/

boolean _lwsem_poll
   (
      /* [IN] the semaphore address */
      LWSEM_STRUCT_PTR sem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   boolean                result;

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwsem_poll(sem_ptr);
    }
#endif
    
   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_lwsem_poll, sem_ptr);

#if MQX_CHECK_VALIDITY
   if (sem_ptr->VALID != LWSEM_VALID) {
      _KLOGX2(KLOG_lwsem_poll, FALSE);
      return(FALSE);
   } /* Endif */
#endif

   _INT_DISABLE();
   if (sem_ptr->VALUE <= 0) {
      result = FALSE;
   } else {
      --sem_ptr->VALUE;
      result = TRUE;
   } /* Endif */
   _INT_ENABLE();

   _KLOGX2(KLOG_lwsem_poll, result);

   return(result);
   
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_lwsem_poll
* Returned Value   : TRUE is semaphore obtained, otherwise FALSE if not 
*   available
* Comments         :
*   This function obtains a semaphore from the lwsem if one is
* available returning TRUE, otherwise it returns FALSE.  This call does not
* block.
*
*END*----------------------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

boolean _usr_lwsem_poll
   (
      /* [IN] the semaphore address */
      LWSEM_STRUCT_PTR sem_ptr
   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)sem_ptr, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_LWSEM_POLL, &params);
}
#endif /* MQX_ENABLE_USER_MODE */
