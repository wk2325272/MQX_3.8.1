/*HEADER********************************************************************
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
* $FileName: psp_prv_asm.h$
* $Version : 3.8.6.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This assembler header file contains private declarations for 
*   use with the mqx assembler files
*
*END***********************************************************************/


//------------------------------------------------------------------------
//                       CONSTANT DECLARATIONS
//
// handle bug in chip revs A and B wrt writing to SPR registers

#if PSP_HANDLE_PPC_SPR_BUG 
      .align    4   // cache line size 16 bytes (2^4)=16
   #if defined(PSP_REV_AB_CHIP)
      _mqx_psp_temp:   .long 0     
   #endif 

   PSP_SPR_WRITE_FIX: .macro REG
      #if defined(PSP_REV_AB_CHIP)
         lis   r2,_mqx_psp_temp@ha
         li    r3,REG
         stw   r3,_mqx_psp_temp@l(r2)
         lwz   r3,_mqx_psp_temp@l(r2)
      #else
      #endif
   .endm
#endif

// SPR registers
#define dsisr     18
#define dar       19
#define srr0      26
#define srr1      27
#define sprg0     272
#define sprg1     273
#define sprg2     274
#define sprg3     275
#define dMiss     976
#define dCmp      977
#define _hash1    978
#define _hash2    979
#define iMiss     980
#define iCmp      981
#define _rpa      982
#if defined(PSP_AMCC405) || defined(PSP_AMCC440)
   #define srr2   990
   #define srr3   991
#endif
#if defined(PSP_AMCC405) || defined(PSP_AMCC440) || defined(PSP_MPC8560)
   #define sprg4  276
   #define sprg5  277
   #define sprg6  278
   #define sprg7  279
#endif
#define  spefscr  512


#if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
    #define STREG evstdd
    #define LDREG evldd
#else
    #define STREG stw 
    #define LDREG lwz
#endif

//------------------------------------------------------------------------
//                          MACRO DECLARATIONS
//

// MACROS to help out

   SET_FUNCTION_ALIGNMENT: .macro
      .align PSP_CACHE_LINE_SIZE
   .ENDM

// clear the RI bit in the MSR, getting ready to modify SRR0 and SRR1
   CLEAR_RI: .macro
// clear the CE bit in the MSR, getting ready to modify SRR0 and SRR1
      #if (PSP_PPC_GROUP==0)
         mfmsr  r0
         rlwinm r0,r0,0,15,13
         mtmsr  r0
// clear the RI bit in the MSR, getting ready to modify SRR0 and SRR1
      #elif (PSP_PPC_GROUP==1)||(PSP_PPC_GROUP==2)
         mfmsr  r0
         rlwinm r0,r0,0,31,29
         mtmsr  r0
// clear the RI bit in the MSR, getting ready to modify SRR0 and SRR1
      #elif (PSP_PPC_GROUP==3)
         mtspr   82,r0  // this clears NRI bit
      #else
         ERROR NO CPU SPECIFIED
      #endif
   .endm

   GET_KERNEL_DATA: .macro REG
// get the kernel data pointer
      #if PSP_KD_IN_SPRG7
         mfspr REG,sprg7
      #else
         addis REG,r0,_mqx_kernel_data@ha      // No small data
         lwz   REG,_mqx_kernel_data@l(REG)
//lwz   REG,_mqx_kernel_data@sdarx(r0)  // Small data model
      #endif
   .endm

// This macro returns the address of the system task stack ptr in the specified register
GET_SYSTEM_STACK: .macro REG
         addis REG,r0,_mqx_system_stack@ha      // No small data
         lwz   REG,_mqx_system_stack@l(REG)
   .endm
   

SAVE_VOLATILE_FP_REGS: .macro

      #if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
      stfd  f0, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*0(r1)
      stfd  f1, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*1(r1)
      stfd  f2, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*2(r1)
      stfd  f3, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*3(r1)
      stfd  f4, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*4(r1)
      stfd  f5, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*5(r1)
      stfd  f6, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*6(r1)
      stfd  f7, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*7(r1)
      stfd  f8, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*8(r1)
      stfd  f9, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*9(r1)
      stfd  f10,PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*10(r1)
      stfd  f11,PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*11(r1)
      stfd  f12,PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*12(r1)
      stfd  f13,PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*13(r1)
      #endif
   .endm

RESTORE_VOLATILE_FP_REGS: .macro
      #if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
      lfd   f0,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*0(r1)
      lfd   f1,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*1(r1)
      lfd   f2,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*2(r1)
      lfd   f3,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*3(r1)
      lfd   f4,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*4(r1)
      lfd   f5,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*5(r1)
      lfd   f6,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*6(r1)
      lfd   f7,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*7(r1)
      lfd   f8,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*8(r1)
      lfd   f9,  PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*9(r1)
      lfd   f10, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*10(r1)
      lfd   f11, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*11(r1)
      lfd   f12, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*12(r1)
      lfd   f13, PSP_VOLATILE_FP_REGISTERS_OFFSET + PSP_SAVED_FP_REGISTER_SIZE*13(r1)
      #endif
   .endm

