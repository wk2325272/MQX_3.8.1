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
* $FileName: pa_tdel.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for cleaning up partitions after a
*   task has been deleted.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_cleanup
* Returned Value   : none
* Comments         :
*    returns all partition blocks owned by the task to the pool
*
*END*---------------------------------------------------------*/

void _partition_cleanup
   (
      /* [IN] the task being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR              kernel_data;
   PARTPOOL_STRUCT_PTR                 partpool_ptr;
   PARTPOOL_BLOCK_STRUCT_PTR           partpool_block_ptr;
   PARTITION_COMPONENT_STRUCT_PTR      part_component_ptr;
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR block_ptr;
   _mqx_uint                           i;

   _GET_KERNEL_DATA(kernel_data);

   part_component_ptr = (PARTITION_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS];

   if (part_component_ptr == NULL) {
      return; /* No work to do! */
   } /* Endif */

#if MQX_CHECK_VALIDITY
   if (part_component_ptr->VALID != PARTITION_VALID) {
      return;
   } /* Endif */
#endif         

   partpool_ptr = (PARTPOOL_STRUCT_PTR)
      ((pointer)part_component_ptr->PARTITIONS.NEXT);
   while (partpool_ptr != (PARTPOOL_STRUCT_PTR)
      ((pointer)&part_component_ptr->PARTITIONS))
   {
      /* Check each partition */
      partpool_block_ptr = &partpool_ptr->POOL;
      while (partpool_block_ptr != NULL) {
         /* Check each poolblock in the partition */
#if MQX_CHECK_VALIDITY
         if (partpool_block_ptr->VALID != PARTITION_VALID) {
            break;
         }/* Endif */
#endif         
         block_ptr = partpool_block_ptr->FIRST_IBLOCK_PTR;
         i = partpool_block_ptr->NUMBER_OF_BLOCKS + 1;
         while (--i) {
            if ((block_ptr->TASK_ID == td_ptr->TASK_ID) &&
               (block_ptr->LINK.PARTITION_PTR == partpool_ptr))
            { /* An allocated Block */
               _int_disable();
               block_ptr->LINK.NEXT_BLOCK_PTR = partpool_ptr->FREE_LIST_PTR;
               partpool_ptr->FREE_LIST_PTR = block_ptr;
               ++partpool_ptr->AVAILABLE;
               CALC_PARTITION_CHECKSUM(block_ptr);
               _int_enable();
            } /* Endif */
            block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
               ((uchar_ptr)block_ptr + partpool_ptr->BLOCK_SIZE);
         } /* Endwhile */
         partpool_block_ptr = partpool_block_ptr->NEXT_POOL_PTR;
      } /* Endwhile */

      partpool_ptr = (PARTPOOL_STRUCT_PTR)partpool_ptr->NEXT;
   } /* Endwhile */

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
