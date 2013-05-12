#ifndef __powerpc_h__
#define __powerpc_h__
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
* $FileName: powerpc.h$
* $Version : 3.8.8.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

/*==========================================================================*/
/*
**                   MQX REQUIRED DEFINITIONS
**
** Other MQX kernel and component functions require these definitions to exist.
*/




#define PSP_HAS_SUPPORT_STRUCT     1




#ifndef PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS
#error Define PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS 
#endif

/* Indicate which endian this PSP is */
#define PSP_ENDIAN                 MQX_BIG_ENDIAN

/*
** Memory alignment requirements.
** The alignment indicates how memory is to be aligned for all memory
** accesses.   This is to avoid mis-aligned transfers of data, thus
** optimizing memory accesses.
*/
#define PSP_MEMORY_ALIGNMENT       (PSP_CACHE_LINE_SIZE-1)
#define PSP_MEMORY_ALIGNMENT_MASK  (~PSP_MEMORY_ALIGNMENT)

/*
** Stack alignment requirements.
** The alignment indicates how the stack is to be initially aligned.
** This is to avoid mis-aligned types on the stack.
** Must be 8 bit aligned according to the PPC EABI (16 for MetaWare's
** implementation of AltiVec).
*/
#define PSP_STACK_ALIGNMENT       (16-1)
#define PSP_STACK_ALIGNMENT_MASK  (~PSP_STACK_ALIGNMENT)

/*
** Define padding needed to make the STOREBLOCK_STRUCT aligned properly
*/
/* The definition of  PSP_MEM_STOREBLOCK_ALIGNMENT has been moved to */
/* the processor include file, e.g. mpc8260.h */

/*
** Indicate the direction of the stack
*/
#define PSP_STACK_GROWS_TO_LOWER_MEM               (1)

/*
** Indicate addressing capability of the CPU
** This is the memory width. i.e., the number of bits addressed
** by each memory location.
*/
#define PSP_MEMORY_ADDRESSING_CAPABILITY (8)

/*
** Indicate alignment restrictions on memory accesses
** For an n-bit memory access,
**
** if n <  PSP_MEMORY_ACCESSING_CAPABILITY,
**         n-bit accesses must be n-bit-aligned
**
** if n >= PSP_MEMORY_ACCESSING_CAPABILITY,
**         n-bit accesses must be PSP_MEMORY_ACCESSING_CAPABILITY-bit-aligned
** May be overridden by CPU specific header file (e.g. mpc5674.h)
*/
#ifndef PSP_MEMORY_ACCESSING_CAPABILITY
#define PSP_MEMORY_ACCESSING_CAPABILITY (8)
#endif

/*
** The maximum number of hardware interrupt vectors.
** Define this macro in the CPU-specific header file (e.g mpc566.h).
*/
//#define PSP_MAXIMUM_INTERRUPT_VECTORS   (48)

/*
**   MINIMUM STACK SIZE FOR A TASK
*/

/*
** Memory overhead on users stack before switching over to the interrupt stack.
**
**
*/
#if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
   #define PSP_NUM_VOLATILE_FP_REGS      (14)   /* Must match the corresponding setting in psp_prv.cw */
   #define PSP_STACK_INTERRUPT_OVERHEAD  (116 + (8 * PSP_NUM_VOLATILE_FP_REGS))
#else
   #define PSP_STACK_INTERRUPT_OVERHEAD  (116)
#endif

/* This much extra stack is required for the logging of mqx functions */
#if MQX_KERNEL_LOGGING
#define PSP_STACK_KLOG_OVERHEAD       (128)
#else
#define PSP_STACK_KLOG_OVERHEAD         (0)
#endif

/* This much extra stack is required if a task destroys itself. */
#if MQX_TASK_DESTRUCTION
#define PSP_TASK_DESTRUCTION_STACK_OVERHEAD (88)
#else
#define PSP_TASK_DESTRUCTION_STACK_OVERHEAD (0)
#endif

/* Minimum stack size for all tasks. */
#define PSP_MINSTACKSIZE \
   (sizeof(PSP_STACK_START_STRUCT) + \
   PSP_TASK_DESTRUCTION_STACK_OVERHEAD + \
   PSP_STACK_INTERRUPT_OVERHEAD + \
   PSP_STACK_KLOG_OVERHEAD)


