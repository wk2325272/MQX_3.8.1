/**HEADER********************************************************************
* 
* Copyright (c) 2011 Freescale Semiconductor;
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
* $FileName: spi_dma_dspi.c$
* $Version : 3.8.2.2$
* $Date    : Apr-13-2012$
*
* Comments:
*
*   The file contains low level DMA DSPI driver functions.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <io_prv.h>
#include <fio_prv.h>
#include "spi.h"
#include "spi_int_prv.h"
#include "spi_dspi_prv.h"
/* wk @130512 */
#include "app_wk.h"  // wk@130512 --> 用户定义变量
#include "event.h" // wk@130103 --> 事件引用文件

extern uint_32 _dspi_polled_init(DSPI_INIT_STRUCT_PTR, pointer _PTR_, char_ptr);
extern uint_32 _dspi_polled_ioctl(VDSPI_INFO_STRUCT_PTR, uint_32, uint_32_ptr, uint_32);

static uint_32 _dspi_dma_init(IO_SPI_INT_DEVICE_STRUCT_PTR, char_ptr);
static uint_32 _dspi_dma_deinit(IO_SPI_INT_DEVICE_STRUCT_PTR, VDSPI_INFO_STRUCT_PTR);
static uint_32 _dspi_dma_rx(IO_SPI_INT_DEVICE_STRUCT_PTR, uchar_ptr, int_32);
static uint_32 _dspi_dma_tx(IO_SPI_INT_DEVICE_STRUCT_PTR, uchar_ptr, int_32);
static uint_32 _dspi_dma_enable(VDSPI_INFO_STRUCT_PTR io_info_ptr);
static void    _dspi_dma_isr_err(pointer parameter);
static void    _dspi_dma_isr_rx(pointer parameter);

/* wk@121120-->deifne for reverse DMA register : 0--> default  1--> wk  */
#define wk_rx 1    
#define wk_tx 1    
#define wk_dma_daddr 1 
#define wk_dma_saddr_tx 1

/* Temporary definitions, will be removed when eDMA API is introduced */

#if (BSP_TWR_K70F120M || BSP_TWR_K60F120M)

#define INT_DMA0 INT_DMA0_DMA16
#define INT_DMA1 INT_DMA1_DMA17
#define INT_DMA2 INT_DMA2_DMA18
#define INT_DMA3 INT_DMA3_DMA19
#define INT_DMA4 INT_DMA4_DMA20
#define INT_DMA5 INT_DMA5_DMA21
#define INT_DMA6 INT_DMA6_DMA22
#define INT_DMA7 INT_DMA7_DMA23
#define INT_DMA8 INT_DMA8_DMA24
#define INT_DMA9 INT_DMA9_DMA25
#define INT_DMA10 INT_DMA10_DMA26
#define INT_DMA11 INT_DMA11_DMA27
#define INT_DMA12 INT_DMA12_DMA28
#define INT_DMA13 INT_DMA13_DMA29
#define INT_DMA14 INT_DMA14_DMA30
#define INT_DMA15 INT_DMA15_DMA31 
#define DMAMUX_CHCFG DMAMUX0_CHCFG
#define SIM_SCGC6_DMAMUX_MASK SIM_SCGC6_DMAMUX0_MASK

#endif

#if (MQX_CPU == PSP_CPU_MK20D50M)
static const uint_32 DMA_INTS[] = {INT_DMA0, INT_DMA1, INT_DMA2, INT_DMA3};
#else
static const uint_32 DMA_INTS[] = {INT_DMA0, INT_DMA1, INT_DMA2, INT_DMA3, INT_DMA4, INT_DMA5, INT_DMA6, INT_DMA7, INT_DMA8, INT_DMA9, INT_DMA10, INT_DMA11, INT_DMA12, INT_DMA13, INT_DMA14, INT_DMA15}; 
#endif

/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_get_rx_count
* Returned Value   : MQX error code
* Comments         :
*    Get allowed DMA RX transfer size.
*
*END*********************************************************************/

