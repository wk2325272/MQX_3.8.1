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
* $FileName: phy_rt8211.h$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the Natioanl Semiconductor
*  PHY chips DP83848, DP83849, DP83640.
*
*END************************************************************************/
#ifndef _phy_rt8211_h_
#define _phy_rt8211_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

/*
** Phy Register Indexes - Common Across Device Types.
*/
#define RT8211_REG_BMCR       0x0000	/* Basic Mode Control Register */
#define RT8211_REG_BMSR       0x0001	/* Basic Mode Status Register */
#define RT8211_REG_IDR1       0x0002	/* PHY Identification Register #1 */
#define RT8211_REG_IDR2       0x0003	/* PHY Identification Register #2 */
#define RT8211_REG_ANAR       0x0004	/* Auto-Negotiation Advertisement Register */
#define RT8211_REG_ANLPAR     0x0005	/* Auto-Negotiation Link Partner Ability Register */
#define RT8211_REG_ANER       0x0006	/* Auto-Negotiation Expansion Register */
#define RT8211_REG_ANNPTR     0x0007	/* Auto-Negotiation Next Page TX Register */
#define RT8211_REG_ANNPRR 	   0x0008	/* Auto-Negotiation Next Page RX Register */
#define RT8211_REG_GBCR       0x0009	/* 1000BaseT Control Register */
#define RT8211_REG_GBSR       0x000a	/* 1000BaseT Status Register */
#define RT8211_REG_RES1       0x000b	/* Reserved */
#define RT8211_REG_PHYCR 	   0x0010	/* PHY Control Register */
#define RT8211_REG_PHYSR      0x0011	/* PHY Status Register */
#define RT8211_REG_INER   	   0x0012	/* Interrupt Enable Register */
#define RT8211_REG_INSR		   0x0013	/* MII Interrupt Status and Event Control Register */
#define RT8211_REG_FCSCR  	   0x0014	/* False Carrier Sense Counter Register */
#define RT8211_REG_RXECR      0x0015	/* Receive Error Counter Register */
#define RT8211_REG_PCSR   		0x0016	/* PCS Sub-Layer Configuration and Status Register */
#define RT8211_REG_RBR    		0x0017	/* RMII and Bypass Register */
#define RT8211_REG_LEDCR  		0x0018	/* LED Direct Control Register */

/* Reserved Registers */


#define RT8211_REG_PHYCTRL		0x0019	/* PHY Control Register */
#define RT8211_REG_10BTSCR		0x001A	/* 10Base-T Status/Control Register */
#define RT8211_REG_CDCTRL1		0x001B	/* CD Test Control Register and BIST Extensions Register */
#define RT8211_REG_EDCR   		0x001D	/* Energy Detect Control Register */


/*
** Common Register Bit Definitions
*/

/* PHY_BMCR Defs */
#define RT8211_REG_BMCR_SPEED1              0x0040
#define RT8211_REG_BMCR_COLLISION_TEST      0x0080
#define RT8211_REG_BMCR_FORCE_FULL_DUP      0x0100
#define RT8211_REG_BMCR_RESTART_AUTONEG     0x0200
#define RT8211_REG_BMCR_ISOLATE             0x0400
#define RT8211_REG_BMCR_POWER_DOWN          0x0800
#define RT8211_REG_BMCR_AUTO_NEG_ENABLE     0x1000
#define RT8211_REG_BMCR_SPEED0              0x2000
#define RT8211_REG_BMCR_LOOPBACK            0x4000
#define RT8211_REG_BMCR_RESET               0x8000

#define RT8211_REG_BMCR_FORCE_SPEED_MASK    (RT8211_REG_BMCR_SPEED0|RT8211_REG_BMCR_SPEED1)
#define RT8211_REG_BMCR_FORCE_SPEED_1000    (RT8211_REG_BMCR_SPEED1)
#define RT8211_REG_BMCR_FORCE_SPEED_100     (RT8211_REG_BMCR_SPEED0)
#define RT8211_REG_BMCR_FORCE_SPEED_10      (RT8211_REG_BMCR_SPEED0|RT8211_REG_BMCR_SPEED1)

