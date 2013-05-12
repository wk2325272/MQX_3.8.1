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
* $FileName: usb_mpc8308.c$
* $Version : 3.8.B.3$
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


struct usb_host_if_struct _bsp_usb_host_ehci0_if = {
    &_usb_ehci_host_callback_table,
    (pointer) &_ehci0_host_init_param,
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
   MPC83XX_USBOTG_STRUCT_PTR  usb_ptr;
   MPC83xx_CLKMOD_STRUCT_PTR  clk_ptr;
   MPC83xx_SYSCON_STRUCT_PTR  sys_ptr;
   uint_32                    immr;
   uint_32                    i, portsc1;
   uint_32                    status = MQX_OK;

   _PSP_GET_IMMR(immr);

   _bsp_usb_io_init(USB_CONNECTOR_ULPI);

   usb_ptr = (MPC83XX_USBOTG_STRUCT_PTR)(immr + MPC83xx_USB_DR_BASE);
   clk_ptr = (MPC83xx_CLKMOD_STRUCT_PTR)(immr + MPC83xx_CLOCK_MODULE_BASE);
   sys_ptr = (MPC83xx_SYSCON_STRUCT_PTR)(immr + MPC83xx_SYSCON_BASE);

   // Enable se0nak fix
   sys_ptr->SPCR |= 0x00400000;
   
   // Set 1:1 USB Clock
   clk_ptr->SCCR &= ~MPC83xx_CLKMOD_SCCR_USBDRCM_MASK;
   _PSP_SYNC();
   clk_ptr->SCCR |= MPC83xx_CLKMOD_SCCR_USBDRCM(1);
   _PSP_SYNC();

   // We need a delay here, otherwise a write to the usb register space causes an exception
   // probably because the USB clock has just been turned on and something has to settle out
   _time_delay(1);
   
   usb_ptr->CONTROL = MPC83XX_USBOTG_USB_OTG_CONTROL_PHY_CLK_SEL;
   while ((usb_ptr->CONTROL & MPC83XX_USBOTG_USB_OTG_CONTROL_PHY_CLK_VLD) ==0){
      _time_delay(1);
   }
   
   usb_ptr->PORTSC1 |= MPC83XX_USBOTG_PORTSC1_PTS_ULPI | MPC83XX_USBOTG_PORTSC1_PP | MPC83XX_USBOTG_PORTSC1_PR;
   _PSP_SYNC();
   
   i=0;
   while ((usb_ptr->PORTSC1 & MPC83XX_USBOTG_PORTSC1_PE)==0) {
      i++;
      if (i>1000) {
         return IO_ERROR;
      }
   }
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
    uint_32 base;
    if (dev_num != 0) {
        return NULL;
    }
   _PSP_GET_IMMR(base);
    
    return (pointer)(base + MPC83xx_USB_DR_BASE);
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
    uint_32 base;
    if (dev_num != 0) {
        return NULL;
    }
    _PSP_GET_IMMR(base);

    return (pointer)((base + MPC83xx_USB_DR_BASE ) + 0x100);
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
    if (dev_num != 0) {
        return 0;
    }
   
    return MPC83xx_USB_DR_VECTOR;
}



void usb_debug_dump_mem(char * tag,  uint_32 addr, uint_32 size) 
{
   uint_32 val[4], i,j;
  

   printf("---%s---\n",tag);
   for (i=0;i<size; i+=16) {
      for (j=0;j<4;j++) {
         val[j] =  *((uint_32_ptr)(addr + i + j*4));
      }
      printf("%08x: %08x %08x %08x %08x\n",  addr + i,
         val[0],val[1],val[2],val[3]);
   } 
}

void usb_debug_dump(void) 
{
return;
   usb_debug_dump_mem("System Configuration", 0xe0000000, 0x200);
   usb_debug_dump_mem("Clock Module",         0xe0000a00, 0x40);
   usb_debug_dump_mem("GPIO Module",          0xe0000c00, 0x40);

   usb_debug_dump_mem("USB Module",           0xe0023000, 0x200);
   usb_debug_dump_mem("USB Module",           0xe0023400, 0x10);
   printf("CONTROL = %08x\n",*((uint_32_ptr)0xe0023500));
   
}

