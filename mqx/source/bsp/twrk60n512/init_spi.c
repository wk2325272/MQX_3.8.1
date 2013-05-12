/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: init_spi.c$
* $Version : 3.8.5.0$
* $Date    : Sep-22-2011$
*
* Comments:
*
*   This file contains the initialization definition for DSPI driver
*
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>


/*
** parameters of SPI initialization
*/

const DSPI_INIT_STRUCT _bsp_dspi0_init = { 
   0,                            /* SPI channel                     */
   SPI_PUSHR_PCS(1 << 0),        /* Chip Select 0                   */
   SPI_DEVICE_MASTER_MODE,       /* Transfer mode                   */  
   BSP_DSPI_BAUDRATE,            /* SPI Baud rate register value    */ 
   BSP_BUS_CLOCK,                /* Relevant bus clock              */         
   SPI_CLK_POL_PHA_MODE0,        /* SPI clock phase                 */   
   BSP_DSPI_RX_BUFFER_SIZE,      /* Rx Buffer Size (interrupt only) */ 
   BSP_DSPI_TX_BUFFER_SIZE,      /* Tx Buffer Size (interrupt only) */
   8,                            /* Rx DMA channel (interrupt only) */
   16,                           /* Rx eDMA source (interrupt only) */
   9,                            /* Tx DMA channel (interrupt only) */
   17                            /* Tx eDMA source (interrupt only) */
};

const DSPI_INIT_STRUCT _bsp_dspi1_init = { 
   1,                            /* SPI channel                     */
   SPI_PUSHR_PCS(1 << 0),        /* Chip Select 0                   */
   SPI_DEVICE_MASTER_MODE,       /* Transfer mode                   */  
   BSP_DSPI_BAUDRATE,            /* SPI Baud rate register value    */ 
   BSP_BUS_CLOCK,                /* Relevant bus clock              */         
   SPI_CLK_POL_PHA_MODE0,        /* SPI clock phase                 */   
   BSP_DSPI_RX_BUFFER_SIZE,      /* Rx Buffer Size (interrupt only) */ 
   BSP_DSPI_TX_BUFFER_SIZE,      /* Tx Buffer Size (interrupt only) */ 
   10,                           /* Rx DMA channel (interrupt only) */
   18,                           /* Rx eDMA source (interrupt only) */
   11,                           /* Tx DMA channel (interrupt only) */
   19                            /* Tx eDMA source (interrupt only) */
};

const DSPI_INIT_STRUCT _bsp_dspi2_init = { 
   2,                            /* SPI channel                     */
   SPI_PUSHR_PCS(1 << 0),        /* Chip Select 0                   */
   SPI_DEVICE_MASTER_MODE,       /* Transfer mode                   */  
   BSP_DSPI_BAUDRATE,            /* SPI Baud rate register value    */ 
   BSP_BUS_CLOCK,                /* Relevant bus clock              */         
   SPI_CLK_POL_PHA_MODE0,        /* SPI clock phase                 */   
   BSP_DSPI_RX_BUFFER_SIZE,      /* Rx Buffer Size (interrupt only) */ 
   BSP_DSPI_TX_BUFFER_SIZE,      /* Tx Buffer Size (interrupt only) */ 
   12,                           /* Rx DMA channel (interrupt only) */
   20,                           /* Rx eDMA source (interrupt only) */
   13,                           /* Tx DMA channel (interrupt only) */
   21                            /* Tx eDMA source (interrupt only) */
};
