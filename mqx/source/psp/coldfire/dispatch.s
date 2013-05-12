;*HEADER********************************************************************
;* 
;* Copyright (c) 2008 Freescale Semiconductor;
;* All Rights Reserved                       
;*
;* Copyright (c) 1989-2008 ARC International;
;* All Rights Reserved
;*
;*************************************************************************** 
;*
;* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
;* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
;* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
;* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
;* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
;* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
;* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
;* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
;* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
;* THE POSSIBILITY OF SUCH DAMAGE.
;*
;**************************************************************************
;*
;* $FileName: dispatch.s$
;* $Version : 3.8.28.0$
;* $Date    : Aug-31-2011$
;*
;* Comments:
;*
;*   This assembler file contains functions for task scheduling
;*
;*END***********************************************************************

        ;.file "dispatch.s"

#include <asm_mac.h>
#include "mqx_cnfg.h"

#define __ASM__
#include "psp.h"
#undef __ASM__
        
#include "types.s"
#include "psp_prv.s"

#ifdef __ACF__
        RSEG KERNEL:CODE (4)     
#else
        .text
        .section KERNEL,16,x
#endif

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _task_block
; Comments         :
; This function is called by a task to save its context and remove
; itself from its ready queue. The next runnable task in the ready queues
; is made active and dispatched.  The state of the task is marked as being
; blocked.
;
;END*------------------------------------------------------------------------

        ASM_PUBLIC(_task_block)
        ASM_ALIGN(4)

ASM_PREFIX(_task_block):
        SAVE_ALL_REGISTERS              ; Save the context of the active task.

        GET_KERNEL_DATA a2              ; Get the address of kernel data
        movea.l (KD_ACTIVE_PTR,a2),a3    ; get active td
        move.w  (KD_DISABLE_SR,a2),d0    ; DISABLE INTERRUPTS
        move.w  d0,sr
        move.l  sp,(TD_STACK_PTR,a3)     ; save stack pointer
#if PSP_HAS_DSP
        SAVE_DSP_REGISTERS a3,a4        ; wipes d1
#endif
        moveq.l #1,d0                   ; Set the block bit
        or.l    d0,(TD_STATE,a3)

#if MQX_KERNEL_LOGGING
        KLOG    a2,ASM_PREFIX(_klog_block_internal) ; kernel log this function
#endif

;       Remove the active task from the ready queue.
        movea.l (TD_TD_PREV,a3),a1       ; get ptr to ready_q structure
        movea.l (TD_TD_NEXT,a3),a4
        move.l  a4,(RQ_HEAD_READY_Q,a1)
        move.l  a1,(TD_TD_PREV,a4)

        bra.w   __sched_internal  ; Search for the next task in the ready queue.


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _sched_start_internal
; Comments         :
;  This function is called from _mqx in order to start the task
;  scheduler running.
;
;END*------------------------------------------------------------------------

        ASM_PUBLIC(_sched_start_internal)
        ASM_ALIGN(4)

