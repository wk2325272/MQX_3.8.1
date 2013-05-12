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
* $FileName: pa_extnd.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for adding another memory area to an
*   existing partition.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_extend
* Returned Value   : _mqx_uint error code
*    MQX_OK, or an error code value
* Comments         :
*    adds more blocks of memory into the partition
*
*END*---------------------------------------------------------*/

_mqx_uint _partition_extend
   (
      /* [IN] the partition to add to */
      _partition_id partition,

      /* [IN] where the additional blocks are to start from */
      pointer       partition_location,

      /* [IN] the size of the partition. */
      _mem_size     partition_size
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR        kernel_data;)
   PARTPOOL_STRUCT_PTR                  partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;
   PARTPOOL_BLOCK_STRUCT_PTR            partpool_block_ptr;
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR  block_ptr;
   _mqx_uint                            number_of_blocks;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE4(KLOG_partition_extend, partition, partition_location, partition_size);

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _KLOGX2(KLOG_partition_extend, PARTITION_INVALID);
      return(PARTITION_INVALID);
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (partpool_ptr->PARTITION_TYPE == PARTITION_DYNAMIC) {
      _KLOGX2(KLOG_partition_extend, MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   } /* Endif */  
#endif

   partpool_block_ptr = (PARTPOOL_BLOCK_STRUCT_PTR)
      _ALIGN_ADDR_TO_HIGHER_MEM(partition_location);
   partition_size     = partition_size - (_mem_size)partpool_block_ptr +
      (_mem_size)partition_location;
   _MEMORY_ALIGN_VAL_SMALLER(partition_size);

#if MQX_CHECK_ERRORS
   if (partition_size < (_mem_size)sizeof(PARTPOOL_STRUCT)) {
      _KLOGX2(KLOG_partition_extend, MQX_INVALID_PARAMETER);
      return(MQX_INVALID_PARAMETER);
   } /* Endif */  
#endif

   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
      ((uchar_ptr)partpool_block_ptr + sizeof(PARTPOOL_BLOCK_STRUCT));
   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
      _ALIGN_ADDR_TO_HIGHER_MEM(block_ptr);

   number_of_blocks = (partition_size -
      ((uchar_ptr)block_ptr - (uchar_ptr)partpool_block_ptr)) / 
      partpool_ptr->BLOCK_SIZE;
   partpool_ptr->TOTAL_BLOCKS += number_of_blocks;

   _partition_extend_internal(partpool_ptr, partpool_block_ptr,
      number_of_blocks);

   _KLOGX5(KLOG_partition_extend, partition, partpool_ptr, partition_size, MQX_OK);

   return MQX_OK;

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
