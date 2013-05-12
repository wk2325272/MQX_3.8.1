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
* $FileName: i2c.c$
* $Version : 3.8.27.3$
* $Date    : Apr-25-2012$
*
* Comments:
*
*   This file contains the source for a simple example of an
*   an application that writes and reads the Microchip 24LC16 EEPROMs
*   using I2C driver.
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <i2c.h>
#include "eeprom.h"


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if defined BSP_KWIKSTIK_K40X256 || defined BSP_TWR_K40X256 || BSP_TWR_K40D100M
#define I2C_DEVICE_POLLED    "i2c1:"
#define I2C_DEVICE_INTERRUPT "ii2c1:"
#define ENABLE_I2C_INTERRUPT  BSPCFG_ENABLE_II2C1
#if ! BSPCFG_ENABLE_I2C1
#error This application requires BSPCFG_ENABLE_I2C1 defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#elif defined BSP_TWRPXS30
#define I2C_DEVICE_POLLED    "i2c2:"
#define I2C_DEVICE_INTERRUPT "ii2c2:"
#define ENABLE_I2C_INTERRUPT  BSPCFG_ENABLE_II2C2
#if ! BSPCFG_ENABLE_I2C2
#error This application requires BSPCFG_ENABLE_I2C2 defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#else
#define I2C_DEVICE_POLLED    "i2c0:"
#define I2C_DEVICE_INTERRUPT "ii2c0:"
#define ENABLE_I2C_INTERRUPT  BSPCFG_ENABLE_II2C0
#if ! BSPCFG_ENABLE_I2C0
#error This application requires BSPCFG_ENABLE_I2C0 defined non-zero in user_config.h. Please recompile BSP with this option.
#endif
#endif


#define BUFFER_SIZE 256
#define TEST_STRING "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
LWSEM_STRUCT        lock;


extern void main_task(uint_32);


