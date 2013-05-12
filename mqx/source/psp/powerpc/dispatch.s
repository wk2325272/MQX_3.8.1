/*
#*HEADER********************************************************************
#* 
#* Copyright (c) 2008-2009 Freescale Semiconductor;
#* All Rights Reserved                       
#*
#* Copyright (c) 1989-2008 ARC International;
#* All Rights Reserved
#*
#*************************************************************************** 
#*
#* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
#* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
#* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
#* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
#* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
#* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
#* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
#* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
#* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
#* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
#* THE POSSIBILITY OF SUCH DAMAGE.
#*
#**************************************************************************
#*
#* $FileName: dispatch.s$
#* $Version : 3.8.10.1$
#* $Date    : Mar-28-2012$
#*
#* Comments:
#*
#*   This assembler file contains functions for task scheduling 
#*   use with the mqx assembler files
#**END***********************************************************************
*/

        .set CHECK_STACK_LIMIT,0
        .set ADDITIONAL_DEBUG_PARAMS,0

#define __ASM__
   #include "psp_cpudef.h"
   #include "mqx_cnfg.h"
   #include "psp_cpu.h"
   #include "mqx.h"
   #include "mqx_prv.h"
#undef __ASM__

        
#include "asm_offsets.h"

        .text
        .section DISPATCH,32,1,6
#include "psp_prv_asm.h"


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _task_block
## Comments         :
##
## This function is called by a task to save it's context and remove
## itself from its ready queue. The next runnable task in the ready queues
## is made active and dispatched.  The state of the task is marked as being
## blocked.
##
##END*------------------------------------------------------------------------
*/

        .globl    _task_block
        SET_FUNCTION_ALIGNMENT

_task_block:
        SAVE_ALL_REGISTERS              ## save context of current task

        GET_KERNEL_DATA r15             ## get the address of kernel data
        lwz     r16,KD_ACTIVE_PTR(r15)  ## get active td into r16
        DISABLE_INTS r3                 ## may use r3
        stw     r1,TD_STACK_PTR(r16)    ## save stack pointer

        KLOG    r15,_klog_block_internal ## kernel log this function

        lwz     r3,TD_STATE(r16)        ## set the blocked bit
##       Remove the active task from the ready queue.
        lwz     r5,TD_TD_PREV(r16)
        lwz     r6,TD_TD_NEXT(r16)
        ori     r3,r3,1
        stw     r3,TD_STATE(r16)
        stw     r6,TD_TD_NEXT(r5)
        stw     r5,TD_TD_PREV(r6)

        b       _sched_internal     ## Search for the next task in the ready q


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _sched_start_internal
## Comments         :
##   This function is called from _mqx in order to start the task
## scheduler running.
##
##END*------------------------------------------------------------------------
*/

        .globl    _sched_start_internal

_sched_start_internal:
        GET_KERNEL_DATA r15
        //lhz      r3,KD_CONFIG2(r15)
        //li       r4,PSP_CNFG
        //or       r3,r3,r4
        //sth      r3,KD_CONFIG2(r15)
#if defined(PSP_AMCC401)||defined(PSP_AMCC403)||defined(PSP_AMCC405)||defined(PSP_AMCC480)
        li       r3, 0
        mtspr    srr2, r3
        mtspr    srr3, r3
#endif
#if (defined(PSP_G2_LE_CORE) || defined(PSP_E200_CORE) || defined(PSP_E300_CORE) || defined(PSP_E500_CORE) || defined(PSP_AMCC450))
        li       r3, 0
        mtspr    csrr0, r3
        mtspr    csrr1, r3
#endif
        b        _sched_internal


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _sched_check_scheduler_internal
## Comments         :
##    This function is called to check whether scheduling is necessary
## It falls through into the next function
##
## Function Name    : _sched_execute_scheduler_internal
## Comments         :
##    This function is called by a task to save its context.
## This is usually done when the task has already been removed from the
## ready queue, and is on some other queue.
## However it can also be called so that the current tasks context is saved
## so that the scheduler can run (in case of a higher priority task being
## available.
##
## It falls through to the next function
##
## Function Name    : _sched_internal
## Comments         :
##   This function is the actual task scheduler... IT IS NOT CALLABLE from C
## rather, other assembler functions in this file jump to it.


##
## Search for the next task in the ready queue.
##
##END *----------------------------------------------------------------------
*/

        .globl    _sched_check_scheduler_internal
        .globl    _sched_execute_scheduler_internal
        .globl    _sched_context_switch_internal
        .globl    _sched_run_internal

_sched_run_internal:
        GET_KERNEL_DATA r15             ## Get address of kernel data
        b       _sched_internal

_sched_check_return:
        blr

        SET_FUNCTION_ALIGNMENT
_sched_check_scheduler_internal:
        GET_KERNEL_DATA r3              ## Get address of kernel data
        lhz     r4,KD_IN_ISR(r3)
        or.     r4,r4,r4
        bne     _sched_check_return     ## We are in an ISR, so return
        lwz     r4,KD_CURRENT_READY_Q(r3)
        lwz     r3,KD_ACTIVE_PTR(r3)
        lwz     r3,TD_MY_QUEUE(r3)
        cmplw   r3,r4
        beq     _sched_check_return     ## Current task is still the active task

_sched_execute_scheduler_internal:
        SAVE_ALL_REGISTERS              ## Save the context of the active task.

        GET_KERNEL_DATA r15             ## Get address of kernel data
        lwz     r16,KD_ACTIVE_PTR(r15)  ## get active td into r16
        DISABLE_INTS r3                 ## may use r3
        stw     r1,TD_STACK_PTR(r16)    ## save stack pointer

        KLOG    r15,_klog_execute_scheduler_internal ## kernel log this function

##
##   MAIN TASK SCHEDULER CODE
##   Arrive here from internal functions with r15 already set

_sched_internal:
        lwz     r17,KD_CURRENT_READY_Q(r15)  ## get current ready q

find_nonempty_queue:
        lwz     r16,0(r17)                ## address of first td
        cmp     r16,r17                   ## ready_q record itself?
        bne     activate
        lwz     r17,RQ_NEXT_Q(r17)        ## try next queue
        cmpi    r17,0
        bne+    find_nonempty_queue

## falls through to here
## No task available to run
no_one_to_run:
##       Set up a system td as running and wait for an interrupt
        addi    r16,r15,KD_SYSTEM_TD
        stw     r16,KD_ACTIVE_PTR(r15)
        GET_SYSTEM_STACK r1
        addi    r1,r1,-16
        ENABLE_INTS r3

## Wait for a while
        addi    r5,r0,0x1000
waiting:
        subi    r5,r5,1
        cmpi    r5,0
        bne+    waiting
        DISABLE_INTS r3
        GET_KERNEL_DATA r15             ## Get address of kernel data
        lwz     r17,KD_READY_Q_LIST(r15)  ## get ready just in case
        b       find_nonempty_queue

