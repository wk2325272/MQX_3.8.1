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
#include <mqx_host.h>
#include <usb_host_msd_bo.h>
#include <usb_host_msd_ufi.h>
#include <usbmfs.h>

static struct msd_struct {
#define  USB_DEVICE_IDLE                   (0)
#define  USB_DEVICE_ATTACHED               (1)
#define  USB_DEVICE_CONFIGURED             (2)
#define  USB_DEVICE_SET_INTERFACE_STARTED  (3)
#define  USB_DEVICE_INTERFACED             (4)
#define  USB_DEVICE_DETACHED               (5)
#define  USB_DEVICE_OTHER                  (6)
    uint_32                          dev_state;  /* Device state, one of above */
    _usb_device_instance_handle      dev_handle;
    _usb_interface_descriptor_handle intf_handle;
    CLASS_CALL_STRUCT                class_intf; /* Class-specific info */
} msd_device;

static void usb_host_mass_bulk_callback(USB_STATUS, pointer, pointer, uint_32);
static void usb_host_mass_ctrl_callback(_usb_pipe_handle, pointer, uchar_ptr, uint_32, uint_32);

void usb_host_msd_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_msd_event
* Returned Value : None
* Comments       :
*     Called when MSD device has been attached, detached, etc.
*END*--------------------------------------------------------------------*/

void usb_host_msd_event
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
          if (msd_device.dev_state == USB_DEVICE_IDLE) {
              msd_device.dev_handle = dev_handle;
              msd_device.intf_handle = intf_handle;
              msd_device.dev_state = USB_DEVICE_ATTACHED;
          } else {
              printf("Mass Storage Device Is Already Attached\n");
              fflush(stdout);
          } /* EndIf */

          printf("State = %d", msd_device.dev_state);
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
          msd_device.dev_state = USB_DEVICE_INTERFACED;

          /* TODO: you should unblock your main application, because device
          ** is now ready to be used from both (host and device) sides.
          ** Open pipes to start communication etc. In order to use mass storage
          ** with MFS, you must include also MFS component. Use demo application
          ** from MQX installation to see how to handle with MFS devices.
          */

          break;
      case USB_DETACH_EVENT:
          if (msd_device.dev_state != USB_DEVICE_IDLE) {
              msd_device.dev_handle = NULL;
              msd_device.intf_handle = NULL;
              msd_device.dev_state = USB_DEVICE_DETACHED;
          } else {
              printf("Mass Storage Device Is Not Attached\n");
              fflush(stdout);
          } /* EndIf */

          printf("----- Detach Event -----\n");
          printf("State = %d", msd_device.dev_state);
          printf("  Class = %d", intf_ptr->bInterfaceClass);
          printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
          printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
          fflush(stdout);

          /* TODO: you should notify your main application that device
          ** was detached.
          */

          break;
      default:
          printf("Mass Storage Device state = %d??\n", msd_device.dev_state);
          fflush(stdout);

          break;
   } /* EndSwitch */

} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_mass_ctrl_callback
* Returned Value : None
* Comments       :
*     Called on completion of a control-pipe transaction.
*
*END*--------------------------------------------------------------------*/

static void usb_host_mass_ctrl_callback
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
      uint_32           status
   )
{ /* Body */

    /* TODO: notify application that the control was done */

} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_mass_bulk_callback
* Returned Value : None
* Comments       :
*     Called on completion of a bulk transaction.
*
*END*--------------------------------------------------------------------*/

void usb_host_mass_bulk_callback
   (
      /* [IN] Status of this command */
      USB_STATUS status,

      /* [IN] pointer to USB_MASS_BULK_ONLY_REQUEST_STRUCT*/
      pointer p1,

      /* [IN] pointer to the command object*/
      pointer p2,

      /* [IN] Length of data transmitted */
      uint_32 buffer_length
   )
{ /* Body */

    /* TODO: read data from buffer, notify application about data retrieval.
    ** If you use MFS, then the callback is already registered with _io_usb_mfs_install
    ** and this function should not be used.
    */

} /* Endbody */
