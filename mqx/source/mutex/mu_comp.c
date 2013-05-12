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
* $FileName: mu_comp.c$
* $Version : 3.8.5.1$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for creating the mutex component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"
                   
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mutex_create_component
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*    Used by a task to install the mutex component into the kernel so that
* other tasks may create and use mutexes.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _mutex_create_component
   ( 
      void
   )
{ /* Body */
            KERNEL_DATA_STRUCT_PTR      kernel_data;
   register MUTEX_COMPONENT_STRUCT_PTR  mutex_component_ptr;
            
   _GET_KERNEL_DATA(kernel_data);            

   _KLOGE1(KLOG_mutex_create_component);

   if (kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES] != NULL) {
      _KLOGX2(KLOG_mutex_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   mutex_component_ptr = (MUTEX_COMPONENT_STRUCT_PTR)
      _mem_alloc_system_zero((_mem_size)sizeof(MUTEX_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (mutex_component_ptr == NULL) {
      _KLOGX2(KLOG_mutex_create_component, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
  _mem_set_type(mutex_component_ptr, MEM_TYPE_MUTEX_COMPONENT);


   _QUEUE_INIT(&mutex_component_ptr->MUTEXES, 0);
   mutex_component_ptr->VALID = MUTEX_VALID;

   _int_disable();

#if MQX_CHECK_ERRORS
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES] != NULL) {
      _int_enable();
      _mem_free(mutex_component_ptr);
      _KLOGX2(KLOG_mutex_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */
#endif

   kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES] = mutex_component_ptr;

#if MQX_COMPONENT_DESTRUCTION
   kernel_data->COMPONENT_CLEANUP[KERNEL_MUTEXES] = _mutex_cleanup;
#endif

   _int_enable();
   
   _KLOGX2(KLOG_mutex_create_component, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
