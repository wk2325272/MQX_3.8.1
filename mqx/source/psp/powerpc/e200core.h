#ifndef __e200core_h__
#define __e200core_h__

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
* $FileName: e200core.h$
* $Version : 3.8.8.1$
* $Date    : May-29-2012$
*
* Comments:
*
*   This file contains the type definitions for the PowerPC e200 Core 
*
*END***********************************************************************/

/*==========================================================================*/
/*
**                   MQX REQUIRED DEFINITIONS
**
** Other MQX kernel and component functions require these definitions to exist.
*/

/*
** Cache and MMU definition values
*/
#define PSP_HAS_MMU                    1

#if defined(PSP_E200_CORE_Z4)
#define PSP_HAS_CODE_CACHE             1
#define PSP_HAS_DATA_CACHE             0
#define PSP_ICACHE_SIZE	               (4*1024)

#elif defined(PSP_E200_CORE_Z7)
#define PSP_HAS_CODE_CACHE             1
#define PSP_HAS_DATA_CACHE             1
#define PSP_DCACHE_SIZE	               (16*1024)
#define PSP_ICACHE_SIZE	               (16*1024)

#else
#define PSP_HAS_CODE_CACHE             1
#define PSP_HAS_DATA_CACHE             1
#define PSP_DCACHE_SIZE	               (32*1024)
#define PSP_ICACHE_SIZE	               (32*1024)
#endif

#define PSP_CACHE_LINE_SIZE            (32)
#define PSP_CACHE_LINE_ALIGNMENT       (0xFFFFFFE0)
#define PSP_CACHE_HAS_BUS_SNOOPING     0 

 /** MGCT: <category name="SPE Registers Settings">  */

/*
** Enable to save/restore all SPE (64 bit GPRs, ACC and SPEFSCR) registers on context switch.
** MGCT: <option type="bool"/>
*/
#ifndef PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS
    #define PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS 0
#endif

/*
** MGCT: <option type="bool"/>
*/
#ifndef PSPCFG_TEST_SAVE_AND_RESTORE_SPE_REGISTERS
    #define PSPCFG_TEST_SAVE_AND_RESTORE_SPE_REGISTERS 0
#endif

/** MGCT: </category> */

/*--------------------------------------------------------------------------*/
/*
**                    CONSTANT DEFINITIONS
*/

/* Indicate processor core type */
#define PSP_E200_CORE

/*
** E200 Core specific Exceptions: 
*/
/* Note: These are not defined by Book-E architecture */
#define PSP_EXCPT_SPE_APU_UNAVAILABLE         (32) /* IVOR32 */
#define PSP_EXCPT_SPE_FP_DATA                 (33) /* IVOR33 */
#define PSP_EXCPT_SPE_FP_ROUND                (34) /* IVOR34 */


/*--------------------------------------------------------------------------*/
/*
** Special Purpose Registers
*/

/* Standard Special Purpose Register Mapping */
/* Book-E defined SPRs */
#define E200CORE_XER         (1) // Integer exception register
#define E200CORE_LR          (8) // Link register
#define E200CORE_CTR         (9) // Count register
#define E200CORE_DEC        (22) // Decrementer register
#define E200CORE_SRR0       (26) // Save/Restore register 0
#define E200CORE_SRR1       (27) // Save/Restore register 1

#define E200CORE_PID        (48) // Process ID register 0
#define E200CORE_DECAR      (54) // Decrementer auto-reload register
#define E200CORE_CSRR0      (58) // Critical SRR register 0
#define E200CORE_CSRR1      (59) // Critical SRR register 1

#define E200CORE_DEAR       (61) // Data exception address register
#define E200CORE_ESR        (62) // Exception syndrome register
#define E200CORE_IVPR       (63) // Interrupt vector prefix register

#define E200CORE_USPRG0    (256) // User SPR general 0 
#define E200CORE_TBL       (268) // Time base lower register 
#define E200CORE_TBU       (269) // Time base upper register 

