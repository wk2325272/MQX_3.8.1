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
* $FileName: mem_xfer.c$
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
 * Function Name    : _mem_transfer
 * Returned Value   : _mqx_uint MQX_OK, or a mqx error code.
 * Comments         :
 *   This routine transfers the ownership of a block of memory from
 *   an owner task to another task.
 *
 *END*--------------------------------------------------------*/

_mqx_uint _mem_transfer
   (  
      /* [IN] the memory block whose ownership is to be transferred */
      pointer  memory_ptr,
      
      /* [IN] the source (owner) task id */
      _task_id source_id,
      
      /* [IN] the target (new owner) task id */
      _task_id target_id
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   STOREBLOCK_STRUCT_PTR  block_ptr;
   TD_STRUCT_PTR          source_td;
   TD_STRUCT_PTR          target_td;
   _mqx_uint              result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE4(KLOG_mem_transfer, memory_ptr, source_id, target_id);

#if MQX_CHECK_ERRORS
   if (memory_ptr == NULL) {
      _task_set_error(MQX_INVALID_POINTER);
      _KLOGX2(KLOG_mem_transfer, MQX_INVALID_POINTER);
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif

   /* Verify the block */
   block_ptr = GET_MEMBLOCK_PTR(memory_ptr);

#if MQX_CHECK_ERRORS
   if (! _MEMORY_ALIGNED(block_ptr)) {
      _task_set_error(MQX_INVALID_POINTER);
      _KLOGX2(KLOG_mem_transfer, MQX_INVALID_POINTER);
      return(MQX_INVALID_POINTER);
   } /* Endif */
#endif

#if MQX_CHECK_VALIDITY
   if ( ! VALID_CHECKSUM(block_ptr) ) {
      _task_set_error(MQX_INVALID_CHECKSUM);
      kernel_data->KD_POOL.POOL_BLOCK_IN_ERROR = block_ptr;
      _KLOGX2(KLOG_mem_transfer, MQX_INVALID_CHECKSUM);
      return(MQX_INVALID_CHECKSUM);
   } /* Endif */
#endif

   source_td = (TD_STRUCT_PTR)_task_get_td(source_id);
   target_td = (TD_STRUCT_PTR)_task_get_td(target_id);

#if MQX_CHECK_ERRORS
   if ( (source_td == NULL) || (target_td == NULL) ) {
      _task_set_error(MQX_INVALID_TASK_ID);
      _KLOGX2(KLOG_mem_transfer, MQX_INVALID_TASK_ID);
      return(MQX_INVALID_TASK_ID);
   } /* Endif */
#endif

   _INT_DISABLE();

   result = _mem_transfer_td_internal(memory_ptr, source_td, target_td);
/* Start CR 2133 */   
   if (result != MQX_OK) _task_set_error(result);
/* End CR 2133 */   

   _INT_ENABLE();

   _KLOGX2(KLOG_mem_transfer, result);
   return(result);

} /* Endbody */

#endif
/* EOF */
