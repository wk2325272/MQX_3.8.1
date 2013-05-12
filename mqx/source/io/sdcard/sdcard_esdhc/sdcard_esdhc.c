/**HEADER********************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: sdcard_esdhc.c$
* $Version : 3.8.8.4$
* $Date    : May-31-2012$
*
* Comments:
*
*   This file contains the SD card driver functions.
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <io_prv.h>
#include <sdcard.h>
#include <sdcard_prv.h>
#include <sdcard_esdhc.h>
#include <esdhc.h>


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _io_sdcard_esdhc_r2_format
* Returned Value   :
* Comments         :
*    Reformats R2 (CID,CSD) as read from ESDHC registers to a byte array.
*
*END*----------------------------------------------------------------------*/

static void _io_sdcard_esdhc_r2_format(uint_32 r[4], uint_8 r2[16])
{
    int i;
    uint_32 tmp;

    i=15;
    while (i)
    {
        /* initialize tpm at the first loop */
        if ((i%4) == 3)
        {
            tmp = r[3-i/4];
        }
        r2[--i] = tmp & 0xff;
        tmp >>= 8;
    }
    r2[15] = 0;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _io_sdcard_esdhc_init
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*    Initializes ESDHC communication, SD card itself and reads its parameters.
*
*END*----------------------------------------------------------------------*/

boolean _io_sdcard_esdhc_init
(
    /* [IN/OUT] SD card file descriptor */
    MQX_FILE_PTR fd_ptr
)
{
    uint_32              baudrate, param, c_size, c_size_mult, read_bl_len;
    ESDHC_COMMAND_STRUCT command;
    IO_DEVICE_STRUCT_PTR io_dev_ptr = fd_ptr->DEV_PTR;
    SDCARD_STRUCT_PTR    sdcard_ptr = (SDCARD_STRUCT_PTR)io_dev_ptr->DRIVER_INIT_PTR;
    uint_8               buffer[64];
    uint_8               csd[16];

    /* Check parameters */
    if ((NULL == sdcard_ptr) || (NULL == sdcard_ptr->COM_DEVICE) || (NULL == sdcard_ptr->INIT))
    {
        return FALSE;
    }

    sdcard_ptr->TIMEOUT = 0;
    sdcard_ptr->NUM_BLOCKS = 0;
    sdcard_ptr->ADDRESS = 0;
    sdcard_ptr->SDHC = FALSE;
    sdcard_ptr->VERSION2 = FALSE;

    /* Initialize and detect card */
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_INIT, NULL))
    {
        return FALSE;
    }

    /* Set low baudrate for card setup */
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_GET_BAUDRATE, &baudrate))
    {
        return FALSE;
    }
    param = 400000;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SET_BAUDRATE, &param))
    {
        return FALSE;
    }

    /* SDHC check */
    param = 0;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_GET_CARD, &param))
    {
        return FALSE;
    }
    if ((ESDHC_CARD_SD == param) || (ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDCOMBO == param) || (ESDHC_CARD_SDHCCOMBO == param))
    {
        if ((ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDHCCOMBO == param))
        {
            sdcard_ptr->SDHC = TRUE;
        }
    }
    else
    {
        return FALSE;
    }

    /* Card identify */
    command.COMMAND = ESDHC_CMD2;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    /* Get card address */
    command.COMMAND = ESDHC_CMD3;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }
    sdcard_ptr->ADDRESS = command.RESPONSE[0] & 0xFFFF0000;

    /* Get card parameters */
    command.COMMAND = ESDHC_CMD9;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = sdcard_ptr->ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    _io_sdcard_esdhc_r2_format(command.RESPONSE, csd);
    sdcard_ptr->NUM_BLOCKS = _io_sdcard_csd_capacity(csd);

    param = _io_sdcard_csd_baudrate(csd);
    if (param > baudrate)
    {
        param = baudrate;
    }
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SET_BAUDRATE, &param))
    {
        return FALSE;
    }

    /* Select card */
    command.COMMAND = ESDHC_CMD7;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = sdcard_ptr->ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    /* Set block size to 512 */
    param = IO_SDCARD_BLOCK_SIZE;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SET_BLOCK_SIZE, &param))
    {
        return FALSE;
    }
    command.COMMAND = ESDHC_CMD16;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = param;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    if (ESDHC_BUS_WIDTH_4BIT == sdcard_ptr->INIT->SIGNALS)
    {
        /* Application specific command */
        command.COMMAND = ESDHC_CMD55;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            return FALSE;
        }

        /* Set bus width == 4 */
        command.COMMAND = ESDHC_ACMD6;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 2;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            return FALSE;
        }

        param = ESDHC_BUS_WIDTH_4BIT;
        if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SET_BUS_WIDTH, &param))
        {
            return FALSE;
        }
    }

    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _io_sdcard_esdhc_read_single_block
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*    Reads sector (512 byte) from SD card with given index into given buffer.
*
*END*----------------------------------------------------------------------*/

