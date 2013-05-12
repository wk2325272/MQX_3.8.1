/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor
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
* $FileName: reg_enet.h$
* $Version : 3.7.3.0$
* $Date    : Feb-7-2011$
*
* Comments:
*   This file contains the type definitions for the ColdFire 32-bit 
*   MAC_NET module.
*
*END************************************************************************/

#ifndef __reg_enet_h__
#define __reg_enet_h__

#define __reg_enet_h__version "$Version:3.7.3.0$"
#define __reg_enet_h__date    "$Date:Feb-7-2011$"

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------------------
   -- Peripheral register structure
   ---------------------------------------------------------------------------- */

typedef struct _ENET_MemMap {
  uint_8 RESERVED_0[4];
  uint_32 EIR;  /* 0x4 */
  uint_32 EIMR;  /* 0x8 */
  uint_8 RESERVED_1[4];
  uint_32 RDAR;  /* 0x10 */
  uint_32 TDAR;  /* 0x14 */
  uint_8 RESERVED_2[12];
  uint_32 ECR;  /* 0x24 */
  uint_8 RESERVED_3[24];
  uint_32 MMFR;  /* 0x40 */
  uint_32 MSCR;  /* 0x44 */
  uint_8 RESERVED_4[28];
  uint_32 MIBC;  /* 0x64 */
  uint_8 RESERVED_5[28];
  uint_32 RCR;  /* 0x84 */
  uint_8 RESERVED_6[60];
  uint_32 TCR;  /* 0xC4 */
  uint_8 RESERVED_7[28];
  uint_32 PALR;  /* 0xE4 */
  uint_32 PAUR;  /* 0xE8 */
  uint_32 OPD;  /* 0xEC */
  uint_8 RESERVED_8[40];
  uint_32 IAUR;  /* 0x118 */
  uint_32 IALR;  /* 0x11C */
  uint_32 GAUR;  /* 0x120 */
  uint_32 GALR;  /* 0x124 */
  uint_8 RESERVED_9[28];
  uint_32 TFWR;  /* 0x144 */
  uint_8 RESERVED_10[56];
  uint_32 RDSR;  /* 0x180 */
  uint_32 TDSR;  /* 0x184 */
  uint_32 MRBR;  /* 0x188 */
  uint_8 RESERVED_11[4];
  uint_32 RSFL;  /* 0x190 */
  uint_32 RSEM;  /* 0x194 */
  uint_32 RAEM;  /* 0x198 */
  uint_32 RAFL;  /* 0x19C */
  uint_32 TSEM;  /* 0x1A0 */
  uint_32 TAEM;  /* 0x1A4 */
  uint_32 TAFL;  /* 0x1A8 */
  uint_32 TIPG;  /* 0x1AC */
  uint_32 FTRL;  /* 0x1B0 */
  uint_8 RESERVED_12[12];
  uint_32 TACC;  /* 0x1C0 */
  uint_32 RACC;  /* 0x1C4 */
  uint_8 RESERVED_13[568];
  uint_32 ATCR;  /* 0x400 */
  uint_32 ATVR;  /* 0x404 */
  uint_32 ATOFF;  /* 0x408 */
  uint_32 ATPER;  /* 0x40C */
  uint_32 ATCOR;  /* 0x410 */
  uint_32 ATINC;  /* 0x414 */
  uint_32 ATSTMP;  /* 0x418 */
  uint_8 RESERVED_14[488];
  uint_32 TGSR;  /* 0x604 */
  struct {  /* 0x608 */
    uint_32 TCSR;  /* 0x608 + index*0x8 + 0 */
    uint_32 TCCR;  /* 0x608 + index*0x8 + 0x4 */
  } CHANNEL[4];
} ENET_MEMMAP, _PTR_ ENET_MEMMAP_PTR;
typedef volatile struct _ENET_MemMap _PTR_ VENET_MEMMAP_PTR;

/* ----------------------------------------------------------------------------
   -- Register accessors
   ---------------------------------------------------------------------------- */

