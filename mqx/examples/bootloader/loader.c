/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: loader.c$
* $Version : 3.8.1.0$
* $Date    : Jul-28-2011$
*
* Comments:
*
*   This file is the parser image for boot loader. 
*
*END************************************************************************/
#include "mqx.h"
#include "bsp.h"
#include "fio.h"
#include "io.h"
#include "psptypes.h"
#include "bootloader.h"
#include <string.h>

/*****************************************************************************
 * Function predefinitions.
 *****************************************************************************/
 uint_32          parse_image_to_ram(uint_8* arr,uint_32 length);
 static uint_8    parse_array_s19   (uint_8* Array,uint_32 size_of_array,uint_8 *Line);
 static uint_8    parse_line_s19    (uint_8_ptr);
 static uint_8    parse_array_cw    (uint_8_ptr, uint_32, uint_8_ptr);
 static uint_8    parse_line_cw     (uint_8_ptr);
 static uint_8    get_hex_value     (uint_8 text);
 static uint_8    get_spair        (uint_8 *arr,uint_8 point);
 static uint_32   get_uint32      (uint_8* arr, uint_32 index);
 /*****************************************************************************
 * Global variables.
 *****************************************************************************/
 
uint_8 _s19_file_done;       /* parsing S19 flag */
uint_32 _s19_address;        /* address to flash */
uint_8 _bootloader_status;   /* status of loading process */
uint_8 _file_type;           /* type of image file */
uint_8 _line[260];           /* line buffer */
extern uint_32          _boot_addr;

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_image_to_ram
* Returned Value : 0 if successful, other value if error
* Comments       : parse and flash an array to flash memory
*     
*END*--------------------------------------------------------------------*/
uint_32 parse_image_to_ram
    (
        /* [IN] the array to parse */
        uint_8*     arr,
        /* [IN] data length of the array */
        uint_32     length
    )
{
    /* Body */
    uint_32 result;
    static  uint_32 bytes_written;  /* number of bytes was written to flash */
    uint_32 header;
    uchar_ptr dst, src;
    
    header = get_uint32(arr,0);

    /* Get image type */
    if(_file_type == UNKNOWN)
    {
        bytes_written = 0;
        /*  first four bytes is SP */
        
#if BSP_TWRMPC5125
       /* CodeWarrior 9.2 Raw binary file */
       _file_type = RAW_BINARY;
#else
       if (header ==(uint_32)(S19_RECORD_HEADER)) {
         /* S-Record file found */
         _file_type = S19_RECORD;
       } else {
          /* CodeWarrior binary file found */
          _file_type = CODE_WARRIOR_BINARY;
       }
#endif       
    } /* EndIf */

    /* Flash image */
    switch (_file_type)
    {
        case RAW_BINARY: 
            /* Raw binary file found*/
            /* the address to write to flash */
            dst =(uchar_ptr) (_boot_addr + bytes_written);
            src =(uchar_ptr) arr;
            /* if flash error , break the loop */
            bytes_written += length;
            /* DisableInterrupts */
            _int_disable();
            while(length) {
               *dst++ = *src++;
               length--;
            }        
            /* EnableInterrupts */
            _int_enable();
            break;
        case CODE_WARRIOR_BINARY:
            /* CodeWarrior binary file found */
            result = parse_array_cw(arr,length,_line);
            break;
        case S19_RECORD:
            /* S19 file found */
            result = parse_array_s19(arr,length,_line);
            break;
        default:
           result = PARSE_IMAGE_ERROR;
           break;
    } /* EndSwitch */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_array_s19
* Returned Value : 0 if successful, other value if error
* Comments       : Get full lines from an array and flash them
*     
*END*--------------------------------------------------------------------*/
static uint_8 parse_array_s19
    (   /* [IN] the array to parse */
        uint_8 *array,
        /* [IN] data length of the array */
        uint_32 size_of_array,
        /* [IN] a allocated buffer */
        uint_8 *line
    )
{
    /* Body */
    uint_8 result;
    uint_16 i,j;
    uint_8 c;
    static uint_8 t=0;
    static uint_8 curL, totalL; /* current Length, total Length */
    static uint_8 newline = 0;

    for (i=0 ; i<size_of_array;i++)
    {
        c = array[i];
        if(curL == 4) 
        {
            /* Get total length of current line */
            t = get_spair(line,2);
            totalL = (uint_8)((t+2)*2);
        } /* EndIf */
        if (curL>=totalL)                   /* Get full line */
        {
            result = parse_line_s19(line);   /* Parse and flash current line */
            for (j=0 ; j<totalL;j++)        /* Reset line and other variables */
            {
                line[j]=0xFF;
            } /* EndFor */
            curL=0;
            totalL=6;
            newline=0;
        } /* EndIf */
        if (newline==0)                     /* Check for newline */
        {
            if (c=='S') 
            {
                newline=1;
                line[curL]=c;
                curL++;
            } /* EndIf */
        }
        else
        {
            line[curL] = c;
            curL++;
        } /* EndIf */
    } /* EndFor */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_line_s19
* Returned Value : 0 if successful, other value if error
* Comments       : Parse and flash a full line
*     
*END*--------------------------------------------------------------------*/
static uint_8 parse_line_s19
    (
        /* [IN] the Line to parse */
        uint_8 *Line
    )
{
    /* Body */
    static uint_8 length;
    static uint_8 checksum;
    static uint_8 i, offset,temp,c_temp,j;
    static uint_8 type;
    static uint_8 data;
    static uint_8 cur_point; /* current pointer */
    uint_8 buffer_to_flash[256];

    c_temp=Line[0];
    if (c_temp!='S')
    {
        _bootloader_status = BOOTLOADER_S19_ERROR;
        return FLASH_IMAGE_ERROR;
    } /* EndIf */
    /* Get record length */
    cur_point = 2;
    length = get_spair(Line,cur_point);
    if(_s19_file_done) 
    {
        /* not a valid S19 file */
        _bootloader_status = BOOTLOADER_S19_ERROR;
        return FLASH_IMAGE_ERROR;
    } /* EndIf */
    cur_point--;
    checksum = length;
    type=Line[1];

     /* Take appropriate action */
     switch (type)
        {
            case '1':
            case '2':
            case '3':
                _s19_address = (uint_32) NULL;
                type -= '0';
                cur_point=4;
                for (i = 0; i <= type ; i++)
                {
                    /* Formulate address */
                    /* Address needs to be word aligned for successful flash program */
                    data = get_spair(Line,cur_point);
                    if(_s19_file_done) 
                    {       
                        /* not a valid S19 file */
                        _bootloader_status = BOOTLOADER_S19_ERROR;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */
                    _s19_address = (_s19_address << 8) | data;
                    /* Maintain 8-bit checksum */
                    checksum = (unsigned char)((data + checksum) & 0x00FF);
                    cur_point+=2;
                } /* EndFor */

                 /* 32-bit cores program flash in 32-bit words */
                 /* Therefore S19 address needs to be adjusted to be word aligned */
                 /* Pad beginning of buffer if address not word aligned */
                 offset = (uint_8) (_s19_address & 0x0003);
                 _s19_address = (uint_32) (_s19_address & 0xFFFFFFFC);
                 length += offset;
                 for (i = 0; i < offset; i++) 
                 {
                     buffer_to_flash[i] = 0xFF; 
                 } /* EndFor */
                 /* Get data and put into buffer */
                 for (i = offset; i < (length - 5); i++)
                 {
                     data=get_spair(Line,cur_point);
                     buffer_to_flash[i] =data;
                     cur_point+=2;
                     if(_s19_file_done) 
                     {
                         /* not a valid S19 file */
                         _bootloader_status = BOOTLOADER_S19_ERROR;
                         return FLASH_IMAGE_ERROR;
                     }
                 } /* EndFor */

                 /* Calculate checksum */
                 for (i = offset; i < (length - 5); i++)
                 {
                     checksum = (unsigned char)((buffer_to_flash[i] + checksum) & 0x00FF);
                 } /* EndFor */
                 /* Get checksum byte */
                 data = get_spair(Line,cur_point);
                 cur_point+=2;
                 if(_s19_file_done) 
                 {
                     /* not a valid S19 file */
                     _bootloader_status = BOOTLOADER_S19_ERROR;
                     return FLASH_IMAGE_ERROR;
                 } /* EndIf */

                 if (((data - ~checksum) & 0x00FF) != 0)
                 {
                     _bootloader_status = BOOTLOADER_S19_ERROR;
                     _s19_file_done = TRUE;
                     return FLASH_IMAGE_ERROR;
                 } /* EndIf */
                 /* For 32-bit cores Flash_Prog writes 32-bit words, not bytes */
                 /* if last 32-bit word in s-record is not complete, finish word */
                 if((i & 0x0003) != 0x0000) 
                 {
                     /* 32-bit word not complete */
                     buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                     buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                     buffer_to_flash[i++] = 0xFF;         /* pad end of word */
                 } /* EndIf */

                 /* NOTE: 8-bit core does not need to pad the end */
                 /* Write buffered data to Flash */
                  /* call flash program */
                  _int_disable();
                  memcpy((pointer)_s19_address,(pointer)buffer_to_flash,i);
                  _int_enable();
                break;
            case '7':
            case '8':
            case '9':
                _s19_address = (uint_32) NULL; 
                type = (unsigned char)(type - '0');
                type = (unsigned char)(10 - type);
                cur_point=4;
                checksum = length;
                /* Get Address */
                for (i = 0; i <= type ; i++)
                {
                	for(j=0;j<length-1;j++)
                	{	
	                    data = get_spair(Line,cur_point);
	                    if(_s19_file_done) 
	                    {
	                        /* not a valid S19 file */
	                        _bootloader_status = BOOTLOADER_S19_ERROR;
	                        return FLASH_IMAGE_ERROR;
	                    } /* EndIf */
						checksum = (unsigned char)((data + checksum) & 0x00FF);
						cur_point+=2;
                	} /* EndFor */

                    
                    /* Read checksum value */
                    data=get_spair(Line,cur_point);
                    if(_s19_file_done)
                    {
                        /* not a valid S19 file */
                        _bootloader_status = BOOTLOADER_S19_ERROR;
                        return FLASH_IMAGE_ERROR;
                    } /* EndIf */
                    
                    /* Check checksum */
                    if (((data - ~checksum) & 0x00FF) != 0)
                    {
                        _bootloader_status = BOOTLOADER_S19_ERROR;
                        _s19_file_done = TRUE;
                        return FLASH_IMAGE_ERROR;
                    }
                    else 
                    {
                        /* File completely read successfully */
                        _bootloader_status = BOOTLOADER_OK;
                        _s19_file_done = TRUE;
                        return FLASH_IMAGE_SUCCESS;
                    } /* EndIf */
                } /* EndFor */
                break;
            case '0':
            case '4':
            case '5':
            case '6':
            default:
            break;
        } /* EndSwitch */
    return FLASH_IMAGE_SUCCESS;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_hex_value
* Returned Value : unsigned char, hex value of character
* Comments       : Converts ASCII character to hex value 
*     
*END*--------------------------------------------------------------------*/
static uint_8 get_hex_value 
   (
     /* [IN] the text to parse */
     uint_8 text
   )
{ /* Body */
    switch (text)
    {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (uint_8)(text - '0');
        case 'A':
        case 'a':
            return 10;
        case 'B':
        case 'b':
            return 11;
        case 'C':
        case 'c':
            return 12;
        case 'D':
        case 'd':
            return 13;
        case 'E':
        case 'e':
            return 14;
        case 'F':
        case 'f':
            return 15;
        default:
            return 0xFF;
    } /* EndSwitch */
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_spair
* Returned Value : unsigned char, converted hex byte
* Comments       : Gets pair of characters, and converts to hex byte
*     
*END*--------------------------------------------------------------------*/
static uint_8 get_spair
   (
     /* [IN] the array to parse */
     uint_8 *arr,
     /* [IN] point to array data */
     uint_8 point
   )
{
    /* Body */
    uint_8 ch;
    uint_8 upper,lower;

    ch = arr[point];
    upper = (uint_8)(get_hex_value(ch));
    if(upper == 0xFF) 
    {
        /* Not a proper S19 file */
        _s19_file_done = TRUE;
    } /* EndIf */
    upper = (uint_8)(upper << 4);
    ch=arr[point+1];
    lower= (uint_8)(get_hex_value(ch));

    if(lower == 0xFF) 
    {
        /* Not a proper S19 file */
        _s19_file_done = TRUE;
    } /* EndIf */
    return (uint_8)(upper | lower);
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_array_cw
* Returned Value : 0 if successful, other value if error
* Comments       : Get full lines from an array and flash them
*     
*END*--------------------------------------------------------------------*/
static uint_8 parse_array_cw
   (
     /* [IN] the array to parse */
     uint_8 *array,
     /* [IN] data length of the array */
     uint_32 size_of_array,
     /* [IN] a allocated buffer */
     uint_8 *line
   )
{
    /* Body */
    uint_8 result;
    uint_16 i,j;
    uint_32 data_length;
    uint_8 c;
    static uint_32 curL,totalL;             /* current Length , taltal Length */
    static uint_8 newline = 0;

    for (i = 0 ; i<size_of_array;i++)
    {
        c = array[i];
        line[curL] = c;
        curL++;
        if(curL == 8) 
        {
            data_length = get_uint32(line,4);
            /* Get total length of current line */
            totalL = data_length + 8;
            /* start a new line with address and data length */
            newline = 1;
        } /* EndIf */
        if(newline)
        {
            if (curL >= totalL)                   /* Get full line */
            {
                /* Parse and flash current line */
                result = parse_line_cw(line);
                /* Reset line and other variables */
                for (j=0 ; j<totalL;j++)
                {
                    line[j]=0xFF;
                }
                curL=0;
                totalL=0;
                newline=0;
            } /* EndIf */
        } /* EndIf */
    } /* EndFor */
    return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : parse_line_cw
* Returned Value : 0 if successful, other value if error
* Comments       : Parse and flash a full line
*     
*END*--------------------------------------------------------------------*/
static uint_8 parse_line_cw
   (
     /* [IN] the line to parse */
     uint_8 *line
   )
{ /* Body */
   uint_8  result = FLASH_IMAGE_SUCCESS;
   uchar_ptr dst;
   uint_32   data_length;
   
   dst = (uchar_ptr)get_uint32(line,0);    /* address to flash */  
   data_length = get_uint32(line,4);       /* length of data */
   line += 8;   
   /* DisableInterrupts */
   _int_disable();
   memcpy((pointer)dst, (pointer)line,data_length);
   /* EnableInterrupts */;
   _int_enable();
   return result;
} /* EndBody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : get_uint32
* Returned Value : result
* Comments       : get a unsign long number from an array
*     
*END*--------------------------------------------------------------------*/
static uint_32 get_uint32 
   (
     /* [IN] the array */
     uint_8* arr,
     /* [IN] the index of array */
     uint_32 index
   )
{
    uint_32 result;
    result = *(uint_32*)(arr + index);
    return result ;
} /* EndBody */
/* EOF */
