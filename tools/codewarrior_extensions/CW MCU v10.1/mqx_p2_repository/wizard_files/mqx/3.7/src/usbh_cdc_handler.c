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
#include <usb_host_cdc.h>

typedef struct acm_device_struct {
   CLASS_CALL_STRUCT                CLASS_INTF; /* Class-specific info */
   /* TODO: add your own device-related items */
} ACM_DEVICE_STRUCT,  _PTR_ ACM_DEVICE_STRUCT_PTR;

typedef struct data_device_struct {
   CLASS_CALL_STRUCT                CLASS_INTF; /* Class-specific info */
   /* TODO: add your own device-related items */
} DATA_DEVICE_STRUCT,  _PTR_ DATA_DEVICE_STRUCT_PTR;

char_ptr device_name = "tty0:";
static USB_CDC_UART_CODING uart_coding;

void usb_host_cdc_acm_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);
void usb_host_cdc_data_event(_usb_device_instance_handle, _usb_interface_descriptor_handle intf_handle, uint_32);

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_cdc_acm_event
* Returned Value : None
* Comments       :
*     Called when acm interface has been attached, detached, etc.
*END*--------------------------------------------------------------------*/

void usb_host_cdc_acm_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint_32                          event_code
   )
{ /* Body */
   INTERFACE_DESCRIPTOR_PTR   intf_ptr =
      (INTERFACE_DESCRIPTOR_PTR)intf_handle;
   ACM_DEVICE_STRUCT_PTR      acm_device;


   fflush(stdout);
   switch (event_code) {
      case USB_CONFIG_EVENT:
         /* Drop through into attach, same processing */
      case USB_ATTACH_EVENT: {
         USB_CDC_DESC_ACM_PTR    acm_desc = NULL;
         USB_CDC_DESC_CM_PTR     cm_desc = NULL;
         USB_CDC_DESC_HEADER_PTR header_desc = NULL;
         USB_CDC_DESC_UNION_PTR  union_desc = NULL;
         _mqx_int                external_data = 0;

         /* finds all the descriptors in the configuration */
         if (USB_OK != usb_class_cdc_get_acm_descriptors(dev_handle,
             intf_handle,
             &acm_desc,
             &cm_desc,
             &header_desc,
             &union_desc))
            break;
         /* we can allocate new acm device */
         if (NULL == (acm_device = USB_mem_alloc_zero(sizeof(ACM_DEVICE_STRUCT))))
            break; //return USBERR_INIT_FAILED;
         /* initialize new interface members and select this interface */
         if (USB_OK != _usb_hostdev_select_interface(dev_handle,
            intf_handle, (pointer)&acm_device->CLASS_INTF))
         {
            USB_mem_free(acm_device);
            break;
         }
         /* set all info got from descriptors to the class interface struct */
         usb_class_cdc_set_acm_descriptors((pointer)&acm_device->CLASS_INTF,
            acm_desc, cm_desc, header_desc, union_desc);

         /* link all already registered data interfaces to this ACM control, if needed */
         if (USB_OK != usb_class_cdc_bind_data_interfaces(dev_handle, (pointer)&acm_device->CLASS_INTF)) {
            USB_mem_free(acm_device);
            break;
         }
         
         printf("----- CDC control interface attach Event -----\n");
         fflush(stdout);
         printf("State = attached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);

         break;
      }
      case USB_INTF_EVENT: 
      {
         CLASS_CALL_STRUCT_PTR   acm_parser;
         USB_STATUS              status;
         
         if (NULL == (acm_parser = usb_class_cdc_get_ctrl_interface(intf_handle)))
             break;
         status = usb_class_cdc_init_ipipe(acm_parser);
         if ((status != USB_OK) && (status != USBERR_OPEN_PIPE_FAILED))
             break;
         
         printf("----- CDC control interface selected -----\n");

         break;
      }
         
      case USB_DETACH_EVENT:
      {
         CLASS_CALL_STRUCT_PTR acm_parser;
         USB_ACM_CLASS_INTF_STRUCT_PTR if_ptr;
         
         if (NULL == (acm_parser = usb_class_cdc_get_ctrl_interface(intf_handle)))
             break;
         if_ptr = (USB_ACM_CLASS_INTF_STRUCT_PTR) acm_parser->class_intf_handle;
         
         _lwevent_set(&if_ptr->acm_event, USB_ACM_DETACH); /* mark we are not using input pipe */

         /* Allow tasks waiting for acm to be finished...
         ** This does have sense only if this task will not be active
         ** or scheduler switches to another task.
         ** For simplification, we dont use any semaphore to indicate that
         ** all tasks have finished its job with device. Instead, we have just
         ** informed them that device is detached and we rely on USB stack layer
         ** that it checking if the device is available returns false.
         ** The code that would synchronize tasks to be finished would look like:
         **
         **  _lwsem_wait(if_ptr->device_using_tasks);
         */

         usb_class_cdc_unbind_data_interfaces(acm_parser);

         _lwevent_destroy(&if_ptr->acm_event);
         USB_unlock();
         
         USB_mem_free(acm_parser);
         /* Use only the interface with desired protocol */
         printf("----- CDC control interface detach event -----\n");
         fflush(stdout);
         printf("State = detached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);

         break;
      }

         break;
      default:
         printf("CDC device: unknown control event\n");
         fflush(stdout);
         break;
   } /* EndSwitch */
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_cdc_data_event
* Returned Value : None
* Comments       :
*     Called when data interface has been attached, detached, etc.
*END*--------------------------------------------------------------------*/

void usb_host_cdc_data_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint_32                          event_code
   )
{ /* Body */
   INTERFACE_DESCRIPTOR_PTR   intf_ptr =
      (INTERFACE_DESCRIPTOR_PTR)intf_handle;
   DATA_DEVICE_STRUCT_PTR     data_device;


   fflush(stdout);
   switch (event_code) {
      case USB_CONFIG_EVENT:
         /* Drop through into attach, same processing */
      case USB_ATTACH_EVENT: {
         /* This data interface could be controlled by some control interface,
          * which could be already initialized (or not). We have to find 
          * that interface. Then we need to bind this interface with
          * found control interface. */
         INTERFACE_DESCRIPTOR_PTR   if_desc;
         
         if (USB_OK != usb_class_cdc_get_ctrl_descriptor(dev_handle,
            intf_handle,
            &if_desc))
            break;
         /* interface descriptor found, so we can allocate new data device */
         if (NULL == (data_device = USB_mem_alloc_zero(sizeof(DATA_DEVICE_STRUCT))))
            break;
         /* initializes interface members and selects it */
         if (USB_OK != _usb_hostdev_select_interface(dev_handle,
            intf_handle, (pointer)&data_device->CLASS_INTF))
         {
            USB_mem_free(data_device);
            break;
         }
         /* binds this data interface with its control interface, if possible */
         if (USB_OK != usb_class_cdc_bind_acm_interface((pointer)&data_device->CLASS_INTF,
            if_desc))
         {
            USB_mem_free(data_device);
            break;
         }
         
         printf("----- CDC data interface attach event -----\n");
         fflush(stdout);
         printf("State = attached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);

         break;
      }   
      case USB_INTF_EVENT: 
      {
         CLASS_CALL_STRUCT_PTR data_parser;
         
         fflush(stdout);
         
         if (NULL == (data_parser = usb_class_cdc_get_data_interface(intf_handle)))
             break;
         if (USB_OK != usb_class_cdc_get_acm_line_coding(data_parser, &uart_coding))
            break;
         if (USB_OK == usb_class_cdc_install_driver(data_parser, device_name)) 
         {
             if (((USB_DATA_CLASS_INTF_STRUCT_PTR) (data_parser->class_intf_handle))->BOUND_CONTROL_INTERFACE != NULL) {

                 /* TODO: notify application that new device was registered */

             }
             printf("----- Device installed -----\n");
         }
         printf("----- CDC data interface selected -----\n");

         break;
      }
         
      case USB_DETACH_EVENT: 
      {
         CLASS_CALL_STRUCT_PTR data_parser;
         USB_DATA_CLASS_INTF_STRUCT_PTR if_ptr;
         
         if (NULL == (data_parser = usb_class_cdc_get_data_interface(intf_handle)))
             break;
         if_ptr = (USB_DATA_CLASS_INTF_STRUCT_PTR) data_parser->class_intf_handle;

         if (if_ptr->in_pipe != NULL)
             _lwevent_set(&if_ptr->data_event, USB_DATA_DETACH); /* mark we are not using input pipe */

         /* Allow tasks waiting for data to be finished...
         ** This does have sense only if this task will not be active
         ** or scheduler switches to another task.
         ** For simplification, we dont use any semaphore to indicate that
         ** all tasks have finished its job with device. Instead, we have just
         ** informed them that device is detached and we rely on USB stack layer
         ** that it checking if the device is available returns false.
         ** The code that would synchronize tasks to be finished would look like:
         **
         **  _lwsem_wait(if_ptr->device_using_tasks);
         */

         /* unbind data interface */
         if (USB_OK != usb_class_cdc_unbind_acm_interface(data_parser))
             break;
         if (USB_OK != usb_class_cdc_uninstall_driver(data_parser))
             break;
         
         if (if_ptr->in_pipe != NULL) {
             _lwevent_destroy(&if_ptr->data_event);
             USB_mem_free(if_ptr->RX_BUFFER);
         }
         
         /* TODO: notify application that device was unregistered */

         printf("----- CDC data interface detach Event -----\n");
         fflush(stdout);
         printf("State = detached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);

         break;
      }

      default:
         printf("CDC device: unknown data event\n");
         fflush(stdout);
         break;
   } /* EndSwitch */
} /* Endbody */