#define ENET_EIR_REG(base) ((base)->EIR)
#define ENET_EIMR_REG(base) ((base)->EIMR)
#define ENET_RDAR_REG(base) ((base)->RDAR)
#define ENET_TDAR_REG(base) ((base)->TDAR)
#define ENET_ECR_REG(base) ((base)->ECR)
#define ENET_MMFR_REG(base) ((base)->MMFR)
#define ENET_MSCR_REG(base) ((base)->MSCR)
#define ENET_MIBC_REG(base) ((base)->MIBC)
#define ENET_RCR_REG(base) ((base)->RCR)
#define ENET_TCR_REG(base) ((base)->TCR)
#define ENET_PALR_REG(base) ((base)->PALR)
#define ENET_PAUR_REG(base) ((base)->PAUR)
#define ENET_OPD_REG(base) ((base)->OPD)
#define ENET_IAUR_REG(base) ((base)->IAUR)
#define ENET_IALR_REG(base) ((base)->IALR)
#define ENET_GAUR_REG(base) ((base)->GAUR)
#define ENET_GALR_REG(base) ((base)->GALR)
#define ENET_TFWR_REG(base) ((base)->TFWR)
#define ENET_RDSR_REG(base) ((base)->RDSR)
#define ENET_TDSR_REG(base) ((base)->TDSR)
#define ENET_MRBR_REG(base) ((base)->MRBR)
#define ENET_RSFL_REG(base) ((base)->RSFL)
#define ENET_RSEM_REG(base) ((base)->RSEM)
#define ENET_RAEM_REG(base) ((base)->RAEM)
#define ENET_RAFL_REG(base) ((base)->RAFL)
#define ENET_TSEM_REG(base) ((base)->TSEM)
#define ENET_TAEM_REG(base) ((base)->TAEM)
#define ENET_TAFL_REG(base) ((base)->TAFL)
#define ENET_TIPG_REG(base) ((base)->TIPG)
#define ENET_FTRL_REG(base) ((base)->FTRL)
#define ENET_TACC_REG(base) ((base)->TACC)
#define ENET_RACC_REG(base) ((base)->RACC)
#define ENET_ATCR_REG(base) ((base)->ATCR)
#define ENET_ATVR_REG(base) ((base)->ATVR)
#define ENET_ATOFF_REG(base) ((base)->ATOFF)
#define ENET_ATPER_REG(base) ((base)->ATPER)
#define ENET_ATCOR_REG(base) ((base)->ATCOR)
#define ENET_ATINC_REG(base) ((base)->ATINC)
#define ENET_ATSTMP_REG(base) ((base)->ATSTMP)
#define ENET_TGSR_REG(base) ((base)->TGSR)
#define ENET_TCSR_REG(base,index) ((base)->CHANNEL[index].TCSR)
#define ENET_TCCR_REG(base,index) ((base)->CHANNEL[index].TCCR)

/* ----------------------------------------------------------------------------
   -- Register bit group masks and offsets
   ---------------------------------------------------------------------------- */