##----------------------------------------------------------

activate:

## Vector registers context savings if required
##
        lwz      r14,TD_FLAGS(r16)       ## flags kept in R14

        #if MQXCFG_ENABLE_DSP && PSP_HAS_DSP

## We now have the new task to run, check if it needs the
## dsp registers co-processor
        andi.    r4,r14,MQX_DSP_TASK
        bne      save_dsp_registers
save_dsp_registers_done:
       #endif

       #if MQXCFG_ENABLE_FP && PSP_HAS_FPU
## We now have the new task to run, check if it needs the
## floating point co-processor
        andi.    r4,r14,MQX_FLOATING_POINT_TASK
        bne      save_fp_registers
save_fp_registers_done:
        #endif

        #if MQX_PROFILING_ENABLE
        PROF_SAVE
        #endif

        #if MQX_RUN_TIME_ERR_CHECK_ENABLE
        RTEC_SAVE
        #endif

        #if MQXCFG_INCLUDE_MMU_SUPPORT
        lwz     r19,KD_ACTIVE_PTR(r15)
        lwz     r3,TD_FLAGS(r19)
        andi.   r3,r3,TASK_MMU_CONTEXT_EXISTS  ## TODO: Fix this
        beq     no_mmu_context_to_clear
        subi    r1,r1,8        ## Make room for Link Register and SP
        bl      _mmu_reset_vcontext_internal
        addi    r1,r1,8
no_mmu_context_to_clear:
        #endif

        lhz      r18,TD_TASK_SR(r16)
        stw      r17,KD_CURRENT_READY_Q(r15)
        stw      r16,KD_ACTIVE_PTR(r15)
        sth      r18,KD_ACTIVE_SR(r15)
        lwz      r1,TD_STACK_PTR(r16)     ## restore stack pointer

        #if MQXCFG_INCLUDE_MMU_SUPPORT
        andi.    r3,r14,TASK_MMU_CONTEXT_EXISTS   ## TODO: Fix this
        beq      no_mmu_context_to_set
        subi     r1,r1,8        ## Make room for Link Register and SP
        bl       _mmu_set_vcontext_internal
        addi     r1,r1,8
no_mmu_context_to_set:
        #endif

        #if MQX_RUN_TIME_ERR_CHECK_ENABLE
        RTEC_RESTORE
        #endif

        #if MQX_PROFILING_ENABLE
        PROF_RESTORE
        #endif

        KLOG     r15,_klog_context_switch_internal

        RESTORE_ALL_REGISTERS             ## restore task context
_sched_context_switch_internal:
        rfi

/*
##--------------------------------------------------------
##
## The following code manages the saving and restoring of
## the DSP registers.  The task about to become active has
## been flagged as a DSP task.  If there is a currently
## active DSP task, the DSP registers must be saved onto
## its stack.
*/

        #if MQXCFG_ENABLE_DSP && PSP_HAS_DSP

save_dsp_registers:
## See if a dsp task is currently active
        lwz      r19,KD_DSP_ACTIVE_PTR(r15)
        cmpi     r19,0
        beq      restore_dsp_context

## See if the dsp task is in fact the task being scheduled
## So, no need to save and restore pointers
        cmp      r19,r16
        beq      no_dsp_registers_to_restore

## Stop the VRU unit, and save the internal
## dsp registers on the stack of the DSP task
        lwz      r3,TD_DSP_CONTEXT_PTR(r19)
        SAVE_DSP_REGISTERS
        lwz      r3,TD_FLAGS(r19)
        ori      r3,r3,TASK_DSP_CONTEXT_SAVED
        stw      r3,TD_FLAGS(r19) ## Indicate DSP context saved

restore_dsp_context:
## Restore the context of the current DSP task if necessary
        stw      r16,KD_DSP_ACTIVE_PTR(r15)
        andi.    r3,r14,TASK_DSP_CONTEXT_SAVED
        beq      no_dsp_registers_to_restore

## Now restore the DSP context
        lwz      r3,TD_DSP_CONTEXT_PTR(r16)
        RESTORE_DSP_REGISTERS

no_dsp_registers_to_restore:
        li       r3,TASK_DSP_CONTEXT_SAVED
        andc     r14,r14,r3
        stw      r14,TD_FLAGS(r16)
        b        save_dsp_registers_done
        #endif

/*
##--------------------------------------------------------
##
## The following code manages the saving and restoring of
## the FP registers.  The task about to become active has
## been flagged as a FP task.  If there is a currently
## active FP task, the FP registers must be saved onto
## its stack.
*/

        #if MQXCFG_ENABLE_FP && PSP_HAS_FPU
save_fp_registers:
## See if a floating point task is currently active
        lwz      r19,KD_FP_ACTIVE_PTR(r15)
        cmpi     r19,0
        beq      restore_fp_context

## See if the floating point task is in fact the task being scheduled
## So, no need to save and restore pointers
        cmp      r19,r16
        beq      no_fp_registers_to_restore

## Stop the floating point unit, and save the internal
## floating point registers on the stack of the FP task
        lwz      r1,TD_STACK_PTR(r19)
        lwz      r3,TD_FLAGS(r19)

        subi     r1,r1,FP_REGISTERS_SIZE  ## back up stack
        SAVE_FLOATING_POINT_REGISTERS

## Save current stack, and indicate in TD, that FP context saved
        stw      r1,TD_STACK_PTR(r19)

        ori      r3,r3,TASK_FLOATING_POINT_CONTEXT_SAVED
        stw      r3,TD_FLAGS(r19)

restore_fp_context:
## Restore the context of the current FP task if necessary
        lwz      r1,TD_STACK_PTR(r16)
        stw      r16,KD_FP_ACTIVE_PTR(r15)
        andi.    r3,r14,TASK_FLOATING_POINT_CONTEXT_SAVED
        beq      no_fp_registers_to_restore

## Now restore the floating point context
        RESTORE_FLOATING_POINT_REGISTERS
        addi     r1,r1,FP_REGISTERS_SIZE

        stw      r1,TD_STACK_PTR(r16)     ## resave stack pointer
no_fp_registers_to_restore:
        li       r3,TASK_FLOATING_POINT_CONTEXT_SAVED
        andc     r14,r14,r3
        stw      r14,TD_FLAGS(r16)
        b        save_fp_registers_done
        #endif


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _psp_save_fp_context_internal
## Returned Value   : none
## Comments         :
##   This function saves the floating point context for the
## current floating point task
##
## THIS FUNCTION MUST BE CALLED DISABLED
##
##END*----------------------------------------------------------------------
*/

        .globl _psp_save_fp_context_internal

        SET_FUNCTION_ALIGNMENT
_psp_save_fp_context_internal:
        #if MQXCFG_ENABLE_FP && PSP_HAS_FPU
