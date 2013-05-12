;*HEADER********************************************************************
;* 
;* Copyright (c) 2008-2011 Freescale Semiconductor;
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
;* $FileName: psp_prv.s$
;* $Version : 3.8.14.0$
;* $Date    : Aug-31-2011$
;*
;* Comments:
;*
;*   This assembler header file contains private declarations for 
;*   use with the mqx assembler files
;*
;*END***********************************************************************

#include <asm_mac.h>
;------------------------------------------------------------------------
;                         EXTERNAL REFERENCES
;

#if MQX_KERNEL_LOGGING
        ASM_EXTERN(_klog_block_internal)
        ASM_EXTERN(_klog_execute_scheduler_internal)
        ASM_EXTERN(_klog_yield_internal)
        ASM_EXTERN(_klog_context_switch_internal)
        ASM_EXTERN(_klog_isr_start_internal)
        ASM_EXTERN(_klog_isr_end_internal)
#endif
        ASM_EXTERN(_mqx_kernel_data)
        ASM_EXTERN(_mqx_system_stack)


;------------------------------------------------------------------------
;                       CONSTANT DECLARATIONS
;

; The following are the bits in the CONFIG field of the kernel data structure
; to set for the psp options.
; NOTE: These must agree with values in mqx_prv.h

#define __ASM__
#include "mqx_prv.h"
#undef __ASM__

/* PSP_HAS_EMAC macro specifies if processor has Enhanced Multiply-Accumulate Unit (EMAC)*/
#ifndef PSP_HAS_EMAC
#error  PSP_HAS_EMAC macro has to be defined in mqx\source\psp\<proc_family>\<proc_type>.h header file
#endif

/* PSP_HAS_DSP macro specifies if processor has Multiply-Accumulate Unit (MAC)*/
/* TODO: rename PSP_HAS_DSP to PSP_HAS_MAC */
#ifndef PSP_HAS_DSP
#define PSP_HAS_DSP macro has to be defined in mqx\source\psp\<proc_family>\<proc_type>.h header file
#endif


; Offset to the stack frame format and vector word from the stack pointer.
; 15 registers slots are allocated before the SR and PC on the stack

ASM_EQUATE(FLINT_SR_OFFSET    , 4*15+2)
ASM_EQUATE(FLINT_FORMAT_OFFSET, FLINT_SR_OFFSET-2)
ASM_EQUATE(FLINT_PC_OFFSET    , FLINT_FORMAT_OFFSET+4)

;  Task FLAGS bits
;  These must match definitions in mqx_prv.h

; MUST match
#if PSP_HAS_FPU 
ASM_EQUATE(FP_TASK_MASK, MQX_FLOATING_POINT_TASK)  ; mqx.h -> mqx_prv.h
#endif

ASM_EQUATE(FP_CONTEXT_SAVED_MASK, TASK_FLOATING_POINT_CONTEXT_SAVED)    ; mqx_prv.h
ASM_EQUATE(FP_CONTEXT_CLEAR_MASK, 0xfdff)
ASM_EQUATE(PREEMPTION_DISABLED,   TASK_PREEMPTION_DISABLED)             ; mqx_prv.h

; DSP register offsets from Stack Base
#if PSP_HAS_DSP
ASM_EQUATE(TD_DSP_MASK     , -4  )
ASM_EQUATE(TD_DSP_MACSR    , -8  )
ASM_EQUATE(TD_DSP_ACCEXT23 , -12 )
ASM_EQUATE(TD_DSP_ACCEXT01 , -16 )
ASM_EQUATE(TD_DSP_ACC3     , -20 )
ASM_EQUATE(TD_DSP_ACC2     , -24 )
ASM_EQUATE(TD_DSP_ACC1     , -28 )
ASM_EQUATE(TD_DSP_ACC0     , -32 )
#endif

;------------------------------------------------------------------------
;                          MACRO DECLARATIONS
;
#ifdef __ACF__
/* IAR Assembler Macros */