/* EIR */
#define ENET_EIR_TS_TIMER_MASK 0x8000u
#define ENET_EIR_TS_TIMER_SHIFT 15
#define ENET_EIR_TS_AVAIL_MASK 0x10000u
#define ENET_EIR_TS_AVAIL_SHIFT 16
#define ENET_EIR_WAKEUP_MASK 0x20000u
#define ENET_EIR_WAKEUP_SHIFT 17
#define ENET_EIR_PLR_MASK 0x40000u
#define ENET_EIR_PLR_SHIFT 18
#define ENET_EIR_UN_MASK 0x80000u
#define ENET_EIR_UN_SHIFT 19
#define ENET_EIR_RL_MASK 0x100000u
#define ENET_EIR_RL_SHIFT 20
#define ENET_EIR_LC_MASK 0x200000u
#define ENET_EIR_LC_SHIFT 21
#define ENET_EIR_EBERR_MASK 0x400000u
#define ENET_EIR_EBERR_SHIFT 22
#define ENET_EIR_MII_MASK 0x800000u
#define ENET_EIR_MII_SHIFT 23
#define ENET_EIR_RXB_MASK 0x1000000u
#define ENET_EIR_RXB_SHIFT 24
#define ENET_EIR_RXF_MASK 0x2000000u
#define ENET_EIR_RXF_SHIFT 25
#define ENET_EIR_TXB_MASK 0x4000000u
#define ENET_EIR_TXB_SHIFT 26
#define ENET_EIR_TXF_MASK 0x8000000u
#define ENET_EIR_TXF_SHIFT 27
#define ENET_EIR_GRA_MASK 0x10000000u
#define ENET_EIR_GRA_SHIFT 28
#define ENET_EIR_BABT_MASK 0x20000000u
#define ENET_EIR_BABT_SHIFT 29
#define ENET_EIR_BABR_MASK 0x40000000u
#define ENET_EIR_BABR_SHIFT 30
#define ENET_EIR_ALL_PENDING (0x7FFF8000)
/* EIMR */
#define ENET_EIMR_TS_TIMER_MASK 0x8000u
#define ENET_EIMR_TS_TIMER_SHIFT 15
#define ENET_EIMR_TS_AVAIL_MASK 0x10000u
#define ENET_EIMR_TS_AVAIL_SHIFT 16
#define ENET_EIMR_WAKEUP_MASK 0x20000u
#define ENET_EIMR_WAKEUP_SHIFT 17
#define ENET_EIMR_PLR_MASK 0x40000u
#define ENET_EIMR_PLR_SHIFT 18
#define ENET_EIMR_UN_MASK 0x80000u
#define ENET_EIMR_UN_SHIFT 19
#define ENET_EIMR_RL_MASK 0x100000u
#define ENET_EIMR_RL_SHIFT 20
#define ENET_EIMR_LC_MASK 0x200000u
#define ENET_EIMR_LC_SHIFT 21
#define ENET_EIMR_EBERR_MASK 0x400000u
#define ENET_EIMR_EBERR_SHIFT 22
#define ENET_EIMR_MII_MASK 0x800000u
#define ENET_EIMR_MII_SHIFT 23
#define ENET_EIMR_RXB_MASK 0x1000000u
#define ENET_EIMR_RXB_SHIFT 24
#define ENET_EIMR_RXF_MASK 0x2000000u
#define ENET_EIMR_RXF_SHIFT 25
#define ENET_EIMR_TXB_MASK 0x4000000u
#define ENET_EIMR_TXB_SHIFT 26
#define ENET_EIMR_TXF_MASK 0x8000000u
#define ENET_EIMR_TXF_SHIFT 27
#define ENET_EIMR_GRA_MASK 0x10000000u
#define ENET_EIMR_GRA_SHIFT 28
#define ENET_EIMR_BABT_MASK 0x20000000u
#define ENET_EIMR_BABT_SHIFT 29
#define ENET_EIMR_BABR_MASK 0x40000000u
#define ENET_EIMR_BABR_SHIFT 30
/* RDAR */
#define ENET_RDAR_RDAR_MASK 0x1000000u
#define ENET_RDAR_RDAR_SHIFT 24
/* TDAR */
#define ENET_TDAR_TDAR_MASK 0x1000000u
#define ENET_TDAR_TDAR_SHIFT 24
/* ECR */
#define ENET_ECR_RESET_MASK 0x1u
#define ENET_ECR_RESET_SHIFT 0
#define ENET_ECR_ETHEREN_MASK 0x2u
#define ENET_ECR_ETHEREN_SHIFT 1
#define ENET_ECR_MAGICEN_MASK 0x4u
#define ENET_ECR_MAGICEN_SHIFT 2
#define ENET_ECR_SLEEP_MASK 0x8u
#define ENET_ECR_SLEEP_SHIFT 3
#define ENET_ECR_EN1588_MASK 0x10u
#define ENET_ECR_EN1588_SHIFT 4
#define ENET_ECR_DBGEN_MASK 0x40u
#define ENET_ECR_DBGEN_SHIFT 6
#define ENET_ECR_STOPEN_MASK 0x80u
#define ENET_ECR_STOPEN_SHIFT 7
/* MMFR */
#define ENET_MMFR_DATA_MASK 0xFFFFu
#define ENET_MMFR_DATA_SHIFT 0
#define ENET_MMFR_TA_MASK 0x30000u
#define ENET_MMFR_TA_SHIFT 16
#define ENET_MMFR_RA_MASK 0x7C0000u
#define ENET_MMFR_RA_SHIFT 18
#define ENET_MMFR_PA_MASK 0xF800000u
#define ENET_MMFR_PA_SHIFT 23
#define ENET_MMFR_OP_MASK 0x30000000u
#define ENET_MMFR_OP_SHIFT 28
#define ENET_MMFR_ST_MASK 0xC0000000u
#define ENET_MMFR_ST_SHIFT 30
/* MSCR */
#define ENET_MSCR_MII_SPEED_MASK 0x7Eu
#define ENET_MSCR_MII_SPEED_SHIFT 1
#define ENET_MSCR_DIS_PRE_MASK 0x80u
#define ENET_MSCR_DIS_PRE_SHIFT 7
#define ENET_MSCR_HOLDTIME_MASK 0x700u
#define ENET_MSCR_HOLDTIME_SHIFT 8
/* MIBC */
#define ENET_MIBC_MIB_CLEAR_MASK 0x20000000u
#define ENET_MIBC_MIB_CLEAR_SHIFT 29
#define ENET_MIBC_MIB_IDLE_MASK 0x40000000u
#define ENET_MIBC_MIB_IDLE_SHIFT 30
#define ENET_MIBC_MIB_DIS_MASK 0x80000000u
#define ENET_MIBC_MIB_DIS_SHIFT 31
/* RCR */
#define ENET_RCR_LOOP_MASK 0x1u
#define ENET_RCR_LOOP_SHIFT 0
#define ENET_RCR_DRT_MASK 0x2u
#define ENET_RCR_DRT_SHIFT 1
#define ENET_RCR_MII_MODE_MASK 0x4u
#define ENET_RCR_MII_MODE_SHIFT 2
#define ENET_RCR_PROM_MASK 0x8u
#define ENET_RCR_PROM_SHIFT 3
#define ENET_RCR_BC_REJ_MASK 0x10u
#define ENET_RCR_BC_REJ_SHIFT 4
#define ENET_RCR_FCE_MASK 0x20u
#define ENET_RCR_FCE_SHIFT 5
#define ENET_RCR_RMII_MODE_MASK 0x100u
#define ENET_RCR_RMII_MODE_SHIFT 8
#define ENET_RCR_RMII_10T_MASK 0x200u
#define ENET_RCR_RMII_10T_SHIFT 9
#define ENET_RCR_PADEN_MASK 0x1000u
#define ENET_RCR_PADEN_SHIFT 12
#define ENET_RCR_PAUFWD_MASK 0x2000u
#define ENET_RCR_PAUFWD_SHIFT 13
#define ENET_RCR_CRCFWD_MASK 0x4000u
#define ENET_RCR_CRCFWD_SHIFT 14
#define ENET_RCR_CFEN_MASK 0x8000u
#define ENET_RCR_CFEN_SHIFT 15
#define ENET_RCR_MAX_FL_MASK 0x3FFF0000u
#define ENET_RCR_MAX_FL_SHIFT 16
#define ENET_RCR_NLC_MASK 0x40000000u
#define ENET_RCR_NLC_SHIFT 30
#define ENET_RCR_GRS_MASK 0x80000000u
#define ENET_RCR_GRS_SHIFT 31
/* TCR */
#define ENET_TCR_GTS_MASK 0x1u
#define ENET_TCR_GTS_SHIFT 0
#define ENET_TCR_FDEN_MASK 0x4u
#define ENET_TCR_FDEN_SHIFT 2
#define ENET_TCR_TFC_PAUSE_MASK 0x8u
#define ENET_TCR_TFC_PAUSE_SHIFT 3
#define ENET_TCR_RFC_PAUSE_MASK 0x10u
#define ENET_TCR_RFC_PAUSE_SHIFT 4
#define ENET_TCR_ADDSEL_MASK 0xE0u
#define ENET_TCR_ADDSEL_SHIFT 5
#define ENET_TCR_ADDINS_MASK 0x100u
#define ENET_TCR_ADDINS_SHIFT 8
#define ENET_TCR_CRCFWD_MASK 0x200u
#define ENET_TCR_CRCFWD_SHIFT 9
/* PALR */
#define ENET_PALR_PADDR1_MASK 0xFFFFFFFFu
#define ENET_PALR_PADDR1_SHIFT 0
/* PAUR */
#define ENET_PAUR_TYPE_MASK 0xFFFFu
#define ENET_PAUR_TYPE_SHIFT 0
#define ENET_PAUR_PADDR2_MASK 0xFFFF0000u
#define ENET_PAUR_PADDR2_SHIFT 16
/* OPD */
#define ENET_OPD_PAUSE_DUR_MASK 0xFFFFu
#define ENET_OPD_PAUSE_DUR_SHIFT 0
#define ENET_OPD_OPCODE_MASK 0xFFFF0000u
#define ENET_OPD_OPCODE_SHIFT 16
/* IAUR */
#define ENET_IAUR_IADDR1_MASK 0xFFFFFFFFu
#define ENET_IAUR_IADDR1_SHIFT 0
/* IALR */
#define ENET_IALR_IADDR2_MASK 0xFFFFFFFFu
#define ENET_IALR_IADDR2_SHIFT 0
/* GAUR */
#define ENET_GAUR_GADDR1_MASK 0xFFFFFFFFu
#define ENET_GAUR_GADDR1_SHIFT 0
/* GALR */
#define ENET_GALR_GADDR2_MASK 0xFFFFFFFFu
#define ENET_GALR_GADDR2_SHIFT 0
/* TFWR */
#define ENET_TFWR_TFWR_MASK 0x3Fu
#define ENET_TFWR_TFWR_SHIFT 0
#define ENET_TFWR_STRFWD_MASK 0x100u
#define ENET_TFWR_STRFWD_SHIFT 8
/* RDSR */
#define ENET_RDSR_R_DES_START_MASK 0xFFFFFFF8u
#define ENET_RDSR_R_DES_START_SHIFT 3
/* TDSR */
#define ENET_TDSR_X_DES_START_MASK 0xFFFFFFF8u
#define ENET_TDSR_X_DES_START_SHIFT 3
/* MRBR */
#define ENET_MRBR_R_BUF_SIZE_MASK 0x3FF0u
#define ENET_MRBR_R_BUF_SIZE_SHIFT 4
/* RSFL */
#define ENET_RSFL_RX_SECTION_FULL_MASK 0xFFu
#define ENET_RSFL_RX_SECTION_FULL_SHIFT 0
/* RSEM */
#define ENET_RSEM_RX_SECTION_EMPTY_MASK 0xFFu
#define ENET_RSEM_RX_SECTION_EMPTY_SHIFT 0
/* RAEM */
#define ENET_RAEM_RX_ALMOST_EMPTY_MASK 0xFFu
#define ENET_RAEM_RX_ALMOST_EMPTY_SHIFT 0
/* RAFL */
#define ENET_RAFL_RX_ALMOST_FULL_MASK 0xFFu
#define ENET_RAFL_RX_ALMOST_FULL_SHIFT 0
/* TSEM */
#define ENET_TSEM_TX_SECTION_EMPTY_MASK 0xFFu
#define ENET_TSEM_TX_SECTION_EMPTY_SHIFT 0
/* TAEM */
#define ENET_TAEM_TX_ALMOST_EMPTY_MASK 0xFFu
#define ENET_TAEM_TX_ALMOST_EMPTY_SHIFT 0
/* TAFL */
#define ENET_TAFL_TX_ALMOST_FULL_MASK 0xFFu
#define ENET_TAFL_TX_ALMOST_FULL_SHIFT 0
/* TIPG */
#define ENET_TIPG_IPG_MASK 0x1Fu
#define ENET_TIPG_IPG_SHIFT 0
/* FTRL */
#define ENET_FTRL_TRUNC_FL_MASK 0x3FFFu
#define ENET_FTRL_TRUNC_FL_SHIFT 0
/* TACC */
#define ENET_TACC_SHIFT16_MASK 0x1u
#define ENET_TACC_SHIFT16_SHIFT 0
#define ENET_TACC_IPCHK_MASK 0x8u
#define ENET_TACC_IPCHK_SHIFT 3
#define ENET_TACC_PROCHK_MASK 0x10u
#define ENET_TACC_PROCHK_SHIFT 4
/* RACC */
#define ENET_RACC_PADREM_MASK 0x1u
#define ENET_RACC_PADREM_SHIFT 0
#define ENET_RACC_IPDIS_MASK 0x2u
#define ENET_RACC_IPDIS_SHIFT 1
#define ENET_RACC_PRODIS_MASK 0x4u
#define ENET_RACC_PRODIS_SHIFT 2
#define ENET_RACC_LINEDIS_MASK 0x40u
#define ENET_RACC_LINEDIS_SHIFT 6
#define ENET_RACC_SHIFT16_MASK 0x80u
#define ENET_RACC_SHIFT16_SHIFT 7
/* ATCR */
#define ENET_ATCR_EN_MASK 0x1u
#define ENET_ATCR_EN_SHIFT 0
#define ENET_ATCR_OFFEN_MASK 0x4u
#define ENET_ATCR_OFFEN_SHIFT 2
#define ENET_ATCR_OFFRST_MASK 0x8u
#define ENET_ATCR_OFFRST_SHIFT 3
#define ENET_ATCR_PEREN_MASK 0x10u
#define ENET_ATCR_PEREN_SHIFT 4
#define ENET_ATCR_PINPER_MASK 0x80u
#define ENET_ATCR_PINPER_SHIFT 7
#define ENET_ATCR_RESTART_MASK 0x200u
#define ENET_ATCR_RESTART_SHIFT 9
#define ENET_ATCR_CAPTURE_MASK 0x800u
#define ENET_ATCR_CAPTURE_SHIFT 11
#define ENET_ATCR_SLAVE_MASK 0x2000u
#define ENET_ATCR_SLAVE_SHIFT 13
/* ATVR */
#define ENET_ATVR_ATIME_MASK 0xFFFFFFFFu
#define ENET_ATVR_ATIME_SHIFT 0
/* ATOFF */
#define ENET_ATOFF_OFFSET_MASK 0xFFFFFFFFu
#define ENET_ATOFF_OFFSET_SHIFT 0
/* ATPER */
#define ENET_ATPER_PERIOD_MASK 0xFFFFFFFFu
#define ENET_ATPER_PERIOD_SHIFT 0
/* ATCOR */
#define ENET_ATCOR_COR_MASK 0x7FFFFFFFu
#define ENET_ATCOR_COR_SHIFT 0
/* ATINC */
#define ENET_ATINC_INC_MASK 0x7Fu
#define ENET_ATINC_INC_SHIFT 0
#define ENET_ATINC_INC_CORR_MASK 0x7F00u
#define ENET_ATINC_INC_CORR_SHIFT 8
/* ATSTMP */
#define ENET_ATSTMP_TIMESTAMP_MASK 0xFFFFFFFFu
#define ENET_ATSTMP_TIMESTAMP_SHIFT 0
/* TGSR */
#define ENET_TGSR_TF0_MASK 0x1u
#define ENET_TGSR_TF0_SHIFT 0
#define ENET_TGSR_TF1_MASK 0x2u
#define ENET_TGSR_TF1_SHIFT 1
#define ENET_TGSR_TF2_MASK 0x4u
#define ENET_TGSR_TF2_SHIFT 2
#define ENET_TGSR_TF3_MASK 0x8u
#define ENET_TGSR_TF3_SHIFT 3
/* TCSR */
#define ENET_TCSR_TDRE_MASK 0x1u
#define ENET_TCSR_TDRE_SHIFT 0
#define ENET_TCSR_TMODE_MASK 0x3Cu
#define ENET_TCSR_TMODE_SHIFT 2
#define ENET_TCSR_TIE_MASK 0x40u
#define ENET_TCSR_TIE_SHIFT 6
#define ENET_TCSR_TF_MASK 0x80u
#define ENET_TCSR_TF_SHIFT 7
/* TCCR */
#define ENET_TCCR_TCC_MASK 0xFFFFFFFFu
#define ENET_TCCR_TCC_SHIFT 0