##       Stop the floating point unit, and save the internal
##       floating point registers on the stack of the FP task
        GET_KERNEL_DATA r3
        lwz      r4,KD_FP_ACTIVE_PTR(r3)
        cmpi     r4,0
        beq      _psp_save_fp_context_done ## no FP task

        lwz      r5,TD_FLAGS(r4)
        andi.    r6,r5,TASK_FLOATING_POINT_CONTEXT_SAVED
        bne      _psp_save_fp_context_done ## FP registers already saved

        mr       r7,r1
        lwz      r1,TD_STACK_PTR(r4)

        subi     r1,r1,FP_REGISTERS_SIZE  ## back up stack
        SAVE_FLOATING_POINT_REGISTERS

##       Save current stack, and indicate in TD, that FP context saved
        stw      r1,TD_STACK_PTR(r4)

##       restore stack pointer
        mr       r1,r7

        ori      r5,r5,TASK_FLOATING_POINT_CONTEXT_SAVED
        stw      r5,TD_FLAGS(r4)

_psp_save_fp_context_done:
        #endif
        blr


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _psp_save_dsp_context_internal
## Returned Value   : none
## Comments         :
##   This function saves the vector registers context for the
## current floating point task
##
## THIS FUNCTION MUST BE CALLED DISABLED
##
##END*----------------------------------------------------------------------
*/

        .globl _psp_save_dsp_context_internal

        SET_FUNCTION_ALIGNMENT
_psp_save_dsp_context_internal:
        #if MQXCFG_ENABLE_DSP && PSP_HAS_DSP

##       Stop the dsp unit, and save the internal
##       vector registers for the DSP task
        GET_KERNEL_DATA r3
        lwz      r4,KD_DSP_ACTIVE_PTR(r3)
        cmpi     r4,0
        beq      _psp_save_dsp_context_done ## no DSP task

        lwz      r5,TD_FLAGS(r4)
        andi.    r3,r5,TASK_DSP_CONTEXT_SAVED
        bne      _psp_save_dsp_context_done ## DSP registers already saved

        lwz      r3,TD_DSP_CONTEXT_PTR(r4)
        SAVE_DSP_REGISTERS

        ori      r5,r5,TASK_DSP_CONTEXT_SAVED
        stw      r5,TD_FLAGS(r4)

_psp_save_dsp_context_done:
        #endif
        blr


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _mem_test_and_set
## Returned Value   : previous value of location
## Comments         :
##   This function tests a location, and if 0, sets it to 1
## it returns the previous value
##
##END*----------------------------------------------------------------------
*/

        .globl   _mem_test_and_set

        SET_FUNCTION_ALIGNMENT
_mem_test_and_set:
   mr      r4,r3

   li      r6,-4       ## 0xfffc
   and.    r4,r4,r6

   xor     r6,r6,r6
   andi.   r3,r3,0x3
   cmpi    r3,0
   beq     zero
   cmpi    r3,1
   beq     one
   cmpi    r3,2
   beq     two
   cmpi    r3,3
   beq     three

zero:
   oris    r6,r6,0x8000      ## find bit
   b       testit

one:
   oris    r6,r6,0x80        ## find bit
   b       testit

two:
   ori     r6,r6,0x8000      ## find bit
   b       testit

three:
   ori     r6,r6,0x80        ## find bit
   b       testit

testit:
   DISABLE_INTS r8
   lwarx   r5,0,r4
   mr      r7,r5
   and.    r5,r5,r6
   bne     bit_was_set

   or      r7,r7,r6
   stwcx.  r7,0,r4
   ENABLE_INTS r8
   bne-    _mem_test_and_set  ## Reservation failed
   xor     r3,r3,r3
   blr

bit_was_set:
   ENABLE_INTS r8
   xor     r3,r3,r3
   ori     r3,r3,0x80
   blr


/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _int_kernel_isr()
## Comments         :
##   This is the assembler level interrupt isr that intercepts all
## interrupts. (When installed on a particular vector).
##
##   Enough registers are saved so that a 'C' isr can be called.
##   If the current stack is not the interrupt stack, then the stack is
##   switched over.
##
##   0 is pushed onto the stack, and a LINK A6 is done.
##   This effectively is what a function call does, BUT the return address is 0.
##   This will allow a 'C'function to walk back on the stack to find where this
##   'interrupt frame' exists.
##
##   An interrupt 'context' is created on the stack, thus allowing for proper
##   operation of MQX 'C' functions that access the error code and _int_enable
##   and _int_disable
##
##   Then interrupt handlers are checked for.  If they have not been installed,
##   or the current ISR number is out the range of installed handlers,
##   the DEFAULT_ISR is called.
##
##   If they have been installed then if a user 'C' hander has not been installed
##   for this vector, the DEFAULT_ISR is called.
##
##   After returning from the call to the 'C' isr the following is checked:
##   If this is a nested ISR, then we do an interrupt return.
##   If the current task is still the highest priority running task, we
##   do an interrupt return.
##   Otherwise we must save the full context for the current task, and
##   enter the scheduler.
##
##END*------------------------------------------------------------------------
*/

        .globl    _int_kernel_isr
        .globl    _int_kernel_isr_return_internal

        SET_FUNCTION_ALIGNMENT
_int_kernel_isr:
        SAVE_ISR_REGISTERS                ## save the registers

        GET_KERNEL_DATA r5                   ## KD in r5

        ## Save other registers if exception_isr is installed
        lhz     r4,KD_FLAGS(r5)
        andi.   r4,r4,1
        beq     no_save_extra
        SAVE_REST_ISR_REGISTERS
no_save_extra:

        ## Check stack bounds for overflow/underflow.
        ## This can be nice to have around as your developing
        ## your application.
        ##
        ## NOTE: if setjmp/longjmp used in _mqx() then
        ##       the stack pointer will be reset to the
        ##       startup stack without the TD pointer
        ##       being reset and we will always get an
        ##       invalid error at _mqx_exit().
        #if CHECK_STACK_LIMIT
        lhz     r7,KD_IN_ISR(r5)             ## Check for swap to interrupt stack
        or.     r7,r7,r7
        bne     aa2
        lwz     r6,KD_ACTIVE_PTR(r5)         ## get TD pointer
        lwz     r7,TD_STACK_BASE(r6)         ## stack start
        cmp     cr0,r1,r7
        ble     cr0,aa1
        b       stack_error                  ## set breakpoint here
aa1:    lwz     r7,TD_STACK_LIMIT(r6)        ## stack start
        cmp     cr0,r1,r7
        bgt     cr0,aa2
        b       stack_error                  ## set breakpoint here
aa2:
        #endif

        xor     r4,r4,r4                     ## get zero into r4

        lhz     r7,KD_IN_ISR(r5)             ## Check for swap to interrupt stack
        or.     r7,r7,r7
        bne     _isr_no_stack_swap

        mr      r8,r1                        ## swap stacks, and remember old
        lwz     r6,KD_ACTIVE_PTR(r5)         ## get TD pointer
        lwz     r1,KD_INTERRUPT_STACK_PTR(r5)
        addi    r1,r1,-16
        stw     r8,TD_STACK_PTR(r6)          ## and save the stack pointer
        stw     r4,KD_INTERRUPT_CONTEXT_PTR(r5) ## provide an isr context base

