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
* $FileName: pa_alloi.c$
* $Version : 3.8.6.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for allocating a block from a partition.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_alloc_internal
* Returned Value   : pointer
*    NULL is returned upon error.
* Comments         :
*    returns a fixed size memory block from the partition.
*
*END*---------------------------------------------------------*/

pointer _partition_alloc_internal
   (
      /* [IN] the partition to obtain the memory block from */
      PARTPOOL_STRUCT_PTR partpool_ptr,
      
      /* [IN] the task to own the partition block */
      TD_STRUCT_PTR       td_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR               kernel_data;
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR  block_ptr;
   PARTPOOL_BLOCK_STRUCT_PTR            partpool_block_ptr;
   _mqx_uint                            number_of_blocks;


#if MQX_CHECK_ERRORS
   if (partpool_ptr == NULL) {
      _task_set_error(PARTITION_INVALID);
      return NULL;
   } /* Endif */
#endif

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _task_set_error(PARTITION_INVALID);
      return NULL;
   } /* Endif */
#endif

   _GET_KERNEL_DATA(kernel_data);

   _INT_DISABLE();

   if (partpool_ptr->AVAILABLE == 0) {
      if ((partpool_ptr->PARTITION_TYPE == PARTITION_DYNAMIC) && 
          partpool_ptr->GROW_BLOCKS &&
          ((partpool_ptr->MAXIMUM_BLOCKS == 0) || 
             (partpool_ptr->TOTAL_BLOCKS < partpool_ptr->MAXIMUM_BLOCKS))
         )
      {
         /* We can attempt to grow the partition */
         number_of_blocks = partpool_ptr->GROW_BLOCKS;
         if ((partpool_ptr->MAXIMUM_BLOCKS != 0) && 
            (number_of_blocks >
            (partpool_ptr->MAXIMUM_BLOCKS - partpool_ptr->TOTAL_BLOCKS)))
         {
            number_of_blocks = partpool_ptr->MAXIMUM_BLOCKS -  partpool_ptr->TOTAL_BLOCKS;
         } /* Endif */

         /* 
         ** Temporarily boost up TOTAL BLOCKS to allow us to
         ** enable interrupts during memory allocation
         */
         partpool_ptr->TOTAL_BLOCKS += number_of_blocks;
         _int_enable();

#if PSP_MEMORY_ALIGNMENT
         partpool_block_ptr = (PARTPOOL_BLOCK_STRUCT_PTR)_mem_alloc_system((_mem_size)
            (sizeof(PARTPOOL_BLOCK_STRUCT) + PSP_MEMORY_ALIGNMENT + 
            (partpool_ptr->BLOCK_SIZE * number_of_blocks)));
#else
         partpool_block_ptr = _mem_alloc_system((_mem_size)
            (sizeof(PARTPOOL_BLOCK_STRUCT) + 
            (partpool_ptr->BLOCK_SIZE * number_of_blocks)));
#endif
         if (partpool_block_ptr != NULL) {
            _mem_set_type(partpool_block_ptr, MEM_TYPE_PARTITION_BLOCKS);

            _partition_extend_internal(partpool_ptr, partpool_block_ptr,
               number_of_blocks);
         } else {
            partpool_ptr->TOTAL_BLOCKS -= number_of_blocks;
         } /* Endif */

         _int_disable();

      } /* Endif */

      if (((volatile PARTPOOL_STRUCT _PTR_)partpool_ptr)->AVAILABLE == 0) {
         _int_enable();
         _task_set_error(PARTITION_OUT_OF_BLOCKS);
         return NULL;
      } /* Endif */
   } /* Endif */

   block_ptr = partpool_ptr->FREE_LIST_PTR;

#if MQX_CHECK_VALIDITY
   if ( ! VALID_PARTITION_CHECKSUM(block_ptr)) {
      _int_enable();
      _task_set_error(PARTITION_BLOCK_INVALID_CHECKSUM);
      return NULL;
   } /* Endif */
#endif
   
   partpool_ptr->FREE_LIST_PTR = block_ptr->LINK.NEXT_BLOCK_PTR;
   --partpool_ptr->AVAILABLE;

  if ((partpool_ptr->TOTAL_BLOCKS - partpool_ptr->AVAILABLE) > 
      partpool_ptr->MAX_BLOCKS_USED) 
   {
      partpool_ptr->MAX_BLOCKS_USED = partpool_ptr->TOTAL_BLOCKS - 
         partpool_ptr->AVAILABLE;
   }/* Endif */
   block_ptr->TASK_ID            = td_ptr->TASK_ID;
   block_ptr->LINK.PARTITION_PTR = partpool_ptr;

   CALC_PARTITION_CHECKSUM(block_ptr);

   /* Reset partition test walker */   
   partpool_ptr->TEST_FREE_PTR = partpool_ptr->FREE_LIST_PTR;
   
   _INT_ENABLE();

   return (pointer)((uchar_ptr)block_ptr + 
      sizeof(INTERNAL_PARTITION_BLOCK_STRUCT));

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
