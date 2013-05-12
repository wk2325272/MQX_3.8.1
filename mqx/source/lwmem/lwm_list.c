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
* $FileName: lwm_list.c$
* $Version : 3.8.8.0$
* $Date    : Sep-26-2011$
*
* Comments:
*
*   This file finds the next owned block for a given task descriptor
*   from the kernel pool, it is only called if the LWMEM allocator is
*   being used as the default memory allocator.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_get_next_block_internal
* Returned Value   : pointer.
* Comments         :
*    Find the next block associated with the specified Task
*
*END*---------------------------------------------------------*/

pointer _lwmem_get_next_block_internal
   ( 
      /* [IN] the td whose blocks are being looked for */
      TD_STRUCT_PTR  td_ptr,

      /* [IN] the block last obtained */
      pointer        in_block_ptr
   )
{
    KERNEL_DATA_STRUCT_PTR kernel_data;
    LWMEM_POOL_STRUCT_PTR lwmem_pool_ptr;
    LWMEM_BLOCK_STRUCT_PTR block_ptr = in_block_ptr;
    LWMEM_BLOCK_STRUCT_PTR free_ptr;
    
    _GET_KERNEL_DATA(kernel_data);
    
    if (block_ptr == NULL) {
        // first item, start on first item in first pool
        lwmem_pool_ptr = kernel_data->KERNEL_LWMEM_POOL;
        block_ptr = lwmem_pool_ptr->POOL_ALLOC_START_PTR;
    } else {
        // continued, get lwmem pool from provided in_block_ptr
        block_ptr = GET_LWMEMBLOCK_PTR(in_block_ptr);
        lwmem_pool_ptr = block_ptr->POOL;
        
        block_ptr = (LWMEM_BLOCK_STRUCT_PTR)((uchar_ptr)block_ptr + block_ptr->BLOCKSIZE);
    }
    
    _int_disable();

    do {
        free_ptr = lwmem_pool_ptr->POOL_FREE_LIST_PTR;
        
        while ((uchar_ptr)block_ptr < (uchar_ptr)lwmem_pool_ptr->POOL_ALLOC_END_PTR){
            if (block_ptr->U.S.TASK_NUMBER == TASK_NUMBER_FROM_TASKID(td_ptr->TASK_ID)) {
                /* check for block is not free block */
                while (free_ptr && free_ptr < block_ptr) {
                    free_ptr = free_ptr->U.NEXTBLOCK;
                }
                
                if (free_ptr != block_ptr) {
                    /* This block is owned by the target task and it's not free block*/
                    _int_enable();
                    
                    return((pointer)((uchar_ptr)block_ptr + sizeof(LWMEM_BLOCK_STRUCT)));
                }
            }
            block_ptr = (LWMEM_BLOCK_STRUCT_PTR)((uchar_ptr)block_ptr + block_ptr->BLOCKSIZE);
        }
        
        // continue in next lwmem pool
        lwmem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)(lwmem_pool_ptr->LINK.NEXT);
        block_ptr = lwmem_pool_ptr->POOL_ALLOC_START_PTR;
    } while (lwmem_pool_ptr != kernel_data->KERNEL_LWMEM_POOL); // repeat until processed lwmem pool is not first pool (pool list is circular list)
    
    _int_enable();

    return(NULL);  
}

#endif /* MQX_USE_LWMEM */

/* EOF */
