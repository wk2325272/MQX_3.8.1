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
* $FileName: kl_stsh.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for displaying stack
*   usage
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_KERNEL_LOGGING
#include "fio.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _klog_show_stack_usage
* Returned Value  : None
* Comments        : This function prints out the stack usage for
*  all tasks currently running in the MQX system.  It assumes that
*  MQX has been configured with MQX_MONITOR_STACK.
*
*END*------------------------------------------------------------------*/

void  _klog_show_stack_usage
   (
      void
   )
{ /* Body */
#if MQX_MONITOR_STACK
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;
   _mem_size              stack_size;
   _mem_size              stack_used;
   _mqx_uint              size;
   
   _GET_KERNEL_DATA(kernel_data);

   /* Now display the gathered information */
   _klog_get_interrupt_stack_usage(&stack_size, &stack_used);
   printf("Stack usage: \n\n");
   printf("Interrupt stack:    size %08ld    used %08ld\n\n", 
      (uint_32)stack_size, (uint_32)stack_used);

   printf("SIZE                  USED                 TASK ID      NAME\n");

   _lwsem_wait((LWSEM_STRUCT_PTR)(&kernel_data->TASK_CREATE_LWSEM));
   td_ptr = (TD_STRUCT_PTR)((uchar_ptr)kernel_data->TD_LIST.NEXT -
      FIELD_OFFSET(TD_STRUCT,TD_LIST_INFO));
   size   = _QUEUE_GET_SIZE(&kernel_data->TD_LIST);

   while (size && td_ptr) {
      _klog_get_task_stack_usage_internal(td_ptr, &stack_size, &stack_used);
      printf("%08ld(%08lX)    %08ld(%08lX)   %08lX     %s\n", 
         (uint_32)stack_size, (uint_32)stack_size, (uint_32)stack_used, 
         (uint_32)stack_used, td_ptr->TASK_ID,
         (td_ptr->TASK_TEMPLATE_PTR->TASK_NAME != NULL) ? 
            td_ptr->TASK_TEMPLATE_PTR->TASK_NAME : "");
      size--;
      td_ptr = (TD_STRUCT_PTR)((uchar_ptr)(td_ptr->TD_LIST_INFO.NEXT) -
         FIELD_OFFSET(TD_STRUCT,TD_LIST_INFO));
   } /* Endwhile */

   _lwsem_post((LWSEM_STRUCT_PTR)(&kernel_data->TASK_CREATE_LWSEM));

#else
   printf("Stack usage: ERROR MQX_MONITOR_STACK not set in mqx_cnfg.h\n\n");
#endif
   
} /* Endbody */
#endif /* MQX_KERNEL_LOGGING */

/* EOF */
