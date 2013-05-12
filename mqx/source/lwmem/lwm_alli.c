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
* $FileName: lwm_alli.c$
* $Version : 3.8.21.1$
* $Date    : Jun-21-2012$
*
* Comments:
*
*   This file contains the function that allocates a memory block.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_alloc_internal
* Returned Value   : pointer. NULL is returned upon error.
* Comments         :
*       Allocate a block of memory for a task from the free list
*
*END*---------------------------------------------------------*/

pointer _lwmem_alloc_internal
   ( 
      /* [IN] the size of the memory block */
      _mem_size      requested_size,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR  td_ptr,
      
      /* [IN] which pool to allocate from */
      _lwmem_pool_id pool_id,
      
      /* [IN] zero the memory after it is allocated  */
      boolean zero
   )
{ /* Body */
   LWMEM_BLOCK_STRUCT_PTR  block_ptr;
   LWMEM_BLOCK_STRUCT_PTR  next_block_ptr;
   LWMEM_BLOCK_STRUCT_PTR  prev_block_ptr = NULL;
   _mem_size               block_size;
   _mem_size               next_block_size;
   LWMEM_POOL_STRUCT_PTR   mem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)pool_id;
      
#if MQX_CHECK_VALIDITY
   if (mem_pool_ptr->VALID != LWMEM_POOL_VALID) {
      _task_set_error(MQX_LWMEM_POOL_INVALID);
      return(NULL);
   } /* Endif */
#endif

   /* 
   ** Adjust message size to allow for block management overhead
   ** and force size to be aligned.
   */
   requested_size += (_mem_size)sizeof(LWMEM_BLOCK_STRUCT);
   if (requested_size <  LWMEM_MIN_MEMORY_STORAGE_SIZE) {
      requested_size = LWMEM_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */

   _MEMORY_ALIGN_VAL_LARGER(requested_size);

   _int_disable();
   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   while ( block_ptr != NULL ) {
      /* Provide window for higher priority tasks */
      mem_pool_ptr->POOL_ALLOC_PTR = block_ptr;
      _int_enable();
      _int_disable();
      block_ptr = mem_pool_ptr->POOL_ALLOC_PTR;
      if (block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR) {
         prev_block_ptr = block_ptr;
      } /* Endif */
      block_size = block_ptr->BLOCKSIZE;
      if (block_size >= requested_size) {
         /* request fits into this block */
         next_block_size = block_size - requested_size;
         if (next_block_size >= LWMEM_MIN_MEMORY_STORAGE_SIZE) {
            /* 
            ** The current block is big enough to split.
            ** into 2 blocks.... the part to be allocated is one block,
            ** and the rest remains as a free block on the free list.
            */
            next_block_ptr = (LWMEM_BLOCK_STRUCT_PTR)(pointer)
               ((uchar_ptr)block_ptr + requested_size);
            /* Initialize the new physical block values */
            next_block_ptr->BLOCKSIZE = next_block_size;
            /* Link new block into the free list */
            next_block_ptr->POOL        = mem_pool_ptr;
            next_block_ptr->U.NEXTBLOCK = block_ptr->U.NEXTBLOCK;
            block_ptr->U.NEXTBLOCK      = (pointer)next_block_ptr;

            /* Modify the current block, to point to this newly created block*/
            block_ptr->BLOCKSIZE = requested_size;
         } else {
            /* Take the entire block */
            requested_size = block_size;
            next_block_ptr = block_ptr->U.NEXTBLOCK;
         } /* Endif */

         if ( block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
            /* At the head of the free list */
            mem_pool_ptr->POOL_FREE_LIST_PTR = next_block_ptr;
         } else {
            prev_block_ptr->U.NEXTBLOCK = next_block_ptr;
         } /* Endif */

         mem_pool_ptr->POOL_ALLOC_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
         mem_pool_ptr->POOL_FREE_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
         mem_pool_ptr->POOL_TEST_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
         mem_pool_ptr->POOL_TEST2_PTR = mem_pool_ptr->POOL_ALLOC_START_PTR;
#if MQX_TASK_DESTRUCTION
         mem_pool_ptr->POOL_DESTROY_PTR = mem_pool_ptr->POOL_ALLOC_START_PTR;
#endif
         /* Indicate the block is in use */
         block_ptr->U.S.TASK_NUMBER = TASK_NUMBER_FROM_TASKID(td_ptr->TASK_ID);
         block_ptr->U.S.MEM_TYPE = 0;
         block_ptr->POOL = (_lwmem_pool_id)mem_pool_ptr;
         
         if ((uint_32)block_ptr > (uint_32)mem_pool_ptr->HIGHWATER) {
            mem_pool_ptr->HIGHWATER = block_ptr;
          }
          
         _int_enable();
         
         if (zero) {
            _mem_zero((pointer)((uchar_ptr)block_ptr + sizeof(LWMEM_BLOCK_STRUCT)), requested_size - sizeof(LWMEM_BLOCK_STRUCT));
         } /* Endif */
         return((pointer)((uchar_ptr)block_ptr + sizeof(LWMEM_BLOCK_STRUCT)));
      } else {
         prev_block_ptr = block_ptr;
         block_ptr = block_ptr->U.NEXTBLOCK;
      } /* Endif */
   } /* Endwhile */

   _int_enable();
   _task_set_error(MQX_OUT_OF_MEMORY);
   return(NULL);
   

} /* Endbody */

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_alloc_at_internal
* Returned Value   : pointer. NULL is returned upon error.
* Comments         :
*       Allocate a block of memory for a task from the free list
*
*END*---------------------------------------------------------*/