/* ----------------------------------------------------------------------------
   -- Peripheral instance base addresses
   ---------------------------------------------------------------------------- */

//#define ENET_BASE_PTR ((ENET_MemMapPtr)0x40004000u)

/* ----------------------------------------------------------------------------
   -- Register instances definition
   ---------------------------------------------------------------------------- */


/*
** ENET
*/

#define ENET_EIR ENET_EIR_REG(ENET_BASE_PTR)
#define ENET_EIMR ENET_EIMR_REG(ENET_BASE_PTR)
#define ENET_RDAR ENET_RDAR_REG(ENET_BASE_PTR)
#define ENET_TDAR ENET_TDAR_REG(ENET_BASE_PTR)
#define ENET_ECR ENET_ECR_REG(ENET_BASE_PTR)
#define ENET_MMFR ENET_MMFR_REG(ENET_BASE_PTR)
#define ENET_MSCR ENET_MSCR_REG(ENET_BASE_PTR)
#define ENET_MIBC ENET_MIBC_REG(ENET_BASE_PTR)
#define ENET_RCR ENET_RCR_REG(ENET_BASE_PTR)
#define ENET_TCR ENET_TCR_REG(ENET_BASE_PTR)
#define ENET_PALR ENET_PALR_REG(ENET_BASE_PTR)
#define ENET_PAUR ENET_PAUR_REG(ENET_BASE_PTR)
#define ENET_OPD ENET_OPD_REG(ENET_BASE_PTR)
#define ENET_IAUR ENET_IAUR_REG(ENET_BASE_PTR)
#define ENET_IALR ENET_IALR_REG(ENET_BASE_PTR)
#define ENET_GAUR ENET_GAUR_REG(ENET_BASE_PTR)
#define ENET_GALR ENET_GALR_REG(ENET_BASE_PTR)
#define ENET_TFWR ENET_TFWR_REG(ENET_BASE_PTR)
#define ENET_RDSR ENET_RDSR_REG(ENET_BASE_PTR)
#define ENET_TDSR ENET_TDSR_REG(ENET_BASE_PTR)
#define ENET_MRBR ENET_MRBR_REG(ENET_BASE_PTR)
#define ENET_RSFL ENET_RSFL_REG(ENET_BASE_PTR)
#define ENET_RSEM ENET_RSEM_REG(ENET_BASE_PTR)
#define ENET_RAEM ENET_RAEM_REG(ENET_BASE_PTR)
#define ENET_RAFL ENET_RAFL_REG(ENET_BASE_PTR)
#define ENET_TSEM ENET_TSEM_REG(ENET_BASE_PTR)
#define ENET_TAEM ENET_TAEM_REG(ENET_BASE_PTR)
#define ENET_TAFL ENET_TAFL_REG(ENET_BASE_PTR)
#define ENET_TIPG ENET_TIPG_REG(ENET_BASE_PTR)
#define ENET_FTRL ENET_FTRL_REG(ENET_BASE_PTR)
#define ENET_TACC ENET_TACC_REG(ENET_BASE_PTR)
#define ENET_RACC ENET_RACC_REG(ENET_BASE_PTR)
#define ENET_ATCR ENET_ATCR_REG(ENET_BASE_PTR)
#define ENET_ATVR ENET_ATVR_REG(ENET_BASE_PTR)
#define ENET_ATOFF ENET_ATOFF_REG(ENET_BASE_PTR)
#define ENET_ATPER ENET_ATPER_REG(ENET_BASE_PTR)
#define ENET_ATCOR ENET_ATCOR_REG(ENET_BASE_PTR)
#define ENET_ATINC ENET_ATINC_REG(ENET_BASE_PTR)
#define ENET_ATSTMP ENET_ATSTMP_REG(ENET_BASE_PTR)
#define ENET_TGSR ENET_TGSR_REG(ENET_BASE_PTR)
#define ENET_TCSR0 ENET_TCSR_REG(ENET_BASE_PTR,0)
#define ENET_TCCR0 ENET_TCCR_REG(ENET_BASE_PTR,0)
#define ENET_TCSR1 ENET_TCSR_REG(ENET_BASE_PTR,1)
#define ENET_TCCR1 ENET_TCCR_REG(ENET_BASE_PTR,1)
#define ENET_TCSR2 ENET_TCSR_REG(ENET_BASE_PTR,2)
#define ENET_TCCR2 ENET_TCCR_REG(ENET_BASE_PTR,2)
#define ENET_TCSR3 ENET_TCSR_REG(ENET_BASE_PTR,3)
#define ENET_TCCR3 ENET_TCCR_REG(ENET_BASE_PTR,3)