_isr_no_stack_swap:
        addi    r7,r7,1
        sth     r7,KD_IN_ISR(r5)             ## Indicate that ISR running

        ## Provide a "BASE" for this interrupt stack frame
        subi    r1,r1,IF_STRUCT_SIZE

        stw     r7,IC_ENABLE_SR+IF_INTERRUPT_CONTEXT(r1)
        stw     r0,IF_EXCEPTION_NUMBER(r1)      ## save interrupt number
        stw     r4,IF_FAKE_RETURN_ADDRESS(r1)   ## A Zero Stack Frame return addr
        stw     r4,IF_FRAME_POINTER(r1)
        addi    r7,r1,IF_FRAME_POINTER
        stw     r4,IC_ERROR_CODE+IF_INTERRUPT_CONTEXT(r1)
        stw     r7,IF_STACK_POINTER(r1)

        lwz     r8,KD_INTERRUPT_CONTEXT_PTR(r5)
        addi    r7,r1,IF_INTERRUPT_CONTEXT
        stw     r8,IC_PREV_CONTEXT+IF_INTERRUPT_CONTEXT(r1)
        stw     r7,KD_INTERRUPT_CONTEXT_PTR(r5) ## store new context pointer

        #if MQX_KERNEL_LOGGING
        lwz     r7,KD_LOG_CONTROL(r5)
        andi.   r7,r7,1
        beq     no_logging3
        mr      r3,r0
        subi    r1,r1,8        ## Make room for Link Register and SP
        bl      _klog_isr_start_internal
        addi    r1,r1,8
        lwz     r0,IF_EXCEPTION_NUMBER(r1)
        GET_KERNEL_DATA r5                   ## KD in r5
no_logging3:
        #endif

##       Find the 'C' isr to run:
        lwz     r7,KD_LAST_USER_ISR_VECTOR(r5)
        or.     r7,r7,r7
        beq     _isr_run_default             ## int table not installed
        cmplw   r0,r7                        ## vector # too high??
        bgt     _isr_run_default             ## is r0 greater than r7?
        lwz     r7,KD_FIRST_USER_ISR_VECTOR(r5)
        subf.   r0,r7,r0                     ## vector # too low??
        blt     _isr_run_default             ## sub calcs r0-r7 < 0 is bad

##       we have the interrupt number in r0 to index into the interrupt
##       table.  Each table entry is 12 bytes in size, so to get to the correct
##       int entry I have to multiply r0 by 12 so we will do:
##              r0 = r0 * 4, r0 = r0 + r0 + r0
        slwi    r10,r0,2
        mr      r7,r10
        add     r10,r10,r7
        add     r10,r10,r7

        lwz     r7,KD_INTERRUPT_TABLE_PTR(r5) ## get the interrupt table pointer
        add     r7,r7,r10                     ## get address of entry in table
        lwz     r9,IT_APP_ISR(r7)             ## get ISR handler
        lwz     r3,IT_APP_ISR_DATA(r7)        ## get notifier data

        #if ADDITIONAL_DEBUG_PARAMS
        ## Enabling this code lets you breakpoint on the ISR function and
        ## see SRR0 + SRR1 in the 2nd & 3rd integer parameter registers
        lwz     r12,TD_STACK_PTR(r6)
        lwz     r4,0(r12)  ## SRR0 in r4
        lwz     r5,20(r12) ## SRR1 in r5
        #endif

_isr_execute:
        mtspr   ctr,r9
        bcctrl 20,0

_int_kernel_isr_return_internal:

        DISABLE_INTS r3                      ## may use r3

        GET_KERNEL_DATA r5                   ## KD in r5

        #if MQX_KERNEL_LOGGING
        lwz     r7,KD_LOG_CONTROL(r5)
        andi.   r7,r7,1
        beq     no_logging4
        lwz     r3,IF_EXCEPTION_NUMBER(r1)
        subi    r1,r1,8        ## Make room for Link Register and SP
        bl      _klog_isr_end_internal
        addi    r1,r1,8
        GET_KERNEL_DATA r5                   ## KD in r5
no_logging4:
        #endif

## clean up stack
        lwz     r8,IC_PREV_CONTEXT+IF_INTERRUPT_CONTEXT(r1)
        stw     r8,KD_INTERRUPT_CONTEXT_PTR(r5) ## get previous ISR context
        addi    r1,r1,IF_STRUCT_SIZE         ## remove the interrupt context

        lhz     r7,KD_IN_ISR(r5)             ## Out of 1 ISR
        subi    r7,r7,1
        sth     r7,KD_IN_ISR(r5)
        cmpi    r7,0
        bne     _int_kernel_isr_nested_interrupt

##       Completed all nested interrupts
        lwz     r6,KD_ACTIVE_PTR(r5)
        lwz     r1,TD_STACK_PTR(r6)          ## Return to old stack

##       ## Check to see if reschedule necessary
        lwz     r7,TD_FLAGS(r6)
        andi.   r7,r7,TASK_PREEMPTION_DISABLED
        bne     _int_kernel_isr_nested_interrupt

##       If a different TD at head of current readyq, then we need to run
##       the scheduler
        lwz     r7,KD_CURRENT_READY_Q(r5)
        lwz     r7,0(r7)
        cmp     r7,r6
        bne     _int_kernel_isr_context_switch

_int_kernel_isr_nested_interrupt:   ## Combine actions
        ## Restore other registers if exception_isr is installed
        lhz     r4,KD_FLAGS(r5)
        andi.   r4,r4,1
        beq     no_restore_extra
        RESTORE_REST_ISR_REGISTERS
no_restore_extra:

        RESTORE_ISR_REGISTERS
        rfi

_isr_run_default:
        lwz     r3,IF_EXCEPTION_NUMBER(r1) ## parameter is vector number
        lwz     r9,KD_DEFAULT_ISR(r5)
        b       _isr_execute

_int_kernel_isr_context_switch:
        ## No need to save other registers if exception_isr is installed
        lhz     r4,KD_FLAGS(r5)
        andi.   r4,r4,1
        bne     no_save_extra_again
        SAVE_REST_ISR_REGISTERS
no_save_extra_again:
        GET_KERNEL_DATA r15
        b       _sched_internal

#if CHECK_STACK_LIMIT
stack_error:
        lhz     r4,KD_FLAGS(r5)
        andi.   r4,r4,1
        bne     extra_already_saved
        SAVE_REST_ISR_REGISTERS
extra_already_saved:
        .extern _stack_error_exception
        lwz     r3,IF_EXCEPTION_NUMBER(r1) ## parameter is vector number
        bl      _stack_error_exception
        b       no_save_extra_again