pointer _lwmem_alloc_at_internal
   ( 
      /* [IN] the size of the memory block */
      _mem_size      requested_size,
      
      /* [IN] the addr of the memory block */
      pointer        requested_addr,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR  td_ptr,
      
      /* [IN] which pool to allocate from */
      _lwmem_pool_id pool_id,
      
      /* [IN] zero the memory after it is allocated  */
      boolean zero
   )
{ /* Body */
   LWMEM_BLOCK_STRUCT_PTR  block_ptr, requested_block_ptr;
   LWMEM_BLOCK_STRUCT_PTR  next_block_ptr;
   LWMEM_BLOCK_STRUCT_PTR  prev_block_ptr = NULL;
   _mem_size               block_size;
   _mem_size               next_block_size, free_block_size;
   LWMEM_POOL_STRUCT_PTR   mem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)pool_id;
      
#if MQX_CHECK_VALIDITY
   if (mem_pool_ptr->VALID != LWMEM_POOL_VALID) {
      _task_set_error(MQX_LWMEM_POOL_INVALID);
      return(NULL);
   } /* Endif */
#endif

   /* 
   ** Adjust message size to allow for block management overhead
   ** and force size to be aligned.
   */
   requested_size += (_mem_size)sizeof(LWMEM_BLOCK_STRUCT);
   if (requested_size <  LWMEM_MIN_MEMORY_STORAGE_SIZE) {
      requested_size = LWMEM_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */

   _MEMORY_ALIGN_VAL_LARGER(requested_size);

   _int_disable();
   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   while ( block_ptr != NULL ) {
      /* Provide window for higher priority tasks */
      mem_pool_ptr->POOL_ALLOC_PTR = block_ptr;
      
      _int_enable();
      _int_disable();
      
      block_ptr = mem_pool_ptr->POOL_ALLOC_PTR;
      if (block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR) {
         prev_block_ptr = block_ptr;
      }
      
      block_size = block_ptr->BLOCKSIZE;
      if ((uchar_ptr)block_ptr + LWMEM_MIN_MEMORY_STORAGE_SIZE <= (uchar_ptr)requested_addr - sizeof(LWMEM_BLOCK_STRUCT) &&
          (uchar_ptr)block_ptr + block_size >= (uchar_ptr)requested_addr - sizeof(LWMEM_BLOCK_STRUCT) + requested_size) {
         /* request fits into this block */
              
         requested_block_ptr = (LWMEM_BLOCK_STRUCT_PTR)((uchar_ptr)requested_addr - sizeof(LWMEM_BLOCK_STRUCT));

         free_block_size = (uchar_ptr)requested_block_ptr - (uchar_ptr)block_ptr;
         /* requested block size is known */
         next_block_size = block_size - requested_size - free_block_size;

         // free_block_size is always > LWMEM_MIN_MEMORY_STORAGE_SIZE
         block_ptr->BLOCKSIZE = free_block_size;

         // chek and prepare free block after requested block
         if (next_block_size >= LWMEM_MIN_MEMORY_STORAGE_SIZE) {
            /* 
            ** The current block is big enough to split.
            ** into 2 blocks.... the part to be allocated is one block,
            ** and the rest remains as a free block on the free list.
            */
            next_block_ptr = (LWMEM_BLOCK_STRUCT_PTR)(pointer)((uchar_ptr)requested_block_ptr + requested_size);
            /* Initialize the new physical block values */
            next_block_ptr->BLOCKSIZE = next_block_size;
            /* Link new block into the free list */
            next_block_ptr->POOL        = mem_pool_ptr;
            next_block_ptr->U.NEXTBLOCK = block_ptr->U.NEXTBLOCK;
            block_ptr->U.NEXTBLOCK = (pointer)next_block_ptr;
         } else {
            /* Take the entire block */
            requested_size += next_block_size;
            next_block_ptr = block_ptr->U.NEXTBLOCK;
         }

         // first free block always stay first         

         // add next free block to list
         //prev_block_ptr->U.NEXTBLOCK = next_block_ptr;

         /* Modify the allocated block information to point to next block */
         requested_block_ptr->BLOCKSIZE = requested_size;
         requested_block_ptr->U.NEXTBLOCK = next_block_ptr;

         mem_pool_ptr->POOL_ALLOC_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
         mem_pool_ptr->POOL_FREE_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
         mem_pool_ptr->POOL_TEST_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
         mem_pool_ptr->POOL_TEST2_PTR = mem_pool_ptr->POOL_ALLOC_START_PTR;
#if MQX_TASK_DESTRUCTION
         mem_pool_ptr->POOL_DESTROY_PTR = mem_pool_ptr->POOL_ALLOC_START_PTR;
#endif
         /* Indicate the block is in use */
         requested_block_ptr->U.S.TASK_NUMBER = TASK_NUMBER_FROM_TASKID(td_ptr->TASK_ID);
         requested_block_ptr->U.S.MEM_TYPE = 0;
         requested_block_ptr->POOL      = (_lwmem_pool_id)mem_pool_ptr;
         
         if ((uint_32)block_ptr > (uint_32)mem_pool_ptr->HIGHWATER) {
            mem_pool_ptr->HIGHWATER = block_ptr;
         }
          
         _int_enable();
         
         if (zero) {
            _mem_zero((pointer)requested_addr, requested_size - sizeof(LWMEM_BLOCK_STRUCT));
         } /* Endif */
         return requested_addr;
      } else {
         prev_block_ptr = block_ptr;
         block_ptr = block_ptr->U.NEXTBLOCK;
      } /* Endif */
   } /* Endwhile */

   _int_enable();
   _task_set_error(MQX_OUT_OF_MEMORY);
   return(NULL);
}

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_alloc_align_internal
* Returned Value   : pointer. NULL is returned upon error.
* Comments         :
*       Allocate a aligned block of memory for a task from the free list
*       
*
*END*---------------------------------------------------------*/
pointer _lwmem_alloc_align_internal
   ( 
      /* [IN] the size of the memory block */
      _mem_size      requested_size,

      /* [IN] requested alignement (e.g. 8 for alignement to 8 bytes) */
      _mem_size      req_align,
      
      /* [IN] owner TD */
      TD_STRUCT_PTR  td_ptr,
      
      /* [IN] which pool to allocate from */
      _lwmem_pool_id pool_id,
      
      /* [IN] zero the memory after it is allocated  */
      boolean zero
   )
{ /* Body */
    LWMEM_BLOCK_STRUCT_PTR  block_ptr;
    LWMEM_BLOCK_STRUCT_PTR  next_block_ptr;
    LWMEM_BLOCK_STRUCT_PTR  prev_block_ptr;
    _mem_size               block_size;
    _mem_size               next_block_size;
    _mem_size               shift;
    LWMEM_POOL_STRUCT_PTR   mem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)pool_id;
      
