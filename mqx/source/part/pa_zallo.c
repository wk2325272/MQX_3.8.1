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
* $FileName: pa_zallo.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for allocating a zero filled partition
*   block.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_alloc_zero
* Returned Value   : pointer
* Comments         :
*    Allocates zero filled block of memory
*
*END*--------------------------------------------------------*/

pointer _partition_alloc_zero
   (
      /* [IN] the partition from which to obtin the memory block */
      _partition_id partition
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   PARTPOOL_STRUCT_PTR    partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;
   pointer                result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_partition_alloc_zero, partition);

   result = _partition_alloc_internal(partpool_ptr, kernel_data->ACTIVE_PTR);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if ( result == NULL ) {
      _KLOGX2(KLOG_partition_alloc_zero, result);
      return (result);
   } /* Endif */
#endif

   _mem_zero(result, (_mem_size)(partpool_ptr->BLOCK_SIZE -
      sizeof(INTERNAL_PARTITION_BLOCK_STRUCT)));

   _KLOGX2(KLOG_partition_alloc_zero, result);
   return (result);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