#define E200CORE_SPRG0     (272) // General SPR 0 
#define E200CORE_SPRG1     (273) // General SPR 1
#define E200CORE_SPRG2     (274) // General SPR 2
#define E200CORE_SPRG3     (275) // General SPR 3
#define E200CORE_SPRG4R    (260) // SPR general register 4 (Read)	
#define E200CORE_SPRG4W    (276) //	General SPR 4 (Read/Write)
#define E200CORE_SPRG5R    (261) //	SPR general register 5 (Read)
#define E200CORE_SPRG5W    (277) //	General SPR 5 (Read/Write)
#define E200CORE_SPRG6R    (262) //	SPR general register 6 (Read)
#define E200CORE_SPRG6W    (278) //	General SPR 6 (Read/Write)
#define E200CORE_SPRG7R    (263) //	SPR general register 7 (Read)
#define E200CORE_SPRG7W    (279) //	General SPR 7 (Read/Write)

#define E200CORE_TBL_WRITE (284) // Time base lower register (Write)
#define E200CORE_TBU_WRITE (285) // Time base upper register (Write)
#define E200CORE_PIR       (286) // Process ID register
#define E200CORE_PVR       (287) // Process version register 

#define E200CORE_DBSR      (304) // Debug status register
#define E200CORE_DBCR0     (308) // Debug control register 0
#define E200CORE_DBCR1     (309) // Debug control register 1
#define E200CORE_DBCR2     (310) // Debug control register 2
#define E200CORE_DBCR3     (561) // Debug control register 3
#define E200CORE_IAC1      (312) // Instruction address compare register 1
#define E200CORE_IAC2      (313) // Instruction address compare register 2
#define E200CORE_IAC3      (314) // Instruction address compare register 3
#define E200CORE_IAC4      (315) // Instruction address compare register 4
#define E200CORE_DAC1      (316) // Data address compare register 1
#define E200CORE_DAC2      (317) // Data address compare register 2

#define E200CORE_TSR       (336) // Timer status register
#define E200CORE_TCR       (340) // Timer control register

#define E200CORE_IVOR0     (400) // Critical Input Interrupt
#define E200CORE_IVOR1     (401) // Machine Check Interrupt
#define E200CORE_IVOR2     (402) // Data Storage Interrupt 
#define E200CORE_IVOR3     (403) // Instruction Storage Interrupt
#define E200CORE_IVOR4     (404) // External Input Interrupt
#define E200CORE_IVOR5     (405) // Alignment Interrupt 
#define E200CORE_IVOR6     (406) // Program Interrupt
#define E200CORE_IVOR7     (407) // Floating-Point Unavailable Interrupt
#define E200CORE_IVOR8     (408) // System Call Interrupt
#define E200CORE_IVOR9     (409) // Auxiliary Processor Unavailable Interrupt
#define E200CORE_IVOR10    (410) // Decrementer Interrupt 
#define E200CORE_IVOR11    (411) // Fixed-Interval Timer Interrupt 
#define E200CORE_IVOR12    (412) // Watchdog Timer Interrupt
#define E200CORE_IVOR13    (413) // Data TLB Error Interrupt 
#define E200CORE_IVOR14    (414) // Instruction TLB Error Interrupt
#define E200CORE_IVOR15    (415) // Debug Interrupt

/* E200 core implementation specific SPRs */

#define E200CORE_SPEFSCR   (512) // FP control and status register
#define E200CORE_L1CFG0    (515) // L1 cache configuration register 0
#define E200CORE_L1CFG1    (516) // L1 cache configuration register 0
#define E200CORE_IVOR32    (528) // SPE/EFPU Unavailable Interrupt
#define E200CORE_IVOR33    (529) // Embedded Floating-Point Data Interrupt
#define E200CORE_IVOR34    (530) // Embedded Floating-Point Round Interrupt 
#define E200CORE_IVOR35    (531) // Performance Monitor Interrupt 
#define E200CORE_CTXCR     (560) // Context control register
#define E200CORE_DBCNT     (562) // Debug count register
#define E200CORE_ALTCTXCR  (568) // Alternate context control register
#define E200CORE_MCSR      (572) // Machine check syndrome register
#define E200CORE_DSRR0     (574) //	Debug interrupt SRR 0
#define E200CORE_DSRR1     (575) //	Debug interrupt SRR 1
#define E200CORE_MAS0      (624) // MMU assist register 0
#define E200CORE_MAS1      (625) // MMU assist register 1
#define E200CORE_MAS2      (626) // MMU assist register 2
#define E200CORE_MAS3      (627) // MMU assist register 3
#define E200CORE_MAS4      (628) // MMU assist register 4
#define E200CORE_MAS6      (630) // MMU assist register 6
#define E200CORE_TLB0CFG   (688) // TLB configuration register 0
#define E200CORE_TLB1CFG   (689) // TLB configuration register 1