; This macro returns the address of the kernel data in the specified register
GET_KERNEL_DATA MACRO REG
        movea.l  ASM_PREFIX(_mqx_kernel_data),REG    ; get the kernel data pointer
                ENDM

; Start CR 1169
; This macro returns the address of the system task stack ptr in the specified register
GET_SYSTEM_STACK MACRO REG
        movea.l  ASM_PREFIX(_mqx_system_stack), REG
         ENDM
; End CR 1169

; This macro saves the context for the running task when called from a C 
; function. Since it is called from a C function, the C scratch registers
; d0,d1,d2,a0,a1 do not have to be saved
SAVE_ALL_REGISTERS MACRO
        lea.l   (-4*16,sp),sp            ; make room on the stack
        movem.l d3-d7,(4*3,sp)           ; save regs (can optimize and
        movem.l a2-a6,(4*10,sp)          ; not save d0,d1,d2,a0,a1)
        move.w  #0x4000,d0               ; set coldfire stack frame
        swap    d0                    
        move.w  sr,d0        
        move.l  d0,(4*15,sp)             ; save SR and format                                         
        ENDM

SAVE_DSP_REGISTERS MACRO TD_REG, TEMPA_REG
#if PSP_HAS_DSP

        movea.l  (TD_STACK_BASE,TD_REG),TEMPA_REG
        
  #if PSP_HAS_EMAC
        
;        move.l  ACCext01,d1
        ASM_CONST16(0xAB81)
        move.l  d1,(TD_DSP_ACCEXT01,TEMPA_REG)
;        move.l  ACCext23,d1
        ASM_CONST16(0xAF81)
        move.l  d1,(TD_DSP_ACCEXT23,TEMPA_REG)
;        move.l  ACC0,d1
        ASM_CONST16(0xA181)
        move.l  d1,(TD_DSP_ACC0,TEMPA_REG)
;        move.l  ACC1,d1
        ASM_CONST16(0xA381)
        move.l  d1,(TD_DSP_ACC1,TEMPA_REG)
;        move.l  ACC2,d1
        ASM_CONST16(0xA581)
        move.l  d1,(TD_DSP_ACC2,TEMPA_REG)
;        move.l  ACC3,d1
        ASM_CONST16(0xA781)
        move.l  d1,(TD_DSP_ACC3,TEMPA_REG)

        move.l  MACSR,d1
        move.l  d1,(TD_DSP_MACSR,TEMPA_REG)
;        move.l  MASK,d1
        ASM_CONST16(0xAD81)
        move.l  d1,(TD_DSP_MASK,TEMPA_REG)

  #else /* PSP_HAS_EMAC */
        
        move.l  ACC, d1
        move.l  d1, (TD_DSP_ACC0,TEMPA_REG)
        
  #endif /* PSP_HAS_EMAC */
        
#endif /* PSP_HAS_DSP */
        
        ENDM
        
; This macro restores the context for a task
RESTORE_ALL_REGISTERS: MACRO
        movem.l  (sp),a0-a6/d0-d7        ; restore regs
        adda.l    #60,sp
        ENDM

RESTORE_DSP_REGISTERS MACRO TD_REG, TEMPA_REG
#if PSP_HAS_DSP
        
        movea.l  (TD_STACK_BASE,TD_REG),TEMPA_REG
        
  #if PSP_HAS_EMAC

        move.l  (TD_DSP_ACC0,TEMPA_REG),d0
;        move.l  d0,ACC0
        ASM_CONST16(0xA100)
        move.l  (TD_DSP_ACC1,TEMPA_REG),d0
;        move.l  d0,ACC1
        ASM_CONST16(0xA300)
        move.l  (TD_DSP_ACC2,TEMPA_REG),d0
;        move.l  d0,ACC2
        ASM_CONST16(0xA500)
        move.l  (TD_DSP_ACC3,TEMPA_REG),d0
;        move.l  d0,ACC3
        ASM_CONST16(0xA700)

        move.l  (TD_DSP_ACCEXT01,TEMPA_REG),d0