#endif

##==============================================================================
*/
        .globl __psp_set_sr
__psp_set_sr:
        mtmsr r3
        blr

        .globl __psp_get_sr
__psp_get_sr:
        mfmsr r3
        blr

## Start CR 1112
#if (defined(PSP_G2_CORE)||defined(PSP_G2_LE_CORE)||defined(PSP_E300_CORE))  // (MQX_CPU==MPC7400)||(MQX_CPU==MPC750)||(MQX_CPU==MPC8240)||
        .globl _psp_get_seg_reg
_psp_get_seg_reg:
        mfsrin r3,r3
        blr

        .globl _psp_mtsrin
_psp_mtsrin:
        mtsrin r3,r4
        blr
#endif
## End CR 1112

        .globl _psp_get_dsisr
_psp_get_dsisr:
        mfspr r3,18
        blr

        .globl _psp_get_srr1
_psp_get_srr1:
        mfspr r3,27
        blr

        .globl _psp_get_dar
_psp_get_dar:
        mfspr r3,19
        blr

        .globl _psp_set_disable_sr
_psp_set_disable_sr:
        mfmsr  r3
        rlwinm r3,r3,0,17,15 ## Clear EE bit
#if (defined(PSP_G2_LE_CORE) || defined(PSP_E300_CORE))
        rlwinm r3,r3,0,25,23 ## Clear critical interrupts
#else if (defined(PSP_E500_CORE)||defined(PSP_E200_CORE))
        rlwinm r3,r3,0,15,13 ## Clear critical interrupts
#endif
        mtmsr  r3
        blr

        .globl _psp_set_enable_sr
_psp_set_enable_sr:
        mfmsr r3
#if (defined(PSP_G2_LE_CORE)||defined(PSP_E300_CORE))
        ori   r3,r3,0x8080 ## Enable external & critical interrupts
#else
        ori   r3,r3,0x8000 ## Enable external interrupts
#endif
#if (defined(PSP_E500_CORE)||defined(PSP_E200_CORE))
        oris  r3,r3,0x0002 ## Enable critical interrupts
#endif
        mtmsr r3
        blr

#if  ((MQX_CPU==PSP_CPU_MPC555)||(MQX_CPU==PSP_CPU_MPC566))
    ## No MMU
#elif  (MQX_CPU==PSP_CPU_MPC5554)
    ## No MMU
#elif  (MQX_CPU==PSP_CPU_MPC5674)
	## Different MMU 
#elif (MQX_CPU == PSP_CPU_MPC5645S)||(MQX_CPU == PSP_CPU_MPXD20))
	## Different MMU	
#elif (MQX_CPU == PSP_CPU_MPC5643L)||(MQX_CPU == PSP_CPU_MPXS20))
	## Different MMU 
#elif ((MQX_CPU == PSP_CPU_MPC5675K)||(MQX_CPU == PSP_CPU_MPXS30))
	## Different MMU 
#elif ((MQX_CPU == PSP_CPU_MPC5668G)||(MQX_CPU == PSP_CPU_MPXN20))
	## Different MMU 
#else
        .globl _psp_tlbie
_psp_tlbie:
        tlbie r3
        blr
#endif

        .globl _psp_get_r2
_psp_get_r2:
        mr r3,r2
        blr

        .globl _psp_get_r13
_psp_get_r13:
        mr r3,r13
        blr

        .globl _psp_get_sp
_psp_get_sp:
        mr r3,r1
        blr

        .globl _psp_set_sp
_psp_set_sp:
        mr r1,r3
        blr

        .globl _psp_goto
_psp_goto:
        mtctr r3
        bcctrl 20,0

#if 0 // ((MQX_CPU==PSP_CPU_MPC821)||(MQX_CPU==PSP_CPU_MPC823)||(MQX_CPU==PSP_CPU_MPC850)||(MQX_CPU==PSP_CPU_MPC855)||(MQX_CPU==PSP_CPU_MPC860)||(MQX_CPU==PSP_CPU_MPC555)||(MQX_CPU==PSP_CPU_MPC566)||(MQX_CPU==PSP_CPU_MPC875))
        .globl _psp_read_immr
_psp_read_immr:
        mfspr  r3,638
        blr
#endif

 #if 0 // (MQX_CPU==PSP_CPU_AMCC403)
        .globl _psp_get_exisr
_psp_get_exisr:
        mfexisr r3
        blr

        .globl _psp_get_pit
_psp_get_pit:
        mfpit r3
        blr

        .globl _psp_set_pit
_psp_set_pit:
        mtpit r3
        blr



#endif

##==============================================================================

#if (MQX_CPU==PSP_CPU_AMCC405)
## sprg4 base of mmu page tables
## sprg5 current tlb to replace
## sprg6 max tlb number
## sprg7 kernel data address

/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_itlb_miss_isr
## Comments         :
##   this function handles Instruction TLB misses when the MMU page tables
## are enabled
##
##END*------------------------------------------------------------------------
*/

       .section itlb_miss,32,1,6

       .globl _psp_itlb_miss_isr
       .globl _psp_itlb_miss_isr_end

_psp_itlb_miss_isr:
        mtspr    sprg0,r3
        mtspr    sprg1,r4
        mtspr    sprg2,r5
        mfcr     r5
        mtspr    sprg3,r5

        mfspr    r4,sprg4        ## Base of mmu page tables
        mfspr    r3,srr0
        rlwinm   r5,r3,12,20,29  ## Get L1 page offset
        lwzx     r5,r4,r5
        cmpi     0x0,r5,0x0
        beq-     itlb_not_found

        rlwinm   r3,r3,21,21,28  ## Get L2 page offset
        add      r3,r3,r5
        lwz      r5,0(r3)        ## Get tlb_hi
        cmpi     0x0,r5,0x0
        beq      itlb_not_found

        mfspr    r4,sprg5        ## tlb counter
        tlbwehi  r5,r4
        lwz      r5,4(r3)        ## Get tlb_lo
        tlbwelo  r5,r4

        addi     r4,r4,1
        mfspr    r3,sprg6
        cmp      r4,r3
        blt      itlb_count_ok
        xor      r4,r4,r4
itlb_count_ok:
        mfspr    r3,sprg3
        mtcr     r3
        mtspr    sprg5,r4
        mfspr    r3,sprg0
        mfspr    r4,sprg1
        mfspr    r5,sprg2
        rfi

itlb_not_found:
        mfspr    r3,sprg3
        mtcr     r3
        mfspr    r3,sprg0
        mfspr    r4,sprg1
        mfspr    r5,sprg2
        mtspr    sprg0,r0       ## MQX interrupt prolog
        mfctr    r0
        mtspr    sprg1,r0
        addis    r0,r0,_int_kernel_isr@h
        ori      r0,r0,_int_kernel_isr@l
        mtctr    r0
        addi     r0,0,2
        bcctr    20,0