#define E200CORE_L1FINV1   (959)  // L1 cache flush/invalidate control register 1 
#define E200CORE_HID0      (1008) // Hardware implementation dependent register 0
#define E200CORE_HID1      (1009) // Hardware implementation dependent register 1
#define E200CORE_L1CSR0    (1010) // L1 cache control/status register 0
#define E200CORE_L1CSR1    (1011) // L1 cache control/status register 1
#define E200CORE_MMUCSR0   (1012) // MMU control and status register 0
#define E200CORE_BUCSR     (1013) // Branch control and status register
#define E200CORE_MMUCFG    (1015) // MMU configuration register
#define E200CORE_L1FINV0   (1016) // L1 cache flush/invalidate control register 0 
#define E200CORE_SVR       (1023) // System version register


/* MSR - Machine State Register - Bit definitions */
#define E200CORE_MSR_UCLE     0x04000000         /* User mode cache lock enable     */
#define E200CORE_MSR_SPE      0x02000000         /* SPE Enable                      */
#define E200CORE_MSR_WE       0x00040000         /* wait state enable               */
#define E200CORE_MSR_CE       0x00020000         /* critical interrupt enable       */
#define E200CORE_MSR_EE       0x00008000         /* external interrupt              */
#define E200CORE_MSR_PR       0x00004000         /* problem state                   */
#define E200CORE_MSR_FP       0x00002000         /* FP Available                    */
#define E200CORE_MSR_ME       0x00001000         /* machine check                   */
#define E200CORE_MSR_FE0      0x00000800         /* Floating-point exception mode 0 */
#define E200CORE_MSR_DE       0x00000200         /* debug enable                    */
#define E200CORE_MSR_FE1      0x00000100         /* Floating-point exception mode 1 */
#define E200CORE_MSR_IS       0x00000020         /* instruction relocate            */
#define E200CORE_MSR_DS       0x00000010         /* data relocate                   */
#define E200CORE_MSR_PMM      0x00000004         /* Performance Monitor mark bit    */
#define E200CORE_MSR_RI       0x00000002         /* Recoverable Interrupt           */

/* TSR - Timer status register - Bit definitions */
#define E200CORE_TSR_ENW      0x80000000         /* next Watchdog event goverened by TSR(1) */
#define E200CORE_TSR_WIS      0x40000000         /* Watchdog interrupt status               */
#define E200CORE_TSR_WRS_MASK 0x30000000         /* Watchdog reset mask                     */
#define E200CORE_TSR_WRS_CHIP 0x20000000         /* Chip reset forced by watchdog           */
#define E200CORE_TSR_DIS      0x08000000         /* DEC interrupt pending                   */
#define E200CORE_TSR_FIS      0x04000000         /* FIS interrupt pending                   */

/* TCR - Timer control register - Bit definitions */
#define E200CORE_TCR_WP_MASK     0xC0000000      /* Watchdog period mask          */
#define E200CORE_TCR_WRC_MASK    0x30000000      /* Watchdog reset mask           */
#define E200CORE_TCR_WRC_CHIP    0x20000000      /* Chip reset forced by watchdog */
#define E200CORE_TCR_WIE         0x08000000      /* Watchdog interrupt enable     */
#define E200CORE_TCR_DIE         0x04000000      /* DEC interrupt enable          */
#define E200CORE_TCR_FP_MASK     0x03000000      /* Fixed interval period mask    */
#define E200CORE_TCR_FIE         0x00800000      /* FIT interrupt enable          */
#define E200CORE_TCR_ARE         0x00400000      /* Auto-reload enable            */
#define E200CORE_TCR_WPEXT_MASK  0x001E0000      /* WP Extension bits             */
#define E200CORE_TCR_FPEXT_MASK  0x0001E000      /* FP Extension bits             */

