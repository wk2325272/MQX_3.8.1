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
* $FileName: usb_twrmpc512x_device.c$
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

struct usb_dev_if_struct _bsp_usb_dev_ehci1_if = {
    &_usb_ehci_dev_callback_table,
    (pointer) &_ehci1_dev_init_param,
    NULL
};

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _bsp_usb_dev_init
* Returned Value   : MQX_OK for success, IO_ERROR for failure
* Comments         :
*    This function performs BSP-specific initialization related to USB
*
*END*----------------------------------------------------------------------*/
_mqx_int _bsp_usb_dev_init(struct usb_dev_if_struct *usb_if)
{

    if (usb_if->DEV_INIT_PARAM == &_ehci0_dev_init_param)
    {
        /* gpio usb init */
        _bsp_usb_io_init(USB_CONNECT_ULPI1);
        /* setup interrupt */
        _mpc5125_enable_interrupt(MPC5125_USB2OTG1_VECTOR);
    }
    else if (usb_if->DEV_INIT_PARAM == &_ehci1_dev_init_param)
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
