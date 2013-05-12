/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: virtual_nic_enet.c$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief The file contains functions needed by the Network 
*        Interface for Virtual NIC Application
*****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
 #include "virtual_nic_enet.h"
/*****************************************************************************
 * Constant and Macro's
 *****************************************************************************/
_enet_address nic_mac_addr; 
_ip_address nic_ip_addr = (_ip_address)ENET_IPADDR;

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/

/****************************************************************************
 * Global Variables
 ****************************************************************************/              
extern CDC_HANDLE g_nic_handle;

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/
 
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void Ethernet_Callback(PCB_PTR  enet_pcb, pointer  handle);
static void VNIC_Send_PCB_Free(PCB_PTR nic_pcb);

/*****************************************************************************
 * Local Variables 
 *****************************************************************************/
_enet_handle g_enet_handle;
static ENET_HEADER g_enet_hdr;

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/******************************************************************************
 * 
 *    @name       VNIC_FEC_Init
 *    
 *    @brief      
 *                  
 *    @param      arg : handle to Identify the controller
 * 
 *    @return     None
 *   
 *****************************************************************************/
uint_32 VNIC_FEC_Init(CDC_CONFIG_STRUCT_PTR cdc_config_ptr)
{
    uint_32  error;
    boolean flag = FALSE;

    ENET_get_mac_address ((uint_32)ENET_DEVICE, nic_ip_addr, nic_mac_addr);

    error = ENET_initialize (ENET_DEVICE, nic_mac_addr, flag, &g_enet_handle);
    if(error != ENET_OK)
    {
        #if _DEBUG
            printf ("ENET_Initialization Failed\n");
        #endif  
        return ENETERR_INIT_FAILED; 
    }

    USB_mem_copy(nic_mac_addr, cdc_config_ptr->mac_address, MAC_ADDR_SIZE);
    cdc_config_ptr->ip_address =  nic_ip_addr;
    
    error = ENET_open(g_enet_handle, ENETPROT_IP, Ethernet_Callback, NULL);
    error |= ENET_open(g_enet_handle, ENETPROT_ARP, Ethernet_Callback, NULL);
    if(error != ENET_OK)
    {
        #if _DEBUG
            printf("ENET_open Failed.\n");
        #endif  
        return ENETERR_OPEN_PROT;
    }
    
    cdc_config_ptr->rndis_max_frame_size = ENET_FRAMESIZE_MAXDATA + ENET_FRAMESIZE_HEAD; 
    
    return error;
}

/******************************************************************************
 * 
 *    @name       VNIC_FEC_Send
 *    
 *    @brief      
 *                  
 *    @param      arg : 
 *    @param      enet_hdr_ptr: 
 * 
 *    @return     None
 *   
 *****************************************************************************/     
uint_32 VNIC_FEC_Send(pointer arg, uint_8_ptr ethernet_frame_ptr, uint_32 payload_size)
{
    uint_16 protocol;
    uint_8_ptr nic_pcb = NULL;
    uint_32 error;
    PCB_FRAGMENT_PTR frag = NULL;
    UNUSED_ARGUMENT(arg)
    
    USB_mem_copy(ethernet_frame_ptr, 
        g_enet_hdr.DEST , MAC_ADDR_SIZE);
    USB_mem_copy(ethernet_frame_ptr + MAC_ADDR_SIZE, 
        g_enet_hdr.SOURCE, MAC_ADDR_SIZE);
    USB_mem_copy(ethernet_frame_ptr + 2 * MAC_ADDR_SIZE,
         g_enet_hdr.TYPE, 2);
        
    protocol = *((uint_16_ptr)(((ENET_HEADER_PTR)ethernet_frame_ptr)->TYPE)); 

    nic_pcb = _mem_alloc_system_zero(sizeof(PCB) + sizeof(PCB_FRAGMENT));
    if(nic_pcb == NULL) 
    {
      #if _DEBUG
        printf("Memalloc Failed in VNIC_FEC_Send\n");
      #endif    
      return ENETERR_ALLOC_PCB;
    } 

    frag = ((PCB_PTR)nic_pcb)->FRAG;
    ((PCB_PTR)nic_pcb)->FREE = (void(_CODE_PTR_)(PCB_PTR))VNIC_Send_PCB_Free;
    ((PCB_PTR)nic_pcb)->PRIVATE = NULL;
    frag[0].LENGTH = payload_size;

    frag[0].FRAGMENT = ethernet_frame_ptr;

    frag[1].LENGTH = 0;
    frag[1].FRAGMENT = NULL;
        
     error = ENET_send(g_enet_handle, (PCB_PTR)nic_pcb, protocol, 
        ((ENET_HEADER_PTR)ethernet_frame_ptr)->DEST, 0); //flag passed as Zero
    
    return error;
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : VNIC_Send_PCB_Free
*  Returned Value : None
*  Comments       :
*        Free nic_pcb
*
*END*-----------------------------------------------------------------*/
void VNIC_Send_PCB_Free(PCB_PTR nic_pcb)
{      
    if(nic_pcb != NULL)
    {
        /* ethernet_frame_ptr is ((PCB_PTR)nic_pcb)->FRAG[0].FRAGMENT used in VNIC_FEC_Send
           rndis_pkt_msg_data_ptr is ethernet_frame_ptr - RNDIS_USB_OVERHEAD_SIZE 
           used in USB_Notif_Callback*/
        USB_mem_free(((PCB_PTR)nic_pcb)->FRAG[0].FRAGMENT - RNDIS_USB_OVERHEAD_SIZE); 
        USB_mem_free((void*)nic_pcb);
    }
}

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : Ethernet_Callback
*  Returned Value : ENET or error code
*  Comments       :
*        Services an IP packet.
*
*END*-----------------------------------------------------------------*/
void Ethernet_Callback
   (
      PCB_PTR  enet_pcb,
         /* [IN] the received packet */
      pointer  handle
         /* [IN] the IP interface structure */
   )
{ /* Body */
    uint_32 length = enet_pcb->FRAG[0].LENGTH;
    uint_8_ptr nic_data_ptr = enet_pcb->FRAG[0].FRAGMENT; 
    UNUSED_ARGUMENT(handle)
    
    VNIC_USB_Send(nic_data_ptr, length, enet_pcb);
    return;
} /* Endbody */


/* EOF */
