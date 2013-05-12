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
* $FileName: mem_size.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function that returns the size of a memory block.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_size
* Returned Value   : _mem_size
*   MQX_INVALID_POINTER, MQX_CORRUPT_STORAGE_POOL, MQX_INVALID_CHECKSUM, 
* Comments         :
*    This routine returns the allocated size (in bytes) of a block
*    allocated using the MQX storage allocator (_mem_alloc).
*
*END*--------------------------------------------------------*/

_mem_size _mem_get_size
   (
      /* [IN] the address of the memory block whose size is wanted */
      pointer mem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   STOREBLOCK_STRUCT_PTR  block_ptr;
   _mem_size              size;
   
   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   if (mem_ptr == NULL) {
      _task_set_error(MQX_INVALID_POINTER);
      return(0);
   } /* Endif */
#endif
   
   /* Compute the start of the block  */
   block_ptr = GET_MEMBLOCK_PTR(mem_ptr);
   
#if MQX_CHECK_ERRORS
   if (! _MEMORY_ALIGNED(block_ptr)) {
      _task_set_error(MQX_INVALID_POINTER);
      return(0);
   } /* Endif */
#endif

   size = block_ptr->BLOCKSIZE;

#if MQX_CHECK_ERRORS
   /* For all free blocks, a check is made to ensure that the user
    * has not corrupted the storage pool. This is done by checking the
    * 'magic value', which should not be corrupted. Alternatively, the
    * user could have passed in an invalid memory pointer.
    */
   if ( BLOCK_IS_FREE(block_ptr) ) {
      kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR = block_ptr;
      _task_set_error(MQX_CORRUPT_STORAGE_POOL);
      return(0);
   } /* Endif */
   
#endif

#if MQX_CHECK_VALIDITY
   if ( (! VALID_CHECKSUM(block_ptr)) ) {
      kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR = block_ptr;
      _task_set_error(MQX_INVALID_CHECKSUM);
      return(0);
   } /* Endif */
#endif
   
   /* The size includes the block overhead, which the user is not
   ** interested in. If the size is less than the overhead,
   ** then there is a bad block or bad block pointer.
   */
#if MQX_CHECK_ERRORS
   if ( size <= (_mem_size)FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA) ) {
      kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR = block_ptr;
      _task_set_error(MQX_CORRUPT_STORAGE_POOL);
      return(0);
   } /* Endif */
#endif

   return ( size - (_mem_size)FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA) );

} /* Endbody */

#endif
/* EOF */
