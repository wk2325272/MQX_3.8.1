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
* $FileName: usb_mcf51mm.c$
* $Version : 3.8.5.6$
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

struct usb_host_if_struct _bsp_usb_host_khci0_if = {
    &_usb_khci_host_callback_table,
    (pointer) &_khci0_host_init_param,
    NULL
};

_mqx_int _bsp_usb_init(pointer param);

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_init(pointer param)
{
    VMCF51MM_STRUCT_PTR reg_ptr = (VMCF51MM_STRUCT_PTR) BSP_IPSBAR;

    /* Enable USB clocking */
    reg_ptr->SIM.SCGC2 |= MCF51XX_SCGC2_USB_MASK;
    /* Use little endian (because low level driver runs as low endian) */
    reg_ptr->SIM.SOPT2 &= ~MCF51XX_SOPT2_USB_BIGEND_MASK;
    /* Reset USB peripheral */
    reg_ptr->USBOTG.USBTRC0 |= USB_USBTRC0_USBRESET_MASK;
    /* Wait while resetting */
    while (reg_ptr->USBOTG.USBTRC0 & USB_USBTRC0_USBRESET_MASK)
    {}
    /* Enable USB regulator and internal pull-up resitors */
    reg_ptr->USBOTG.USBTRC0 |= USB_USBTRC0_USBVREN_MASK | USB_USBTRC0_USBPU_MASK;

    _bsp_usb_io_init();
    
    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_host_init
* Returned Value   : 0 for success, -1 for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB host
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_host_init(struct usb_host_if_struct *usb_if)
{
    return _bsp_usb_init(NULL);
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_capability_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Capability Registers
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_base(uint_8 dev_num)
{
   if (dev_num != 0)
   {
      return NULL;
   }
   
   return (pointer)(&((VMCF51MM_STRUCT_PTR)BSP_IPSBAR)->USBOTG);
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_vector
* Returned Value   : Vector Number upon success, 0 upon failure
* Comments         :
*    This function returns the vector number of the VUSBHS Host Module
*
*END*----------------------------------------------------------------------*/
uint_8 _bsp_get_usb_vector
(
   uint_8 dev_num
)
{
   if (dev_num != 0)
   {
      return 0;
   }
   
   return MCF51MM_INT_Vusb;
}

#if MCF51MM_REV2_USB_PATCH
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_shadow_vector
* Returned Value   : Vector Number upon success, 0 upon failure
* Comments         :
*    This function returns the software interrupt level 6 vector used to
*    defer NMI call from USB peripheral (note: USB NMI is only on the
*    first MCF51MM Rev. <= 2.0 parts a
*
*END*----------------------------------------------------------------------*/
uint_8 _bsp_get_usb_shadow_vector
(
   uint_8 dev_num
)
{
   if (dev_num != 0)
   {
      return 0;
   }
   
   return MCF51MM_INT_VL6swi;
}
#endif

/* EOF */
