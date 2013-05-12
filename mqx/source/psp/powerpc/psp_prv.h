#ifndef __psp_prv_h__
#define __psp_prv_h__
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
* $FileName: psp_prv.h$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains psp private declarations for use when compiling
*   the kernel.
*
*END************************************************************************/


/* This macro modifies the context of a blocked task so that the
** task will execute the provided function when it next runs
*/
#define _PSP_SET_PC_OF_BLOCKED_TASK(stack_ptr, func) \
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->RETURN_ADDRESS = \
   (void (_CODE_PTR_)(uint_32))(func)

/* This macro modifies the interrupt priority of a blocked task so that the
** task will execute at the correct interrupt priority when it restarts
*/
#define _PSP_SET_SR_OF_BLOCKED_TASK(stack_ptr, sr_value)

/* This macro modifies the context of a task that has been interrupted
** so that the task will execute the provided function when the isr returns
*/
#define _PSP_SET_PC_OF_INTERRUPTED_TASK(stack_ptr, func) \
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->RETURN_ADDRESS = \
   (void (_CODE_PTR_)(uint_32))(func)

#define _PSP_GENERATE_INTERRUPT(vector) \
{ \
   KERNEL_DATA_STRUCT_PTR         kernel_data;\
   INTERRUPT_TABLE_STRUCT_PTR     table_ptr;\
   PSP_INTERRUPT_FRAME_STRUCT_PTR exception_frame_ptr;\
   uint_32                        old_vector;\
\
   _GET_KERNEL_DATA(kernel_data);\
\
   exception_frame_ptr = (pointer)kernel_data->INTERRUPT_CONTEXT_PTR;\
   old_vector = exception_frame_ptr->EXCEPTION_NUMBER;\
   if ((vector < kernel_data->FIRST_USER_ISR_VECTOR) ||\
      (vector > kernel_data->LAST_USER_ISR_VECTOR))\
   {\
      exception_frame_ptr->EXCEPTION_NUMBER = vector;\
      _KLOG(_klog_isr_start_internal(vector);)\
      (*kernel_data->DEFAULT_ISR)((pointer)vector);\
      _KLOG(_klog_isr_end_internal(vector);)\
   } else {\
      table_ptr = &kernel_data->INTERRUPT_TABLE_PTR[vector -\
         kernel_data->FIRST_USER_ISR_VECTOR];\
      exception_frame_ptr->EXCEPTION_NUMBER = vector;\
      _KLOG(_klog_isr_start_internal(vector);)\
      (*table_ptr->APP_ISR)(table_ptr->APP_ISR_DATA);\
      _KLOG(_klog_isr_end_internal(vector);)\
   } /* Endif */\
   exception_frame_ptr->EXCEPTION_NUMBER = old_vector;\
}

/*--------------------------------------------------------------------------*/
/*
**                PROTOTYPES OF PRIVATE PSP FUNCTIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

extern void _psp_tlb_inst_violation_esr(void);
extern void _psp_tlb_data_violation_esr(void);

extern uchar _psp_itlb_miss_isr[];
extern uchar _psp_itlb_miss_isr_end[];
extern uchar _psp_dtlb_miss_isr[];
extern uchar _psp_dtlb_miss_isr_end[];

extern uchar _psp_dtlb_miss_on_load_isr[];
extern uchar _psp_dtlb_miss_on_load_isr_end[];
extern uchar _psp_dtlb_miss_on_store_isr[];
extern uchar _psp_dtlb_miss_on_store_isr_end[];

#ifndef PSP_NO_MMU_YET
//Start CR 1340
#if ((MQX_CPU==405) || defined(PSP_G2_CORE) || defined(PSP_E300_CORE) || (MQX_CPU==7400) || (MQX_CPU==850) || (MQX_CPU==855) || (MQX_CPU==860))
//End CR 1340
extern _mqx_uint _mmu_set_vmem_loc_internal(PSP_PAGE_INFO_STRUCT_PTR, _mqx_uint);
extern void      _mmu_reset_vcontext_internal(void);
extern void      _mmu_set_vcontext_internal(void);
#endif
#endif

extern void      _psp_dump_context(pointer);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
