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
* $FileName: mem_allp.c$
* $Version : 3.6.6.0$
* $Date    : Oct-27-2010$
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
* Function Name    : _mem_alloc_from
* Returned Value   : pointer NULL is returned upon error.
* Comments         : Allocates a block of memory from a specified
*   pool
*
*END*---------------------------------------------------------*/

pointer _mem_alloc_from
   (
      /* [IN] the pool to allocate from */
      _mem_pool_id  pool_id,

      /* [IN] the size of the memory block */
      _mem_size     size

   )
{
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   MEMPOOL_STRUCT_PTR      mem_pool_ptr;
   _mqx_uint                error;
   pointer                 result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_mem_alloc_from, size);

   mem_pool_ptr = (MEMPOOL_STRUCT_PTR)pool_id;

   _INT_DISABLE();

   result = _mem_alloc_internal(size, kernel_data->ACTIVE_PTR, mem_pool_ptr, &error);

#if MQX_CHECK_ERRORS
   if (error != MQX_OK) {
      _task_set_error(error);
   } /* Endif */
#endif

   /* update the memory allocation pointer in case a lower priority
   ** task was preempted inside _mem_alloc_internal
   */
   mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

   _INT_ENABLE();


   _KLOGX3(KLOG_mem_alloc_from, result, mem_pool_ptr->POOL_BLOCK_IN_ERROR);
   return(result);

}

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_alloc_align_from
* Returned Value   : pointer NULL is returned upon error.
* Comments         : Allocates a block of memory from a specified
*   pool
*
*END*---------------------------------------------------------*/

pointer _mem_alloc_align_from
   (
      /* [IN] the pool to allocate from */
      _mem_pool_id  pool_id,

      /* [IN] the size of the memory block */
      _mem_size size,
      
      /* [IN] the alignment of the memory block */
      _mem_size align
   )
{
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   MEMPOOL_STRUCT_PTR      mem_pool_ptr;
   _mqx_uint               error;
   pointer                 result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_mem_alloc_from, size);

   mem_pool_ptr = (MEMPOOL_STRUCT_PTR)pool_id;

   _INT_DISABLE();

   result = _mem_alloc_internal_align(size, align, kernel_data->ACTIVE_PTR, mem_pool_ptr, &error);

#if MQX_CHECK_ERRORS
   if (error != MQX_OK) {
      _task_set_error(error);
   } /* Endif */
#endif

   /* update the memory allocation pointer in case a lower priority
   ** task was preempted inside _mem_alloc_internal
   */
   mem_pool_ptr->POOL_ALLOC_CURRENT_BLOCK = mem_pool_ptr->POOL_FREE_LIST_PTR;

   _INT_ENABLE();


   _KLOGX3(KLOG_mem_alloc_from, result, mem_pool_ptr->POOL_BLOCK_IN_ERROR);
   return(result);

}

#endif