/* Minimum stack size for the Idle Task
**   Idle task written in C uses at most 8 bytes of stack:
**     1 Link Instruction, 1 local variable (pointer)
*/
#define PSP_IDLE_STACK_SIZE (PSP_MINSTACKSIZE + 8)

/* PowerPC addresses do not need to be normalized
** (ie as for example the Intel chips in real mode do require this)
*/
#define _PSP_NORMALIZE_MEMORY(x) (x)

/*
** Standard cache macros
*/
#define _DCACHE_ENABLE(n)                _dcache_enable(n)
#define _DCACHE_DISABLE()                _dcache_disable()
#define _DCACHE_FLUSH(p)                 _dcache_flush()
#define _DCACHE_FLUSH_LINE(p)            _dcache_flush_line(p)
#define _DCACHE_FLUSH_MLINES(p, n)       _dcache_flush_mlines(p, n)
#define _DCACHE_INVALIDATE()             _dcache_invalidate()
#define _DCACHE_INVALIDATE_LINE(p)       _dcache_invalidate_line(p)
#define _DCACHE_INVALIDATE_MLINES(p, n)  _dcache_invalidate_mlines(p, n)
#define _ICACHE_ENABLE(n)                _icache_enable(n)
#define _ICACHE_DISABLE()                _icache_disable()
#define _ICACHE_INVALIDATE()             _icache_invalidate()
#define _ICACHE_INVALIDATE_LINE(p)       _icache_invalidate_line(p)
#define _ICACHE_INVALIDATE_MLINES(p, n)  _icache_invalidate_mlines(p, n)

#define _DCACHE_FLUSH_MBYTES             _DCACHE_FLUSH_MLINES
#define _DCACHE_INVALIDATE_MBYTES        _DCACHE_INVALIDATE_MLINES

/*==========================================================================*/
/*
**                    PSP SPECIFIC DEFINITIONS
**
** These definitions will change from PSP to PSP
*/

/* Processor family */
#define PSP_POWERPC 1

#if defined(PSP_MPC801) ||defined(PSP_MPC821) || defined(PSP_MPC823) ||  defined(PSP_MPC850) ||defined(PSP_MPC855) || defined(PSP_MPC860)
   #define PSP_HANDLE_PPC_SPR_BUG 0
#else
   #define PSP_HANDLE_PPC_SPR_BUG 1
#endif



#if defined(PSP_AMCC405)||defined(PSP_AMCC4440)||defined(PSP_MPC8560)||defined(PSP_E500_CORE)||defined(PSP_E200_CORE)
   #define PSP_KD_IN_SPRG7 1
#else
   #define PSP_KD_IN_SPRG7 0
#endif

#if defined(PSP_AMCC401)||defined(PSP_AMCC403)||defined(PSP_AMCC480)||defined(PSP_AMCC405)||defined(PSP_AMCC440)||defined(PSP_E500_CORE)||defined(PSP_E200_CORE)
   #define PSP_PPC_GROUP 0
#elif defined(PSP_MPC603)||defined(PSP_MPC604)||defined(PSP_MPC740)||defined(PSP_MPC750)||defined(PSP_MPC7400)||defined(PSP_MPC8240)|| defined(PSP_G2_CORE)
    #define PSP_PPC_GROUP 1
#elif defined(PSP_G2_LE_CORE) || defined(PSP_E300_CORE)
   #define PSP_PPC_GROUP 2
#elif defined(PSP_MPC505)||defined(PSP_MPC555)||defined(PSP_MPC566)||defined(PSP_MPC801)|| \
      defined(PSP_MPC821)||defined(PSP_MPC823)||defined(PSP_MPC850)||defined(PSP_MPC855)||defined(PSP_MPC860)||defined(PSP_MPC875)
   #define PSP_PPC_GROUP 3
#else
   #error undefined PPC group
#endif





