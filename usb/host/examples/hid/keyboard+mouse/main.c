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
* $FileName: main.c$
* $Version : 3.8.12.4$
* $Date    : May-22-2012$
*
* Comments:
*
*   This file is an example of device drivers for the HID class. This example
*   demonstrates the keyboard functionality. Note that a real keyboard driver also
*   needs to distinguish between intentionally repeated and unrepeated key presses.
*   This example simply demonstrates how to receive data from a USB Keyboard. 
*   Interpretation of data is upto the application and customers can add the code
*   for it.
*
*END************************************************************************/
#include <mqx.h>
#include <lwevent.h>

#include <bsp.h>

#include <usb.h>
#include <host_main.h>
#include <host_rcv.h>
#include <usb_host_hub_sm.h>
#include <usb_host_hid.h>

#include "hidkeyboard.h"
#include "hidmouse.h"

#if !MQX_HAS_TIME_SLICE
#error This application requires that MQX_HAS_TIME_SLICE is set to 1
#endif


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

/*
** Globals
*/

/* Table of driver capabilities this application wants to use */
static  USB_HOST_DRIVER_INFO DriverInfoTable[] =
{
   {
      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_HID,                /* Class code                       */
      USB_SUBCLASS_HID_BOOT,        /* Sub-Class code                   */
      USB_PROTOCOL_HID_KEYBOARD,    /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_hid_keyboard_event   /* Application call back function   */
   },
   {
      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_HID,                /* Class code                       */
      USB_SUBCLASS_HID_BOOT,        /* Sub-Class code                   */
      USB_PROTOCOL_HID_MOUSE,       /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_hid_mouse_event      /* Application call back function   */
   },
   /* USB 2.0 hub */
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
      {0x00,0x00},                  /* All-zero entry terminates        */
      {0x00,0x00},                  /* driver info list.                */
      0,
      0,
      0,
      0,
      NULL
   }
};

_usb_host_handle        host_handle;

#define MAX_SUPPORTED_USAGE_ID 40

#define MAIN_TASK          (10)
#define KEYBOARD_TASK      (11)
#define MOUSE_TASK         (12)
extern void Main_Task(uint_32 param);
extern void Keyboard_Task(uint_32 param);
extern void Mouse_Task(uint_32 param);

TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK,      Main_Task,      3000L,  9L, "Main",      MQX_AUTO_START_TASK},
   { KEYBOARD_TASK,  Keyboard_Task,  3000L,  9L, "Keyboard",  MQX_TIME_SLICE_TASK},
   { MOUSE_TASK,     Mouse_Task,     3000L,  9L, "Mouse",     MQX_TIME_SLICE_TASK},
   { 0L,             0L,             0L,     0L, 0L,          0L }
};

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : main (Main_Task if using MQX)
* Returned Value : none
* Comments       :
*     Execution starts here
*
*END*--------------------------------------------------------------------*/

void Main_Task ( uint_32 param )
{ /* Body */
   USB_STATUS           status = USB_OK;
   
   /* _usb_otg_init needs to be done with interrupts disabled */
   _int_disable();

   _int_install_unexpected_isr();
   if (MQX_OK != _usb_host_driver_install(USBCFG_DEFAULT_HOST_CONTROLLER)) {
      printf("\n Driver installation failed");
      _task_block();
   }

   /*
   ** It means that we are going to act like host, so we initialize the
   ** host stack. This call will allow USB system to allocate memory for
   ** data structures, it uses later (e.g pipes etc.).
   */
   status = _usb_host_init (USBCFG_DEFAULT_HOST_CONTROLLER, &host_handle);

   if (status != USB_OK) 
   {
      printf("\nUSB Host Initialization failed. STATUS: %x", status);
      fflush(stdout);
   }

   /*
   ** since we are going to act as the host driver, register the driver
   ** information for wanted class/subclass/protocols
   */
   status = _usb_host_driver_info_register (
                                    host_handle,
                                    DriverInfoTable
                                    );
   if (status != USB_OK) {
      printf("\nDriver Registration failed. STATUS: %x", status);
      fflush(stdout);
   }
          
   _int_enable();

   printf("\nMQX USB HID Keyboard + Mouse Demo");
   fflush(stdout);

   _task_create(0, MOUSE_TASK, (uint_32) host_handle);
   _task_create(0, KEYBOARD_TASK, (uint_32) host_handle);

   /* The main task has done its job, so exit */
} /* Endbody */

/* EOF */

