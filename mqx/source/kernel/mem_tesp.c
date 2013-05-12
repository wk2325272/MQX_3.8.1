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
* $FileName: mem_tesp.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function that tests a memory pool's validity
*   It checks for incorrect checksums, and incorrect memory pointers.
*   This function can run concurrently with any other memory functions.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_test_pool
* Returned Value   : _mqx_uint
*    A task error code on error, MQX_OK if no error
*    CORRUPT_STORAGE_POOL_POINTERS, CORRUPT_STORAGE_POOL,
*    INVALID_CHECKSUM, CORRUPT_STORAGE_POOL_FREE_LIST
* Comments         :
*   This function a specified memory for any errors.  It uses
* global memory pointers, disable and enable to allow it
* to run freely in the background as a low priority task,
* but have a small amount
* of disabled code.
*
*END*--------------------------------------------------------*/

_mqx_uint _mem_test_pool
   (
      /* [IN] the pool to check */
      _mem_pool_id pool_id

   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR  kernel_data;)
          STOREBLOCK_STRUCT_PTR   next_block_ptr;
          MEMPOOL_STRUCT_PTR      mem_pool_ptr;
          _mqx_uint               result = MQX_OK;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE2(KLOG_mem_test_pool, pool_id);

   mem_pool_ptr = (MEMPOOL_STRUCT_PTR)pool_id;

   /* First check the physical blocks */

   mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK = (STOREBLOCK_STRUCT_PTR)mem_pool_ptr->POOL_PTR;
   while (mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK < mem_pool_ptr->POOL_END_PTR){
      if ( (! mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK) ||
           (! _MEMORY_ALIGNED(mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK)))
      {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = 
            (STOREBLOCK_STRUCT_PTR)mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK;
         result = MQX_CORRUPT_STORAGE_POOL;
         break;
      } /* Endif */

      _int_disable();
      if ( ! VALID_CHECKSUM(mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = 
            (STOREBLOCK_STRUCT_PTR)mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK;
         _int_enable();
         result = MQX_INVALID_CHECKSUM;
         break;
      } /* Endif */

      next_block_ptr = NEXT_PHYS(mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK);
      if (next_block_ptr->PREVBLOCK != mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK){
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = next_block_ptr;
         _int_enable();
         result = MQX_CORRUPT_STORAGE_POOL;
         break;
      } /* Endif */
      mem_pool_ptr->POOL_PHYSICAL_CHECK_BLOCK = next_block_ptr;
      _int_enable();

   } /* Endwhile */

   if (result != MQX_OK) {
      _KLOGX3(KLOG_mem_test_pool, result, mem_pool_ptr->POOL_BLOCK_IN_ERROR);
      return(result);
   } /* Endif */

   /* Now check the free list */
   _int_disable();
   if ( mem_pool_ptr->POOL_FREE_LIST_PTR == NULL ) { /* no free list to check */
      _int_enable();
      return MQX_OK;
   } /* Endif */

   mem_pool_ptr->POOL_FREE_CHECK_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;
   next_block_ptr = mem_pool_ptr->POOL_FREE_CHECK_BLOCK;
   if ( next_block_ptr->USER_AREA != (pointer)NULL ) {
      _KLOGX3(KLOG_mem_test_pool, MQX_CORRUPT_STORAGE_POOL_FREE_LIST, next_block_ptr );
      mem_pool_ptr->POOL_BLOCK_IN_ERROR = next_block_ptr;
      _int_enable();
      return(MQX_CORRUPT_STORAGE_POOL_FREE_LIST);
   } /* Endif */

   _int_enable();

   while ( mem_pool_ptr->POOL_FREE_CHECK_BLOCK < mem_pool_ptr->POOL_END_PTR ) {
      if ( (! mem_pool_ptr->POOL_FREE_CHECK_BLOCK) ||
           (! _MEMORY_ALIGNED(mem_pool_ptr->POOL_FREE_CHECK_BLOCK)))
      {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = mem_pool_ptr->POOL_FREE_CHECK_BLOCK;
         result = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         break;
      } /* Endif */

      _int_disable();
      if ( ! VALID_CHECKSUM(mem_pool_ptr->POOL_FREE_CHECK_BLOCK) ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = mem_pool_ptr->POOL_FREE_CHECK_BLOCK;
         _int_enable();
         result = MQX_INVALID_CHECKSUM;
         break;
      } /* Endif */
      _int_enable();

      _int_disable();
      if ( BLOCK_IS_USED(mem_pool_ptr->POOL_FREE_CHECK_BLOCK) ) {
         /* An allocated block on the free list */
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = mem_pool_ptr->POOL_FREE_CHECK_BLOCK;
         _int_enable();  
         result = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         break;
      } /* Endif */

      next_block_ptr = (STOREBLOCK_STRUCT_PTR)
         NEXT_FREE(mem_pool_ptr->POOL_FREE_CHECK_BLOCK);
      if ( ! next_block_ptr ) {
         _int_enable();  /* If zero, free list has been completed */
         break;
      } /* Endif */
      if ( next_block_ptr->USER_AREA != (char _PTR_)mem_pool_ptr->POOL_FREE_CHECK_BLOCK ) {
         mem_pool_ptr->POOL_BLOCK_IN_ERROR = mem_pool_ptr->POOL_FREE_CHECK_BLOCK;
         _int_enable();
         result = MQX_CORRUPT_STORAGE_POOL_FREE_LIST;
         break;
      } /* Endif */
      mem_pool_ptr->POOL_FREE_CHECK_BLOCK = next_block_ptr;
      _int_enable();

   } /* Endwhile */

   if (result == MQX_OK) {
      _KLOGX2(KLOG_mem_test_pool, result);
   } else {
      _KLOGX3(KLOG_mem_test_pool, result, mem_pool_ptr->POOL_BLOCK_IN_ERROR);
   } /* Endif */
   return(result);

} /* Endbody */

#endif
/* EOF */
