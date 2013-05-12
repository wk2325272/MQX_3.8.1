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
* $FileName: pa_creat.c$
* $Version : 3.0.5.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a partition.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _partition_create
* Returned Value   : _partition_id - ID of partition created if successful
*   or PARTITION_NULL_ID otherwise
* Comments         :
*    Create a partition from kernel memory,
*  with a certain number of blocks.
*
*END*-----------------------------------------------------------------*/

_partition_id _partition_create
   ( 
      /* 
      ** [IN] the size of each block in the partition in smallest 
      ** addressable units
      */
      _mem_size  block_size,
      
      /* [IN] the initial number of blocks in the partition */
      _mqx_uint  initial_blocks,

      /* 
      ** [IN] the number of blocks to grow by if all partition blocks are 
      ** in use 
      */
      _mqx_uint  grow_blocks,

      /* [IN] the maximum number of blocks to allow in the partition */
      _mqx_uint  maximum_blocks
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   PARTPOOL_STRUCT_PTR           partpool_ptr;
   _mem_size                     actual_size;
   _mqx_uint                     result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE5(KLOG_partition_create, block_size, initial_blocks, grow_blocks,
      maximum_blocks);

#if MQX_CHECK_ERRORS
   if (kernel_data->IN_ISR) {
      _task_set_error(MQX_CANNOT_CALL_FUNCTION_FROM_ISR);
      _KLOGX2(KLOG_partition_create, PARTITION_NULL_ID);
      return(PARTITION_NULL_ID);
   } /* Endif */

   if (block_size == 0) {
      _task_set_error(MQX_INVALID_PARAMETER);
      _KLOGX2(KLOG_partition_create, PARTITION_NULL_ID);
      return(PARTITION_NULL_ID);
   } /* Endif */
#endif

   actual_size = (_mem_size)sizeof(INTERNAL_PARTITION_BLOCK_STRUCT) + block_size;
   _MEMORY_ALIGN_VAL_LARGER(actual_size);

#if PSP_MEMORY_ALIGNMENT
   partpool_ptr = (PARTPOOL_STRUCT_PTR)_mem_alloc_system(
      (_mem_size)(sizeof(PARTPOOL_STRUCT) + PSP_MEMORY_ALIGNMENT + 
      (actual_size * initial_blocks)));
#else
   partpool_ptr = ((PARTPOOL_STRUCT_PTR))_mem_alloc_system(
      (_mem_size)(sizeof(PARTPOOL_STRUCT) + (actual_size * initial_blocks)));
#endif
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (partpool_ptr == NULL) {
      _KLOGX2(KLOG_partition_create, PARTITION_NULL_ID);
      return(PARTITION_NULL_ID);
   }/* Endif */
#endif
   _mem_set_type(partpool_ptr, MEM_TYPE_PARTITION);

   result = _partition_create_internal(partpool_ptr, actual_size, 
      initial_blocks);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (result != MQX_OK) {
      _KLOGX2(KLOG_partition_create, PARTITION_NULL_ID);
      _mem_free(partpool_ptr);
      return(PARTITION_NULL_ID);
   } /* Endif */
#endif

   partpool_ptr->PARTITION_TYPE = PARTITION_DYNAMIC;
   partpool_ptr->GROW_BLOCKS    = grow_blocks;
   partpool_ptr->MAXIMUM_BLOCKS = maximum_blocks;

   _KLOGX3(KLOG_partition_create, partpool_ptr, MQX_OK);
   return((_partition_id)partpool_ptr);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
