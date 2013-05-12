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
* $FileName: coldfire.h$
* $Version : 3.8.28.1$
* $Date    : Apr-2-2012$
*
* Comments:
*
*   This file contains the type definitions for the COLDFIRE core
*   processor.
*
*END************************************************************************/
#ifndef __coldfire_h__
#define __coldfire_h__

#ifdef __cplusplus
extern "C" {
#endif

/*==========================================================================*/
/*
**                   MQX REQUIRED DEFINITIONS
**
** Other MQX kernel and component functions require these definitions to exist.
*/

/* Indicate which endian this PSP is */
#define PSP_ENDIAN      MQX_BIG_ENDIAN

/* PSP_HAS_EMAC macro specifies if processor has Enhanced Multiply-Accumulate Unit (EMAC)*/
#ifndef PSP_HAS_EMAC
#error  PSP_HAS_EMAC macro has to be defined in mqx\source\psp\<proc_family>\<proc_type>.h header file
#endif

/* PSP_HAS_DSP macro specifies if processor has Multiply-Accumulate Unit (MAC)*/
#ifndef PSP_HAS_DSP
#define PSP_HAS_DSP macro has to be defined in mqx\source\psp\<proc_family>\<proc_type>.h header file
#endif

#ifndef PSP_HAS_FPU
   #define PSP_HAS_FPU     0
#endif

#ifndef PSP_HAS_MMU
   #define PSP_HAS_MMU     0
#endif

#ifndef PSP_ACR_CNT
   #define PSP_ACR_CNT     0
#endif

#ifndef PSP_HAS_SUPPORT_STRUCT
    #define PSP_HAS_SUPPORT_STRUCT  PSP_ACR_CNT
#endif    

#ifndef PSP_STOP_ON_IDLE
    #define PSP_STOP_ON_IDLE   1
#endif  

/*
** Memory alignment requirements.
** The alignment indicates how memory is to be aligned for all memory
** accesses.   This is to avoid mis-aligned transfers of data, thus
** optimizing memory accesses.
*/

#ifdef PSP_CACHE_LINE_SIZE
   #define PSP_MEMORY_ALIGNMENT       (PSP_CACHE_LINE_SIZE-1)
#else
   #define PSP_MEMORY_ALIGNMENT       (4-1)
#endif
#define PSP_MEMORY_ALIGNMENT_MASK  (~PSP_MEMORY_ALIGNMENT)

/*
** Stack alignment requirements.
** The alignment indicates how the stack is to be initially aligned.
** This is to avoid mis-aligned types on the stack
*/
#define PSP_STACK_ALIGNMENT       PSP_MEMORY_ALIGNMENT
#define PSP_STACK_ALIGNMENT_MASK  PSP_MEMORY_ALIGNMENT_MASK

/*
** Indicate the direction of the stack
*/
#define PSP_STACK_GROWS_TO_LOWER_MEM         (1)

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
*/
#define PSP_MEMORY_ACCESSING_CAPABILITY (8)

/*
** Cache and MMU definition values
*/
/* See CPU specific header file */

/*
** The maximum number of hardware interrupt vectors
*/
#define PSP_MAXIMUM_INTERRUPT_VECTORS (256)

/*
**   MINIMUM STACK SIZE FOR A TASK
*/

/*
** Memory overhead on users stack before switching over to the interrupt stack.
**
**     6 * (HW interrupt stack frame: SR, PC, Format, reg d0) = 72
**     1 * Bus error stack frame, reg d0 = 28
**     4 more registers before switch to interrupt stack = 16
**
*/
#define PSP_STACK_INTERRUPT_OVERHEAD (116)

/* This much extra stack is required for the logging of mqx functions */
#if MQX_KERNEL_LOGGING
#define PSP_STACK_KLOG_OVERHEAD      (128)
#else
#define PSP_STACK_KLOG_OVERHEAD      (0)
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

/* COLDFIRE addresses do not need to be normalized
** (ie as for example the Intel chips in real mode do require this)
*/
#define _PSP_NORMALIZE_MEMORY(x) (x)

/*
** Standard cache macros
*/
/* See CPU specific header file */

/*==========================================================================*/
/*
**                    PSP SPECIFIC DEFINITIONS
**
** These definitions will change from PSP to PSP
*/

/* Processor family */
#define PSP_COLDFIRE 1

/*
** Exception vector numbers
*/
#define PSP_EXCPT_ACCESS_ERROR    (2)
#define PSP_EXCPT_ADDRESS_ERROR   (3)
#define PSP_EXCPT_ILLEGAL_INSTR   (4)
#define PSP_EXCPT_PRIVILEGE_VIOL  (8)
#define PSP_EXCPT_TRACE           (9)
#define PSP_EXCPT_1010_EMUL       (10)
#define PSP_EXCPT_1111_EMUL       (11)
#define PSP_EXCPT_DEBUG           (12)
#define PSP_EXCPT_FORMAT_ERROR    (14)
#define PSP_EXCPT_UNINIT_INT_VEC  (15)
#define PSP_EXCPT_SPURIOUS_INT    (24)
#define PSP_EXCPT_LEVEL1_AUTO     (25)
#define PSP_EXCPT_LEVEL2_AUTO     (26)
#define PSP_EXCPT_LEVEL3_AUTO     (27)
#define PSP_EXCPT_LEVEL4_AUTO     (28)
#define PSP_EXCPT_LEVEL5_AUTO     (29)
#define PSP_EXCPT_LEVEL6_AUTO     (30)
#define PSP_EXCPT_LEVEL7_AUTO     (31)
#define PSP_EXCPT_TRAP0           (32)
#define PSP_EXCPT_TRAP1           (33)
#define PSP_EXCPT_TRAP2           (34)
#define PSP_EXCPT_TRAP3           (35)
#define PSP_EXCPT_TRAP4           (36)
#define PSP_EXCPT_TRAP5           (37)
#define PSP_EXCPT_TRAP6           (38)
#define PSP_EXCPT_TRAP7           (39)
#define PSP_EXCPT_TRAP8           (40)
#define PSP_EXCPT_TRAP9           (41)
#define PSP_EXCPT_TRAP10          (42)
#define PSP_EXCPT_TRAP11          (43)
#define PSP_EXCPT_TRAP12          (44)
#define PSP_EXCPT_TRAP13          (45)
#define PSP_EXCPT_TRAP14          (46)
#define PSP_EXCPT_TRAP15          (47)


/*
** Bits found in the Status Register
*/
#define PSP_SR_TRACE_ALL          (0x8000)
#define PSP_SR_TRACE_FLOW         (0x4000)
#define PSP_SR_SUPERVISORY_STATE  (0x2000)
#define PSP_SR_INTERRUPT_STATE    (0x1000)
#define PSP_SR_PRIORITY_MASK      (0x0700)
#define PSP_SR_EXTEND             (0x0010)
#define PSP_SR_NEGATIVE           (0x0008)
#define PSP_SR_ZERO               (0x0004)
#define PSP_SR_OVERFLOW           (0x0002)
#define PSP_SR_CARRY              (0x0001)

/*
** Function code values
*/
#define PSP_FC_USER_DATA_SPACE     (0x01)
#define PSP_FC_USER_PROGRAM_SPACE  (0x02)
#define PSP_FC_SUPER_DATA_SPACE    (0x05)
#define PSP_FC_SUPER_PROGRAM_SPACE (0x06)
#define PSP_FC_CPU_SPACE           (0x07)

/*
** Defined Format values for the CPU32 processor found in the format
** and vector word of the stack frame.
** NOTE: only the top nibble of the format and vector is the format.
*/
#define PSP_NORMAL_STACK_FRAME                      (0x4000)

/* Obtain the format bits from the format and vector (FAV) word */
#define PSP_FAV_GET_FORMAT(x)                       ((x) & 0xF000)

/* Obtain the vector bits from the format and vector word */
#define PSP_FAV_GET_VECTOR_OFFSET(x) ((x) & 0x3FC)
#define PSP_FAV_GET_VECTOR_NUMBER(x) (PSP_FAV_GET_VECTOR_OFFSET(x)>>2)

#ifndef __ASM__

#if PSP_HAS_FPU
/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED FP STRUCT
**
** This structure defines the registers stored by MQX when a floating
** point task is blocked.  When the FLOATING_POINT_REGISTERS_STORED bit
** is set task's FLAGS, then this structure is valid.
*/
typedef struct psp_blocked_fp_struct
{
   uint_32  FPCR;       // Floating point control register
   uint_32  FPSR;       // Floating point status register
   uint_32  FPIAR;      // Floating point instruction address register
   uint_32  TID;        // Testing for correctness
   double   FPR[8];     // The floating point computation registers
} PSP_BLOCKED_FP_STRUCT, _PTR_ PSP_BLOCKED_FP_STRUCT_PTR;

#endif

/*==========================================================================*/
/*                     PSP SPECIFIC DATA TYPES                              */

/*-----------------------------------------------------------------------*/
/*
** PSP NORMAL STACK FRAME STRUCT
**
** The stack frame pushed onto the stack for a normal stack frame
**
** The format in the FORMAT_AND_VECTOR indicates a
**   NORMAL_STACK_FRAME: 0x0---
*/
typedef struct psp_normal_stack_frame_struct
{
   uint_16              FORMAT_AND_VECTOR;
   uint_16              STATUS_REGISTER;
   void     (_CODE_PTR_ RETURN_ADDRESS)(void);
} PSP_NORMAL_STACK_FRAME_STRUCT, _PTR_ PSP_NORMAL_STACK_FRAME_STRUCT_PTR;

/*-----------------------------------------------------------------------*/
/*
** PSP FUNCTION CALL STRUCT
**
** This structure defines what a function call pushes onto the stack
*/
typedef struct psp_function_call_struct
{

   /* Pushed onto the stack by the LINK instrution when a function is entered*/
   pointer              A6_REGISTER;

   /* Pushed onto the stack by the JSR instruction */
   void     (_CODE_PTR_ RETURN_ADDRESS)(void);

} PSP_FUNCTION_CALL_STRUCT, _PTR_ PSP_FUNCTION_CALL_STRUCT_PTR;

/*==========================================================================*/
/*                         MQX DATA TYPES                                   */


#if PSP_HAS_DSP
/*--------------------------------------------------------------------------*/
/*
** TD EXTENSION STRUCT
**
** Extra context information for a task, currently kept on the stack of
** a task.
*/
typedef struct td_extension_struct
{

// TODO add EMAC define - reduce regs count for DSP only
   /* DSP registers are saved here */
   uint_32                  ACC0;
   uint_32                  ACC1;
   uint_32                  ACC2;
   uint_32                  ACC3;
   uint_32                  ACCEXT10;
   uint_32                  ACCEXT23;
   uint_32                  MACSR;
   uint_32                  MASK;

} TD_EXTEND_STRUCT, _PTR_ TD_EXTEND_STRUCT_PTR;
#endif


/*-----------------------------------------------------------------------*/
/*
** PSP STORED REGISTERS STRUCT
**
** This structure defines the registers stored on the stack by MQX
** for a task that is not running.
*/
typedef struct psp_stored_registers_struct
{
   uint_32 D0;
   uint_32 D1;
   uint_32 D2;
   uint_32 D3;
   uint_32 D4;
   uint_32 D5;
   uint_32 D6;
   uint_32 D7;
   pointer A0;
   pointer A1;
   pointer A2;
   pointer A3;
   pointer A4;
   pointer A5;
   pointer A6;
} PSP_STORED_REGISTERS_STRUCT, _PTR_ PSP_STORED_REGISTERS_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP BLOCKED STACK STRUCT
**
** This is what a stack looks like for a task that is NOT the active
** task
*/
typedef struct psp_blocked_stack_struct
{

   /* the registers of the task */
   PSP_STORED_REGISTERS_STRUCT   REGISTERS;

   /* An interrupt frame created by software so that the task can be
   ** activated using a return from interrupt
   */
   PSP_NORMAL_STACK_FRAME_STRUCT FRAME;

} PSP_BLOCKED_STACK_STRUCT, _PTR_ PSP_BLOCKED_STACK_STRUCT_PTR;


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
   uint_16 ENABLE_SR;

   /* The interrupt vector number for this context */
   uint_16 EXCEPTION_NUMBER;

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
** For the first interrupt that interrupts a running task, this structure
** defines what the top of the stack for the task looks like,
** and the STACK_PTR field of the task descriptor will point to this structure.
**
** The rest of the interrupt frame is then built upon the interrupt stack.
**
** For a nested interrupt, this basic frame is also built upon the interrupt
** stack.
**
*/

typedef struct psp_basic_int_frame_struct
{

   /* The registers pushed by _int_kernel_isr
   ** Not all are stored to
   */
   PSP_STORED_REGISTERS_STRUCT   REGISTERS;

   /* The stack frame pushed by the CPU */
   PSP_NORMAL_STACK_FRAME_STRUCT FRAME;

} PSP_BASIC_INT_FRAME_STRUCT, _PTR_ PSP_BASIC_INT_FRAME_STRUCT_PTR;


/*-----------------------------------------------------------------------*/
/*
** PSP INTERRUPT FRAME STRUCT
**
** This structure defines the stack as it appears presented to an interrupt
** service routine.  Note that this frame is on the interrupt stack.
**
** However for the very first interrupt (IN_ISR == 1)
** part of this structure (BASIC_INT_FRAME_STRUCT) is found on the
** stack of the interrupted task.
**
*/

typedef struct  psp_interrupt_frame_struct
{

   /* the isr data, passed as a parameter to the ISR */
   pointer                          ISR_DATA;

   /* Interrupt context information for the current ISR, used by MQX
   ** primitives.
   */
   PSP_INT_CONTEXT_STRUCT           INTERRUPT_CONTEXT;

   /* The stack frame pushed by the CPU at the start of _int_kernel_isr */
   PSP_BASIC_INT_FRAME_STRUCT       BASIC_FRAME;

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

   /* The function to call when the task "returns" */
   void             (_CODE_PTR_ EXIT_ADDRESS)();

   /* The task's parameter */
   uint_32                      PARAMETER;

   /* The following two fields are used to create a "bottom" of stack
   ** that debuggers will recognize
   */
   /* base of frame pointer , value 0, the starting A6 register points here */
   uint_32                      ZERO_LINK_ADDRESS;

   /* close the stack frame with a return address of 0 */
   uint_32                      ZERO_RETURN_ADDRESS;

#if PSP_HAS_DSP
   /* Extra task context information kept here, rather than in the
   ** task descriptor
   */
   TD_EXTEND_STRUCT             TD_EXTENSION;
#endif

} PSP_STACK_START_STRUCT, _PTR_ PSP_STACK_START_STRUCT_PTR;


/*--------------------------------------------------------------------------*/
/*
**                  EXTERNS FOR PSP GLOBAL VARIABLES
*/

/*--------------------------------------------------------------------------*/
/*
**                  PROTOTYPES OF PSP FUNCTIONS
*/

extern volatile uint_32 _psp_saved_mbar;
extern volatile uint_32 _psp_saved_vbr;
extern volatile uint_32 _psp_saved_cacr;

#if PSP_ACR_CNT
extern volatile uint_32 _psp_saved_acr0;
extern volatile uint_32 _psp_saved_acr1;
#endif // PSP_ACR_CNT

#if PSP_ACR_CNT == 4
extern volatile uint_32 _psp_saved_acr2;
extern volatile uint_32 _psp_saved_acr3;
#endif // PSP_ACR_CNT == 4

/* Generic PSP prototypes */
extern _mqx_uint  _psp_int_init(_mqx_uint, _mqx_uint);
extern void       _psp_int_install(void);


/* PSP Specific prototypes */
extern uint_32   _psp_get_sr(void);
extern uint_32   _psp_set_sr(uint_32);

#if PSP_ACR_CNT
extern uint_32   _psp_get_acr(uint_32);
extern uint_32   _psp_set_acr(uint_32,uint_32);
#endif

extern void      _psp_set_cacr(uint_32);
extern void      _psp_set_mbar(uint_32);

extern uint_32   _psp_load_long_swapped(pointer);
extern void      _psp_store_long_swapped(pointer, uint_32);

// cache
extern void      _dcache_flush_mlines(pointer, uint_32, uint_32);
extern void      _icache_invalidate_mlines(pointer, uint_32, uint_32);

#endif // __ASM__

#ifdef __cplusplus
}
#endif


#endif
/* EOF */
