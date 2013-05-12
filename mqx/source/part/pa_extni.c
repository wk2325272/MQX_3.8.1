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
* $FileName: pa_extni.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for adding another memory area to an
*   existing partition.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_extend_internal
* Returned Value   : void
* Comments         :
*    adds more blocks of memory into the partition
*
*END*---------------------------------------------------------*/

void _partition_extend_internal
   (
      /* [IN] the partition to add to */
      PARTPOOL_STRUCT_PTR        partpool_ptr,

      /* [IN] the new block to add */
      PARTPOOL_BLOCK_STRUCT_PTR  partpool_block_ptr,
      
      /* [IN] the number of blocks to add */
      _mqx_uint                  number_of_blocks
   )
{ /* Body */
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR block_ptr;

/* START CR 308 */
   partpool_block_ptr->VALID            = PARTITION_VALID;
/* END CR 308 */
   partpool_block_ptr->NUMBER_OF_BLOCKS = number_of_blocks;

   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)(
      (uchar_ptr)partpool_block_ptr + sizeof(PARTPOOL_BLOCK_STRUCT));
   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
      _ALIGN_ADDR_TO_HIGHER_MEM(block_ptr);

   partpool_block_ptr->FIRST_IBLOCK_PTR = block_ptr;
   
   while (number_of_blocks--) {
      _int_disable();
      block_ptr->LINK.NEXT_BLOCK_PTR = partpool_ptr->FREE_LIST_PTR;
      CALC_PARTITION_CHECKSUM(block_ptr);
      partpool_ptr->FREE_LIST_PTR    = block_ptr;
      partpool_ptr->AVAILABLE++;
      _int_enable();
      block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
         ((uchar_ptr)block_ptr + partpool_ptr->BLOCK_SIZE);
   } /* Endwhile */
   
   _int_disable();
   partpool_block_ptr->NEXT_POOL_PTR = partpool_ptr->POOL.NEXT_POOL_PTR;
   partpool_ptr->POOL.NEXT_POOL_PTR  = partpool_block_ptr;
   _int_enable();

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
