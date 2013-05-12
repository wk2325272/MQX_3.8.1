#ifndef __main_h_
#define __main_h_


#ifdef __USB_OTG__
#include "otgapi.h"
#include "devapi.h"
#else
#include "hostapi.h"
#endif


#define MAIN_TASK              1
#define MAX_FRAME_SIZE         1024
#define HOST_CONTROLLER_NUMBER 0


extern void Main_task(uint_32);
void usb_host_event(_usb_device_instance_handle,_usb_interface_descriptor_handle,uint_32);


#endif /* __main_h_ */

