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
* $FileName: memory_int.c$
* $Version : 3.8.11.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the functions which write and read the SPI memories
*   using the SPI driver in interrupt mode.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include "memory.h"

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_chip_erase
* Comments  : This function erases the whole memory SPI chip
*
*END*----------------------------------------------------------------------*/
void memory_chip_erase_interrupt (MQX_FILE_PTR spifd)
{
   _mqx_int result;
   
   /* This operation must be write-enabled */
   memory_set_write_latch_interrupt (spifd, TRUE);

   memory_read_status_interrupt (spifd);

   printf ("Erase whole memory chip:\n");
   send_buffer[0] = SPI_MEMORY_CHIP_ERASE;
     
   /* Write instruction */
   do 
   {
      result = fwrite (send_buffer, 1, 1, spifd);
   } while (result < 1);
   
   /* Wait till transfer end (and deactivate CS) */
   fflush (spifd);

   while (memory_read_status_interrupt (spifd) & 1)
   {
      _time_delay (1000);
   }

   printf ("Erase chip ... ");
   if (result != 1) 
   {
      printf ("ERROR\n");
   }
   else
   {
      printf ("OK\n");
   }
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_set_write_latch_interrupt
* Comments  : This function sets latch to enable/disable memory write 
*             operation
*
*END*----------------------------------------------------------------------*/
void memory_set_write_latch_interrupt (MQX_FILE_PTR spifd, boolean enable)
{
   _mqx_int result;
   
   if (enable)
   {
      printf ("Enable write latch in memory ... ");
      send_buffer[0] = SPI_MEMORY_WRITE_LATCH_ENABLE;
   } else {
      printf ("Disable write latch in memory ... ");
      send_buffer[0] = SPI_MEMORY_WRITE_LATCH_DISABLE;
   }
     
   /* Write instruction */
   do 
   {
      result = fwrite (send_buffer, 1, 1, spifd);
   } while (result < 1);
   
   /* Wait till transfer end (and deactivate CS) */
   fflush (spifd);

   if (result != 1) 
   {
      printf ("ERROR\n");
   }
   else
   {
      printf ("OK\n");
   }
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_set_protection_interrupt
* Comments  : This function sets write protection in memory status register
*
*END*----------------------------------------------------------------------*/
void memory_set_protection_interrupt (MQX_FILE_PTR spifd, boolean protect)
{
   _mqx_int result, i;
   uint_8 protection;
   
   /* Must do it twice to ensure right transitions in protection status register */
   for (i = 0; i < 2; i++)
   {
      /* Each write operation must be enabled in memory */
      memory_set_write_latch_interrupt (spifd, TRUE);
   
      memory_read_status_interrupt (spifd);
   
      if (protect)
      {
         printf ("Write protect memory ... ");
         protection = 0xFF;
      } else {
         printf ("Write unprotect memory ... ");
         protection = 0x00;
      }
     
      send_buffer[0] = SPI_MEMORY_WRITE_STATUS;
      send_buffer[1] = protection;
   
      /* Write instruction */
      do 
      {
         result = fwrite (send_buffer, 1, 2, spifd);
      } while (result < 2);
   
      /* Wait till transfer end (and deactivate CS) */
      fflush (spifd);

      if (result != 2) 
      {
         printf ("ERROR\n");
      }
      else
      {
         printf ("OK\n");
      }
   }
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_read_status_interrupt
* Comments  : This function reads memory status register 
* Return: 
*         Status read.             
*
*END*----------------------------------------------------------------------*/
uint_8 memory_read_status_interrupt (MQX_FILE_PTR spifd)
{
   _mqx_int result;
   uint_8 state = 0xFF;

   printf ("Read memory status ... ");

   send_buffer[0] = SPI_MEMORY_READ_STATUS;

   /* Write instruction */
   do 
   {
     result = fwrite (send_buffer, 1, 1, spifd);
   } while (result < 1);
   
   if (result != 1)
   {
     /* Stop transfer */
      printf ("ERROR (1)\n");
      return state;
   }
  
   /* Read memory status */
   do 
   {
      result = fread (&state, 1, 1, spifd);
   } while (result < 1);

   /* Wait till transfer end (and deactivate CS) */
   fflush (spifd);

   if (result != 1) 
   {
      printf ("ERROR (2)\n");
   }
   else
   {
      printf ("0x%02x\n", state);
   }
   
   return state;
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_write_byte_interrupt
* Comments  : This function writes a data byte to memory  
*     
*
*END*----------------------------------------------------------------------*/
void memory_write_byte_interrupt (MQX_FILE_PTR spifd, uint_32 addr, uchar data)
{
   _mqx_int result;

   /* Each write operation must be enabled in memory */
   memory_set_write_latch_interrupt (spifd, TRUE);

   memory_read_status_interrupt (spifd);

   send_buffer[0] = SPI_MEMORY_WRITE_DATA;                                               // Write instruction
   for (result = SPI_MEMORY_ADDRESS_BYTES; result != 0; result--)
   {
      send_buffer[result] = (uchar)((addr >> ((SPI_MEMORY_ADDRESS_BYTES - result) << 3)) & 0xFF); // Address
   }
   send_buffer[1 + SPI_MEMORY_ADDRESS_BYTES] = data;                                     // Data
   
   printf ("Write byte to location 0x%08x in memory ... ", addr);
   
   /* Write instruction, address and byte */
   result = 0;
   do 
   {
      result += fwrite (send_buffer + result, 1, 1 + SPI_MEMORY_ADDRESS_BYTES + 1 - result, spifd);
   } while (result < 1 + SPI_MEMORY_ADDRESS_BYTES + 1);
   
   /* Wait till transfer end (and deactivate CS) */
   fflush (spifd);

   if (result != 1 + SPI_MEMORY_ADDRESS_BYTES + 1) 
   {
      printf ("ERROR\n");
   }
   else 
   {
      printf ("0x%02x\n", data);
   }
   
   /* There is 5 ms internal write cycle needed for memory */
   _time_delay (5);
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_read_byte_interrupt
* Comments  : This function reads a data byte from memory
* Return: 
*         Byte read.             
*
*END*----------------------------------------------------------------------*/
uint_8 memory_read_byte_interrupt (MQX_FILE_PTR spifd, uint_32 addr)
{
   _mqx_int result;
   uint_8 data = 0;    
   
   send_buffer[0] = SPI_MEMORY_READ_DATA;                                                // Read instruction
   for (result = SPI_MEMORY_ADDRESS_BYTES; result != 0; result--)
   {
      send_buffer[result] = (uchar)((addr >> ((SPI_MEMORY_ADDRESS_BYTES - result) << 3)) & 0xFF); // Address
   }

   printf ("Read byte from location 0x%08x in memory ... ", addr);

   /* Write instruction and address */
   result = 0;
   do 
   {
      result += fwrite (send_buffer + result, 1, 1 + SPI_MEMORY_ADDRESS_BYTES - result, spifd);
   } while (result < 1 + SPI_MEMORY_ADDRESS_BYTES);

   if (result != 1 + SPI_MEMORY_ADDRESS_BYTES)
   {
      /* Stop transfer */
      printf ("ERROR (1)\n");
      return data;
   }

   /* Read data from memory */
   do 
   {
      result = fread (&data, 1, 1, spifd);
   } while (result < 1);
   
   /* Wait till transfer end (and deactivate CS) */
   fflush (spifd);

   if (result != 1) 
   {
      printf ("ERROR (2)\n");
   }
   else
   {
      printf ("0x%02x\n", data);
   }
   
   return data;
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_write_data_interrupt
* Comments  : This function writes data buffer to memory using page write
* Return: 
*         Number of bytes written.             
*
*END*----------------------------------------------------------------------*/
uint_32 memory_write_data_interrupt (MQX_FILE_PTR spifd, uint_32 addr, uint_32 size, uchar_ptr data)
{
   uint_32 i, len, count = size;
   _mqx_int result;

   while (count > 0) 
   {
      /* Each write operation must be enabled in memory */
      memory_set_write_latch_interrupt (spifd, TRUE);

      memory_read_status_interrupt (spifd);

      len = count;
      if (len > SPI_MEMORY_PAGE_SIZE - (addr & (SPI_MEMORY_PAGE_SIZE - 1))) len = SPI_MEMORY_PAGE_SIZE - (addr & (SPI_MEMORY_PAGE_SIZE - 1));
      count -= len;

      printf ("Page write %d bytes to location 0x%08x in memory:\n", len, addr);

      send_buffer[0] = SPI_MEMORY_WRITE_DATA;                                     // Write instruction
      for (i = SPI_MEMORY_ADDRESS_BYTES; i != 0; i--)
      {
         send_buffer[i] = (uchar)((addr >> ((SPI_MEMORY_ADDRESS_BYTES - i) << 3)) & 0xFF); // Address
      }
      for (i = 0; i < len; i++)
      {
         send_buffer[1 + SPI_MEMORY_ADDRESS_BYTES + i] = data[i];                 // Data
      }

      /* Write instruction, address and data */
      result = 0;
      do 
      {
         result += fwrite (send_buffer + result, 1, 1 + SPI_MEMORY_ADDRESS_BYTES + (_mqx_int)(len - result), spifd);
      } while (result < 1 + SPI_MEMORY_ADDRESS_BYTES + len);
     
      /* Wait till transfer end (and deactivate CS) */
      fflush (spifd);

      if (result != 1 + SPI_MEMORY_ADDRESS_BYTES + len) 
      {
         printf ("ERROR\n"); 
         return size - count;
      }
      else
      {
         for (i = 0; i < len; i++)
            printf ("%c", data[i]);
         printf ("\n"); 
      }
     
      /* Move to next block */
      addr += len;
      data += len;

      /* There is 5 ms internal write cycle needed for memory */
      _time_delay (5);
   }
   
   return size;
}

/*FUNCTION*---------------------------------------------------------------
* 
* Function Name : memory_read_data_interrupt
* Comments  : This function reads data from memory into given buffer
* Return: 
*         Number of bytes read.             
*
*END*----------------------------------------------------------------------*/
uint_32 memory_read_data_interrupt (MQX_FILE_PTR spifd, uint_32 addr, uint_32 size, uchar_ptr data)
{
   uint_32 i, param;
   _mqx_int result;
   
   printf ("Reading %d bytes from location 0x%08x in memory: ", size, addr);

   send_buffer[0] = SPI_MEMORY_READ_DATA;                                                // Read instruction
   for (result = SPI_MEMORY_ADDRESS_BYTES; result != 0; result--)
   {
      send_buffer[result] = (uchar)((addr >> ((SPI_MEMORY_ADDRESS_BYTES - result) << 3)) & 0xFF); // Address
   }

   /* Write instruction and address */
   result = 0;
   do 
   {
      result += fwrite (send_buffer + result, 1, 1 + SPI_MEMORY_ADDRESS_BYTES - result, spifd);
   } while (result < 1 + SPI_MEMORY_ADDRESS_BYTES);

   if (result != 1 + SPI_MEMORY_ADDRESS_BYTES)
   {
      /* Stop transfer */
      printf ("ERROR (1)\n");
      return 0;
   }

   /* Get actual flags */
   if (SPI_OK != ioctl (spifd, IO_IOCTL_SPI_GET_FLAGS, &param)) 
   {
      printf ("ERROR (2)\n");
      return 0;
   }

   /* Flush but do not de-assert CS */
   param |= SPI_FLAG_NO_DEASSERT_ON_FLUSH;
   if (SPI_OK != ioctl (spifd, IO_IOCTL_SPI_SET_FLAGS, &param)) 
   {
      printf ("ERROR (3)\n");
      return 0;
   }
   fflush (spifd);
  
   /* Read size bytes of data */
   result = 0;
   do 
   {
      result += fread (data + result, 1, (_mqx_int)(size - result), spifd);
   } while (result < size);

   /* Wait till transfer end and de-assert CS */
   if (SPI_OK != ioctl (spifd, IO_IOCTL_SPI_FLUSH_DEASSERT_CS, &param)) 
   {
      printf ("ERROR (4)\n");
      return 0;
   }

   /* Get actual flags */
   if (SPI_OK != ioctl (spifd, IO_IOCTL_SPI_GET_FLAGS, &param)) 
   {
      printf ("ERROR (5)\n");
      return 0;
   }

   /* Revert opening flags */
   param &= (~ SPI_FLAG_NO_DEASSERT_ON_FLUSH);
   if (SPI_OK != ioctl (spifd, IO_IOCTL_SPI_SET_FLAGS, &param)) 
   {
      printf ("ERROR (6)\n");
      return 0;
   }

   if (result != size) 
   {
      printf ("ERROR (7)\n"); 
      return 0;
   } 
   else
   {
      printf ("\n"); 
      for (i = 0; i < result; i++)
         printf ("%c", data[i]);
      printf ("\n"); 
   }
   
   return size;
}

/* EOF */