const TASK_TEMPLATE_STRUCT MQX_template_list[] = 
{
  /* Task Index,   Function,  Stack,  Priority, Name,   Attributes,          Param, Time Slice */
   { 10,           main_task, 2000L,    8L,     "Main", MQX_AUTO_START_TASK, 0,     0 },
   { 0 }
};

      
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
   MQX_FILE_PTR         fd;
   uint_32                        i, j;
   _mqx_int                     param, result;
   I2C_STATISTICS_STRUCT        stats;
   uchar_ptr                    buffer;

   /* I2C transaction lock */
   _lwsem_create (&lock, 1);
   
   /* Allocate receive buffer */
   buffer = _mem_alloc_zero (BUFFER_SIZE);
   if (buffer == NULL) 
   {
      printf ("ERROR getting receive buffer!\n");
      _task_block ();
   }

   
   printf ("\n\n-------------- Polled I2C master example --------------\n\n");

   /* Open the I2C driver */         
   fd = fopen (I2C_DEVICE_POLLED, NULL);
   if (fd == NULL) 
   {
      printf ("ERROR opening the I2C driver!\n");
      _task_block ();
   }

   /* Test ioctl commands */
   param = 100000;
   printf ("Set current baud rate to %d ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_BAUD, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   printf ("Get current baud rate ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_BAUD, &param))
   {
      printf ("%d\n", param);
   } else {
      printf ("ERROR\n");
   }

   printf ("Set master mode ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_MASTER_MODE, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   printf ("Get current mode ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_MODE, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }
   
   param = 0x60;
   printf ("Set station address to 0x%02x ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_STATION_ADDRESS, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   param = 0x00;
   printf ("Get station address ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATION_ADDRESS, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }

   printf ("Clear statistics ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_CLEAR_STATISTICS, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   printf ("Get statistics ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATISTICS, (pointer)&stats))
   {
      printf ("OK\n  Interrupts:  %d\n", stats.INTERRUPTS);
      printf ("  Rx packets:  %d\n", stats.RX_PACKETS);
      printf ("  Tx packets:  %d\n", stats.TX_PACKETS);
      printf ("  Tx lost arb: %d\n", stats.TX_LOST_ARBITRATIONS);
      printf ("  Tx as slave: %d\n", stats.TX_ADDRESSED_AS_SLAVE);
      printf ("  Tx naks:     %d\n", stats.TX_NAKS);
   } else {
      printf ("ERROR\n");
   }

   printf ("Get current state ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATE, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }
   
   param = I2C_EEPROM_BUS_ADDRESS;
   printf ("Set destination address to 0x%02x ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   param = 0x00;
   printf ("Get destination address ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_DESTINATION_ADDRESS, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }
   
   /* Test EEPROM communication */
   i2c_write_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, (uchar_ptr)TEST_STRING, 0);
   i2c_read_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, buffer, 0);

   i2c_write_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, (uchar_ptr)TEST_STRING, 1);
   i2c_read_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, buffer, 1);
   printf ("Received: %c (0x%02x)\n", buffer[0], buffer[0]);
   if (buffer[0] != TEST_STRING[0])
   {
      printf ("ERROR\n");
      _task_block ();
   }

   _mem_zero (buffer, BUFFER_SIZE);
   i2c_write_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, buffer, sizeof(TEST_STRING));
   i2c_read_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, buffer, sizeof(TEST_STRING));
   for (result = 0; result < BUFFER_SIZE; result++)
   {
      if (0 != buffer[result])
      {
         printf ("\nERROR during memory clearing\n");
         _task_block ();
      }
   }
   
   i2c_write_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, (uchar_ptr)TEST_STRING, sizeof(TEST_STRING));
   i2c_read_eeprom_polled (fd, I2C_EEPROM_MEMORY_ADDRESS1, buffer, sizeof(TEST_STRING));
   printf ("Received: ");
   for (result = 0; result < sizeof(TEST_STRING); result++)
   {
      printf ("%c", buffer[result]);
      if (buffer[result] != TEST_STRING[result])
      {
         printf ("\nERROR\n");
         _task_block ();
      }
   }
   printf ("\n");
   
   /* Test special cases: write 1 byte (address pointer), read 1 byte (dump memory) */
   _lwsem_wait (&lock);
   buffer[0] = 0;
   buffer[1] = 0;
   fwrite (buffer, 1, I2C_EEPROM_MEMORY_WIDTH, fd);
   if (I2C_OK != ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, NULL))
   {
      printf ("\nERROR during flush\n");
   }
   if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
   {
      printf ("\nERROR during stop\n");
   }
   
   printf ("\nMemory dump:\n");
   for (i = 0; i < 16; i++)
   {
      for (j = 0; j < 16; j++)
      {
         _time_delay (1);
         param = 1;
         if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_RX_REQUEST, &param))
         {
            printf ("\nERROR during set rx request\n");
         }
         fread (&(buffer[j]), 1, 1, fd);
         if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
         {
            printf ("\nERROR during stop\n");
         }
      }
      printf ("0x%02x: ", i * 16);
      for (j = 0; j < 16; j++)
      {
         printf ("%02x ", buffer[j]);
      }
      for (j = 0; j < 16; j++)
      {
         if ((32 <= buffer[j]) && (buffer[j] <= 128))
         {
            printf ("%c", buffer[j]);
         }
         else
         {
            printf (".");
         }
      }
      printf ("\n");
   }
   _lwsem_post (&lock);
   
   printf ("\nGet statistics ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATISTICS, (pointer)&stats))
   {
      printf ("OK\n  Interrupts:  %d\n", stats.INTERRUPTS);
      printf ("  Rx packets:  %d\n", stats.RX_PACKETS);
      printf ("  Tx packets:  %d\n", stats.TX_PACKETS);
      printf ("  Tx lost arb: %d\n", stats.TX_LOST_ARBITRATIONS);
      printf ("  Tx as slave: %d\n", stats.TX_ADDRESSED_AS_SLAVE);
      printf ("  Tx naks:     %d\n", stats.TX_NAKS);
   } else {
      printf ("ERROR\n");
   }

   printf ("Get current state ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATE, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }

   /* Close the driver */
   result = fclose (fd);
   if (result) 
   {
      printf ("ERROR during close, returned: %d\n", result);
   }

   
#if ENABLE_I2C_INTERRUPT

   printf ("\n\n-------------- Interrupt I2C master example --------------\n\n");
  
   /* Open the I2C driver */         
   fd = fopen (I2C_DEVICE_INTERRUPT, NULL);
   if (fd == NULL) 
   {
      printf ("Failed to open the I2C driver!\n");
      _task_block ();
   }

   /* Test ioctl commands */
   printf ("Get current baud rate ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_BAUD, &param))
   {
      printf ("%d\n", param);
   } else {
      printf ("ERROR\n");
   }

   printf ("Set master mode ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_MASTER_MODE, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   printf ("Get current mode ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_MODE, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }
   
   param = 0x60;
   printf ("Set station address to 0x%02x ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_STATION_ADDRESS, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   param = 0x00;
   printf ("Get station address ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATION_ADDRESS, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }

   printf ("Clear statistics ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_CLEAR_STATISTICS, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   printf ("Get statistics ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATISTICS, (pointer)&stats))
   {
      printf ("OK\n  Interrupts:  %d\n", stats.INTERRUPTS);
      printf ("  Rx packets:  %d\n", stats.RX_PACKETS);
      printf ("  Tx packets:  %d\n", stats.TX_PACKETS);
      printf ("  Tx lost arb: %d\n", stats.TX_LOST_ARBITRATIONS);
      printf ("  Tx as slave: %d\n", stats.TX_ADDRESSED_AS_SLAVE);
      printf ("  Tx naks:     %d\n", stats.TX_NAKS);
   } else {
      printf ("ERROR\n");
   }

   printf ("Get current state ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATE, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }
   
   param = I2C_EEPROM_BUS_ADDRESS;
   printf ("Set destination address to 0x%02x ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   param = 0x00;
   printf ("Get destination address ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_DESTINATION_ADDRESS, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }
   
   /* Test EEPROM communication */
   i2c_write_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, (uchar_ptr)TEST_STRING, 0);
   i2c_read_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, buffer, 0);

   i2c_write_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, (uchar_ptr)TEST_STRING, 1);
   i2c_read_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, buffer, 1);
   printf ("Received: %c (0x%02x)\n", buffer[0], buffer[0]);
   if (buffer[0] != TEST_STRING[0])
   {
      printf ("ERROR\n");
      _task_block ();
   }
   
   _mem_zero (buffer, BUFFER_SIZE);
   i2c_write_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, buffer, sizeof(TEST_STRING));
   i2c_read_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, buffer, sizeof(TEST_STRING));
   for (result = 0; result < BUFFER_SIZE; result++)
   {
      if (0 != buffer[result])
      {
         printf ("\nERROR during memory clearing\n");
         _task_block ();
      }
   }
      
   i2c_write_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, (uchar_ptr)TEST_STRING, sizeof(TEST_STRING));
   i2c_read_eeprom_interrupt (fd, I2C_EEPROM_MEMORY_ADDRESS2, buffer, sizeof(TEST_STRING));
   printf ("Received: ");
   for (result = 0; result < sizeof(TEST_STRING); result++)
   {
      printf ("%c", buffer[result]);
      if (buffer[result] != TEST_STRING[result])
      {
         printf ("\nERROR\n");
         _task_block ();
      }
   }
   printf ("\n");

   /* Test special cases: write 1 byte (address pointer), read 1 byte (dump memory) */
   _lwsem_wait (&lock);
   buffer[0] = 0;
   buffer[1] = 0;
   fwrite (buffer, 1, I2C_EEPROM_MEMORY_WIDTH, fd);
   if (I2C_OK != ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, NULL))
   {
      printf ("\nERROR during flush\n");
   }
   if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
   {
      printf ("\nERROR during stop\n");
   }
   
   printf ("\nMemory dump:\n");
   for (i = 0; i < 16; i++)
   {
      for (j = 0; j < 16; j++)
      {
         _time_delay (1);
         param = 1;
         if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_RX_REQUEST, &param))
         {
            printf ("\nERROR during set rx request\n");
         }
         do 
         {
            result = fread (&(buffer[j]), 1, 1, fd);
         } while (0 == result);
         if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
         {
            printf ("\nERROR during stop\n");
         }
      }
      printf ("0x%02x: ", i * 16);
      for (j = 0; j < 16; j++)
      {
         printf ("%02x ", buffer[j]);
      }
      for (j = 0; j < 16; j++)
      {
         if ((32 <= buffer[j]) && (buffer[j] <= 128))
         {
            printf ("%c", buffer[j]);
         }
         else
         {
            printf (".");
         }
      }
      printf ("\n");
   }
   _lwsem_post (&lock);
   
   printf ("\nGet statistics ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATISTICS, (pointer)&stats))
   {
      printf ("OK\n  Interrupts:  %d\n", stats.INTERRUPTS);
      printf ("  Rx packets:  %d\n", stats.RX_PACKETS);
      printf ("  Tx packets:  %d\n", stats.TX_PACKETS);
      printf ("  Tx lost arb: %d\n", stats.TX_LOST_ARBITRATIONS);
      printf ("  Tx as slave: %d\n", stats.TX_ADDRESSED_AS_SLAVE);
      printf ("  Tx naks:     %d\n", stats.TX_NAKS);
   } else {
      printf ("ERROR\n");
   }

   printf ("Get current state ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_GET_STATE, &param))
   {
      printf ("0x%02x\n", param);
   } else {
      printf ("ERROR\n");
   }

   /* Close the driver */
   result = fclose (fd);
   if (result) 
   {
      printf ("ERROR during close, returned: %d\n", result);
   }

#endif

   
   /* Free transation lock */
   _lwsem_destroy (&lock);

   /* Free buffer */
   _mem_free (buffer);
   printf("Example finished.\n");
   _task_block();
 
} /* Endbody */

/* EOF */
