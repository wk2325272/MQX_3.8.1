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
* $FileName: usb_mpc8308_device.c$
* $Version : 3.8.0.3$
* $Date    : Jun-13-2012$
*
* Comments:
*
*   This file contains board-specific USB initialization functions.
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "usb_bsp.h"

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
   MPC83XX_USBOTG_STRUCT_PTR  usb_ptr;
   MPC83xx_CLKMOD_STRUCT_PTR  clk_ptr;
   MPC83xx_SYSCON_STRUCT_PTR  sys_ptr;
   uint_32                    immr;
   uint_32 status = MQX_OK;

   _PSP_GET_IMMR(immr);

   usb_ptr = (MPC83XX_USBOTG_STRUCT_PTR)(immr + MPC83xx_USB_DR_BASE);
   clk_ptr = (MPC83xx_CLKMOD_STRUCT_PTR)(immr + MPC83xx_CLOCK_MODULE_BASE);
   sys_ptr = (MPC83xx_SYSCON_STRUCT_PTR)(immr + MPC83xx_SYSCON_BASE);


   _bsp_usb_io_init(USB_CONNECTOR_ULPI);

   // Set 1:1 USB Clock
   clk_ptr->SCCR &= ~MPC83xx_CLKMOD_SCCR_USBDRCM_MASK;
   _PSP_SYNC();
   clk_ptr->SCCR |= MPC83xx_CLKMOD_SCCR_USBDRCM(1);
   _PSP_SYNC();

   // We need a delay here, otherwise a write to the usb register space causes an exception
   // probably because the USB clock has just been turned on and something has to settle out
   _time_delay(1);
   
   usb_ptr->CONTROL = MPC83XX_USBOTG_USB_OTG_CONTROL_PHY_CLK_SEL;
   while ((usb_ptr->CONTROL & MPC83XX_USBOTG_USB_OTG_CONTROL_PHY_CLK_VLD) == 0){
      _time_delay(1);
   }

   usb_ptr->PORTSC1 |= MPC83XX_USBOTG_PORTSC1_PTS_ULPI;
   usb_ptr->CONTROL = MPC83XX_USBOTG_USB_OTG_CONTROL_PHY_CLK_SEL | MPC83XX_USBOTG_USB_OTG_CONTROL_USB_EN;

   _PSP_SYNC();

   usb_ptr->SNOOP1 = 0x00000000 | 0x1e;
   usb_ptr->SNOOP2 = 0x80000000 | 0x1e;
   usb_ptr->AGE_CNT_THRESH = 0x00000040;
   usb_ptr->PRI_CTRL =  0x0000000c;
   usb_ptr->SI_CTRL  =  0x00000001;
   
   // setup interrupt
   _mpc83xx_enable_interrupt(MPC83xx_USB_DR_VECTOR);

   return MQX_OK;
}
