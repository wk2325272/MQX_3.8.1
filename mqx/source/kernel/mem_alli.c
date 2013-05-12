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
* $FileName: mem_alli.c$
* $Version : 3.8.21.1$
* $Date    : Mar-14-2012$
*
* Comments:
*
*   This file contains the function that allocates a memory block.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_internal
* Returned Value   : pointer. NULL is returned upon error.
* Comments         :
*    THIS FUNCTION MUST BE CALLED DISABLED
*
*    A pointer to memory that can be used is returned.
*    _mem_alloc_internal walks down the free list, looking for a 
*    block big enough to satisfy the request.  If the found block 
*    is bigger than needed, then a new free block is created from 
*    the remnants, and added into the free list.  
*
*    A global memory pointer is used, this allows higher priority 
*    tasks to pre-empt the current task in _mem_alloc_internal.  
*    Care is taken to make sure that when the higher priority task 
*    is completed, the memory pool search pointer is reset to the 
*    beginning of the free list.
*
*    Since the memory test function may be running concurrently
*    with the allocator, the memory test pointers are reset.
*
*END*---------------------------------------------------------*/

pointer _mem_alloc_internal
   ( 
      /* [IN] the size of the memory block */
      _mem_size           requested_size,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR       td_ptr,
      
      /* [IN] which pool to allocate from */
      MEMPOOL_STRUCT_PTR  mem_pool_ptr,

      /* [OUT] error code for operation */
      _mqx_uint_ptr       error_ptr
   )
{ /* Body */
   STOREBLOCK_STRUCT_PTR   block_ptr;
   STOREBLOCK_STRUCT_PTR   next_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_next_block_ptr;
   _mem_size               block_size;
   _mem_size               next_block_size;

   *error_ptr = MQX_OK;

   /* 
   ** Adjust message size to allow for block management overhead
   ** and force size to be aligned.
   */
   requested_size += (_mem_size)(FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA));
