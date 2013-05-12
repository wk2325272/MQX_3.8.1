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
* $FileName: spi.c$
* $Version : 3.8.36.2$
* $Date    : Feb-21-2012$
*
* Comments:
*
*   This file contains the source for a simple example of an
*   application that writes and reads the SPI memory using the SPI driver.
*   It's already configured for onboard SPI flash where available.
*
*END************************************************************************/


#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <spi.h>
#include "memory.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#ifndef BSP_SPI_MEMORY_CHANNEL
#error This application requires BSP_SPI_MEMORY_CHANNEL to be defined. Please set it to appropriate SPI channel number in user_config.h and recompile BSP with this option.
#endif


#if BSP_SPI_MEMORY_CHANNEL == 0

    #if ! BSPCFG_ENABLE_SPI0
        #error This application requires BSPCFG_ENABLE_SPI0 defined non-zero in user_config.h. Please recompile kernel with this option.
    #else
        #define TEST_CHANNEL_POLLED "spi0:"
    #endif

    #if BSPCFG_ENABLE_ISPI0
        #define TEST_CHANNEL_INT    "ispi0:"
    #endif

#elif BSP_SPI_MEMORY_CHANNEL == 1

    #if ! BSPCFG_ENABLE_SPI1
        #error This application requires BSPCFG_ENABLE_SPI1 defined non-zero in user_config.h. Please recompile kernel with this option.
    #else
        #define TEST_CHANNEL_POLLED "spi1:"
    #endif

    #if BSPCFG_ENABLE_ISPI1
        #define TEST_CHANNEL_INT    "ispi1:"
    #endif

#elif BSP_SPI_MEMORY_CHANNEL == 2

    #if ! BSPCFG_ENABLE_SPI2
        #error This application requires BSPCFG_ENABLE_SPI2 defined non-zero in user_config.h. Please recompile kernel with this option.
    #else
       #define TEST_CHANNEL_POLLED  "spi2:"
    #endif

    #if BSPCFG_ENABLE_ISPI2
        #define TEST_CHANNEL_INT    "ispi2:"
    #endif

#else

     #error Unsupported SPI channel number. Please check settings of BSP_SPI_MEMORY_CHANNEL in BSP.

#endif

/* Test strings */
#define TEST_BYTE_1       0xBA
#define TEST_BYTE_2       0xA5
#define TEST_STRING_SHORT "Hello,World!"
#define TEST_STRING_LONG  "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz1234567890"


uchar   send_buffer[1 + SPI_MEMORY_ADDRESS_BYTES + SPI_MEMORY_PAGE_SIZE] = {0};
uchar   recv_buffer[sizeof(TEST_STRING_LONG)] = {0};


const char_ptr device_mode[] = 
{
    "SPI_DEVICE_MASTER_MODE",
    "SPI_DEVICE_SLAVE_MODE", 
    "SPI_DEVICE_BIO_MASTER_MODE", 
    "SPI_DEVICE_BIO_SLAVE_MODE"
};

const char_ptr clock_mode[] = 
{
    "SPI_CLK_POL_PHA_MODE0",
    "SPI_CLK_POL_PHA_MODE1",
    "SPI_CLK_POL_PHA_MODE2",
    "SPI_CLK_POL_PHA_MODE3"
};


extern void main_task (uint_32);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    { 10L,          main_task,  1500L,  8L,         "Main", MQX_AUTO_START_TASK, 0,     0  },
    { 0 }
};


/* DEBUG */
#define SPI_DEBUG_STATUS            1
#define SPI_DEBUG_BYTE              1
#define SPI_DEBUG_WRITE_DATA_SHORT  1
#define SPI_DEBUG_WRITE_DATA_LONG   1

#ifdef BSP_SPI_MEMORY_GPIO_CS

/*FUNCTION*---------------------------------------------------------------
*
* Function Name : set_CS
* Comments  : This function sets chip select signal to enable/disable memory.
*             It's used only on platforms with manual CS handling.
*END*----------------------------------------------------------------------*/


