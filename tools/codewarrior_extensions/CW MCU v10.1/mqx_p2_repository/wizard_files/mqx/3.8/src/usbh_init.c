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
*END************************************************************************/
#include "main.h"
#include "usb_classes.h"

static _usb_host_handle host_handle;  /* Global handle for calling host   */

extern void usb_host_cdc_acm_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);
extern void usb_host_cdc_data_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);
extern void usb_host_hid_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);
extern void usb_host_msd_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);
void usb_host_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);

/************************************************************************************
Table of driver capabilities this application wants to use. See Host API document for
details on How to define a driver info table.
************************************************************************************/

static USB_HOST_DRIVER_INFO DriverInfoTable[] =
{
#ifdef USBCLASS_INC_HUB
    /* USB 1.1 hub */
    {
        {0x00,0x00},                  /* Vendor ID per USB-IF             */
        {0x00,0x00},                  /* Product ID per manufacturer      */
        USB_CLASS_HUB,                /* Class code                       */
        USB_SUBCLASS_HUB_NONE,        /* Sub-Class code                   */
        USB_PROTOCOL_HUB_LS,          /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_hub_device_event     /* Application call back function   */
    },
#endif
#ifdef USBCLASS_INC_CDC
   /* Abstract control class */
   {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        USB_CLASS_COMMUNICATION,      /* Class code                       */
        USB_SUBCLASS_COM_ABSTRACT,    /* Sub-Class code                   */
        0xFF,                         /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_cdc_acm_event        /* Application call back function   */
   },
   /* Data flow class */
   {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        USB_CLASS_DATA,               /* Class code                       */
        0xFF,                         /* Sub-Class code                   */
        0xFF,                         /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_cdc_data_event       /* Application call back function   */
    },
#endif
#ifdef USBCLASS_INC_MASS
    /* Floppy drive */
    {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        USB_CLASS_MASS_STORAGE,       /* Class code                       */
        USB_SUBCLASS_MASS_UFI,        /* Sub-Class code                   */
        USB_PROTOCOL_MASS_BULK,       /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_msd_event            /* Application call back function   */
    },
    /* USB 2.0 hard drive */
    {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        USB_CLASS_MASS_STORAGE,       /* Class code                       */
        USB_SUBCLASS_MASS_SCSI,       /* Sub-Class code                   */
        USB_PROTOCOL_MASS_BULK,       /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_msd_event            /* Application call back function   */
    },
#endif
#ifdef USBCLASS_INC_HID
    /* Keyboard HID */
    {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        USB_CLASS_HID,                /* Class code                       */
        USB_SUBCLASS_HID_BOOT,        /* Sub-Class code                   */
        USB_PROTOCOL_HID_KEYBOARD,    /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_hid_event            /* Application call back function   */
    },
    /* Mouse HID */
    {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        USB_CLASS_HID,                /* Class code                       */
        USB_SUBCLASS_HID_BOOT,        /* Sub-Class code                   */
        USB_PROTOCOL_HID_MOUSE,       /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_hid_event            /* Application call back function   */
    },
#endif
#ifdef USBCLASS_INC_PHDC
    {
        {0x00, 0x00},                 /* Vendor ID per USB-IF           */
        {0x00, 0x00},                 /* Product ID per manufacturer    */
        USB_CLASS_PHDC,               /* Class code (PHDC)              */
        0,                            /* Sub-Class code (PHDC does not assign a subclass)   */
        0,                            /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_phdc_manager_event   /* Application call back function   */
    },
#endif
    {
        {0x00, 0x00},                 /* Vendor ID per USB-IF             */
        {0x00, 0x00},                 /* Product ID per manufacturer      */
        0,                            /* Class code                       */
        0,                            /* Sub-Class code                   */
        0,                            /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_event                /* Application call back function   */
    }
};

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usbh_init
* Returned Value : None
* Comments       :
*     Initializes USB host
*END*--------------------------------------------------------------------*/

void usbh_init()
{
    USB_STATUS status = USB_OK;
   
    if (MQX_OK != _usb_host_driver_install(USBCFG_DEFAULT_HOST_CONTROLLER)) {
        printf("\n Driver installation failed");
        _task_block();
    }

    status = _usb_host_init(HOST_CONTROLLER_NUMBER, &host_handle);

    if (status != USB_OK) 
    {
        printf("USB Host Initialization failed. STATUS: %x\n", status);
        _int_enable();
        _task_block();
    }

    /*
    ** Since we are going to act as the host driver, register the driver
    ** information for wanted class/subclass/protocols
    */
    status = _usb_host_driver_info_register(host_handle, DriverInfoTable);
    if (status != USB_OK) 
    {
        printf("Driver Registration failed. STATUS: %x\n", status);
        _int_enable();
        _task_block();
    }

    _int_enable();

    printf("\nMQX USB\nWaiting for USB device to be attached...\n");
    fflush(stdout);

    /* If a device will be attached, handler in usbh_<class>_handler.c file
    ** will catch that event
    */
}   

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_event
* Returned Value : None
* Comments       :
*     Called when a device is attached / detached, but not recognized by
*     any class driver.
*END*--------------------------------------------------------------------*/

void usb_host_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint_32                          event_code
   )
{
    switch (event_code) {
        case USB_CONFIG_EVENT:
        case USB_ATTACH_EVENT:
            printf("----- Unknown device Attach Event -----\n");
            fflush(stdout);
            break;
        case USB_DETACH_EVENT:
            printf("----- Unknown device Detach Event -----\n");
            fflush(stdout);
            break;
        default:
            break;
    }
}