#ifdef PSP_BOOKE
// PowerPC Book-E Defined exception numbers (16..31 are reserved)
#define PSP_EXCPT_CRITICAL                      ( 0)
#define PSP_EXCPT_MACHINE_CHECK                 ( 1)
#define PSP_EXCPT_DATA_STORAGE                  ( 2)
#define PSP_EXCPT_INSTRUCTION_STORAGE           ( 3)
#define PSP_EXCPT_EXTERNAL                      ( 4)
#define PSP_EXCPT_ALIGNMENT                     ( 5)
#define PSP_EXCPT_PROGRAM                       ( 6)
#define PSP_EXCPT_FLOATING_POINT_UNAVAILABLE    ( 7)
#define PSP_EXCPT_SYSTEM_CALL                   ( 8)
#define PSP_EXCPT_AUX_PROC_UNAVAILABLE          ( 9)
#define PSP_EXCPT_DECREMENTER                   (10)
#define PSP_EXCPT_FIXED_INTERVAL_TIMER          (11)
#define PSP_EXCPT_WATCHDOG_TIMER                (12)
#define PSP_EXCPT_DATA_TLB_ERROR                (13)
#define PSP_EXCPT_INSTRUCTION_TLB_ERROR         (14)
#define PSP_EXCPT_DEBUG                         (15)
#else
/*
** Generic PowerPC Exception numbers
*/
#define PSP_EXCPT_RESET                         ( 1)
#define PSP_EXCPT_MACHINE_CHECK                 ( 2)
#define PSP_EXCPT_DATA_STORAGE                  ( 3)
#define PSP_EXCPT_INSTRUCTION_STORAGE           ( 4)
#define PSP_EXCPT_EXTERNAL                      ( 5)
#define PSP_EXCPT_ALIGNMENT                     ( 6)
#define PSP_EXCPT_PROGRAM                       ( 7)
#define PSP_EXCPT_FLOATING_POINT_UNAVAILABLE    ( 8)
#define PSP_EXCPT_DECREMENTER                   ( 9)
#define PSP_EXCPT_SYSTEM_CALL                   (12)
#define PSP_EXCPT_TRACE                         (13)
#define PSP_EXCPT_FLOATING_POINT_ASSIST         (14)
#endif

/*
** The following define the Condition register bits
*/
#define PSP_CR_LT             (0x80000000)
#define PSP_CR_GT             (0x40000000)
#define PSP_CR_EQ             (0x20000000)
#define PSP_CR_SO             (0x10000000)
#define PSP_CR_FX             (0x08000000)
#define PSP_CR_FEX            (0x04000000)
#define PSP_CR_VX             (0x02000000)
#define PSP_CR_OX             (0x01000000)
#define PSP_CR2_LT            (0x00800000)
#define PSP_CR2_GT            (0x00400000)
#define PSP_CR2_EQ            (0x00200000)
#define PSP_CR2_SO            (0x00100000)
#define PSP_CR3_LT            (0x00080000)
#define PSP_CR3_GT            (0x00040000)
#define PSP_CR3_EQ            (0x00020000)
#define PSP_CR3_SO            (0x00010000)
#define PSP_CR4_LT            (0x00008000)
#define PSP_CR4_GT            (0x00004000)
#define PSP_CR4_EQ            (0x00002000)
#define PSP_CR4_SO            (0x00001000)
#define PSP_CR5_LT            (0x00000800)
#define PSP_CR5_GT            (0x00000400)
#define PSP_CR5_EQ            (0x00000200)
#define PSP_CR5_SO            (0x00000100)
#define PSP_CR6_LT            (0x00000080)
#define PSP_CR6_GT            (0x00000040)
#define PSP_CR6_EQ            (0x00000020)
#define PSP_CR6_SO            (0x00000010)
#define PSP_CR7_LT            (0x00000008)
#define PSP_CR7_GT            (0x00000004)
#define PSP_CR7_EQ            (0x00000002)
#define PSP_CR7_SO            (0x00000001)

/*
** The following define the Machine State Register bits
*/
#define PSP_MSR_POW           (0x00040000)
#define PSP_MSR_TGPR          (0x00020000)
#define PSP_MSR_ILE           (0x00010000)
#define PSP_MSR_EE            (0x00008000)
#define PSP_MSR_PR            (0x00004000)
#define PSP_MSR_FP            (0x00002000)
#define PSP_MSR_ME            (0x00001000)
#define PSP_MSR_FE0           (0x00000800)
#define PSP_MSR_SE            (0x00000400)
#define PSP_MSR_BE            (0x00000200)
#define PSP_MSR_FE1           (0x00000100)
#define PSP_MSR_IP            (0x00000040)
#define PSP_MSR_IR            (0x00000020)
#define PSP_MSR_DR            (0x00000010)
#define PSP_MSR_RI            (0x00000002)
#define PSP_MSR_LE            (0x00000001)

