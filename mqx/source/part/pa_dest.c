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
* $FileName: pa_dest.c$
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
* Function Name    : _partition_destroy
* Returned Value   : MQX_OK or an error code
* Comments         :
*    Destroys a partition allocated in kernel memory
*
*END*-----------------------------------------------------------------*/

_mqx_uint _partition_destroy
   ( 
      /* [IN] the partition to destory */
      _partition_id partition
   )
{ /* Body */
   /* Start CR 532 */
   /* _KLOGM(KERNEL_DATA_STRUCT_PTR kernel_data;) */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   /* End   CR 532 */
   PARTPOOL_STRUCT_PTR           partpool_ptr;
   PARTPOOL_BLOCK_STRUCT_PTR     current_ptr;
   PARTPOOL_BLOCK_STRUCT_PTR     next_ptr;
   _mqx_uint                     result;
   /* Start CR 532 */
   PARTITION_COMPONENT_STRUCT_PTR part_component_ptr;

   /* _KLOGM(_GET_KERNEL_DATA(kernel_data);) */
   _GET_KERNEL_DATA(kernel_data);
   /* End   CR 532 */

   _KLOGE2(KLOG_partition_destroy, partition);

   /* Start CR 532 */
   part_component_ptr = (PARTITION_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_PARTITIONS];
   /* End   CR 532 */

   partpool_ptr = (PARTPOOL_STRUCT_PTR)partition;
   
#if MQX_CHECK_ERRORS
   /* Start CR 532 */
   if (part_component_ptr == NULL){
      _KLOGX2(KLOG_partition_destroy, MQX_COMPONENT_DOES_NOT_EXIST);
      return MQX_COMPONENT_DOES_NOT_EXIST;
   } /* Endif */
   /* End   CR 532 */

   if (partpool_ptr->POOL.VALID != PARTITION_VALID) {
      _KLOGX2(KLOG_partition_destroy, MQX_INVALID_PARAMETER);
      return MQX_INVALID_PARAMETER;
   } /* Endif */

   if (partpool_ptr->PARTITION_TYPE != PARTITION_DYNAMIC) {
      _KLOGX2(KLOG_partition_destroy, PARTITION_INVALID_TYPE);
      return PARTITION_INVALID_TYPE;
   } /* Endif */
#endif

   if (partpool_ptr->TOTAL_BLOCKS != partpool_ptr->AVAILABLE) {
      _KLOGX2(KLOG_partition_destroy, PARTITION_ALL_BLOCKS_NOT_FREE);
      return PARTITION_ALL_BLOCKS_NOT_FREE;
   } /* Endif */

   /* Start CR 532 */
   /* Remove the partition from the list of partitions maintained by MQX */
   _int_disable();
   _QUEUE_REMOVE(&part_component_ptr->PARTITIONS, partpool_ptr);
   _int_enable();
   /* End  CR 532 */

   partpool_ptr->POOL.VALID = 0;

   /* Free any extensions */
   current_ptr = partpool_ptr->POOL.NEXT_POOL_PTR;
   while(current_ptr) {
      next_ptr = current_ptr->NEXT_POOL_PTR;
      _mem_free(current_ptr);
      current_ptr = next_ptr;
   } /* Endwhile */  

   result = _mem_free(partpool_ptr);

   _KLOGX2(KLOG_partition_destroy, result);

   return result;

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