ASM_PREFIX(_sched_start_internal):
        GET_KERNEL_DATA a2
        bra.w           __sched_internal


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _sched_check_scheduler_internal
; Comments         :
;    This function is called to check whether scheduling is necessary
; It falls through into the next function
;
; Function Name    : _sched_execute_scheduler_internal
; Comments         :
;    This function is called by a task to save its context.
; This is usually done when the task has already been removed from the
; ready queue, and is on some other queue.
; However it can also be called so that the current tasks context is saved
; so that the scheduler can run (in case of a higher priority task being
; available.
;
; It falls through to the next function
;
; Function Name    : _sched_internal
; Comments         :
;   This function is the actual task scheduler... IT IS NOT CALLABLE from C
; rather, other assembler functions in this file jump to it.
;
;END*------------------------------------------------------------------------

        ASM_PUBLIC(_sched_check_scheduler_internal)
        ASM_PUBLIC(_sched_execute_scheduler_internal)
        ASM_PUBLIC(__sched_context_switch_internal)
        ASM_PUBLIC(_sched_run_internal)
ASM_PREFIX(_sched_run_internal):
        GET_KERNEL_DATA a2              ; Get address of kernel data
        bra.w    __sched_internal

__sched_check_return:
        rts
        ASM_CONST16(0x0000)


        ASM_ALIGN(4)
ASM_PREFIX(_sched_check_scheduler_internal):
;       Use A0, D0: both scratch registers
        GET_KERNEL_DATA a0              ; Get address of kernel data
        move.w  (KD_IN_ISR,a0),d0
        bne.b   __sched_check_return    ; We are in an ISR, so return
        move.l  (KD_CURRENT_READY_Q,a0),d0
        movea.l (KD_ACTIVE_PTR,a0),a0
        cmp.l   (TD_MY_QUEUE,a0),d0
        beq.b   __sched_check_return    ; Current task is still the active task

ASM_PREFIX(_sched_execute_scheduler_internal):
        SAVE_ALL_REGISTERS              ; Save the context of the active task.

        GET_KERNEL_DATA a2              ; Get address of kernel data
        movea.l (KD_ACTIVE_PTR,a2),a3    ; get active td
        move.w  (KD_DISABLE_SR,a2),d0    ; DISABLE INTERRUPTS
        move.w  d0,sr
        move.l  sp,(TD_STACK_PTR,a3)     ; save stack pointer
#if PSP_HAS_DSP
        SAVE_DSP_REGISTERS a3,a4        ; wipes d1
#endif

#if MQX_KERNEL_LOGGING
        KLOG    a2,ASM_PREFIX(_klog_execute_scheduler_internal) ; kernel log this function
#endif

;
;  MAIN TASK SCHEDULER CODE
;  Arrive here from other assembler functions with a2 already set

__sched_internal:
        movea.l (KD_CURRENT_READY_Q,a2),a1  ; get current ready q

find_nonempty_queue:
        movea.l (a1),a3                 ; address of first td
        cmpa.l  a3,a1                   ; ready_q structure itself?
        bne.b   activate
        movea.l (RQ_NEXT_Q,a1),a1        ; try next queue
        move.l  a1,d0
        bne.b   find_nonempty_queue

       ; No task available to run
no_one_to_run:
;       Set up system task running and wait for an interrupt
        lea.l   (KD_SYSTEM_TD,a2),a3
        move.l  a3,(KD_ACTIVE_PTR,a2)
        move.w  #0x2000,d0
        move.w  d0,(KD_ACTIVE_SR,a2)
; Start CR 1169
;       move.l  KD_INTERRUPT_STACK_PTR(a2),sp
        GET_SYSTEM_STACK a1
        movea.l a1, sp
; End CR 1169
#if PSP_STOP_ON_IDLE
        stop    #0x2000  
#else                
        move.w  #0x2000,sr
#endif        


        movea.l (KD_READY_Q_LIST,a2),a1  ; get ready just in case
        bra.b   find_nonempty_queue

activate:
#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
;       We now have the new task to run, check if it needs the
;       floating point co-processor
        move.w   (TD_FLAGS+2,a3),d0
        andi.l   #FP_TASK_MASK,d0
        bne.b    do_floating_point
#endif

no_floating_point:
        move.l   a1,(KD_CURRENT_READY_Q,a2)
        move.l   a3,(KD_ACTIVE_PTR,a2)
        move.w   (TD_TASK_SR,a3),(KD_ACTIVE_SR,a2)
        movea.l  (TD_STACK_PTR,a3),sp     ; restore stack pointer

#if MQX_KERNEL_LOGGING
        KLOG     a2,ASM_PREFIX(_klog_context_switch_internal)  ; do kernel logging
#endif

#if PSP_HAS_DSP
        RESTORE_DSP_REGISTERS a3,a0      ; kills d0
#endif
        RESTORE_ALL_REGISTERS            ; restore task context
__sched_context_switch_internal:
        rte
        ASM_CONST16(0x0000)

#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
        ASM_ALIGN(4)
do_floating_point:
;       See if a floating point task is currently active
        movea.l  (KD_FP_ACTIVE_PTR,a2),a4
        tst.l    a4
        beq.b    restore_fp_context

;       See if the floating point task is in fact the task being scheduled
;       So, no need to save and restore pointers
        cmpa.l   a3,a4
        beq.b    no_floating_point

;       Stop the floating point unit, and save the internal
;       floating point registers in the floating point context
;       save area allocated for the task
        movea.l  (TD_FLOAT_CONTEXT_PTR,a4), a0
        fmove.l  FPSR,(FP_FPSR_OFFSET,a0)
        fmove.l  FPCR,FP_FPCR_OFFSET(a0)
        fmove.l  FPIAR,FP_FPIAR_OFFSET(a0)
        fmovem   FP0-FP7,(FP_FPR0_OFFSET,a0)

;       Indicate in TD that FP context saved
        move.w   (TD_FLAGS+2,a4),d0
        or.l     #FP_CONTEXT_SAVED_MASK,d0      ; Set the block bit
        move.w   d0,(TD_FLAGS+2,a4)

restore_fp_context:
;       Restore the context of the current FP task if necessary
        move.l   a3,(KD_FP_ACTIVE_PTR,a2)
        move.w   (TD_FLAGS+2,a3),d0
        andi.l   #FP_CONTEXT_SAVED_MASK,d0
        beq.b    no_fp_registers_to_restore

;       Now restore the floating point context
        movea.l  (TD_FLOAT_CONTEXT_PTR,a3), a0

  #if MQX_FP_CONTEXT_CHECK
;       This code tests that the floating point buffer we are restoring
;       the floating point context from really belongs to task 'a3'.  If
;       the task ID's don't match then something has been corrupted.
;       This code can be disabled for faster performance.
        move.l   (TD_TASK_ID,a3),d0
        cmp.l    (FP_TID_OFFSET,a0),d0
        beq.b    fp_context_check_ok
        move.l   #4138,d0
        move.l   d0,(a7)
        ASM_EXTERN(ASM_PREFIX(_mqx_fatal_error))
        jsr (ASM_PREFIX(_mqx_fatal_error))
        halt
fp_context_check_ok:
  #endif

        fmovem   (FP_FPR0_OFFSET,a0),FP0-FP7
        fmove.l  (FP_FPCR_OFFSET,a0),FPCR
        fmove.l  (FP_FPIAR_OFFSET,a0),FPIAR
        fmove.l  (FP_FPSR_OFFSET,a0),FPSR

no_fp_registers_to_restore:
        move.w   (TD_FLAGS+2,a3),d0
        and.l    #FP_CONTEXT_CLEAR_MASK,d0
        move.w   d0,(TD_FLAGS+2,a3)
        jmp      (no_floating_point)
#endif

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_save_fp_context_internal
; Returned Value   : none
; Comments         :
;   This function saves the floating point context for the
; current floating point task
;
; THIS FUNCTION MUST BE CALLED DISABLED
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(_psp_save_fp_context_internal)
        ASM_ALIGN(4)

ASM_PREFIX(_psp_save_fp_context_internal):
#if MQXCFG_ENABLE_FP && PSP_HAS_FPU
;       Stop the floating point unit, and save the internal
;       floating point registers on the stack of the FP task
        GET_KERNEL_DATA a0
        movea.l  (KD_FP_ACTIVE_PTR,a0),a0
        move.w   (TD_FLAGS+2,a0),d0
        andi.l   #FP_CONTEXT_SAVED_MASK,d0
        bne.b    _psp_save_fp_context_done

        or.l     #FP_CONTEXT_SAVED_MASK,d0
        move.w   d0,(TD_FLAGS+2,a0)
        movea.l  (TD_FLOAT_CONTEXT_PTR,a0), a0
        fmove.l  FPSR,(FP_FPSR_OFFSET,a0)
        fmove.l  FPCR,FP_FPCR_OFFSET(a0)
        fmove.l  FPIAR,FP_FPIAR_OFFSET(a0)
        fmovem   FP0-FP7,(FP_FPR0_OFFSET,a0)
#endif
_psp_save_fp_context_done:
        rts
        ASM_CONST16(0x0000)

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_a6_sp_and_goto
; Returned Value   : previous value of location
; Comments         :
;   This function tests a byte location, and if 0, sets it to 0x80.
;   It returns the previous value of the byte.
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(_psp_set_a6_sp_and_goto)
        ASM_ALIGN(4)

ASM_PREFIX(_psp_set_a6_sp_and_goto):
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   movea.l  (4,sp), a6
   movea.l  (12,sp), a0
   movea.l  (8,sp), sp
#else
   movea.l  d0, a6
   movea.l  d1, sp
   movea.l  d2, a0   
#endif
   jmp      (a0)
   rts
   ASM_CONST16(0x0000)

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _mem_test_and_set
; Returned Value   : previous value of location
; Comments         :
;   This function tests a byte location, and if 0, sets it to 0x80.
;   It returns the previous value of the byte.
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(_mem_test_and_set)
        ASM_ALIGN(4)

ASM_PREFIX(_mem_test_and_set):
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   movea.l (4,sp),a0
#endif
   bset.b  #7,(a0)
   bne.b   bit_was_set
   clr.l   d0
   rts
   ASM_CONST16(0x0000)

bit_was_set:
   move.l  #0x80,d0
   rts
   ASM_CONST16(0x0000)

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_sr
; Returned Value   : none
; Comments         :
;   This function sets the sr register
;
;END*----------------------------------------------------------------------
        ASM_PUBLIC(__psp_set_sr)
        ASM_ALIGN(4)

ASM_PREFIX(__psp_set_sr):
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4,sp),d0
#endif
   move.w   d0,sr
   rts
   ASM_CONST16(0x0000)

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_get_sr
; Returned Value   : none
; Comments         :
;   This function gets the sr register
;
;END*----------------------------------------------------------------------
        ASM_PUBLIC(__psp_get_sr)
        ASM_ALIGN(4)