/* MQX Register offsets NOTE: r1 is stack pointer
** R2 and R13 saved in EABI mode for pointing to small const and small data
*/
#define PSP_GPR_0  (0)
#define PSP_GPR_3  (1)
#define PSP_GPR_4  (2)
#define PSP_GPR_5  (3)
#define PSP_GPR_6  (4)
#define PSP_GPR_7  (5)
#define PSP_GPR_8  (6)
#define PSP_GPR_9  (7)
#define PSP_GPR_10 (8)
#define PSP_GPR_11 (9)
#define PSP_GPR_12 (10)
#define PSP_GPR_13 (11)
#define PSP_GPR_14 (12)
#define PSP_GPR_15 (13)
#define PSP_GPR_16 (14)
#define PSP_GPR_17 (15)
#define PSP_GPR_18 (16)
#define PSP_GPR_19 (17)
#define PSP_GPR_20 (18)
#define PSP_GPR_21 (19)
#define PSP_GPR_22 (20)
#define PSP_GPR_23 (21)
#define PSP_GPR_24 (22)
#define PSP_GPR_25 (23)
#define PSP_GPR_26 (24)
#define PSP_GPR_27 (25)
#define PSP_GPR_28 (26)
#define PSP_GPR_29 (27)
#define PSP_GPR_30 (28)
#define PSP_GPR_31 (29)

// Constants to be used in assembler for saving/restoring registers
// We do all this so we don't have hardcoded values in psp_prv.cw

#define PSP_NUM_SAVED_GP_REGISTERS      30
#define PSP_NUM_SAVED_EXTRA_REGISTERS   6
#define PSP_SAVED_EXTRA_REGISTE_SIZE    4
#define PSP_NUM_FP_REGISTERS            32
#define PSP_SAVED_FP_REGISTER_SIZE      8


#if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
    #define PSP_SAVED_GP_REGISTER_SIZE 8
    #define PSP_SIZE_SAVED_SPE_REGISTERS    (12)
#else
    #define PSP_SAVED_GP_REGISTER_SIZE 4
    #define PSP_SIZE_SAVED_SPE_REGISTERS    (0)
#endif
#if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
    #define PSP_SIZE_SAVED_VOLATILE_FP_REGISTERS    (PSP_NUM_VOLATILE_FP_REGS*PSP_SAVED_FP_REGISTER_SIZE)
    //#define FP_REGISTERS_SIZE                       (PSP_SAVED_FP_REGISTER_SIZE*(33 - PSP_NUM_VOLATILE_FP_REGS))
#else
    #define PSP_SIZE_SAVED_VOLATILE_FP_REGISTERS    (0)
   // #define FP_REGISTERS_SIZE                       (PSP_SAVED_FP_REGISTER_SIZE*33)
#endif

#define PSP_SIZE_SAVED_EXTRA_REGISTERS          (PSP_NUM_SAVED_EXTRA_REGISTERS*PSP_SAVED_EXTRA_REGISTE_SIZE)
#define PSP_SIZE_SAVED_GP_REGISTERS             (PSP_SAVED_GP_REGISTER_SIZE*PSP_NUM_SAVED_GP_REGISTERS)

#define PSP_GP_REGISTERS_OFFSET                 (PSP_SIZE_SAVED_EXTRA_REGISTERS)
#define PSP_GP13_REGISTERS_OFFSET               (PSP_SIZE_SAVED_EXTRA_REGISTERS + PSP_SAVED_GP_REGISTER_SIZE*11)
#define PSP_VOLATILE_FP_REGISTERS_OFFSET        (PSP_SIZE_SAVED_EXTRA_REGISTERS + PSP_SIZE_SAVED_GP_REGISTERS)
#define PSP_SPE_REGISTERS_OFFSET        (PSP_SIZE_SAVED_EXTRA_REGISTERS + PSP_SIZE_SAVED_GP_REGISTERS)

#define PSP_BLOCKED_STACK_STRUCT_SIZE (PSP_SIZE_SAVED_EXTRA_REGISTERS + PSP_SIZE_SAVED_GP_REGISTERS + PSP_SIZE_SAVED_VOLATILE_FP_REGISTERS + PSP_SIZE_SAVED_SPE_REGISTERS)


