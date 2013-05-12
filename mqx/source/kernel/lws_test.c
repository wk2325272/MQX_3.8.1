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
* $FileName: lws_test.c$
* $Version : 3.8.4.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for testing all light weight
*   semaphores in the system.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _lwsem_test
* Returned Value   : MQX_OK or an error code
* Comments         :
*   This function tests all light weight semaphores for consistency and
* validity.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _lwsem_test
   ( 
      /* [OUT] the light weight semapohre in error */
      pointer _PTR_ lwsem_error_ptr,

      /* [OUT] the td on a light weight semaphore in error */
      pointer _PTR_ td_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   LWSEM_STRUCT_PTR       sem_ptr;
   _mqx_uint              queue_size;
   _mqx_uint              result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_lwsem_test, lwsem_error_ptr, td_error_ptr);

   *td_error_ptr = NULL;
   *lwsem_error_ptr = NULL;

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_lwsem_test, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   }/* Endif */
#endif

   _int_disable();

   result = _queue_test((QUEUE_STRUCT_PTR)&kernel_data->LWSEM, lwsem_error_ptr);
   if (result != MQX_OK) {
      _KLOGX3(KLOG_lwsem_test, result, *lwsem_error_ptr);
      return(result);
   } /* Endif */

   sem_ptr    = (LWSEM_STRUCT_PTR)((pointer)kernel_data->LWSEM.NEXT);
   queue_size = _QUEUE_GET_SIZE(&kernel_data->LWSEM);
   while (queue_size--) {
      if (sem_ptr->VALID != LWSEM_VALID) {
         result = MQX_INVALID_LWSEM;
         break;
      } /* Endif */

      result = _queue_test(&sem_ptr->TD_QUEUE, td_error_ptr);
      if (result != MQX_OK) {
         break;
      } /* Endif */
      
      sem_ptr = sem_ptr->NEXT;
   } /* Endwhile */

   _int_enable();

   if (result != MQX_OK) {
      *lwsem_error_ptr = (pointer)sem_ptr;
   } /* Endif */
   _KLOGX4(KLOG_lwsem_test, result, *lwsem_error_ptr, *td_error_ptr);

   return(result);
   
}

#if MQX_ENABLE_USER_MODE

_mqx_uint _lwsem_usr_check
    (
        /* [IN] the location of the lwsem */
        LWSEM_STRUCT_PTR tst_sem_ptr
    )
{
    KERNEL_DATA_STRUCT_PTR kernel_data;
    LWSEM_STRUCT_PTR sem_ptr;
    _mqx_uint result = MQX_INVALID_LWSEM;
    _mqx_uint queue_size;

    _GET_KERNEL_DATA(kernel_data);                                         

    sem_ptr = (LWSEM_STRUCT_PTR)((pointer)kernel_data->USR_LWSEM.NEXT);
    queue_size = _QUEUE_GET_SIZE(&kernel_data->USR_LWSEM);
    
    while (queue_size--) {
        if (sem_ptr->VALID != LWSEM_VALID) {
            break;
        }
        
        if (tst_sem_ptr == sem_ptr) {
            result = MQX_OK;
            break;
        }
        
        sem_ptr = (LWSEM_STRUCT_PTR)(pointer)sem_ptr->NEXT;
    }
    
    return result;
}

#endif /* MQX_ENABLE_USER_MODE */