/* DBSR - Debug status register - Bit definitions */
#define E200CORE_DBSR_IDE        0x80000000      /* Imprecise debug event                */
#define E200CORE_DBSR_UDE        0x40000000      /* Unconditional debug event            */
#define E200CORE_DBSR_MRR_MASK   0x30000000      /* Most recent reset                    */
#define E200CORE_DBSR_MRR_HRST   0x20000000      /* Hard reset occured                   */
#define E200CORE_DBSR_ICMP       0x08000000      /* Instruction completion dbg event     */
#define E200CORE_DBSR_BRT        0x04000000      /* Branch taken debug event             */
#define E200CORE_DBSR_IRPT       0x02000000      /* Interrupt debug event                */
#define E200CORE_DBSR_TRAP       0x01000000      /* Trap debug event                     */
#define E200CORE_DBSR_IAC1       0x00800000      /* IAC 1 debug event                    */
#define E200CORE_DBSR_IAC2       0x00400000      /* IAC 2 debug event                    */
#define E200CORE_DBSR_IAC3       0x00200000      /* IAC 3 debug event                    */
#define E200CORE_DBSR_IAC4       0x00100000      /* IAC 4 debug event                    */
#define E200CORE_DBSR_DAC1R      0x00080000      /* DAC 1 read debug event               */
#define E200CORE_DBSR_DAC1W      0x00040000      /* DAC 1 write debug event              */
#define E200CORE_DBSR_DAC2R      0x00020000      /* DAC 2 read debug event               */
#define E200CORE_DBSR_DAC2W      0x00010000      /* DAC 2 write debug event              */
#define E200CORE_DBSR_RET        0x00008000      /* Return debug event                   */
#define E200CORE_DBSR_DEVT1      0x00000400      /* External debug event 1               */
#define E200CORE_DBSR_DEVT2      0x00000200      /* External debug event 2               */
#define E200CORE_DBSR_DCNT1      0x00000100      /* Debug counter 1 debug event          */
#define E200CORE_DBSR_DCNT2      0x00000080      /* Debug counter 2 debug event          */
#define E200CORE_DBSR_CIRPT      0x00000040      /* Critical interrupt taken debug event */
#define E200CORE_DBSR_CRET       0x00000020      /* Critical return debug event          */
#define E200CORE_DBSR_CNT1TRG    0x00000001      /* Counter 1 triggered                  */


/* HID0 - Hardware Implementation-Dependent Register 0 */
#define E200CORE_HID0_EMCP       0x80000000      /* Enable machine check signal /mcp    */
#define E200CORE_HID0_BPRED_MASK 0x03000000      /* Branch prediction control           */
#define E200CORE_HID0_DOZE       0x00800000      /* Doze power management mode asserted */
#define E200CORE_HID0_NAP        0x00400000      /* Nap mode                            */
#define E200CORE_HID0_SLEEP      0x00200000      /* Sleep mode                          */
#define E200CORE_HID0_ICR        0x00020000      /* Interrupt inputs clear reservation  */
#define E200CORE_HID0_NHR        0x00010000      /* Not hardware reset                  */
#define E200CORE_HID0_TBEN       0x00004000      /* Time base and Decrementer enable    */
#define E200CORE_HID0_SEL_TBCLK  0x00002000      /* Select time base clock 1 = TBCLK    */
#define E200CORE_HID0_DCLREE     0x00001000      /* Debug interrupt clears MSR[EE]      */
#define E200CORE_HID0_DCLRCE     0x00000800      /* Debug interrupt clears MSR[CE]      */
#define E200CORE_HID0_CICLRDE    0x00000400      /* Critical interrupt clears           */
#define E200CORE_HID0_MCCLRDE    0x00000200      /* Machine check interrupt clears      */
#define E200CORE_HID0_DAPUEN     0x00000100      /* Debug APU enable                    */

