/**HEADER********************************************************************
*
* Copyright (c) 2009 Freescale Semiconductor;
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
* $FileName: spi_dspi.h$
* $Version : 3.8.10.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the definitions of constants and structures
*   required for the SPI driver
*
*END************************************************************************/

#ifndef __spi_dspi_h__
#define __spi_dspi_h__


/*--------------------------------------------------------------------------*/
/*
**                    CONSTANT DEFINITIONS
*/

#define DSPI_CS_COUNT       (8)


/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

/*
** MCF5XXX_DSPI_INIT_STRUCT
**
** This structure defines the initialization parameters to be used
** when a spi port is initialized.
*/
typedef struct dspi_init_struct
{
   /* spi channel number */
   uint_32 CHANNEL;

   /* The SPI default chip select */
   uint_32 CS;

   /* The SPI transfer mode */
   uint_32 TRANSFER_MODE;

   /* The Baud rate for the channel */
   uint_32 BAUD_RATE;

   /* The clock speed of CPU */
   uint_32 CLOCK_SPEED;

   /* The SPI clock phase and polarity mode */
   uint_32 CLOCK_POL_PHASE;

   /* The maximum size of each receive */
   uint_32 RX_BUFFER_SIZE;

   /* The maximum size of each transmit */
   uint_32 TX_BUFFER_SIZE;

   /* The DMA channel for RX */
   uint_32 DMA_RX_CHANNEL;

   /* The DMA request source for RX */
   uint_32 DMA_RX_SOURCE;

   /* The DMA channel for TX */
   uint_32 DMA_TX_CHANNEL;

   /* The DMA request source for TX */
   uint_32 DMA_TX_SOURCE;

} DSPI_INIT_STRUCT, _PTR_ DSPI_INIT_STRUCT_PTR;

typedef const DSPI_INIT_STRUCT _PTR_ DSPI_INIT_STRUCT_CPTR;


/* This definition is required by BSPs for defining CS */
#ifndef DSPI_PUSHR_PCS_MASK
#define DSPI_PUSHR_PCS_MASK                       0x3F0000u
#define DSPI_PUSHR_PCS_SHIFT                      16
#define DSPI_PUSHR_PCS(x)   (((x) << DSPI_PUSHR_PCS_SHIFT) & DSPI_PUSHR_PCS_MASK)
#endif


/*--------------------------------------------------------------------------*/
/*
**                        FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif

extern _mqx_uint _dspi_polled_install(char_ptr, DSPI_INIT_STRUCT_CPTR);
extern uint_32   _dspi_int_install(char_ptr, DSPI_INIT_STRUCT_CPTR);
extern uint_32   _dspi_dma_install(char_ptr, DSPI_INIT_STRUCT_CPTR);

extern pointer   _bsp_get_dspi_base_address(uint_8);
extern uint_32   _bsp_get_dspi_vectors(uint_32 dev_num, const uint_32 _PTR_ _PTR_ vectors_ptr);
extern boolean   _bsp_dspi_enable_access(uint_32 device);

#ifdef __cplusplus
}
#endif

#endif
