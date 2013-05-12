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
* $FileName: mem_free.c$
* $Version : 3.0.5.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function that frees dynamically allocated memory.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_free
* Returned Value   : _mqx_uint MQX_OK or a MQX error code.
* Comments         :
*   This function frees the given block of memory.  It performs
*   error checking validating that the block being freed was 
*   obtained by the same task who is freeing it.  
*   It also coalesces any free block found physically
*   on either side of the block being freed.
*   If coalescing is not possible, then the block is placed onto the free list.
*
*END*---------------------------------------------------------*/

_mqx_uint _mem_free
   (
      /* [IN] the address of the memory block to free */
      pointer mem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   STOREBLOCK_STRUCT_PTR   block_ptr;
   STOREBLOCK_STRUCT_PTR   prev_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_block_ptr;
   MEMPOOL_STRUCT_PTR      mem_pool_ptr;
   TD_STRUCT_PTR           td_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_mem_free, mem_ptr);

#if MQX_CHECK_ERRORS
   /* Verify the passed in parameter */
   if (mem_ptr == NULL) {
      _task_set_error(MQX_INVALID_POINTER);
      _KLOGX2(KLOG_mem_free, MQX_INVALID_POINTER);
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif
   
   block_ptr = GET_MEMBLOCK_PTR(mem_ptr);

#if MQX_CHECK_ERRORS
   /* Verify pointer alignment */
   if ( !_MEMORY_ALIGNED(block_ptr) || 
      (block_ptr->BLOCKSIZE < MQX_MIN_MEMORY_STORAGE_SIZE) || 
      BLOCK_IS_FREE(block_ptr) )
   {
      _task_set_error(MQX_INVALID_POINTER);
      _KLOGX2(KLOG_mem_free, MQX_INVALID_POINTER);
      return(MQX_INVALID_POINTER);
   } /* Endif */

#endif

   _INT_DISABLE();

#if MQX_CHECK_VALIDITY
   if ( ! VALID_CHECKSUM(block_ptr) ) {
      _int_enable();
      _task_set_error(MQX_INVALID_CHECKSUM);
      _KLOGX2(KLOG_mem_free, MQX_INVALID_CHECKSUM);
      return(MQX_INVALID_CHECKSUM);
   } /* Endif */
#endif
   
   mem_pool_ptr = (MEMPOOL_STRUCT_PTR)block_ptr->MEM_POOL_PTR;
   td_ptr = SYSTEM_TD_PTR(kernel_data);
   if (block_ptr->TASK_NUMBER != (TASK_NUMBER_FROM_TASKID(td_ptr->TASK_ID))) {
      td_ptr = kernel_data->ACTIVE_PTR;
   } /* Endif */

   /*  
   ** Walk through the memory resources of the task descriptor.
   ** Two pointers are maintained, one to the current block
   ** and one to the previous block.
   */
   next_block_ptr = (STOREBLOCK_STRUCT_PTR)td_ptr->MEMORY_RESOURCE_LIST;
   prev_block_ptr = (STOREBLOCK_STRUCT_PTR) 
      ( (uchar_ptr)(&td_ptr->MEMORY_RESOURCE_LIST) -
         FIELD_OFFSET(STOREBLOCK_STRUCT,NEXTBLOCK));
      
   /* 
   ** Scan the task's memory resource list searching for the block to
   ** free, Stop when the current pointer is equal to the block to free
   ** or the end of the list is reached.
   */
   while ( next_block_ptr  && ((pointer)next_block_ptr != mem_ptr) ) {
      /* 
      ** The block is not found, and the end of the list has not been
      ** reached, so move down the list.
      */
      prev_block_ptr = GET_MEMBLOCK_PTR(next_block_ptr);
      next_block_ptr = (STOREBLOCK_STRUCT_PTR)prev_block_ptr->NEXTBLOCK;
   } /* Endwhile */

#if MQX_CHECK_ERRORS
   if (next_block_ptr == NULL) {
      _int_enable();
      /* The specified block does not belong to the calling task. */
      _task_set_error(MQX_NOT_RESOURCE_OWNER);
      _KLOGX2(KLOG_mem_free, MQX_NOT_RESOURCE_OWNER);
      return(MQX_NOT_RESOURCE_OWNER);
   } /* Endif */
#endif

   /* Remove the memory block from the resource list of the calling task. */
   prev_block_ptr->NEXTBLOCK = block_ptr->NEXTBLOCK;
   
   #if MQX_ALLOW_TYPED_MEMORY
   block_ptr->MEM_TYPE = 0;
   #endif
   
   /* 
   ** Check if the neighbouring blocks are free, so we
   ** can coalesce the blocks.
   */
   if ( _mem_check_coalesce_internal(block_ptr) ) {
      /* No need to add block to free list if coalesced */
      _INT_ENABLE();
      _KLOGX2(KLOG_mem_free, MQX_OK);
      return(MQX_OK);
   } /* Endif */


#if MQX_MEMORY_FREE_LIST_SORTED == 1

   next_block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   if (next_block_ptr != NULL) {

      /* Insertion sort into the free list by address*/
      while (next_block_ptr < block_ptr ) {   
         /* This takes some time, so allow higher priority tasks
         ** to interrupt us.
         */

         if (NEXT_FREE(next_block_ptr) == NULL) {
            /* At end of free list */
            break;
         } /* Endif */
         
         next_block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(next_block_ptr);

         /* Save the current location in case premption occurs */
         mem_pool_ptr->POOL_FREE_CURRENT_BLOCK = next_block_ptr;
         _INT_ENABLE();
         _INT_DISABLE();
         
         /* Pick up where left off */
         next_block_ptr = mem_pool_ptr->POOL_FREE_CURRENT_BLOCK;

         if ( _mem_check_coalesce_internal(block_ptr) ) {
            /* No need to add block to free list if coalesced */
            _INT_ENABLE();
            _KLOGX2(KLOG_mem_free, MQX_OK);
            return(MQX_OK);
         } /* Endif */
      } /* Endwhile */

   } /* Endif */

   /* We have found the correct location */
   
   /* Make the block a free block */
   block_ptr->NEXTBLOCK    = NULL;
   block_ptr->BLOCKSIZE    = block_ptr->BLOCKSIZE;
   MARK_BLOCK_AS_FREE(block_ptr);
   CALC_CHECKSUM(block_ptr);

   /* Insert current block just before next block */
   if (next_block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
      /* We are inserting at the head of the free list */
      mem_pool_ptr->POOL_FREE_LIST_PTR = block_ptr;
      if ( next_block_ptr != NULL ) {
         PREV_FREE(next_block_ptr) = (pointer)block_ptr;
      } /* Endif */
      PREV_FREE(block_ptr) = NULL;
      NEXT_FREE(block_ptr) = (pointer)next_block_ptr;

   } else if (next_block_ptr < block_ptr) { 
      /* We are inserting at the end of the free list */
      NEXT_FREE(block_ptr) = NULL;
      PREV_FREE(block_ptr) = (pointer)next_block_ptr;
      NEXT_FREE(next_block_ptr) = (pointer)block_ptr;

   } else {  
      /* We are inserting into the middle of the free list */
      PREV_FREE(block_ptr) = PREV_FREE(next_block_ptr);
      PREV_FREE(next_block_ptr) = (pointer)block_ptr;
      NEXT_FREE(block_ptr) = (pointer)next_block_ptr;
      NEXT_FREE(PREV_FREE(block_ptr)) = (pointer)block_ptr;
   } /* Endif */

   /* 
   ** Reset the freelist current block pointer in case we pre-empted
   ** another task
   */
   mem_pool_ptr->POOL_FREE_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

#else

   /* Make the block a free block */
   block_ptr->NEXTBLOCK    = NULL;
   MARK_BLOCK_AS_FREE(block_ptr);
   CALC_CHECKSUM(block_ptr);

   /* Put the block at the head of the free list */
   next_block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   mem_pool_ptr->POOL_FREE_LIST_PTR = block_ptr;

   if ( next_block_ptr != NULL ) {
      PREV_FREE(next_block_ptr) = (pointer)block_ptr;
   } /* Endif */
   PREV_FREE(block_ptr) = NULL;
   NEXT_FREE(block_ptr) = (pointer)next_block_ptr;

#endif

   /* Reset the _mem_test pointers */
   mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK = 
      (STOREBLOCK_STRUCT_PTR)mem_pool_ptr->POOL_PTR;
   mem_pool_ptr->POOL_FREE_CHECK_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

   _INT_ENABLE();
   _KLOGX2(KLOG_mem_free, MQX_OK);
   return(MQX_OK);

} /* Endbody */

#endif
/* EOF */