SAVE_R3_R31: .macro
        #if  defined(PSP_MPC7400)
        stw   r3, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
        stw   r4, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*2(r1)
        stw   r5, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*3(r1)
        stw   r6, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*4(r1)
        stw   r7, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*5(r1)
        stw   r8, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*6(r1)
        stw   r9, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*7(r1)
        stw   r10,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*8(r1)
        stw   r11,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*9(r1)
        stw   r12,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*10(r1)
        stw   r13,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*11(r1)
        stw   r14,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*12(r1)
        stw   r15,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*13(r1)
        stw   r16,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*14(r1)
        stw   r17,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*15(r1)
        stw   r18,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*16(r1)
        stw   r19,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*17(r1)
        stw   r20,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*18(r1)
        stw   r21,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*19(r1)
        stw   r22,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*20(r1)
        stw   r23,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*21(r1)
        stw   r24,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*22(r1)
        stw   r25,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*23(r1)
        stw   r26,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*24(r1)
        stw   r27,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*25(r1)
        stw   r28,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*26(r1)
        stw   r29,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*27(r1)
        stw   r30,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*28(r1)
        stw   r31,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*29(r1)
     #elif (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
        addi    r1,r1,PSP_GP_REGISTERS_OFFSET   
        STREG   r3, PSP_SAVED_GP_REGISTER_SIZE*1(r1)
        STREG   r4, PSP_SAVED_GP_REGISTER_SIZE*2(r1)
        STREG   r5, PSP_SAVED_GP_REGISTER_SIZE*3(r1)
        STREG   r6, PSP_SAVED_GP_REGISTER_SIZE*4(r1)
        STREG   r7, PSP_SAVED_GP_REGISTER_SIZE*5(r1)
        STREG   r8, PSP_SAVED_GP_REGISTER_SIZE*6(r1)
        STREG   r9, PSP_SAVED_GP_REGISTER_SIZE*7(r1)
        STREG   r10,PSP_SAVED_GP_REGISTER_SIZE*8(r1)
        STREG   r11,PSP_SAVED_GP_REGISTER_SIZE*9(r1)
        STREG   r12,PSP_SAVED_GP_REGISTER_SIZE*10(r1)
        STREG   r13,PSP_SAVED_GP_REGISTER_SIZE*11(r1)
        STREG   r14,PSP_SAVED_GP_REGISTER_SIZE*12(r1)
        STREG   r15,PSP_SAVED_GP_REGISTER_SIZE*13(r1)
        STREG   r16,PSP_SAVED_GP_REGISTER_SIZE*14(r1)
        STREG   r17,PSP_SAVED_GP_REGISTER_SIZE*15(r1)
        STREG   r18,PSP_SAVED_GP_REGISTER_SIZE*16(r1)
        STREG   r19,PSP_SAVED_GP_REGISTER_SIZE*17(r1)
        STREG   r20,PSP_SAVED_GP_REGISTER_SIZE*18(r1)
        STREG   r21,PSP_SAVED_GP_REGISTER_SIZE*19(r1)
        STREG   r22,PSP_SAVED_GP_REGISTER_SIZE*20(r1)
        STREG   r23,PSP_SAVED_GP_REGISTER_SIZE*21(r1)
        STREG   r24,PSP_SAVED_GP_REGISTER_SIZE*22(r1)
        STREG   r25,PSP_SAVED_GP_REGISTER_SIZE*23(r1)
        STREG   r26,PSP_SAVED_GP_REGISTER_SIZE*24(r1)
        STREG   r27,PSP_SAVED_GP_REGISTER_SIZE*25(r1)
        STREG   r28,PSP_SAVED_GP_REGISTER_SIZE*26(r1)
        STREG   r29,PSP_SAVED_GP_REGISTER_SIZE*27(r1)
        STREG   r30,PSP_SAVED_GP_REGISTER_SIZE*28(r1)
        STREG   r31,PSP_SAVED_GP_REGISTER_SIZE*29(r1)
        addi    r1,r1,-PSP_GP_REGISTERS_OFFSET
     #else
        stmw   r3,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
     #endif

.endm
SAVE_R13_R31: .macro
        #if  defined(PSP_MPC7400)
        stw   r13,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*0(r1)
        stw   r14,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
        stw   r15,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*2(r1)
        stw   r16,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*3(r1)
        stw   r17,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*4(r1)
        stw   r18,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*5(r1)
        stw   r19,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*6(r1)
        stw   r20,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*7(r1)
        stw   r21,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*8(r1)
        stw   r22,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*9(r1)
        stw   r23,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*10(r1)
        stw   r24,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*11(r1)
        stw   r25,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*12(r1)
        stw   r26,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*13(r1)
        stw   r27,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*14(r1)
        stw   r28,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*15(r1)
        stw   r29,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*16(r1)
        stw   r30,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*17(r1)
        stw   r31,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*18(r1)
     #elif (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
        addi    r1,r1,PSP_GP13_REGISTERS_OFFSET
        STREG   r13,PSP_SAVED_GP_REGISTER_SIZE*0(r1)
        STREG   r14,PSP_SAVED_GP_REGISTER_SIZE*1(r1)
        STREG   r15,PSP_SAVED_GP_REGISTER_SIZE*2(r1)
        STREG   r16,PSP_SAVED_GP_REGISTER_SIZE*3(r1)
        STREG   r17,PSP_SAVED_GP_REGISTER_SIZE*4(r1)
        STREG   r18,PSP_SAVED_GP_REGISTER_SIZE*5(r1)
        STREG   r19,PSP_SAVED_GP_REGISTER_SIZE*6(r1)
        STREG   r20,PSP_SAVED_GP_REGISTER_SIZE*7(r1)
        STREG   r21,PSP_SAVED_GP_REGISTER_SIZE*8(r1)
        STREG   r22,PSP_SAVED_GP_REGISTER_SIZE*9(r1)
        STREG   r23,PSP_SAVED_GP_REGISTER_SIZE*10(r1)
        STREG   r24,PSP_SAVED_GP_REGISTER_SIZE*11(r1)
        STREG   r25,PSP_SAVED_GP_REGISTER_SIZE*12(r1)
        STREG   r26,PSP_SAVED_GP_REGISTER_SIZE*13(r1)
        STREG   r27,PSP_SAVED_GP_REGISTER_SIZE*14(r1)
        STREG   r28,PSP_SAVED_GP_REGISTER_SIZE*15(r1)
        STREG   r29,PSP_SAVED_GP_REGISTER_SIZE*16(r1)
        STREG   r30,PSP_SAVED_GP_REGISTER_SIZE*17(r1)
        STREG   r31,PSP_SAVED_GP_REGISTER_SIZE*18(r1)
        addi    r1,r1,-PSP_GP13_REGISTERS_OFFSET
     #else
        stmw   r13,PSP_GP13_REGISTERS_OFFSET(r1)
     #endif
   .endm