static uint_32 _dspi_dma_get_rx_count 
(
    /* DSPI state info */
    VDSPI_INFO_STRUCT_PTR io_info_ptr
)
{
    uint_32               size;
    
    if (io_info_ptr->RX_OUT > io_info_ptr->RX_IN)
    {
        /* Limit by free space left, stop 1 element before out pointer */
        size = (io_info_ptr->RX_OUT - io_info_ptr->RX_IN) - 1;
    }
    else
    {
        /* Space available to the end of the SW buffer */
        size = io_info_ptr->INIT.RX_BUFFER_SIZE - io_info_ptr->RX_IN;
        
        /* Stop 1 element before out pointer */
        if (0 == io_info_ptr->RX_OUT)
        {
            size--;
        }
    }
    
    /* Limit to half of the SW buffer */
    if (size > (io_info_ptr->INIT.RX_BUFFER_SIZE >> 1))
    {
        size = io_info_ptr->INIT.RX_BUFFER_SIZE >> 1;
    }
    
    return size;
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_get_tx_count
* Returned Value   : MQX error code
* Comments         :
*    Get allowed DMA TX transfer size.
*
*END*********************************************************************/

static uint_32 _dspi_dma_get_tx_count 
(
    /* DSPI state info */
    VDSPI_INFO_STRUCT_PTR io_info_ptr
)
{
    uint_32               size;
    
    if (io_info_ptr->TX_IN >= io_info_ptr->TX_OUT)
    {
        /* Limit by data left */
        size = io_info_ptr->TX_IN - io_info_ptr->TX_OUT;
    }
    else
    {
        /* Data available to the end of the SW buffer */
        size = io_info_ptr->INIT.TX_BUFFER_SIZE - io_info_ptr->TX_OUT; 
    }
    
    /* Limit to half of the SW buffer */
    if (size > (io_info_ptr->INIT.TX_BUFFER_SIZE >> 1))
    {
        size = io_info_ptr->INIT.TX_BUFFER_SIZE >> 1;
    }
    
    return size;
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_setup
* Returned Value   : MQX error code
* Comments         :
*    Setup RX and TX DMA transfers.
*
*END*********************************************************************/

static void _dspi_dma_setup 
    (
        /* [IN] DSPI state info */
        VDSPI_INFO_STRUCT_PTR io_info_ptr,
        
        /* [IN] Whether to use/move inside RX buffer */
        uint_32               rx_offset,
        
        /* [IN] Whether to use/move inside TX buffer */
        uint_32               tx_offset,
        
        /* [IN] Byte size of the transfer */
        uint_32               size        
    )
{
    uint_32                   channel;
    
    /* Setup and run RX DMA */
    channel = io_info_ptr->INIT.DMA_RX_CHANNEL;
#if !wk_rx  // wk_rx = 0   <1>
    DMA_ATTR(channel) = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);  
    DMA_NBYTES_MLNO(channel) = sizeof (uint_32);
#endif
#if wk_rx  // wk_rx = 1   <1>
     DMA_ATTR(channel) = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);
     DMA_NBYTES_MLNO(channel) = sizeof(uchar);
#endif
     
    DMA_SADDR(channel) = (vuint_32)&(io_info_ptr->DSPI_PTR->POPR);
#if !wk_rx // wk_rx = 0   <2>
    DMA_SOFF(channel) = 0;
    DMA_SLAST(channel) = 0;
#endif
#if wk_rx // wk_r = 1    <2>
     DMA_SOFF(channel) = 0;   // wk ? 4
     DMA_SLAST(channel) = 0;  // wk ? 8
#endif   

#if !wk_dma_daddr // wk_dma_addr = 0 
    if (rx_offset)
    {
        DMA_DADDR(channel) = (vuint_32)&(io_info_ptr->RX_BUFFER[io_info_ptr->RX_IN]);
    }
    else
    {
        DMA_DADDR(channel) = (vuint_32)&(io_info_ptr->RX_DATA);
    }
#endif
#if wk_dma_daddr  // wk_dma_daddr = 1
    DMA_DADDR(channel) = (vuint_32)BufRxchar; // SPI2的接收 DMA 通道目的地址绑定到 BufRxchar
#endif
    
#if !wk_rx // wk_rx = 0   <3>
    DMA_DOFF(channel) = rx_offset;
    DMA_DLAST_SGA(channel) = 0;
#endif
#if wk_rx  // wk_rx = 1  <3>
    DMA_DOFF(channel) = 1; // wk ? 4
    DMA_DLAST_SGA(channel) = 0;  // wk ? 8
#endif     
    
#if !wk_rx // wk_rx = 0   <4>
    DMA_CITER_ELINKNO(channel) = DMA_CITER_ELINKNO_CITER(size);
    DMA_BITER_ELINKNO(channel) = DMA_BITER_ELINKNO_BITER(size);
#endif
#if wk_rx   // wk_rx = 1  <4>
     DMA_CITER_ELINKNO(channel) = DataSize;
     DMA_BITER_ELINKNO(channel) = DataSize;
#endif    
    
    DMA_CSR(channel) = DMA_CSR_INTMAJOR_MASK | DMA_CSR_DREQ_MASK;
    if (size)
    {
        DMA_SERQ = channel;
    }
  DMA_SERQ = channel; // wk@130512 -> 重要：配置完成直接开启 DMA，而不是通过外部函数调用开启  
/*********************************************************************************/  
    /* Setup and run TX DMA */
    channel = io_info_ptr->INIT.DMA_TX_CHANNEL;
    
#if !wk_tx // wk_tx = 0   <1>
    DMA_ATTR(channel) = DMA_ATTR_SSIZE(2) | DMA_ATTR_DSIZE(2);  
    DMA_NBYTES_MLNO(channel) = sizeof (uint_32);
#endif
#if wk_tx // wk_tx = 1   <1>
    DMA_ATTR(channel) = DMA_ATTR_SSIZE(0) | DMA_ATTR_DSIZE(0);  
    DMA_NBYTES_MLNO(channel) = sizeof (uchar);
#endif

#if !wk_dma_saddr_tx // default    
    if (tx_offset)
    {
        DMA_SADDR(channel) = (vuint_32)&(io_info_ptr->TX_BUFFER[io_info_ptr->TX_OUT]);
        
    }
    else
    {
        DMA_SADDR(channel) = (vuint_32)&(io_info_ptr->TX_DATA);
    }
#endif
#if wk_dma_saddr_tx // wk_dma_saddr_tx = 1
    DMA_SADDR(channel) = (vuint_32)SysDataSend;
#endif

#if !wk_tx   // wk_tx = 0   <2> 
    DMA_SOFF(channel) = tx_offset;
    DMA_SLAST(channel) = 0;
#endif
#if wk_tx  // wk_tx = 1  <2>
    DMA_SOFF(channel) = 1;  
    DMA_SLAST(channel) = 0;  // wk ? 8
#endif
    
    DMA_DADDR(channel) = (vuint_32)&(io_info_ptr->DSPI_PTR->PUSHR);

#if !wk_tx  // wk_tx = 0   <3> 
    DMA_DOFF(channel) = 0;
    DMA_DLAST_SGA(channel) = 0;
#endif
#if wk_tx  // wk_tx = 1  <3>
    DMA_DOFF(channel) = 0; // wk ? 4
    DMA_DLAST_SGA(channel) = 0;  // wk ? 8
#endif
  
#if !wk_tx  // wk_tx = 0   <4>
    DMA_CITER_ELINKNO(channel) = DMA_CITER_ELINKNO_CITER(size);
    DMA_BITER_ELINKNO(channel) = DMA_BITER_ELINKNO_BITER(size);
#endif
#if wk_tx  // wk_tx = 1  <4>
    DMA_CITER_ELINKNO(channel) = 56;
    DMA_BITER_ELINKNO(channel) = 56;
#endif
    
    DMA_CSR(channel) = DMA_CSR_DREQ_MASK;
/* wk @130512 --> SPI 发送 DMA 配置完成后开启方式的修改 */
//    if (size)
//    {
//        DMA_SERQ = channel;
//    }
    if (SPI_Send)
    {
        DMA_SERQ = channel;
    }
/* wk @130512 --> end */
    
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_install
* Returned Value   : MQX error code
* Comments         :
*    Install an SPI device.
*
*END*********************************************************************/

uint_32 _dspi_dma_install
    (
        /* [IN] A string that identifies the device for fopen */
        char_ptr                         identifier,
  
        /* [IN] The I/O init data pointer */
        DSPI_INIT_STRUCT_CPTR            init_data_ptr
    )
{
    return _io_spi_int_install (identifier,
        (uint_32 (_CODE_PTR_)(pointer,char_ptr))_dspi_dma_init,
        (uint_32 (_CODE_PTR_)(pointer))_dspi_dma_enable,
        (uint_32 (_CODE_PTR_)(pointer, pointer))_dspi_dma_deinit,
        (_mqx_int (_CODE_PTR_)(pointer, char_ptr, int_32))_dspi_dma_rx,
        (_mqx_int (_CODE_PTR_)(pointer, char_ptr, int_32))_dspi_dma_tx,
        (_mqx_int (_CODE_PTR_)(pointer, uint_32, _mqx_uint_ptr, _mqx_uint))_dspi_polled_ioctl, 
        (pointer)init_data_ptr);
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_init  
* Returned Value   : MQX error code
* Comments         :
*    This function initializes the SPI module 
*
*END*********************************************************************/

static uint_32 _dspi_dma_init
    (
        /* [IN] The address of the device specific information */
        IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

        /* [IN] The rest of the name of the device opened */
        char_ptr                     open_name_ptr
    )
{
    VDSPI_REG_STRUCT_PTR             dspi_ptr;
    VDSPI_INFO_STRUCT_PTR            io_info_ptr;
    DSPI_INIT_STRUCT_PTR             spi_init_ptr;
    uint_32                          result = SPI_OK;
    const uint_32 _PTR_              vectors;
    int                              num_vectors;
    int                              i;

    /* Initialization as in polled mode */
    spi_init_ptr = (DSPI_INIT_STRUCT_PTR)(int_io_dev_ptr->DEV_INIT_DATA_PTR);
    result = _dspi_polled_init (spi_init_ptr, &(int_io_dev_ptr->DEV_INFO_PTR), open_name_ptr);
    if (result)
    {
        return result;
    }
    
    num_vectors = _bsp_get_dspi_vectors (spi_init_ptr->CHANNEL, &vectors);
    if (0 == num_vectors)
    {
        _mem_free (int_io_dev_ptr->DEV_INFO_PTR);
        int_io_dev_ptr->DEV_INFO_PTR = NULL;
        return MQX_INVALID_VECTORED_INTERRUPT;
    }
   
    io_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
    dspi_ptr = io_info_ptr->DSPI_PTR;

    /* Allocate buffers */
    io_info_ptr->RX_BUFFER = (uint_32_ptr)_mem_alloc_system (sizeof (uint_32) * spi_init_ptr->RX_BUFFER_SIZE);
    if (NULL == io_info_ptr->RX_BUFFER) 
    {
        _mem_free (int_io_dev_ptr->DEV_INFO_PTR);
        int_io_dev_ptr->DEV_INFO_PTR = NULL;
        return MQX_OUT_OF_MEMORY;
    }
    io_info_ptr->TX_BUFFER = (uint_32_ptr)_mem_alloc_system (sizeof (uint_32) * spi_init_ptr->TX_BUFFER_SIZE);
    if (NULL == io_info_ptr->TX_BUFFER) 
    {
        _mem_free (int_io_dev_ptr->DEV_INFO_PTR);
        int_io_dev_ptr->DEV_INFO_PTR = NULL;
        _mem_free (io_info_ptr->RX_BUFFER);
        io_info_ptr->RX_BUFFER = NULL;
        return MQX_OUT_OF_MEMORY;
    }
    _mem_set_type (io_info_ptr->RX_BUFFER,MEM_TYPE_IO_SPI_IN_BUFFER);       
    _mem_set_type (io_info_ptr->TX_BUFFER,MEM_TYPE_IO_SPI_OUT_BUFFER);       
   
    /* Install ISRs */
    for (i=0; i<num_vectors; i++) 
    {
        io_info_ptr->OLD_ISR_DATA[i] = _int_get_isr_data(vectors[i]);
        io_info_ptr->OLD_ISR[i] = _int_install_isr(vectors[i], _dspi_dma_isr_err, int_io_dev_ptr);
    }
   
    _int_install_isr (DMA_INTS[io_info_ptr->INIT.DMA_RX_CHANNEL], _dspi_dma_isr_rx, int_io_dev_ptr);
   
    return SPI_OK; 
}

/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_deinit  
* Returned Value   : MQX error code
* Comments         :
*    This function de-initializes the SPI module 
*
*END*********************************************************************/

static uint_32 _dspi_dma_deinit
    (
        /* [IN] The address of the device specific information */
        IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

        /* [IN] The address of the channel specific information */
        VDSPI_INFO_STRUCT_PTR        io_info_ptr
    )
{
    VDSPI_REG_STRUCT_PTR             dspi_ptr;
    uint_32                          index;
    const uint_32 _PTR_              vectors;
    int                              num_vectors;
    int                              i;
   
    if ((NULL == io_info_ptr) || (NULL == int_io_dev_ptr)) 
    {
        return SPI_ERROR_DEINIT_FAILED;
    }
   
    /* Disable the SPI */
    dspi_ptr = io_info_ptr->DSPI_PTR;
    index = dspi_ptr->MCR & DSPI_MCR_MSTR_MASK;
    dspi_ptr->MCR |= DSPI_MCR_HALT_MASK;
    dspi_ptr->RSER = 0;

    /* Disable all chip selects */
    if (0 != index) 
    {
        for (index = 0; index < DSPI_CS_COUNT; index++)
        {
            if ((NULL != io_info_ptr->CS_CALLBACK[index]) && (0 != (io_info_ptr->CS_ACTIVE & (1 << index))))
            {
                io_info_ptr->CS_CALLBACK[index] (DSPI_PUSHR_PCS(1 << index), 1, io_info_ptr->CS_USERDATA[index]);
            }                
        }
        io_info_ptr->CS_ACTIVE = 0;
    }

    /* Disable DMA */
    index = io_info_ptr->INIT.DMA_RX_CHANNEL;
    DMAMUX_CHCFG(index) = 0;

    index = io_info_ptr->INIT.DMA_TX_CHANNEL;
    DMAMUX_CHCFG(index) = 0;

   _bsp_int_init ((IRQInterruptIndex)DMA_INTS[io_info_ptr->INIT.DMA_RX_CHANNEL], BSP_DSPI_INT_LEVEL, 0, FALSE);
   
    /* Return original interrupt vectors */
    num_vectors = _bsp_get_dspi_vectors (io_info_ptr->INIT.CHANNEL, &vectors);
    for (i=0; i<num_vectors; i++) 
    {
        _int_install_isr (vectors[i], io_info_ptr->OLD_ISR[i], io_info_ptr->OLD_ISR_DATA[i]);
    }

    /* Release buffers */
    _mem_free (int_io_dev_ptr->DEV_INFO_PTR);
    int_io_dev_ptr->DEV_INFO_PTR = NULL;
    _mem_free (io_info_ptr->RX_BUFFER);
    io_info_ptr->RX_BUFFER = NULL;
    _mem_free (io_info_ptr->TX_BUFFER);
    io_info_ptr->TX_BUFFER = NULL;
      
    return SPI_OK;
}


/*FUNCTION****************************************************************
*
* Function Name    : _dspi_dma_enable
* Returned Value   : MQX error code
* Comments         :
*    This function enables receive and error interrupt.
*
*END*********************************************************************/

static uint_32 _dspi_dma_enable
    ( 
        /* [IN] The address of the channel specific information */
        VDSPI_INFO_STRUCT_PTR io_info_ptr
    )
{
    VDSPI_REG_STRUCT_PTR      dspi_ptr;
    const uint_32 _PTR_       vectors;
    int                       num_vectors;
    int                       i, channel;

    if (NULL == io_info_ptr)   
    {
        return SPI_ERROR_INVALID_PARAMETER;   
    }
   
    num_vectors = _bsp_get_dspi_vectors (io_info_ptr->INIT.CHANNEL, &vectors);
    for (i=0; i<num_vectors; i++) 
    {
        _bsp_int_init ((PSP_INTERRUPT_TABLE_INDEX)vectors[i], BSP_DSPI_INT_LEVEL, 0, TRUE);
    }
    _bsp_int_init ((IRQInterruptIndex)DMA_INTS[io_info_ptr->INIT.DMA_RX_CHANNEL], BSP_DSPI_INT_LEVEL, 0, TRUE);

    dspi_ptr = io_info_ptr->DSPI_PTR;
    dspi_ptr->MCR |= DSPI_MCR_HALT_MASK;
    dspi_ptr->RSER |= SPI_RSER_RFOF_RE_MASK | SPI_RSER_TFUF_RE_MASK | SPI_RSER_RFDF_RE_MASK | SPI_RSER_TFFF_RE_MASK | SPI_RSER_RFDF_DIRS_MASK | SPI_RSER_TFFF_DIRS_MASK;
    dspi_ptr->MCR &= (~ DSPI_MCR_HALT_MASK);

    /* initial DMA setup */
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    
    DMA_CERQ = io_info_ptr->INIT.DMA_RX_CHANNEL;
    DMAMUX_CHCFG(io_info_ptr->INIT.DMA_RX_CHANNEL) = 0;
    DMA_CERQ = io_info_ptr->INIT.DMA_TX_CHANNEL;
    DMAMUX_CHCFG(io_info_ptr->INIT.DMA_TX_CHANNEL) = 0;
    
    _dspi_dma_setup (io_info_ptr, 0, 0, 0);
    
    DMAMUX_CHCFG(io_info_ptr->INIT.DMA_RX_CHANNEL) = DMAMUX_CHCFG_ENBL_MASK | io_info_ptr->INIT.DMA_RX_SOURCE;    
    DMAMUX_CHCFG(io_info_ptr->INIT.DMA_TX_CHANNEL) = DMAMUX_CHCFG_ENBL_MASK | io_info_ptr->INIT.DMA_TX_SOURCE;
      
    return SPI_OK;
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_rx
* Returned Value   : Returns the number of bytes received
* Comments         : 
*   Reads the data into provided array.
*
*END*********************************************************************/

static uint_32 _dspi_dma_rx
    (
        /* [IN] The address of the device specific information */
        IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

        /* [IN] The array characters are to be read from */
        uchar_ptr                    buffer,
      
        /* [IN] Number of char's to transmit */
        int_32                       size
    )
{
    VDSPI_INFO_STRUCT_PTR            io_info_ptr;
    VDSPI_REG_STRUCT_PTR             dspi_ptr;
    uint_32                          num, index, data, channel, buflen;
    uint_32_ptr                      rx_ptr;
    vuint_32                         flags;

    io_info_ptr  = int_io_dev_ptr->DEV_INFO_PTR;
    dspi_ptr = io_info_ptr->DSPI_PTR;
    buflen = io_info_ptr->INIT.RX_BUFFER_SIZE;
   
    /* Check whether new data in rx buffer */
    flags = io_info_ptr->DMA_FLAGS;
    rx_ptr = io_info_ptr->RX_BUFFER;
    index = io_info_ptr->RX_OUT;
    channel = io_info_ptr->INIT.DMA_RX_CHANNEL;
    for (num = 0; num < size; num++) 
    {
        if (index == io_info_ptr->RX_IN) 
        {
            break;
        }
        data = rx_ptr[index];
        index++;
        if (index >= buflen) 
        {
            index = 0;
        }
        if (DSPI_CTAR_FMSZ_GET(dspi_ptr->CTAR[0]) > 7)
        {
            *buffer++ = (uint_8)(data >> 8);
        }
        *buffer++ = (uint_8)data;
    }
    io_info_ptr->RX_OUT = index;
    size -= num;

    if ((0 != size) && (0 == (flags & (DSPI_DMA_FLAG_RX | DSPI_DMA_FLAG_TX))))
    {
        /* Not enough data, assert chip selects and enable further transfer */
        if (dspi_ptr->MCR & DSPI_MCR_MSTR_MASK) 
        {
            if (io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE)
            {
                for (index = 0; index < DSPI_CS_COUNT; index++)
                {
                    if ((0 != ((io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE) & (1 << index))) && (NULL != io_info_ptr->CS_CALLBACK[index]))
                    {
                        io_info_ptr->CS_CALLBACK[index] (DSPI_PUSHR_PCS(1 << index), (io_info_ptr->CS_ACTIVE >> index) & 1, io_info_ptr->CS_USERDATA[index]);
                    }
                }
                io_info_ptr->CS_ACTIVE = io_info_ptr->CS;
            }
        }
      
        /* Signal ongoing DMA */
        io_info_ptr->DMA_FLAGS |= DSPI_DMA_FLAG_RX;
     
        /* Adjust DMA transfer length */
        buflen = _dspi_dma_get_rx_count (io_info_ptr);
        if (buflen > size)
        {
            buflen = size;
        }
        io_info_ptr->RX_REQUEST = size - buflen;
        io_info_ptr->RX_COUNT = buflen; 
        io_info_ptr->TX_COUNT = 0;
        
        io_info_ptr->TX_DATA = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(io_info_ptr->CS) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA(0xFFFF);

        _dspi_dma_setup (io_info_ptr, sizeof (uint_32), 0, buflen);
    }
    return num;
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_tx
* Returned Value   : return number of byte transmitted
* Comments         : 
*   Writes the provided data into trasmit buffer
*
*END*********************************************************************/

static uint_32 _dspi_dma_tx
    (
        /* [IN] The address of the device specific information */
        IO_SPI_INT_DEVICE_STRUCT_PTR int_io_dev_ptr,

        /* [IN] The array to store data */
        uchar_ptr                    buffer,
      
        /* [IN] Number of char's to transmit */
        int_32                       size
    )
{
    VDSPI_INFO_STRUCT_PTR            io_info_ptr;
    VDSPI_REG_STRUCT_PTR             dspi_ptr;
    uint_32                          num, index, data, channel, buflen, tmp, flags;
    uint_32_ptr                      tx_ptr;
    
    io_info_ptr = int_io_dev_ptr->DEV_INFO_PTR;
    dspi_ptr = io_info_ptr->DSPI_PTR;
    buflen = io_info_ptr->INIT.TX_BUFFER_SIZE;

    /* Fill new data into transmit buffer */
    tx_ptr = io_info_ptr->TX_BUFFER;
    index = io_info_ptr->TX_IN;
    channel = io_info_ptr->INIT.DMA_TX_CHANNEL;
    for (num = 0; num < size; num++) 
    {
        tmp = index + 1;
        if (tmp >= buflen) 
        {
            tmp = 0;
        }
        if (tmp == io_info_ptr->TX_OUT) 
        {
            break;
        }
        data = *buffer++;
        if (DSPI_CTAR_FMSZ_GET(dspi_ptr->CTAR[0]) > 7)
        {
            data = (data << 8) | *buffer++;
        }
        tx_ptr[index] = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(io_info_ptr->CS) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA(data);
        index = tmp;
    }

    while (1)
    {
        data = io_info_ptr->STATS.INTERRUPTS;
        io_info_ptr->TX_IN = index;
        flags = io_info_ptr->DMA_FLAGS;
        if (data == io_info_ptr->STATS.INTERRUPTS)
        {
            break;
        }
    }

    if ((0 != size) && (0 == (flags & (DSPI_DMA_FLAG_RX | DSPI_DMA_FLAG_TX))))
    {
        /* Adjust DMA transfer length */
        buflen = _dspi_dma_get_tx_count (io_info_ptr);
        if (int_io_dev_ptr->FLAGS & SPI_FLAG_FULL_DUPLEX)
        {
            tmp = _dspi_dma_get_rx_count (io_info_ptr);
            if (buflen > tmp)
            {
                buflen = tmp;
            }
        }
        
        if (0 != buflen) 
        {
            /* At least one byte to transmit, assert chip selects and enable transfer */
            if (dspi_ptr->MCR & DSPI_MCR_MSTR_MASK) 
            {
                if (io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE)
                {
                    for (index = 0; index < DSPI_CS_COUNT; index++)
                    {
                        if ((0 != ((io_info_ptr->CS ^ io_info_ptr->CS_ACTIVE) & (1 << index))) && (NULL != io_info_ptr->CS_CALLBACK[index]))
                        {
                            io_info_ptr->CS_CALLBACK[index] (DSPI_PUSHR_PCS(1 << index), (io_info_ptr->CS_ACTIVE >> index) & 1, io_info_ptr->CS_USERDATA[index]);
                        }
                    }
                    io_info_ptr->CS_ACTIVE = io_info_ptr->CS;
                }
            }
        
            /* Signal ongoing DMA */
            io_info_ptr->DMA_FLAGS |= DSPI_DMA_FLAG_TX;
            
            io_info_ptr->RX_REQUEST = 0;
            io_info_ptr->TX_COUNT = buflen; 
            
            channel = io_info_ptr->INIT.DMA_RX_CHANNEL;
            if (int_io_dev_ptr->FLAGS & SPI_FLAG_FULL_DUPLEX)
            {
                io_info_ptr->RX_COUNT = buflen;
                
                _dspi_dma_setup (io_info_ptr, sizeof (uint_32), sizeof (uint_32), buflen);
            }
            else 
            {
                io_info_ptr->RX_COUNT = 0;
                
                _dspi_dma_setup (io_info_ptr, 0, sizeof (uint_32), buflen);
            }
        }
    }
    return num;
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_isr_err
* Returned Value   : SPI error interrupt routine
* Comments         : 
*   Just for statistics and error clearing.
*
*END*********************************************************************/

static void _dspi_dma_isr_err
    (
        /* [IN] The address of the device specific information */
        pointer           parameter
    )
{
    VDSPI_INFO_STRUCT_PTR io_info_ptr;
    VDSPI_REG_STRUCT_PTR  dspi_ptr;
    uint_32               state;
   
    io_info_ptr = ((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->DEV_INFO_PTR;
    dspi_ptr = (VDSPI_REG_STRUCT_PTR)(io_info_ptr->DSPI_PTR);
   
    /* Read interrupt state */
    state = dspi_ptr->SR;
    io_info_ptr->STATS.INTERRUPTS++;

    if (state & DSPI_SR_TFUF_MASK)
    {
        dspi_ptr->SR |= DSPI_SR_TFUF_MASK;
        io_info_ptr->STATS.TX_UNDERFLOWS++;
    }
    if (state & DSPI_SR_RFOF_MASK)
    {
        dspi_ptr->SR |= DSPI_SR_RFOF_MASK;
        io_info_ptr->STATS.RX_OVERFLOWS++;
    }
}


/*FUNCTION****************************************************************
* 
* Function Name    : _dspi_dma_isr_rx
* Returned Value   : SPI DMA RX routine
* Comments         : 
*   SPI DMA receive done routine.
*
*END*********************************************************************/

static void _dspi_dma_isr_rx
    (
        /* [IN] The address of the device specific information */
        pointer           parameter
    )
{
    VDSPI_INFO_STRUCT_PTR io_info_ptr;
    uint_32               state, channel, size, tmp;
   
    io_info_ptr = ((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->DEV_INFO_PTR;
   
    /* Clear RX interrupt */
    channel = io_info_ptr->INIT.DMA_RX_CHANNEL;
    DMA_CINT = channel;

#if 0 // wk@130512 --> 中断函数中，没有此部分，功能正常
    /* Stats... */
    io_info_ptr->STATS.INTERRUPTS++;
    io_info_ptr->STATS.RX_PACKETS += io_info_ptr->RX_COUNT;
    io_info_ptr->STATS.TX_PACKETS += io_info_ptr->TX_COUNT;

    /* Move forward in TX buffer */
    io_info_ptr->TX_OUT += io_info_ptr->TX_COUNT;
    if (io_info_ptr->TX_OUT >= io_info_ptr->INIT.TX_BUFFER_SIZE)
    {
        io_info_ptr->TX_OUT = 0;
    }
    io_info_ptr->TX_COUNT = 0;
   
    /* Move in RX buffer */
    io_info_ptr->RX_IN += io_info_ptr->RX_COUNT; 
    if (io_info_ptr->RX_IN >= io_info_ptr->INIT.RX_BUFFER_SIZE)
    {
        io_info_ptr->RX_IN = 0;
    }
    io_info_ptr->RX_COUNT = 0;
   
    /* TX ongoing */
    if (io_info_ptr->DMA_FLAGS & DSPI_DMA_FLAG_TX)
    {
        size = _dspi_dma_get_tx_count (io_info_ptr);
        if (0 != size)
        {
            /* Adjust DMA transfer length */
            if ((((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->FLAGS) & SPI_FLAG_FULL_DUPLEX)
            {
                tmp = _dspi_dma_get_rx_count (io_info_ptr);
                if (size > tmp)
                {
                    size = tmp;
                }
            }
            if (0 != size)
            {
                /* Setup and run dummy or duplex RX DMA */
                if (((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->FLAGS & SPI_FLAG_FULL_DUPLEX)
                {
                    io_info_ptr->RX_COUNT = size;
                    
                    _dspi_dma_setup (io_info_ptr, sizeof (uint_32), sizeof (uint_32), size);
                }
                else 
                {
                    _dspi_dma_setup (io_info_ptr, 0, sizeof (uint_32), size);
                }
            }
            io_info_ptr->TX_COUNT = size;
        }
        else
        {
            /* Nothing to send, check RX_REQUEST, switch to RX or finish */
            if (0 != io_info_ptr->RX_REQUEST)
            {
                io_info_ptr->DMA_FLAGS = DSPI_DMA_FLAG_RX;
            }
        }
    }
   
    /* RX ongoing */
    if (io_info_ptr->DMA_FLAGS & DSPI_DMA_FLAG_RX)
    {
        if (0 != io_info_ptr->RX_REQUEST)
        {
            size = _dspi_dma_get_rx_count (io_info_ptr);
            if (size > io_info_ptr->RX_REQUEST)
            {
                size = io_info_ptr->RX_REQUEST;
            }
            if (0 != size)
            {
                /* Setup and run data RX DMA */
                io_info_ptr->RX_REQUEST -= size;
                io_info_ptr->RX_COUNT = size;
                
                io_info_ptr->TX_DATA = SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS(io_info_ptr->CS) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_TXDATA(0xFFFF);
                
                _dspi_dma_setup (io_info_ptr, sizeof (uint_32), 0, size);
            }
            io_info_ptr->TX_COUNT = 0; 
        }
        else
        {
            /* check TX buffer, switch to TX or finish */
            size = _dspi_dma_get_tx_count (io_info_ptr);
            if (0 != size)
            {
                io_info_ptr->DMA_FLAGS = DSPI_DMA_FLAG_TX;
                
                /* Adjust DMA transfer length */
                if ((((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->FLAGS) & SPI_FLAG_FULL_DUPLEX)
                {
                    tmp = _dspi_dma_get_rx_count (io_info_ptr);
                    if (size > tmp)
                    {
                        size = tmp;
                    }
                }
                if (0 != size)
                {
                    /* Setup and run dummy or duplex RX DMA */
                    if (((IO_SPI_INT_DEVICE_STRUCT_PTR)parameter)->FLAGS & SPI_FLAG_FULL_DUPLEX)
                    {
                        io_info_ptr->RX_COUNT = size;
                        
                        _dspi_dma_setup (io_info_ptr, sizeof (uint_32), sizeof (uint_32), size);
                    }
                    else 
                    {
                        _dspi_dma_setup (io_info_ptr, 0, sizeof (uint_32), size);
                    }
                }
            }
            io_info_ptr->TX_COUNT = size;
        }
    }
   
    /* No transfers, clear flags */
    if (0 == size)
    {
        io_info_ptr->DMA_FLAGS = 0;
    }
#endif // wk@130512 --> 中断函数中，没有此部分，功能正常
 
    _dspi_dma_setup (io_info_ptr, sizeof (uint_32), sizeof (uint_32), 0); // wk -->  这个必须要有,才能启动下一次DMA数据传送
 
#define SPIDMA_EVENT 0

#if SPIDMA_EVENT 
    
   pointer pspidma_event;
    
#ifdef _SPIDMA_DBUG_
   if(_event_open("spidma_event",&pspidma_event) != MQX_OK)
    {
      printf("\n Open event failed of spi_dma_dspi.c");
    }
    else 
      printf("\n Open event OK of spi_dma_dspi.c");
      
    if (_event_set(pspidma_event,0x02) != MQX_OK) {
       printf("\n Set Event failed of spi_dma_dspi.c");
    }  
    else
       printf("\n Set Event OK of spi_dma_dspi.c");   
#endif
    
#ifndef _SPIDMA_DBUG_
    _event_open("spidma_event",&pspidma_event);
    _event_set(pspidma_event,0x02);
#endif
      
#endif // wk --> SPIDMA_EVENT 
      
#if !SPIDMA_EVENT  // 没有使用事件时通过此函数调用应用程序接收SPI数据
    DMA_RecData_OK();  // 调用外部函数处理 DMA 接收的数据 --> 应用程序中的函数
#endif // wk --> !SPIDMA_EVENT
}

/* EOF */