#ifndef __ASM__

/*==========================================================================*/
/*                         PROCESSOR SPECIFIC DATA TYPES                    */

/*-----------------------------------------------------------------------*/
/*
** PSP FUNCTION CALL STRUCT
**
** This structure defines what a function call pushes onto the stack
*/
typedef struct psp_function_call_struct
{

   /* The location of the previous SP */
   pointer              SP_REGISTER;

   /* Pushed onto the stack by the JSR instruction */
   void     (_CODE_PTR_ RETURN_ADDRESS)(void);

} PSP_FUNCTION_CALL_STRUCT, _PTR_ PSP_FUNCTION_CALL_STRUCT_PTR;


/*==========================================================================*/
/*                         MQX DATA TYPES                                   */

#if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
typedef uint_64 _psp_register;
#else
typedef uint_32 _psp_register;
#endif

/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED STACK STRUCT
**
** This is what a stack looks like for a task that is NOT the active
** task
*/
typedef struct psp_blocked_stack_struct
{
   void    (_CODE_PTR_ RETURN_ADDRESS)(uint_32);  /* SRR0 */
   uint_32 LR;   /* Link register */
   uint_32 CTR;
   uint_32 XER;
   uint_32 CCR;
   uint_32 MSR;
   /* General purpose registers R0, R3-r31 */
  
   _psp_register GPR[PSP_NUM_SAVED_GP_REGISTERS];
   #if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
   /* Floating point registers */
   uint_64 FPR[PSP_NUM_VOLATILE_FP_REGS];
   #elif (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS == 1)
   uint_64  ACC;
   uint_32  SPEFSCR;
   #endif
} PSP_BLOCKED_STACK_STRUCT, _PTR_ PSP_BLOCKED_STACK_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED FP STACK STRUCT
**
** This structure defines the registers stored on the stack by MQX
** when a floating point task is blocked.  These are stored asynchronously
** onto the stack, after the PSP BLOCKED STACK STRUCT has been pushed.
** When the FLOATING_POINT_REGISTERS_STORED bit is set
** task's FLAGS, then this structure is present on the stack.
*/
typedef struct psp_blocked_fp_stack_struct
{
   double  FPSCR;
   /* Start CR 1724 */
   #if (PSP_ALWAYS_SAVE_AND_RESTORE_VOLATILE_FP_REGS == 1)
   double  FPR[32 - PSP_NUM_VOLATILE_FP_REGS];
   #else
   double  FPR[32];
   #endif
   /* End CR 1724 */
} PSP_BLOCKED_FP_STACK_STRUCT, _PTR_ PSP_BLOCKED_FP_STACK_STRUCT_PTR;

/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED DSP (ALTIVEC) STACK STRUCT
**
** This structure defines the registers stored on the stack by MQX
** when a dsp task is blocked.  These are stored asynchronously
** onto the stack, after the PSP BLOCKED STACK STRUCT has been pushed.
** When the DSP_REGISTERS_STORED bit is set
** task's FLAGS, then this structure is present on the stack.
*/
typedef struct psp_blocked_dsp_stack_struct
{
   double  VSCR[2];
   double  VR[64];
} PSP_BLOCKED_DSP_STACK_STRUCT, _PTR_ PSP_BLOCKED_DSP_STACK_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP INTERRUPT CONTEXT STRUCT
**
** This structure provides a "context" for mqx primitives to use while executing
** in an interrupt handler.  A link list of these contexts is built on the
** interrupt stack.  The head of this list (the current interrupt context) is
** pointed to by the INTERRUPT_CONTEXT_PTR field of the kernel data structure.
**
*/
typedef struct psp_int_context_struct
{

   /* The "task" error code for use by mqx functions while in the ISR */
   uint_32 ERROR_CODE;

   /* Used by the _int_enable function while in the ISR */
   uint_32 ENABLE_SR;

   /* Address of previous context, NULL if none */
   struct psp_int_context_struct _PTR_ PREV_CONTEXT;

} PSP_INT_CONTEXT_STRUCT, _PTR_ PSP_INT_CONTEXT_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP BASIC INT FRAME STRUCT
**
** This structure is what is pushed onto the memory on the current stack
** when an interrupt occurs.
**
*/

