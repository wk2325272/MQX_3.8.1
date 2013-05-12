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
* $FileName: pa_creaa.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for creating a partition at a location.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*--------------------------------------------------------------
* 
* Function Name    : _partition_create_at
* Returned Value   : _partition_id - ID of partition created if successful
*   or PARTITION_NULL_ID otherwise
* Comments         :
*   Create a partition at a location, with blocks of a certain size.
*
*END*-----------------------------------------------------------------*/

_partition_id _partition_create_at
   ( 
      /* [IN] where the partition is to start from. */
      pointer    partition_location,

      /* [IN] the size of the partition */
      _mem_size  partition_size,

      /* [IN] the size of each block in the partition in addressable uints */
      _mem_size  block_size
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;)
   PARTPOOL_STRUCT_PTR           partpool_ptr;
   uchar_ptr                     firstblock_ptr;
   _mqx_uint                     number_of_blocks;
   _mqx_uint                     result;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)

   _KLOGE4(KLOG_partition_create_at, partition_location, partition_size, block_size);

#if MQX_CHECK_ERRORS
   if (block_size == 0) {
      _task_set_error(MQX_INVALID_PARAMETER);
      _KLOGX2(KLOG_partition_create_at, PARTITION_NULL_ID);
      return(PARTITION_NULL_ID);
   } /* Endif */

   if (partition_size < sizeof(PARTPOOL_STRUCT)) {
      _task_set_error(MQX_INVALID_PARAMETER);
      _KLOGX2(KLOG_partition_create_at, PARTITION_NULL_ID);
      return(PARTITION_NULL_ID);
   } /* Endif */  
#endif

   block_size += (_mem_size)sizeof(INTERNAL_PARTITION_BLOCK_STRUCT);
   _MEMORY_ALIGN_VAL_LARGER(block_size);

   partpool_ptr = (PARTPOOL_STRUCT_PTR)
      _ALIGN_ADDR_TO_HIGHER_MEM(partition_location);
   
   firstblock_ptr  = (uchar_ptr)partpool_ptr + sizeof(PARTPOOL_STRUCT);
   firstblock_ptr  = (uchar_ptr)_ALIGN_ADDR_TO_HIGHER_MEM(firstblock_ptr);
   partition_size -= (_mem_size)firstblock_ptr - (_mem_size)partition_location;

   number_of_blocks = (_mqx_uint)(partition_size / block_size);

#if MQX_CHECK_ERRORS
   if (!number_of_blocks) {
      _KLOGX2(KLOG_partition_create_at, PARTITION_NULL_ID);
      return(PARTITION_NULL_ID);
   } /* Endif */
#endif

   result = _partition_create_internal(partpool_ptr, block_size, 
      number_of_blocks);
#if MQX_CHECK_ERRORS
   if (result != MQX_OK) {
      _KLOGX2(KLOG_partition_create_at, result);
      return(PARTITION_NULL_ID);
   } /* Endif */
#endif

   partpool_ptr->PARTITION_TYPE = PARTITION_STATIC;

   _KLOGX5(KLOG_partition_create_at, partpool_ptr, partition_size, block_size, MQX_OK);
   return((_partition_id)partpool_ptr);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
