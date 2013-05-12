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
* $FileName: pa_comp.c$
* $Version : 3.8.5.1$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the function for creating and installing the
*   partition component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _partition_create_component
* Returned Value   : _mqx_uint MQX_OK, MQX_OUT_OF_MEMORY
* Comments         :
*   This function creates the partition kernel component.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _partition_create_component
   ( 
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR          kernel_data;
   PARTITION_COMPONENT_STRUCT_PTR  part_component_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE1(KLOG_partition_create_component);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _KLOGX2(KLOG_partition_create_component, MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      return(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
   } /* Endif */
#endif

   _lwsem_wait((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);

#if MQX_CHECK_ERRORS
   if (kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS] != NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
      _KLOGX2(KLOG_partition_create_component, MQX_OK);
      return(MQX_OK);
   } /* Endif */
#endif

   part_component_ptr = _mem_alloc_system_zero(
      (_mem_size)sizeof(PARTITION_COMPONENT_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (part_component_ptr == NULL) {
      _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);
      _KLOGX2(KLOG_partition_create_component, MQX_OUT_OF_MEMORY);
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(part_component_ptr, MEM_TYPE_PARTITION_COMPONENT);

   kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS] = part_component_ptr;
/* START CR 308 */
   part_component_ptr->VALID = PARTITION_VALID;
/* END CR 308 */

   _QUEUE_INIT(&part_component_ptr->PARTITIONS, 0);

#if MQX_COMPONENT_DESTRUCTION
   kernel_data->COMPONENT_CLEANUP[KERNEL_PARTITIONS] = _partition_cleanup;
#endif

   _lwsem_post((LWSEM_STRUCT_PTR)&kernel_data->COMPONENT_CREATE_LWSEM);

   _KLOGX2(KLOG_partition_create_component, MQX_OK);

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