typedef struct psp_basic_int_frame_struct
{

   /* The registers pushed by _int_kernel_isr
   ** Not all registers are stored to..
   */
   PSP_BLOCKED_STACK_STRUCT STACK;

} PSP_BASIC_INT_FRAME_STRUCT, _PTR_ PSP_BASIC_INT_FRAME_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP INTERRUPT FRAME STRUCT
**
** This structure defines the stack as it appears presented to an interrupt
** service routine.  Note that this frame is on the interrupt stack.
*/

typedef struct  psp_interrupt_frame_struct
{

   /* dummy stack pointer, pointing to frame_pointer */
   pointer                          STACK_POINTER;

   /* Storage location for return address for 'C' functions */
   void                 (_CODE_PTR_ RETURN_ADDRESS)(void);

   /* Interrupt context information for the current ISR, used by MQX
   ** primitives.
   */
   PSP_INT_CONTEXT_STRUCT           INTERRUPT_CONTEXT;

   /* The next 2 fields define the fake function call stack frame.
   ** _int_kernel_isr builds the fake function call stack frame so that
   ** the exception handler can find this data structure on the stack.
   */
   pointer                          FRAME_POINTER;

   /* This has a value of 0, and is used when walking down the function call
   ** stack frames on the interrupt stack, in order to find this data structure
   */
   pointer                          FAKE_RETURN_ADDRESS;

   /* The exception number for this interrupt frame */
   uint_32                          EXCEPTION_NUMBER;

} PSP_INTERRUPT_FRAME_STRUCT, _PTR_ PSP_INTERRUPT_FRAME_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP STACK START STRUCT
**
** This structure is used during the initialization of a new task.
** It is overlaid on the bottom of the task's stack
**
*/
typedef struct psp_stack_start_struct
{
   /* The start up registers for the task */
   PSP_BLOCKED_STACK_STRUCT     INITIAL_CONTEXT;

   /* Previous stack pointer for exit return */
   pointer                      PREVIOUS_STACK_POINTER;

   /* The function to call when the task "returns" */
   void             (_CODE_PTR_ EXIT_ADDRESS)(void);

   /* The task's parameter */
   uint_32                      PARAMETER;

   /* The following two fields are used to create a "bottom" of stack
   ** that debuggers will recognize
   */
   /* End stack pointer    */
   pointer                      ZERO_STACK_POINTER;

   /* close the stack frame with a return address of 0 */
   uint_32                      ZERO_RETURN_ADDRESS;

   /*
   ** Alignment to make sure stack pointer when enter task
   ** is 8 byte aligned.
   */
   uint_32                      RESERVED[1];

} PSP_STACK_START_STRUCT, _PTR_ PSP_STACK_START_STRUCT_PTR;

/*--------------------------------------------------------------------------*/
/*
**                  PROTOTYPES OF PSP FUNCTIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Generic PSP prototypes */
extern _mqx_uint _psp_int_install(void);
extern uint_32   _psp_int_init(uint_32, uint_32);
extern void      _psp_generate_interrupt(uint_32);

/* PSP MSR register prototypes */
extern uint_32   _psp_get_sr(void);
extern uint_32   _psp_set_sr(uint_32);

/* PSP Special purpose registers access functions */
extern void      _psp_set_spr(uint_32, uint_32);
extern uint_32   _psp_get_spr(uint_32);

/* PSP Cache prototypes */
extern void      _dcache_enable(uint_32);
extern void      _dcache_disable(void);
extern void      _dcache_flush(void);
extern void      _dcache_flush_line(pointer);
extern void      _dcache_flush_mlines(pointer, uint_32);
extern void      _dcache_invalidate(void);
extern void      _dcache_invalidate_line(pointer);
extern void      _dcache_invalidate_mlines(pointer, uint_32);

extern void      _icache_enable(uint_32);
extern void      _icache_disable(void);
extern void      _icache_invalidate(void);
extern void      _icache_invalidate_line(pointer);
extern void      _icache_invalidate_mlines(pointer, uint_32);

extern PSP_SUPPORT_STRUCT_PTR _psp_get_support_ptr(void);
extern void _psp_set_support_ptr(PSP_SUPPORT_STRUCT_PTR support_ptr);

#ifdef __cplusplus
}
#endif

#endif

#endif
/* EOF */
