#ifndef __mdio_h__
#define __mdio_h__
/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: mdio.h$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file is generic to all Ethernet MACs connected via an MDIO
*   compliant phy. It contains register addresses and bit definitions
*   for those registers.
*
*END************************************************************************/


/* Common registers */
#define MDIO_BMCR                              (0)  // Basic Mode Control
#define MDIO_BMSR                              (1)  // Basic Mode Status
#define MDIO_PHYID1                            (2)
#define MDIO_PHYID2                            (3)
#define MDIO_ANAR                              (4)  // Auto Negotiate Advertising 
#define MDIO_ANLPAR                            (5)  // Auto Neg Link Partner Ability
#define MDIO_ANER                              (6)  // Auto Neg Expansion
#define MDIO_ANNPTR                            (7)  // Auto Neg Next Page TX
#define MDIO_ANNPRR                            (8)  // Auto Neg Next Page RX
#define MDIO_GBCR                              (9)  // 1000Base-T Control
#define MDIO_GBSR                              (10) // 1000Base-T Status
#define MDIO_GBESR                             (15) // 1000Base-T Extended Status
#define MDIO_PHYCR                             (16) // PHY Specific Control
#define MDIO_PHYSR                             (17) // PHY Specific Status
#define MDIO_INER                              (18) // Interrupt Enable
#define MDIO_INSR                              (19) // Interrupt Status
#define MDIO_EPHYCR                            (20) // Extended PHY Control
#define MDIO_RXERC                             (21) // Receive Error Counter
#define MDIO_LEDCR                             (24) // LED Control


/* Reg 0: Basic Mode Control bit defines */
#define MDIO_CTRL_RESET         (0x8000)
#define MDIO_CTRL_LOOPBACK      (0x4000)
#define MDIO_CTRL_SPEED_0       (0x2000)
#define MDIO_CTRL_AUTONEG       (0x1000)
#define MDIO_CTRL_PWR_DOWN      (0x0800)
#define MDIO_CTRL_ISOLATE       (0x0400)
#define MDIO_CTRL_RESTART_AUTO  (0x0200) // Restart Auto Negotiation
#define MDIO_CTRL_FULL_DUPLEX   (0x0100)
#define MDIO_CTRL_COLL          (0x0080) // Collision Test
#define MDIO_CTRL_SPEED_1       (0x0040)
#define MDIO_CTRL_RESERVED      (0x003F)

/* Reg 1: Basic Mode Status bit defines */
#define MDIO_STAT1_100BASE_T4           (0x8000)
#define MDIO_STAT1_100BASE_X_FULL_DUP   (0x4000)
#define MDIO_STAT1_100BASE_X_HALF_DUP   (0x2000)
#define MDIO_STAT1_10_FULL_DUP          (0x1000)
#define MDIO_STAT1_10_HALF_DUP          (0x0800)
#define MDIO_STAT1_100BASE_T2_FULL_DUP  (0x0400)
#define MDIO_STAT1_100BASE_T2_HALF_DUP  (0x0200)
#define MDIO_STAT1_1000BASE_T_STATUS    (0x0100) // Device supports Reg 15
#define MDIO_STAT1_SUP_MF_PREAMBLE      (0x0040)
#define MDIO_STAT1_AUTONEG_COMPLETE     (0x0020)
#define MDIO_STAT1_REMOTE_FAULT         (0x0010)
#define MDIO_STAT1_AUTONEG_ABILITY      (0x0008)
#define MDIO_STAT1_LINK_UP              (0x0004)
#define MDIO_STAT1_JABBER_DET           (0x0002)
#define MDIO_STAT1_EXT_ABILITY          (0x0001)