#if MQX_CHECK_VALIDITY
    if (mem_pool_ptr->VALID != LWMEM_POOL_VALID) {
        _task_set_error(MQX_LWMEM_POOL_INVALID);
        return(NULL);
    }
#endif

    /* 
    ** Adjust message size to allow for block management overhead
    ** and force size to be aligned.
    */
    requested_size += (_mem_size)sizeof(LWMEM_BLOCK_STRUCT);
    if (requested_size <  LWMEM_MIN_MEMORY_STORAGE_SIZE) {
        requested_size = LWMEM_MIN_MEMORY_STORAGE_SIZE;
    }

    _MEMORY_ALIGN_VAL_LARGER(requested_size);

    _int_disable();
    block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
    while ( block_ptr != NULL ) {
        /* Provide window for higher priority tasks */
        mem_pool_ptr->POOL_ALLOC_PTR = block_ptr;
        _int_enable();
        _int_disable();
        block_ptr = mem_pool_ptr->POOL_ALLOC_PTR;
        
        if (block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR) {
            prev_block_ptr = block_ptr;
        }

        shift = (((_mem_size)block_ptr + sizeof(LWMEM_BLOCK_STRUCT) + req_align) & ~(req_align - 1)) - ((_mem_size)block_ptr + sizeof(LWMEM_BLOCK_STRUCT));
  
        if (shift < (2 * LWMEM_MIN_MEMORY_STORAGE_SIZE)) {
            shift = (((_mem_size)block_ptr + sizeof(LWMEM_BLOCK_STRUCT) + (3 * LWMEM_MIN_MEMORY_STORAGE_SIZE) + req_align) & ~(req_align - 1)) - ((_mem_size)block_ptr + sizeof(LWMEM_BLOCK_STRUCT));
        }            
        
        block_size = block_ptr->BLOCKSIZE;
        if (block_size >= requested_size + shift) {
            /* request fits into this block */
            
            // create new free block
            prev_block_ptr = block_ptr;
            block_ptr = (LWMEM_BLOCK_STRUCT_PTR)(((uchar_ptr)block_ptr) + shift);
            prev_block_ptr->BLOCKSIZE = shift;
            block_ptr->U.NEXTBLOCK = prev_block_ptr->U.NEXTBLOCK;
            
            next_block_size = block_size - requested_size - shift;
            if (next_block_size >= LWMEM_MIN_MEMORY_STORAGE_SIZE) {
                /* 
                ** The current block is big enough to split.
                ** into 2 blocks.... the part to be allocated is one block,
                ** and the rest remains as a free block on the free list.
                */
                next_block_ptr = (LWMEM_BLOCK_STRUCT_PTR)(pointer)((uchar_ptr)block_ptr + requested_size);
                /* Initialize the new physical block values */
                next_block_ptr->BLOCKSIZE = next_block_size;
                /* Link new block into the free list */
                next_block_ptr->POOL        = mem_pool_ptr;
                next_block_ptr->U.NEXTBLOCK = block_ptr->U.NEXTBLOCK;
                block_ptr->U.NEXTBLOCK      = (pointer)next_block_ptr;
    
                /* Modify the current block, to point to this newly created block*/
                block_ptr->BLOCKSIZE = requested_size;
            } else {
                /* Take the entire block */
                requested_size = block_size;
                next_block_ptr = block_ptr->U.NEXTBLOCK;
            } /* Endif */

            if ( block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR ) {
                /* At the head of the free list */
                mem_pool_ptr->POOL_FREE_LIST_PTR = next_block_ptr;
            } else {
                prev_block_ptr->U.NEXTBLOCK = next_block_ptr;
            } /* Endif */

            mem_pool_ptr->POOL_ALLOC_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
            mem_pool_ptr->POOL_FREE_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
            mem_pool_ptr->POOL_TEST_PTR = mem_pool_ptr->POOL_FREE_LIST_PTR;
            mem_pool_ptr->POOL_TEST2_PTR = mem_pool_ptr->POOL_ALLOC_START_PTR;
#if MQX_TASK_DESTRUCTION
            mem_pool_ptr->POOL_DESTROY_PTR = mem_pool_ptr->POOL_ALLOC_START_PTR;
#endif
            /* Indicate the block is in use */
            block_ptr->U.S.TASK_NUMBER = TASK_NUMBER_FROM_TASKID(td_ptr->TASK_ID);
            block_ptr->U.S.MEM_TYPE = 0;
            block_ptr->POOL = (_lwmem_pool_id)mem_pool_ptr;
         
            if ((uint_32)block_ptr > (uint_32)mem_pool_ptr->HIGHWATER) {
                mem_pool_ptr->HIGHWATER = block_ptr;
            }
          
            _int_enable();
         
            if (zero) {
                _mem_zero((pointer)((uchar_ptr)block_ptr + sizeof(LWMEM_BLOCK_STRUCT)), requested_size - sizeof(LWMEM_BLOCK_STRUCT));
            }
            
            return ((pointer)((uchar_ptr)block_ptr + sizeof(LWMEM_BLOCK_STRUCT)));
        } else {
            prev_block_ptr = block_ptr;
            block_ptr = block_ptr->U.NEXTBLOCK;
        }
    }

    _int_enable();
    _task_set_error(MQX_OUT_OF_MEMORY);
    return(NULL);
}

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _usr_lwmem_alloc_internal
* Returned Value   : pointer. NULL is returned upon error.
* Comments         : allocates a block of memory
*
*END*---------------------------------------------------------*/

#if MQX_ENABLE_USER_MODE

pointer _usr_lwmem_alloc_internal
    (
        /* [IN] the size of the memory block */
        _mem_size requested_size
    )
{
    KERNEL_DATA_STRUCT_PTR  kernel_data;
    pointer                 result;
   
    _GET_KERNEL_DATA(kernel_data);
    _KLOGE2(KLOG_usr_lwmem_alloc, requested_size);

    result = _lwmem_alloc_internal(requested_size, kernel_data->ACTIVE_PTR,
        (_lwmem_pool_id)kernel_data->KD_USER_POOL, FALSE);

    _KLOGX2(KLOG_usr_lwmem_alloc, result);
    return(result);
}

#endif /* MQX_ENABLE_USER_MODE */
#endif /* MQX_USE_LWMEM */

/* EOF */
