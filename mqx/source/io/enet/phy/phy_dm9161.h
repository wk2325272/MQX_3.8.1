/**HEADER********************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2010 Embedded Access Inc.;
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
* $FileName: phy_dm9161.h$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the Davicom PHY DM9161A
*
*END************************************************************************/
#ifndef _DM9161_h_
#define _DM9161_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

/*
** Phy Register Indexes - Common Across Device Types.
*/
#define DM9161_REG_BMCR            (0x0000) /* Basic Mode Control Register */
#define DM9161_REG_BMSR            (0x0001) /* Basic Mode Status Register */
#define DM9161_REG_IDR1            (0x0002) /* PHY Identification Register #1 */
#define DM9161_REG_IDR2            (0x0003) /* PHY Identification Register #2 */
#define DM9161_REG_ANAR            (0x0004) /* Auto-Negotiation Advertisement Register */
#define DM9161_REG_ANLPAR          (0x0005) /* Auto-Negotiation Link Partner Ability Register */
#define DM9161_REG_ANER            (0x0006) /* Auto-Negotiation Expansion Register */


#define DM9161_REG_DSCR            (0x0010) /* Davicom Specified Configuration Register */
#define DM9161_REG_DSCSR           (0x0011) /* Davicom Specified Configuration and Status Register */
#define DM9161_REG_10BTSCR         (0x0012) /* 10Base-T Status/Control Register */
#define DM9161_REG_PWDOR           (0x0013) /* Power Down Control Register */
#define DM9161_REG_SCR             (0x0014) /* Specified Configuration Register */
#define DM9161_REG_DSIR            (0x0015) /* Davicom Specified Interrupt Register */
#define DM9161_REG_RECR            (0x0016) /* Davicom Specified Receive Error Counter Register */
#define DM9161_REG_DISCR           (0x0017) /* Davicom Specified Disconnect Counter Register */
#define DM9161_REG_RLSR            (0x0018) /* Davicom Hardware Reset Latch State Register */

#define DM9161_CTRL_RESET          (0x8000)
#define DM9161_CTRL_LOOPBACK       (0x4000)
#define DM9161_CTRL_SPEED          (0x2000)
#define DM9161_CTRL_AUTONEG        (0x1000)
#define DM9161_CTRL_DUPLEX         (0x0100)
#define DM9161_CTRL_RESTART_AUTO   (0x0200)
#define DM9161_CTRL_COLL           (0x0080)

/* older definitions compatible with 82xx drivers */

#define DM9161_CTRL_REG            (0x00)
#define DM9161_STATUS_REG          (0x01)
#define DM9161_AUTONEG_CTRL_REG    (0x04)
#define DM9161_AUTONEG_LINK_REG    (0x05)
#define DM9161_AUTONEG_EXP_REG     (0x06)
#define DM9161_AUX_CFG_REG         (0x10)
#define DM9161_AUX_STAT_REG        (0x11)
#define DM9161_10BASET_STAT_REG    (0x12)


/*
** Common Register Bit Definitions
*/

/* PHY_BMCR Defs */
#define DM9161_REG_BMCR_RESET               (0x8000)
#define DM9161_REG_BMCR_LOOPBACK            (0x4000)
#define DM9161_REG_BMCR_FORCE_SPEED_100     (0x2000)
#define DM9161_REG_BMCR_AUTO_NEG_ENABLE     (0x1000)
#define DM9161_REG_BMCR_FORCE_SPEED_10      (0x0000)
#define DM9161_REG_BMCR_POWER_DOWN          (0x0800)
#define DM9161_REG_BMCR_ISOLATE             (0x0400)
#define DM9161_REG_BMCR_RESTART_AUTONEG     (0x0200)
#define DM9161_REG_BMCR_FORCE_FULL_DUP      (0x0100)
#define DM9161_REG_BMCR_COLLISION_TEST      (0x0080)


/* PHY_BMSR Defs */
#define DM9161_REG_BMSR_100T4_CAPABLE       (0x8000)
#define DM9161_REG_BMSR_100x_FULL_DUP       (0x4000)
#define DM9161_REG_BMSR_100x_HALF_DUP       (0x2000)
#define DM9161_REG_BMSR_10T_FULL_DUP        (0x1000)
#define DM9161_REG_BMSR_10T_HALF_DUP        (0x0800)
#define DM9161_REG_BMSR_PREAMBLE_SUPPRESS   (0x0040)
#define DM9161_REG_BMSR_AUTO_NEG_COMPLETE   (0x0020)
#define DM9161_REG_BMSR_REMOTE_FAULT        (0x0010)
#define DM9161_REG_BMSR_AUTO_NEG_ABILITY    (0x0008)
#define DM9161_REG_BMSR_LINK_STATUS         (0x0004)
#define DM9161_REG_BMSR_JABBER_DETECT       (0x0002)
#define DM9161_REG_BMSR_EXTENDED_CAPABLE    (0x0001)