;        move.l  d0,ACCEXT01
        ASM_CONST16(0xAB00)
        move.l  (TD_DSP_ACCEXT23,TEMPA_REG),d0
;        move.l  d0,ACCext23
        ASM_CONST16(0xAF00)

        move.l  (TD_DSP_MASK,TEMPA_REG),d0
;        move.l  d0,MASK
        ASM_CONST16(0xAD00)
        move.l  (TD_DSP_MACSR,TEMPA_REG),d0
        move.l  d0,MACSR
        
  #else /* PSP_HAS_EMAC */
        
    move.l  (TD_DSP_ACC0,TEMPA_REG), d0
    move.l  d0, ACC
        
  #endif /* PSP_HAS_EMAC */
        
#endif  /* PSP_HAS_DSP */
        ENDM

SAVE_ISR_REGISTERS MACRO
        lea      (-14*4,sp),sp           ; make room on the stack
        move.l   (14*4,sp),(sp)          ; move D0 to proper location in
                                        ; saved stack frame
        movem.l  d1-d2,(4,sp)            ; save isr registers (just scratch and d2)
        movem.l  a0-a2,(4*8,sp)
        ENDM

RESTORE_ISR_REGISTERS MACRO
        movem.l (sp),d0-d2
        movem.l (8*4,sp),a0-a2
        lea.l   (4*15,sp),sp
        ENDM

SAVE_REST_ISR_REGISTERS MACRO
        movem.l d3-d7,(3*4,sp)           ; save rest
        movem.l a3-a6,(11*4,sp) 
        ENDM
                
RESTORE_REST_ISR_REGISTERS MACRO
        movem.l  (sp),a0-a6/d0-d7       ; restore regs
        adda.l    #60,sp
        ENDM

; This macro calls the kernel logging function, if logging enabled
KLOG   MACRO KDATA, KLOG_FUNCTION
#if MQX_KERNEL_LOGGING
        LOCAL no_log
        btst.b  #0,(KD_LOG_CONTROL+3,KDATA)
        beq.b   no_log
        jsr     (KLOG_FUNCTION).L
no_log:
#endif
        ENDM
        
#else /* __ACF__ */ 

; This macro returns the address of the kernel data in the specified register
GET_KERNEL_DATA: .macro REG
        move.l  ASM_PREFIX(_mqx_kernel_data),REG    ; get the kernel data pointer
        .endm

; This macro returns the address of the system task stack ptr in the specified register
GET_SYSTEM_STACK: .macro REG
        move.l  ASM_PREFIX(_mqx_system_stack), REG
        .endm

; This macro saves the context for the running task when called from a C 
; function. Since it is called from a C function, the C scratch registers
; d0,d1,d2,a0,a1 do not have to be saved
SAVE_ALL_REGISTERS: .macro
        lea.l   -4*16(sp),sp            ; make room on the stack
        movem.l d3-d7,4*3(sp)           ; save regs (can optimize and
        movem.l a2-a6,4*10(sp)          ; not save d0,d1,d2,a0,a1)
        move.w  #$4000,d0               ; set coldfire stack frame
        swap    d0        
        move.w  sr,d0        
        move.l  d0,4*15(sp)             ; save SR and format                                         
        .endm

SAVE_DSP_REGISTERS: .macro TD_REG TEMPA_REG
        #if PSP_HAS_DSP

        move.l  TD_STACK_BASE(TD_REG),TEMPA_REG
        
        #if PSP_HAS_EMAC
        
;        move.l  ACCext01,d1
        .word   0xAB81
        move.l  d1,TD_DSP_ACCEXT01(TEMPA_REG)
;        move.l  ACCext23,d1
        .word   0xAF81
        move.l  d1,TD_DSP_ACCEXT23(TEMPA_REG)

;        move.l  ACC0,d1
        .word   0xA181
        move.l  d1,TD_DSP_ACC0(TEMPA_REG)
