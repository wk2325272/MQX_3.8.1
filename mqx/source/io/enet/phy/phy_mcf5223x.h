/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: phy_mcf5223x.h$
* $Version : 3.8.4.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the Freescale embedded
*  PHY chips on mcf5223x Coldfire processors.
*
*END************************************************************************/
#ifndef _phy_mcf5223x_h_
#define _phy_mcf5223x_h_ 1

#ifdef __cplusplus
extern "C" {
#endif


/*
** Phy Register Indexes - Common Across Device Types.
*/
#define MCF5223X_PHY_CR                       0x0000    /* Control Register */
#define MCF5223X_PHY_SR                       0x0001    /* Status Register */
#define MCF5223X_PHY_IDR1                     0x0002    /* Identification Register #1 */
#define MCF5223X_PHY_IDR2                     0x0003    /* Identification Register #2 */
#define MCF5223X_PHY_ANAR                     0x0004    /* Auto-Negotiation Advertisement Register */
#define MCF5223X_PHY_ANLPAR                   0x0005    /* Auto-Negotiation Link Partner Ability Register */
#define MCF5223X_PHY_ANER                     0x0006    /* Auto-Negotiation Expansion Register */
#define MCF5223X_PHY_ANNPTR                   0x0007    /* Auto-Negotiation Next Page TX Register */
#define MCF5223X_PHY_ICR                      0x0010    /* Interrupt Control Register */
#define MCF5223X_PHY_PSR                      0x0011    /* Proprietary Status Register */
#define MCF5223X_PHY_PCR                      0x0012    /* Proprietary Control Register */


/*
** Common Register Bit Definitions
*/

/* PHY_CR Defs */
#define MCF5223X_PHY_CR_COLLISION_TEST        0x0080
#define MCF5223X_PHY_CR_FORCE_FULL_DUP        0x0100
#define MCF5223X_PHY_CR_RESTART_AUTONEG       0x0200
#define MCF5223X_PHY_CR_ISOLATE               0x0400
#define MCF5223X_PHY_CR_POWER_DOWN            0x0800
#define MCF5223X_PHY_CR_AUTO_NEG_ENABLE       0x1000
#define MCF5223X_PHY_CR_FORCE_SPEED_100       0x2000
#define MCF5223X_PHY_CR_FORCE_SPEED_10        0x0000
#define MCF5223X_PHY_CR_FORCE_SPEED_MASK      0x2000
#define MCF5223X_PHY_CR_LOOPBACK              0x4000
#define MCF5223X_PHY_CR_RESET                 0x8000

/* PHY_SR Defs */
#define MCF5223X_PHY_SR_EXTENDED_CAPABILITY   0x0001
#define MCF5223X_PHY_SR_JABBER_DETECT         0x0002
#define MCF5223X_PHY_SR_LINK_STATUS           0x0004
#define MCF5223X_PHY_SR_AN_ABILITY            0x0008
#define MCF5223X_PHY_SR_REMOTE_FAULT          0x0010
#define MCF5223X_PHY_SR_AN_COMPLETE           0x0020
#define MCF5223X_PHY_SR_PREAMBLE_SUPPRESS     0x0040
#define MCF5223X_PHY_SR_10T_HALF_DUP          0x0800
#define MCF5223X_PHY_SR_10T_FULL_DUP          0x1000
#define MCF5223X_PHY_SR_100X_HALF_DUP         0x2000
#define MCF5223X_PHY_SR_100X_FULL_DUP         0x4000
#define MCF5223X_PHY_SR_100T4_CAPABLE         0x8000

/* PHY_IDR2 Defs */
#define MCF5223X_PHY_IDR2_OUI_MASK            0xFC00
#define MCF5223X_PHY_IDR2_MODEL_NUMBER_MASK   0x03F0
#define MCF5223X_PHY_IDR2_REVISION_MASK       0x000F
#define MCF5223X_PHY_IDR2_MODEL_SHIFT         4

/* PHY_ANAR Defs */
#define MCF5223X_PHY_ANAR_PROTO_SEL_MASK      0x001F
#define MCF5223X_PHY_ANAR_PROTO_8023          0x0001
#define MCF5223X_PHY_ANAR_10T_HALF_DUP        0x0020
#define MCF5223X_PHY_ANAR_10T_FULL_DUP        0x0040
#define MCF5223X_PHY_ANAR_100T_HALF_DUP       0x0080
#define MCF5223X_PHY_ANAR_100T_FULL_DUP       0x0100
#define MCF5223X_PHY_ANAR_FLOW_CONTROL        0x0400
#define MCF5223X_PHY_ANAR_REMOTE_FAULT        0x2000
#define MCF5223X_PHY_ANAR_NEXT_PAGE           0x8000

/* PHY_ANLPAR Defs */
#define MCF5223X_PHY_ANLPAR_PROTO_SEL_MASK    0x001F
#define MCF5223X_PHY_ANLPAR_CODEFIELD_MASK    0x07FF
#define MCF5223X_PHY_ANLPAR_10T_HALF_DUP      0x0020
#define MCF5223X_PHY_ANLPAR_10T_FULL_DUP      0x0040
#define MCF5223X_PHY_ANLPAR_100T_HALF_DUP     0x0080
#define MCF5223X_PHY_ANLPAR_100T_FULL_DUP     0x0100
#define MCF5223X_PHY_ANLPAR_100T4_FULL_DUP    0x0200
#define MCF5223X_PHY_ANLPAR_FLOW_CONTROL      0x0400
#define MCF5223X_PHY_ANLPAR_TGL               0x0800
#define MCF5223X_PHY_ANLPAR_ACK2              0x1000
#define MCF5223X_PHY_ANLPAR_REMOTE_FAULT      0x2000
#define MCF5223X_PHY_ANLPAR_MESSAGE_PAGE      0x2000
#define MCF5223X_PHY_ANLPAR_ACK               0x4000
#define MCF5223X_PHY_ANLPAR_NEXT_PAGE         0x8000

/* PHY_ANER Defs */
#define MCF5223X_PHY_ANER_FAR_AN_ABLE         0x0001
#define MCF5223X_PHY_ANER_PAGE_RECEIVED       0x0002
#define MCF5223X_PHY_ANER_NEXT_PAGE_ABLE      0x0004
#define MCF5223X_PHY_ANER_FAR_NEXT_PAGE_ABLE  0x0008
#define MCF5223X_PHY_ANER_FAR_DETECT_FAULT    0x0010

/* PHY_ANNPTR Defs */
#define MCF5223X_PHY_ANNPTR_CODEFIELD_MASK    0x07FF
#define MCF5223X_PHY_ANNPTR_TOGGLE            0x0800
#define MCF5223X_PHY_ANNPTR_ACK2              0x1000
#define MCF5223X_PHY_ANNPTR_MSG_PAGE          0x2000
#define MCF5223X_PHY_ANNPTR_NEXT_PAGE         0x8000

/* PHY_ICR Defs */
#define MCF5223X_PHY_ICR_JABBER_IF            0x0001
#define MCF5223X_PHY_ICR_REMOTE_FAULT_IF      0x0002
#define MCF5223X_PHY_ICR_PAR_DETECT_FAULT_IF  0x0004
#define MCF5223X_PHY_ICR_AUTO_NEG_CHANGED_IF  0x0008
#define MCF5223X_PHY_ICR_LINK_CHANGED_IF      0x0010
#define MCF5223X_PHY_ICR_PAGE_RECEIVED_IF     0x0020
#define MCF5223X_PHY_ICR_ACK_BIT_RECEIVED_IF  0x0040
#define MCF5223X_PHY_ICR_JABBER_IE            0x0100
#define MCF5223X_PHY_ICR_REMOTE_FAULT_IE      0x0200
#define MCF5223X_PHY_ICR_PAR_DETECT_FAULT_IE  0x0400
#define MCF5223X_PHY_ICR_AUTO_NEG_CHANGED_IE  0x0800
#define MCF5223X_PHY_ICR_LINK_CHANGED_IE      0x1000
#define MCF5223X_PHY_ICR_PAGE_RECEIVED_IE     0x2000
#define MCF5223X_PHY_ICR_ACK_BIT_RECEIVED_IE  0x4000

/* PHY_PSR Defs */
#define MCF5223X_PHY_PSR_POLARITY_REVERSED    0x0020
#define MCF5223X_PHY_PSR_AN_COMMON_MODE       0x0100
#define MCF5223X_PHY_PSR_PAGE_RECEIVED        0x0200
#define MCF5223X_PHY_PSR_AN_COMPLETE          0x0400
#define MCF5223X_PHY_PSR_SPEED                0x1000
#define MCF5223X_PHY_PSR_DUPLEX_MODE          0x2000
#define MCF5223X_PHY_PSR_LINK_STATUS          0x4000

/* PHY_PCR Defs */
#define MCF5223X_PHY_PCR_TX_RX_TEST           0x0008
#define MCF5223X_PHY_PCR_TX_RX_DIS_ALOOP      0x0010
#define MCF5223X_PHY_PCR_SCRAMBLER_BYPASS     0x0020
#define MCF5223X_PHY_PCR_ENCODER_BYPASS       0x0040
#define MCF5223X_PHY_PCR_DISABLE_ALIGN        0x0080
#define MCF5223X_PHY_PCR_DISABLE_POL_CONN     0x0100
#define MCF5223X_PHY_PCR_LINK_TEST_DIS        0x0200
#define MCF5223X_PHY_PCR_JABBER_DETECT_ENABLE 0x0400
#define MCF5223X_PHY_PCR_MII_LOOP_DIS         0x2000
#define MCF5223X_PHY_PCR_FAR_FAULT_DIS        0x4000


extern const ENET_PHY_IF_STRUCT phy_mcf5223x_IF;

#define MII_TIMEOUT                     (0x10000)

#ifdef __cplusplus
}
#endif

#endif

/* EOF */