/* PHY_IDR1 Defs */
#define DM9161_REG_IDR1_DAVICOM_OUI_VAL     (0x0181)    /* OUI bits 3-18 */

/* PHY_IDR2 Defs */
#define DM9161_REG_IDR2_OUI_MASK            (0xFC00)
#define DM9161_REG_IDR2_MODEL_NUMBER_MASK   (0x03F0)
#define DM9161_REG_IDR2_REVISION_MASK       (0x000F)
#define DM9161_REG_IDR2_MODEL_SHIFT         (4)

#define DM9161_REG_IDR2_DAVICOM_OUI_VAL     (0xB800)    /* OUI bits 19-24 (15:10), Vendor Model (9:4), Revision (3:0) */
#define DM9161_REG_IDR2_MODEL_DM9161_VAL    (0x00A0)
#define DM9161_REG_IDR2_REV_DM9161_VAL      (0x0000)

/* PHY_ANAR Defs */
#define DM9161_REG_ANAR_NEXT_PAGE_IND       (0x8000)
#define DM9161_REG_ANAR_ACK                 (0x4000)
#define DM9161_REG_ANAR_REMOTE_FAULT        (0x2000)
#define DM9161_REG_ANAR_PAUSE_SUPPORT       (0x0400)
#define DM9161_REG_ANAR_100T4_SUPPORT       (0x0200)
#define DM9161_REG_ANAR_100T_FULL_DUP       (0x0100)
#define DM9161_REG_ANAR_100T_HALF_DUP       (0x0080)
#define DM9161_REG_ANAR_10T_FULL_DUP        (0x0040)
#define DM9161_REG_ANAR_10T_HALF_DUP        (0x0020)
#define DM9161_REG_ANAR_PROTO_8023          (0x0001)

/* PHY_ANLPAR Defs */
#define DM9161_REG_ANLPAR_NEXT_PAGE_IND     (0x8000)
#define DM9161_REG_ANLPAR_ACK               (0x4000)
#define DM9161_REG_ANLPAR_REMOTE_FAULT      (0x2000)
#define DM9161_REG_ANLPAR_FLOW_SUPPORT      (0x0400)
#define DM9161_REG_ANLPAR_100T4_SUPPORT     (0x0200)
#define DM9161_REG_ANLPAR_100T_FULL_DUP     (0x0100)
#define DM9161_REG_ANLPAR_100T_HALF_DUP     (0x0080)
#define DM9161_REG_ANLPAR_10T_FULL_DUP      (0x0040)
#define DM9161_REG_ANLPAR_10T_HALF_DUP      (0x0020)

#define DM9161_REG_ANLPAR_PROTO_SEL_MASK    (0x001F)

/* PHY_ANER Defs */
#define DM9161_REG_ANER_PARALLEL_DET_FAULT  (0x0010)
#define DM9161_REG_ANER_PRT_NEXT_PAGE_ABLE  (0x0008)
#define DM9161_REG_ANER_NEXT_PAGE_ABLE      (0x0004)
#define DM9161_REG_ANER_PAGE_RX             (0x0002)
#define DM9161_REG_ANER_AUTO_NEG_CAPABLE    (0x0001)


/* DM9161 Specific Registers */

/* PHY_DSCR Defs */
#define DM9161_REG_DSCR_BP_4B5B             (0x8000)
#define DM9161_REG_DSCR_BP_SCR              (0x4000)
#define DM9161_REG_DSCR_BP_ALIGN            (0x2000)
#define DM9161_REG_DSCR_BP_ADPOK            (0x1000)
#define DM9161_REG_DSCR_REPEATER            (0x0800)
#define DM9161_REG_DSCR_TX                  (0x0400)
#define DM9161_REG_DSCR_RMII_ENABLE         (0x0100)
#define DM9161_REG_DSCR_F_LINK_100          (0x0080)
#define DM9161_REG_DSCR_SPLED_CTL           (0x0040)
#define DM9161_REG_DSCR_COLLED_CTL          (0x0020)
#define DM9161_REG_DSCR_RPDCTR_EN           (0x0010)
#define DM9161_REG_DSCR_SMRST               (0x0008)
#define DM9161_REG_DSCR_MFP_SC              (0x0004)
#define DM9161_REG_DSCR_SLEEP               (0x0002)
#define DM9161_REG_DSCR_RLOUT               (0x0001)