;        move.l  ACC1,d1
        .word   0xA381
        move.l  d1,TD_DSP_ACC1(TEMPA_REG)
;        move.l  ACC2,d1
        .word   0xA581
        move.l  d1,TD_DSP_ACC2(TEMPA_REG)
;        move.l  ACC3,d1
        .word   0xA781
        move.l  d1,TD_DSP_ACC3(TEMPA_REG)

        move.l  MACSR,d1
        move.l  d1,TD_DSP_MACSR(TEMPA_REG)
;        move.l  MASK,d1
        .word   0xAD81
        move.l  d1,TD_DSP_MASK(TEMPA_REG)
        
        #else   // PSP_HAS_EMAC
        
        move.l  ACC, d1
        move.l  d1, TD_DSP_ACC0(TEMPA_REG)
        
        #endif  // PSP_HAS_EMAC
        
        #endif  // PSP_HAS_DSP
        
        .endm
        
; This macro restores the context for a task
RESTORE_ALL_REGISTERS: .macro
        movem.l  (sp),a0-a6/d0-d7        ; restore regs
        add.l    #60,sp
        .endm

RESTORE_DSP_REGISTERS: .macro TD_REG TEMPA_REG
        #if PSP_HAS_DSP
        
        move.l  TD_STACK_BASE(TD_REG),TEMPA_REG
        
        #if PSP_HAS_EMAC

        move.l  TD_DSP_ACC0(TEMPA_REG),d0
;        move.l  d0,ACC0
        .word   0xA100
        move.l  TD_DSP_ACC1(TEMPA_REG),d0
;        move.l  d0,ACC1
        .word   0xA300
        move.l  TD_DSP_ACC2(TEMPA_REG),d0
;        move.l  d0,ACC2
        .word   0xA500
        move.l  TD_DSP_ACC3(TEMPA_REG),d0
;        move.l  d0,ACC3
        .word   0xA700

        move.l  TD_DSP_ACCEXT01(TEMPA_REG),d0
;        move.l  d0,ACCEXT01
        .word   0xAB00
        move.l  TD_DSP_ACCEXT23(TEMPA_REG),d0
;        move.l  d0,ACCext23
        .word   0xAF00

        move.l  TD_DSP_MASK(TEMPA_REG),d0
;        move.l  d0,MASK
        .word   0xAD00
        move.l  TD_DSP_MACSR(TEMPA_REG),d0
        move.l  d0,MACSR
        
        #else   // PSP_HAS_EMAC
        
        move.l  TD_DSP_ACC0(TEMPA_REG), d0
        move.l  d0, ACC
        
        #endif  // PSP_HAS_EMAC
        
        #endif  // PSP_HAS_DSP
        .endm

SAVE_ISR_REGISTERS: .macro
        lea      -14*4(sp),sp           ; make room on the stack
        move.l   14*4(sp),(sp)          ; move D0 to proper location in
                                        ; saved stack frame
        movem.l  d1-d2,4(sp)            ; save isr registers (just scratch and d2)
        movem.l  a0-a2,4*8(sp)
        .endm

RESTORE_ISR_REGISTERS: .macro
        movem.l (sp),d0-d2
        movem.l 8*4(sp),a0-a2
        lea.l   4*15(sp),sp
        .endm

SAVE_REST_ISR_REGISTERS: .macro
        movem.l d3-d7,3*4(sp)           ; save rest
        movem.l a3-a6,11*4(sp) 
        .endm
                
RESTORE_REST_ISR_REGISTERS: .macro
        movem.l  (sp),a0-a6/d0-d7       ; restore regs
        add.l    #60,sp
        .endm

; This macro calls the kernel logging function, if logging enabled
KLOG:   .macro KDATA,KLOG_FUNCTION
        .if MQX_KERNEL_LOGGING
        btst.b  #0,KD_LOG_CONTROL+3(KDATA)
        beq.b   no_log\@
        jsr     KLOG_FUNCTION
no_log\@:
        .endif
        .endm
        
#endif /* __ACF__ */        
; EOF