/* HID1 - Hardware Implementation-Dependent Register 1 */
#define E200CORE_HID1_ATS		 0x00000080          /* Atomic status */

/* L1CSR0 - L1 Cache Control/Status register 0 */
#define E200CORE_L1CSR0_WID     ((x & 0xF) << 28) /* Way instruction disable */  
#define E200CORE_L1CSR0_WDD(x)  ((x & 0xF) << 24) /* Way data disable */
#define E200CORE_L1CSR0_AWID    0x00800000        /* Additional ways instruction disable */
#define E200CORE_L1CSR0_AWDD    0x00400000        /* Additional ways data disable */
#define E200CORE_L1CSR0_CWM     0x00100000        /* Cache write mode */

#define E200CORE_L1CSR0_DPB     0x00080000        /* Disable push buffer */
#define E200CORE_L1CSR0_DSB     0x00040000        /* Disable store buffer */
#define E200CORE_L1CSR0_DSTRM   0x00020000        /* Disable streaming */
#define E200CORE_L1CSR0_CPE     0x00010000        /* Cache parity enable */
#define E200CORE_L1CSR0_CUL     0x00000400        /* Cache unable to lock */
#define E200CORE_L1CSR0_CLO     0x00000200        /* Cache lock overflow */
#define E200CORE_L1CSR0_CLFC    0x00000100        /* Cache lock bits flash clear */
#define E200CORE_L1CSR0_CABT    0x00000004        /* Cache operation aborted */
#define E200CORE_L1CSR0_CINV    0x00000002        /* Cache invalidate */
#define E200CORE_L1CSR0_CE      0x00000001        /* Cache enable */

/* L1CSR1 - L1 Cache Control/Status register 1 */
#define E200CORE_L1CSR1_ICECE   0x00010000        /* Cache error checking enable */
#define E200CORE_L1CSR1_ICEI    0x00008000        /* Cache error injection enable */
#define E200CORE_L1CSR1_ICEDT   0x00003000        /* Cache error type */
#define E200CORE_L1CSR1_ICUL    0x00000400        /* Cache unable to lock */
#define E200CORE_L1CSR1_ICLO    0x00000200        /* Cache lock overflow */
#define E200CORE_L1CSR1_ICLFC   0x00000100        /* Cache lock bits flash clear */
#define E200CORE_L1CSR1_ICLOA   0x00000080        /* Cache lock overflow allocate */
#define E200CORE_L1CSR1_ICEA    0x00000060        /* Cache error action */
#define E200CORE_L1CSR1_ICABT   0x00000004        /* Cache operation aborted */
#define E200CORE_L1CSR1_ICINV   0x00000002        /* Cache invalidate */
#define E200CORE_L1CSR1_ICE     0x00000001        /* Cache enable */


/* Bit definitions for MMU supported block size */
#define E200CORE_TLB_4K_BLOCK_SIZE     (0x00000100)
#define E200CORE_TLB_16K_BLOCK_SIZE    (0x00000200)
#define E200CORE_TLB_64K_BLOCK_SIZE    (0x00000300)
#define E200CORE_TLB_256K_BLOCK_SIZE   (0x00000400)
#define E200CORE_TLB_1MEG_BLOCK_SIZE   (0x00000500)
#define E200CORE_TLB_4MEG_BLOCK_SIZE   (0x00000600)
#define E200CORE_TLB_16MEG_BLOCK_SIZE  (0x00000700)
#define E200CORE_TLB_64MEG_BLOCK_SIZE  (0x00000800)
#define E200CORE_TLB_256MEG_BLOCK_SIZE (0x00000900)

#define E200CORE_TLB_BLOCK_SIZE(size)  (size<<7)

