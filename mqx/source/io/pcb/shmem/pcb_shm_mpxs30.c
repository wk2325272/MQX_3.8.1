/**HEADER********************************************************************
*
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: pcb_shm_mpxs30.c$
* $Version : 3.8.1.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains the MPXS30 core specific code for the
*   shared memory driver.
*
*END************************************************************************/

#include "mqx_inc.h"
#include "fio.h"
#include "io.h"
#include "io_prv.h"
#include "io_pcb.h"
#include "iopcbprv.h"
#include "pcb_shm.h"
#include "pcbshmv.h"
#include "bsp.h"


/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _bsp_io_pcb_shm_int_trigger
* Returned Value  : _mqx_uint result   MQX_OK or an error code.
* Comments        :
*     This function triggers an interrupt to the destination processor.
*
*END*---------------------------------------------------------------------*/

void _bsp_io_pcb_shm_int_trigger(uint_32 vector)
{
    _qintc_set_core_sw_interrupt(_psp_core_num()==0?1:0,vector);
}


/*NOTIFIER*-------------------------------------------------------------
*
*  Function Name  : _bsp_io_pcb_shm_rx_isr
*  Returned Value : void
*  Comments       :
*        Packet reception ISR.
*
*END*-----------------------------------------------------------------*/

static void _bsp_io_pcb_shm_rx_isr
   (
      /* [IN] the info structure */
      pointer     handle
   )
{
    IO_PCB_SHM_INFO_STRUCT_PTR    info_ptr = (IO_PCB_SHM_INFO_STRUCT_PTR)handle;

   _qintc_clear_sw_interrupt(info_ptr->INIT.RX_VECTOR);
   _io_pcb_shm_rx_isr(handle);

}

/*NOTIFIER*-------------------------------------------------------------
*
*  Function Name  : _bsp_io_pcb_shm_tx_isr
*  Returned Value : void
*  Comments       :
*        Packet reception ISR.
*
*END*-----------------------------------------------------------------*/

static void _bsp_io_pcb_shm_tx_isr
   (
      /* [IN] the info structure */
      pointer     handle
   )
{
    IO_PCB_SHM_INFO_STRUCT_PTR    info_ptr = (IO_PCB_SHM_INFO_STRUCT_PTR)handle;

    _qintc_clear_sw_interrupt(info_ptr->INIT.TX_VECTOR);
   _io_pcb_shm_tx_isr(handle);

}

/*FUNCTION*-----------------------------------------------------------------
*
* Function Name   : _bsp_io_pcb_shm_int_install
* Returned Value  : _mqx_uint result   MQX_OK or an error code.
* Comments        :
*     This function is use to install the shared memory device ISRs.
*
*END*---------------------------------------------------------------------*/

_mqx_uint _bsp_io_pcb_shm_int_install
   (
      /* [IN] the initialization record for this device */
      IO_PCB_SHM_INIT_STRUCT_PTR    init_ptr,

      /* [IN] the context record for this device */
      IO_PCB_SHM_INFO_STRUCT_PTR    info_ptr
   )
{
    if (!_int_install_isr(init_ptr->RX_VECTOR, _bsp_io_pcb_shm_rx_isr, info_ptr))
    {
       return MQX_IO_PCB_SHM_INSTALL_ISR_FAILLED;
    }

    /* Install the tx finished ISR */
    if (!_int_install_isr(init_ptr->TX_VECTOR, _bsp_io_pcb_shm_tx_isr, info_ptr))
    {
       return MQX_IO_PCB_SHM_INSTALL_ISR_FAILLED;
    }

    _qintc_set_prio(init_ptr->RX_VECTOR, BSPCFG_IO_PCB_SHM_RX_PRIO);
    _qintc_set_prio(init_ptr->TX_VECTOR, BSPCFG_IO_PCB_SHM_TX_PRIO);

    return MQX_OK;
}
