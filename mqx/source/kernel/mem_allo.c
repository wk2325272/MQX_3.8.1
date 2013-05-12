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
* $FileName: mem_allo.c$
* $Version : 3.8.9.1$
* $Date    : Jun-6-2012$
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
* Function Name    : _mem_alloc
* Returned Value   : pointer. NULL is returned upon error.
* Comments         : allocates a block of memory
*
*END*---------------------------------------------------------*/

pointer _mem_alloc
   (
      /* [IN] the size of the memory block */
      _mem_size size
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   _mqx_uint               error;
   pointer                 result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_mem_alloc, size);

   _INT_DISABLE();
   result = _mem_alloc_internal(size, kernel_data->ACTIVE_PTR,
      (MEMPOOL_STRUCT_PTR)&kernel_data->KD_POOL, &error);

   /* update the memory allocation pointer in case a lower priority
   ** task was preempted inside _mem_alloc_internal
   */
   kernel_data->KD_POOL.POOL_ALLOC_CURRENT_BLOCK = 
      kernel_data->KD_POOL.POOL_FREE_LIST_PTR;

   _INT_ENABLE();
   
#if MQX_CHECK_ERRORS
   if (error != MQX_OK) {
      _task_set_error(error);
   } /* Endif */
#endif

   _KLOGX3(KLOG_mem_alloc, result, kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR);
   return(result);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_align
* Returned Value   : pointer. NULL is returned upon error.
* Comments         : allocates an aligned block of memory
*
*END*---------------------------------------------------------*/
pointer _mem_alloc_align
   (
      /* [IN] the size of the memory block */
      _mem_size size,
      /* [IN] the alignment of the memory block */
     _mem_size align
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   _mqx_uint               error;
   pointer                 result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_mem_alloc, size);

   _INT_DISABLE();
   result = _mem_alloc_internal_align(size, align, kernel_data->ACTIVE_PTR, (MEMPOOL_STRUCT_PTR)&kernel_data->KD_POOL, &error);

   /* update the memory allocation pointer in case a lower priority
   ** task was preempted inside _mem_alloc_internal
   */
   kernel_data->KD_POOL.POOL_ALLOC_CURRENT_BLOCK = kernel_data->KD_POOL.POOL_FREE_LIST_PTR;

   _INT_ENABLE();
   
#if MQX_CHECK_ERRORS
   if (error != MQX_OK) {
      _task_set_error(error);
   } /* Endif */
#endif

   _KLOGX3(KLOG_mem_alloc_align, result, kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR);
   return(result);
}

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_at
* Returned Value   : pointer. NULL is returned upon error.
* Comments         : allocates a block of memory starting 
*                    at the requested address
*
*END*---------------------------------------------------------*/
pointer _mem_alloc_at(
      /* [IN] the size of the memory block */
      _mem_size size,
      /* [IN] the addr of the memory block */
      pointer addr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   _mqx_uint               error;
   pointer                 result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_mem_alloc, size);

   _INT_DISABLE();
   result = _mem_alloc_at_internal(size, addr, kernel_data->ACTIVE_PTR, (MEMPOOL_STRUCT_PTR)&kernel_data->KD_POOL, &error);

   /* update the memory allocation pointer in case a lower priority
   ** task was preempted inside _mem_alloc_internal
   */
   kernel_data->KD_POOL.POOL_ALLOC_CURRENT_BLOCK = kernel_data->KD_POOL.POOL_FREE_LIST_PTR;

   _INT_ENABLE();
   
#if MQX_CHECK_ERRORS
   if (error != MQX_OK) {
      _task_set_error(error);
   }
#endif

   _KLOGX3(KLOG_mem_alloc_at, result, kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR);
   return(result);
}

#if MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_uncached
* Returned Value   : pointer. NULL is returned upon error.
* Comments         : allocates a block of memory
*
*END*---------------------------------------------------------*/
pointer _mem_alloc_uncached
    (
        /* [IN] the size of the memory block */
        _mem_size size
    )
{
    KERNEL_DATA_STRUCT_PTR  kernel_data;
    pointer                 result;

    _GET_KERNEL_DATA(kernel_data);
    //_KLOGE2(KLOG_mem_alloc_uncached, req_size);

    result  = _mem_alloc_from(kernel_data->UNCACHED_POOL, size);

   //_KLOGX3(KLOG_mem_alloc_uncached, result, kernel_data->UNCACHED_POOL->POOL_BLOCK_IN_ERROR);
    
    return result;
}

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_align_uncached
* Returned Value   : pointer. NULL is returned upon error.
* Comments         : allocates an aligned block of memory
*
*END*---------------------------------------------------------*/
pointer _mem_alloc_align_uncached
    (
        /* [IN] the size of the memory block */
        _mem_size size,
        /* [IN] alignment of the memory block */
        _mem_size align
    )
{
    KERNEL_DATA_STRUCT_PTR  kernel_data;
    pointer                 result;

    _GET_KERNEL_DATA(kernel_data);
    //_KLOGE2(KLOG_mem_alloc_uncached, size);

    result  = _mem_alloc_align_from(kernel_data->UNCACHED_POOL, size, align);

   //_KLOGX3(KLOG_mem_alloc_uncached, result, kernel_data->UNCACHED_POOL->POOL_BLOCK_IN_ERROR);
    
    return result;
}

#endif // MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE

#endif