#define E200CORE_TLB_VALID             (0x80000000)
#define E200CORE_TLB_IPROT             (0x40000000)
#define E200CORE_MAS0_TLBSEL_TLB1	   (0x10000000)
#define E200CORE_TLB_MEM_MASK		   (0xFFFF0000)
#define E200CORE_TLB_PAGE_MASK         (0xFFFFFC00)
#define E200CORE_TLB_TID(x)            ((x&0xff)<<16)
#define E200CORE_TLB_NUM_MASK		   (0x001F0000)
#define E200CORE_TLB_NUM_00			   (0x00000000)
#define E200CORE_TLB_NUM_01			   (0x00010000)
#define E200CORE_TLB_NUM_02			   (0x00020000)
#define E200CORE_TLB_NUM_03			   (0x00030000)
#define E200CORE_TLB_NUM_04			   (0x00040000)
#define E200CORE_TLB_NUM_05			   (0x00050000)
#define E200CORE_TLB_NUM_06			   (0x00060000)
#define E200CORE_TLB_NUM_07			   (0x00070000)
#define E200CORE_TLB_NUM_08			   (0x00080000)
#define E200CORE_TLB_NUM_09			   (0x00090000)
#define E200CORE_TLB_NUM_10			   (0x000A0000)
#define E200CORE_TLB_NUM_11			   (0x000B0000)
#define E200CORE_TLB_NUM_12			   (0x000C0000)
#define E200CORE_TLB_NUM_13			   (0x000D0000)
#define E200CORE_TLB_NUM_14			   (0x000E0000)
#define E200CORE_TLB_NUM_15			   (0x000F0000)
#define E200CORE_TLB_NUM(n)            (n<<16)

/* User MMU flags definitions for MMU ACCESS RIGHTS */
#define E200CORE_TLB_UX        		   (0x00000020)
#define E200CORE_TLB_SX         	   (0x00000010)
#define E200CORE_TLB_UW         	   (0x00000008)
#define E200CORE_TLB_SW         	   (0x00000004)
#define E200CORE_TLB_UR         	   (0x00000002)
#define E200CORE_TLB_SR         	   (0x00000001)
#define E200CORE_TLB_SXWR         	   (0x00000015)
#define E200CORE_TLB_UXWR         	   (0x0000002A)
#define E200CORE_TLB_SUXWR         	   (0x0000003F)

/* User MMU flags definitions for MMU ACCESS */
#define E200CORE_TLB_FLAG_MASK		   (0x0000003F)
#define E200CORE_MMU_VLE               (0x00000020)
#define E200CORE_MMU_CACHE_WRITE_THRU  (0x00000010)
#define E200CORE_MMU_CACHE_INHIBIT     (0x00000008)
#define E200CORE_MMU_CACHE_COHERENT    (0x00000004)
#define E200CORE_MMU_CACHE_GUARDED     (0x00000002)
#define E200CORE_MMU_LITTLE_ENDIAN	   (0x00000001)


/*-----------------------------------------------------------------------
**                      FUNCTION PROTOTYPES
*/

#ifndef __ASM__



#ifdef __cplusplus
extern "C" {
#endif

extern uchar   __KERNEL_DATA_VERIFY_ENABLE[];

extern asm  uint_32 _PSP_GET_VECTOR_TABLE();
extern uint_32 _psp_core_num(void);
void  _psp_set_pid( register uint_32 pid);
extern int_32 _e200_enable_timer(boolean external);

extern void _mmu_reserve_tlb(uint_32 tlb_no);
extern void _mmu_release_tlb(uint_32 tlb_no);
extern void  _mmu_set_pid( register uint_32 pid);
extern _mqx_uint _mmu_add_virtual_region(uchar_ptr phys_mem_ptr, uchar_ptr virt_mem_ptr, _mem_size input_size, _mqx_uint flags,_mqx_uint tid);


extern void    __interrupt_vector_table(void);
extern void    __ivor0(void);
extern void    __machine_check(void);
extern void    __ivor2(void);
extern void    __ivor3(void);
extern void    __ivor4(void);
extern void    __ivor5(void);
extern void    __ivor6(void);
extern void    __ivor7(void);
extern void    __ivor8(void);
extern void    __ivor9(void);
extern void    __ivor10(void);
extern void    __ivor11(void);
extern void    __ivor12(void);
extern void    __ivor13(void);
extern void    __ivor14(void);
extern void    __ivor15(void);
extern void    __ivor32(void);
extern void    __ivor33(void);
extern void    __ivor34(void);
   
#ifdef __cplusplus
}
#endif
#endif /* __ASM__ */

#endif
/* EOF */
