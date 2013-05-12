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
* $FileName: config.h$
* $Version : 3.8.B.5$
* $Date    : Jun-22-2012$
*
* Comments:
*
*   The Common Configurable information for the RTCS 
*   Shell examples.
*
*END************************************************************************/


 
/* RAMDISK can not be used from MRAM targets !!!  Whole MRAM is used for RamDisk. */ 
#define DEMOCFG_ENABLE_RAMDISK     0
/* Enable iwconfig commad */
#define DEMOCFG_USE_WIFI           0
/* Enable PPP */
#define DEMOCFG_ENABLE_PPP         0


#define ENET_DEVICE BSP_DEFAULT_ENET_DEVICE

/* PPP device must be set manually and 
** must be different from the default IO channel (BSP_DEFAULT_IO_CHANNEL) 
*/

/* if mcf52259evb */
#define PPP_DEVICE      "ittyb:" 


#ifdef ENET_DEVICE
/*
** The target's IP address and netmask
*/

#ifndef ENET_IPADDR
    #define ENET_IPADDR  IPADDR(192,168,1,4) 
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,255,0) 
#endif

#endif  /* ENET_DEVICE */


#if DEMOCFG_USE_WIFI
   #include "iwcfg.h"
   
   #define DEMOCFG_SSID            "NGZG"
   //Possible Values ENET_MEDIACTL_MODE_INFRA or ENET_MEDIACTL_MODE_ADHOC
   #define DEMOCFG_NW_MODE         ENET_MEDIACTL_MODE_INFRA 
   /* Posible values.
    * 1. ENET_MEDIACTL_SECURITY_TYPE_WPA
    * 2.ENET_MEDIACTL_SECURITY_TYPE_WPA2
    * 3.ENET_MEDIACTL_SECURITY_TYPE_NONE
    * 4.ENET_MEDIACTL_SECURITY_TYPE_WEP
    */   
   #define DEMOCFG_SECURITY        ENET_MEDIACTL_SECURITY_TYPE_NONE 
   #define DEMOCFG_PASSPHRASE      NULL
   #define DEMOCFG_WEP_KEY         "ABCDE"
   #define DEMOCFG_WEP_KEY_INDEX   1 //can be 1,2,3, or 4
   #define DEMOCFG_DEFAULT_DEVICE  1
#endif

#ifdef PPP_DEVICE
/*
** Define PPP_DEVICE_DUN or PPP_DEVICE_RAS only when using PPP 
** to communicate to Win9x Dial-Up Networking over a null-modem.
** Define PPP_DEVICE_DUN if the board as PPP server is requested.
** Define PPP_DEVICE_RAS if the board as PPP client is requested.
** This is ignored if PPP_DEVICE is not #define'd
*/

#define PPP_DEVICE_DUN  0    /* The board serves as the PPP server */
#define PPP_DEVICE_RAS  1    /* The board serves as the PPP client */ 

/*
** Define the local and remote IP addresses for the PPP link
** These are ignored if PPP_DEVICE is not #define'd
*/
#define PPP_LOCADDR     IPADDR(192,168,0,216)
#define PPP_PEERADDR    IPADDR(192,168,0,217)

/*
** Define a default gateway
*/
#define GATE_ADDR       IPADDR(192,168,0,1)

#endif /* PPP_DEVICE */

extern int_32 shell_ppp_start(int_32 argc, char_ptr argv[] );
extern int_32 shell_ppp_stop(int_32 argc, char_ptr argv[] );




/* EOF */
