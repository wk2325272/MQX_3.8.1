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
* $FileName: mem_fprt.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function that will free part of an allocated
*   memory block.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_free_part
* Returned Value   : _mqx_uint an mqx error code
* Comments         :
*   This function cuts an allocated block to the requested size
*
*END*--------------------------------------------------------*/

_mqx_uint _mem_free_part
   (
      /* [IN] the address of the memory block whose size is to change */
      pointer    mem_ptr,

      /* [IN] the new size for the block */
      _mem_size  requested_size
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   STOREBLOCK_STRUCT_PTR  block_ptr;
   STOREBLOCK_STRUCT_PTR  prev_block_ptr;
   STOREBLOCK_STRUCT_PTR  next_block_ptr;
   STOREBLOCK_STRUCT_PTR  new_block_ptr;
   _mem_size              size;
   _mem_size              block_size;
   _mem_size              new_block_size;
   _mqx_uint              result_code;

   
   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_mem_free_part, mem_ptr, requested_size);

#if MQX_CHECK_ERRORS
   /* Make sure a correct pointer was passed in.    */
   if (mem_ptr == NULL) {
      _task_set_error(MQX_INVALID_POINTER);
      _KLOGX2(KLOG_mem_free_part, MQX_INVALID_POINTER);
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif 

   /* Verify the block size */
   block_ptr = GET_MEMBLOCK_PTR(mem_ptr);

#if MQX_CHECK_ERRORS
   if (! _MEMORY_ALIGNED(block_ptr)) {
      _task_set_error(MQX_INVALID_POINTER);
      _KLOGX2(KLOG_mem_free_part, MQX_INVALID_POINTER);
      return(MQX_INVALID_POINTER);
   } /* Endif */

   if ( (block_ptr->BLOCKSIZE < MQX_MIN_MEMORY_STORAGE_SIZE) ||
      BLOCK_IS_FREE(block_ptr) )
   {
      _task_set_error(MQX_INVALID_POINTER);
      kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR = block_ptr;
      _KLOGX3(KLOG_mem_free_part, MQX_INVALID_POINTER, block_ptr);
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif

#if MQX_CHECK_VALIDITY   
   _int_disable();
   if ( ! VALID_CHECKSUM(block_ptr) ) {
      _int_enable();
      _task_set_error(MQX_INVALID_CHECKSUM);
      kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR = block_ptr;
      _KLOGX3(KLOG_mem_free_part, MQX_INVALID_CHECKSUM, block_ptr);
      return(MQX_INVALID_CHECKSUM);
   } /* Endif */
   _int_enable();
#endif

   /*  Walk through the memory resources of the task descriptor.
    *  Two pointers are maintained, one to the current block
    *  and one to the previous block.
    */
   next_block_ptr   = (STOREBLOCK_STRUCT_PTR)
      kernel_data->ACTIVE_PTR->MEMORY_RESOURCE_LIST;
   prev_block_ptr = GET_MEMBLOCK_PTR(&kernel_data->ACTIVE_PTR->MEMORY_RESOURCE_LIST);
      
   /* Scan the task's memory resource list searching for the block to
    * free, Stop when the current pointer is equal to the block to free
    * or the end of the list is reached.
    */
   while (  next_block_ptr  &&
   ((pointer)next_block_ptr != mem_ptr) ) {
      /* The block is not found, and the end of the list has not been
       * reached, so move down the list.
       */
      prev_block_ptr = GET_MEMBLOCK_PTR(next_block_ptr);
      next_block_ptr = (STOREBLOCK_STRUCT_PTR)prev_block_ptr->NEXTBLOCK;
   } /* Endwhile */

#if MQX_CHECK_ERRORS
   if ( next_block_ptr == NULL ) {
      /* The specified block does not belong to the calling task. */
      _task_set_error(MQX_NOT_RESOURCE_OWNER);
      _KLOGX2(KLOG_mem_free_part, MQX_NOT_RESOURCE_OWNER);
      return(MQX_NOT_RESOURCE_OWNER);
   } /* Endif */
#endif

    /* determine the size of the block.  */
   block_size = block_ptr->BLOCKSIZE;

   size = requested_size + (_mem_size)FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA);
   if (size < MQX_MIN_MEMORY_STORAGE_SIZE) {
      size = MQX_MIN_MEMORY_STORAGE_SIZE;
   } /* Endif */
   _MEMORY_ALIGN_VAL_LARGER(size);

#if MQX_CHECK_ERRORS
   /* Verify that the size parameter is within range of the block size. */
   if (size <= block_size) {
#endif
      /* Adjust the size to allow for the overhead and force alignment */

      /* Compute the size of the new_ block that would be created. */
      new_block_size = block_size - size;

      /* Decide if it worthwile to split the block. If the amount of space
       * returned is not at least twice the size of the block overhead, 
       * then dont bother.
       */
      if (new_block_size >= (2*MQX_MIN_MEMORY_STORAGE_SIZE) ) {

         /* Create an 'inuse' block */
         new_block_ptr            = 
            (STOREBLOCK_STRUCT_PTR)((char _PTR_)block_ptr + size);
         new_block_ptr->BLOCKSIZE    = new_block_size;
         PREV_PHYS(new_block_ptr)    = block_ptr;
         new_block_ptr->TASK_NUMBER  = block_ptr->TASK_NUMBER;
         new_block_ptr->MEM_POOL_PTR = block_ptr->MEM_POOL_PTR;
         CALC_CHECKSUM(new_block_ptr);
         _int_disable();
         /* Split the block */
         block_ptr->BLOCKSIZE        = size;
         CALC_CHECKSUM(block_ptr);

         /* make sure right physical neighbour knows about it */
         block_ptr = NEXT_PHYS(new_block_ptr);
         PREV_PHYS(block_ptr) = new_block_ptr;
         CALC_CHECKSUM(block_ptr);

         /* Link the new block onto the requestor's task descriptor. */
         new_block_ptr->NEXTBLOCK = kernel_data->ACTIVE_PTR->MEMORY_RESOURCE_LIST;
         kernel_data->ACTIVE_PTR->MEMORY_RESOURCE_LIST =
            (char _PTR_)(&new_block_ptr->USER_AREA);
         _int_enable();

         result_code = _mem_free((pointer)&new_block_ptr->USER_AREA);
      } else {
         result_code = MQX_OK;
      } /* Endif */
#if MQX_CHECK_ERRORS
   } else {
      result_code = MQX_INVALID_SIZE;
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if ( result_code != MQX_OK ) {
      _task_set_error(result_code);
   } /* Endif */
#endif

   _KLOGX2(KLOG_mem_free_part, result_code);
   return (result_code);

} /* Endbody */

#endif

/* EOF */
