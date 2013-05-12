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
* $FileName: mem_crep.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function that initializes a memory pool.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _mem_create_pool
* Returned Value   : A handle to the memory pool or 
*                    NULL_MEM_POOL_ID on error.
* Comments         :
*   This function initializes a memory storage pool. Will set task error
* code if error occurs
* 
*END*---------------------------------------------------------*/

_mem_pool_id _mem_create_pool
   (
      /* [IN] the start of the memory pool */
      pointer    start,

      /* [IN] the size of the memory pool */
      _mem_size  size
   )
{ /* Body */
   _KLOGM(KERNEL_DATA_STRUCT_PTR  kernel_data;)
   MEMPOOL_STRUCT_PTR             mem_pool_ptr;
   uchar_ptr                      end;
   _mqx_uint                      error;

   _KLOGM(_GET_KERNEL_DATA(kernel_data);)
   _KLOGE3(KLOG_mem_create_pool, start, size);

#if MQX_CHECK_ERRORS
   if (size < MQX_MIN_MEMORY_POOL_SIZE) {
      error = MQX_MEM_POOL_TOO_SMALL;
      _task_set_error(error);
      _KLOGX4(KLOG_mem_create_pool, start, size, error);
      return NULL;
   } /* Endif */
#endif
   
   mem_pool_ptr = (MEMPOOL_STRUCT_PTR)_ALIGN_ADDR_TO_HIGHER_MEM(start);
   _mem_zero((pointer)mem_pool_ptr, (_mem_size)sizeof(MEMPOOL_STRUCT));

   end   = (uchar_ptr)start + size;
   start = (pointer)((uchar_ptr)mem_pool_ptr + sizeof(MEMPOOL_STRUCT));
   error = _mem_create_pool_internal(start, (pointer)end, mem_pool_ptr);

#if (MQX_CHECK_ERRORS)
   if (error != MQX_OK ) {
      _task_set_error(error);
      _KLOGX4(KLOG_mem_create_pool, start, size, error);
      return NULL;
   } /* Endif */
#endif
  
   return ((_mem_pool_id)mem_pool_ptr);

} /* Endbody */

#endif
/* EOF */
