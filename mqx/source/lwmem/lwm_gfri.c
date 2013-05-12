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
* $FileName: lwm_gfri.c$
* $Version : 3.5.1.0$
* $Date    : Jan-13-2010$
*
* Comments:
*
*   This file contains the function that gets unallocated size of the pool.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_get_free_internal
* Returned Value   : size of free memory
* Comments         :
*       Returns summarized size of free memory blocks in the pool
*
*END*---------------------------------------------------------*/

_mem_size _lwmem_get_free_internal
   ( 
      /* [IN] which pool to get free size from */
      _lwmem_pool_id pool_id
   )
{ /* Body */
   LWMEM_BLOCK_STRUCT_PTR  block_ptr;
   _mem_size               total_size;
   LWMEM_POOL_STRUCT_PTR   mem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)pool_id;
      
#if MQX_CHECK_VALIDITY
   if (mem_pool_ptr->VALID != LWMEM_POOL_VALID) {
      _task_set_error(MQX_LWMEM_POOL_INVALID);
      return(0);
   } /* Endif */
#endif

   _int_disable();
   block_ptr = mem_pool_ptr->POOL_FREE_LIST_PTR;
   while ( block_ptr != NULL ) {
      /* Provide window for higher priority tasks */
      mem_pool_ptr->POOL_ALLOC_PTR = block_ptr;
      _int_enable();
      _int_disable();
      block_ptr = mem_pool_ptr->POOL_ALLOC_PTR;
      if (block_ptr == mem_pool_ptr->POOL_FREE_LIST_PTR) {
         total_size = 0; /* some task with higher priority did reset our loop pointer */
      } /* Endif */
      total_size += block_ptr->BLOCKSIZE;
      block_ptr = block_ptr->U.NEXTBLOCK;
   } /* Endwhile */

   _int_enable();
   return(total_size);
} /* Endbody */

#endif /* MQX_USE_LWMEM */

/* EOF */
