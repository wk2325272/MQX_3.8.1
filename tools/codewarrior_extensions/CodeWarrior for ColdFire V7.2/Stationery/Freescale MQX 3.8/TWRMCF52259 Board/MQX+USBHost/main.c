/****************************************************************************
* 
*   This file contains MQX+USBHost stationery code.
*
****************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <mqx_host.h>
#include <usb_host_hub_sm.h>
#include "main.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


/************************************************************************************
Table of driver capabilities this application wants to use. See Host API document for
details on How to define a driver info table.
************************************************************************************/

static USB_HOST_DRIVER_INFO DriverInfoTable[] =
{
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
    {
        {0x00,0x00},                  /* Vendor ID per USB-IF             */
        {0x00,0x00},                  /* Product ID per manufacturer      */
        0,                            /* Class code                       */
        0,                            /* Sub-Class code                   */
        0,                            /* Protocol                         */
        0,                            /* Reserved                         */
        usb_host_event                /* Application call back function   */
    }
};

static _usb_host_handle host_handle;  /* Global handle for calling host   */


TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
    { MAIN_TASK, Main_task,   3000,  9,   "main", MQX_AUTO_START_TASK},
    { 0,         0,           0,     0,   0,      0                  }
};


/*FUNCTION*-----------------------------------------------------
* 
* Task Name    : usb_host_event
* Comments     :
*    This callback function is called on USBHost events.
*
*END*-----------------------------------------------------*/

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
    INTERFACE_DESCRIPTOR_PTR intf_ptr = (INTERFACE_DESCRIPTOR_PTR)intf_handle;
      
    switch (event_code) 
    {
        case USB_CONFIG_EVENT:
            /* Drop through into attach, same processing */
        case USB_ATTACH_EVENT:
            printf("----- Attach Event -----\n");
            printf("  Class = %d", intf_ptr->bInterfaceClass);
            printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
            printf("  Protocol = %d\n\n", intf_ptr->bInterfaceProtocol);
            fflush(stdout);
            break;
        case USB_INTF_EVENT:
            break;
        case USB_DETACH_EVENT:
            /* Use only the interface with desired protocol */
            printf("----- Detach Event -----\n");
            printf("  Class = %d", intf_ptr->bInterfaceClass);
            printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
            printf("  Protocol = %d\n\n", intf_ptr->bInterfaceProtocol);
            fflush(stdout);
            break;
        default:
            break;
    }
}


/*TASK*-----------------------------------------------------------------
*
* Function Name  : Main_task
* Comments       :
*    This task initializes USBHost and waits for USB events.
*
*END------------------------------------------------------------------*/

void Main_task(uint_32 initial_data)
{
    USB_STATUS status = USB_OK;
   
    _int_disable();

    _int_install_unexpected_isr();
    
    _usb_host_driver_install(0, (pointer)&_bsp_usb_host_callback_table);

    status = _usb_host_init
        (HOST_CONTROLLER_NUMBER,   /* Use value in header file */
        MAX_FRAME_SIZE,            /* Frame size per USB spec  */
        &host_handle);             /* Returned pointer */
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
   
    while (1) 
       {};
}