#if MQX_CHECK_ERRORS
   if (requested_size <  MQX_MIN_MEMORY_STORAGE_SIZE) {
      requested_size = MQX_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */
#endif

   _MEMORY_ALIGN_VAL_LARGER(requested_size);

   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   
   while ( TRUE ) {      

      /* 
      ** Save the current block pointer.
      ** We will be enabling access to higher priority tasks.
      ** A higher priority task may pre-empt the current task
      ** and may do a memory allocation.  If this is true,
      ** the higher priority task will reset the POOL_ALLOC_CURRENT_BLOCK
      ** upon exit, and the current task will start the search over
      ** again.  
      */
      mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = block_ptr;

      /* allow pending interrupts */
      _int_enable();
      _int_disable();

      /* Get block pointer in case reset by a higher priority task */
      block_ptr = mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK;

      if (block_ptr == NULL) { /* Null pointer */
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_OUT_OF_MEMORY;
         return( NULL );        /* request failed */
      } /* Endif */

#if MQX_CHECK_VALIDITY
      if ( !_MEMORY_ALIGNED(block_ptr) || BLOCK_IS_USED(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         return((pointer)NULL);
      } /* Endif */
#endif

#if MQX_CHECK_VALIDITY
      if ( ! VALID_CHECKSUM(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_INVALID_CHECKSUM;
         return((pointer)NULL);
      } /* Endif */
#endif

      block_size = block_ptr->BLOCKSIZE;

      if (block_size >= requested_size) {
         /* request fits into this block */

         next_block_size = block_size - requested_size;
         if ( next_block_size >= (2 * MQX_MIN_MEMORY_STORAGE_SIZE) ) {
            /* 
            ** The current block is big enough to split.
            ** into 2 blocks.... the part to be allocated is one block,
            ** and the rest remains as a free block on the free list.
            */

            next_block_ptr = (STOREBLOCK_STRUCT_PTR)
               ((char _PTR_)block_ptr + requested_size);

            /* Initialize the new physical block values */
            next_block_ptr->BLOCKSIZE = next_block_size;
            next_block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)block_ptr;
            MARK_BLOCK_AS_FREE(next_block_ptr);

            CALC_CHECKSUM(next_block_ptr);

            /* Link new block into the free list */
            next_block_ptr->NEXTBLOCK = block_ptr->NEXTBLOCK;
            block_ptr->NEXTBLOCK      = (pointer)next_block_ptr;

            next_block_ptr->USER_AREA = (pointer)block_ptr;
            if ( next_block_ptr->NEXTBLOCK != NULL ) {
               ((STOREBLOCK_STRUCT_PTR)next_block_ptr->NEXTBLOCK)->
                  USER_AREA = (pointer)next_block_ptr;
            } /* Endif */
           
            /* 
            ** Modify the block on the other side of the next block
            ** (the next next block) so that it's previous block pointer
            ** correctly point to the next block.
            */
            next_next_block_ptr = (STOREBLOCK_STRUCT_PTR)
               NEXT_PHYS(next_block_ptr);
            PREV_PHYS(next_next_block_ptr) = next_block_ptr;
            CALC_CHECKSUM(next_next_block_ptr);

         } else {

            /* Take the entire block */
            requested_size = block_size;

         } /* Endif */

         /* Set the size of the block */
         block_ptr->BLOCKSIZE = requested_size;
         
         /* Indicate the block is in use */
         MARK_BLOCK_AS_USED(block_ptr, td_ptr->TASK_ID);
         block_ptr->MEM_TYPE = 0;

         CALC_CHECKSUM(block_ptr);

         /* Unlink the block from the free list */
         if ( block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
            /* At the head of the free list */

            mem_pool_ptr->POOL_FREE_LIST_PTR = (STOREBLOCK_STRUCT_PTR)
               NEXT_FREE(block_ptr);
            if (mem_pool_ptr->POOL_FREE_LIST_PTR != NULL ) {
               PREV_FREE(mem_pool_ptr->POOL_FREE_LIST_PTR) = 0;
            } /* Endif */

         } else {

            /* 
            ** NOTE: PREV_FREE guaranteed to be non-zero 
            ** Have to make the PREV_FREE of this block
            ** point to the NEXT_FREE of this block
            */
            NEXT_FREE(PREV_FREE(block_ptr)) = NEXT_FREE(block_ptr);
            if ( NEXT_FREE(block_ptr) != NULL ) {
               /* 
               ** Now have to make the NEXT_FREE of this block
               ** point to the PREV_FREE of this block
               */
               PREV_FREE(NEXT_FREE(block_ptr)) = PREV_FREE(block_ptr);
            } /* Endif */

         } /* Endif */

#if MQX_MEMORY_FREE_LIST_SORTED == 1
         if ( block_ptr == mem_pool_ptr->POOL_FREE_CURRENT_BLOCK ) {
            /* Reset the freelist insertion sort by _mem_free */
            mem_pool_ptr->POOL_FREE_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
         } /* Endif */
#endif

         /* Reset the __mem_test freelist pointer */
         mem_pool_ptr->POOL_FREE_CHECK_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

         /* 
         ** Set the curent pool block to the start of the free list, so
         ** that if this task pre-empted another that was performing a
         ** _mem_alloc, the other task will restart it's search for a block
         */
         mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
            
          /* Remember some statistics - only for blocks which fall into the 
          ** main pool area. Ignore blocks in the extended areas (EXT_LIST)
          */
         next_block_ptr = NEXT_PHYS(block_ptr);
         if (((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_HIGHEST_MEMORY_USED) &&
             ((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_PTR) && 
             ((char _PTR_)(next_block_ptr) < (char _PTR_) mem_pool_ptr->POOL_END_PTR)
             )
         {
            mem_pool_ptr->POOL_HIGHEST_MEMORY_USED = ((char _PTR_)(next_block_ptr) - 1);
         } /* Endif */

         /* Link the block onto the task descriptor. */
         block_ptr->NEXTBLOCK = td_ptr->MEMORY_RESOURCE_LIST;
         td_ptr->MEMORY_RESOURCE_LIST = (pointer)(&block_ptr->USER_AREA);

         block_ptr->MEM_POOL_PTR = (pointer)mem_pool_ptr;

#if MQX_CHECK_VALIDITY
      /* Check that user area is aligned on a cache line boundary */
      if ( !_MEMORY_ALIGNED(&block_ptr->USER_AREA) ) {
         *error_ptr = MQX_INVALID_CONFIGURATION;
         return((pointer)NULL);
      } /* Endif */
#endif
      
         return( (pointer)(&block_ptr->USER_AREA ) );

      } else {
         block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(block_ptr);
      } /* Endif */

   } /* Endwhile */

#ifdef lint
   return( NULL );        /* to satisfy lint */
#endif

} /* Endbody */

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_at_internal
* Returned Value   : pointer. NULL is returned upon error.
* Comments         :
*END*---------------------------------------------------------*/

pointer _mem_alloc_at_internal
    ( 
      /* [IN] the size of the memory block */
      _mem_size           requested_size,
      
      /* [IN] the addr of the memory block */
      pointer             requested_addr,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR       td_ptr,
      
      /* [IN] which pool to allocate from */
      MEMPOOL_STRUCT_PTR  mem_pool_ptr,

      /* [OUT] error code for operation */
      _mqx_uint_ptr       error_ptr
   )
{ /* Body */
   STOREBLOCK_STRUCT_PTR   block_ptr, free_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_next_block_ptr;
   _mem_size               block_size, next_block_size, free_block_size;

   *error_ptr = MQX_OK;

   /* 
   ** Adjust message size to allow for block management overhead
   ** and force size to be aligned.
   */
   requested_size += (_mem_size)(FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA));
#if MQX_CHECK_ERRORS
   if (requested_size <  MQX_MIN_MEMORY_STORAGE_SIZE) {
      requested_size = MQX_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */
#endif

   _MEMORY_ALIGN_VAL_LARGER(requested_size);

   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   
   while ( TRUE ) {      

      /* 
      ** Save the current block pointer.
      ** We will be enabling access to higher priority tasks.
      ** A higher priority task may pre-empt the current task
      ** and may do a memory allocation.  If this is true,
      ** the higher priority task will reset the POOL_ALLOC_CURRENT_BLOCK
      ** upon exit, and the current task will start the search over
      ** again.  
      */
      mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = block_ptr;

      /* allow pending interrupts */
      _int_enable();
      _int_disable();

      /* Get block pointer in case reset by a higher priority task */
      block_ptr = mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK;

      if (block_ptr == NULL) { /* Null pointer */
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_OUT_OF_MEMORY;
         return( NULL );        /* request failed */
      } /* Endif */

#if MQX_CHECK_VALIDITY
      if ( !_MEMORY_ALIGNED(block_ptr) || BLOCK_IS_USED(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         return((pointer)NULL);
      } /* Endif */
#endif

#if MQX_CHECK_VALIDITY
      if ( ! VALID_CHECKSUM(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_INVALID_CHECKSUM;
         return((pointer)NULL);
      } /* Endif */
#endif

      block_size = block_ptr->BLOCKSIZE;

      if ((uchar_ptr)&block_ptr->USER_AREA <= (uchar_ptr)requested_addr && 
          (uchar_ptr)&block_ptr->USER_AREA + block_size >= (uchar_ptr)requested_addr + requested_size &&
          block_size >= requested_size) {
         /* request fits into this block */
              
         // create new free block
         free_block_ptr = (STOREBLOCK_STRUCT_PTR)((char _PTR_)block_ptr);

         block_ptr = (STOREBLOCK_STRUCT_PTR)((uchar_ptr)requested_addr - (_mem_size)(FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA)));
         //block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)free_block_ptr;
         
         free_block_size = (uchar_ptr)block_ptr - (uchar_ptr)free_block_ptr;

         next_block_size = block_size - requested_size - free_block_size;
         if (next_block_size >= (2 * MQX_MIN_MEMORY_STORAGE_SIZE) ) {
            /* 
            ** The current block is big enough to split.
            ** into 2 blocks.... the part to be allocated is one block,
            ** and the rest remains as a free block on the free list.
            */

            next_block_ptr = (STOREBLOCK_STRUCT_PTR)((char _PTR_)block_ptr + requested_size);

            /* Initialize the new physical block values */
            next_block_ptr->BLOCKSIZE = next_block_size;
            next_block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)block_ptr;
            MARK_BLOCK_AS_FREE(next_block_ptr);

            CALC_CHECKSUM(next_block_ptr);

            /* Link new block into the free list */
            next_block_ptr->NEXTBLOCK = free_block_ptr->NEXTBLOCK;
            block_ptr->NEXTBLOCK = (pointer)next_block_ptr;

            next_block_ptr->USER_AREA = (pointer)block_ptr;
            if (next_block_ptr->NEXTBLOCK != NULL ) {
               ((STOREBLOCK_STRUCT_PTR)next_block_ptr->NEXTBLOCK)->USER_AREA = (pointer)next_block_ptr;
            }
           
            /* 
            ** Modify the current block, to point to this newly created
            ** block which is now the next physical block.
            */
            block_ptr->BLOCKSIZE = requested_size;

            /* 
            ** Modify the block on the other side of the next block
            ** (the next next block) so that it's previous block pointer
            ** correctly point to the next block.
            */
            next_next_block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_PHYS(next_block_ptr);
            PREV_PHYS(next_next_block_ptr) = next_block_ptr;
            CALC_CHECKSUM(next_next_block_ptr);
         } else {
            /* Take the entire block */
            requested_size += next_block_size;
            next_block_ptr = free_block_ptr->NEXTBLOCK;
         }
         
         if (free_block_size >= (2 * MQX_MIN_MEMORY_STORAGE_SIZE) ) {
            /* modify the new physical block values */
            free_block_ptr->BLOCKSIZE = free_block_size;
            free_block_ptr->NEXTBLOCK = (pointer)next_block_ptr;
            MARK_BLOCK_AS_FREE(free_block_ptr);

            CALC_CHECKSUM(free_block_ptr);

            /* Set the size of the block */
            block_ptr->BLOCKSIZE = requested_size;
            block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)free_block_ptr;
            block_ptr->MEM_TYPE = 0;
            MARK_BLOCK_AS_USED(block_ptr, td_ptr->TASK_ID);

            CALC_CHECKSUM(block_ptr);
         }
         else {
            /* Set the size of the block */
            block_ptr->PREVBLOCK = PREV_PHYS(free_block_ptr);//->PREVBLOCK;
            block_ptr->BLOCKSIZE = requested_size;
            block_ptr->MEM_TYPE = 0;
            MARK_BLOCK_AS_USED(block_ptr, td_ptr->TASK_ID);
            CALC_CHECKSUM(block_ptr);

            (block_ptr->PREVBLOCK)->BLOCKSIZE += free_block_size;
            CALC_CHECKSUM(block_ptr->PREVBLOCK);

            /* Unlink the block from the free list */
            if (free_block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR) {
               /* At the head of the free list */
    
               //mem_pool_ptr->POOL_FREE_LIST_PTR = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(free_block_ptr);
               mem_pool_ptr->POOL_FREE_LIST_PTR = next_block_ptr;
               if (mem_pool_ptr->POOL_FREE_LIST_PTR != NULL ) {
                  PREV_FREE(mem_pool_ptr->POOL_FREE_LIST_PTR) = 0;
               }
            } else {
                /* 
                ** NOTE: PREV_FREE guaranteed to be non-zero 
                ** Have to make the PREV_FREE of this block
                ** point to the NEXT_FREE of this block
                */
                NEXT_FREE(PREV_FREE(free_block_ptr)) = NEXT_FREE(free_block_ptr);
                if ( NEXT_FREE(free_block_ptr) != NULL ) {
                   /* 
                   ** Now have to make the NEXT_FREE of this block
                   ** point to the PREV_FREE of this block
                   */
                   PREV_FREE(NEXT_FREE(free_block_ptr)) = PREV_FREE(free_block_ptr);
                }
            }
#if MQX_MEMORY_FREE_LIST_SORTED == 1
            if ( free_block_ptr == mem_pool_ptr->POOL_FREE_CURRENT_BLOCK ) {
                /* Reset the freelist insertion sort by _mem_free */
                mem_pool_ptr->POOL_FREE_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
            }
#endif
         }

         /* Reset the __mem_test freelist pointer */
         mem_pool_ptr->POOL_FREE_CHECK_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

         /* 
         ** Set the curent pool block to the start of the free list, so
         ** that if this task pre-empted another that was performing a
         ** _mem_alloc, the other task will restart it's search for a block
         */
         mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
            
          /* Remember some statistics */
         next_block_ptr = NEXT_PHYS(block_ptr);
         if (((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_HIGHEST_MEMORY_USED) &&
             ((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_PTR) && 
             ((char _PTR_)(next_block_ptr) < (char _PTR_) mem_pool_ptr->POOL_END_PTR)
             )
         {
            mem_pool_ptr->POOL_HIGHEST_MEMORY_USED = ((char _PTR_)(next_block_ptr) - 1);
         }

         /* Link the block onto the task descriptor. */
         block_ptr->NEXTBLOCK = td_ptr->MEMORY_RESOURCE_LIST;
         td_ptr->MEMORY_RESOURCE_LIST = (pointer)(&block_ptr->USER_AREA);

         block_ptr->MEM_POOL_PTR = (pointer)mem_pool_ptr;

#if MQX_CHECK_VALIDITY
         /* Check that user area is aligned on a cache line boundary */
         if ( !_MEMORY_ALIGNED(&block_ptr->USER_AREA) ) {
             *error_ptr = MQX_INVALID_CONFIGURATION;
             return((pointer)NULL);
         } /* Endif */
#endif
      
         return ((pointer)(&block_ptr->USER_AREA));
      } else {
         block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(block_ptr);
      }
   }

#ifdef lint
   return( NULL );        /* to satisfy lint */
#endif
}

#if MQX_ALLOC_ALLIGN_RESIZE

pointer _mem_alloc_internal_align
   ( 
      /* [IN] the size of the memory block */
      _mem_size           requested_size,
      
      _mem_size           req_align,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR       td_ptr,
      
      /* [IN] which pool to allocate from */
      MEMPOOL_STRUCT_PTR  mem_pool_ptr,

      /* [OUT] error code for operation */
      _mqx_uint_ptr       error_ptr
   )
{ /* Body */
   STOREBLOCK_STRUCT_PTR   block_ptr, old_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_next_block_ptr;
   _mem_size               block_size;
   _mem_size               next_block_size;
   _mem_size               shift;
   STOREBLOCK_STRUCT       saved_block;

   *error_ptr = MQX_OK;

   /* 
   ** Adjust message size to allow for block management overhead
   ** and force size to be aligned.
   */
   requested_size += (_mem_size)(FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA));
#if MQX_CHECK_ERRORS
   if (requested_size <  MQX_MIN_MEMORY_STORAGE_SIZE) {
      requested_size = MQX_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */
#endif

   _MEMORY_ALIGN_VAL_LARGER(requested_size);

   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   
   while ( TRUE ) {      

      /* 
      ** Save the current block pointer.
      ** We will be enabling access to higher priority tasks.
      ** A higher priority task may pre-empt the current task
      ** and may do a memory allocation.  If this is true,
      ** the higher priority task will reset the POOL_ALLOC_CURRENT_BLOCK
      ** upon exit, and the current task will start the search over
      ** again.  
      */
      mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = block_ptr;

      /* allow pending interrupts */
      _int_enable();
      _int_disable();

      /* Get block pointer in case reset by a higher priority task */
      block_ptr = mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK;

      if (block_ptr == NULL) { /* Null pointer */
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_OUT_OF_MEMORY;
         return( NULL );        /* request failed */
      } /* Endif */

#if MQX_CHECK_VALIDITY
      if ( !_MEMORY_ALIGNED(block_ptr) || BLOCK_IS_USED(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         return((pointer)NULL);
      } /* Endif */
#endif

#if MQX_CHECK_VALIDITY
      if ( ! VALID_CHECKSUM(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_INVALID_CHECKSUM;
         return((pointer)NULL);
      } /* Endif */
#endif

      block_size = block_ptr->BLOCKSIZE;
      shift = (((_mem_size)&block_ptr->USER_AREA + req_align) & ~(req_align - 1)) - (_mem_size)&block_ptr->USER_AREA;

      if (block_size >= requested_size + shift) {
         old_block_ptr = block_ptr;
         
         if (shift) {
            _mem_copy(block_ptr, &saved_block, sizeof(STOREBLOCK_STRUCT));
            // move block by shift
            (char _PTR_)block_ptr += shift;
            _mem_copy(&saved_block, block_ptr, sizeof(STOREBLOCK_STRUCT));

            block_ptr->BLOCKSIZE -= shift;
            CALC_CHECKSUM(block_ptr);
         
            //PREV_PHYS(block_ptr)->NEXTBLOCK = &block_ptr->USER_AREA;
            PREV_PHYS(block_ptr)->BLOCKSIZE += shift;
            CALC_CHECKSUM(PREV_PHYS(block_ptr));
         
            block_size = block_ptr->BLOCKSIZE;
         }
      
         /* request fits into this block */

         next_block_size = block_size - requested_size;
         if ( next_block_size >= (2 * MQX_MIN_MEMORY_STORAGE_SIZE) ) {
            /* 
            ** The current block is big enough to split.
            ** into 2 blocks.... the part to be allocated is one block,
            ** and the rest remains as a free block on the free list.
            */

            next_block_ptr = (STOREBLOCK_STRUCT_PTR)
               ((char _PTR_)block_ptr + requested_size);

            /* Initialize the new physical block values */
            next_block_ptr->BLOCKSIZE = next_block_size;
            next_block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)block_ptr;
            MARK_BLOCK_AS_FREE(next_block_ptr);

            CALC_CHECKSUM(next_block_ptr);

            /* Link new block into the free list */
            next_block_ptr->NEXTBLOCK = block_ptr->NEXTBLOCK;
            block_ptr->NEXTBLOCK      = (pointer)next_block_ptr;

            next_block_ptr->USER_AREA = (pointer)free_block_ptr;
            if ( next_block_ptr->NEXTBLOCK != NULL ) {
               ((STOREBLOCK_STRUCT_PTR)next_block_ptr->NEXTBLOCK)->
                  USER_AREA = (pointer)next_block_ptr;
            } /* Endif */
           
            /* 
            ** Modify the current block, to point to this newly created
            ** block which is now the next physical block.
            */
            block_ptr->BLOCKSIZE = requested_size;

            /* 
            ** Modify the block on the other side of the next block
            ** (the next next block) so that it's previous block pointer
            ** correctly point to the next block.
            */
            next_next_block_ptr = (STOREBLOCK_STRUCT_PTR)
               NEXT_PHYS(next_block_ptr);
            PREV_PHYS(next_next_block_ptr) = next_block_ptr;
            CALC_CHECKSUM(next_next_block_ptr);

         } else {

            /* Take the entire block */
            requested_size = block_size;

         } /* Endif */

         /* Set the size of the block */
         block_ptr->BLOCKSIZE = requested_size;
         block_ptr->MEM_TYPE = 0;
         
         /* Indicate the block is in use */
         MARK_BLOCK_AS_USED(block_ptr, td_ptr->TASK_ID);

         CALC_CHECKSUM(block_ptr);

         /* Unlink the block from the free list */
         //if ( block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
         if (old_block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
            /* At the head of the free list */

            mem_pool_ptr->POOL_FREE_LIST_PTR = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(block_ptr);
            if (mem_pool_ptr->POOL_FREE_LIST_PTR != NULL ) {
               PREV_FREE(mem_pool_ptr->POOL_FREE_LIST_PTR) = 0;
            } /* Endif */

         } else {

            /* 
            ** NOTE: PREV_FREE guaranteed to be non-zero 
            ** Have to make the PREV_FREE of this block
            ** point to the NEXT_FREE of this block
            */
            NEXT_FREE(PREV_FREE(block_ptr)) = NEXT_FREE(block_ptr);
            if ( NEXT_FREE(block_ptr) != NULL ) {
               /* 
               ** Now have to make the NEXT_FREE of this block
               ** point to the PREV_FREE of this block
               */
               PREV_FREE(NEXT_FREE(block_ptr)) = PREV_FREE(block_ptr);
            } /* Endif */

         } /* Endif */

#if MQX_MEMORY_FREE_LIST_SORTED == 1
         //if ( block_ptr == mem_pool_ptr->POOL_FREE_CURRENT_BLOCK ) {
         if (old_block_ptr == mem_pool_ptr->POOL_FREE_CURRENT_BLOCK ) {
            /* Reset the freelist insertion sort by _mem_free */
            mem_pool_ptr->POOL_FREE_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
         } /* Endif */
#endif

         /* Reset the __mem_test freelist pointer */
         mem_pool_ptr->POOL_FREE_CHECK_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

         /* 
         ** Set the curent pool block to the start of the free list, so
         ** that if this task pre-empted another that was performing a
         ** _mem_alloc, the other task will restart it's search for a block
         */
         mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
            
          /* Remember some statistics */
         next_block_ptr = NEXT_PHYS(block_ptr);
         if (((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_HIGHEST_MEMORY_USED) &&
             ((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_PTR) && 
             ((char _PTR_)(next_block_ptr) < (char _PTR_) mem_pool_ptr->POOL_END_PTR)
             )
         {
            mem_pool_ptr->POOL_HIGHEST_MEMORY_USED = ((char _PTR_)(next_block_ptr) - 1);
         } /* Endif */

         /* Link the block onto the task descriptor. */
         block_ptr->NEXTBLOCK = td_ptr->MEMORY_RESOURCE_LIST;
         td_ptr->MEMORY_RESOURCE_LIST = (pointer)(&block_ptr->USER_AREA);

         block_ptr->MEM_POOL_PTR = (pointer)mem_pool_ptr;

#if MQX_CHECK_VALIDITY
      /* Check that user area is aligned on a cache line boundary */
      if ( !_MEMORY_ALIGNED(&block_ptr->USER_AREA) ) {
         *error_ptr = MQX_INVALID_CONFIGURATION;
         return((pointer)NULL);
      } /* Endif */
#endif
      
         return( (pointer)(&block_ptr->USER_AREA ) );

      } else {
         block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(block_ptr);
      } /* Endif */

   } /* Endwhile */

#ifdef lint
   return( NULL );        /* to satisfy lint */
#endif

} /* Endbody */

#else

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_internal_align
* Returned Value   : pointer. NULL is returned upon error.
* Comments         :
*       Allocate a aligned block of memory for a task from the free list
*       
*
*END*---------------------------------------------------------*/
pointer _mem_alloc_internal_align
   ( 
      /* [IN] the size of the memory block */
      _mem_size           requested_size,
      
      /* [IN] requested alignement (e.g. 8 for alignement to 8 bytes) */
      _mem_size           req_align,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR       td_ptr,
      
      /* [IN] which pool to allocate from */
      MEMPOOL_STRUCT_PTR  mem_pool_ptr,

      /* [OUT] error code for operation */
      _mqx_uint_ptr       error_ptr
   )
{ /* Body */
   STOREBLOCK_STRUCT_PTR   block_ptr, free_block_ptr;
   STOREBLOCK_STRUCT_PTR   next_block_ptr = NULL;
   STOREBLOCK_STRUCT_PTR   next_next_block_ptr;
   _mem_size               block_size;
   _mem_size               next_block_size;
   _mem_size               shift;

   *error_ptr = MQX_OK;

   /* 
   ** Adjust message size to allow for block management overhead
   ** and force size to be aligned.
   */
   requested_size += (_mem_size)(FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA));
#if MQX_CHECK_ERRORS
   if (requested_size <  MQX_MIN_MEMORY_STORAGE_SIZE) {
      requested_size = MQX_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */
#endif

   _MEMORY_ALIGN_VAL_LARGER(requested_size);

   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   
   while ( TRUE ) {      

      /* 
      ** Save the current block pointer.
      ** We will be enabling access to higher priority tasks.
      ** A higher priority task may pre-empt the current task
      ** and may do a memory allocation.  If this is true,
      ** the higher priority task will reset the POOL_ALLOC_CURRENT_BLOCK
      ** upon exit, and the current task will start the search over
      ** again.  
      */
      mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = block_ptr;

      /* allow pending interrupts */
      _int_enable();
      _int_disable();

      /* Get block pointer in case reset by a higher priority task */
      block_ptr = mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK;

      if (block_ptr == NULL) { /* Null pointer */
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_OUT_OF_MEMORY;
         return( NULL );        /* request failed */
      } /* Endif */

#if MQX_CHECK_VALIDITY
      if ( !_MEMORY_ALIGNED(block_ptr) || BLOCK_IS_USED(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         return((pointer)NULL);
      }
      
      if ( ! VALID_CHECKSUM(block_ptr) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = block_ptr;
         *error_ptr = MQX_INVALID_CHECKSUM;
         return((pointer)NULL);
      }
#endif

      block_size = block_ptr->BLOCKSIZE;
      shift = (((_mem_size)&block_ptr->USER_AREA + req_align) & ~(req_align - 1)) - (_mem_size)&block_ptr->USER_AREA;
      
      if (shift < (2 * MQX_MIN_MEMORY_STORAGE_SIZE)) {
         shift = (((_mem_size)&block_ptr->USER_AREA + (3 * MQX_MIN_MEMORY_STORAGE_SIZE) + req_align) & ~(req_align - 1)) - (_mem_size)&block_ptr->USER_AREA;
      }

      if (block_size >= requested_size + shift) {
         /* request fits into this block */

         // create new free block
         free_block_ptr = (STOREBLOCK_STRUCT_PTR)((char _PTR_)block_ptr);
         block_ptr = (STOREBLOCK_STRUCT_PTR)(((char _PTR_)block_ptr) + shift);
         
         block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)free_block_ptr;

         next_block_size = block_size - requested_size - shift;
         if (next_block_size >= (2 * MQX_MIN_MEMORY_STORAGE_SIZE) ) {
            /* 
            ** The current block is big enough to split.
            ** into 2 blocks.... the part to be allocated is one block,
            ** and the rest remains as a free block on the free list.
            */

            next_block_ptr = (STOREBLOCK_STRUCT_PTR)((char _PTR_)block_ptr + requested_size);

            /* Initialize the new physical block values */
            next_block_ptr->BLOCKSIZE = next_block_size;
            next_block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)block_ptr;
            MARK_BLOCK_AS_FREE(next_block_ptr);

            CALC_CHECKSUM(next_block_ptr);

            /* Link new block into the free list */
            next_block_ptr->NEXTBLOCK = free_block_ptr->NEXTBLOCK;
            block_ptr->NEXTBLOCK = (pointer)next_block_ptr;

            next_block_ptr->USER_AREA = (pointer)free_block_ptr;
            if (next_block_ptr->NEXTBLOCK != NULL ) {
               ((STOREBLOCK_STRUCT_PTR)next_block_ptr->NEXTBLOCK)->USER_AREA = (pointer)next_block_ptr;
            }
           
            /* 
            ** Modify the current block, to point to this newly created
            ** block which is now the next physical block.
            */
            block_ptr->BLOCKSIZE = requested_size;

            /* 
            ** Modify the block on the other side of the next block
            ** (the next next block) so that it's previous block pointer
            ** correctly point to the next block.
            */
            next_next_block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_PHYS(next_block_ptr);
            PREV_PHYS(next_next_block_ptr) = next_block_ptr;
            CALC_CHECKSUM(next_next_block_ptr);
         } else {

            /* Take the entire block */
            requested_size = next_block_size;

         } /* Endif */
         

         /* modify the new physical block values */
         free_block_ptr->BLOCKSIZE = shift;
         free_block_ptr->NEXTBLOCK = (pointer)next_block_ptr;
         //free_block_ptr->USER_AREA
         MARK_BLOCK_AS_FREE(free_block_ptr);

         CALC_CHECKSUM(free_block_ptr);


         /* Set the size of the block */
         block_ptr->BLOCKSIZE = requested_size;
         block_ptr->PREVBLOCK = (STOREBLOCK_STRUCT_PTR)free_block_ptr;
         block_ptr->MEM_TYPE = 0;
         MARK_BLOCK_AS_USED(block_ptr, td_ptr->TASK_ID);

         CALC_CHECKSUM(block_ptr);
            
          /* Remember some statistics */
         next_block_ptr = NEXT_PHYS(block_ptr);
         if (((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_HIGHEST_MEMORY_USED) &&
             ((char _PTR_)(next_block_ptr) > (char _PTR_) mem_pool_ptr->POOL_PTR) && 
             ((char _PTR_)(next_block_ptr) < (char _PTR_) mem_pool_ptr->POOL_END_PTR)
             )
         {
            mem_pool_ptr->POOL_HIGHEST_MEMORY_USED = ((char _PTR_)(next_block_ptr) - 1);
         } /* Endif */

         /* Link the block onto the task descriptor. */
         block_ptr->NEXTBLOCK = td_ptr->MEMORY_RESOURCE_LIST;
         td_ptr->MEMORY_RESOURCE_LIST = (pointer)(&block_ptr->USER_AREA);

         block_ptr->MEM_POOL_PTR = (pointer)mem_pool_ptr;

#if MQX_CHECK_VALIDITY
      /* Check that user area is aligned on a cache line boundary */
      if ( !_MEMORY_ALIGNED(&block_ptr->USER_AREA) ) {
         *error_ptr = MQX_INVALID_CONFIGURATION;
         return((pointer)NULL);
      } /* Endif */
#endif
      
         return( (pointer)(&block_ptr->USER_AREA ) );

      } else {
         block_ptr = (STOREBLOCK_STRUCT_PTR)NEXT_FREE(block_ptr);
      }

   }

#ifdef lint
   return( NULL );        /* to satisfy lint */
#endif
}

#endif
#endif

/* EOF */
