#ifndef __e300core_h__
#define __e300core_h__

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
* $FileName: e300core.h$
* $Version : 3.8.8.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the type definitions for the PowerPC e300 Core family
*
*END************************************************************************/

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
#define PSP_HAS_CODE_CACHE             1
#define PSP_HAS_DATA_CACHE             1
#define PSP_CACHE_HAS_BUS_SNOOPING     1 



#if PSP_MQX_CPU_IS_MPC830x || PSP_MQX_CPU_IS_MPC832x

   /* e300c2 and e300c3 cores have 16K I$ and 16K D$ */
   /* MPC830x MPC831x and MPC832x are e300c2 or e300c3   */
   #define PSP_DCACHE_SIZE             (16*1024)
   #define PSP_ICACHE_SIZE             (16*1024)

#else

   /* e300c1 and e300c4 cores have 32k I$ and 32k D$ */
   /* MPC834x and MPC837x are e300c1 and e300c4 cores */
   #define PSP_DCACHE_SIZE             (32*1024)
   #define PSP_ICACHE_SIZE             (32*1024)

#endif


#define PSP_CACHE_LINE_SIZE            (32)
#define PSP_CACHE_LINE_ALIGNMENT       (0xFFFFFFE0)




/*--------------------------------------------------------------------------*/
/*
**                    CONSTANT DEFINITIONS
*/

/* Indicate processor core type */
#define PSP_E300_CORE

/* Core Exceptions */
#define PSP_EXCPT_CRITICAL                   (10)
#define EXCPT_TLB_INST_MISS                  (16)
#define EXCPT_TLB_DATA_MISS_ON_LOAD          (17)
#define EXCPT_TLB_DATA_MISS_ON_STORE         (18)
#define EXCPT_INST_ADDR_BRKPT                (19)
#define EXCPT_SYSTEM_MANAGEMENT              (20)

#undef  PSP_MAXIMUM_INTERRUPT_VECTORS
#define PSP_MAXIMUM_INTERRUPT_VECTORS   (EXCPT_SYSTEM_MANAGEMENT+1)

/*--------------------------------------------------------------------------*/
/*
** Special Purpose Registers
*/

/* Standard Special Purpose Register Mapping */
#define E300CORE_XER         (1)
#define E300CORE_LR          (8)
#define E300CORE_CTR         (9)
#define E300CORE_DSISR      (18)
#define E300CORE_DAR        (19)
#define E300CORE_DEC        (22)
#define E300CORE_SDR1       (25)
#define E300CORE_SRR0       (26)
#define E300CORE_SRR1       (27)

#define E300CORE_CSRR0      (58)
#define E300CORE_CSRR1      (59)

#define E300CORE_TBL       (268)
#define E300CORE_TBU       (269)

#define E300CORE_SPRG0     (272)
#define E300CORE_SPRG1     (273)
#define E300CORE_SPRG2     (274)
#define E300CORE_SPRG3     (275)
#define E300CORE_SPRG4     (276)    
#define E300CORE_SPRG5     (277)    
#define E300CORE_SPRG6     (278)    
#define E300CORE_SPRG7     (279)    

#define E300CORE_TBL_WRITE (284)
#define E300CORE_TBU_WRITE (285)
#define E300CORE_SVR       (286)
#define E300CORE_PVR       (287)

#define E300CORE_IBCR      (309)
#define E300CORE_DBCR      (310)
#define E300CORE_MBAR      (311)
#define E300CORE_DABR2     (317)

