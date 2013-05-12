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
* $FileName: usb_mcf5441.c$
* $Version : 3.8.7.4$
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

struct usb_host_if_struct _bsp_usb_host_ehci0_if = {
    &_usb_ehci_host_callback_table,
    (pointer) &_ehci0_host_init_param,
    NULL
};

struct usb_host_if_struct _bsp_usb_host_ehci1_if = {
    &_usb_ehci_host_callback_table,
    (pointer) &_ehci1_host_init_param,
    NULL
};

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_host_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_host_init(struct usb_host_if_struct *usb_if)
{
    VMCF5441_STRUCT_PTR reg_ptr = _PSP_GET_IPSBAR();
    VMCF5XXX_USBOTG_STRUCT_PTR usb_ptr;
    uint_32 status;

    if (usb_if->HOST_INIT_PARAM == &_ehci0_host_init_param) {
        usb_ptr = &((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG;

        /* disable second FEC clock */
        reg_ptr->SCM_PMM.PMM.PPMSR0 = 54;
        
        /* Reset USB PHY chip as from POR it is in unknown state */
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

        _bsp_usb_io_init(USBCFG_USBOTG_CONNECTOR, USB_MODE_HOST);

        /* Enable external peripheral clock for USB_OTG and USB_HOST modules */
        reg_ptr->CCM.MISCCR &= ~0x0001;

        /* setup interrupt */
        status = _mcf5441_int_init(MCF5441_INT_USB_OTG, 3, TRUE);

    }
    else if (usb_if->HOST_INIT_PARAM == &_ehci1_host_init_param) {
        usb_ptr = &((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST;

        /* Enable peripheral clock for USB_HOST module */    
        reg_ptr->SCM_PMM.PMM.PPMCR0 = 45;

        if (USBCFG_USBHOST_CONNECTOR == USB_CONNECTOR_ULPI)
        {
            /* set that USB HOST is using ULPI */
            reg_ptr->CCM.MISCCR2 |= 0x0001;
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_ULPI;
        }
        else
        {
            /* set that USB OTG is using ULPI */
            reg_ptr->CCM.MISCCR2 &= ~0x0001;
            usb_ptr->PORTSC1 &= ~MCF5XXX_USBOTG_PORTSC1_PTS;
            usb_ptr->PORTSC1 |= MCF5XXX_USBOTG_PORTSC1_PTS_OCT;
        }

        _bsp_usb_io_init(USBCFG_USBHOST_CONNECTOR, USB_MODE_HOST);
    
        /* Enable external peripheral clock for USB_OTG and USB_HOST module */    
        reg_ptr->CCM.MISCCR &= ~0x0001;

        /* setup interrupt */
        status = _mcf5441_int_init(MCF5441_INT_USB_HOST, 2, TRUE);

    }
    else
        status = IO_ERROR;
    
    return status;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of USB OTG module
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_base(uint_8 dev_num);
pointer _bsp_get_usb_base(uint_8 dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (pointer)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG);
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (pointer)(&((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST);
    }

    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_capability_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Capability Registers
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_capability_register_base(uint_8 dev_num);
pointer _bsp_get_usb_capability_register_base(uint_8 dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (pointer)((uint_32)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG) + 0x100);
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (pointer)((uint_32)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST) + 0x100);
    }

    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_timer_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Timer Registers
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_timer_register_base(uint_8 dev_num);
pointer _bsp_get_usb_timer_register_base(uint_8 dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (pointer)((uint_32)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBOTG) + 0x080);
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (pointer)((uint_32)&(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->USBHOST) + 0x080);
    }

    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_otg_csr
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the U0CSR register
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_otg_csr(uint_8 dev_num);
pointer _bsp_get_usb_otg_csr(uint_8 dev_num)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return (pointer) &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->CCM).UOCSR;
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return (pointer) &(((VMCF5441_STRUCT_PTR)_PSP_GET_IPSBAR())->CCM).UHCSR;
    }
    
    return NULL;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_vector
* Returned Value   : Vector Number upon success, 0 upon failure
* Comments         :
*    This function returns the vector number of the VUSBHS Host Module
*
*END*----------------------------------------------------------------------*/
uint_8 _bsp_get_usb_vector(uint_8 dev_num);
uint_8 _bsp_get_usb_vector
(
    uint_8 dev_num
)
{
    if (dev_num == USBCFG_CONTROLLER_OTG) {
        return MCF5441_INT_USB_OTG;
    }
    else if (dev_num == USBCFG_CONTROLLER_HOST) {
        return MCF5441_INT_USB_HOST;
    }

    return 0;
}
