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
* $FileName: mem_util.c$
* $Version : 3.0.8.0$
* $Date    : Mar-11-2009$
*
* Comments:
*
*   This file contains functions that return information about the
*   memory pool.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_highwater
* Returned Value   : pointer
* Comments         :
*   This function returns the highest memory address ever used in the
*   kernel memory area.
*
*END*--------------------------------------------------------*/

pointer _mem_get_highwater
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   return (kernel_data->KD_POOL.POOL_HIGHEST_MEMORY_USED);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_highwater_pool
* Returned Value   : pointer
* Comments         :
*   This function returns the highest memory address ever used in the
*   specified memory pool.
*
*END*--------------------------------------------------------*/

pointer _mem_get_highwater_pool
   (
      _mem_pool_id pool_id
   )
{ /* Body */
   MEMPOOL_STRUCT_PTR     mem_pool_ptr = (MEMPOOL_STRUCT_PTR)pool_id;

   return (mem_pool_ptr->POOL_HIGHEST_MEMORY_USED);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_error
* Returned Value   : pointer
* Comments         :
*   This function returns the last memory block which 
*   caused a corrupt memory pool error in kernel data.
*
*END*--------------------------------------------------------*/

pointer _mem_get_error
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;
   register pointer                user_ptr;

   _GET_KERNEL_DATA(kernel_data);

   user_ptr = (pointer)((uchar_ptr)kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR +
      FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA));
   return (user_ptr);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_error_pool
* Returned Value   : pointer
* Comments         :
*   This function returns the last memory block which 
*   caused a corrupt memory pool error in the specified pool.
*
*END*--------------------------------------------------------*/

pointer _mem_get_error_pool
   (
      _mem_pool_id pool_id
   )
{ /* Body */
   MEMPOOL_STRUCT_PTR  mem_pool_ptr = (MEMPOOL_STRUCT_PTR)pool_id;
   register pointer    user_ptr;

   user_ptr = (pointer)((uchar_ptr)mem_pool_ptr->POOL_BLOCK_IN_ERROR +
      FIELD_OFFSET(STOREBLOCK_STRUCT,USER_AREA));
   return (user_ptr);

} /* Endbody */



/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_system_pool_id
* Returned Value   : _mem_pool_id
* Comments         :
*   This function returns the pool id of the system pool.
*
*END*--------------------------------------------------------*/

_mem_pool_id _mem_get_system_pool_id(void)
{
   register KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   return (_mem_pool_id) &kernel_data->KD_POOL;
} 

#if MQX_ALLOW_TYPED_MEMORY
/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_get_type
* Returned Value   : true on success
* Comments         :
*   This function gets the type of the specified block
*
*END*--------------------------------------------------------*/

_mem_type _mem_get_type   
   (
      /* [IN] the address of the memory block to type */
      pointer mem_ptr
   )
{
   STOREBLOCK_STRUCT_PTR   block_ptr;

   block_ptr = GET_MEMBLOCK_PTR(mem_ptr);
   return block_ptr->MEM_TYPE;
}


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_set_type
* Returned Value   : true on success
* Comments         :
*   This function sets the type of the specified block
*
*END*--------------------------------------------------------*/

boolean _mem_set_type   
   (
      /* [IN] the address of the memory block to type */
      pointer mem_ptr,
      _mem_type mem_type
   )
{
   STOREBLOCK_STRUCT_PTR   block_ptr;
   
   if (mem_ptr != NULL) {
      block_ptr = GET_MEMBLOCK_PTR(mem_ptr);
      _int_disable();
      block_ptr->MEM_TYPE= mem_type;
      CALC_CHECKSUM(block_ptr);
      _int_enable();
      return TRUE;
   } else {
      return FALSE;
   }
}

#endif

#endif
/* EOF */