/* PHY_BMSR Defs */
#define RT8211_REG_BMSR_EXTENDED_CAPABLE    0x0001
#define RT8211_REG_BMSR_JABBER_DETECT       0x0002
#define RT8211_REG_BMSR_LINK_STATUS         0x0004
#define RT8211_REG_BMSR_AUTO_NEG_ABILITY    0x0008
#define RT8211_REG_BMSR_REMOTE_FAULT        0x0010
#define RT8211_REG_BMSR_AUTO_NEG_COMPLETE   0x0020
#define RT8211_REG_BMSR_PREAMBLE_SUPPRESS   0x0040
#define RT8211_REG_BMSR_RESERVED            0x0080
#define RT8211_REG_BMSR_1000T_EXT_STATUS    0x0100
#define RT8211_REG_BMSR_100T2_HALF_DUP      0x0200
#define RT8211_REG_BMSR_100T2_FULL_DUP      0x0400
#define RT8211_REG_BMSR_10T_HALF_DUP        0x0800
#define RT8211_REG_BMSR_10T_FULL_DUP        0x1000
#define RT8211_REG_BMSR_100X_HALF_DUP       0x2000
#define RT8211_REG_BMSR_100X_FULL_DUP       0x4000
#define RT8211_REG_BMSR_100T4_CAPABLE       0x8000

/* PHY_IDR1 Defs */
#define RT8211_REG_IDR1_NATIONAL_OUI_VAL    0x2000    /* OUI bits 3-18 */

/* PHY_IDR2 Defs */
#define RT8211_REG_IDR2_OUI_MASK            0xFC00
#define RT8211_REG_IDR2_MODEL_NUMBER_MASK   0x03F0
#define RT8211_REG_IDR2_REVISION_MASK       0x000F
#define RT8211_REG_IDR2_MODEL_SHIFT         4

#define RT8211_REG_IDR2_NATIONAL_OUI_VAL    0x5C00    /* OUI bits 19-24 (15:10), Vendor Model (9:4), Revision (3:0) */
#define RT8211_REG_IDR2_MODEL_DP83848_VAL   0x0090    /* AspenPhy */
#define RT8211_REG_IDR2_MODEL_DP48_MINI_VAL 0x00C0    /* AspenPhy - Mini */
#define RT8211_REG_IDR2_MODEL_DP83849_VAL   0x00A0    /* Dual AspenPhy */
#define RT8211_REG_IDR2_MODEL_DP83640_VAL   0x00E0    /* High Precision Phy */

#define RT8211_REG_IDR2_REV_DP83848E_VAL    0x0001    /* DP83848 Enhanced version */

/* PHY_ANAR Defs */
#define RT8211_REG_ANAR_PROTO_SEL_MASK      0x001F
#define RT8211_REG_ANAR_PROTO_8023          0x0001
#define RT8211_REG_ANAR_10T_HALF_DUP        0x0020
#define RT8211_REG_ANAR_10T_FULL_DUP        0x0040
#define RT8211_REG_ANAR_100T_HALF_DUP       0x0080
#define RT8211_REG_ANAR_100T_FULL_DUP       0x0100
#define RT8211_REG_ANAR_100T4_SUPPORT       0x0200
#define RT8211_REG_ANAR_PAUSE_SUPPORT       0x0400
#define RT8211_REG_ANAR_ASY_PAUSE_SUPPORT   0x0800
#define RT8211_REG_ANAR_RESERVED0           0x1000
#define RT8211_REG_ANAR_REMOTE_FAULT        0x2000
#define RT8211_REG_ANAR_RESERVED1           0x4000
#define RT8211_REG_ANAR_NEXT_PAGE_IND       0x8000

