/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
#ifndef __usb_classes_h__
#define __usb_classes_h__

#warning Please uncomment following definitions for class to be used in your application and follow TODO in the generated files.

//#define USBCLASS_INC_HID
//#define USBCLASS_INC_MASS
//#define USBCLASS_INC_CDC
//#define USBCLASS_INC_PRINTER
//#define USBCLASS_INC_HUB

#ifdef USBCLASS_INC_MASS
#include "usb_host_msd_bo.h"
#endif

#ifdef USBCLASS_INC_PRINTER
#include "usb_host_printer.h"
#endif

#ifdef USBCLASS_INC_HID
#include "usb_host_hid.h"
#endif

#ifdef USBCLASS_INC_CDC
#include "usb_host_cdc.h"
#endif

#ifdef USBCLASS_INC_AUDIO
#include "usb_host_audio.h"
#endif

#ifdef USBCLASS_INC_PHDC
#include "usb_host_phdc.h"
#endif

/* here hub is considered as device from host point of view */
#ifdef USBCLASS_INC_HUB
#include "usb_host_hub.h"
#include "usb_host_hub_sm.h"
#endif

#endif
/* EOF */
