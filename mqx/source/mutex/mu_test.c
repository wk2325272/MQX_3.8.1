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
* $FileName: mu_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing the mutex component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_test
* Returned Value   : _mqx_uint MQX_OK or a MQX error code.
* Comments         :
*    This function tests the mutex component.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_test
   (
      /* [OUT] - the mutex in error */
      pointer _PTR_ mutex_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   MUTEX_COMPONENT_STRUCT_PTR mutex_component_ptr;
   MUTEX_STRUCT_PTR           mutex_ptr;
   _mqx_uint                  result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_mutex_test, mutex_error_ptr);

   *mutex_error_ptr = NULL;

   mutex_component_ptr = (MUTEX_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES];
   if (mutex_component_ptr == NULL) {
      _KLOGX2(KLOG_mutex_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   if (mutex_component_ptr->VALID != MUTEX_VALID) {
      _KLOGX2(KLOG_mutex_test, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */

   _int_disable();

   /* Make sure that the queue of mutexes is ok */
   result = _queue_test(&mutex_component_ptr->MUTEXES, mutex_error_ptr);
   if (result != MQX_OK) {
      _int_enable();
      _KLOGX3(KLOG_mutex_test, result, *mutex_error_ptr);
      return(result);
   } /* Endif */

   mutex_ptr = (MUTEX_STRUCT_PTR)((pointer)mutex_component_ptr->MUTEXES.NEXT);
   while (mutex_ptr != (MUTEX_STRUCT_PTR)
      ((pointer)&mutex_component_ptr->MUTEXES))
   {
      if (mutex_ptr->VALID != MUTEX_VALID) {
         _int_enable();
         *mutex_error_ptr = mutex_ptr;
         _KLOGX3(KLOG_mutex_test, MQX_EINVAL, mutex_ptr);
         return(MQX_EINVAL);
      } /* Endif */
      result = _queue_test(&mutex_ptr->WAITING_TASKS, mutex_error_ptr);
      if (result != MQX_OK) {
         _int_enable();
         *mutex_error_ptr = mutex_ptr;
         _KLOGX3(KLOG_mutex_test, result, mutex_ptr);
         return(result);
      } /* Endif */
      mutex_ptr = (MUTEX_STRUCT_PTR)((pointer)mutex_ptr->LINK.NEXT);
   } /* Endif */
   
   _int_enable();

   _KLOGX2(KLOG_mutex_test, MQX_OK);
   return(MQX_OK);
   
} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
