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
* $FileName: pa_util.c$
* $Version : 3.0.5.0$
* $Date    : Nov-27-2008$
*
* Comments:
*
*   This file contains utility functions for partition.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_get_max_used_blocks 
* Returned Value   : _mqx_uint - 0 on error
* Comments         :
*    returns the maximum number of blocks used in the partition
*
*END*---------------------------------------------------------*/

_mqx_uint _partition_get_max_used_blocks
   (
      /* [IN] the partition to obtain information about */
      _partition_id partition
   )
{ /* Body */
   PARTPOOL_STRUCT_PTR partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _task_set_error(PARTITION_INVALID);
      return 0;
   } /* Endif */
#endif

   return(partpool_ptr->MAX_BLOCKS_USED);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_get_total_blocks
* Returned Value   : _mqx_uint - 0 on error
* Comments         :
*    returns the total number of blocks in this partition
*
*END*---------------------------------------------------------*/

_mqx_uint _partition_get_total_blocks
   (
      /* [IN] the partition to obtain information about */
      _partition_id partition
   )
{ /* Body */
   PARTPOOL_STRUCT_PTR       partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _task_set_error(PARTITION_INVALID);
      return 0;
   } /* Endif */
#endif

   return(partpool_ptr->TOTAL_BLOCKS);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_get_block_size
* Returned Value   : _mem_size
*    0 returned upon error
* Comments         :
*    returns the size for the memory block in the partition
*
*END*---------------------------------------------------------*/

_mem_size _partition_get_block_size
   (
      /* [IN] the partition to obtain information about */
      _partition_id partition
   )
{ /* Body */
   PARTPOOL_STRUCT_PTR    partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _task_set_error(PARTITION_INVALID);
      return 0;
   } /* Endif */
#endif

   return(partpool_ptr->BLOCK_SIZE -
      (_mem_size)sizeof(INTERNAL_PARTITION_BLOCK_STRUCT));

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_get_total_size
* Returned Value   : _mem_size
*    0 returned upon error
* Comments         :
*    returns the total size in bytes that this partition occupies
*
*END*---------------------------------------------------------*/

_mem_size _partition_get_total_size
   (
      /* [IN] the partition to obtain information about */
      _partition_id partition
   )
{ /* Body */
   PARTPOOL_STRUCT_PTR       partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;
   PARTPOOL_BLOCK_STRUCT_PTR partpool_block_ptr;
   register _mem_size        size;
   uchar_ptr                 tmp_ptr;

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _task_set_error(PARTITION_INVALID);
      return 0;
   } /* Endif */
#endif
   size = 0;
   partpool_block_ptr = partpool_ptr->POOL.NEXT_POOL_PTR;
   while (partpool_block_ptr != NULL) {
      tmp_ptr = (uchar_ptr)partpool_block_ptr + sizeof(PARTPOOL_BLOCK_STRUCT);
      tmp_ptr = (uchar_ptr)_ALIGN_ADDR_TO_HIGHER_MEM(tmp_ptr);
      size += (_mem_size)(tmp_ptr - (uchar_ptr)partpool_block_ptr);
      partpool_block_ptr = partpool_block_ptr->NEXT_POOL_PTR;
   } /* Endif */

   tmp_ptr = (uchar_ptr)partition + sizeof(PARTPOOL_STRUCT);
   tmp_ptr = (uchar_ptr)_ALIGN_ADDR_TO_HIGHER_MEM(tmp_ptr);

   size += (_mem_size)(tmp_ptr - (uchar_ptr)partition);

   return(size + partpool_ptr->BLOCK_SIZE * partpool_ptr->TOTAL_BLOCKS);
            
} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_calculate_size
* Returned Value   : _mem_size
* Comments         :
*    returns the total size a partition would occupy if created.
*
*END*---------------------------------------------------------*/

_mem_size _partition_calculate_size
   (
      /* [IN] the number of blocks in the partition */
      _mqx_uint number_of_blocks,

      /* [IN] the block size */
      _mem_size block_size
   )
{ /* Body */
   _mem_size actual_block_size;
   _mem_size size;

   actual_block_size = sizeof(INTERNAL_PARTITION_BLOCK_STRUCT) + block_size;
   _MEMORY_ALIGN_VAL_LARGER(actual_block_size);

#if PSP_MEMORY_ALIGNMENT
   size = sizeof(PARTPOOL_STRUCT) + PSP_MEMORY_ALIGNMENT
      + actual_block_size * number_of_blocks;
#else
   size = sizeof(PARTPOOL_STRUCT) + actual_block_size * number_of_blocks;
#endif

   return(size);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_calculate_blocks
* Returned Value   : _mqx_uint
* Comments         :
*    returns the number of blocks a partion would have if created.
*
*END*---------------------------------------------------------*/

_mqx_uint _partition_calculate_blocks
   (
      /* [IN] the maximum size of memory the partition can occupy */
      _mem_size partition_size,

      /* [IN] the block size */
      _mem_size block_size
   )
{ /* Body */
   _mem_size actual_size;
   _mem_size size;

   actual_size = sizeof(INTERNAL_PARTITION_BLOCK_STRUCT) + block_size;
   _MEMORY_ALIGN_VAL_LARGER(actual_size);

   size = (_mem_size)sizeof(PARTPOOL_STRUCT);
   _MEMORY_ALIGN_VAL_LARGER(size);
   return((_mqx_uint)((partition_size - size) / actual_size));

} /* Endbody */




/*FUNCTION*-----------------------------------------------------------------
*
* Task Name    : _partition_get_number_of
* Comments     :
*
*END*----------------------------------------------------------------------*/

_mqx_uint _partition_get_number_of
   (
      void
   )
{
   uint_32                    count = 0;
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   PARTITION_COMPONENT_STRUCT_PTR      part_component_ptr;


   _GET_KERNEL_DATA(kernel_data);
   
   part_component_ptr = (PARTITION_COMPONENT_STRUCT_PTR)  kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS];

   if (part_component_ptr) {
      count = part_component_ptr->PARTITIONS.SIZE;
   } else {
      count = 0;
   } /* Endif */

   return count;
}
         
#endif /* MQX_USE_PARTITIONS */

/* EOF */
