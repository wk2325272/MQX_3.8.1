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
* $FileName: mem_xftd.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   Thie file contains the function for tranfering ownership of a memory
*   block between tasks.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
 * 
 * Function Name    : _mem_transfer_td_internal
 * Returned Value   : _mqx_uint MQX_OK, or a mqx error code.
 * Comments         :
 *   This routine transfers the ownership of a block of memory from
 *   an owner task to another task.
 *
 *END*--------------------------------------------------------*/

_mqx_uint _mem_transfer_td_internal
   (  
      /* [IN] the memory block whose ownership is to be transferred */
      pointer  memory_ptr,
      
      /* [IN] the source (owner) td */
      TD_STRUCT_PTR source_td,
      
      /* [IN] the target (new owner) td */
      TD_STRUCT_PTR target_td
   )
{ /* Body */
   STOREBLOCK_STRUCT_PTR  block_ptr;
   STOREBLOCK_STRUCT_PTR  prev_block_ptr;
   STOREBLOCK_STRUCT_PTR  next_block_ptr;

   block_ptr = GET_MEMBLOCK_PTR(memory_ptr);

   next_block_ptr = (STOREBLOCK_STRUCT_PTR)source_td->MEMORY_RESOURCE_LIST;
   /* START CR 1806 */  
   /* prev_block_ptr = GET_MEMBLOCK_PTR(&source_td->MEMORY_RESOURCE_LIST); */
   if (memory_ptr == next_block_ptr) {
      /*
      ** This is the last item on the resource list of the
      ** source task. Remove it from the resource list by
      ** pointing the resource list to the next item.
      */
      source_td->MEMORY_RESOURCE_LIST = block_ptr->NEXTBLOCK;
   } else { 
       /* Scan the task's memory resource list searching for the block.
        * Stop when the current pointer is equal to the block,
        * or the end of the list is reached.
        */
       while (  next_block_ptr  && ((pointer)next_block_ptr != memory_ptr) ) {
          prev_block_ptr = GET_MEMBLOCK_PTR(next_block_ptr);
          next_block_ptr = (STOREBLOCK_STRUCT_PTR)prev_block_ptr->NEXTBLOCK;
       } /* Endwhile */

       if ( ! next_block_ptr  ) {
          /* The specified block does not belong to the source task. */
          return(MQX_NOT_RESOURCE_OWNER);
       } /* Endif */

       /* Remove the memory block from the resource list of the
       ** source task.
       */
       prev_block_ptr->NEXTBLOCK = block_ptr->NEXTBLOCK;
    } /* Endif */
    /* END CR 1806 */
    _mem_transfer_internal(memory_ptr, target_td);

    return(MQX_OK);

 } /* Endbody */

#endif
/* EOF */
