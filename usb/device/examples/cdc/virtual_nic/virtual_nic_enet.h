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
* $FileName: virtual_nic_enet.h$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief The file contains Macro's and structures needed by the Network 
*        Interface for Virtual NIC Application
*
*****************************************************************************/

#ifndef _VIRTUAL_NIC_ENET_H
#define _VIRTUAL_NIC_ENET_H  1


/******************************************************************************
 * Includes
 *****************************************************************************/
#include "virtual_nic.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define ENET_DEVICE                 (BSP_DEFAULT_ENET_DEVICE)

#ifndef ENETPROT_IP
    #define ENETPROT_IP                 (0x0800)
#endif  

#ifndef ENETPROT_ARP                
    #define ENETPROT_ARP                (0x0806)
#endif
#define RNDIS_ETHER_HDR_SIZE            (14)
#define REMOTE_NDIS_PACKET_MSG          (0x00000001)
#define RNDIS_USB_OVERHEAD_SIZE         (44)
#define RNDIS_DATA_OFFSET               (36)
#define MAC_ADDR_SIZE                   (6)

// Helpful macros
#define IPBYTES(a)            ((a>>24)&0xFF),((a>>16)&0xFF),((a>> 8)&0xFF),(a&0xFF)
#define IPADDR(a,b,c,d)       ((a&0xFF)<<24)|((b&0xFF)<<16)|((c&0xFF)<<8)|(d&0xFF)


#ifndef ENET_IPADDR
    #define ENET_IPADDR  IPADDR(169,254,3,3)/* Default IP Address */
#endif

#ifndef ENET_IPMASK
    #define ENET_IPMASK  IPADDR(255,255,0,0) 
#endif

/*****************************************************************************
 * Global variables
 *****************************************************************************/

/******************************************************************************
 * Types
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint_32 VNIC_FEC_Init(CDC_CONFIG_STRUCT_PTR cdc_config_ptr);
extern uint_32 VNIC_FEC_Send(pointer arg, uint_8_ptr ethernet_frame_ptr, uint_32 payload_size);

#endif 


/* EOF */
