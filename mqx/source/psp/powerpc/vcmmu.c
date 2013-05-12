/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: vcmmu.c$
* $Version : 3.0.2.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains utiltity functions for use with the virtual
*   MMU functions for the PowerPC.
*
*END************************************************************************/

#include "mqx_inc.h"

#if defined(PSP_NO_MMU_YET) || defined(PSP_NO_VMMU_YET)
int ___safsahkfsdlkd;
#else

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _mmu_create_vcontext
* Returned Value   : _mqx_uint
* Comments         :
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _mmu_create_vcontext
   (
      /* [IN] the task to which a virtual context is to be added */
      _task_id task_id
   )
{ /* Body */
   TD_STRUCT_PTR                  td_ptr;
   PSP_VIRTUAL_CONTEXT_STRUCT_PTR context_ptr;

   td_ptr = _task_get_td(task_id);
   if (td_ptr == NULL) {
      return(MQX_INVALID_TASK_ID);
   }/* Endif */
   context_ptr = _mem_alloc_system_zero(sizeof(PSP_VIRTUAL_CONTEXT_STRUCT));
   if (context_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   }/* Endif */
   _queue_init(&context_ptr->PAGE_INFO,0);
   _int_disable();
   if (td_ptr->FLAGS & TASK_MMU_CONTEXT_EXISTS) {
      _int_enable();
      _mem_free(context_ptr);
      return(MQX_MMU_CONTEXT_EXISTS);
   } /* Endif */

   td_ptr->MMU_VIRTUAL_CONTEXT_PTR = context_ptr;
   td_ptr->FLAGS |= TASK_MMU_CONTEXT_EXISTS;
   _int_enable();
   return(MQX_OK);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _mmu_destroy_vcontext
* Returned Value   : _mqx_uint
* Comments         :
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _mmu_destroy_vcontext
   (
      /* [IN] the task for which a virtual context is to be removed */
      _task_id task_id
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR         kernel_data;
   TD_STRUCT_PTR                  td_ptr;
   PSP_VIRTUAL_CONTEXT_STRUCT_PTR context_ptr;
   PSP_PAGE_INFO_STRUCT_PTR       mem_ptr;
   PSP_SUPPORT_STRUCT_PTR         psp_support_ptr;

   _GET_KERNEL_DATA(kernel_data);
   psp_support_ptr = kernel_data->PSP_SUPPORT_PTR;
   td_ptr = _task_get_td(task_id);
   if (td_ptr == NULL) {
      return(MQX_INVALID_TASK_ID);
   }/* Endif */
   _int_disable();
   if ((td_ptr->FLAGS & TASK_MMU_CONTEXT_EXISTS) == 0) {
      _int_enable();
      return(MQX_MMU_CONTEXT_DOES_NOT_EXIST);
   } /* Endif */
   if (td_ptr == kernel_data->ACTIVE_PTR) {
      /* Remove task MMU pages from the MMU table */
      _mmu_reset_vcontext_internal();
   }/* Endif */
   td_ptr->FLAGS &= ~TASK_MMU_CONTEXT_EXISTS;
   context_ptr = td_ptr->MMU_VIRTUAL_CONTEXT_PTR;
   td_ptr->MMU_VIRTUAL_CONTEXT_PTR = NULL;
   _lwsem_wait(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
   _int_enable();

   while (_QUEUE_GET_SIZE(&context_ptr->PAGE_INFO)) {
      _QUEUE_DEQUEUE(&context_ptr->PAGE_INFO, mem_ptr);
      _QUEUE_ENQUEUE(&psp_support_ptr->VPAGE_FREELIST, &mem_ptr->ELEMENT);
   } /* Endwhile */
   _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
   _mem_free(context_ptr);
   return(MQX_OK);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _mmu_add_vcontext
* Returned Value   : uint_32
* Comments         :
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _mmu_add_vcontext
   (
      /* [IN] the task to which a virtual context is to be added */
      _task_id  task_id,

      /* [IN] the virtual address to use */
      pointer   vaddr,

      /* [IN] the size */
      _mem_size input_size,

      /* [IN] the MMU flags to use */
      _mqx_uint flags
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR         kernel_data;
   TD_STRUCT_PTR                  td_ptr;
   PSP_VIRTUAL_CONTEXT_STRUCT_PTR context_ptr;
   PSP_PAGE_INFO_STRUCT_PTR       mem_ptr;
   PSP_SUPPORT_STRUCT_PTR         psp_support_ptr;
   uint_32                        page_size;
   int_32                         test_size;
   int_32                         size = (int_32)input_size;
   uint_32                        result;
   uchar_ptr                      taddr;
   pointer                        tmp;

   _GET_KERNEL_DATA(kernel_data);
   td_ptr = _task_get_td(task_id);
   if (td_ptr == NULL) {
      return(MQX_INVALID_TASK_ID);
   }/* Endif */
   context_ptr = td_ptr->MMU_VIRTUAL_CONTEXT_PTR;
   psp_support_ptr = kernel_data->PSP_SUPPORT_PTR;
   page_size = psp_support_ptr->PAGE_SIZE;

   _lwsem_wait(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
   if ((td_ptr->FLAGS & TASK_MMU_CONTEXT_EXISTS) == 0) {
      _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
      return(MQX_MMU_CONTEXT_DOES_NOT_EXIST);
   } /* Endif */

   if (size > (_QUEUE_GET_SIZE(&psp_support_ptr->VPAGE_FREELIST) * page_size)) {
      _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
      return(MQX_OUT_OF_MEMORY);
   }/* Endif */

   /* Verify virtual memory not in use */
   test_size = size;
   taddr = vaddr;
   while (test_size > 0) {
      /* Use Vtop !! */
      if (_mmu_vtop(taddr,&tmp) == MQX_OK) {
         _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
         return(MQX_INVALID_PARAMETER);
      }/* Endif */
      taddr += page_size;
      test_size -= page_size;
   } /* Endwhile */

   while (size > 0) {
      _QUEUE_DEQUEUE(&psp_support_ptr->VPAGE_FREELIST,mem_ptr);
      mem_ptr->VADDR = vaddr;
      result = _mmu_set_vmem_loc_internal(mem_ptr, flags);
      if (result != MQX_OK) {
         _QUEUE_ENQUEUE(&psp_support_ptr->VPAGE_FREELIST,mem_ptr);
         _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
         return(MQX_OUT_OF_MEMORY);
      } /* Endif */
      if (td_ptr == kernel_data->ACTIVE_PTR) {
         result = _mmu_add_vregion(mem_ptr->ADDR, mem_ptr->VADDR, page_size,
            flags);
         if (result != MQX_OK) {
            _QUEUE_ENQUEUE(&psp_support_ptr->VPAGE_FREELIST,mem_ptr);
            _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
            return(MQX_OUT_OF_MEMORY);
         } /* Endif */
      } /* Endif */
      _QUEUE_ENQUEUE(&context_ptr->PAGE_INFO,mem_ptr);
      size  -= page_size;
      vaddr = (pointer)((uint_32)vaddr + page_size);
   } /* Endwhile */

   _lwsem_post(&psp_support_ptr->VPAGE_FREELIST_LWSEM);
   return(MQX_OK);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _mmu_create_vtask
* Returned Value   :
* Comments         :
*
*END*-------------------------------------------------------------------------*/

_task_id _mmu_create_vtask
   (
      /* [IN] the task template number */
      _mqx_uint                template_number,

      /* [IN] the task creation parameter */
      _mqx_uint                parameter,

      /* [IN] the data address to copy initialization values from*/
      pointer                  data_init_ptr,

      /* [IN] the virtual address for the virtual memory */
      pointer                  vaddr,

      /* [IN] the size of the virtual memory */
      _mem_size                size,

      /* [IN] the MMU flags to use */
      _mqx_uint                flags
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   TD_STRUCT_PTR          my_td_ptr;
   uint_32                result;
   _task_id               task_id;

   _GET_KERNEL_DATA(kernel_data);
   my_td_ptr = kernel_data->ACTIVE_PTR;
   if (my_td_ptr->FLAGS & TASK_MMU_CONTEXT_EXISTS) {
      return(MQX_MMU_PARENT_TASK_CANNOT_BE_MMU);
   } /* Endif */

   task_id = _task_create_blocked(0, template_number, parameter);
   if (task_id == MQX_NULL_TASK_ID) {
      return(MQX_NULL_TASK_ID);
   } /* Endif */
   td_ptr = _task_get_td(task_id);
   result = _mmu_create_vcontext(task_id);
   if (result != MQX_OK) {
      _task_destroy(task_id);
      _task_set_error(result);
      return(MQX_NULL_TASK_ID);
   } /* Endif */
   result = _mmu_add_vcontext(task_id, vaddr, size, flags);
   if (result != MQX_OK) {
      _task_destroy(task_id);
      _task_set_error(result);
      return(MQX_NULL_TASK_ID);
   } /* Endif */

   if (data_init_ptr != NULL) {
      _int_disable();
      my_td_ptr->MMU_VIRTUAL_CONTEXT_PTR =
         td_ptr->MMU_VIRTUAL_CONTEXT_PTR;
      my_td_ptr->FLAGS |= TASK_MMU_CONTEXT_EXISTS;
      _mmu_set_vcontext_internal();
      _int_enable();
      _mem_copy(data_init_ptr, vaddr, size);
      _int_disable();
      _mmu_reset_vcontext_internal();
      my_td_ptr->FLAGS &= ~TASK_MMU_CONTEXT_EXISTS;
      my_td_ptr->MMU_VIRTUAL_CONTEXT_PTR = NULL;
      _int_enable();
   } /* Endif */

   _task_ready(td_ptr);
   return(task_id);

} /* Endbody */

#endif /* PSP_NO_MMU_YET */
/* EOF */