ASM_PREFIX(__psp_get_sr):
   move.w   sr,d0
   rts
   ASM_CONST16(0x0000)


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_vbr
; Returned Value   : uint_32 vbr value
; Comments         :
;   This function sets the vbr register
;
;END*----------------------------------------------------------------------
        ASM_PUBLIC(_psp_set_vbr)
        ASM_ALIGN(4)
        
ASM_PREFIX(_psp_set_vbr):
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4,sp),d0
#endif
   movec    d0, vbr
   rts
   ASM_CONST16(0x0000)


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_cpucr
; Returned Value   : none
; Comments         :
;   This function sets the cpucr register
;
;END*----------------------------------------------------------------------
        ASM_PUBLIC(_psp_set_cpucr)
        ASM_ALIGN(4)

ASM_PREFIX(_psp_set_cpucr):
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4,sp),d0
#endif
;   movec    d0,cpucr
   ASM_CONST16(0x4E7B)
   ASM_CONST16(0x0802)
   rts
   ASM_CONST16(0x0000)


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_cacr
; Returned Value   : uint_32 cacr value
; Comments         :
;   This function sets the cacr register
;
;END*----------------------------------------------------------------------
        ASM_PUBLIC(_psp_set_cacr)
        ASM_ALIGN(4)

ASM_PREFIX(_psp_set_cacr):
#ifdef __ACF__   /* ACF uses register passing */
   movec    d0, 0x002
#else

#if PSP_ABI == PSP_ABI_STD
    move.l   (4,sp),d0
#endif

    movec    d0, cacr
#endif
   rts
   ASM_CONST16(0x0000)


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_mbar
; Returned Value   : none
; Comments         :
;   This function sets the mbar register
;
;END*----------------------------------------------------------------------
        ASM_PUBLIC(_psp_set_mbar)
        ASM_ALIGN(4)

ASM_PREFIX(_psp_set_mbar):
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4,sp),d0
#endif
   movec    d0, mbar
   rts
   ASM_CONST16(0x0000)


#if PSP_ACR_CNT

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_set_acr
; Returned Value   : none
; Comments         :
;   This function sets the specified acr register
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(_psp_set_acr)
        ASM_EXTERN(_psp_saved_acr0)
        ASM_EXTERN(_psp_saved_acr1)