#define ENET_INT_TX_INTF	  0       // Transmit frame interrupt
#define ENET_INT_TX_INTB	  1       // Transmit buffer interrupt
#define ENET_INT_TX_UN	      2       // Transmit FIFO underrun
#define ENET_INT_RL		      3       // Collision retry limit
#define ENET_INT_RX_INTF	  4       // Receive frame interrupt
#define ENET_INT_RX_INTB	  5       // Receive buffer interrupt
#define ENET_INT_MII		  6       // MII interrupt
#define ENET_INT_LC		      7       // Late collision
#define ENET_INT_GRA		  8       // Graceful stop complete
#define ENET_INT_EBERR	      9       // Ethernet bus error
#define ENET_INT_BABT	     10       // Babbling transmit error
#define ENET_INT_BABR	     11       // Babbling receive error
#define ENET_INT_TS_AVAIL    12       // Babbling receive error
#define ENET_INT_WAKEUP	     13       // Babbling receive error
#define ENET_INT_PLR	     14       // Babbling receive error
#define ENET_NUM_INTS	     15      


/* Ethernet Buffer descriptor bits */
#define ENET_BD_ETHER_RX_EMPTY                (1<<15)
#define ENET_BD_ETHER_RX_SOFTWARE_OWNERSHIP_1 (1<<14)
#define ENET_BD_ETHER_RX_WRAP                 (1<<13)
#define ENET_BD_ETHER_RX_SOFTWARE_OWNERSHIP_2 (1<<12)
#define ENET_BD_ETHER_RX_LAST_FRAME           (1<<11)
#define ENET_BD_ETHER_RX_MISS                 (1<<8)
#define ENET_BD_ETHER_RX_BROADCAST            (1<<7)
#define ENET_BD_ETHER_RX_MULTICAST            (1<<6)
#define ENET_BD_ETHER_RX_LENGTH_VIOLATION     (1<<5)
#define ENET_BD_ETHER_RX_NON_OCTET            (1<<4)
#define ENET_BD_ETHER_RX_CRC_ERROR            (1<<2)
#define ENET_BD_ETHER_RX_OVERRUN              (1<<1)
#define ENET_BD_ETHER_RX_TRUNCATED            (1<<0)