_psp_itlb_miss_isr_end:

/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_dtlb_miss_isr
## Comments         :
##   this function handles Data TLB misses when the MMU page tables
## are enabled
##
##END*------------------------------------------------------------------------
*/

       .section dtlb_miss,32,1,6
       .globl _psp_dtlb_miss_isr
       .globl _psp_dtlb_miss_isr_end

_psp_dtlb_miss_isr:
        mtspr    sprg0,r3
        mtspr    sprg1,r4
        mtspr    sprg2,r5
        mfcr     r5
        mtspr    sprg3,r5

        mfspr    r4,sprg4        ## Base of mmu page tables
        mfspr    r3,0x3D5        ## DEAR
        rlwinm   r5,r3,12,20,29  ## Get L1 page offset
        lwzx     r5,r4,r5
        cmpi     0x0,r5,0x0
        beq-     dtlb_not_found

        rlwinm   r3,r3,21,21,28  ## Get L2 page offset
        add      r3,r3,r5
        lwz      r5,0(r3)        ## Get tlb_hi
        cmpi     0x0,r5,0x0
        beq      dtlb_not_found

        mfspr    r4,sprg5        ## tlb counter
        tlbwehi  r5,r4
        lwz      r5,4(r3)        ## Get tlb_lo
        tlbwelo  r5,r4

        addi     r4,r4,1
        mfspr    r3,sprg6
        cmp      r4,r3
        blt      dtlb_count_ok
        xor      r4,r4,r4
dtlb_count_ok:
        mtspr    sprg5,r4
        mfspr    r3,sprg3
        mtcr     r3
        mfspr    r3,sprg0
        mfspr    r4,sprg1
        mfspr    r5,sprg2
        rfi

dtlb_not_found:
        mfspr    r3,sprg3
        mtcr     r3
        mfspr    r3,sprg0
        mfspr    r4,sprg1
        mfspr    r5,sprg2

        mtspr    sprg0,r0       ## MQX interrupt prolog
        mfctr    r0
        mtspr    sprg1,r0
        addis    r0,r0,_int_kernel_isr@h
        ori      r0,r0,_int_kernel_isr@l
        mtctr    r0
        addi     r0,0,2
        bcctr    20,0

_psp_dtlb_miss_isr_end:

#endif

##==============================================================================

#if (defined(PSP_G2_CORE)||defined(PSP_G2_LE_CORE)||defined(PSP_E300_CORE))  // (MQX_CPU==PSP_CPU_MPC603)||(MQX_CPU==PSP_CPU_MPC604)||(MQX_CPU==PSP_CPU_MPC740)||(MQX_CPU==PSP_CPU_MPC750)||(MQX_CPU==PSP_CPU_MPC8240)||

/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_tlb_inst_violation_esr
## Comments         :
##   this function handles Instruction TLB misses
##
##END*------------------------------------------------------------------------
*/

         .section tlb_inst_violation,32,1,6
         .globl _psp_tlb_inst_violation_esr

         SET_FUNCTION_ALIGNMENT
_psp_tlb_inst_violation_esr:
         mfspr    r3,srr1        ## Get srr1
         rlwinm   r2,r3,0,16,31  ## Clean srr1 &= 0xffff
         addis    r2,r2,0x4000   ## OR in srr1<1>=1 to flag pte not found
         mtctr    r0             ## Restore counter
         mtspr    srr1,r2        ## Set srr1
         mfmsr    r0             ## Get msr
         xoris    r0,r0,2        ## Flip the msr<tgpr> bit
         mtcrf    0x80,r3        ## Restore CR0
         mtmsr    r0             ## Flip back to the native gprs
         b        isExcpt        ## Jump to instruction storage exception 0x300
         rfi


/*
##FUNCTION*-------------------------------------------------------------------
##
## Function Name    : _psp_tlb_data_violation_esr
## Returned Value   : none
## Comments         :
##   This is the assembler level exception that intercepts a TLB exception
##  which occurs when a data address cannot be translated by the DTLBs.
##
##END*----------------------------------------------------------------------
*/

         .section tlb_data_violation,32,1,6
         .globl _psp_tlb_data_violation_esr

         SET_FUNCTION_ALIGNMENT
_psp_tlb_data_violation_esr:
         mfspr    r3,srr1        ## Get srr1
         rlwinm   r1,r3,9,6,6    ## Get srr1<flag> to bit 6 for load/store, zero rest
         addis    r1,r1,0x4000   ## OR in dsisr<1>=1 to flag pte not found
         mtctr    r0             ## Restore counter
         rlwinm   r2,r3,0,16,31  ## Clean srr1 &= 0xffff
         mtspr    srr1,r2        ## Set srr1
         mtspr    18,r1          ## Load the dsisr (SPR 18)
         mfspr    r1,976         ## Get miss address (SPR 976)
         rlwinm.  r2,r2,0,31,31  ## Test LE bit
         bne      dsi2esr        ## If little endian then:
         xori     r1,r1,0x07     ## De-mung the data address
dsi2esr:
         mtspr    dar,r1         ## Put in dar
         mfmsr    r0             ## Get msr
         xoris    r0,r0,2        ## Flip the msr<tgpr> bit
         mtcrf    0x80,r3        ## Restore CR0
         mtmsr    r0             ## Flip back to the native gprs
         b        dsExcpt        ## Jump to data storage exception 0x400


/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_itlb_miss_isr
## Comments         :
##   this function handles Instruction TLB misses when the MMU page tables
## are enabled
##
##END*------------------------------------------------------------------------

##  Vector 0x300 & 0x400 positions for the tlb exceptions to jump to once
## they have finished execution.

## When relocated to ROM we need the code pre-initialized.  It
## can't be relocated to RAM unless the "near" branch instructions are changed
## to "far" branches.
*/
         .section .dsExcpt,32,1,6    ## 0x300
dsExcpt:
     mtsprg0  r0
         mfctr    r0
         mtsprg1  r0
         addis    r0,r0,_int_kernel_isr@h
         ori      r0,r0,_int_kernel_isr@l
         mtctr    r0
         li       r0,3
         bctr

         .section .isExcpt,32,1,6    ## 0x400
isExcpt:
     mtsprg0  r0
         mfctr    r0
         mtsprg1  r0
         addis    r0,r0,_int_kernel_isr@h
         ori      r0,r0,_int_kernel_isr@l
         mtctr    r0
         li       r0,4
         bctr

         .section itlb_miss,32,1,6

         .globl _psp_itlb_miss_isr
         .globl _psp_itlb_miss_isr_end

_psp_itlb_miss_isr:
        mfspr r2, _hash1  ## get first pointer
        li    r1, 8      ## load 8 for counter
        mfctr r0         ## save counter
        mfspr r3, iCmp   ## get first compare value
        addi  r2, r2, -8 ## pre dec the pointer