/* PHY_DSCSR Defs */
#define DM9161_REG_DSCSR_100T_FULL_DUP      (0x8000)
#define DM9161_REG_DSCSR_100T_HALF_DUP      (0x4000)
#define DM9161_REG_DSCSR_10T_FULL_DUP       (0x2000)
#define DM9161_REG_DSCSR_10T_HALF_DUP       (0x1000)
#define DM9161_REG_DSCSR_PHYADR(x)          ((x & (0x1f) << 4)
#define DM9161_REG_DSCSR_PHYADR_MASK        (0x01F0)
#define DM9161_REG_DSCSR_ANMB_MASK          (0x000F)


/* PHY_10BTCSR Defs */
#define DM9161_REG_10BTCSR_LP_EN            (0x4000)
#define DM9161_REG_10BTCSR_HBE              (0x2000)
#define DM9161_REG_10BTCSR_SQUELCH          (0x1000)
#define DM9161_REG_10BTCSR_JABEN            (0x0800)
#define DM9161_REG_10BTCSR_10BT_SER         (0x0400)
#define DM9161_REG_10BTCSR_POLR             (0x0001)

/* PHY_CSR Defs */
#define DM9161_REG_CSR_MDIX_CNTL            (0x0080)
#define DM9161_REG_CSR_AUTONEG_DPBK         (0x0040)
#define DM9161_REG_CSR_MDIX_FIX_VAL         (0x0020)
#define DM9161_REG_CSR_MDIX_DOWN            (0x0010)
#define DM9161_REG_CSR_RMII_VER             (0x0002)
#define DM9161_REG_CSR_PD_VALUE             (0x0001)

/* PHY_DSIR Defs */
#define DM9161_REG_DSIR_INTR_PEND           (0x8000)
#define DM9161_REG_DSIR_FDX_MASK            (0x0800)
#define DM9161_REG_DSIR_SPD_MASK            (0x0400)
#define DM9161_REG_DSIR_LINK_MASK           (0x0200)
#define DM9161_REG_DSIR_INTR_MASK           (0x0100)
#define DM9161_REG_DSIR_FDX_CHANGE          (0x0010)
#define DM9161_REG_DSIR_SPD_CHANGE          (0x0008)
#define DM9161_REG_DSIR_LINK_CHANGE         (0x0004)
#define DM9161_REG_DSIR_INTR_STATUS         (0x0001)


#define DM9161_REG_CSR_                     (0x0000)


#define DM9161_REG_PHYSR_100MB              (0x4000)
#define DM9161_REG_PHYSR_10MB               (0x0000)
#define DM9161_REG_PHYSR_FULL_DUPLEX        (0x2000)
#define DM9161_REG_PHYSR_PAGE_REC           (0x1000)
#define DM9161_REG_PHYSR_SPEED_DUP_RESOLVED (0x0800)
#define DM9161_REG_PHYSR_LINK_OK            (0x0400)
#define DM9161_REG_PHYSR_MDI_CROSSOVER      (0x0040)
#define DM9161_REG_PHYSR_JABBER             (0x0001)


/* PHY_RLSR Defs */
#define DM9161_HWRST_LATCH_STATE_LEDMODE    (0x8000)
#define DM9161_HWRST_LATCH_STATE_MDINTR     (0x4000)
#define DM9161_HWRST_LATCH_STATE_CABLESTS   (0x2000)
#define DM9161_HWRST_LATCH_STATE_TXCLK      (0x1000)
#define DM9161_HWRST_LATCH_STATE_COL        (0x0800)
#define DM9161_HWRST_LATCH_STATE_RXCLK      (0x0400)
#define DM9161_HWRST_LATCH_STATE_RXER       (0x0200)
#define DM9161_HWRST_LATCH_STATE_RXDV       (0x0100)
#define DM9161_HWRST_LATCH_STATE_LINKLED    (0x0080)
#define DM9161_HWRST_LATCH_STATE_SPOLED     (0x0040)
#define DM9161_HWRST_LATCH_STATE_FDXLED     (0x0020)
#define DM9161_HWRST_LATCH_STATE_CRS        (0x0010)
#define DM9161_HWRST_LATCH_STATE_RXD3       (0x0008)
#define DM9161_HWRST_LATCH_STATE_RXD2       (0x0004)
#define DM9161_HWRST_LATCH_STATE_RXD1       (0x0002)
#define DM9161_HWRST_LATCH_STATE_RXD0       (0x0001)


/* PHY IDENTIFIER */


#define MII_TIMEOUT                     (0x10000)

extern const ENET_PHY_IF_STRUCT phy_dm9161_IF;


#ifdef __cplusplus
}
#endif

#endif