RESTORE_R3_R31: .macro
      #if defined(PSP_MPC7400)
      lwz  r3, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
      lwz  r4, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*2(r1)
      lwz  r5, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*3(r1)
      lwz  r6, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*4(r1)
      lwz  r7, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*5(r1)
      lwz  r8, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*6(r1)
      lwz  r9, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*7(r1)
      lwz  r10,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*8(r1)
      lwz  r11,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*9(r1)
      lwz  r12,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*10(r1)
      lwz  r13,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*11(r1)
      lwz  r14,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*12(r1)
      lwz  r15,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*13(r1)
      lwz  r16,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*14(r1)
      lwz  r17,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*15(r1)
      lwz  r18,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*16(r1)
      lwz  r19,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*17(r1)
      lwz  r20,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*18(r1)
      lwz  r21,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*19(r1)
      lwz  r22,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*20(r1)
      lwz  r23,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*21(r1)
      lwz  r24,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*22(r1)
      lwz  r25,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*23(r1)
      lwz  r26,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*24(r1)
      lwz  r27,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*25(r1)
      lwz  r28,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*26(r1)
      lwz  r29,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*27(r1)
      lwz  r30,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*28(r1)
      lwz  r31,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*29((r1)
     #elif (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)

      addi    r1,r1,PSP_GP_REGISTERS_OFFSET
      LDREG  r3,  PSP_SAVED_GP_REGISTER_SIZE*1(r1)
      LDREG  r4,  PSP_SAVED_GP_REGISTER_SIZE*2(r1)
      LDREG  r5,  PSP_SAVED_GP_REGISTER_SIZE*3(r1)
      LDREG  r6,  PSP_SAVED_GP_REGISTER_SIZE*4(r1)
      LDREG  r7,  PSP_SAVED_GP_REGISTER_SIZE*5(r1)
      LDREG  r8,  PSP_SAVED_GP_REGISTER_SIZE*6(r1)
      LDREG  r9,  PSP_SAVED_GP_REGISTER_SIZE*7(r1)
      LDREG  r10, PSP_SAVED_GP_REGISTER_SIZE*8(r1)
      LDREG  r11, PSP_SAVED_GP_REGISTER_SIZE*9(r1)
      LDREG  r12, PSP_SAVED_GP_REGISTER_SIZE*10(r1)
      LDREG  r13, PSP_SAVED_GP_REGISTER_SIZE*11(r1)
      LDREG  r14, PSP_SAVED_GP_REGISTER_SIZE*12(r1)
      LDREG  r15, PSP_SAVED_GP_REGISTER_SIZE*13(r1)
      LDREG  r16, PSP_SAVED_GP_REGISTER_SIZE*14(r1)
      LDREG  r17, PSP_SAVED_GP_REGISTER_SIZE*15(r1)
      LDREG  r18, PSP_SAVED_GP_REGISTER_SIZE*16(r1)
      LDREG  r19, PSP_SAVED_GP_REGISTER_SIZE*17(r1)
      LDREG  r20, PSP_SAVED_GP_REGISTER_SIZE*18(r1)
      LDREG  r21, PSP_SAVED_GP_REGISTER_SIZE*19(r1)
      LDREG  r22, PSP_SAVED_GP_REGISTER_SIZE*20(r1)
      LDREG  r23, PSP_SAVED_GP_REGISTER_SIZE*21(r1)
      LDREG  r24, PSP_SAVED_GP_REGISTER_SIZE*22(r1)
      LDREG  r25, PSP_SAVED_GP_REGISTER_SIZE*23(r1)
      LDREG  r26, PSP_SAVED_GP_REGISTER_SIZE*24(r1)
      LDREG  r27, PSP_SAVED_GP_REGISTER_SIZE*25(r1)
      LDREG  r28, PSP_SAVED_GP_REGISTER_SIZE*26(r1)
      LDREG  r29, PSP_SAVED_GP_REGISTER_SIZE*27(r1)
      LDREG  r30, PSP_SAVED_GP_REGISTER_SIZE*28(r1)
      LDREG  r31, PSP_SAVED_GP_REGISTER_SIZE*29(r1)
      addi   r1,r1,-PSP_GP_REGISTERS_OFFSET
      #else
      lmw   r3,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
     #endif
     .endm


RESTORE_R13_R31: .macro
      #if defined(PSP_MPC7400)
      lwz   r13,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*0(r1)
      lwz   r14,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
      lwz   r15,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*2(r1)
      lwz   r16,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*3(r1)
      lwz   r17,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*4(r1)
      lwz   r18,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*5(r1)
      lwz   r19,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*6(r1)
      lwz   r20,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*7(r1)
      lwz   r21,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*8(r1)
      lwz   r22,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*9(r1)
      lwz   r23,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*10(r1)
      lwz   r24,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*11(r1)
      lwz   r25,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*12(r1)
      lwz   r26,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*13(r1)
      lwz   r27,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*14(r1)
      lwz   r28,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*15(r1)
      lwz   r29,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*16(r1)
      lwz   r30,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*17(r1)
      lwz   r31,PSP_GP13_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*18(r1)
     #elif (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
      addi    r1,r1,PSP_GP13_REGISTERS_OFFSET
      LDREG   r13, PSP_SAVED_GP_REGISTER_SIZE*0(r1)
      LDREG   r14, PSP_SAVED_GP_REGISTER_SIZE*1(r1)
      LDREG   r15, PSP_SAVED_GP_REGISTER_SIZE*2(r1)
      LDREG   r16, PSP_SAVED_GP_REGISTER_SIZE*3(r1)
      LDREG   r17, PSP_SAVED_GP_REGISTER_SIZE*4(r1)
      LDREG   r18, PSP_SAVED_GP_REGISTER_SIZE*5(r1)
      LDREG   r19, PSP_SAVED_GP_REGISTER_SIZE*6(r1)
      LDREG   r20, PSP_SAVED_GP_REGISTER_SIZE*7(r1)
      LDREG   r21, PSP_SAVED_GP_REGISTER_SIZE*8(r1)
      LDREG   r22, PSP_SAVED_GP_REGISTER_SIZE*9(r1)
      LDREG   r23, PSP_SAVED_GP_REGISTER_SIZE*10(r1)
      LDREG   r24, PSP_SAVED_GP_REGISTER_SIZE*11(r1)
      LDREG   r25, PSP_SAVED_GP_REGISTER_SIZE*12(r1)
      LDREG   r26, PSP_SAVED_GP_REGISTER_SIZE*13(r1)
      LDREG   r27, PSP_SAVED_GP_REGISTER_SIZE*14(r1)
      LDREG   r28, PSP_SAVED_GP_REGISTER_SIZE*15(r1)
      LDREG   r29, PSP_SAVED_GP_REGISTER_SIZE*16(r1)
      LDREG   r30, PSP_SAVED_GP_REGISTER_SIZE*17(r1)
      LDREG   r31, PSP_SAVED_GP_REGISTER_SIZE*18(r1)
      addi    r1,r1,-PSP_GP13_REGISTERS_OFFSET
   #else
      lmw r13,PSP_GP13_REGISTERS_OFFSET(r1)
   #endif
   .endm


SAVE_SPE_REGS: .macro
   #if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
   addi  r1,r1,PSP_SPE_REGISTERS_OFFSET
   evxor   r0,r0,r0       // Save the ACCumulator by setting r0=0. then
   evaddsmiaaw  r0,r0   // r0=ACC+r0  
   STREG r0,0(r1)
   mfspr r0,spefscr
   stw   r0, 8(r1)
   addi  r1,r1,-PSP_SPE_REGISTERS_OFFSET
   #endif
   .endm

RESTORE_SPE_REGS: .macro
   #if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
   addi  r1,r1,PSP_SPE_REGISTERS_OFFSET
   LDREG r0,0(r1)
   evmra r0,r0
   lwz   r0, 8(r1)
   mtspr spefscr,r0
   addi  r1,r1,-PSP_SPE_REGISTERS_OFFSET

   #endif
   .endm


DISABLE_INTS: .macro REG
      #if (PSP_PPC_GROUP==0)
         wrteei 0
      #elif (PSP_PPC_GROUP==1)
         mfmsr  REG
         rlwinm REG,REG,0,17,15
         mtmsr  REG
      #elif (PSP_PPC_GROUP==2)
         mfmsr  REG
         rlwinm REG,REG,0,17,15 // Clear EE bit
         rlwinm REG,REG,0,25,23 // Clear critical interrupts
         mtmsr  REG
      #elif (PSP_PPC_GROUP==3)
         mtspr 81,0
      #else
         #error NO CPU SPECIFIED
      #endif
   .endm


ENABLE_INTS: .macro REG
      #if (PSP_PPC_GROUP==0)
         wrteei 1
      #elif (PSP_PPC_GROUP==1)
         mfmsr REG
         ori   REG,REG,0x8000
         mtmsr REG
      #elif (PSP_PPC_GROUP==2)
         mfmsr  REG
         ori   REG,REG,0x8080
         mtmsr  REG
      #elif (PSP_PPC_GROUP==3)
         mtspr 80,0
      #else
         #error NO CPU SPECIFIED
      #endif
   .endm

// Called with interrupts enabled.
SAVE_ALL_REGISTERS: .macro
      subi r1,r1,PSP_BLOCKED_STACK_STRUCT_SIZE
      STREG  r0,PSP_GP_REGISTERS_OFFSET(r1)

#if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
      // We don't need to save the lower 32 bits of r3-12, for the
      // reasons stated below, but we do need to save the upper 32 bits,
      // which means saving everything
      SAVE_R3_R31
#else
      // We do not need to save r3-r12 in this macro as it is used at the
      // start of functions called from 'C'. 
      // (_task_block, _sched_execute_scheduler_internal)
      // These registers are 'C' compiler scratch registers, 
      // which 'C' does not expect to be preserved
      SAVE_R13_R31
#endif
      SAVE_SPE_REGS
      SAVE_VOLATILE_FP_REGS

      mfmsr r0         // Read MSR
      stw   r0,20(r1)

      mfcr  r0         // Read CCR
      stw   r0,16(r1)

      mfxer r0         // Read XER
      stw   r0,12(r1)

      mfctr r0         // Read CTR
      stw   r0,8(r1)

      mflr  r0         // Read Link Register
      stw   r0,4(r1)
      stw   r0,0(r1)   // Store also as SRR0

   .endm

// Called with interrupts disabled.
RESTORE_ALL_REGISTERS: .macro
      RESTORE_SPE_REGS
      RESTORE_VOLATILE_FP_REGS

      RESTORE_R3_R31
      
      lwz   r0,12(r1)
      mtxer r0         // Restore XER

      lwz   r0,8(r1)
      mtctr r0         // Restore CTR

      lwz   r0,4(r1)
      mtlr  r0         // Get Link Register  (Return address)

      CLEAR_RI         // clear the RI bit in the MSR

      lwz   r0,16(r1)
      mtcrf 0xFF,r0    // Restore CCR

      lwz   r0,0(r1)
      mtspr srr0,r0    // Get SRR0

      lwz   r0,20(r1)  // 
      mtspr srr1,r0    // GET MSR, and put into SRR1

      LDREG   r0,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*0(r1)

      addi  r1,r1,PSP_BLOCKED_STACK_STRUCT_SIZE

      sync

   .endm


// Called with interrupts disabled
SAVE_ISR_REGISTERS: .macro
      // arrive here with:
      //  r0  in sprg0
      //  ctr in sprg1
      //  interrupt number in r0

      mtspr sprg2,r3
      mtspr sprg3,r4

#if defined(PSP_AMCC401)||defined(PSP_AMCC403)
 // nothing to do
#else
      mfspr r3,srr1    // Read saved MSR (SRR1)
      // Restore MSR bits from saved MSR register (in r3)
      #if MQXCFG_ENABLE_DSP && PSP_HAS_DSP
         li      r4,0x2932    // get state of FP, FP INT, IR, DR and RI bits
         and     r3,r3,r4
         oris    r3,r3,0x0200 // set Altivec enable bit
         ori     r3,r3,E200CORE_MSR_FP // set FP available bit
      #elif MQXCFG_ENABLE_FP && PSP_HAS_FPU
          li      r4,0x2032    // get state of FP, IR, DR and RI bits
          and     r3,r3,r4
          ori     r3,r3,0x00002000 // set FP available bit
      #elif defined(PSP_AMCC480)||defined(PSP_AMCC405)
          li      r4,0x0030 // get state of IR, DR
          and     r3,r3,r4
      #elif (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
          li      r4,0x0032 // get state of IR, DR and RI bits
          and     r3,r3,r4
          oris    r3,r3,(E200CORE_MSR_SPE>>16) // set SPU available bit
      #else
          li      r4,0x0032 // get state of IR, DR and RI bits
          and     r3,r3,r4
      #endif
      mfmsr r4
      or    r4,r4,r3   // Restore bits saved from SRR0 into MSR
      mtmsr r4         // Reprogram MSR
      isync
      sync
#endif

      subi r1,r1,PSP_BLOCKED_STACK_STRUCT_SIZE

      mfspr r3,sprg0  // get R0
      STREG  r3, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*0(r1)
      mfspr r3,sprg2  // get R3
      STREG  r3, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
      mfspr r4,sprg3  // get R4
      STREG  r4, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*2(r1)
      STREG  r5, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*3(r1)
      STREG  r6, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*4(r1)
      STREG  r7, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*5(r1)
      STREG  r8, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*6(r1)
      STREG  r9, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*7(r1)
      STREG  r10,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*8(r1)
      STREG  r11,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*9(r1)
      STREG  r12,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*10(r1)

      SAVE_VOLATILE_FP_REGS
 
      mflr  r3
      stw   r3,4(r1)

      mfcr  r3         // Read CCR
      stw   r3,16(r1)

      mfxer r3         // Read XER
      stw   r3,12(r1)

      //mfctr r3         // Read CTR
      mfspr r3,sprg1
      stw   r3,8(r1)

#if defined(PSP_AMCC401)||defined(PSP_AMCC403)||defined(PSP_AMCC480)||defined(PSP_AMCC405)
// check if it is a critical exception
      mfspr r3,srr2    // Get SRR2  (Return address)
      add.  r3,r3,r3
      beq   _save_int_regs_non_crit
      stw   r3,0(r1)   // Save return address
      xor   r3, r3, r3
      mtspr srr2, r3        
      mtspr srr3, r3        
      b     _save_int_regs_noncrit_skip
_save_int_regs_non_crit:
      rlwinm r4,r4,0,15,13 // clear CE bit
#endif
#if defined(PSP_AMCC480)||(defined(PSP_G2_LE_CORE)) || defined(PSP_E200_CORE) || defined(PSP_E300_CORE) || defined(PSP_E500_CORE))
// check if it is a critical exception
      mfspr r3,csrr0    // Get CSRR0  (Return address)
      cmpi  cr0,r3,0
      beq   _save_int_regs_non_crit
      stw   r3,0(r1)   // Save return address
      li    r3, 0
      mtspr csrr0, r3        
      mfspr r3,csrr1  // Read saved MSR (SRR1)
      b     _save_int_regs_noncrit_skip
_save_int_regs_non_crit:
#endif
      mfspr r3,srr0    // Get SRR0  (Return address)
      stw   r3,0(r1)
      mfspr r3,srr1    // Read saved MSR (SRR1)

_save_int_regs_noncrit_skip:
     #if MQXCFG_ENABLE_DSP && PSP_HAS_DSP
         oris  r3,r3,0x0200 // set Altivec enable bit
         ori   r3,r3,0x2000 // set FP available bit
     #elif MQXCFG_ENABLE_FP && PSP_HAS_FPU
         ori   r3,r3,0x2000 // set FP available bit
     #endif
     stw   r3,20(r1)

    .endm

// called with interrupts disabled
SAVE_REST_ISR_REGISTERS: .macro
    SAVE_R13_R31
    SAVE_SPE_REGS
    .endm
   
// Called with interrupts disabled
RESTORE_ISR_REGISTERS: .macro
      RESTORE_VOLATILE_FP_REGS

      LDREG  r3, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*1(r1)
      LDREG  r4, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*2(r1)
      LDREG  r5, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*3(r1)
      LDREG  r6, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*4(r1)
      LDREG  r7, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*5(r1)
      LDREG  r8, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*6(r1)
      LDREG  r9, PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*7(r1)
      LDREG  r10,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*8(r1)
      LDREG  r11,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*9(r1)
      LDREG  r12,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*10(r1)

      lwz   r0,12(r1)
      mtxer r0         // Restore XER

      lwz   r0,8(r1)
      mtctr r0         // Restore CTR

      lwz   r0,4(r1)
      mtlr  r0         // Get Link Register  (Return address)

      CLEAR_RI         // clear the RI bit in the MSR

      lwz   r0,16(r1)
      mtcrf 0xFF,r0    // Restore CCR

      lwz   r0,0(r1)
      mtspr srr0,r0    // and put into SRR0

      lwz   r0,20(r1)
      mtspr srr1,r0    // GET MSR, and put into SRR1

      LDREG   r0,PSP_GP_REGISTERS_OFFSET+PSP_SAVED_GP_REGISTER_SIZE*0(r1)

      addi r1,r1,PSP_BLOCKED_STACK_STRUCT_SIZE
   .endm

// called with interrupts disabled
RESTORE_REST_ISR_REGISTERS: .macro 
   RESTORE_SPE_REGS
   RESTORE_R13_R31
   .endm

// Called with interrupt disabled
SAVE_DSP_REGISTERS: .macro 
   #if defined(PSP_MPC7400)
      
      addi r3,r3,16
      stvx  v0,r0,r3
     
      addi r3,r3,-16
      mfvscr  v0      
      stvx  v0,r0,r3
      
      addi r3,r3,32
      stvx  v1,r0,r3
      
      addi r3,r3,16
      stvx  v2,r0,r3
      
      addi r3,r3,16
      stvx  v3,r0,r3
      
      addi r3,r3,16
      stvx  v4,r0,r3
      
      addi r3,r3,16
      stvx  v5,r0,r3
      
      addi r3,r3,16
      stvx  v6,r0,r3
      
      addi r3,r3,16
      stvx  v7,r0,r3
      
      addi r3,r3,16
      stvx  v8,r0,r3
      
      addi r3,r3,16
      stvx  v9,r0,r3
      
      addi r3,r3,16
      stvx  v10,r0,r3
      
      addi r3,r3,16
      stvx  v11,r0,r3
      
      addi r3,r3,16
      stvx  v12,r0,r3
      
      addi r3,r3,16
      stvx  v13,r0,r3
      
      addi r3,r3,16
      stvx  v14,r0,r3
      
      addi r3,r3,16
      stvx  v15,r0,r3
  
      addi r3,r3,16
      stvx  v16,r0,r3
      
      addi r3,r3,16
      stvx  v17,r0,r3
      
      addi r3,r3,16
      stvx  v18,r0,r3
      
      addi r3,r3,16
      stvx  v19,r0,r3
      
      addi r3,r3,16
      stvx  v20,r0,r3
      
      addi r3,r3,16
      stvx  v21,r0,r3
      
      addi r3,r3,16
      stvx  v22,r0,r3
      
      addi r3,r3,16
      stvx  v23,r0,r3
      
      addi r3,r3,16
      stvx  v24,r0,r3
      
      addi r3,r3,16
      stvx  v25,r0,r3
      
      addi r3,r3,16
      stvx  v26,r0,r3
      
      addi r3,r3,16
      stvx  v27,r0,r3
      
      addi r3,r3,16
      stvx  v28,r0,r3

      addi r3,r3,16
      stvx  v29,r0,r3
      
      addi r3,r3,16
      stvx  v30,r0,r3
      
      addi r3,r3,16
      stvx  v31,r0,r3
      
   #endif
   .endm
   
// called with interrupts disabled
RESTORE_DSP_REGISTERS: .macro
   #if defined(PSP_MPC7400)
     
      lvx  v0,r0,r3
      mtvscr v0      
      
      addi r3,r3,16
      lvx  v0,r0,r3
      
      addi r3,r3,16
      lvx  v1,r0,r3
      
      addi r3,r3,16
      lvx  v2,r0,r3
      
      addi r3,r3,16
      lvx  v3,r0,r3
      
      addi r3,r3,16
      lvx  v4,r0,r3
      
      addi r3,r3,16
      lvx  v5,r0,r3
      
      addi r3,r3,16
      lvx  v6,r0,r3
      
      addi r3,r3,16
      lvx  v7,r0,r3
      
      addi r3,r3,16
      lvx  v8,r0,r3
      
      addi r3,r3,16
      lvx  v9,r0,r3
      
      addi r3,r3,16
      lvx  v10,r0,r3
      
      addi r3,r3,16
      lvx  v11,r0,r3
      
      addi r3,r3,16
      lvx  v12,r0,r3
      
      addi r3,r3,16
      lvx  v13,r0,r3
      
      addi r3,r3,16
      lvx  v14,r0,r3
      
      addi r3,r3,16
      lvx  v15,r0,r3
      
      addi r3,r3,16
      lvx  v16,r0,r3
      
      addi r3,r3,16
      lvx  v17,r0,r3
      
      addi r3,r3,16
      lvx  v18,r0,r3
      
      addi r3,r3,16
      lvx  v19,r0,r3
      
      addi r3,r3,16
      lvx  v20,r0,r3
      
      addi r3,r3,16
      lvx  v21,r0,r3
      
      addi r3,r3,16
      lvx  v22,r0,r3
      
      addi r3,r3,16
      lvx  v23,r0,r3
      
      addi r3,r3,16
      lvx  v24,r0,r3
      
      addi r3,r3,16
      lvx  v25,r0,r3
      
      addi r3,r3,16
      lvx  v26,r0,r3
      
      addi r3,r3,16
      lvx  v27,r0,r3
      
      addi r3,r3,16
      lvx  v28,r0,r3
      
      addi r3,r3,16
      lvx  v29,r0,r3
      
      addi r3,r3,16
      lvx  v30,r0,r3
      
      addi r3,r3,16
      lvx  v31,r0,r3

   #endif
   .endm
   
#if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
FP_REGISTERS_SIZE .equ 8*(33 - PSP_NUM_VOLATILE_FP_REGS)

// Called with interrupt disabled
   SAVE_FLOATING_POINT_REGISTERS: .macro 

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor generic
      #endif

      stfd  f14,PSP_SAVED_FP_REGISTER_SIZE*1(r1)
      mffs  f14
      stfd  f14,PSP_SAVED_FP_REGISTER_SIZE*0(r1)

      stfd  f15,PSP_SAVED_FP_REGISTER_SIZE*2(r1)
      stfd  f16,PSP_SAVED_FP_REGISTER_SIZE*3(r1)
      stfd  f17,PSP_SAVED_FP_REGISTER_SIZE*4(r1)
      stfd  f18,PSP_SAVED_FP_REGISTER_SIZE*5(r1)
      stfd  f19,PSP_SAVED_FP_REGISTER_SIZE*6(r1)
      stfd  f20,PSP_SAVED_FP_REGISTER_SIZE*7(r1)
      stfd  f21,PSP_SAVED_FP_REGISTER_SIZE*8(r1)
      stfd  f22,PSP_SAVED_FP_REGISTER_SIZE*9(r1)
      stfd  f23,PSP_SAVED_FP_REGISTER_SIZE*10(r1)
      stfd  f24,PSP_SAVED_FP_REGISTER_SIZE*11(r1)
      stfd  f25,PSP_SAVED_FP_REGISTER_SIZE*12(r1)
      stfd  f26,PSP_SAVED_FP_REGISTER_SIZE*13(r1)
      stfd  f27,PSP_SAVED_FP_REGISTER_SIZE*14(r1)
      stfd  f28,PSP_SAVED_FP_REGISTER_SIZE*15(r1)
      stfd  f29,PSP_SAVED_FP_REGISTER_SIZE*16(r1)
      stfd  f30,PSP_SAVED_FP_REGISTER_SIZE*17(r1)
      stfd  f31,PSP_SAVED_FP_REGISTER_SIZE*18(r1)

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor 8260
      #endif

   .endm

// called with interrupts disabled
   RESTORE_FLOATING_POINT_REGISTERS: .macro

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor generic
      #endif

      lfd  f14,PSP_SAVED_FP_REGISTER_SIZE*0(r1)
      mtfsf 255,f14
      lfd  f14,PSP_SAVED_FP_REGISTER_SIZE*1(r1)
      lfd  f15,PSP_SAVED_FP_REGISTER_SIZE*2(r1)
      lfd  f16,PSP_SAVED_FP_REGISTER_SIZE*3(r1)
      lfd  f17,PSP_SAVED_FP_REGISTER_SIZE*4(r1)
      lfd  f18,PSP_SAVED_FP_REGISTER_SIZE*5(r1)
      lfd  f19,PSP_SAVED_FP_REGISTER_SIZE*6(r1)
      lfd  f20,PSP_SAVED_FP_REGISTER_SIZE*7(r1)
      lfd  f21,PSP_SAVED_FP_REGISTER_SIZE*8(r1)
      lfd  f22,PSP_SAVED_FP_REGISTER_SIZE*9(r1)
      lfd  f23,PSP_SAVED_FP_REGISTER_SIZE*10(r1)
      lfd  f24,PSP_SAVED_FP_REGISTER_SIZE*11(r1)
      lfd  f25,PSP_SAVED_FP_REGISTER_SIZE*12(r1)
      lfd  f26,PSP_SAVED_FP_REGISTER_SIZE*13(r1)
      lfd  f27,PSP_SAVED_FP_REGISTER_SIZE*14(r1)
      lfd  f28,PSP_SAVED_FP_REGISTER_SIZE*15(r1)
      lfd  f29,PSP_SAVED_FP_REGISTER_SIZE*16(r1)
      lfd  f30,PSP_SAVED_FP_REGISTER_SIZE*17(r1)
      lfd  f31,PSP_SAVED_FP_REGISTER_SIZE*18(r1)

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor 8260
      #endif

   .endm
#else
FP_REGISTERS_SIZE .equ 8*33 

// Called with interrupt disabled
   SAVE_FLOATING_POINT_REGISTERS: .macro 

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor generic
      #endif

      stfd  f0,PSP_SAVED_FP_REGISTER_SIZE*1(r1)

      mffs  f0      
      stfd  f0,PSP_SAVED_FP_REGISTER_SIZE*0(r1)

      stfd  f1,PSP_SAVED_FP_REGISTER_SIZE*2(r1)
      stfd  f2,PSP_SAVED_FP_REGISTER_SIZE*3(r1)
      stfd  f3,PSP_SAVED_FP_REGISTER_SIZE*4(r1)
      stfd  f4,PSP_SAVED_FP_REGISTER_SIZE*5(r1)
      stfd  f5,PSP_SAVED_FP_REGISTER_SIZE*6(r1)
      stfd  f6,PSP_SAVED_FP_REGISTER_SIZE*7(r1)
      stfd  f7,PSP_SAVED_FP_REGISTER_SIZE*8(r1)
      stfd  f8,PSP_SAVED_FP_REGISTER_SIZE*9(r1)
      stfd  f9,PSP_SAVED_FP_REGISTER_SIZE*10(r1)
      stfd  f10,PSP_SAVED_FP_REGISTER_SIZE*11(r1)
      stfd  f11,PSP_SAVED_FP_REGISTER_SIZE*12(r1)
      stfd  f12,PSP_SAVED_FP_REGISTER_SIZE*13(r1)
      stfd  f13,PSP_SAVED_FP_REGISTER_SIZE*14(r1)
      stfd  f14,PSP_SAVED_FP_REGISTER_SIZE*15(r1)
      stfd  f15,PSP_SAVED_FP_REGISTER_SIZE*16(r1)
      stfd  f16,PSP_SAVED_FP_REGISTER_SIZE*17(r1)
      stfd  f17,PSP_SAVED_FP_REGISTER_SIZE*18(r1)
      stfd  f18,PSP_SAVED_FP_REGISTER_SIZE*19(r1)
      stfd  f19,PSP_SAVED_FP_REGISTER_SIZE*20(r1)
      stfd  f20,PSP_SAVED_FP_REGISTER_SIZE*21(r1)
      stfd  f21,PSP_SAVED_FP_REGISTER_SIZE*22(r1)
      stfd  f22,PSP_SAVED_FP_REGISTER_SIZE*23(r1)
      stfd  f23,PSP_SAVED_FP_REGISTER_SIZE*24(r1)
      stfd  f24,PSP_SAVED_FP_REGISTER_SIZE*25(r1)
      stfd  f25,PSP_SAVED_FP_REGISTER_SIZE*26(r1)
      stfd  f26,PSP_SAVED_FP_REGISTER_SIZE*27(r1)
      stfd  f27,PSP_SAVED_FP_REGISTER_SIZE*28(r1)
      stfd  f28,PSP_SAVED_FP_REGISTER_SIZE*29(r1)
      stfd  f29,PSP_SAVED_FP_REGISTER_SIZE*30(r1)
      stfd  f30,PSP_SAVED_FP_REGISTER_SIZE*31(r1)
      stfd  f31,PSP_SAVED_FP_REGISTER_SIZE*32(r1)

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor 8260
      #endif

   .endm

// called with interrupts disabled
   RESTORE_FLOATING_POINT_REGISTERS: .macro

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor generic
      #endif

      lfd  f0,PSP_SAVED_FP_REGISTER_SIZE*0(r1)
      mtfsf 255,f0
      lfd  f0,PSP_SAVED_FP_REGISTER_SIZE*1(r1)
      lfd  f1,PSP_SAVED_FP_REGISTER_SIZE*2(r1)
      lfd  f2,PSP_SAVED_FP_REGISTER_SIZE*3(r1)
      lfd  f3,PSP_SAVED_FP_REGISTER_SIZE*4(r1)
      lfd  f4,PSP_SAVED_FP_REGISTER_SIZE*5(r1)
      lfd  f5,PSP_SAVED_FP_REGISTER_SIZE*6(r1)
      lfd  f6,PSP_SAVED_FP_REGISTER_SIZE*7(r1)
      lfd  f7,PSP_SAVED_FP_REGISTER_SIZE*8(r1)
      lfd  f8,PSP_SAVED_FP_REGISTER_SIZE*9(r1)
      lfd  f9,PSP_SAVED_FP_REGISTER_SIZE*10(r1)
      lfd  f10,PSP_SAVED_FP_REGISTER_SIZE*11(r1)
      lfd  f11,PSP_SAVED_FP_REGISTER_SIZE*12(r1)
      lfd  f12,PSP_SAVED_FP_REGISTER_SIZE*13(r1)
      lfd  f13,PSP_SAVED_FP_REGISTER_SIZE*14(r1)
      lfd  f14,PSP_SAVED_FP_REGISTER_SIZE*15(r1)
      lfd  f15,PSP_SAVED_FP_REGISTER_SIZE*16(r1)
      lfd  f16,PSP_SAVED_FP_REGISTER_SIZE*17(r1)
      lfd  f17,PSP_SAVED_FP_REGISTER_SIZE*18(r1)
      lfd  f18,PSP_SAVED_FP_REGISTER_SIZE*19(r1)
      lfd  f19,PSP_SAVED_FP_REGISTER_SIZE*20(r1)
      lfd  f20,PSP_SAVED_FP_REGISTER_SIZE*21(r1)
      lfd  f21,PSP_SAVED_FP_REGISTER_SIZE*22(r1)
      lfd  f22,PSP_SAVED_FP_REGISTER_SIZE*23(r1)
      lfd  f23,PSP_SAVED_FP_REGISTER_SIZE*24(r1)
      lfd  f24,PSP_SAVED_FP_REGISTER_SIZE*25(r1)
      lfd  f25,PSP_SAVED_FP_REGISTER_SIZE*26(r1)
      lfd  f26,PSP_SAVED_FP_REGISTER_SIZE*27(r1)
      lfd  f27,PSP_SAVED_FP_REGISTER_SIZE*28(r1)
      lfd  f28,PSP_SAVED_FP_REGISTER_SIZE*29(r1)
      lfd  f29,PSP_SAVED_FP_REGISTER_SIZE*30(r1)
      lfd  f30,PSP_SAVED_FP_REGISTER_SIZE*31(r1)
      lfd  f31,PSP_SAVED_FP_REGISTER_SIZE*32(r1)

      // needed to resolve CodeWarrior 8.0 assembler bug regarding floating
      // point instructions
      #if defined(PSP_G2_CORE)
            .option processor 8260
      #endif

   .endm
#endif

// This macro calls the kernel logging function, if logging enabled
KLOG: .macro KDATA,KLOG_FUNCTION
        #if MQX_KERNEL_LOGGING
        lwz     r3,KD_LOG_CONTROL(KDATA)
        andi.   r3,r3,1
        beq     no_log\@
        subi    r1,r1,8        // Make room for Link Register and SP
        bl      KLOG_FUNCTION
        addi    r1,r1,8
no_log\@:
        #endif
   .endm

// This macro calls the profiling save context function
PROF_SAVE: .macro 
        subi    r1,r1,8        // Make room for Link Register and SP
        bl      _profiler_save_context
        addi    r1,r1,8
    .endm

// This macro calls the profiling restore context function
PROF_RESTORE: .macro 
        subi    r1,r1,8        // Make room for Link Register and SP
        bl      _profiler_restore_context
        addi    r1,r1,8
    .endm

// This macro calls the run-time error checking save context function
RTEC_SAVE: .macro 
        subi    r1,r1,8        // Make room for Link Register and SP
        bl      _rterrchk_save_context
        addi    r1,r1,8
     .endm

// This macro calls the run-time error checking restore context function
RTEC_RESTORE: .macro 
        subi    r1,r1,8        // Make room for Link Register and SP
        bl      _rterrchk_restore_context
        addi    r1,r1,8
     .endm

//------------------------------------------------------------------------
//                         EXTERNAL REFERENCES
//
        .extern   _mmu_reset_vcontext_internal
        .extern   _mmu_set_vcontext_internal

        #if MQX_PROFILING_ENABLE
        .extern   _profiler_save_context
        .extern   _profiler_restore_context
        #endif

        #if MQX_RUN_TIME_ERR_CHECK_ENABLE
        .extern   _rterrchk_save_context
        .extern   _rterrchk_restore_context
        #endif

        #if MQX_KERNEL_LOGGING
        .extern   _klog_block_internal
        .extern   _klog_execute_scheduler_internal
        .extern   _klog_yield_internal
        .extern   _klog_context_switch_internal
        .extern   _klog_isr_start_internal
        .extern   _klog_isr_end_internal
        #endif

        .extern   _mqx_kernel_data
        .extern   _mqx_system_stack       


// EOF
