
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
* $FileName: crc_demo.c$
* $Version : 3.8.1.3$
* $Date    : Apr-25-2012$
*
* Comments:
*
*   The file contains definitions used in user program and/or in other
*   kernel modules to access CRC module
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <crc_kn.h>

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if ! BSPCFG_ENABLE_CRC
#error This application requires BSPCFG_ENABLE_CRC defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

/* Task IDs */
#define CRC_TASK 5
#define MAX_CHARS   256

extern void CRC_main(uint_32);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,   Function,   Stack,  Priority,     Name,     Attributes,          Param, Time Slice */
    { CRC_TASK,   CRC_main,      1500,   5,        "CRC_demo",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

uint_8  strMsg[MAX_CHARS];
uint_8  isCRC16 = 1;
uint_32 polyTab[4] = 
{
      0x1021,
      0x8408,
      0x8005,
      0x04C11DB7L  
};

/* Function Prototypes */
static uint_32  Input_Message(uint_8 *MsgBuf);
static uint_32  ConvertASCII2Word(uint_8 *s4ASCII);
static uint_32  Input_Word(void);

/*TASK*-------------------------------------------------------------------
* 
* Task Name : CRC_main
* Comments  :
*     Demo application for CRC driver
*END*----------------------------------------------------------------------*/
void CRC_main
(
   uint_32 initial_data
)
{
  uint_32  crc_error=0;
  uint_32  poly;
  uint_32  tot;
  uint_32  totr;
  uint_32  fxor;
  uint_32  tcrc;  
  uint_8   gbQuit = 0;
  uint_32  bByteCount;
  uint_8   ch;
  uint_32  seed,crc;
  uint_8   mode = 1;
 
  isCRC16 = 1;
  printf("\n====== CRC Lab ==========\r\n"); 
  do
  {
     if (mode == 1)
     {
         mode = 0;
         printf("\nPlease select CRC width (16-bit/32-bit):\r\n");
         printf("\n1. CRC16\n");
         printf("\n2. CRC32\n");
         printf("\n select:");
         do
         {
           ch = fgetc(stdin);
         }while ((ch != '1') && (ch != '2'));
         printf("%c\r\n",ch);
         isCRC16 = !(ch-'1');
         
         printf("\nPlease select CRC polynomial:\r\n");
         printf("\n1. poly = 0x1021 (CRC-CCITT)\r\n");
         printf("\n2. poly = 0x8408 (XMODEM)\r\n");
         printf("\n3. poly = 0x8005 (ARC)\r\n");
         printf("\n4. poly = 0x04C11DB7 (CRC32) \r\n");
         printf("\n5. others\r\n");
         printf("\nselect:");
         do
         {
           ch = fgetc(stdin); 
         }while(!((ch < '6') && (ch > '0')));
         
         printf("%c\r\n",ch);
         if(ch == '5')
         {
           printf("Please enter a polynomial in hex format:"); 
           poly = Input_Word();
         }
         else
         {
           poly = polyTab[ch-'1'];
         }
         tcrc = (isCRC16)? 0: 1;  
         
         printf("\nPlease select type of Transpose for input:\r\n");
         printf("\n1. No Transposition\r\n");
         printf("\n2. Only transpose bits in a byte\r\n");
         printf("\n3. Transpose both bits and bytes\r\n");
         printf("\n4. Only transpose bytes\r\n");
         printf("\nselect:");
         do
         {
           ch = fgetc(stdin);
         }while (!((ch < '5') && (ch > '0')));
         printf("ch = %c \r\n",ch);
         tot = ch - '1';
         
         printf("\nPlease select type of Transpose for Read:\r\n");
         printf("\n1. No Transposition\r\n");
         printf("\n2. Only transpose bits in a byte\r\n");
         printf("\n3. Transpose both bits and bytes\r\n");
         printf("\n4. Only transpose bytes\r\n");
         printf("\nselect:");
         do
         {
           ch = fgetc(stdin);
         }while (!((ch < '5') && (ch > '0')));
         printf("ch = %c\r\n",ch);
         totr = ch - '1';
         
         printf("XOR final checksum (y/n)?");
         do
         {
           ch = fgetc(stdin);
         }while ((ch != 'y') && (ch != 'n'));     
         printf("ch = %c\r\n",ch);
         fxor = (ch == 'y')? 1: 0;
         
         printf("\r\nPlease enter seed in hex:\r\n");
         seed = Input_Word();
      }
      printf("\nPlease enter an ASCII Message:");
      bByteCount = Input_Message(&strMsg[0]);
      crc_error=CRC_Config(poly,tot,totr,fxor,tcrc);  
      if(isCRC16)
      {
        crc = CRC_Cal_16(seed, &strMsg[0], bByteCount);
        printf("\n CRC result = %#04.4x\r\n",(crc & 0x0000FFFFL));
      }
      else
      {
        crc = CRC_Cal_32(seed, &strMsg[0], bByteCount);
        printf("\n CRC result = %#08.8x\r\n",crc);        
      }
      printf("\nPress any key to enter new data...,except 'q' to quit, 'c' to re-configuration !\r\n");
      do
      {
        ch = fgetc(stdin);
      }while( !ch );
      if(ch == 'q')
      {
        gbQuit = 1;
      }           
      if(ch == 'c')
      {
        mode = 1;
      }           
   }while(!gbQuit);
    
  gbQuit = 0;
  printf("\nDemo is quited!\r\n");
}

/*FUNCTION****************************************************************
* 
* Function Name    : Input_Message
* Returned Value   : uint_32
* Comments         : 
*   get a message from terminal. The message is terminated by ENTER key.
*   The maximum number of characters in the message is defined by 
*   MAX_CHARS which is 256.
*
*END*********************************************************************/
uint_32  Input_Message
(
   /* [OUT] The array of input characters */
   uint_8 *MsgBuf
)
{
  uint_32 ch;
  uint_32 count;
  
  count = 0;
  do
  {
    ch = fgetc(stdin);
    if((ch == 10) || (ch == 27))
    {     
      break;
    }
    MsgBuf[count++] = ch; 
    if(count >= MAX_CHARS)
    {
      break;
    }
  }while(1);
  MsgBuf[count]= '\0';
  printf("Input data = %s",MsgBuf);
    
  return (count);
}

/*FUNCTION****************************************************************
* 
* Function Name    : Input_Message
* Returned Value   : uint_32
* Comments         : 
*   get a word in hex from Terminal. word width is 4 digits. 
*
*END*********************************************************************/
uint_32  Input_Word
(
   void
)
{
  uint_32  ch;
  uint_8  count;
  uint_8  buf[8];
  uint_32 w;
  uint_32 i;
  
  count = 0; w = 0;
  for(i= 0;i< 8;i++)
  {
    buf[i] = 0;
  }
  do
  {
    ch = fgetc(stdin);
    if ((ch == 10)||(ch == 27)) /*enter character*/
    {     
      break;
    }
    if( ((ch >= '0') && (ch <='9')) ||
        ((ch >= 'a') && (ch <='f')) ||
        ((ch >= 'A') && (ch <='F')))
    {
        buf[count++]  = ch;
    }
    if(count >= (isCRC16?4:8))
    {
      break;
    }
  }while(1);
  printf("\r\ninput word = %s",buf);
    
  w = ConvertASCII2Word(buf);
  
  return (w);  
}

/*FUNCTION****************************************************************
*                                                                         
* Function Name    : ConvertASCII2Word                                        
* Returned Value   : uint_32                                              
* Comments         :                                                      
*   Convert an ASCII string to an integer word. word width is 4 digits. 
*                                                                         
*END*********************************************************************/
uint_32  ConvertASCII2Word
(
   /* [IN] ASCII string input */
	uint_8 *s4ASCII
)
{
    uint_8   i;
    uint_32   ch;
    uint_32  w;
    w = 0;
    for(i = 0;i <= (isCRC16?3:7);i++)  
    {
        ch = s4ASCII[i];
        if(!ch)
        {
          break;
        }
        if((ch >= '0') && (ch <='9'))
        {
            ch -= '0';        
        } else if((ch >= 'a') && (ch <='f'))
        {
            ch = ch - 'a' + 10;
        }else if((ch >= 'A') && (ch <='F'))
        {
            ch = ch - 'A' + 10;
        }
        w <<= 4;
        w += ch;
    }
    return (w);
}
