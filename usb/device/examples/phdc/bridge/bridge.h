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
* $FileName: bridge.h$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief The file contains macros and functiuon api declarations needed by 
*        phdc bridge app
*
*****************************************************************************/

#ifndef _BRIDGE_H
#define _BRIDGE_H
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_descriptor.h"
#include "ieee11073_phd_types.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define ETHERNET_BRIDGE                                 (1)

#define BRIDGE_TYPE                                     (ETHERNET_BRIDGE)

/****** BRIDGE SPECIFIC INCLUDES AND DEFINES ****************/
#if (BRIDGE_TYPE == ETHERNET_BRIDGE)
/* Configure the IP address of Agent as 172.16.0.xxx, 
   where xxx is any number from 1 to 254 except 216*/
    #include "phdc_server.h"
    #define Bridge_Interface_Init       Ethernet_Bridge_Init
    #define Bridge_Interface_Open       Ethernet_Bridge_Open
    #define Bridge_Interface_Read       Ethernet_Bridge_Recv 
    #define Bridge_Interface_Write      Ethernet_Bridge_Send 
    #define Bridge_Interface_Close      Ethernet_Close_Connection
#else
    #error "Undefined Bridge Type!!!"
#endif
/**********************************************************/

#define SEND_DATA_QOS                                   (0x88)

#define APDU_HEADER_SIZE                                (4)

#ifndef ABORT_MESSAGE_SIZE                              
    #define ABORT_MESSAGE_SIZE                              (6)
#endif  

#define RECV_TASK                       (12)
#define SEND_TASK                       (13)

#define USB_ENUM_COMPLETED              (0x00000001)
#define USB_ENUM_COMPLETED_MASK         (0x00000001)
#define BRIDGE_RECV_EVENT_MASK          (0x00000002)
#define BRIDGE_RECV_EVENT               (0x00000002)
#define BRIDGE_SEND_EVENT_MASK          (0x00000004)
#define BRIDGE_SEND_EVENT               (0x00000004)
/******************************************************************************
 * Types
 *****************************************************************************/ 
typedef struct _bridge_variable_struct
{
    uint_32 handle;
    uint_8 usb_enum_complete;
}BRIDGE_GLOBAL_VARIABLE_STRUCT, _PTR_ PTR_BRIDGE_GLOBAL_VARIABLE_STRUCT;

/******************************************************************************
 * Global Functions
 *****************************************************************************/

#endif

/* EOF */
