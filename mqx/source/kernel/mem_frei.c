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
* $FileName: mem_frei.c$
* $Version : 3.0.4.0$
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
* Function Name    : _mem_coalesce_internal
* Returned Value   : boolean
* Comments         :
*   This function coalesces any free block found adjacent
*   to the provided block.  It returns a flag 
*   indicating whether or not coalescing was performed.
*
*END*---------------------------------------------------------*/

boolean _mem_check_coalesce_internal
   (
      /* 
      ** [IN] the address of a memory block, whose neighbours have
      ** to be checked to see if they are free.
      */
      STOREBLOCK_STRUCT_PTR passed_block_ptr

   )
{ /* Body */
   register STOREBLOCK_STRUCT_PTR   block_ptr;
   register STOREBLOCK_STRUCT_PTR   prev_block_ptr;
   register STOREBLOCK_STRUCT_PTR   next_block_ptr;
   MEMPOOL_STRUCT_PTR               mem_pool_ptr;
   boolean                          have_coalesced = FALSE;

   block_ptr    = passed_block_ptr;
   mem_pool_ptr = (MEMPOOL_STRUCT_PTR)block_ptr->MEM_POOL_PTR;

   /* Check the previous physical neighbour */
   prev_block_ptr = PREV_PHYS(block_ptr);
   if ((prev_block_ptr != NULL) && BLOCK_IS_FREE(prev_block_ptr)){  
      /* the block previous to this one is free */

      /*  make the current block a free block so it can't be freed again */
      block_ptr->NEXTBLOCK = NULL;
      MARK_BLOCK_AS_FREE(block_ptr);

      /* Add the current block to the previous block */
      prev_block_ptr->BLOCKSIZE += block_ptr->BLOCKSIZE;

      /* Modify the next physical block to point to the previous block */
      next_block_ptr = NEXT_PHYS(block_ptr);
      PREV_PHYS(next_block_ptr) = prev_block_ptr;
      CALC_CHECKSUM(prev_block_ptr);
      CALC_CHECKSUM(next_block_ptr);

      block_ptr = prev_block_ptr;  /* Set up as the current block */
      have_coalesced = TRUE;

   } /* Endif */
   
   /* Now, check the next block to see if it is free */
   next_block_ptr = NEXT_PHYS(block_ptr);
   if ( BLOCK_IS_FREE(next_block_ptr) ) {
      /* the next block is free */   

      if ( mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK == next_block_ptr ) {
         /* We must modify the _mem_alloc pointer to not point
         ** at the next block
         */
         mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = block_ptr;
      } /* Endif */

      if (have_coalesced) {  
         /* the current block is already on the free list */      

         /* Remove the block from the free list */
         if ( block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
            mem_pool_ptr->POOL_FREE_LIST_PTR = 
               (STOREBLOCK_STRUCT_PTR)NEXT_FREE(block_ptr);
            if ( NEXT_FREE(block_ptr) != NULL ) {
               PREV_FREE(NEXT_FREE(block_ptr)) = 0;
            } /* Endif */
         } else {
            NEXT_FREE(PREV_FREE(block_ptr)) = NEXT_FREE(block_ptr);
            if ( NEXT_FREE(block_ptr) != NULL ) {
               PREV_FREE(NEXT_FREE(block_ptr)) = PREV_FREE(block_ptr);
            } /* Endif */
         } /* Endif */

      } else {

         /* Make the block a free block */
         block_ptr->NEXTBLOCK  = NULL;
         MARK_BLOCK_AS_FREE(block_ptr);

      } /* Endif */

      /* 
      ** The current block is now a free block not on the free list .
      ** the freelist pointers have to be modified so that the next
      ** block is removed from the list, replace with the current block.
      */

      /* set the next free block to be the same as the one on the free list */
      NEXT_FREE(block_ptr) = NEXT_FREE(next_block_ptr);

      /* 
      ** And set the back pointer of the block after the next free block 
      ** to point back to the current block
      */
      if ( NEXT_FREE(block_ptr) != NULL ) {
         PREV_FREE(NEXT_FREE(block_ptr)) = (STOREBLOCK_STRUCT_PTR)block_ptr;
      } /* Endif */

      if (next_block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR) {
         /* 
         ** If the next block pointer was at the head of the free list,
         ** the kernel free list pointer must be updated
         */
         mem_pool_ptr->POOL_FREE_LIST_PTR = block_ptr;
         PREV_FREE(block_ptr) = NULL;
      } else {
         /* 
         ** Otherwise we need to adjust the pointers of the block that
         ** was previous to the next block on the free list
         */
         PREV_FREE(block_ptr) = PREV_FREE(next_block_ptr);
         if ( PREV_FREE(block_ptr) != NULL ) {
            NEXT_FREE(PREV_FREE(block_ptr)) = (pointer)block_ptr;
         } /* Endif */
      } /* Endif */

      /* Add the next block onto the current block */
      block_ptr->BLOCKSIZE += next_block_ptr->BLOCKSIZE;

      /* 
      ** Reset the previous physical block pointer of
      ** the block after the next block (ie the next next block)
      */
      prev_block_ptr = NEXT_PHYS(next_block_ptr);
      PREV_PHYS(prev_block_ptr) = block_ptr;
      CALC_CHECKSUM(prev_block_ptr);
      CALC_CHECKSUM(block_ptr);

      have_coalesced = TRUE;
   } /* Endif */


   if (have_coalesced) {

      /* Reset the _mem_test pointers */
      mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK = 
         (STOREBLOCK_STRUCT_PTR)mem_pool_ptr->POOL_PTR;
      mem_pool_ptr->POOL_FREE_CHECK_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

#if MQX_MEMORY_FREE_LIST_SORTED == 1
      /* 
      ** Reset the freelist current block pointer in case we pre-empted
      ** another task
      */
      mem_pool_ptr->POOL_FREE_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
#endif
   } /* Endif */

   return (have_coalesced);
      
} /* Endbody */


#endif
/* EOF */
