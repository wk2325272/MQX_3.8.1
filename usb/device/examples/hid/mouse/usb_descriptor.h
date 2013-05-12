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
* $FileName: usb_descriptor.h$
* $Version : 3.6.2.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
* @brief The file contains USB stack class layer api header function.
*
*****************************************************************************/

#ifndef _USB_DESCRIPTOR_H
#define _USB_DESCRIPTOR_H 1

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "usb_class.h"
#include "usb_hid.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define BCD_USB_VERSION                  (0x0200)

#define REMOTE_WAKEUP_SHIFT              (5)
#define REMOTE_WAKEUP_SUPPORT            (0)/*1:TRUE;0:FALSE*/

/* Various descriptor sizes */
#define DEVICE_DESCRIPTOR_SIZE            (18)
#define CONFIG_DESC_SIZE                  (34)
#define REPORT_DESC_SIZE                  (52)
#define CONFIG_ONLY_DESC_SIZE             (9)
#define IFACE_ONLY_DESC_SIZE              (9)   
#define HID_ONLY_DESC_SIZE                (9)   
#define ENDP_ONLY_DESC_SIZE               (7)

#if HIGH_SPEED_DEVICE
    #define DEVICE_QUALIFIER_DESCRIPTOR_SIZE    (10)
    #define OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE  (CONFIG_DESC_SIZE)  
#endif

/* HID buffer size */
#define HID_BUFFER_SIZE                   (8)
/* Max descriptors provided by the Application */
#define USB_MAX_STD_DESCRIPTORS               (8)
#define USB_MAX_CLASS_SPECIFIC_DESCRIPTORS    (2)
/* Max configuration supported by the Application */
#define USB_MAX_CONFIG_SUPPORTED          (1)

/* Max string descriptors supported by the Application */
#define USB_MAX_STRING_DESCRIPTORS        (3)

/* Max language codes supported by the USB */
#define USB_MAX_LANGUAGES_SUPPORTED       (1)


#define HID_DESC_ENDPOINT_COUNT (1)
#define HID_ENDPOINT (1)
#define HID_ENDPOINT_PACKET_SIZE (8)


/* string descriptors sizes */
#define USB_STR_DESC_SIZE (2)
#define USB_STR_0_SIZE  (2)
#define USB_STR_1_SIZE  (56)
#define USB_STR_2_SIZE  (34)
#define USB_STR_n_SIZE  (32)

/* descriptors codes */
#define USB_DEVICE_DESCRIPTOR     (1)
#define USB_CONFIG_DESCRIPTOR     (2)
#define USB_STRING_DESCRIPTOR     (3)
#define USB_IFACE_DESCRIPTOR      (4)
#define USB_ENDPOINT_DESCRIPTOR   (5)

#if HIGH_SPEED_DEVICE
    #define USB_DEVQUAL_DESCRIPTOR      (6)
    #define USB_OTHER_SPEED_DESCRIPTOR  (7)
#endif

#define USB_HID_DESCRIPTOR        (0x21)
#define USB_REPORT_DESCRIPTOR     (0x22)

#define USB_MAX_SUPPORTED_INTERFACES     (1)

#define DEVICE_DESC_DEVICE_CLASS             (0x00)
#define DEVICE_DESC_DEVICE_SUBCLASS          (0x00)
#define DEVICE_DESC_DEVICE_PROTOCOL          (0x00)
#define DEVICE_DESC_NUM_CONFIG_SUPPOTED      (0x01)
/* Keep the following macro Zero if you dont Support Other Speed Configuration
   If you suppoort Other Speeds make it 0x01 */
#define DEVICE_OTHER_DESC_NUM_CONFIG_SUPPOTED  (0x00) 
#define CONFIG_DESC_NUM_INTERFACES_SUPPOTED  (0x01)
#define CONFIG_DESC_CURRENT_DRAWN            (0x32)

/******************************************************************************
 * Types
 *****************************************************************************/ 

/******************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint_8 USB_Desc_Get_Descriptor( 
     HID_HANDLE handle, 
     uint_8 type,
     uint_8 str_num, 
     uint_16 index,
     uint_8_ptr *descriptor,
     uint_32 *size);
      
extern uint_8 USB_Desc_Get_Interface(
                              HID_HANDLE handle, 
                              uint_8 interface, 
                              uint_8_ptr alt_interface);


extern uint_8 USB_Desc_Set_Interface(
                              HID_HANDLE handle, 
                              uint_8 interface, 
                              uint_8 alt_interface);

extern boolean USB_Desc_Valid_Configation(HID_HANDLE handle, 
                                                          uint_16 config_val); 

extern boolean USB_Desc_Remote_Wakeup(HID_HANDLE handle); 

extern USB_ENDPOINTS *USB_Desc_Get_Endpoints(HID_HANDLE handle); 

#endif

/* EOF */
