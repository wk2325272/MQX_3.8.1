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
* $FileName: usb_twrmpc512x.c$
* $Version : 3.8.3.1$
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
* Returned Value   : MQX_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_host_init(struct usb_host_if_struct *usb_if)
{
    if (usb_if->HOST_INIT_PARAM == &_ehci0_host_init_param)
    {
        /* gpio usb init */
        _bsp_usb_io_init(USB_CONNECT_ULPI1);
        /* setup interrupt */
        _mpc5125_enable_interrupt(MPC5125_USB2OTG1_VECTOR);
    }
    else if (usb_if->HOST_INIT_PARAM == &_ehci1_host_init_param)
    {
        /* gpio usb init */
        _bsp_usb_io_init(USB_CONNECT_ULPI2);
        /* setup interrupt */
        _mpc5125_enable_interrupt(MPC5125_USB2OTG2_VECTOR);
    }
    else
    {
        return  IO_ERROR ;
    }
    return MQX_OK;
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of USB OTG module
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_base(uint_8 dev_num)
{

   switch(dev_num ){
    case USB_CONNECT_ULPI1:
        return (pointer)(&((VMPC5125_STRUCT_PTR)MPC5125_GET_IMMR())->USB_ULPI1);
    case USB_CONNECT_ULPI2:
           return (pointer)(&((VMPC5125_STRUCT_PTR)MPC5125_GET_IMMR())->USB_ULPI2);
    default:
       return NULL;
   }  /* End Switch */

}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_capability_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Capability Registers
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_capability_register_base(uint_8 dev_num)
{


   switch(dev_num ){
       case USB_CONNECT_ULPI1:
        return (pointer)((uint_32)&(((VMPC5125_STRUCT_PTR)MPC5125_GET_IMMR())->USB_ULPI1) + 0x100);
      case USB_CONNECT_ULPI2:
        return (pointer)((uint_32)&(((VMPC5125_STRUCT_PTR)MPC5125_GET_IMMR())->USB_ULPI2) + 0x100);

    default :
        return NULL ;
   } /* End Switch */

}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_timer_register_base
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the VUSBHS Timer Registers
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_timer_register_base(uint_8 dev_num)
{

   switch(dev_num ){
        case USB_CONNECT_ULPI1:
        return (pointer)((uint_32)&(((VMPC5125_STRUCT_PTR)MPC5125_GET_IMMR())->USB_ULPI1) + 0x080);
     case USB_CONNECT_ULPI2:
       return (pointer)((uint_32)&(((VMPC5125_STRUCT_PTR)MPC5125_GET_IMMR())->USB_ULPI2) + 0x080);
     default :
        return NULL ;
   } /* End Switch */

}
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_get_usb_otg_csr
* Returned Value   : Address upon success, NULL upon failure
* Comments         :
*    This function returns the address of the U0CSR register
*
*END*----------------------------------------------------------------------*/
pointer _bsp_get_usb_otg_csr(uint_8 dev_num)
{
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
uint_8 _bsp_get_usb_vector
(
    uint_8 dev_num
)

{

   switch(dev_num ){
     case USB_CONNECT_ULPI1:
           return MPC5125_USB2OTG1_VECTOR;
     case USB_CONNECT_ULPI2:
        return MPC5125_USB2OTG2_VECTOR;
        default :
           return IO_ERROR;
   } /* End Switch */

}
