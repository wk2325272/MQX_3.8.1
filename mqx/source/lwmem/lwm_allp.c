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
* $FileName: lwm_allp.c$
* $Version : 3.8.10.0$
* $Date    : Aug-30-2011$
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
* Function Name    : _lwmem_alloc_from
* Returned Value   : pointer NULL is returned upon error.
* Comments         : Allocates a block of memory from a specified
*   pool
*
*END*---------------------------------------------------------*/

pointer _lwmem_alloc_from
   (
      /* [IN] the pool to allocate from */
      _lwmem_pool_id  pool_id,

      /* [IN] the size of the memory block */
      _mem_size       requested_size

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   pointer                 result;
   
#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        return _usr_lwmem_alloc_from(pool_id, requested_size);
    }
#endif

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE3(KLOG_lwmem_alloc_from, pool_id, requested_size);

   result = _lwmem_alloc_internal(requested_size, kernel_data->ACTIVE_PTR, 
      pool_id, FALSE);

   _KLOGX2(KLOG_lwmem_alloc_from, result);
   return(result);
}


#if MQX_ENABLE_USER_MODE

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _usr_lwmem_alloc_from
* Returned Value   : pointer NULL is returned upon error.
* Comments         : Allocates a block of memory from a specified
*   pool
*
*END*---------------------------------------------------------*/

pointer _usr_lwmem_alloc_from
   (
      /* [IN] the pool to allocate from */
      _lwmem_pool_id  pool_id,

      /* [IN] the size of the memory block */
      _mem_size       requested_size

   )
{
    MQX_API_CALL_PARAMS params = {(uint_32)pool_id, (uint_32)requested_size, 0, 0, 0};
    return (pointer)_mqx_api_call(MQX_API_LWMEM_ALLOC_FROM, &params);
}

#endif /* MQX_ENABLE_USER_MODE */

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_alloc_align_from
* Returned Value   : pointer NULL is returned upon error.
* Comments         : Allocates a block of memory from a specified
*   pool aligned to required value
*
*END*---------------------------------------------------------*/

pointer _lwmem_alloc_align_from
    (
        /* [IN] the pool to allocate from */
        _lwmem_pool_id  pool_id,
        /* [IN] the size of the memory block */
        _mem_size requested_size,
        /* [IN] align requested value */
        _mem_size req_align
    )
{
    KERNEL_DATA_STRUCT_PTR  kernel_data;
    pointer                 result;

    _GET_KERNEL_DATA(kernel_data);
    _KLOGE4(KLOG_lwmem_alloc_align_from, pool_id, requested_size, req_align);

    result = _lwmem_alloc_align_internal(requested_size, req_align, kernel_data->ACTIVE_PTR, pool_id, FALSE);

    _KLOGX2(KLOG_lwmem_alloc_align_from, result);
    
    return(result);
}

#endif /* MQX_USE_LWMEM */

/* EOF */
