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
* $FileName: se_fopn.c$
* $Version : 3.0.5.0$
* $Date    : Nov-27-2008$
*
* Comments:
*
*   This file contains the function for opening a semaphore quickly.
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
* Function Name    : _sem_open_fast
* Returned Value   : _mqx_uint MQX_OK, MQX_INVALID_COMPONENT_BASE,
*                            MQX_INVALID_COMPONENT_NAME,
*                            SEM_INVALID_SEMAPHORE_HANDLE,
*                            SEM_SEMAPHORE_NOT_FOUND, SEM_SEMAPHORE_DELETED,
*                            a Task Error code, 
* Comments         :
*   This function opens an instance of a semaphore for use by the task.
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_open_fast
   (
      /* [IN]  - the semaphore to access */
      _mqx_uint       sem_index,

      /* [IN/OUT] -  The ADDRESS of a pointer variable, into which an
      ** access value will be stored.  This value is required for all
      ** functions wishing to use the semaphore.
      */
      pointer _PTR_ returned_sem_ptr
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR    kernel_data;
   register SEM_COMPONENT_STRUCT_PTR  sem_component_ptr;
            SEM_STRUCT_PTR            sem_ptr;
   register SEM_CONNECTION_STRUCT_PTR sem_connection_ptr;
            _mqx_uint                  result;
            _mqx_max_type                 tmp;

   _GET_KERNEL_DATA(kernel_data);                                         
   
   _KLOGE3(KLOG_sem_open_fast, sem_index, returned_sem_ptr);

   *returned_sem_ptr = NULL;
   
   sem_component_ptr = (SEM_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_SEMAPHORES];
#if MQX_CHECK_ERRORS
   if (sem_component_ptr == NULL) {
      _KLOGX3(KLOG_sem_open_fast, MQX_COMPONENT_DOES_NOT_EXIST, 0);
      return(MQX_COMPONENT_DOES_NOT_EXIST);
   } /* Endif */
#endif
#if MQX_CHECK_VALIDITY
   if (sem_component_ptr->VALID != SEM_VALID) {
      _KLOGX3(KLOG_sem_open_fast, MQX_INVALID_COMPONENT_BASE, 0);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   result = _name_find_internal_by_index(sem_component_ptr->NAME_TABLE_HANDLE,
      sem_index, &tmp);
#if MQX_CHECK_ERRORS
   if (result != MQX_OK) {
      if (result == NAME_NOT_FOUND) {
         _KLOGX3(KLOG_sem_open_fast, SEM_SEMAPHORE_NOT_FOUND, 0);
         return(SEM_SEMAPHORE_NOT_FOUND);
      } /* Endif */
      _KLOGX3(KLOG_sem_open_fast, result, 0);
      return(result);
   } /* Endif */
#endif
   
   sem_ptr = (SEM_STRUCT_PTR)tmp;

#if MQX_CHECK_ERRORS
   if (sem_ptr->VALID != SEM_VALID) {
      /* Semaphore not valid */
      _KLOGX3(KLOG_sem_open_fast, SEM_INVALID_SEMAPHORE, 0);
      return(SEM_INVALID_SEMAPHORE);
   } /* Endif */
#endif

#if MQX_COMPONENT_DESTRUCTION
   if (sem_ptr->DELAYED_DESTROY) {
      /* Semaphore in delayed destroy state */
      _KLOGX3(KLOG_sem_open_fast, SEM_SEMAPHORE_DELETED, 0);
      return(SEM_SEMAPHORE_DELETED);
   } /* Endif */
#endif

   sem_connection_ptr = (SEM_CONNECTION_STRUCT_PTR)_mem_alloc_zero(
      (_mem_size)sizeof(SEM_CONNECTION_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (sem_connection_ptr == NULL) {
      _KLOGX3(KLOG_sem_open_fast, MQX_OUT_OF_MEMORY, 0);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
  _mem_set_type(sem_connection_ptr, MEM_TYPE_SEMAPHORE_CONNECTION);
   
   sem_connection_ptr->SEM_PTR = sem_ptr;
   sem_connection_ptr->VALID   = SEM_VALID;
   sem_connection_ptr->TD_PTR  = kernel_data->ACTIVE_PTR;

   *returned_sem_ptr = (pointer)sem_connection_ptr;

   _KLOGX3(KLOG_sem_open_fast, MQX_OK, sem_connection_ptr);
   return(MQX_OK);
   
} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
