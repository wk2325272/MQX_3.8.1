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
#include <usb.h>
#include <host_main.h>
#include <host_rcv.h>
#include <host_snd.h>
#include <usb_host_hid.h>

static struct hid_struct {
#define  USB_DEVICE_IDLE                   (0)
#define  USB_DEVICE_ATTACHED               (1)
#define  USB_DEVICE_CONFIGURED             (2)
#define  USB_DEVICE_SET_INTERFACE_STARTED  (3)
#define  USB_DEVICE_INTERFACED             (4)
#define  USB_DEVICE_SETTING_PROTOCOL       (5)
#define  USB_DEVICE_DETACHED               (6)
#define  USB_DEVICE_OTHER                  (7)
#define  USB_DEVICE_INUSE                  (8)

    uint_32                          dev_state;  /* Device state, one of above */
    _usb_device_instance_handle      dev_handle;
    _usb_interface_descriptor_handle intf_handle;
    CLASS_CALL_STRUCT                class_intf; /* Class-specific info */
} hid_device;

static HID_COMMAND                   hid_com;

static void usb_host_hid_recv_callback(_usb_pipe_handle, pointer, uchar_ptr, uint_32, USB_STATUS);
static void usb_host_hid_ctrl_callback(_usb_pipe_handle, pointer, uchar_ptr, uint_32, USB_STATUS);

void usb_host_hid_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_event
* Returned Value : None
* Comments       :
*     Called when HID device has been attached, detached, etc.
*END*--------------------------------------------------------------------*/

void usb_host_hid_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint_32                          event_code
   )
{ /* Body */
   INTERFACE_DESCRIPTOR_PTR intf_ptr = (INTERFACE_DESCRIPTOR_PTR)intf_handle;
      
   switch (event_code) {
      case USB_ATTACH_EVENT:
          printf("----- Attach Event -----\n");
          /* Drop through into attach, same processing */

      case USB_CONFIG_EVENT:
          if (hid_device.dev_state == USB_DEVICE_IDLE) {
              hid_device.dev_handle = dev_handle;
              hid_device.intf_handle = intf_handle;
              hid_device.dev_state = USB_DEVICE_ATTACHED;
          } else {
              printf("HID Device Is Already Attached\n");
              fflush(stdout);
          } /* EndIf */

          printf("State = %d", hid_device.dev_state);
          printf("  Class = %d", intf_ptr->bInterfaceClass);
          printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
          printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
          fflush(stdout);

          /* TODO: you should do your own initialization and then call
          ** _usb_hostdev_select_interface in order to initialize driver
          */

          break;
      case USB_INTF_EVENT:
          printf("----- Interfaced Event -----\n");
          hid_device.dev_state = USB_DEVICE_SETTING_PROTOCOL;

          /* TODO: you should unblock your main application, because device
          ** is now ready to be used from both (host and device) sides.
          ** Here we set protocol for mouse and then return from this event handler.
          
          hid_com->CLASS_PTR = (CLASS_CALL_STRUCT_PTR) &hid_device.class_intf;
          hid_com->CALLBACK_FN = usb_host_hid_ctrl_callback;
          hid_com->CALLBACK_PARAM = 0;
          
          status = usb_class_hid_set_protocol(&hid_com, USB_PROTOCOL_HID_MOUSE);
         
          if (status != USB_STATUS_TRANSFER_QUEUED) {
                printf("\nError in usb_class_hid_set_protocol: %x", status);
                fflush(stdout);
          }
          */

          break;
      case USB_DETACH_EVENT:
          if (hid_device.dev_state != USB_DEVICE_IDLE) {
              hid_device.dev_handle = NULL;
              hid_device.intf_handle = NULL;
              hid_device.dev_state = USB_DEVICE_DETACHED;
          } else {
              printf("HID Device Is Not Attached\n");
              fflush(stdout);
          } /* EndIf */

          printf("----- Detach Event -----\n");
          printf("State = %d", hid_device.dev_state);
          printf("  Class = %d", intf_ptr->bInterfaceClass);
          printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
          printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
          fflush(stdout);

          /* TODO: you should notify your main application that device
          ** was detached.
          */

          break;
      default:
          printf("HID Device state = %d??\n", hid_device.dev_state);
          fflush(stdout);

          break;
   } /* EndSwitch */

} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_ctrl_callback
* Returned Value : None
* Comments       :
*     Called when a command is completed
*END*--------------------------------------------------------------------*/

void usb_host_hid_ctrl_callback
   (
      /* [IN] pointer to pipe */
      _usb_pipe_handle  pipe_handle,

      /* [IN] user-defined parameter */
      pointer           user_parm,

      /* [IN] buffer address */
      uchar_ptr         buffer,

      /* [IN] length of data transferred */
      uint_32           buflen,

      /* [IN] status, hopefully USB_OK or USB_DONE */
      USB_STATUS        status
   )
{ /* Body */
   if (status == USBERR_ENDPOINT_STALLED) {
      printf("\nHID Set_Protocol Request BOOT is not supported!\n");
      fflush(stdout);
   }
   else if (status) {
      printf("\nHID Set_Protocol Request BOOT failed!: 0x%x ... END!\n", status);
      _task_block();
   } /* Endif */

    if(hid_device.dev_state == USB_DEVICE_SETTING_PROTOCOL)
        hid_device.dev_state = USB_DEVICE_INUSE;
    
    /* TODO: notify application that status has changed */

} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_hid_recv_callback
* Returned Value : None
* Comments       :
*     Called when data is received on a pipe
*END*--------------------------------------------------------------------*/

void usb_host_hid_recv_callback
   (
      /* [IN] pointer to pipe */
      _usb_pipe_handle  pipe_handle,

      /* [IN] user-defined parameter */
      pointer           user_parm,

      /* [IN] buffer address */
      uchar_ptr         buffer,

      /* [IN] length of data transferred */
      uint_32           buflen,

      /* [IN] status, hopefully USB_OK or USB_DONE */
      USB_STATUS        status
   )
{ /* Body */
   /* TODO: notify application that data are available */
}
