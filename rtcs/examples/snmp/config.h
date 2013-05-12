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
* $Version : 3.7.10.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   Configurable information for the RTCS examples.
*
*END************************************************************************/

/* Disable the cache */
#define MON_TYPE   MQX_MONITOR_TYPE_BDM

/*
** Uncomment this line to enable Ethernet
*/
#define ENET_DEVICE     0

/*
** Define the Ethernet MAC address, IP address and IP network mask
** These are ignored if ENET_DEVICE is not #define'd
*/
#ifndef ENET_IPADDR
    #define ENET_IPADDR  IPADDR(129,9,200,1) 
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,255,0) 
#endif

/* Define TRAP receiver addresses here */
#define ENET_TRAP_ADDR1 IPADDR(129,9,200,30)
#define ENET_TRAP_ADDR2 IPADDR(129,9,200,39)
#define ENET_TRAP_ADDR3 IPADDR(129,9,200,45)
#define ENET_TRAP_ADDR4 IPADDR(129,9,200,250)
#define ENET_TRAP_ADDR5 IPADDR(129,9,200,180)

/*
** The target's Ethernet address
** The first three bytes should be 00 00 5E
** The last three bytes should match the last three bytes of
** your test IP address
*/
#undef  _IPBN       /* IP address byte number b */
#define _IPBN(b)    (((ENET_IPADDR) >> (b*8)) & 255)
#define ENET_ENETADDR   {0x00, 0x00, 0x5E, _IPBN(2), _IPBN(1), _IPBN(0)}

/*
** Uncomment this line to enable PPP
*/

//#define PPP_DEVICE      "ittya:"

/*
** Define PPP_DEVICE_DUN only when using PPP to communicate
** to Win9x Dial-Up Networking over a null-modem
** This is ignored if PPP_DEVICE is not #define'd
*/
#define PPP_DEVICE_DUN

/*
** Define the local and remote IP addresses for the PPP link
** These are ignored if PPP_DEVICE is not #define'd
*/
#define PPP_LOCADDR     0xC009C849
#define PPP_PEERADDR    0xC009C8C9

/*
** Define a default gateway
*/
#define GATE_ADDR       0xC009C801

#ifndef ENET_IPGATEWAY
    #define ENET_IPGATEWAY  IPADDR(169,254,0,1) 
#endif

/*
** Define a server to be used by the example clients
*/
//#define SERVER_ADDR     0xC00002CC

/* EOF */
