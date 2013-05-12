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
* $FileName: se_fcrt.c$
* $Version : 3.0.5.0$
* $Date    : Nov-27-2008$
*
* Comments:
*
*   This file contains the function for creating a semaphore quickly.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SEMAPHORES
#include "name.h"
#include "name_prv.h"
#include "sem.h"
#include "sem_prv.h"
#include "mqx_str.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sem_create_fast
* Returned Value   : _mqx_uint MQX_OK, a Task Error Code, MQX_INVALID_COMPONENT_BASE
*                            MQX_INVALID_COMPONENT_NAME,
*                            SEM_SEMAPHORE_EXISTS, SEM_SEMAPHORE_TABLE_FULL
* Comments         :
*    This function creates an instance of a semaphore.  This semaphore may
* be given an initial count.  The semaphore may queue waiting
* tasks by priority.  The semaphore may also cause a task owning the
* semaphore to raise it's priority to the highest priority of any waiting
* task (priority inheritance).
*    _sem_create_fast access the semaphore via an index number rather than a name.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_create_fast
   (
      /* [IN]  - the semaphore to initialize */
      _mqx_uint sem_index,

      /* [IN]  - The initial value of the counting semaphore */
      _mqx_uint initial_count,

      /* 
      ** [IN]  - Bit flags consisting of the any of the following:
      **         SEM_PRIORITY_QUEUEING, SEM_PRIORITY_INHERITANCE, SEM_STRICT
      */
      _mqx_uint flags
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR   kernel_data;
   register SEM_COMPONENT_STRUCT_PTR sem_component_ptr;
   register SEM_STRUCT_PTR           sem_ptr;
            _mqx_uint                 result;

   _GET_KERNEL_DATA(kernel_data);                                         

   _KLOGE4(KLOG_sem_create_fast, sem_index, initial_count, flags);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_sem_create_fast, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (flags & SEM_STRICT) {
      if (initial_count == 0) {
         _KLOGX2(KLOG_sem_create_fast, SEM_INCORRECT_INITIAL_COUNT);
         return(SEM_INCORRECT_INITIAL_COUNT);
      }/* Endif */
   } else {
      if (flags & SEM_PRIORITY_INHERITANCE) {
         _KLOGX2(KLOG_sem_create_fast, SEM_INVALID_POLICY);
         return(SEM_INVALID_POLICY);
      } /* Endif */
   }/* Endif */
#endif

   sem_component_ptr = (SEM_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_SEMAPHORES];
   if (sem_component_ptr == NULL) {
      result = _sem_create_component(SEM_DEFAULT_INITIAL_NUMBER, 
         SEM_DEFAULT_GROW_NUMBER, SEM_DEFAULT_MAXIMUM_NUMBER);
      sem_component_ptr = (SEM_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_SEMAPHORES];
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (sem_component_ptr == NULL) {
         _KLOGX2(KLOG_sem_create_fast, result);
         return(result);
      } /* Endif */
#endif
   } /* Endif */
   
#if MQX_CHECK_VALIDITY
   if (sem_component_ptr->VALID != SEM_VALID) {
      _KLOGX2(KLOG_sem_create_fast, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif
   
   sem_ptr = (SEM_STRUCT_PTR)_mem_alloc_system_zero((_mem_size)sizeof(SEM_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (sem_ptr == NULL) {
      _KLOGX2(KLOG_sem_create_fast, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
  _mem_set_type(sem_ptr, MEM_TYPE_SEMAPHORE);

   sem_ptr->POLICY    = flags;
   sem_ptr->COUNT     = initial_count;
   if (flags & SEM_STRICT) {
      sem_ptr->MAX_COUNT = initial_count;
   } else {
      sem_ptr->MAX_COUNT = MAX_MQX_UINT;
   }/* Endif */
   _QUEUE_INIT(&sem_ptr->WAITING_TASKS, SEM_MAX_WAITING_TASKS);
   _QUEUE_INIT(&sem_ptr->OWNING_TASKS, SEM_MAX_WAITING_TASKS);

   _str_mqx_uint_to_hex_string(sem_index, sem_ptr->NAME);

   result = _name_add_internal_by_index(sem_component_ptr->NAME_TABLE_HANDLE,
      sem_ptr->NAME, (_mqx_uint)sem_ptr, sem_index);
#if MQX_CHECK_ERRORS
   if (result != MQX_OK) {
      _mem_free(sem_ptr);
      if (result == NAME_EXISTS) {
         _KLOGX2(KLOG_sem_create_fast, SEM_SEMAPHORE_EXISTS);
         return(SEM_SEMAPHORE_EXISTS);
      } else if (result == NAME_TABLE_FULL) {
         _KLOGX2(KLOG_sem_create_fast, SEM_SEMAPHORE_TABLE_FULL);
         return(SEM_SEMAPHORE_TABLE_FULL);
      } /* Endif */
      _KLOGX2(KLOG_sem_create_fast, result);
      return(result);
   } /* Endif */
#endif

   sem_ptr->VALID = SEM_VALID;

   _KLOGX2(KLOG_sem_create_fast, result);
   return(result);
   
} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
