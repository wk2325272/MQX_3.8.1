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
* $FileName: mem_init.c$
* $Version : 3.8.6.5$
* $Date    : Jun-21-2012$
*
* Comments:
*
*   This file contains the function that initializes the memory pool.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

#if MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE   
extern uchar __UNCACHED_DATA_START[];
extern uchar __UNCACHED_DATA_END[];
#endif // MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE   

/*FUNCTION*-----------------------------------------------------
*
* Function Name    : _mem_init_internal
* Returned Value   : _mqx_uint error_code
*       MQX_OK, INIT_KERNEL_MEMORY_TOO_SMALL
* Comments         :
*   This function initializes the memory storage pool.
*
*END*---------------------------------------------------------*/

_mqx_uint _mem_init_internal
   (
      void
   )
{ /* Body */
#if MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE   
   pointer __uncached_data_start = (pointer )__UNCACHED_DATA_START;
   pointer __uncached_data_end   = (pointer )__UNCACHED_DATA_END;
#endif // MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE   

   KERNEL_DATA_STRUCT_PTR kernel_data;
   pointer start;
   _mqx_uint result;

   _GET_KERNEL_DATA(kernel_data);

   _QUEUE_INIT(&kernel_data->MEM_COMP.POOLS, 0);

   _lwsem_create((LWSEM_STRUCT_PTR)&kernel_data->MEM_COMP.SEM, 1);

   kernel_data->MEM_COMP.VALID = MEMPOOL_VALID;

   /*
   ** Move the MQX memory pool pointer past the end of kernel data.
   */
   start = (pointer)((uchar_ptr)kernel_data + sizeof(KERNEL_DATA_STRUCT));

   result = _mem_create_pool_internal(start, kernel_data->INIT.END_OF_KERNEL_MEMORY, (MEMPOOL_STRUCT_PTR)&kernel_data->KD_POOL);

#if MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE
   if (result == MQX_OK)
   {
      if ((__uncached_data_start <=  start) && (start <= __uncached_data_end))
      {
         kernel_data->UNCACHED_POOL = &kernel_data->KD_POOL;
      }
      else
      {
         /* The pool state structure is created at the bottom of the pool */
         kernel_data->UNCACHED_POOL = (MEMPOOL_STRUCT_PTR)_mem_create_pool(__uncached_data_start, (_mem_size)__uncached_data_end - (_mem_size)__uncached_data_start);
      }
    }
#endif // MQX_USE_UNCACHED_MEM && PSP_HAS_DATA_CACHE

   return result;

} /* Endbody */

#endif
/* EOF */