im0:    mtctr r1         ## load counter
im1:    lwzu  r1, 8(r2)  ## get next pte
        cmp   0, r1, r3  ## see if found pte
        bdnzf eq,im1        ## dec count br if cmp ne and if count not zero
        bne instrSecHash ## if not found set up second hash or exit

_psp_itlb_miss_found:
        lwz   r1, +4(r2) ## load tlb entry lower-word
        andi. r3, r1, 8  ## check G bit
        bne   doISIp     ## if guarded, take an ISI
        mtctr r0         ## restore counter
        mfspr r0, iMiss  ## get the miss address for the tlbli
        mfspr r3, srr1   ## get the saved cr0 bits
        mtcrf 0x80, r3   ## restore CR0
        mtspr _rpa, r1    ## set the pte
        ori   r1, r1, 0x100 ## set reference bit
        tlbli r0         ## load the itlb
        stw   r1, +4(r2) ## update page table
        rfi

instrSecHash:
        andi. r1, r3, 0x0040 ## see if we have done second hash
        bne   doISI          ## if so, go to ISI exception
        mfspr r2, _hash2      ## get the second pointer
        ori   r3, r3, 0x0040 ## change the compare value
        addi  r1, 0, 8       ## load 8 for counter
        addi  r2, r2, -8     ## pre dec for update on load
        b     im0            ## try second hash

doISIp:
        mfspr  r3, srr1       ## get srr1
        rlwinm r2,r3,0,16,31  ## Clean srr1 &= 0xffff
        addis  r2, r2, 0x0800 ## or in srr<4> = 1 to flag prot violation
        b      isi1
doISI:
        mfspr  r3, srr1       ## get srr1
        rlwinm r2,r3,0,16,31  ## Clean srr1 &= 0xffff
        addis  r2, r2, 0x4000 ## or in srr1<1> = 1 to flag pte not found
isi1:   mtctr  r0             ## restore counter
        mtspr  srr1, r2       ## set srr1
        mfmsr  r0             ## get msr
        xoris  r0, r0, 2      ## flip the msr<tgpr> bit
        mtcrf  0x80, r3       ## restore CR0
        mtmsr  r0             ## flip back to the native gprs
        b      isExcpt        ## go to instr. access exception

_psp_itlb_miss_isr_end:

/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_dtlb_miss_on_load_isr
## Comments         :
##   this function handles Instruction TLB misses when the MMU page tables
## are enabled
##
##END*------------------------------------------------------------------------
*/

         .section dtlb_miss_load,32,1,6

         .globl _psp_dtlb_miss_on_load_isr
         .globl _psp_dtlb_miss_on_load_isr_end

_psp_dtlb_miss_on_load_isr:
        mfspr r2, _hash1     ## get first pointer
        addi  r1, 0, 8      ## load 8 for counter
        mfctr r0            ## save counter
        mfspr r3, dCmp      ## get first compare value
        addi  r2, r2, -8    ## pre dec the pointer
dm0:    mtctr r1            ## load counter
dm1:    lwzu  r1, 8(r2)     ## get next pte
        cmp   0, r1, r3     ## see if found pte
        bdnzf eq,dm1        ## dec count br if cmp ne and if count not zero
        bne   dataSecHash   ## if not found set up second hash or exit
        lwz   r1, +4(r2)    ## load tlb entry lower-word
        mtctr r0            ## restore counter
        mfspr r0, dMiss     ## get the miss address for the tlbld
        mfspr r3, srr1      ## get the saved cr0 bits
        mtcrf 0x80, r3      ## restore CR0
        mtspr _rpa, r1       ## set the pte
        ori   r1, r1, 0x100 ## set reference bit
        tlbld r0            ## load the dtlb
        stw   r1, +4(r2)    ## update page table
        rfi

dataSecHash:
        andi. r1, r3, 0x0040 ## see if we have done second hash
        bne   doDSI          ## if so, go to DSI exception
        mfspr r2, _hash2      ## get the second pointer
        ori   r3, r3, 0x0040 ## change the compare value
        addi  r1, 0, 8       ## load 8 for counter
        addi  r2, r2, -8     ## pre dec for update on load
        b     dm0            ## try second hash

_psp_dtlb_miss_on_load_isr_end:

/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_dtlb_miss_on_store_isr
## Comments         :
##   this function handles data TLB misses when the MMU page tables
## are enabled
##
##END*------------------------------------------------------------------------
*/

         .section dtlb_miss_store,32,1,6

         .globl _psp_dtlb_miss_on_store_isr
         .globl _psp_dtlb_miss_on_store_isr_end

_psp_dtlb_miss_on_store_isr:
        mfspr r2, _hash1   ## get first pointer
        addi  r1, 0, 8    ## load 8 for counter
        mfctr r0          ## save counter
        mfspr r3, dCmp    ## get first compare value
        addi  r2, r2, -8  ## pre dec the pointer
ceq0:   mtctr r1          ## load counter
ceq1:   lwzu  r1, 8(r2)   ## get next pte
        cmp   0, r1, r3   ## see if found pte
        bdnzf eq,ceq1     ## dec count br if cmp ne and if count not zero
        bne   cEq0SecHash ## if not found set up second hash or exit
        lwz   r1, +4(r2)  ## load tlb entry lower-word
        andi. r3,r1,0x80  ## check the C-bit
        beq   cEq0ChkProt ## if (C==0) go check protection modes
ceq2:   mtctr r0          ## restore counter
        mfspr r0, dMiss   ## get the miss address for the tlbld
        mfspr r3, srr1    ## get the saved cr0 bits (See section 6.3 table 6-6)
        mtcrf 0x80, r3    ## restore CR0
        mtspr _rpa, r1     ## set the pte
        tlbld r0          ## load the dtlb
        rfi

cEq0SecHash:
        andi. r1, r3, 0x0040 ## see if we have done second hash
        bne   doDSI          ## if so, go to DSI exception
        mfspr r2, _hash2      ## get the second pointer
        ori   r3, r3, 0x0040 ## change the compare value
        addi  r1, 0, 8       ## load 8 for counter
        addi  r2, r2, -8     ## pre dec for update on load
        b     ceq0           ## try second hash

cEq0ChkProt:
        rlwinm. r3,r1,30,0,1 ## test PP
        bge-  chk0           ## if (PP==00 or PP==01) goto chk0:
        andi. r3,r1,1        ## test PP[0]
        beq+  chk2           ## return if PP[0]==0
        b     doDSIp         ## else DSIp
chk0:   mfspr r3,srr1        ## get old msr
        andis. r3,r3,0x0008  ## test the KEY bit (SRR1-bit 12)
        beq   chk2           ## if (KEY==0) goto chk2:
        b     doDSIp         ## else DSIp
chk2:   ori   r1, r1, 0x180  ## set reference and change bit
        stw   r1, 4(r2)      ## update page table
        b     ceq2           ## and back we go

