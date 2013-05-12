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
* $FileName: lwm_size.c$
* $Version : 3.0.8.0$
* $Date    : Jul-1-2009$
*
* Comments:
*
*   This file contains the function that returns the size of a memory block.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_get_size
* Returned Value   : _mem_size
* Comments         :
*    This routine returns the allocated size (in bytes) of a block
*    allocated using the MQX storage allocator (_lwmem_alloc).
*
*END*--------------------------------------------------------*/

_mem_size _lwmem_get_size
   (
      /* [IN] the address of the memory block whose size is wanted */
      pointer mem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   LWMEM_BLOCK_STRUCT_PTR block_ptr;
   
   _GET_KERNEL_DATA(kernel_data);
#if MQX_CHECK_ERRORS
   if (mem_ptr == NULL) {
      _task_set_error(MQX_INVALID_POINTER);
      return(0);
   } /* Endif */
#endif
   
   /* Compute the start of the block  */
   block_ptr = GET_LWMEMBLOCK_PTR(mem_ptr);
   /* The size includes the block overhead, which the user is not
   ** interested in. If the size is less than the overhead,
   ** then there is a bad block or bad block pointer.
   */
#if MQX_CHECK_ERRORS
   if ( block_ptr->BLOCKSIZE <= (_mem_size)sizeof(LWMEM_BLOCK_STRUCT) ) {
      _task_set_error(MQX_INVALID_POINTER);
      return(0);
   } /* Endif */
#endif

   return ( block_ptr->BLOCKSIZE - (_mem_size)sizeof(LWMEM_BLOCK_STRUCT) );

} /* Endbody */


// Move to standalone file

#if MQX_ALLOW_TYPED_MEMORY
/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_get_type
* Returned Value   : true on success
* Comments         :
*   This function gets the type of the specified block
*
*END*--------------------------------------------------------*/

_mem_type _lwmem_get_type   
   (
      /* [IN] the address of the memory block to type */
      pointer mem_ptr
   )
{
   LWMEM_BLOCK_STRUCT_PTR block_ptr;

   block_ptr = GET_LWMEMBLOCK_PTR(mem_ptr);
   return block_ptr->U.S.MEM_TYPE;
}


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_set_type
* Returned Value   : true on success
* Comments         :
*   This function sets the type of the specified block
*
*END*--------------------------------------------------------*/

boolean _lwmem_set_type   
   (
      /* [IN] the address of the memory block to type */
      pointer mem_ptr,
      _mem_type mem_type
   )
{
   LWMEM_BLOCK_STRUCT_PTR block_ptr;
   
   if (mem_ptr != NULL) {
      block_ptr = GET_LWMEMBLOCK_PTR(mem_ptr);
      block_ptr->U.S.MEM_TYPE = mem_type;
      return TRUE;
   } else {
      return FALSE;
   }
}
#endif

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_get_highwater
* Returned Value   : true on success
* Comments         :
*   This function sets the type of the specified block
*
*END*--------------------------------------------------------*/

pointer _lwmem_get_highwater(void) 
{
   KERNEL_DATA_STRUCT_PTR  kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   return(((LWMEM_POOL_STRUCT_PTR)(kernel_data->KERNEL_LWMEM_POOL))->HIGHWATER);
  
}

#endif /* MQX_USE_LWMEM */
/* EOF */