#define E300CORE_IBATU0    (528)
#define E300CORE_IBATL0    (529)
#define E300CORE_IBATU1    (530)
#define E300CORE_IBATL1    (531)
#define E300CORE_IBATU2    (532)
#define E300CORE_IBATL2    (533)
#define E300CORE_IBATU3    (534)
#define E300CORE_IBATL3    (535)
#define E300CORE_IBATU4    (560)
#define E300CORE_IBATL4    (561)
#define E300CORE_IBATU5    (562)
#define E300CORE_IBATL5    (563)
#define E300CORE_IBATU6    (564)
#define E300CORE_IBATL6    (565)
#define E300CORE_IBATU7    (566)
#define E300CORE_IBATL7    (567)
#define E300CORE_DBATU0    (536)
#define E300CORE_DBATL0    (537)
#define E300CORE_DBATU1    (538)
#define E300CORE_DBATL1    (539)
#define E300CORE_DBATU2    (540)
#define E300CORE_DBATL2    (541)
#define E300CORE_DBATU3    (542)
#define E300CORE_DBATL3    (543)
#define E300CORE_DBATU4    (568)    
#define E300CORE_DBATL4    (569)
#define E300CORE_DBATU5    (570)
#define E300CORE_DBATL5    (571)    
#define E300CORE_DBATU6    (572)        
#define E300CORE_DBATL6    (573)    
#define E300CORE_DBATU7    (574)    
#define E300CORE_DBATL7    (575)    
#define E300CORE_DMISS     (976)
#define E300CORE_DCMP      (977)
#define E300CORE_HASH1     (978)
#define E300CORE_HASH2     (979)
#define E300CORE_IMISS     (980)
#define E300CORE_ICMP      (981)
#define E300CORE_RPA       (982)
#define E300CORE_HID0      (1008)
#define E300CORE_HID1      (1009)
#define E300CORE_IABR      (1010)
#define E300CORE_HID2      (1011)
#define E300CORE_DABR      (1013)
#define E300CORE_IABR2     (1018)

/* page sizes */
#define E300CORE_MMU_PAGE_TABLE_SIZE  (64 * 1024)
#define E300CORE_MMU_PAGE_SIZE        (4 * 1024)


/* Number of BAT entries for the mmu table */
#define E300CORE_NUM_DBATS   (8)
#define E300CORE_NUM_IBATS   (8)
#define PSP_MSR_CE         (0x00000080)

/* Bit definitions for BATU */
#define E300CORE_BATU_BEPI_MASK           (0xFFFE0000)
#define E300CORE_BATU_128K_BLOCK_SIZE     (0x00000000)
#define E300CORE_BATU_256K_BLOCK_SIZE     (0x00000004)
#define E300CORE_BATU_512K_BLOCK_SIZE     (0x0000000C)
#define E300CORE_BATU_1MEG_BLOCK_SIZE     (0x0000001C)
#define E300CORE_BATU_2MEG_BLOCK_SIZE     (0x0000003C)
#define E300CORE_BATU_4MEG_BLOCK_SIZE     (0x0000007C)
#define E300CORE_BATU_8MEG_BLOCK_SIZE     (0x000000FC)
#define E300CORE_BATU_16MEG_BLOCK_SIZE    (0x000001FC)
#define E300CORE_BATU_32MEG_BLOCK_SIZE    (0x000003FC)
#define E300CORE_BATU_64MEG_BLOCK_SIZE    (0x000007FC)
#define E300CORE_BATU_128MEG_BLOCK_SIZE   (0x00000FFC)
#define E300CORE_BATU_256MEG_BLOCK_SIZE   (0x00001FFC)
#define E300CORE_BATU_SUPER_ACCESS        (0x00000002)
#define E300CORE_BATU_USER_ACCESS         (0x00000001)

/* Bit definitions for BATL */
#define E300CORE_BATL_BRPN_MASK           (0xFFFE0000)
#define E300CORE_BATL_CACHE_WRITE_THRU    (0x00000040)
#define E300CORE_BATL_CACHE_INHIBIT       (0x00000020)
#define E300CORE_BATL_CACHE_COHERENT      (0x00000010)
#define E300CORE_BATL_CACHE_GUARDED       (0x00000008)
#define E300CORE_BATL_READ_WRITE          (0x00000002)
#define E300CORE_BATL_READ_ONLY           (0x00000001)

