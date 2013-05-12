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
* $FileName: lwm_crep.c$
* $Version : 3.8.14.0$
* $Date    : Sep-26-2011$
*
* Comments:
*
*   This file contains the function that initializes a memory pool.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_create_pool
* Returned Value   : A handle to the memory pool or 
*                    NULL_MEM_POOL_ID on error.
* Comments         :
*   This function initializes a memory storage pool. Will set task error
* code if error occurs
* 
*END*---------------------------------------------------------*/

_lwmem_pool_id _lwmem_create_pool
   (
      /* [IN] the location of the light weight memory pool definition */
      LWMEM_POOL_STRUCT_PTR mem_pool_ptr,

      /* [IN] the start of the memory pool */
      pointer               start,

      /* [IN] the size of the memory pool */
      _mem_size             size
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   LWMEM_BLOCK_STRUCT_PTR block_ptr;
   uchar_ptr              end;
   
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwmem_create_pool(mem_pool_ptr, start, size);
    }
#endif

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_lwmem_create_pool, start, size);

   /* Set the end of memory (aligned) */
   end = (uchar_ptr)start + size;
   mem_pool_ptr->POOL_ALLOC_END_PTR = (pointer)_ALIGN_ADDR_TO_LOWER_MEM(end);

   /* Align the start of the pool */
   block_ptr = (LWMEM_BLOCK_STRUCT_PTR)_ALIGN_ADDR_TO_HIGHER_MEM(start);
   mem_pool_ptr->POOL_ALLOC_START_PTR = (pointer)block_ptr;
   mem_pool_ptr->HIGHWATER = (pointer)block_ptr;

   /* Set up the first block as an idle block */
   block_ptr->BLOCKSIZE   = (uchar_ptr)mem_pool_ptr->POOL_ALLOC_END_PTR -
      (uchar_ptr)block_ptr;
   block_ptr->U.NEXTBLOCK = NULL;
   block_ptr->POOL        = mem_pool_ptr;
   mem_pool_ptr->POOL_FREE_LIST_PTR = block_ptr;
   mem_pool_ptr->POOL_ALLOC_PTR     = block_ptr;
   mem_pool_ptr->POOL_FREE_PTR      = block_ptr;
   mem_pool_ptr->POOL_TEST_PTR      = block_ptr;
   
   /* Protect the list of pools while adding new pool */
   _int_disable();
   if (kernel_data->LWMEM_POOLS.NEXT == NULL) {
      /* Initialize the light weight memory */
      _QUEUE_INIT(&kernel_data->LWMEM_POOLS, 0);
   } /* Endif */
   _QUEUE_ENQUEUE(&kernel_data->LWMEM_POOLS, &mem_pool_ptr->LINK);
   _int_enable();
   mem_pool_ptr->VALID = LWMEM_POOL_VALID;
   
   _KLOGX2(KLOG_lwmem_create_pool, mem_pool_ptr);
   return ((_lwmem_pool_id)mem_pool_ptr);

}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_set_pool_access
* Returned Value   : 
* Comments         :
* 
*END*---------------------------------------------------------*/

_mqx_uint _mem_set_pool_access
    (
        /* [IN] memory pool id */
        _lwmem_pool_id mem_pool_id,

        /* [IN] memory pool access rights */
        uint_32 access
    )
{
    LWMEM_POOL_STRUCT_PTR mem_pool_ptr = (_lwmem_pool_id)mem_pool_id;
    
    if (LWMEM_POOL_VALID == mem_pool_ptr->VALID) {
        _psp_mpu_add_region(mem_pool_ptr->POOL_ALLOC_START_PTR, mem_pool_ptr->POOL_ALLOC_END_PTR, access);
    }
    
    return MQX_OK;
}
                          
/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _usr_lwmem_create_pool
* Returned Value   : A handle to the memory pool or 
*                    NULL_MEM_POOL_ID on error.
* Comments         :
*   This function initializes a memory storage pool. Will set task error
* code if error occurs
* 
*END*---------------------------------------------------------*/

_lwmem_pool_id _usr_lwmem_create_pool
    (
        /* [IN] the location of the light weight memory pool definition */
        LWMEM_POOL_STRUCT_PTR mem_pool_ptr,

        /* [IN] the start of the memory pool */
        pointer               start,

        /* [IN] the size of the memory pool */
        _mem_size             size
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)mem_pool_ptr, (uint_32)start, (uint_32)size, 0, 0};
    return (_lwmem_pool_id)_mqx_api_call(MQX_API_LWMEM_CREATE_POOL, &params);
}

#endif /* MQX_ENABLE_USER_MODE */

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_create_pool_mapped
* Returned Value   : A handle to the memory pool or 
*                    NULL_MEM_POOL_ID on error.
* Comments         :
*   This function initializes a memory storage pool. Will set task error
* code if error occurs
* 
*END*---------------------------------------------------------*/
_lwmem_pool_id _lwmem_create_pool_mapped
   (
      /* [IN] the start of the memory pool */
      pointer    start,

      /* [IN] the size of the memory pool */
      _mem_size  size
   )
{ /* Body */
   LWMEM_POOL_STRUCT_PTR          mem_pool_ptr;

   mem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)_ALIGN_ADDR_TO_HIGHER_MEM(start);
   _mem_zero((pointer)mem_pool_ptr, (_mem_size)sizeof(LWMEM_POOL_STRUCT));

   start = (pointer)((uchar_ptr)mem_pool_ptr + sizeof(LWMEM_POOL_STRUCT));
   _lwmem_create_pool(mem_pool_ptr, start, size - sizeof(LWMEM_POOL_STRUCT));   // real pool size is decreased by pool header struct (must be - memory overwrite)

   return ((_mem_pool_id)mem_pool_ptr);

} /* Endbody */


#endif /* MQX_USE_LWMEM */

/* EOF */