doDSI:
        mfspr r3, srr1       ## get srr1
        rlwinm r1, r3, 9,6,6 ## get srr1<flag> to bit 6 for load/store, zero rest
        addis r1, r1, 0x4000 ## or in dsisr<1> = 1 to flag pte not found
        b     dsi1

doDSIp:
        mfspr r3, srr1        ## get srr1
        rlwinm r1, r3, 9,6,6  ## get srr1<flag> to bit 6 for load/store, zero rest
        addis r1, r1, 0x0800  ## or in dsisr<4> = 1 to flag prot violation
dsi1:   mtctr r0              ## restore counter
        rlwinm r2,r3,0,16,31  ## Clean srr1 &= 0xffff
        mtspr srr1, r2        ## set srr1
        mtspr dsisr, r1       ## load the dsisr
        mfspr r1, dMiss       ## get miss address
        rlwinm. r2,r2,0,31,31 ## test LE bit
        beq   dsi2            ## if little endian then:
        xori  r1,r1,0x07      ## de-mung the data address
dsi2:   mtspr dar, r1         ## put in dar
        mfmsr r0              ## get msr
        xoris r0, r0, 2       ## flip the msr<tgpr> bit
        mtcrf 0x80, r3        ## restore CR0
        mtmsr r0              ## flip back to the native gprs
        b     dsExcpt         ## branch to DSI exception

_psp_dtlb_miss_on_store_isr_end:

   #endif

##==============================================================================


#undef #TEMP_DEFINE
#if ((MQX_CPU==PSP_CPU_MPC801)||(MQX_CPU==PSP_CPU_MPC821))
   #define TEMP_DEFINE 1
#elif ((MQX_CPU==PSP_CPU_MPC823)||(MQX_CPU==PSP_CPU_MPC850))
   #define TEMP_DEFINE 1
#elif ((MQX_CPU==PSP_CPU_MPC855)||(MQX_CPU==PSP_CPU_MPC860))
   #define TEMP_DEFINE 1
#elif ((MQX_CPU==PSP_CPU_MPC875))
   #define TEMP_DEFINE 1
#else
   #define TEMP_DEFINE 0
#endif

#if TEMP_DEFINE 

/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_itlb_miss_isr
## Comments         :
##   this function handles Instruction TLB misses
##
##END*------------------------------------------------------------------------
*/

        .section  itlb_miss,32,1,6
        .globl    _psp_itlb_miss_isr
        .globl    _psp_itlb_miss_isr_end
_psp_itlb_miss_isr:
        #if defined(PSP_REV_AB_CHIP)
           mtspr  274,r2
           mtspr  275,r3
        #endif

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x3f80
   #endif
        mtspr  799,r1     ## save r1 in M_TW
        mfspr  r1,26      ## get inst miss effective addr from SRR0

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x3780
   #endif
        mtspr  795,r1     ## store to MD_EPN

        mfspr  r1,796     ## read level one pointer from M_TWB
        lwz    r1,0(r1)   ## load level one page entry

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x2b80
   #endif
        mtspr  789,r1     ## store level 1 attributes to MI_TWC

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x3b80
   #endif
        mtspr  797,r1     ## store level 2 base pointer to MD_TWC

        mfspr  r1,797     ## get level 2 pointer

        lwz    r1,0(r1)   ## load level 2 page entry

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x2d80
   #endif
        mtspr  790,r1     ## write TLB entry via MI_RPN

        mfspr  r1,799     ## restore r1 from M_TW

        #if defined(PSP_REV_AB_CHIP)
           mfspr  r2,274
           mfspr  r3,275
        #endif

        rfi

_psp_itlb_miss_isr_end: .long 0


/*
##ISR*--------------------------------------------------------------------
##
## Function Name    : _psp_dtlb_miss_isr
## Comments         :
##   this function handles Instruction TLB misses
##
##END*------------------------------------------------------------------------
*/
        .section  dtlb_miss,32,1,6
        .globl    _psp_dtlb_miss_isr
        .globl    _psp_dtlb_miss_isr_end
_psp_dtlb_miss_isr:
        #if defined(PSP_REV_AB_CHIP)
           mtspr  274,r2
           mtspr  275,r3
        #endif

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x3f80
   #endif
        mtspr  799,r1     ## save r1 in M_TW

        mfspr  r1,796     ## read level one pointer from M_TWB
        lwz    r1,0(r1)   ## load level one page entry

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x3b80
   #endif
        mtspr  797,r1     ## store level 2 base pointer to MD_TWC

        mfspr  r1,797     ## get level 2 pointer

        lwz    r1,0(r1)   ## load level 2 page entry

   #if (MQX_CPU==PSP_CPU_MPC860)
        PSP_SPR_WRITE_FIX 0x3d80
   #endif
        mtspr  798,r1     ## write TLB entry via MD_RPN

        mfspr  r1,799     ## restore r1 from M_TW

        #if defined(PSP_REV_AB_CHIP)
           mfspr  r2,274
           mfspr  r3,275
        #endif

        rfi

_psp_dtlb_miss_isr_end: .long 0

#endif


#if (MQX_CPU==PSP_CPU_MPC7400)
        .text
        .globl    __mqx_longjmp
        SET_FUNCTION_ALIGNMENT
__mqx_longjmp:
        .extern longjmp
        b      longjmp
        .type  __mqx_longjmp,@function
        .size  __mqx_longjmp, . - __mqx_longjmp
#endif

#if (defined(PSP_G2_LE_CORE)||defined(PSP_E300_CORE))
        .text
        .globl __psp_get_mbar
        SET_FUNCTION_ALIGNMENT
__psp_get_mbar:
        mfspr   r3, 311
    blr
        .type  __psp_get_mbar,@function
        .size  __psp_get_mbar, . - __psp_get_mbar
        .globl __psp_set_mbar
__psp_set_mbar:
        mtspr   311, r3
    blr
        .type  __psp_set_mbar,@function
        .size  __psp_set_mbar, . - __psp_set_mbar
#endif


    .text
        .align  2
    .globl _ppc_saved_lr
_ppc_saved_lr:
    lwz r3,0(r1)    ## Previous stack pointer
    lwz r3,4(r3)    ## Previous link register
    blr
        .type _ppc_saved_lr,@function
        .size _ppc_saved_lr, . - _ppc_saved_lr

#if ( (defined(PSP_G2_LE_CORE))  || defined(PSP_G2_CORE)|| defined(PSP_E300_CORE))  // || (MQX_CPU==PSP_CPU_MPC7400)||( MQX_CPU==PSP_CPU_MPC603) 
        .globl _psp_read_timebase
_psp_read_timebase:
    mftbu   r3
    mftb    r4
    mftbu   r5
    cmpw    cr0,r5,r3
    bne     cr0,_psp_read_timebase
    blr
        .type  _psp_read_timebase,@function
        .size  _psp_read_timebase, . - _psp_read_timebase
#endif

## EOF
