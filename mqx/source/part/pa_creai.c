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
* $FileName: pa_creai.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a partition.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _partition_create_internal
* Returned Value   : _mqx_uint - MQX_OK or error
* Comments         :
*    Create a partition at a location, 
* with a specified number of blocks.
*
*END*-----------------------------------------------------------------*/

_mqx_uint _partition_create_internal
   ( 
      /* [IN] the start of the partition */
      PARTPOOL_STRUCT_PTR partpool_ptr,
      
      /* [IN] the total size of each block with overheads */
      _mem_size           actual_size,

      /* [IN] the initial number of blocks in the partition */
      _mqx_uint           initial_blocks

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR              kernel_data;
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR block_ptr;
   PARTITION_COMPONENT_STRUCT_PTR      part_component_ptr;
   _mqx_uint                           result;

   _GET_KERNEL_DATA(kernel_data);

   part_component_ptr = (PARTITION_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS];
   if (part_component_ptr == NULL) {
      result = _partition_create_component();
      part_component_ptr = (PARTITION_COMPONENT_STRUCT_PTR)
         kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS];
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
      if (part_component_ptr == NULL){
         return(result);
      } /* Endif */
#endif
   } /* Endif */

   _mem_zero(partpool_ptr, (_mem_size)sizeof(PARTPOOL_STRUCT));

   partpool_ptr->BLOCK_SIZE            = actual_size;
/* START CR 308 */
   partpool_ptr->POOL.VALID            = PARTITION_VALID;
/* END CR 308 */
   partpool_ptr->POOL.NUMBER_OF_BLOCKS = initial_blocks;
   partpool_ptr->AVAILABLE             = initial_blocks;
   partpool_ptr->TOTAL_BLOCKS          = initial_blocks;

   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)((uchar_ptr)partpool_ptr + 
      sizeof(PARTPOOL_STRUCT));
   
   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
      _ALIGN_ADDR_TO_HIGHER_MEM(block_ptr);

   partpool_ptr->POOL.FIRST_IBLOCK_PTR = block_ptr;
   
   while (initial_blocks--)  {
      block_ptr->LINK.NEXT_BLOCK_PTR = partpool_ptr->FREE_LIST_PTR;
      CALC_PARTITION_CHECKSUM(block_ptr);
      partpool_ptr->FREE_LIST_PTR  = block_ptr;
      block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)((uchar_ptr)block_ptr + 
         actual_size);
   } /* Endwhile */
   
   _int_disable();
   _QUEUE_ENQUEUE(&part_component_ptr->PARTITIONS, partpool_ptr);
   _int_enable();

   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