#if PSP_ACR_CNT == 4
        ASM_EXTERN(_psp_saved_acr2)
        ASM_EXTERN(_psp_saved_acr3)
#endif
        ASM_ALIGN(4)

ASM_PREFIX(_psp_set_acr):
#if PSP_ACR_CNT == 4
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4, sp), d0
   move.l   (8, sp), d1
#endif
   cmpi.l   #0, d0

   bne.b    l_acr1
   movec    d1,acr0
   move.l   d1,ASM_PREFIX(_psp_saved_acr0)
   rts
   ASM_CONST16(0x0000)
l_acr1:

;   cmpi     #1,d0
   cmpi.l   #1,d0

   bne.b    l_acr2
   movec    d1,acr1
   move.l   d1,ASM_PREFIX(_psp_saved_acr1)
   rts
   ASM_CONST16(0x0000)
l_acr2:

;   cmpi     #2,d0
   cmpi.l   #2,d0

   bne.b    l_acr3
   movec    d1,acr2
   move.l   d1,ASM_PREFIX(_psp_saved_acr2)
   rts
   ASM_CONST16(0x0000)
l_acr3:
   movec    d1,acr3
   move.l   d1,ASM_PREFIX(_psp_saved_acr3)
   rts
   ASM_CONST16(0x0000)
#else // PSP_ACR_CNT == 4
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4, sp), d0
   move.l   (8, sp), d1
#endif
   cmpi.l   #0,d0

   bne.b    l_acr1
   movec    d1,acr0
   move.l   d1,(ASM_PREFIX(_psp_saved_acr0))
   rts
   ASM_CONST16(0x0000)
l_acr1:
   movec    d1,acr1
   move.l   d1,(ASM_PREFIX(_psp_saved_acr1))
   rts
   ASM_CONST16(0x0000)
#endif // PSP_ACR_CNT == 4


;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_get_acr
; Returned Value   : uint_32
; Comments         :
;   This function gets the specified acr register
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(_psp_get_acr)
        ASM_EXTERN(_psp_saved_acr0)
        ASM_EXTERN(_psp_saved_acr1)

#if PSP_ACR_CNT == 4
        ASM_EXTERN(_psp_saved_acr2)
        ASM_EXTERN(_psp_saved_acr3)
#endif
        ASM_ALIGN(4)

ASM_PREFIX(_psp_get_acr):

#if PSP_ACR_CNT == 4
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4,sp),d0
#endif
   cmpi.l   #0,d0

   bne.b    gl_acr1
   move.l   (ASM_PREFIX(_psp_saved_acr0)),d0
   rts
   ASM_CONST16(0x0000)
gl_acr1:

;   cmpi     #1,d0
   cmpi.l   #1,d0

   bne.b    gl_acr2
   move.l   (ASM_PREFIX(_psp_saved_acr1)),d0
   rts
   ASM_CONST16(0x0000)
gl_acr2:

;   cmpi     #2,d0
   cmpi.l   #2,d0

   bne.b    gl_acr3
   move.l   ASM_PREFIX(_psp_saved_acr2), d0
   rts
   ASM_CONST16(0x0000)
gl_acr3:
   move.l   ASM_PREFIX(_psp_saved_acr3), d0
   rts
   ASM_CONST16(0x0000)
#else // PSP_ACR_CNT == 4
//#ifndef __ACF__   /* ACF uses register passing */
#if PSP_ABI == PSP_ABI_STD
   move.l   (4,sp),d0
#endif
;   cmpi     #0,d0
   cmpi.l   #0,d0

   bne.b    gl_acr1
   move.l   (ASM_PREFIX(_psp_saved_acr0)),d0
   rts
   ASM_CONST16(0x0000)
gl_acr1:
   move.l (ASM_PREFIX(_psp_saved_acr1)),d0
   rts
   ASM_CONST16(0x0000)
#endif // PSP_ACR_CNT == 4

#endif // PSP_ACR_CNT

;ISR*-----------------------------------------------------------------------
;
; Function Name    : _int_kernel_isr()
; Comments         :
;   This is the assembler level interrupt isr that intercepts all
; interrupts. (When installed on a particular vector).
;
;   Enough registers are saved so that a 'C' isr can be called.
;   If the current stack is not the interrupt stack, then the stack is
;   switched over.
;
;   0 is pushed onto the stack, and a LINK A6 is done.
;   This effectively is what a function call does, BUT the return address is 0.
;   This will allow a 'C'function to walk back on the stack to find where this
;   'interrupt frame' exists.
;
;   An interrupt 'context' is created on the stack, thus allowing for proper
;   operation of MQX 'C' functions that access the error code and _int_enable
;   and _int_disable
;
;   Then interrupt handlers are checked for.  If they have not been installed,
;   or the current ISR number is out the range of installed handlers,
;   the DEFAULT_ISR is called.
;
;   If they have been installed then if a user 'C' hander has not been installed
;   for this vector, the DEFAULT_ISR is called.
;
;   After returning from the call to the 'C' isr the following is checked:
;   If this is a nested ISR, then we do an interrupt return.
;   If the current task is still the highest priority running task, we
;   do an interrupt return.
;   Otherwise we must save the full context for the current task, and
;   enter the scheduler.
;
;END*------------------------------------------------------------------------

        ASM_PUBLIC(_int_kernel_isr)
        ASM_PUBLIC(_int_kernel_isr_return_internal)
        ASM_ALIGN(4)
        
