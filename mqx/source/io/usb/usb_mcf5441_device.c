/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: usb_mcf5441_device.c$
* $Version : 3.8.0.2$
* $Date    : May-31-2012$
*
* Comments:
*
*   This file contains board-specific USB initialization functions.
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"

struct usb_dev_if_struct _bsp_usb_dev_ehci0_if = {
    &_usb_ehci_dev_callback_table,
    (pointer) &_ehci0_dev_init_param,
    NULL
};


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_dev_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_dev_init(struct usb_dev_if_struct *usb_if)
{
    VMCF5441_STRUCT_PTR reg_ptr = _PSP_GET_IPSBAR();
    VMCF5XXX_USBOTG_STRUCT_PTR usb_ptr;
    uint_32 status;

    /* Burst Configuration Register (BCR) - System Control Module (SCM) 
    The BCR register enables or disables the eSDHC, USB host, and USB On-the-Go modules for bursting 
    to/from the crossbar switch slave modules. There is an enable field for the slaves, and either 
    direction (read and write) is supported via the GBR and GBW bits.*/
    reg_ptr->SCM_PMM.SCM.BCR |= 0x3ff;

    if (usb_if->DEV_INIT_PARAM == &_ehci0_dev_init_param) {
        usb_ptr = &((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG;

        /* disable second FEC clock */
        reg_ptr->SCM_PMM.PMM.PPMSR0 = 54;

        * ((volatile uint_8 *) 0xec090000) = 0x40;
        _time_delay(1);
        * ((volatile uint_8 *) 0xec090000) = 0x00;

        /* Enable peripheral clock for USB_OTG module */    
        reg_ptr->SCM_PMM.PMM.PPMCR0 = 44;

        if (USBCFG_USBOTG_CONNECTOR == USB_CONNECTOR_ULPI) 
        {
            /* set that USB OTG is using ULPI */
            reg_ptr->CCM.MISCCR2 &= ~0x0001;

            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_ULPI | MCF5XXX_USBOTG_PORTSC1_PFSC;

            /* FEC0 as RMII mode */
            reg_ptr->MACNET0.RCR = 0x104;
        }
        else
        {
            /* set that USB HOST is using ULPI */
            reg_ptr->CCM.MISCCR2 |= 0x0001;
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_OCT;
        }
        
        _bsp_usb_io_init(USBCFG_USBOTG_CONNECTOR, USB_MODE_DEVICE);

        /* Enable external peripheral clock for USB_OTG and USB_HOST modules */
        reg_ptr->CCM.MISCCR &= ~0x0001;

        /* Set interrupt threshold control = 0 */
        reg_ptr->USBOTG.USBCMD &= 0xFF00FFFF;           

        /* setup interrupt */
        status = _mcf5441_int_init(MCF5441_INT_USB_OTG, 2, TRUE);
    
    } 
    else {

        status = IO_ERROR; /* cannot configure USB-HOST IP for device */

    }

    if (status != MQX_OK) {
        return status;
    }

    return MQX_OK;
}
