/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: bootloader.h$
* $Version : 3.8.5.2$
* $Date    : Feb-13-2012$
*
* Comments: 
*
*   The Common Configurable information for Nand Flash boot loader
*   
*
*END***********************************************************************/
#ifndef _nandbootloader_h_
#define _nandbootloader_h_

#include "config.h"

#if BSP_TWRMPC5125 || BSP_TWRMCF54418 || BSP_M5329EVB
   #define BOOTLOADER_NAND_FLASH_SUPPORT 1
#elif BSP_M54455EVB
   #define FLASH_NAME_INIT              "flashx1:"
   #define BOOTLOADER_NOR_FLASH_SUPPORT 1
#elif BSP_M53015EVB
   #define FLASH_NAME_INIT              "flash:"   
   #define BOOTLOADER_NOR_FLASH_SUPPORT 1
#endif

#if BOOTLOADER_NAND_FLASH_SUPPORT
   #define FLASH_NAME_INIT              "nandflash:"
   #define IMAGE_START_BLOCK            (300)
   #define NAND_FLASH_PAGE_SIZE         BSP_VIRTUAL_PAGE_SIZE
   #define NAND_FLASH_BLOCK_SIZE        BSP_NAND_BLOCK_SIZE
   #define NAND_FLASH_NUM_BLOCKS        BSP_NAND_NUM_BLOCKS
   #define IMAGE_TABLE_BLOCK_LOCATION   (NAND_FLASH_NUM_BLOCKS-1)
   #define IMAGE_TABLE_LOCATION         (IMAGE_TABLE_BLOCK_LOCATION * (NAND_FLASH_BLOCK_SIZE/NAND_FLASH_PAGE_SIZE))
   #define IMAGE_TABLE_SIZE             (1)              /* size in page */
   #define RW_UNIT_SIZE                 (1)
#elif BOOTLOADER_NOR_FLASH_SUPPORT
   #define IMAGE_START_SECTOR           (30)
   #define IMAGE_TABLE_LOCATION         (0x00100000)
   #define IMAGE_TABLE_SIZE             (512)
   #define RW_UNIT_SIZE                 (512)
   #define NAND_FLASH_PAGE_SIZE         (512)
#endif

#define IMAGE_VALID                    (0xFFFF1010)
#define IMAGE_DELETED                  (0x01 << 0)
#define IMAGE_END_TABLE                (0x01 << 1)

#define AUTOBOOT_TIMEOUT               (5)
#define RESET_MAGIC_NUMBER             (0x52535445)

/* Error code */
#define BOOTLOADER_OK                  (0)
#define BOOTLOADER_ERROR_BASE          (0x80)
#define FLASH_IMAGE_SUCCESS            (0)
#define FLASH_ERASE_UNIT_FAILED        (BOOTLOADER_ERROR_BASE | 0x01)
#define CHECK_SUM_IMAGE_FAILED         (BOOTLOADER_ERROR_BASE | 0x02)
#define INVALID_PARAMETER              (BOOTLOADER_ERROR_BASE | 0x03)
#define WRITE_TABLE_FAILED             (BOOTLOADER_ERROR_BASE | 0x04)
#define WRITE_DATA_FAILED              (BOOTLOADER_ERROR_BASE | 0x05)
#define BOOTLOADER_OUT_OF_MEMORY       (BOOTLOADER_ERROR_BASE | 0x06)
#define PARSE_IMAGE_ERROR              (BOOTLOADER_ERROR_BASE | 0x07)
#define BOOTLOADER_S19_ERROR           (BOOTLOADER_ERROR_BASE | 0x08)
#define FLASH_IMAGE_ERROR              (BOOTLOADER_ERROR_BASE | 0x09)

/* File types */
#define UNKNOWN                        (0)
#define RAW_BINARY                     (1)
#define CODE_WARRIOR_BINARY            (2)
#define S19_RECORD                     (3)
#define S19_RECORD_HEADER              (0x53303033)

/* Image commands */
#define IMAGE_GET_TIME_OUT             (1)
#define IMAGE_SET_TIME_OUT             (2)
#define IMAGE_GET_NAME                 (3)
#define IMAGE_SET_NAME                 (4)
#define IMAGE_SET_BOOT_ADDR            (5)
#define IMAGE_GET_BOOT_ADDR            (6)
#define IMAGE_SET_AUTO_BOOT            (7)
#define IMAGE_GET_AUTO_BOOT            (8)

#define AUTOBOOT_ENABLE                (1)
#define AUTOBOOT_DISABLE               (2)
#define AUTOBOOT_RESET                 (3)

#define IMAGE_NAME_LENGTH              (20)

typedef struct image_info {
   /* Validating number of image */
   uint_32 VALID;
   /* Table index number */
   uint_32 INDEX;
   /* Boot address should be found in liker map file image */
   uint_32 BOOT_ADDR;
   /* Data size of image */
   uint_32 SIZE;
   /* Image check sum number */
   uint_32 CHECK_SUM;
   /* Block(Sector) location in NAND(NOR) flash support */
   uint_32 BLOCK;
   /* Data size in block (sector) if NAND(NOR) support*/
   uint_32 BLOCK_NUM;
   /* Image property */
   uint_32 PROPERTY;
   /* Auto boot flag */
   boolean AUTO;
   /* Name of image */
   uchar   NAME[IMAGE_NAME_LENGTH];
   /* Auto boot time out in second */
   uint_32 BOOT_TIMEOUT;
} IMAGE_INFO, _PTR_ IMAGE_INFO_PTR;

int_32 _bootloader_init_table(MQX_FILE_PTR);
uint_32 _bootloader_del_image(MQX_FILE_PTR, uint_32);
void _bootloader_list_image(MQX_FILE_PTR);
int_32 _bootloader_set_default(uint_32);
void _bootloader_reset(void);
int_32 _bootloader_get_autoboot(MQX_FILE_PTR, uint_32_ptr, uint_32_ptr);
int_32 _bootloader_del_table(MQX_FILE_PTR);
int_32 _bootloader_set_timeout(uint_32, uint_32);
int_32 _bootloader_check_available (MQX_FILE_PTR, uint_32_ptr, uint_32_ptr);
int_32 _bootloader_update_table(MQX_FILE_PTR,uint_32, uint_32,uint_32,uint_32,uchar_ptr);
int_32 _bootloader_load(  MQX_FILE_PTR, uint_32);
int_32 _bootloader_imgctl ( MQX_FILE_PTR, uint_32, uint_32, pointer);
int_32 _bootloader_check_bad_block (MQX_FILE_PTR);
uint_32  _bootloader_store_image_data (MQX_FILE_PTR, uchar_ptr, uint_32, boolean, uint_32, uchar_ptr, boolean);
#endif

/* EOF */
