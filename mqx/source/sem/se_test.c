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
* $FileName: se_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing the semaphore component.
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
* Function Name    : _sem_test
* Returned Value   : _mqx_uint MQX_OK, or a MQX error code
* Comments         :
*    This function tests the semaphore component for validity and consistency.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sem_test
   (
      /* [OUT]  - the semaphore in error */
      pointer _PTR_ sem_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   SEM_COMPONENT_STRUCT_PTR sem_component_ptr;
   SEM_STRUCT_PTR           sem_ptr;
   pointer                  table_ptr;
   _mqx_max_type            data;
   _mqx_uint                result;
   _mqx_uint                i;

   _GET_KERNEL_DATA(kernel_data);                                         

   _KLOGE2(KLOG_sem_test, sem_error_ptr);

   *sem_error_ptr = NULL;

   sem_component_ptr = (SEM_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_SEMAPHORES];
   if (sem_component_ptr == NULL) {
      _KLOGX3(KLOG_sem_test, MQX_OK, 0);
      return(MQX_OK);
   } /* Endif */
   
   if (sem_component_ptr->VALID != SEM_VALID) {
      _KLOGX3(KLOG_sem_test, MQX_INVALID_COMPONENT_BASE, 0);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
   
   _int_disable();
   /* Test the semaphore name table */
   result = _name_test_internal((NAME_COMPONENT_STRUCT_PTR)
      sem_component_ptr->NAME_TABLE_HANDLE, &table_ptr, sem_error_ptr);
   _int_enable();
   if (result != MQX_OK) {
      _KLOGX3(KLOG_sem_test, MQX_INVALID_COMPONENT_BASE, *sem_error_ptr);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */

   i = 0;
   while (TRUE) {
      _int_disable();
      result =  _name_find_internal_by_index(
         sem_component_ptr->NAME_TABLE_HANDLE, i++, &data);
      if (result != MQX_OK) {
         result = MQX_OK;
         break;
      } /* Endif */

      sem_ptr = (SEM_STRUCT_PTR)data;
      if (sem_ptr->VALID != SEM_VALID) {
         result = SEM_INVALID_SEMAPHORE;
         break;
      } /* Endif */
      result = _queue_test(&sem_ptr->WAITING_TASKS, sem_error_ptr);
      if (result != MQX_OK) {
         break;
      } /* Endif */
      result = _queue_test(&sem_ptr->OWNING_TASKS, sem_error_ptr);
      if (result != MQX_OK) {
         break;
      } /* Endif */
      _int_enable();

   } /* Endwhile */

   _int_enable();

   _KLOGX3(KLOG_sem_test, result, *sem_error_ptr);
   return(result);
   
} /* Endbody */
#endif /* MQX_USE_SEMAPHORES */

/* EOF */