void set_CS (uint_32 cs_mask, uint_32 logic_level, pointer user_data)
{

   LWGPIO_STRUCT_PTR spigpio = (LWGPIO_STRUCT_PTR)user_data;

   if (cs_mask & BSP_SPI_MEMORY_SPI_CS)
      {
        lwgpio_set_value(spigpio, logic_level ? LWGPIO_VALUE_HIGH : LWGPIO_VALUE_LOW);
      }
}
#endif /* BSP_SPI_MEMORY_GPIO_CS */

/*TASK*-------------------------------------------------------------------
*
* Task Name : main_task
* Comments  :
*
*END*----------------------------------------------------------------------*/
void main_task
   (
      uint_32 dummy
   )
{
   MQX_FILE_PTR           spifd;
   uint_32                param, result, i = 0;
   SPI_STATISTICS_STRUCT  stats;
   SPI_READ_WRITE_STRUCT  rw;

#ifdef BSP_SPI_MEMORY_GPIO_CS
   LWGPIO_STRUCT          spigpio;
   SPI_CS_CALLBACK_STRUCT callback;
#endif

#ifdef BSP_M51EMDEMO
/*
**  The M51DEMOEM board has PTA6 & PTA7 pins dedicated for
**  controling ~WP and ~HOLD signals on MRAM memory.
**  For the purpose of the demo this pins are set to LOG1.
*/
   LWGPIO_STRUCT          lwgpio_wphold;
   
   /* Open GPIO file containing SPI pin SS == chip select for memory */
   if (!lwgpio_init(&lwgpio_wphold, (LWGPIO_PORT_A | LWGPIO_PIN6), LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE))
   {
      printf("Initializing GPIO with associated pins failed.\n");
      _time_delay (200L);
      _task_block();
   }
   lwgpio_set_functionality(&lwgpio_wphold, BSP_SPI_MUX_GPIO);/*BSP_SPI_MUX_GPIO need define in BSP for function mux as GPIO*/
   lwgpio_set_value(&lwgpio_wphold, LWGPIO_VALUE_HIGH);
   
   if (!lwgpio_init(&lwgpio_wphold, (LWGPIO_PORT_A | LWGPIO_PIN7), LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE))
   {
      printf("Initializing GPIO with associated pins failed.\n");
      _time_delay (200L);
      _task_block();
   }
   lwgpio_set_functionality(&lwgpio_wphold, BSP_SPI_MUX_GPIO);/*BSP_SPI_MUX_GPIO need define in BSP for function mux as GPIO*/
   lwgpio_set_value(&lwgpio_wphold, LWGPIO_VALUE_HIGH);
#endif


   printf ("\n-------------- SPI driver example --------------\n\n");

   printf ("This example application demonstrates usage of SPI driver.\n");
   printf ("It transfers data to/from external memory over SPI bus.\n");
   printf ("The default settings in memory.h apply to TWR-MEM flash memory.\n");


   printf ("\n-------------- Polled SPI example --------------\n\n");
   /* Open the SPI driver */
   spifd = fopen (TEST_CHANNEL_POLLED, NULL);

   if (NULL == spifd)
   {
      printf ("Error opening SPI driver!\n");
      _time_delay (200L);
      _task_block ();
   }

#if BSP_SPI_MEMORY_GPIO_CS
   /* Open GPIO file containing SPI pin SS == chip select for memory */

   if (!lwgpio_init(&spigpio, BSP_SPI_MEMORY_GPIO_CS, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE))
   {
       printf("Initializing GPIO with associated pins failed.\n");
       _time_delay (200L);
       _task_block();
   }
   lwgpio_set_functionality(&spigpio,BSP_SPI_MUX_GPIO);/*BSP_SPI_MUX_GPIO need define in BSP for function mux as GPIO*/

   /* Set CS callback */

   callback.MASK = BSP_SPI_MEMORY_SPI_CS;
   callback.CALLBACK = set_CS;
   callback.USERDATA = &spigpio;
   printf ("Setting CS callback ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_CS_CALLBACK, &callback))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

#endif

   /* Display baud rate */
   printf ("Current baud rate ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_BAUD, &param))
   {
      printf ("%d Hz\n", param);
   } else {
      printf ("ERROR\n");
   }

   /* Set a different rate */
   param = 500000;
   printf ("Changing the baud rate to %d Hz ... ", param);
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_BAUD, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Display baud rate */
   printf ("Current baud rate ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_BAUD, &param))
   {
      printf ("%d Hz\n", param);
   } else {
      printf ("ERROR\n");
   }

   /* Set clock mode */
   param = SPI_CLK_POL_PHA_MODE0;
   printf ("Setting clock mode to %s ... ", clock_mode[param]);
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_MODE, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get clock mode */
   printf ("Getting clock mode ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_MODE, &param))
   {
      printf ("%s\n", clock_mode[param]);
   } else {
      printf ("ERROR\n");
   }

   /* Set big endian */
   param = SPI_DEVICE_BIG_ENDIAN;
   printf ("Setting endian to %s ... ", param == SPI_DEVICE_BIG_ENDIAN ? "SPI_DEVICE_BIG_ENDIAN" : "SPI_DEVICE_LITTLE_ENDIAN");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_ENDIAN, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get endian */
   printf ("Getting endian ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_ENDIAN, &param))
   {
      printf ("%s\n", param == SPI_DEVICE_BIG_ENDIAN ? "SPI_DEVICE_BIG_ENDIAN" : "SPI_DEVICE_LITTLE_ENDIAN");
   } else {
      printf ("ERROR\n");
   }

   /* Set transfer mode */
   param = SPI_DEVICE_MASTER_MODE;
   printf ("Setting transfer mode to %s ... ", device_mode[param]);
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_TRANSFER_MODE, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get transfer mode */
   printf ("Getting transfer mode ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_TRANSFER_MODE, &param))
   {
      printf ("%s\n", device_mode[param]);
   } else {
      printf ("ERROR\n");
   }

   /* Clear statistics */
   printf ("Clearing statistics ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_CLEAR_STATS, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get statistics */
   printf ("Getting statistics:\n");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_STATS, &stats))
   {
      printf ("Interrupts:   %d\n", stats.INTERRUPTS);
      printf ("Rx packets:   %d\n", stats.RX_PACKETS);
      printf ("Rx overflow:  %d\n", stats.RX_OVERFLOWS);
      printf ("Tx packets:   %d\n", stats.TX_PACKETS);
      printf ("Tx aborts :   %d\n", stats.TX_ABORTS);
      printf ("Tx underflow: %d\n", stats.TX_UNDERFLOWS);
   } else {
      printf ("ERROR\n");
   }
   printf ("\n");

#if SPI_DEBUG_STATUS

   /* Read status */
   memory_read_status (spifd);

   /* Disable protection */
   memory_set_protection (spifd, FALSE);

   /* Erase memory before tests */
   memory_chip_erase (spifd);

   printf ("\n");

#endif

#if SPI_DEBUG_BYTE

   /* Write a data byte to memory */
   memory_write_byte (spifd, SPI_MEMORY_ADDR1, TEST_BYTE_1);

   /* Read status */
   memory_read_status (spifd);

   /* Read a data byte from memory */
   if (TEST_BYTE_1 == memory_read_byte (spifd, SPI_MEMORY_ADDR1))
   {
      printf ("Byte test ... OK\n");
   } else {
      printf ("Byte test ... ERROR\n");
   }

   /* Get statistics */
   printf ("Getting statistics:\n");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_STATS, &stats))
   {
      printf ("Interrupts:   %d\n", stats.INTERRUPTS);
      printf ("Rx packets:   %d\n", stats.RX_PACKETS);
      printf ("Rx overflow:  %d\n", stats.RX_OVERFLOWS);
      printf ("Tx packets:   %d\n", stats.TX_PACKETS);
      printf ("Tx aborts :   %d\n", stats.TX_ABORTS);
      printf ("Tx underflow: %d\n", stats.TX_UNDERFLOWS);
   } else {
      printf ("ERROR\n");
   }
   printf ("\n");

#endif

#if SPI_DEBUG_WRITE_DATA_SHORT

   /* Write short data to memory */
   memory_write_data (spifd, SPI_MEMORY_ADDR2, sizeof(TEST_STRING_SHORT) - 1, (uchar_ptr)TEST_STRING_SHORT);

   /* Read status */
   memory_read_status (spifd);

   /* Read data */
   result = memory_read_data (spifd, SPI_MEMORY_ADDR2, sizeof(TEST_STRING_SHORT) - 1, recv_buffer);

   /* Test result */
   if ((result != sizeof(TEST_STRING_SHORT) - 1) || (0 != memcmp (TEST_STRING_SHORT, recv_buffer, result)))
   {
      printf ("Write short data test ... ERROR\n");
   } else {
      printf ("Write short data test ... OK\n");
   }
   printf ("\n");

#endif

#if SPI_DEBUG_WRITE_DATA_LONG

   /* Page write to memory */
   memory_write_data (spifd, SPI_MEMORY_ADDR3, sizeof(TEST_STRING_LONG) - 1, (uchar_ptr)TEST_STRING_LONG);

   /* Read status */
   memory_read_status (spifd);

   /* Read data */
   result = memory_read_data (spifd, SPI_MEMORY_ADDR3, sizeof(TEST_STRING_LONG) - 1, recv_buffer);

   /* Test result */
   if ((result != sizeof(TEST_STRING_LONG) - 1) || (0 != memcmp (TEST_STRING_LONG, recv_buffer, result)))
   {
      printf ("Write long data test ... ERROR\n");
   } else {
      printf ("Write long data test ... OK\n");
   }
   printf ("\n");

#endif

   /* Test simultaneous write and read */
   memset (send_buffer, 0, sizeof (send_buffer));
   memset (recv_buffer, 0, sizeof (recv_buffer));
   send_buffer[0] = SPI_MEMORY_READ_DATA;
   for (i = SPI_MEMORY_ADDRESS_BYTES; i != 0; i--)
   {
      send_buffer[i] = (SPI_MEMORY_ADDR1 >> ((SPI_MEMORY_ADDRESS_BYTES - i) << 3)) & 0xFF;
   }
   rw.BUFFER_LENGTH = 10;
   rw.WRITE_BUFFER = (char_ptr)send_buffer;
   rw.READ_BUFFER = (char_ptr)recv_buffer;
   printf ("IO_IOCTL_SPI_READ_WRITE ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_READ_WRITE, &rw))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   fflush (spifd);
   printf ("Simultaneous write and read - memory read from 0x%08x (%d):\n", SPI_MEMORY_ADDR1, rw.BUFFER_LENGTH);
   printf ("Write: ");
   for (i = 0; i < rw.BUFFER_LENGTH; i++)
   {
      printf ("0x%02x ", (uchar)rw.WRITE_BUFFER[i]);
   }
   printf ("\nRead : ");
   for (i = 0; i < rw.BUFFER_LENGTH; i++)
   {
      printf ("0x%02x ", (uchar)rw.READ_BUFFER[i]);
   }
   if (TEST_BYTE_1 == (uchar)rw.READ_BUFFER[1 + SPI_MEMORY_ADDRESS_BYTES])
   {
      printf ("\nSimultaneous read/write (data == 0x%02x) ... OK\n\n", (uchar)rw.READ_BUFFER[1 + SPI_MEMORY_ADDRESS_BYTES]);
   }
   else
   {
      printf ("\nSimultaneous read/write (data == 0x%02x) ... ERROR\n\n", (uchar)rw.READ_BUFFER[1 + SPI_MEMORY_ADDRESS_BYTES]);
   }

   /* Close the SPI */
   result = (uint_32)fclose (spifd);
   if (result)
   {
      printf ("Error closing SPI, returned: 0x%08x\n", result);
   }

#if defined (TEST_CHANNEL_INT)

   printf ("\n-------------- Interrupt SPI example --------------\n\n");

   /* Open the interrupt SPI driver */
   spifd = fopen (TEST_CHANNEL_INT, (pointer)(SPI_FLAG_HALF_DUPLEX));  // same as NULL
   if (NULL == spifd)
   {
      printf ("Error opening ISPI driver!\n");
      _time_delay(200L);
      _task_block ();
   }

#if BSP_SPI_MEMORY_GPIO_CS

   /* Open GPIO file containing SPI pin SS == chip select for memory */

       if (!lwgpio_init(&spigpio, BSP_SPI_MEMORY_GPIO_CS, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE))
       {
           printf("Initializing GPIO with associated pins failed.\n");
           _time_delay (200L);
           _task_block();
       }
       lwgpio_set_functionality(&spigpio,BSP_SPI_MUX_GPIO);/*BSP_SPI_MUX_GPIO need define in BSP for function mux as GPIO*/


   /* Set CS callback */
   callback.MASK = BSP_SPI_MEMORY_SPI_CS;
   callback.CALLBACK = set_CS;
   callback.USERDATA = &spigpio;
   printf ("Setting CS callback ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_CS_CALLBACK, &callback))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

#endif

   /* Display baud rate */
   printf ("Current baud rate ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_BAUD, &param))
   {
      printf ("%d Hz\n", param);
   } else {
      printf ("ERROR\n");
   }

   /* Set a different rate */
   param = 500000;
   printf ("Changing the baud rate to %d Hz ... ", param);
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_BAUD, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Display baud rate */
   printf ("Current baud rate ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_BAUD, &param))
   {
      printf ("%d Hz\n", param);
   } else {
      printf ("ERROR\n");
   }

   /* Set clock mode */
   param = SPI_CLK_POL_PHA_MODE0;
   printf ("Setting clock mode to %s ... ", clock_mode[param]);
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_MODE, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get clock mode */
   printf ("Getting clock mode ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_MODE, &param))
   {
      printf ("%s\n", clock_mode[param]);
   } else {
      printf ("ERROR\n");
   }

   /* Set big endian */
   param = SPI_DEVICE_BIG_ENDIAN;
   printf ("Setting endian to %s ... ", param == SPI_DEVICE_BIG_ENDIAN ? "SPI_DEVICE_BIG_ENDIAN" : "SPI_DEVICE_LITTLE_ENDIAN");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_ENDIAN, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get endian */
   printf ("Getting endian ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_ENDIAN, &param))
   {
      printf ("%s\n", param == SPI_DEVICE_BIG_ENDIAN ? "SPI_DEVICE_BIG_ENDIAN" : "SPI_DEVICE_LITTLE_ENDIAN");
   } else {
      printf ("ERROR\n");
   }
   
   /* Set transfer mode */
   param = SPI_DEVICE_MASTER_MODE;
   printf ("Setting transfer mode to %s ... ", device_mode[param]);
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_SET_TRANSFER_MODE, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get transfer mode */
   printf ("Getting transfer mode ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_TRANSFER_MODE, &param))
   {
      printf ("%s\n", device_mode[param]);
   } else {
      printf ("ERROR\n");
   }

   /* Clear statistics */
   printf ("Clearing statistics ... ");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_CLEAR_STATS, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Get statistics */
   printf ("Getting statistics:\n");
   if (SPI_OK == ioctl (spifd, IO_IOCTL_SPI_GET_STATS, &stats))
   {
      printf ("Interrupts:   %d\n", stats.INTERRUPTS);
      printf ("Rx packets:   %d\n", stats.RX_PACKETS);
      printf ("Rx overflow:  %d\n", stats.RX_OVERFLOWS);
      printf ("Tx packets:   %d\n", stats.TX_PACKETS);
      printf ("Tx aborts :   %d\n", stats.TX_ABORTS);
      printf ("Tx underflow: %d\n", stats.TX_UNDERFLOWS);
   } else {
      printf ("ERROR\n");
   }
   printf ("\n");

#if SPI_DEBUG_STATUS

   /* Read status */
   memory_read_status_interrupt (spifd);

   /* Disable protection */
   memory_set_protection_interrupt (spifd, FALSE);

   /* Erase memory before tests */
   memory_chip_erase_interrupt (spifd);

   printf ("\n");

#endif

#if SPI_DEBUG_BYTE

   /* Write a data byte to memory */
   memory_write_byte_interrupt (spifd, SPI_MEMORY_ADDR1, TEST_BYTE_2);

   /* Read status */
   memory_read_status_interrupt (spifd);

   /* Read a data byte from memory */
   if (TEST_BYTE_2 == memory_read_byte_interrupt (spifd, SPI_MEMORY_ADDR1))
   {
      printf ("Byte test ... OK\n");
   } else {
      printf ("Byte test ... ERROR\n");
   }

   /* Get statistics */
   printf ("Getting statistics:\n");
   if (SPI_OK == ioctl(spifd, IO_IOCTL_SPI_GET_STATS, &stats))
   {
      printf ("Interrupts:   %d\n", stats.INTERRUPTS);
      printf ("Rx packets:   %d\n", stats.RX_PACKETS);
      printf ("Rx overflow:  %d\n", stats.RX_OVERFLOWS);
      printf ("Tx packets:   %d\n", stats.TX_PACKETS);
      printf ("Tx aborts :   %d\n", stats.TX_ABORTS);
      printf ("Tx underflow: %d\n", stats.TX_UNDERFLOWS);
   } else {
      printf ("ERROR\n");
   }
   printf ("\n");

#endif

#if SPI_DEBUG_WRITE_DATA_SHORT

   /* Write short data to memory */
   memory_write_data_interrupt (spifd, SPI_MEMORY_ADDR3, sizeof(TEST_STRING_SHORT) - 1, (uchar_ptr)TEST_STRING_SHORT);

   /* Read status */
   memory_read_status_interrupt (spifd);

   /* Read data */
   result = memory_read_data_interrupt (spifd, SPI_MEMORY_ADDR3, sizeof(TEST_STRING_SHORT) - 1, recv_buffer);

   /* Test result */
   if ((result != sizeof(TEST_STRING_SHORT) - 1) || (0 != memcmp (TEST_STRING_SHORT, recv_buffer, result)))
   {
      printf ("Write short data test ... ERROR\n");
   } else {
      printf ("Write short data test ... OK\n");
   }
   printf ("\n");

#endif

#if SPI_DEBUG_WRITE_DATA_LONG

   /* Page write to memory */
   memory_write_data_interrupt (spifd, SPI_MEMORY_ADDR2, sizeof(TEST_STRING_LONG) - 1, (uchar_ptr)TEST_STRING_LONG);

   /* Read status */
   memory_read_status_interrupt (spifd);

   /* Read data */
   result = memory_read_data_interrupt (spifd, SPI_MEMORY_ADDR2, sizeof(TEST_STRING_LONG) - 1, recv_buffer);

   /* Test result */
   if ((result != sizeof(TEST_STRING_LONG) - 1) || (0 != memcmp (TEST_STRING_LONG, recv_buffer, result)))
   {
      printf ("Write long data test ... ERROR\n");
   } else {
      printf ("Write long data test ... OK\n");
   }
   printf ("\n");

#endif

   /* Close the SPI */
   result = (uint_32)fclose (spifd);
   if (result)
   {
      printf ("Error closing ISPI, returned: 0x%08x\n", result);
   }

#endif

   printf ("\n-------------- End of example --------------\n\n");

} /* Endbody */

/* EOF */

