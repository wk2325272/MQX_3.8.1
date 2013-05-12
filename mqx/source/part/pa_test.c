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
* $FileName: pa_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for checking the validity of 
*   all partitions in the system.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_test
* Returned Value   : error_code
*    MQX_OK or an error code
* Comments         :
*    checks all partition block checksums for validity
*
*END*---------------------------------------------------------*/

_mqx_uint _partition_test
   ( 
      /* [OUT] if an error occured, the partition pool in error */
      _partition_id _PTR_ partpool_in_error,

      /* [OUT] if an error occured, the partition pool block in error */
      pointer       _PTR_ partpool_block_in_error,

      /* [OUT] if an error occured, the partition block in error */
      pointer       _PTR_ block_in_error
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR               kernel_data;
   PARTPOOL_STRUCT_PTR                  partpool_ptr;
   PARTPOOL_BLOCK_STRUCT_PTR            partpool_block_ptr;
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR  block_ptr;
   PARTITION_COMPONENT_STRUCT_PTR       part_component_ptr;
   _mqx_uint                            i;
   _mqx_uint                            result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE1(KLOG_partition_test);

   part_component_ptr = (PARTITION_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS];
   if (part_component_ptr == NULL) {
      _KLOGX2(KLOG_partition_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

#if MQX_CHECK_VALIDITY
   if (part_component_ptr->VALID != PARTITION_VALID) {
      _KLOGX2(KLOG_partition_test, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */
#endif

   _int_disable();

   /* Make sure that the queue of partitions is ok */
   result = _queue_test(&part_component_ptr->PARTITIONS, partpool_in_error);
   _int_enable();
   if (result != MQX_OK) {
      _KLOGX3(KLOG_partition_test, result, *partpool_in_error);
      return(result);
   } /* Endif */

   partpool_ptr = (PARTPOOL_STRUCT_PTR)
      ((pointer)part_component_ptr->PARTITIONS.NEXT);
   while (partpool_ptr != (PARTPOOL_STRUCT_PTR)
      ((pointer)&part_component_ptr->PARTITIONS))
   {
      /* Check all partitions */
      *partpool_in_error = partpool_ptr;

      partpool_block_ptr = &partpool_ptr->POOL;
      while (partpool_block_ptr != NULL  ) {
         /* Check all partition pool blocks contiguously */
         *partpool_block_in_error = partpool_block_ptr;

         block_ptr = partpool_block_ptr->FIRST_IBLOCK_PTR;
         i = partpool_block_ptr->NUMBER_OF_BLOCKS + 1;
         while (--i) {
#if MQX_CHECK_VALIDITY
            if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
               *block_in_error = NULL;
               _KLOGX2(KLOG_partition_test, PARTITION_INVALID);
               return(PARTITION_INVALID);
            } /* Endif */
#endif
            _int_disable();
            if (!VALID_PARTITION_CHECKSUM(block_ptr)) {
               _int_enable();
               *block_in_error = (pointer)((uchar_ptr)block_ptr + 
                  sizeof(INTERNAL_PARTITION_BLOCK_STRUCT));
               _KLOGX2(KLOG_partition_test, PARTITION_BLOCK_INVALID_CHECKSUM);
               return(PARTITION_BLOCK_INVALID_CHECKSUM);
            } /* Endif */
           _int_enable();
            block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
               ((uchar_ptr)block_ptr + partpool_ptr->BLOCK_SIZE);
         } /* Endwhile */

         partpool_block_ptr = partpool_block_ptr->NEXT_POOL_PTR;
      } /* Endwhile */

      /* Check partition free list */
      _int_disable();
      block_ptr = partpool_ptr->FREE_LIST_PTR;
      while (block_ptr != NULL) {
         partpool_ptr->TEST_FREE_PTR = block_ptr;
         _int_enable();
         _int_disable();
         block_ptr = 
            (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)partpool_ptr->TEST_FREE_PTR;
         if (!VALID_PARTITION_CHECKSUM(block_ptr)) {
            _int_enable();
            *block_in_error = (pointer)((uchar_ptr)block_ptr + 
               sizeof(INTERNAL_PARTITION_BLOCK_STRUCT));
            _KLOGX2(KLOG_partition_test, PARTITION_BLOCK_INVALID_CHECKSUM);
            return(PARTITION_BLOCK_INVALID_CHECKSUM);
         } /* Endif */
         block_ptr = block_ptr->LINK.NEXT_BLOCK_PTR;
      } /* Endwhile */

      partpool_ptr = (PARTPOOL_STRUCT_PTR)partpool_ptr->NEXT;
      _int_enable();
   } /* Endwhile */

   _KLOGX2(KLOG_partition_test, MQX_OK);
   return MQX_OK;

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
