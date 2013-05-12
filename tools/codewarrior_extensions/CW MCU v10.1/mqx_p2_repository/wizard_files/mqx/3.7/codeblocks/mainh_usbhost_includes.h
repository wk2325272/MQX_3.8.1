#include <mqx_host.h>
#include "usb_classes.h"

#ifdef USBCLASS_INC_HUB
#include <usb_host_hub_sm.h>
#endif

#ifdef USBCLASS_INC_CDC
#include <usb_host_cdc.h>
#endif

#ifdef USBCLASS_INC_MASS
#include <usb_host_msd_bo.h>
#include <usb_host_msd_ufi.h>
#include <usbmfs.h>
#endif

#ifdef USBCLASS_INC_HID
#include <usb_host_hid.h>
#endif

#ifdef USBCLASS_INC_PHDC
#include <usb_host_phdc.h>
#endif

#ifdef USBCLASS_INC_PRINTER
#include <usb_host_printer.h>
#endif
