#ifndef __esdhc_h__
#define __esdhc_h__
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
* $FileName: esdhc.h$
* $Version : 3.7.5.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the definitions of constants and structures
*   required for the eSDHC driver
*
*END************************************************************************/


/*--------------------------------------------------------------------------*/
/*
**                    CONSTANT DEFINITIONS
*/

/* 
** IOCTL calls specific to eSDHC
*/
#define IO_IOCTL_ESDHC_INIT                  _IO(IO_TYPE_ESDHC,0x01)
#define IO_IOCTL_ESDHC_SEND_COMMAND          _IO(IO_TYPE_ESDHC,0x02)
#define IO_IOCTL_ESDHC_GET_CARD              _IO(IO_TYPE_ESDHC,0x03)
#define IO_IOCTL_ESDHC_GET_BAUDRATE          _IO(IO_TYPE_ESDHC,0x04)
#define IO_IOCTL_ESDHC_SET_BAUDRATE          _IO(IO_TYPE_ESDHC,0x05)
#define IO_IOCTL_ESDHC_GET_BUS_WIDTH         _IO(IO_TYPE_ESDHC,0x06)
#define IO_IOCTL_ESDHC_SET_BUS_WIDTH         _IO(IO_TYPE_ESDHC,0x07)
#define IO_IOCTL_ESDHC_GET_BLOCK_SIZE        _IO(IO_TYPE_ESDHC,0x08)
#define IO_IOCTL_ESDHC_SET_BLOCK_SIZE        _IO(IO_TYPE_ESDHC,0x09)

/* ESDHC error codes */
#define ESDHC_OK                             (0x00)
#define ESDHC_ERROR_INIT_FAILED              (ESDHC_ERROR_BASE | 0x01)
#define ESDHC_ERROR_COMMAND_FAILED           (ESDHC_ERROR_BASE | 0x02)
#define ESDHC_ERROR_COMMAND_TIMEOUT          (ESDHC_ERROR_BASE | 0x03)
#define ESDHC_ERROR_DATA_TRANSFER            (ESDHC_ERROR_BASE | 0x04)
#define ESDHC_ERROR_INVALID_BUS_WIDTH        (ESDHC_ERROR_BASE | 0x05)

/* ESDHC bus widths */
#define ESDHC_BUS_WIDTH_1BIT                 (0x00)
#define ESDHC_BUS_WIDTH_4BIT                 (0x01)
#define ESDHC_BUS_WIDTH_8BIT                 (0x02)

/* ESDHC card types */
#define ESDHC_CARD_NONE                      (0x00)
#define ESDHC_CARD_UNKNOWN                   (0x01)
#define ESDHC_CARD_SD                        (0x02)
#define ESDHC_CARD_SDHC                      (0x03)
#define ESDHC_CARD_SDIO                      (0x04)
#define ESDHC_CARD_SDCOMBO                   (0x05)
#define ESDHC_CARD_SDHCCOMBO                 (0x06)
#define ESDHC_CARD_MMC                       (0x07)
#define ESDHC_CARD_CEATA                     (0x08)

/* ESDHC command types */
#define ESDHC_TYPE_NORMAL                    (0x00)
#define ESDHC_TYPE_SUSPEND                   (0x01)
#define ESDHC_TYPE_RESUME                    (0x02)
#define ESDHC_TYPE_ABORT                     (0x03)
#define ESDHC_TYPE_SWITCH_BUSY               (0x04)

/* ESDHC commands */
#define ESDHC_CMD0                           (0)
#define ESDHC_CMD1                           (1)
#define ESDHC_CMD2                           (2)
#define ESDHC_CMD3                           (3)
#define ESDHC_CMD4                           (4)
#define ESDHC_CMD5                           (5)
#define ESDHC_CMD6                           (6)
#define ESDHC_CMD7                           (7)
#define ESDHC_CMD8                           (8)
#define ESDHC_CMD9                           (9)
#define ESDHC_CMD10                          (10)
#define ESDHC_CMD11                          (11)
#define ESDHC_CMD12                          (12)
#define ESDHC_CMD13                          (13)
#define ESDHC_CMD15                          (15)
#define ESDHC_CMD16                          (16)
#define ESDHC_CMD17                          (17)
#define ESDHC_CMD18                          (18)
#define ESDHC_CMD20                          (20)
#define ESDHC_CMD24                          (24)
#define ESDHC_CMD25                          (25)
#define ESDHC_CMD26                          (26)
#define ESDHC_CMD27                          (27)
#define ESDHC_CMD28                          (28)
#define ESDHC_CMD29                          (29)
#define ESDHC_CMD30                          (30)
#define ESDHC_CMD32                          (32)
#define ESDHC_CMD33                          (33)
#define ESDHC_CMD34                          (34)
#define ESDHC_CMD35                          (35)
#define ESDHC_CMD36                          (36)
#define ESDHC_CMD37                          (37)
#define ESDHC_CMD38                          (38)
#define ESDHC_CMD39                          (39)
#define ESDHC_CMD40                          (40)
#define ESDHC_CMD42                          (42)
#define ESDHC_CMD52                          (52)
#define ESDHC_CMD53                          (53)
#define ESDHC_CMD55                          (55)
#define ESDHC_CMD56                          (56)
#define ESDHC_CMD60                          (60)
#define ESDHC_CMD61                          (61)
#define ESDHC_ACMD6                          (0x40 + 6)
#define ESDHC_ACMD13                         (0x40 + 13)
#define ESDHC_ACMD22                         (0x40 + 22)
#define ESDHC_ACMD23                         (0x40 + 23)
#define ESDHC_ACMD41                         (0x40 + 41)
#define ESDHC_ACMD42                         (0x40 + 42)
#define ESDHC_ACMD51                         (0x40 + 51)

/*--------------------------------------------------------------------------*/
/*
**                    DATATYPE DECLARATIONS
*/

typedef struct esdhc_command_struct
{
    uint_8  COMMAND;
    uint_8  TYPE;
    uint_32 ARGUMENT;
    boolean READ;
    uint_32 BLOCKS;
    uint_32 RESPONSE[4];
    
} ESDHC_COMMAND_STRUCT, _PTR_ ESDHC_COMMAND_STRUCT_PTR;



/*
** ESDHC_INIT_STRUCT
**
** This structure defines the initialization parameters to be used
** when a esdhc driver is initialized.
*/
typedef struct esdhc_init_struct
{
    /* The device number */
    uint_32 CHANNEL;

    /* The communication baud rate */
    uint_32 BAUD_RATE;

    /* The module input clock */
    uint_32 CLOCK_SPEED;
   
} ESDHC_INIT_STRUCT, _PTR_ ESDHC_INIT_STRUCT_PTR;

typedef const ESDHC_INIT_STRUCT _PTR_ ESDHC_INIT_STRUCT_CPTR;

/*--------------------------------------------------------------------------*/
/* 
**                        FUNCTION PROTOTYPES
*/

#ifdef __cplusplus
extern "C" {
#endif


extern pointer _bsp_get_esdhc_base_address(uint_8);
extern uint_32 _bsp_get_esdhc_vector(uint_8);
extern _mqx_int _esdhc_install(char_ptr,ESDHC_INIT_STRUCT_CPTR);


#ifdef __cplusplus
}
#endif


#endif 

/* EOF */