_isr_save_extra_registers:
        SAVE_REST_ISR_REGISTERS
        bra.w   _isr_no_save_extra

ASM_PREFIX(_int_kernel_isr):
        move.l  d0,-(sp)                     ; save D0
        move.w  sr,d0                        ; remember SR value
        move.w  #0x2700,sr                    ; set level to full disable

        SAVE_ISR_REGISTERS                   ; save the registers

        GET_KERNEL_DATA a0                   ; get the kernel data address

#if MQX_GUERRILLA_INTERRUPTS_EXIST

;        move.w  KD_DISABLE_SR(a0),sr         ; reset to correct disable level
         move.w  KD_DISABLE_SR(a0),d1         ; reset to correct disable level
         move.w  d1,sr                        ;

#endif

        ; save other registers if exception_isr is installed
        clr.l   d1
        btst.b  d1,(KD_FLAGS+1,a0)            ; check for bit0 of 16 bit #
        bne.w   _isr_save_extra_registers
_isr_no_save_extra:
        move.w  (FLINT_FORMAT_OFFSET,sp),d1   ; get format and vector offset
        andi.l  #0x3fc,d1                     ; get vector number
        lsr.l   #2,d1

        move.w  (KD_IN_ISR,a0),d2             ; Check for swap to int stack
;        tst.w   KD_IN_ISR(a0)                ; Check for swap to int stack
        bne.b   _isr_no_stack_swap

        movea.l (KD_ACTIVE_PTR,a0),a1         ; Get TD pointer
        move.l  sp,(TD_STACK_PTR,a1)          ; and save the stack pointer

#if PSP_HAS_DSP
        movea.l d1,a2                        ; save d1
        SAVE_DSP_REGISTERS a1,a0             ; wipes d1
        GET_KERNEL_DATA a0                   ; get the kernel data address
        move.l  a2,d1                        ; restore d1
#endif

        movea.l  (KD_INTERRUPT_STACK_PTR,a0),sp

_isr_no_stack_swap:
;        move.w  KD_IN_ISR(a0),d2             ; Indicate that ISR running
        addq.l  #1,d2
        move.w  d2,(KD_IN_ISR,a0)

        subq.l  #IC_STRUCT_SIZE-8,sp         ; create interrupt context
        subq.l  #8,sp

        ; Initialize the interrupt "context"
        clr.l   (sp)                         ; Clear the error code
        move.w  d0,(4,sp)                     ; save SR as ENABLE sr
        move.w  d1,(6,sp)                     ; save interrupt # in context
        move.l  (KD_INTERRUPT_CONTEXT_PTR,a0),(IC_PREV_CONTEXT,sp) ; set isr cntxt
        move.l  sp,(KD_INTERRUPT_CONTEXT_PTR,a0) ; store new context pointer

#if MQX_KERNEL_LOGGING
        btst.b  #0,(KD_LOG_CONTROL+3,a0)
        beq.b   _isr_no_logging
        lea      (-4*6,sp),sp                ; make room on the stack
        movem.l d0/d1/d2/a0/a1/a2,(0,sp)     ; save registers used
        
    //#ifdef __ACF__
    #if PSP_ABI == PSP_ABI_REG
        move.l  d1,d0
    #else
        move.l  d1,-(sp)                    ; Interrupt number
    #endif
     
        jsr     (ASM_PREFIX(_klog_isr_start_internal)).L
        
    //#ifdef __ACF__
    #if PSP_ABI == PSP_ABI_REG
       movem.l (0,sp),d0/d1/d2/a0/a1/a2     ; restore scratch registers
       adda.l  #6*4,sp
    #else
        movem.l (4,sp),d0/d1/d2/a0/a1/a2     ; restore scratch registers
        adda.l  #7*4,sp
    #endif
    
_isr_no_logging:
#endif // MQX_KERNEL_LOGGING

        move.w  d0,sr                        ; Lower to running sr level
        
#if MQX_SPARSE_ISR_TABLE
;       Find the 'C' isr to run:
        clr.l   d0
        move.l  d1, d2
        move.w  (KD_LAST_USER_ISR_VECTOR + 2, a0), d0
        beq.w   _isr_run_default             ; int table not installed
        cmp.l   d0, d1                        ; vector # too high??
        bgt.w   _isr_run_default             ; cmp calcs d0-d1 > 0 is bad
        move.w  (KD_FIRST_USER_ISR_VECTOR + 2, a0), d0
        sub.l   d0, d1                        ; vector # too low??
        bge.b   _int_kernel_isr_vect_ok     ; cmp calcs d0-d1 < 0 is bad
        bra.w   _isr_run_default

;       we have the interrupt # relative to start of interrupt table
;       Each table entry is 12 bytes in size, so to get to the correct
;       int entry we have to multiply d1 by 12..
_int_kernel_isr_vect_ok:

        lsr.l   #MQX_SPARSE_ISR_SHIFT, d1

        mulu.w  #4,d1
        
        movea.l (KD_INTERRUPT_TABLE_PTR, a0), a1  ; get the interrupt table pointer
        adda.l  d1, a1                          ; get address of entry in table - to linked list

        movea.l (a1), a1                         ; get address of first isr in linked list
        tst.l   a1
        beq.w   _isr_run_default                ; isr not used
        
