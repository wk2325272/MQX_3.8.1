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
* $FileName: eeprom_polled.c$
* $Version : 3.8.6.4$
* $Date    : Feb-21-2012$
*
* Comments:
*
*   This file contains read/write functions to access I2C EEPROMs
*   using I2C polled driver.
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <i2c.h>
#include "eeprom.h"


/*FUNCTION****************************************************************
* 
* Function Name    : i2c_write_eeprom_polled
* Returned Value   : void
* Comments         : 
*   Writes the provided data buffer at address in I2C EEPROMs
*
*END*********************************************************************/

void i2c_write_eeprom_polled
   (
      /* [IN] The file pointer for the I2C channel */
      MQX_FILE_PTR fd,

      /* [IN] The address in EEPROM to write to */
      uint_32    addr,

      /* [IN] The array of characters are to be written in EEPROM */
      uchar_ptr  buffer,

      /* [IN] Number of bytes in that buffer */
      _mqx_int   n      
   )
{ /* Body */

   uint_32       param;
   _mqx_int    length;
   _mqx_int    result;
   uint_8        mem[I2C_EEPROM_MEMORY_WIDTH];

   /* Protect I2C transaction in multitask environment */
   _lwsem_wait (&lock);

   printf ("Writing %d bytes to address 0x%08x ...\n", n, addr);
   do
   {
      /* I2C bus address also contains memory block index */
      param = I2C_EEPROM_BUS_ADDRESS;
      printf ("  Set I2C bus address to 0x%02x ... ", param);
      if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }

      length = (_mqx_int)(I2C_EEPROM_PAGE_SIZE - (addr & (I2C_EEPROM_PAGE_SIZE - 1)));
      if (length > n) length = n;

      /* Initiate start and send I2C bus address */
      printf ("  I2C start, send address and get ack ... ");
      fwrite (mem, 1, 0, fd);

      /* Check ack (device exists) */
      if (I2C_OK == ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
      {
         printf ("OK ... ack == %d\n", param);
         if (param) 
         {
            /* Stop I2C transfer */
            printf ("  Stop transfer ... ");
            if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
            {
               printf ("OK\n");
            } else {
               printf ("ERROR\n");
            }
            break;
         }
      } else {
         printf ("ERROR\n");
      }

      /* Write address within memory block */
#if I2C_EEPROM_MEMORY_WIDTH == 2
      mem[0] = (uint_8)(addr >> 8);
      mem[1] = (uint_8)addr;
      printf ("  Write to address 0x%02x%02x ... ", mem[0], mem[1]);
      result = fwrite (mem, 1, 2, fd);
      if (2 == result)
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }
#else
      mem[0] = (uint_8)addr;
      printf ("  Write to address 0x%02x ... ", mem[0]);
      result = fwrite (mem, 1, 1, fd);
      if (1 == result)
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }
#endif

      /* Page write of data */
      printf ("  Page write %d bytes ... ", length);
      result = fwrite (buffer, 1, length, fd);
      if (result == length)
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }
      
      /* Wait for completion */
      printf ("  Flush ... ");
      result = fflush (fd);
      if (MQX_OK == result)
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }

      /* Stop I2C transfer - initiate EEPROM write cycle */
      printf ("  Stop transfer ... ");
      if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }

      /* Wait for EEPROM write cycle finish - acknowledge */
      result = 0;
      do 
      {  /* Sends just I2C bus address, returns acknowledge bit and stops */
         fwrite (&result, 1, 0, fd);
         
         if (I2C_OK != ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
         {
            printf ("  ERROR during wait (flush)\n");
         }
         
         if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
         {
            printf ("  ERROR during wait (stop)\n");
         }
         result++;
      } while ((param & 1) || (result <= 1));
      
      printf ("  EEPROM polled %d times.\n", result);

      /* Next round */
      buffer += length;
      addr += length;
      n -= length;
        
   } while (n);

   /* Release the transaction lock */
   _lwsem_post (&lock);
} /* Endbody */

   
/*FUNCTION****************************************************************
* 
* Function Name    : i2c_read_eeprom_polled
* Returned Value   : void
* Comments         : 
*   Reads into the provided data buffer from address in I2C EEPROM
*
*END*********************************************************************/

void i2c_read_eeprom_polled
   (
      /* [IN] The file pointer for the I2C channel */
      MQX_FILE_PTR fd,

      /* [IN] The address in EEPROM to read from */
      uint_32    addr,

      /* [IN] The array of characters to be written into */
      uchar_ptr  buffer,

      /* [IN] Number of bytes to read */
      _mqx_int   n      
   )
{ /* Body */
   _mqx_int    param;
   _mqx_int    result;
   uint_8        mem[I2C_EEPROM_MEMORY_WIDTH];

   printf ("Reading %d bytes from address 0x%08x ...\n", n, addr);
   if (0 == n) 
   {
      printf ("  Nothing to do.\n");
      return;
   }

   /* Protect I2C transaction in multitask environment */
   _lwsem_wait (&lock);
   
   /* I2C bus address also contains memory block index */
   param = I2C_EEPROM_BUS_ADDRESS;
   printf ("  Set I2C bus address to 0x%02x ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Initiate start and send I2C bus address */
   printf ("  I2C start, send address and get ack ... ");
   fwrite (mem, 1, 0, fd);

   /* Check ack (device exists) */
   if (I2C_OK == ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
   {
      printf ("OK ... ack == %d\n", param);
      if (param) 
      {
         /* Stop I2C transfer */
         printf ("  Stop transfer ... ");
         if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
         {
            printf ("OK\n");
         } else {
            printf ("ERROR\n");
         }
         
         /* Release the transaction lock */
         _lwsem_post (&lock);
         
         return;
      }
   } else {
      printf ("ERROR\n");
   }

   /* Write address within memory block */
#if I2C_EEPROM_MEMORY_WIDTH == 2
      mem[0] = (uint_8)(addr >> 8);
      mem[1] = (uint_8)addr;
      printf ("  Write to address 0x%02x%02x ... ", mem[0], mem[1]);
      result = fwrite (mem, 1, 2, fd);
      if (2 == result)
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }
#else
      mem[0] = (uint_8)addr;
      printf ("  Write to address 0x%02x ... ", mem[0]);
      result = fwrite (mem, 1, 1, fd);
      if (1 == result)
      {
         printf ("OK\n");
      } else {
         printf ("ERROR\n");
      }
#endif

   /* Wait for completion */
   printf ("  Flush ... ");
   result = fflush (fd);
   if (MQX_OK == result)
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Restart I2C transfer for reading */
   printf ("  Initiate repeated start ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_REPEATED_START, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Set read request */
   param = n;
   printf ("  Set number of bytes requested to %d ... ", param);
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_SET_RX_REQUEST, &param))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }

   /* Read all data */
   printf ("  Read %d bytes ... ", n);
   result = fread (buffer, 1, n, fd);
   if (result == n)
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
      
   /* Stop I2C transfer - initiate EEPROM write cycle */
   printf ("  Stop transfer ... ");
   if (I2C_OK == ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
   {
      printf ("OK\n");
   } else {
      printf ("ERROR\n");
   }
   
   /* Release the transaction lock */
   _lwsem_post (&lock);
} /* Endbody */