static boolean _io_sdcard_esdhc_read_single_block
(
    /* [IN] SD card info */
    MQX_FILE_PTR fd_ptr,

    /* [OUT] Buffer to fill with read 512 bytes */
    uchar_ptr buffer,

    /* [IN] Index of sector to read */
    uint_32   index
)
{
    ESDHC_COMMAND_STRUCT command;
    IO_DEVICE_STRUCT_PTR io_dev_ptr = fd_ptr->DEV_PTR;
    SDCARD_STRUCT_PTR    sdcard_ptr = (SDCARD_STRUCT_PTR)io_dev_ptr->DRIVER_INIT_PTR;

    /* Check parameters */
    if ((NULL == sdcard_ptr) || (NULL == sdcard_ptr->COM_DEVICE) || (NULL == sdcard_ptr->INIT) || (NULL == buffer))
    {
        return FALSE;
    }

    /* SD card data address adjustment */
    if (! sdcard_ptr->SDHC)
    {
        index <<= IO_SDCARD_BLOCK_SIZE_POWER;
    }

    /* Read block command */
    command.COMMAND = ESDHC_CMD17;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = index;
    command.READ = TRUE;
    command.BLOCKS = 1;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    /* Read data */
    if (IO_SDCARD_BLOCK_SIZE != fread (buffer, 1, IO_SDCARD_BLOCK_SIZE, sdcard_ptr->COM_DEVICE))
    {
        return FALSE;
    }

    /* Wait for transfer complete */
    if (ESDHC_OK != fflush (sdcard_ptr->COM_DEVICE))
    {
        return FALSE;
    }

    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _io_sdcard_esdhc_read_block
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*    Reads sectors from SD card starting with given index into given buffer.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_sdcard_esdhc_read_block
(
    /* [IN] SD card info */
    MQX_FILE_PTR fd_ptr,

    /* [OUT] Buffer to fill with read 512 bytes */
    uchar_ptr buffer,

    /* [IN] Index of sector to read */
    uint_32   index,

    /* [IN] Number of sectors to be written */
    uint_32   num
)
{
    uint_32   i;

    /* 2do: support for multi-sector transfer to improve performance */
    for (i = 0; i < num; i++)
    {
        if (!_io_sdcard_esdhc_read_single_block(fd_ptr, buffer, index)) break;
        buffer += IO_SDCARD_BLOCK_SIZE;
        index++;
    }

    return i;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _io_sdcard_esdhc_write_single_block
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*    Writes sector (512 byte) with given index to SD card from given buffer.
*
*END*----------------------------------------------------------------------*/

static boolean _io_sdcard_esdhc_write_single_block
(
    /* [IN] SD card file descriptor */
    MQX_FILE_PTR fd_ptr,

    /* [IN] Buffer with 512 bytes to write */
    uchar_ptr buffer,

    /* [IN] Index of sector to write */
    uint_32   index
)
{
    ESDHC_COMMAND_STRUCT command;
    IO_DEVICE_STRUCT_PTR io_dev_ptr = fd_ptr->DEV_PTR;
    SDCARD_STRUCT_PTR    sdcard_ptr = (SDCARD_STRUCT_PTR)io_dev_ptr->DRIVER_INIT_PTR;

    /* Check parameters */
    if ((NULL == sdcard_ptr) || (NULL == sdcard_ptr->COM_DEVICE) || (NULL == sdcard_ptr->INIT) || (NULL == buffer))
    {
        return FALSE;
    }

    /* SD card data address adjustment */
    if (! sdcard_ptr->SDHC)
    {
        index <<= IO_SDCARD_BLOCK_SIZE_POWER;
    }

    /* Write block command */
    command.COMMAND = ESDHC_CMD24;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = index;
    command.READ = FALSE;
    command.BLOCKS = 1;
    if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
    {
        return FALSE;
    }

    /* Write data */
    if (IO_SDCARD_BLOCK_SIZE != fwrite (buffer, 1, IO_SDCARD_BLOCK_SIZE, sdcard_ptr->COM_DEVICE))
    {
        return FALSE;
    }

    /* Wait for transfer complete */
    if (ESDHC_OK != fflush (sdcard_ptr->COM_DEVICE))
    {
        return FALSE;
    }

    /* Wait for card ready / transaction state */
    do
    {
        command.COMMAND = ESDHC_CMD13;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sdcard_ptr->ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_OK != ioctl (sdcard_ptr->COM_DEVICE, IO_IOCTL_ESDHC_SEND_COMMAND, &command))
        {
            return FALSE;
        }

        /* Card status error check */
        if (command.RESPONSE[0] & 0xFFD98008)
        {
            return FALSE;
        }
    } while (0x000000900 != (command.RESPONSE[0] & 0x00001F00));


    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _io_sdcard_esdhc_write_block
* Returned Value   : TRUE if successful, FALSE otherwise
* Comments         :
*    Writes sectors starting with given index to SD card from given buffer.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_sdcard_esdhc_write_block
(
    /* [IN] SD card file descriptor */
    MQX_FILE_PTR fd_ptr,

    /* [IN] Buffer with data to write */
    uchar_ptr buffer,

    /* [IN] Index of first sector to write */
    uint_32   index,

    /* [IN] Number of sectors to be written */
    uint_32   num
)
{
    uint_32   i;

    /* 2do: support for multi-sector transfer to improve performance */
    for (i = 0; i < num; i++)
    {
        if (!_io_sdcard_esdhc_write_single_block(fd_ptr, buffer, index)) break;
        buffer += IO_SDCARD_BLOCK_SIZE;
        index++;
    }

    return i;
}

/* EOF */