_isr_search:

        move.l  (HASH_ISR_NUM, a1), d0            ; get isr num
        tst.l   d0
        beq.w   _isr_search_fail
        
        cmp.l   d0, d2
        beq.w   _isr_search_suc
        
        movea.l (HASH_ISR_NEXT, a1), a1           ; next vector in list
        tst.l   a1
        bne.w   _isr_search

        
_isr_search_fail:
        bra     _isr_run_default
_isr_search_suc:

//#ifdef  __ACF__
#if PSP_ABI == PSP_ABI_REG
        movea.l (HASH_ISR_DATA, a1), a0        ; IAR reg convention
#else
        move.l  (HASH_ISR_DATA, a1), -(sp)     ; push notifier data
#endif
        
        
        movea.l (HASH_ISR_ADDR, a1), a1               ; get ISR handler

#else
;       Find the 'C' isr to run:
        clr.l   d0
        move.w  (KD_LAST_USER_ISR_VECTOR+2,a0),d0
        beq.w   _isr_run_default             ; int table not installed
        cmp.l   d0,d1                        ; vector # too high??
        bgt.w   _isr_run_default             ; cmp calcs d0-d1 > 0 is bad
        move.w  (KD_FIRST_USER_ISR_VECTOR+2,a0),d0
        sub.l   d0,d1                        ; vector # too low??
        bge.b   _int_kernel_isr_vect_ok     ; cmp calcs d0-d1 < 0 is bad
        bra.w   _isr_run_default

;       we have the interrupt # relative to start of interrupt table
;       Each table entry is 12 bytes in size, so to get to the correct
;       int entry we have to multiply d1 by 12..
_int_kernel_isr_vect_ok:
        move.w  d1,d0
        mulu.w  #12,d0                        ; d0 is now 3*4 * int #

        movea.l (KD_INTERRUPT_TABLE_PTR,a0),a1 ; get the interrupt table pointer
        adda.l  d0,a1                         ; get address of entry in table
//#ifdef  __ACF__
#if PSP_ABI == PSP_ABI_REG
        movea.l (IT_APP_ISR_DATA,a1), a0        ; IAR reg convention
#else
        move.l  (IT_APP_ISR_DATA,a1), -(sp)     ; push notifier data
#endif

        movea.l (IT_APP_ISR,a1),a1             ; get ISR handler
#endif

_isr_execute:
        jsr     (a1)                         ; transfer to 'C' isr
#if PSP_ABI == PSP_ABI_STD
        adda.l  #4,sp                        ; get rid of  'C' isr parameter
#endif

ASM_PREFIX(_int_kernel_isr_return_internal):
        GET_KERNEL_DATA a0                   ; get kernel data addres
        move.w  (KD_DISABLE_SR,a0),d1         ; set level to kernel disable
        move.w  d1,sr                        ; set level to kernel disable

#if MQX_KERNEL_LOGGING
        btst.b  #0,(KD_LOG_CONTROL+3,a0)
        beq.b   no_logging4
        clr.l   d1
        move.w  (6,sp),d1                     ; get interrupt #
        lea     (-6*4,sp),sp                 ; make room on the stack
        movem.l d0/d1/d2/a0/a1/a2,(0,sp)      ; save registers used
        
     #if PSP_ABI == PSP_ABI_REG
        move.l  d1,d0
     #else
        move.l  d1,-(sp)                     ; Interrupt number
     #endif
        jsr     (ASM_PREFIX(_klog_isr_end_internal)).L
        
     #if PSP_ABI == PSP_ABI_REG
        movem.l (0,sp),d0/d1/d2/a0/a1/a2      ; restore scratch registers
        adda.l  #6*4,sp
     #else
        movem.l (4,sp),d0/d1/d2/a0/a1/a2      ; restore scratch registers
        adda.l  #7*4,sp
     #endif
no_logging4:
#endif

        move.l  (IC_PREV_CONTEXT,sp),(KD_INTERRUPT_CONTEXT_PTR,a0) ; get previous ISR context
        addq.l  #IC_STRUCT_SIZE-8,sp         ; remove the interrupt context
        addq.l  #8,sp                        ;

        clr.l   d0
        move.w  (KD_IN_ISR,a0),d0             ; out of 1 ISR
        subq.l   #1,d0
        move.w  d0,(KD_IN_ISR,a0)
        bne.w   _isr_nested_interrupt

;       Completed all nested interrupts
        movea.l (KD_ACTIVE_PTR,a0),a1         ; Get TD pointer
        movea.l (TD_STACK_PTR,a1),sp          ; Return to old stack

;       Check SR value on stack for HW nested stack frames
        clr.l   d0
        clr.l   d1
        move.b  (FLINT_SR_OFFSET,sp),d0
        andi.l  #0x27,d0
        move.b  (KD_ACTIVE_SR,a0),d1          ; Just check lower 3 bits!!
        cmp.l   d1,d0
        bne.b   _isr_nested_interrupt

;       Check to see if reschedule necessary
        move.w  (TD_FLAGS+2,a1),d0
        andi.l  #PREEMPTION_DISABLED,d0
        bne.b   _isr_nested_interrupt        ; task has preemption disabled

;        If a different TD at head of current readyq, then we need to run
;        the scheduler
        move.l  a1,d0
        movea.l (KD_CURRENT_READY_Q,a0),a1
        cmp.l   (a1),d0
        bne.w   _isr_context_switch          ; diffent td at head of readyq

        movea.l (KD_ACTIVE_PTR,a0),a1         ; Get TD pointer