#define ENET_BD_EXT0_ETHER_RX_MAC_ERR         (1<<15)
#define ENET_BD_EXT0_ETHER_RX_PHY_ERR         (1<<10)
#define ENET_BD_EXT0_ETHER_RX_COLLISION       (1<<9)
#define ENET_BD_EXT0_ETHER_RX_UNICAST         (1<<8)
#define ENET_BD_EXT0_ETHER_RX_GENERATE_INTR   (1<<7)

#define ENET_BD_EXT1_ETHER_RX_IPHEADER_CHSUM_ERR  (1<<5)
#define ENET_BD_EXT1_ETHER_RX_PROTOCOL_CHSUM_ERR  (1<<4)
#define ENET_BD_EXT1_ETHER_RX_VLAN                (1<<2)
#define ENET_BD_EXT1_ETHER_RX_IPV6FRAME           (1<<1)
#define ENET_BD_EXT1_ETHER_RX_IPV4FRAGMENT        (1<<0)

#define ENET_BD_EXT2_ETHER_RX_LAST_BD_UPDATE_DONE (1<<15)

#define ENET_BD_ETHER_TX_READY                (1<<15)
#define ENET_BD_ETHER_TX_SOFTWARE_OWNERSHIP_1 (1<<14)
#define ENET_BD_ETHER_TX_WRAP                 (1<<13)
#define ENET_BD_ETHER_TX_SOFTWARE_OWNERSHIP_2 (1<<12)
#define ENET_BD_ETHER_TX_LAST                 (1<<11)
#define ENET_BD_ETHER_TX_SEND_CRC             (1<<10)

