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
* $FileName: phy_lxt971.h$
* $Version : 3.0.1.0$
* $Date    : Jun-23-2009$
*
* Comments:
*
*   This file contains definitions that belongs to the PHY chip
*   LXT971A.
*
*END************************************************************************/
#ifndef _lxt971_h_
#define _lxt971_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

/*
** Register definitions for the PHY.
*/

enum lxt971_phy_reg {
   LXT971_MII_REG_CR       = 0,  // Control Register
   LXT971_MII_REG_SR       = 1,  // Status Register
   LXT971_MII_REG_PHYIR1   = 2,  // PHY Identification Register 1
   LXT971_MII_REG_PHYIR2   = 3,  // PHY Identification Register 2
   LXT971_MII_REG_ANAR     = 4,  // A-N Advertisement Register
   LXT971_MII_REG_ANLPAR   = 5,  // A-N Link Partner Ability Register
   LXT971_MII_REG_ANER     = 6,  // A-N Expansion Register
   LXT971_MII_REG_ANNPTR   = 7,  // A-N Next Page Transmit Register
   LXT971_MII_REG_ANLPRNPR = 8,  // A-N Link Partner Received Next Page Reg.

   LXT971_MII_REG_XSTAT    = 15,  // Extended status register (MPC5200)
   LXT971_MII_REG_PCR      = 16,  // Port Control Register
   LXT971_MII_REG_SR2      = 17,  // Status Register 2
   LXT971_MII_REG_IER      = 18,  // Interrupt Enable Register
   LXT971_MII_REG_ISR      = 19,  // Interrupt Status Register
   LXT971_MII_REG_LCR      = 20,  // LED Control Register
   LXT971_MII_REG_TCR      = 30   // Transmit Control Register
};

// values for LXT971_MII_REG_SR Status Register

#define LXT971_MII_REG_SR_EXTCAP        0x0001
#define LXT971_MII_REG_SR_JABBER        0x0002
#define LXT971_MII_REG_SR_LINK_STATUS   0x0004
#define LXT971_MII_REG_SR_AN_ABLE       0x0008 // Auto-negotiate ability
#define LXT971_MII_REG_SR_REMOTE_FAULT  0x0010
#define LXT971_MII_REG_SR_AN_COMPLETE   0x0020 // Auto-negotiate completed


// values for LXT971_MII_REG_SR2 Status Register

#define LXT971_MII_REG_SR2_10_100_MODE  0x4000 // 10/100 Mode


/*==========================================================================*/
/*
** Definitions defined by Motorola in the MPC5200
*/
#define LXT971_MII_DATA_ST              0x40000000      // Start of frame delimiter
#define LXT971_MII_DATA_OP_RD           0x20000000      // Perform a read operation
#define LXT971_MII_DATA_OP_WR           0x10000000      // Perform a write operation
#define LXT971_MII_DATA_PA_MSK          0x0f800000      // PHY Address field mask
#define LXT971_MII_DATA_RA_MSK          0x007c0000      // PHY Register field mask
#define LXT971_MII_DATA_TA              0x00020000      // Turnaround
#define LXT971_MII_DATA_DATAMSK         0x00000fff      // PHY data field

#define LXT971_MII_DATA_RA_SHIFT        0x12            // MII register address bits
#define LXT971_MII_DATA_PA_SHIFT        0x17            // MII PHY address bits

/*
 * MII control register bits
 */
#define LXT971_MII_CNTL_RESET           0x8000  // PHY reset
#define LXT971_MII_CNTL_LOOPBACK        0x4000  // enable loopback mode
//                                      0x2040  reserved
#define LXT971_MII_CNTL_SPEED_1000      0x0040  // set speed 1000 Mbps
#define LXT971_MII_CNTL_SPEED_100       0x2000  // set speed 100 Mbps
#define LXT971_MII_CNTL_SPEED_10        0x0000  // set speed 10 Mbps
#define LXT971_MII_CNTL_AUTONEG_EN      0x1000  // enable auto-negotiation
#define LXT971_MII_CNTL_POWER_DOWN      0x0800  // power down
#define LXT971_MII_CNTL_ISOLATE         0x0400  // electrically isolate PHY from MII
#define LXT971_MII_CNTL_RESTART_AN      0x0200  // restart auto-negotiation
#define LXT971_MII_CNTL_FDX             0x0100  // full duplex mode
#define LXT971_MII_CNTL_COL_TEST        0x0080  // enable collision signal test
//                                      0x003f  reserved

/*
 * MII status register bits
 */
