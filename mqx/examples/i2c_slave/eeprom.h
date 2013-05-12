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
* $Version : 3.8.1.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains definitions for the I2C slave EEPROM example.
*
*END************************************************************************/

/* The I2C EEPROM parameters */
#define I2C_EEPROM_ADDRESS   0x50     /* I2C bus address of EEPROM */
#define I2C_EEPROM_SIZE      0x0100   /* Overall memory byte size */
#define I2C_EEPROM_PAGE_SIZE 0x10     /* Maximum bytes per write */

/* Global settings */
#define I2C_POLLED_MODE 1

#endif
/* EOF */
