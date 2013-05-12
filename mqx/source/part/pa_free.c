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
* $FileName: pa_free.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for freeing a partition block.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_free
* Returned Value   : MQX_OK or an error code
* Comments         :
*   This function frees the given block of memory.
*
*END*---------------------------------------------------------*/

_mqx_uint _partition_free
   (
      /* [IN] the address of the memory block to free */
      pointer mem_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR              kernel_data;
   PARTPOOL_STRUCT_PTR                 partpool_ptr;
   INTERNAL_PARTITION_BLOCK_STRUCT_PTR block_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_partition_free, mem_ptr);

   block_ptr = (INTERNAL_PARTITION_BLOCK_STRUCT_PTR)
      ((uchar_ptr)mem_ptr - sizeof(INTERNAL_PARTITION_BLOCK_STRUCT));

#if MQX_CHECK_VALIDITY
   if (! VALID_PARTITION_CHECKSUM(block_ptr)) {
      _KLOGX2(KLOG_partition_free, PARTITION_BLOCK_INVALID_CHECKSUM);
      return(PARTITION_BLOCK_INVALID_CHECKSUM);
   } /* Endif */
#endif

#if MQX_CHECK_ERRORS
   if (block_ptr->TASK_ID != SYSTEM_TD_PTR(kernel_data)->TASK_ID) {
      /* Let system blocks be freed by anyone */
      if (block_ptr->TASK_ID != kernel_data->ACTIVE_PTR->TASK_ID) {
         _KLOGX2(KLOG_partition_free, MQX_NOT_RESOURCE_OWNER);
         return(MQX_NOT_RESOURCE_OWNER);
      } /* Endif */
   } /* Endif */
#endif

   partpool_ptr = block_ptr->LINK.PARTITION_PTR;
   _INT_DISABLE();

#if MQX_CHECK_VALIDITY
   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _int_enable();
      _KLOGX2(KLOG_partition_free, PARTITION_INVALID);
      return(PARTITION_INVALID);
   } /* Endif */
#endif

   block_ptr->TASK_ID = 0;
   block_ptr->LINK.NEXT_BLOCK_PTR = partpool_ptr->FREE_LIST_PTR;
   partpool_ptr->FREE_LIST_PTR    = block_ptr;

   ++partpool_ptr->AVAILABLE;
   CALC_PARTITION_CHECKSUM(block_ptr);

   _INT_ENABLE();
   _KLOGX2(KLOG_partition_free, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