#define ENET_BD_EXT0_ETHER_TX_GENERATE_INTR          (1<<14)
#define ENET_BD_EXT0_ETHER_TX_TIMESTAMP              (1<<13)
#define ENET_BD_EXT0_ETHER_TX_INSERT_PROT_SPEC_CHSUM (1<<12)
#define ENET_BD_EXT0_ETHER_TX_INSERT_IPHEADER_CHSUM  (1<<11)

#define ENET_BD_EXT1_ETHER_TX_ERR                  (1<<15)
#define ENET_BD_EXT1_ETHER_TX_UNDERFLOW_ERR        (1<<13)
#define ENET_BD_EXT1_ETHER_TX_EXCESS_COLLISION_ERR (1<<12)
#define ENET_BD_EXT1_ETHER_TX_FRAME_WIDTH_ERR      (1<<11)
#define ENET_BD_EXT1_ETHER_TX_LATE_COLLISION_ERR   (1<<10)
#define ENET_BD_EXT1_ETHER_TX_LATE_OVERFLOW_ERR    (1<<9)
#define ENET_BD_EXT1_ETHER_TX_LATE_TIMESTAMP_ERR   (1<<8)

#define ENET_BD_EXT2_ETHER_TX_LAST_BD_UPDATE_DONE  (1<<15)

/*
** ENET_BD_STRUCT
** This structure defines what the Ethernet buffer descriptor looks like
*/
typedef struct enet_bd_struct
{
   uint_16   CONTROL;
   uint_16   LENGTH;
   uchar_ptr BUFFER;
   uint_16   CONTROL_EXT0;
   uint_16   CONTROL_EXT1;
   uint_8    HEADER_LENGTH;
   uint_8    PROTOCOL_TYPE;
   uint_16   PAYLOAD_CHECKSUM;
   uint_16   CONTROL_EXT2;
   uint_16   reserved0;
   uchar_ptr TIMESTAMP;
   uint_16   reserved1;
   uint_16   reserved2;
   uint_16   reserved3;
   uint_16   reserved4;
} ENET_BD_STRUCT, _PTR_ ENET_BD_STRUCT_PTR;
typedef volatile struct enet_bd_struct _PTR_ VENET_BD_STRUCT_PTR;

#ifdef __cplusplus
}
#endif


#endif