/* Reg 2 & 3 are fixed PHY ID values

/* Reg 4: Autonegotiation advertisement bit defines */
#define MDIO_AUTO_ADV_NEXT_PAGE         (0x8000)
#define MDIO_AUTO_ADV_REMOTE_FAULT      (0x2000)
#define MDIO_AUTO_ADV_ASYM_PAUSE        (0x0800)
#define MDIO_AUTO_ADV_PAUSE             (0x0400)
#define MDIO_AUTO_ADV_100BASE_T4        (0x0200)
#define MDIO_AUTO_ADV_100BASE_TX_FULL   (0x0100)
#define MDIO_AUTO_ADV_100BASE_TX        (0x0080)
#define MDIO_AUTO_ADV_10BASE_T_FULL     (0x0040)
#define MDIO_AUTO_ADV_10BASE_T          (0x0020)
#define MDIO_AUTO_SELECTOR              (0x0001) // Indicates 802.3 support - duh


#define MDIO_AUTO_ADV_SELECTOR_802_3            (0x01)
#define MDIO_AUTO_ADV_SELECTOR_802_9_ISLAN_16T  (0x02)
#define MDIO_AUTO_ADV_SELECTOR_802_5            (0x03)

/* Reg 5: Auto Neg. Link partner abilities bit defines */
#define MDIO_AUTO_LNK_NEXT_PAGE         (0x8000)
#define MDIO_AUTO_LNK_ACK               (0x4000)
#define MDIO_AUTO_LNK_REMOTE_FAULT      (0x2000)
#define MDIO_AUTO_LNK_ASYM_PAUSE        (0x0800)
#define MDIO_AUTO_LNK_PAUSE             (0x0400)
#define MDIO_AUTO_LNK_100BASE_T4        (0x0200)
#define MDIO_AUTO_LNK_100BASE_TX_FULL   (0x0100)
#define MDIO_AUTO_LNK_100BASE_TX        (0x0080)
#define MDIO_AUTO_LNK_10BASE_T_FULL     (0x0040)
#define MDIO_AUTO_LNK_10BASE_T          (0x0020)

/* Reg 6: Auto Neg. Expansion bit defines */
#define MDIO_AUTO_PAR_DET_FAULT         (0x0010)
#define MDIO_AUTO_LNK_NEXT_PAGEABLE     (0x0008)
#define MDIO_AUTO_PAGE_RECEIVED         (0x0002)
#define MDIO_AUTO_LNK_AUTO_NEG          (0x0001)

/* Reg 7: Auto Neg. Next Page Transmit */
/* Reg 8: Auto Neg. Next Page Transmit */

/* Reg 9: 1000Base-T Control */
#define MDIO_GB_ENB_MASTER_SLAVE         (0x1000)
#define MDIO_GB_ADV_MASTER_SLAVE         (0x0800)
#define MDIO_GB_ADV_PORT_TYPE            (0x0400)
#define MDIO_GB_ADV_1000T_FULL           (0x0200)
#define MDIO_GB_ADV_1000T_HALF           (0x0100)

/* Reg 10: 1000Base-T Status */
#define MDIO_GB_MASTER_SLAVE_FAULT       (0x8000)
#define MDIO_GB_MASTER_SLAVE_RES         (0x4000)
#define MDIO_GB_LOC_REC_STAT             (0x2000)
#define MDIO_GB_RMT_REC_STAT             (0x1000)
#define MDIO_GB_LNK_1000BT_FULL          (0x0800)
#define MDIO_GB_LNK_1000BT_HALF          (0x0400)
#define MDIO_GB_ERR_COUNT_MASK           (0x00FF)

/* Reg 15: 1000Base-T Extended Status register bit defines */
#define MDIO_EXTSTAT_1000BASE_X_FULL_DUP  (0x8000)
#define MDIO_EXTSTAT_1000BASE_X_HALF_DUP  (0x4000)
#define MDIO_EXTSTAT_1000BASE_T_FULL_DUP  (0x2000)
#define MDIO_EXTSTAT_1000BASE_T_HALF_DUP  (0x1000)

/* Reg 16: PHY Specific Control bit defines */


#endif
