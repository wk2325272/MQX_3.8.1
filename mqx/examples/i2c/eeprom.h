#ifndef __eeprom_h__
#define __eeprom_h__
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
* $FileName: eeprom.h$
* $Version : 3.8.5.2$
* $Date    : Feb-21-2012$
*
* Comments:
*
*   This file contains definitions for the I2C master example.
*
*END************************************************************************/

#if defined(BSP_MPC8308RDB)
#define I2C_EEPROM_PAGE_SIZE       0x40     /* Maximum bytes per write for M24256 */
#define I2C_EEPROM_MEMORY_WIDTH    0x02     /* Memory address width in bytes */
#elif defined(BSP_M53015EVB)
#define I2C_EEPROM_PAGE_SIZE       0x08     /* Maximum bytes per write for AT24C02B */
#define I2C_EEPROM_MEMORY_WIDTH    0x01     /* Memory address width in bytes */
#else
#define I2C_EEPROM_PAGE_SIZE       0x10     /* Maximum bytes per write for 24LC16 */
#define I2C_EEPROM_MEMORY_WIDTH    0x01     /* Memory address width in bytes */
#endif

/* The I2C EEPROM test addresses */
#define I2C_EEPROM_BUS_ADDRESS     0x50     /* I2C bus address of EEPROM */
#define I2C_EEPROM_MEMORY_ADDRESS1 0x1A     /* Memory test address 1 */
#define I2C_EEPROM_MEMORY_ADDRESS2 0x7B     /* Memory test address 2 */

/* Global variables */
extern LWSEM_STRUCT lock;

/* Function prototypes */
extern void i2c_write_eeprom_polled (MQX_FILE_PTR, uint_32, uchar_ptr, _mqx_int);
extern void i2c_read_eeprom_polled (MQX_FILE_PTR, uint_32, uchar_ptr, _mqx_int);
extern void i2c_write_eeprom_interrupt (MQX_FILE_PTR, uint_32, uchar_ptr, _mqx_int);
extern void i2c_read_eeprom_interrupt (MQX_FILE_PTR, uint_32, uchar_ptr, _mqx_int);

#endif
/* EOF */