/* PHY_ANLPAR Defs */
#define RT8211_REG_ANLPAR_PROTO_SEL_MASK    0x001F
#define RT8211_REG_ANLPAR_10T_HALF_DUP      0x0020
#define RT8211_REG_ANLPAR_10T_FULL_DUP      0x0040
#define RT8211_REG_ANLPAR_100T_HALF_DUP     0x0080
#define RT8211_REG_ANLPAR_100T_FULL_DUP     0x0100
#define RT8211_REG_ANLPAR_100T4_SUPPORT     0x0200
#define RT8211_REG_ANLPAR_PAUSE_SUPPORT     0x0400
#define RT8211_REG_ANLPAR_ASY_PAUSE         0x0800
#define RT8211_REG_ANLPAR_RESERVED0         0x1000
#define RT8211_REG_ANLPAR_REMOTE_FAULT      0x2000
#define RT8211_REG_ANLPAR_ACK               0x4000
#define RT8211_REG_ANLPAR_NEXT_PAGE_IND     0x8000

/* PHY_ANER Defs */
#define RT8211_REG_ANER_AUTO_NEG_CAPABLE    0x0001
#define RT8211_REG_ANER_PAGE_RX             0x0002
#define RT8211_REG_ANER_NEXT_PAGE_ABLE      0x0004
#define RT8211_REG_ANER_PRT_NEXT_PAGE_ABLE  0x0008
#define RT8211_REG_ANER_PARALLEL_DET_FAULT  0x0010

/* PHY_ANNPTR Defs */
#define RT8211_REG_ANNPTR_CODE_MASK         0x07FF
#define RT8211_REG_ANNPTR_TOGGLE            0x0800
#define RT8211_REG_ANNPTR_ACK2              0x1000
#define RT8211_REG_ANNPTR_MSG_PAGE          0x2000
#define RT8211_REG_ANNPTR_ACK               0x4000
#define RT8211_REG_ANNPTR_NEXT_PAGE_IND     0x8000

/* RT8211 Specific Registers */

/* 1000BaseT Control Defs */
#define RT8211_REG_GBCR_TESTMODE            0xE000
#define RT8211_REG_GBCR_MANUAL_MASTER       0x1000
#define RT8211_REG_GBCR_ADVERTISE_MASTER    0x0800
#define RT8211_REG_GBCR_PORT_PREF_MASTER    0x0400
#define RT8211_REG_GBCR_ADVERTISE_FULLDUP   0x0200
#define RT8211_REG_GBCR_ADVERTISE_HALFDUP   0x0100

/* PHY_PHYSR Defs */
#define RT8211_REG_PHYSR_SPEED              0xC000
#define RT8211_REG_PHYSR_1000MB             0x8000
#define RT8211_REG_PHYSR_100MB              0x4000
#define RT8211_REG_PHYSR_10MB               0x0000
#define RT8211_REG_PHYSR_FULL_DUPLEX        0x2000
#define RT8211_REG_PHYSR_PAGE_REC           0x1000
#define RT8211_REG_PHYSR_SPEED_DUP_RESOLVED 0x0800
#define RT8211_REG_PHYSR_LINK_OK            0x0400
#define RT8211_REG_PHYSR_MDI_CROSSOVER      0x0040
#define RT8211_REG_PHYSR_JABBER             0x0001

/* PHY IDENTIFIER */
#define RT8211_PHY_ID                       28


extern const ENET_PHY_IF_STRUCT phy_rt8211_IF;

#define MII_TIMEOUT                     (0x10000)

extern boolean phy_rt8211_discover_addr(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern boolean phy_rt8211_init(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern uint_32 phy_rt8211_get_speed(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern boolean phy_rt8211_get_link_status(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern boolean phy_rt8211_get_duplex(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern void phy_rt8211_dump( ENET_CONTEXT_STRUCT_PTR enet_ptr );


#ifdef __cplusplus
}
#endif

#endif

/* EOF */