#if PSP_HAS_DSP
        RESTORE_DSP_REGISTERS a1,a2          ; kills d0
#endif

        ; save other registers if exception_isr is installed
        clr.l   d0
        btst.b  d0,(KD_FLAGS + 1, a0)            ; check for bit0 of 16 bit #
        bne.b   _isr_restore_extra
        RESTORE_ISR_REGISTERS
        rte
        ASM_CONST16(0x0000)

_isr_nested_interrupt:
        movea.l (KD_ACTIVE_PTR,a0),a1         ; Get TD pointer

        ; save other registers if exception_isr is installed
        clr.l   d0
        btst.b  d0,(KD_FLAGS+1,a0)            ; check for bit0 of 16 bit #
        bne.b   _isr_restore_extra
        RESTORE_ISR_REGISTERS
        rte
        ASM_CONST16(0x0000)

_isr_restore_extra:                          ; restore extra registers
        RESTORE_REST_ISR_REGISTERS
        rte
        ASM_CONST16(0x0000)

_isr_run_default:
        clr.l   d1
        move.w  (6,sp),d1                     ; get interrupt # in context
        
#if PSP_ABI == PSP_ABI_REG
        move.l  d1,d0
#else
        move.l  d1,-(sp)                     ; d1 has vector #
#endif

        movea.l (KD_DEFAULT_ISR,a0),a1
        bra.w   _isr_execute

_isr_context_switch:
        ; No need to save other registers if exception_isr is installed
        clr.l   d0
        btst.b  d0,(KD_FLAGS+1,a0)            ; check for bit0 of 16 bit #
        bne.b   _isr_no_save_extra_again
        SAVE_REST_ISR_REGISTERS
_isr_no_save_extra_again:
        GET_KERNEL_DATA a2
        bra.w   __sched_internal

; Do not include the cache code if the CPU has no cache
#if PSP_HAS_CODE_CACHE || PSP_HAS_DATA_CACHE
     
;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _icache_invalidate_mlines
; Returned Value   : 
; Comments         :
;   This function invalidate m lines from instruction cache
;
;END*----------------------------------------------------------------------
     
        ASM_PUBLIC(_icache_invalidate_mlines)
ASM_PREFIX(_icache_invalidate_mlines):
        link     a6,#0
        
#if PSP_ABI == PSP_ABI_STD
        movea.l  8(a6),a1       ; Base address to begin invalidating
        move.l   12(a6),d2      ; Number of lines to invalidate
        move.l   16(a6),d1      ; Size of one cache line
#else
        move.l  a0, a1
        move.l  d0, d2
#endif

        moveq    #0,d0          ; for (d0 = 0; d0 < mlines; d0++)
        bra.s    test_mlines
invalidate_another_line:

#if PSP_CACHE_SPLIT
;        ASM_CONST16(0xF4A9)      ; cpushl ic,(a1)
        cpushl  ic, (a1)
#else
;       ASM_CONST16(0xF4E9)         ; cpushl bc,(a1)
        cpushl  bc, (a1)
#endif

        adda.l   d1,a1
        addq.l   #1,d0
test_mlines:
        cmp.l    d2,d0
        bcs.s    invalidate_another_line
        unlk     a6
        rts

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _dcache_flush_mlines
; Returned Value   : 
; Comments         :
;   This function flush data cache m lines
;
;END*----------------------------------------------------------------------

#if 0
        ASM_PUBLIC(_dcache_flush_mlines)
ASM_PREFIX(_dcache_flush_mlines):
        link     a6,#0
        
#if PSP_ABI == PSP_ABI_STD
        movea.l  (8,a6),a1       ; Base address to begin invalidating
        move.l   (12,a6),d2      ; Number of lines to flush
        move.l   (16,a6),d1      ; Size of one cache line
#else
        move.l  a0, a1
        move.l  d0, d2
#endif
        
        moveq    #0,d0          ; for (d0 = 0; d0 < mlines; d0++)
        bra.s    (test_flush_mlines)
flush_another_line:

#if PSP_CACHE_SPLIT
;        ASM_CONST16(0xF469)    ; cpushl dc,(a1)
        cpushl dc, (a1)
#else
;        ASM_CONST16(0xF4E9)    ; cpushl bc,(a1)
        cpushl bc, (a1)
#endif
        
        adda.l   d1,a1
        addq.l   #1,d0
test_flush_mlines:
        cmp.l    d2,d0
        bcs.s    flush_another_line
        unlk     a6
        rts
#endif        
        
;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _psp_dcache_flush_all
; Comments         :
;    This function will flush the entire data cache.
;
;END*------------------------------------------------------------------------
        ASM_PUBLIC(_psp_dcache_flush_all)
ASM_PREFIX(_psp_dcache_flush_all):
    link     a6,#0
    
#if PSP_ABI == PSP_ABI_STD
    lea      -16(a7),a7         ;Allocate frame to store D3,D4,D5
    movem.l  d3-d5,4(a7)        ;Cannot trash D3,D4,D5
    move.l    8(a6),d2          ;param1 => int cache_line_size
    move.l   12(a6),d3          ;param2 => int num_ways
    move.l   16(a6),d4          ;param3 => int num_sets
#else
    lea      -16(a7),a7         ;Allocate frame to store D3,D4,D5
    movem.l  d3-d5, 4(a7)        ;Cannot trash D3,D4,D5

    move.l  d2, d4
    move.l  d1, d3
    move.l  d0, d2