#define LXT971_MII_STAT_100BT4          0x8000  // supports 100Base-T4
#define LXT971_MII_STAT_100BX_FDX       0x4000  // supports 100Base-X full duplex
#define LXT971_MII_STAT_100BX_HDX       0x2000  // supports 100Base-X half duplex
#define LXT971_MII_STAT_10_FDX          0x1000  // supports 10 Mbps full duplex
#define LXT971_MII_STAT_10_HDX          0x0800  // supports 10 Mbps half duplex
#define LXT971_MII_STAT_100BT2_FDX      0x0400  // supports 100Base-T2 full duplex
#define LXT971_MII_STAT_100BT2_HDX      0x0200  // supports 100Base-T2 half duplex
#define LXT971_MII_STAT_EXT             0x0100  // extended status in reg 15
//                                      0x0080  reserved
#define LXT971_MII_STAT_MFP_SUPR        0x0040  // mgmt frames w/ preamble suppressed
#define LXT971_MII_STAT_AUTO_NEG        0x0020  // auto-negotiation complete
#define LXT971_MII_STAT_REM_FAULT       0x0010  // remote fault condition detected
#define LXT971_MII_STAT_AN_CAP          0x0008  // PHY able to perform auto-negotiation
#define LXT971_MII_STAT_LINK            0x0004  // link status, 1=up
#define LXT971_MII_STAT_JABBER          0x0002  // jabber condition detected
#define LXT971_MII_STAT_EXT_CAP         0x0001  // extended register capable

/*
 * MII auto-negotiation advertisement bits
 */
#define LXT971_MII_AN_NEXT_PAGE         0x8000  // port has ability to send mult pages
//                                      0x4000  reserved
#define LXT971_MII_AN_REM_FAULT         0x2000  // remote fault
//                                      0x1000  reserved
#define LXT971_MII_AN_ASYM_PAUSE        0x0800  // asymmetric pause
#define LXT971_MII_AN_PAUSE             0x0400  // pause enabled for full duplex links
#define LXT971_MII_AN_100BT4            0x0200  // advertise 100Base-T4 capable
#define LXT971_MII_AN_100BTX_FDX        0x0100  // advertise 100Base-TX full duplex cap
#define LXT971_MII_AN_100BTX            0x0080  // advertise 100Base-TX capable
#define LXT971_MII_AN_10BT_FDX          0x0040  // advertise 10Base-T full duplex capab
#define LXT971_MII_AN_10BT              0x0020  // advertise 10Base-T capable
#define LXT971_MII_AN_802_9             0x0002  // IEEE 802.9 ISLAN-16T
#define LXT971_MII_AN_802_3             0x0001  // IEEE 802.3

/*
 * MII LED configuration
 */
#define LXT971_MII_LED_SPEED            0x0     // display speed status (continuous)
#define LXT971_MII_LED_TX               0x1     // display transmit status (stretched)
#define LXT971_MII_LED_RX               0x2     // display receive status (stretched)
#define LXT971_MII_LED_COL              0x3     // display collision status (stretched)
#define LXT971_MII_LED_LINK             0x4     // display link status (continuous)
#define LXT971_MII_LED_DUPLEX           0x5     // display duplex status (continuous)
//                                      0x6     unused
#define LXT971_MII_LED_RXTX             0x7     // display rx or tx activity (stretch)
#define LXT971_MII_LED_ON               0x8     // test mode, turn on LED (continuous)
#define LXT971_MII_LED_OFF              0x9     // test mode, turn off LED (continuous)
#define LXT971_MII_LED_BLINK_FAST       0xa     // test mode, blink LED fast (cont)
#define LXT971_MII_LED_BLINK_SLOW       0xb     // test mode, blink LED slow (cont)
#define LXT971_MII_LED_LINK_RX          0xc     // display link and rx status combined
#define LXT971_MII_LED_LINK_ACT         0xd     // display link and rx/tx activity comb
#define LXT971_MII_LED_DUPLEX_COL       0xe     // display duplex and collision comb
//                                      0xf     unused

#define LXT971_MII_LED1_MASK            0xf000  // mask the bits above for LED 1
#define LXT971_MII_LED2_MASK            0x0f00  // mask the bits above for LED 2
#define LXT971_MII_LED3_MASK            0x00f0  // mask the bits above for LED 3
#define LXT971_MII_LED1_SHIFT           12      // shift the bits above to LED 1
#define LXT971_MII_LED2_SHIFT           8       // shift the bits above to LED 2
#define LXT971_MII_LED3_SHIFT           4       // shift the bits above to LED 3

#define LXT971_MII_LED_PULSE_30         0x0000  // stretch LED events to 30ms
#define LXT971_MII_LED_PULSE_60         0x0004  // stretch LED events to 60ms
#define LXT971_MII_LED_PULSE_100        0x0008  // stretch LED events to 100ms
//                                      0x000c  unused
#define LXT971_MII_LED_PULSE_STRE       0x0002  // enable pulse stretching of all LEDs
//                                      0x0001  unused


#ifdef __cplusplus
}
#endif

#endif /* _lxt971_h_ */
/* EOF */