/* User MMU flags definitions */
#define E300CORE_MMU_CACHE_INHIBIT        (0x00000001) 
#define E300CORE_MMU_CACHE_WRITE_THRU     (0x00000002)
#define E300CORE_MMU_CACHE_GUARDED        (0x00000004)
#define E300CORE_MMU_CACHE_COHERENT       (0x00000008)
#define E300CORE_MMU_SET_IBAT             (0x00000010)
#define E300CORE_MMU_SET_DBAT             (0x00000020)
#define E300CORE_MMU_READ_ONLY            (0x00000040)
#define E300CORE_MMU_DBAT0_PAGED_REGION   (0x80000000) /* MMU Feature */

/* SDR1 register bits. */
#define E300CORE_SDR1_HTABORG_MASK   (0xFFFF0000)
#define E300CORE_SDR1_HTABMASK_MASK  (0x000001FF)

/* SR registers bits. */
#define E300CORE_SR_T_BIT            (0x80000000)
#define E300CORE_SR_Ks_BIT           (0x40000000)
#define E300CORE_SR_Kp_BIT           (0x20000000)
#define E300CORE_SR_N_BIT            (0x10000000)
#define E300CORE_SR_VSID_MASK        (0x00FFFFFF)

/* Upper PTE */
#define E300CORE_PTE_VALID_MASK      (0x80000000)
#define E300CORE_PTE_VSID_MASK       (0x7FFFFF80)
#define E300CORE_PTE_H_MASK          (0x00000040)
#define E300CORE_PTE_API_MASK        (0x0000003F)
#define E300CORE_PTE_VSID_API_MASK   (E300CORE_PTE_VSID_MASK|E300CORE_PTE_API_MASK)

/* Lower PTE */
#define E300CORE_PTE_RPN_MASK         (0xFFFFF000)
#define E300CORE_PTE_REFERENCED       (0x00000100)
#define E300CORE_PTE_CHANGED          (0x00000080)
#define E300CORE_PTE_CACHE_WRITE_THRU (0x00000040)
#define E300CORE_PTE_CACHE_INHIBIT    (0x00000020)
#define E300CORE_PTE_CACHE_COHERENT   (0x00000010)
#define E300CORE_PTE_CACHE_GUARDED    (0x00000008)
#define E300CORE_PTE_READ_WRITE       (0x00000002)
#define E300CORE_PTE_READ_ONLY        (0x00000001)

/* Macros for finding a page index */
#define E300CORE_GET_VADDR_SEGID(x)      ((x) >> 28)
#define E300CORE_GET_VADDR_PAGE_INDEX(x)(((x) >> 12) & 0xffff)
#define E300CORE_GET_VADDR_API(x)       (((x) >> 22) & 0x3f)
#define E300CORE_GET_VADDR_VSID(x) \
   (_PSP_GET_SEG_REG(x) & E300CORE_SR_VSID_MASK)

#define E300CORE_GET_PTE_API(x)      ((x) & E300CORE_PTE_API_MASK)
#define E300CORE_GET_PTE_VSID(x)     (((x) & E300CORE_PTE_VSID_MASK) >> 7)
#define E300CORE_GET_PTE_RPN(x)      ((x) & E300CORE_PTE_RPN_MASK)

/* Error codes from _mmu_add_region */
#define E300CORE_OUT_OF_IBATS             (0x00100001)
#define E300CORE_OUT_OF_DBATS             (0x00100002)
#define E300CORE_MMU_INVALID_FLAGS        (0x00100003)
#define E300CORE_MMU_INVALID_SIZE         (0x00100004)

/* Number of TLB entries in mmu table */
#define E300CORE_NUM_TLBS   (64)

/* Set and Get MBAR (311) values in E300CORE */
#define _PSP_SET_MBAR(x)      _PSP_SPR_SET(311, x)
#define _PSP_GET_MBAR(x)      _PSP_SPR_GET(x, 311)

#define _PSP_GET_IMMR(x)      (x = (uint_32)__IMMR_BASE)


#endif