#endif

    nop                         ;synchronize/flush store buffer

    move.w   sr,d5              ;** Disable interrupts
    move.l   #0x2700,d0         ;Cannot have the data cache changing
    move.w   d0,sr

    moveq.l  #0,d0              ;initialize way counter
    moveq.l  #0,d1              ;initialize set counter
    move.l   d0,a0              ;initialize cpushl pointer
setloop:
#if PSP_CACHE_SPLIT
    cpushl   dc, (a0)           ;push cache line a0
#else
    cpushl   bc, (a0)           ;push cache line a0
#endif

    add.l    d2,a0              ;increment set index by 1
    addq.l   #1,d1              ;increment set counter
    cmp.l    d4,d1              ;are sets for this way done?
    blt      setloop
    moveq.l  #0,d1              ;set counter to zero again
    addq.l   #1,d0              ;increment to next way
    move.l   d0,a0              ;set = 0, way = d0
    cmp.l    d3,d0              ;flushed all the ways?
    blt      setloop

    move.w   d5,sr              ;** Re-enable interrupts
    movem.l  4(a7),d3-d5
    unlk     a6
    rts

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _dcache_flush
; Comments         :
;    This function will flush the data cache
;
;END*------------------------------------------------------------------------
        ASM_PUBLIC(_dcache_flush)
ASM_PREFIX(_dcache_flush):
    
#if PSP_ABI == PSP_ABI_STD
        movea.l (4, sp), a0         ; start address
        move.l  (8, sp), d0         ; line cnt
        move.l  (12, sp), d1        ; cache_line_size
#endif

        move.w  sr, d2              ; disable interrupts
        ;lea     -8(sp), sp          ; allocate frame to store SR
        move.l  d2, -(sp)
        
        move.l  #0x2700, d2         ;Cannot have the data cache changing
        move.w  d2, sr

        
        move.l  a0, d2
        and.l   #0xfffffff0, d2
        movea.l d2, a0

        moveq   #0, d2              ; for (d2 = 0; d2 < mlines (d0); d2++)
        bra.s   dcache_flush_test

dcache_flush_line:
        movea.l a0, a1
        
        // TODO add support for split cache
#if PSP_CACHE_SPLIT        
        cpushl  dc, (a1)
        
        adda.l  #1, a1
        cpushl  dc, (a1)

        adda.l  #1, a1
        cpushl  dc, (a1)

        adda.l  #1, a1
        cpushl  dc, (a1)
#else
        cpushl  bc, (a1)
        
        adda.l  #1, a1
        cpushl  bc, (a1)

        adda.l  #1, a1
        cpushl  bc, (a1)

        adda.l  #1, a1
        cpushl  bc, (a1)
#endif
        
        adda.l  d1, a0
        addq.l  #1, d2
dcache_flush_test:
        cmp.l   d0, d2
        bcs.s   dcache_flush_line
        
        move.l (sp)+, d2
        move.w  d2, sr              ; enable interrupts
        
        rts

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : _dcache_flush_phy
; Returned Value   : 
; Comments         :
;   This function flush data cache by physical address (mcf54455 feature)
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(_dcache_flush_phy)
ASM_PREFIX(_dcache_flush_phy):

#if PSP_ABI == PSP_ABI_STD
        movea.l (4, sp), a0         ; Base address to begin invalidating
        move.l  (8, sp), d0         ; Number of lines to flush
        move.l  (12, sp), d1        ; Size of one cache line
#endif
        
        moveq   #0, d2              ; for (d2 = 0; d2 < mlines (d0); d2++)
        bra.s   dcache_flush_phy_test

dcache_flush_phy_line:
        cpushl  dc, (a0)
        
        adda.l  d1, a0
        addq.l  #1, d2
dcache_flush_phy_test:
        cmp.l   d0, d2
        bcs.s   dcache_flush_phy_line

        rts

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : __psp_enable_acrs
; Returned Value   :
; Comments         :
;   This function enable ACR registers
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(__psp_enable_acrs)
ASM_PREFIX(__psp_enable_acrs):
    link     a6,#0
#if PSP_ABI == PSP_ABI_STD
    movea.l  (8,a6),a1           ; [IN] int ACR_VAL[4] param
#else
    movea.l     a0, a1
#endif
    
    
    move.l   (a1),d0
    movec    d0,acr0
    move.l   4(a1),d0
    movec    d0,acr1
 #if PSP_ACR_CNT == 4
    move.l   8(a1),d0
    movec    d0,acr2
    move.l   12(a1),d0
    movec    d0,acr3
 #endif // PSP_ACR_CNT == 4
    nop                         ; Sync pipeline
    unlk     a6
    rts

;FUNCTION*-------------------------------------------------------------------
;
; Function Name    : __psp_clear_acrs
; Returned Value   : 
; Comments         :
;   This function clear ACR registers
;
;END*----------------------------------------------------------------------

        ASM_PUBLIC(__psp_clear_acrs)
ASM_PREFIX(__psp_clear_acrs):
    moveq.l  #0,d0
    movec    d0,ACR0
    movec    d0,ACR1
    
#if PSP_ACR_CNT == 4
    movec    d0,ACR2
    movec    d0,ACR3
#endif // PSP_ACR_CNT == 4

    nop                         ; Sync pipeline
    rts

#endif
     
