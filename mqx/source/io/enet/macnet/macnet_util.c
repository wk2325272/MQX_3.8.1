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
* $FileName: macnet_util.c$
* $Version : 3.7.7.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the MACNET util functions.
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "enet.h"
#include "enetprv.h"
#include "macnet_prv.h"


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_install_isr
*  Returned Value : ENET_OK or error code
*  Comments       :
*         
*
*END*-----------------------------------------------------------------*/

boolean MACNET_install_isr( 
   ENET_CONTEXT_STRUCT_PTR enet_ptr, 
   uint_32                 int_num, 
   uint_32                 int_index, 
   void _CODE_PTR_         isr, 
   uint_32                 level, 
   uint_32                 sublevel  ) 
{
   uint_32  vector = MACNET_get_vector(enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER, int_index);
   typedef void(*ISR_PTR)(pointer);
 
#if BSPCFG_ENET_RESTORE
   MACNET_CONTEXT_STRUCT_PTR    macnet_context_ptr = (MACNET_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;


   /* Save old ISR and data */
   macnet_context_ptr->OLDISR_PTR[int_num]   = (void *)_int_get_isr(vector);
   macnet_context_ptr->OLDISR_DATA[int_num] = _int_get_isr_data(vector);
#endif

   if (_int_install_isr(vector, (ISR_PTR)isr, (pointer)enet_ptr)==NULL) {
      return FALSE;
   }

   /* Initialize interrupt priority and level */
   _bsp_int_init((PSP_INTERRUPT_TABLE_INDEX)vector, level, sublevel, TRUE);

   return TRUE;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_install_isrs
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

boolean MACNET_install_isrs( ENET_CONTEXT_STRUCT_PTR enet_ptr, MACNET_INIT_STRUCT const * enet_init_ptr  ) 
{
   boolean bOK;
   typedef void(*FC_INT_PTR);
   
   bOK = MACNET_install_isr(enet_ptr, 0, ENET_INT_TX_INTB, (FC_INT_PTR)MACNET_TX_ISR, enet_init_ptr->ETX_LEVEL,enet_init_ptr->ETX_SUBLEVEL  ) ;
   if (bOK) {
      bOK = MACNET_install_isr(enet_ptr, 1, ENET_INT_TX_INTF, (FC_INT_PTR)MACNET_TX_ISR, enet_init_ptr->ETX_LEVEL,enet_init_ptr->ETX_SUBLEVEL  ) ;
   }
   if (bOK) {
      bOK = MACNET_install_isr(enet_ptr, 2, ENET_INT_RX_INTB, (FC_INT_PTR)MACNET_RX_ISR, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL  ) ;
   }
   if (bOK) {
      bOK = MACNET_install_isr(enet_ptr, 3, ENET_INT_RX_INTF, (FC_INT_PTR)MACNET_RX_ISR, enet_init_ptr->ERX_LEVEL,enet_init_ptr->ERX_SUBLEVEL  ) ;
   }
#if ENETCFG_SUPPORT_PTP
   if (bOK) {
      bOK = MACNET_ptp_install_ts_avail_isr(enet_ptr, enet_init_ptr ) ;
   }
   /* Just one MACNET module handles 1588timer and incrementation of seconds */
   if (bOK && (enet_ptr->PARAM_PTR->OPTIONS & ENET_OPTION_PTP_MASTER_CLK)) {
      bOK = MACNET_ptp_install_ts_timer_isr(enet_ptr, enet_init_ptr ) ;
   }
#endif /* ENETCFG_SUPPORT_PTP */

   return bOK;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_mask_interrupts
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

void MACNET_mask_interrupts( ENET_CONTEXT_STRUCT_PTR enet_ptr ) 
{
   uint_32  i;
   
   for (i=0;i<ENET_NUM_INTS;i++) {
      _bsp_int_disable((PSP_INTERRUPT_TABLE_INDEX)(MACNET_get_vector(enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER,i)));
   }
}


#if BSPCFG_ENET_RESTORE

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_uninstall_isr
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

static void MACNET_uninstall_isr( ENET_CONTEXT_STRUCT_PTR enet_ptr, uint_32 int_num, uint_32 int_index  ) 
{
   uint_32  vector = MACNET_get_vector(enet_ptr->PARAM_PTR->ENET_IF->MAC_NUMBER, int_index);
   MACNET_CONTEXT_STRUCT_PTR    macnet_context_ptr = (MACNET_CONTEXT_STRUCT_PTR) enet_ptr->MAC_CONTEXT_PTR;
 
   typedef void( * FC_INT_PTR)(pointer);
   _bsp_int_disable((PSP_INTERRUPT_TABLE_INDEX)vector);
   if (macnet_context_ptr->OLDISR_PTR[int_num]) {
      _int_install_isr(vector, (FC_INT_PTR)macnet_context_ptr->OLDISR_PTR[int_num], macnet_context_ptr->OLDISR_DATA[int_num]);
      macnet_context_ptr->OLDISR_PTR[int_num] = NULL;
   }
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_uninstall_all_isrs
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

void MACNET_uninstall_all_isrs( ENET_CONTEXT_STRUCT_PTR enet_ptr ) 
{
   MACNET_uninstall_isr(enet_ptr, 0, ENET_INT_TX_INTB) ;
   MACNET_uninstall_isr(enet_ptr, 1, ENET_INT_TX_INTF) ;
   MACNET_uninstall_isr(enet_ptr, 2, ENET_INT_RX_INTB) ;
   MACNET_uninstall_isr(enet_ptr, 3, ENET_INT_RX_INTF) ;
   MACNET_uninstall_isr(enet_ptr, 4, ENET_INT_RX_INTF) ;
}
#endif


/*FUNCTION*-------------------------------------------------------------
* 
* Function Name    : MACNET_get_vector
* Returned Value   : MQX vector number for specified interrupt
* Comments         :
*    This function returns index into MQX interrupt vector table for
*    specified enet interrupt. If not known, returns 0.
*
*END*-----------------------------------------------------------------*/


uint_32 MACNET_get_vector 
(
    uint_32 device,
    uint_32 vector_index
)
{ 
    uint_32 index = 0;

    if ((device < MACNET_DEVICE_COUNT) && (vector_index < ENET_NUM_INTS)) {
      index = MACNET_vectors[device][vector_index];
    }

    return index;
} 


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : MACNET_free_context
*  Returned Value :  
*  Comments       :
*        
*
*END*-----------------------------------------------------------------*/

void MACNET_free_context( MACNET_CONTEXT_STRUCT_PTR macnet_context_ptr ) 
{
   if (macnet_context_ptr) {
      if (macnet_context_ptr->UNALIGNED_BUFFERS) {
         _mem_free((pointer)macnet_context_ptr->UNALIGNED_BUFFERS);
      }
      if (macnet_context_ptr->RX_PCB_BASE) {
         _mem_free((pointer)macnet_context_ptr->RX_PCB_BASE);
      }
      if (macnet_context_ptr->TxPCBS_PTR) {
         _mem_free((pointer)macnet_context_ptr->TxPCBS_PTR);
      }
      if (macnet_context_ptr->UNALIGNED_RING_PTR) {
         _mem_free((pointer)macnet_context_ptr->UNALIGNED_RING_PTR);
      }
   
      _mem_free((pointer)macnet_context_ptr);
   }
}


/* EOF */
