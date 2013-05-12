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
* $FileName: i2s_int_ki2s.c$
* $Version : 3.8.B.3$
* $Date    : Jun-13-2012$
*
* Comments:
*
*   This file contains low level functions for the I2S interrupt device driver
*   for Kinetis family.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <lwevent.h>
#include "i2s.h"
#include "i2s_int_prv.h"
#include "i2s_ki2s_prv.h"

uint_32 _ki2s_int_init (IO_I2S_DEVICE_STRUCT_PTR, char_ptr);
uint_32 _ki2s_int_deinit (IO_I2S_DEVICE_STRUCT_PTR, KI2S_INFO_STRUCT_PTR);
uint_32 _ki2s_int_rx (IO_I2S_DEVICE_STRUCT_PTR, char_ptr, uint_32);
uint_32 _ki2s_int_tx (IO_I2S_DEVICE_STRUCT_PTR, char_ptr, uint_32);
uint_32 _ki2s_int_ioctl (KI2S_INFO_STRUCT_PTR io_info_ptr, uint_32 cmd, pointer param_ptr);
static void _ki2s_int_isr (pointer);
static uint_32 _ki2s_format_in_data(char_ptr input, AUDIO_DATA_FORMAT_PTR format);
static void _ki2s_format_in_data_end (void);
static void _ki2s_format_out_data(uint_32 input, char_ptr output, AUDIO_DATA_FORMAT_PTR format);
static uint_32 _ki2s_sine_wave(uint_32 fs, uint_32 freq, int_16_ptr* output);
static _mqx_int _ki2s_hw_init(KI2S_INIT_STRUCT_PTR, I2S_MemMapPtr);

/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_int_install
* Returned Value   : MQX error code
* Comments         :
*    Install an I2S device.
*
*END*********************************************************************/

uint_32 _ki2s_int_install
   (
      /* [IN] A string that identifies the device for fopen */
      char_ptr           identifier,

      /* [IN] The I/O init data pointer */
      KI2S_INIT_STRUCT_CPTR  init_data_ptr
   )
{ /* Body */

   return _io_i2s_install(identifier,
      (uint_32 (_CODE_PTR_)(pointer, char_ptr))_ki2s_int_init,
      (uint_32 (_CODE_PTR_)(pointer, pointer))_ki2s_int_deinit,
      (int_32 (_CODE_PTR_)(pointer, char_ptr, int_32))_ki2s_int_rx,
      (int_32 (_CODE_PTR_)(pointer, char_ptr, int_32))_ki2s_int_tx,
      (int_32 (_CODE_PTR_)(pointer, uint_32, uint_32_ptr))_ki2s_int_ioctl,
      (pointer)init_data_ptr);

} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_format_in_data_init_ram_function
* Returned Value   : pointer to format_in_data in ram.
* Comments         :
*
*
*END*********************************************************************/

static char_ptr _ki2s_format_in_data_init_ram_function
(
      void
)
{
    char_ptr ram_code_ptr;

    /* allocate space to run flash command out of RAM */
    ram_code_ptr = _mem_alloc_system_zero((char_ptr)_ki2s_format_in_data_end  - (char_ptr)_ki2s_format_in_data);
    if(NULL == ram_code_ptr)
    {
       #ifdef I2S_DEBUG
       printf("_i2s_init: Error - unable to alloc memory.\n");
       #endif

    }
    /* copy code to RAM buffer */
    _mem_copy ((char_ptr)_ki2s_format_in_data - 1, ram_code_ptr, (char_ptr)_ki2s_format_in_data_end  - (char_ptr)_ki2s_format_in_data);

    return ram_code_ptr;
}

/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_int_init
* Returned Value   : MQX error code
* Comments         :
*    This function initializes an I2S device.
*
*END*********************************************************************/

uint_32 _ki2s_int_init
   (
      /* [IN] Initialization information for the device being opened */
      IO_I2S_DEVICE_STRUCT_PTR  io_i2s_dev_ptr,

      /* [IN] The rest of the name of the device opened */
      char_ptr                      open_name_ptr

   )
{ /* Body */
   I2S_MemMapPtr        i2s_ptr;
   KI2S_INIT_STRUCT_PTR i2s_init_ptr;
   KI2S_INFO_STRUCT_PTR io_info_ptr;
   uint_8               channel, mode, data_bits, clk_source, endianity, aligment, bits, size, channels;
   _mqx_uint              vector, buffer_size;
   _mqx_uint            result = 0;

   /*Variables initialization*/
   i2s_init_ptr = (KI2S_INIT_STRUCT_PTR)(io_i2s_dev_ptr->DEV_INIT_DATA_PTR);
   if (i2s_init_ptr == NULL)
   {
      #ifdef I2S_DEBUG
       printf("_i2s_init: Error - Invalid init struct pointer.\n");
      #endif
      return(I2S_ERROR_INVALID_PARAMETER);
   }
   else
   {
      channel = i2s_init_ptr->CHANNEL;
      mode = i2s_init_ptr->MODE;
      data_bits = i2s_init_ptr->DATA_BITS;
      clk_source = i2s_init_ptr->CLOCK_SOURCE;
      buffer_size = i2s_init_ptr->BUFFER_SIZE;
      endianity = i2s_init_ptr->IO_FORMAT->ENDIAN;
      aligment = i2s_init_ptr->IO_FORMAT->ALIGNMENT;
      bits = i2s_init_ptr->IO_FORMAT->BITS;
      size = i2s_init_ptr->IO_FORMAT->SIZE;
      channels = i2s_init_ptr->IO_FORMAT->CHANNELS;
   }

   if ((data_bits < 8) || (data_bits > 24) || (data_bits == 14) || (data_bits % 2))
   {
      #ifdef I2S_DEBUG
       printf("_i2s_init: Error - Unsupported data word length.\n");
      #endif
      return (I2S_ERROR_WORD_LENGTH_UNSUPPORTED);
   }

   if ((channel > 1) || (channel < 0))
   {
      #ifdef I2S_DEBUG
       printf("_i2s_init: Error - Invalid I2S channel.\n");
      #endif
      return(I2S_ERROR_CHANNEL_INVALID);
   }

   if (mode > I2S_MODE_SLAVE)
   {
      #ifdef I2S_DEBUG
       printf("_i2s_init: Error - Invalid I2S mode.\n");
      #endif
      return(I2S_ERROR_MODE_INVALID);
   }
   if (clk_source > I2S_CLK_EXT)
   {
      #ifdef I2S_DEBUG
       printf("_i2s_init: Error - Invalid I2S clock source.\n");
      #endif
      return(I2S_ERROR_CLK_INVALID);
   }
   if (buffer_size < 2)
   {
      #ifdef I2S_DEBUG
       printf("_i2s_init: Error - I2S transmit buffer size too small.\n");
      #endif
      return(I2S_ERROR_BUFFER_SMALL);
   }
   if ((aligment > AUDIO_ALIGNMENT_LEFT) || (bits < AUDIO_BIT_SIZE_MIN) ||
      (bits > AUDIO_BIT_SIZE_MAX) || (endianity > AUDIO_LITTLE_ENDIAN) ||
      (size * I2S_CHAR_BIT < bits) || (channels > 2) || (channels == 0))
   {
      #ifdef I2S_DEBUG
        printf("_i2s_init: Error - Invalid I/O data format.\n");
      #endif
      return(AUDIO_ERROR_INVALID_IO_FORMAT);
   }
   i2s_ptr = _bsp_get_i2s_base_address(channel);
   io_info_ptr = (KI2S_INFO_STRUCT_PTR) _mem_alloc_system_zero ((uint_32)sizeof (KI2S_INFO_STRUCT));
   if (NULL == io_info_ptr)
   {
      return MQX_OUT_OF_MEMORY;
   }
   _mem_set_type (io_info_ptr, MEM_TYPE_IO_I2S_INFO_STRUCT);

   /* I2S board specific IO pins initialization */
   _bsp_i2s_io_init(channel);
   /* I2S board specific registers initialization */
   _ki2s_hw_init(i2s_init_ptr, i2s_ptr);

   /*Info struct setup*/
   io_info_ptr->INIT = *i2s_init_ptr;
   io_info_ptr->I2S_PTR = i2s_ptr;
   io_info_ptr->MODE = i2s_init_ptr->MODE;
   io_info_ptr->DATA_BITS = i2s_init_ptr->DATA_BITS;
   io_info_ptr->CLOCK_SOURCE = i2s_init_ptr->CLOCK_SOURCE;
   if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_INT)
   {
      io_info_ptr->MCLK_FREQ = BSP_SYSTEM_CLOCK / SSIDIV_MIN;
      io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / 4;
      /* DIV2 = 1 in default and 2 is inner division */
      io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / io_info_ptr->DATA_BITS;
   }
   else
   {
      io_info_ptr->MCLK_FREQ = 0;
      io_info_ptr->BCLK_FREQ = 0;
      io_info_ptr->FS_FREQ = 0;
   }
   io_info_ptr->MCLK_DIV = SSIDIV_MIN;
   io_info_ptr->BCLK_DIV = 4;
   io_info_ptr->TX_DUMMY = i2s_init_ptr->TX_DUMMY;
   io_info_ptr->ONTHEWAY = FALSE;
   io_info_ptr->BUFFER_L.IN = 0;
   io_info_ptr->BUFFER_L.OUT = 0;
   io_info_ptr->BUFFER_L.DATA = NULL;
   io_info_ptr->BUFFER_L.SIZE = 0;
   io_info_ptr->BUFFER_L.SPACE = 0;
   io_info_ptr->BUFFER_R.IN = 0;
   io_info_ptr->BUFFER_R.OUT = 0;
   io_info_ptr->BUFFER_R.DATA = NULL;
   io_info_ptr->BUFFER_R.SIZE = 0;
   io_info_ptr->BUFFER_R.SPACE = 0;
   io_info_ptr->STATS.INTERRUPTS = 0;
   io_info_ptr->STATS.UNDERRUNS_L = 0;
   io_info_ptr->STATS.UNDERRUNS_R = 0;
   io_info_ptr->STATS.OVERRUNS_L = 0;
   io_info_ptr->STATS.OVERRUNS_R = 0;
   io_info_ptr->STATS.RX_PACKETS = 0;
   io_info_ptr->STATS.TX_PACKETS = 0;
   io_info_ptr->STATS.PACKETS_REQUESTED_L = 0;
   io_info_ptr->STATS.PACKETS_PROCESSED_L = 0;
   io_info_ptr->STATS.PACKETS_QUEUED_L = 0;
   io_info_ptr->STATS.PACKETS_REQUESTED_R = 0;
   io_info_ptr->STATS.PACKETS_PROCESSED_R = 0;
   io_info_ptr->STATS.PACKETS_QUEUED_R = 0;
   io_info_ptr->DATA_L = NULL;
   io_info_ptr->DATA_R = NULL;
   io_info_ptr->EVENT_IO_FINISHED = _mem_alloc_system_zero(sizeof(LWEVENT_STRUCT));
   result = _lwevent_create(io_info_ptr->EVENT_IO_FINISHED, LWEVENT_AUTO_CLEAR);
   if (result != MQX_OK) {
      #ifdef I2S_DEBUG
      printf("i2s_init - Error: Unable to create event_int_done.\n");
      #endif
      _task_set_error(result);
   }
   io_info_ptr->IO_FORMAT.ENDIAN = endianity;
   io_info_ptr->IO_FORMAT.BITS = bits;
   io_info_ptr->IO_FORMAT.ALIGNMENT = aligment;
   io_info_ptr->IO_FORMAT.SIZE = size;
   io_info_ptr->IO_FORMAT.CHANNELS = channels;
   io_info_ptr->DUMMY.DATA = NULL;
   io_info_ptr->DUMMY.LENGTH = 0;
   io_info_ptr->DUMMY.INDEX_L = 0;
   io_info_ptr->DUMMY.INDEX_R = 0;
   io_info_ptr->CLK_ALWAYS_ON = TRUE;
   io_info_ptr->FIRST_IO = TRUE;

   /* Buffers allocation */
   io_info_ptr->BUFFER_L.DATA = (uint_32_ptr) _mem_alloc_system(buffer_size * sizeof(uint_32));
   if (io_info_ptr->BUFFER_L.DATA == NULL)
   {
      return MQX_OUT_OF_MEMORY;
   }
   io_info_ptr->BUFFER_L.SPACE = buffer_size;
   io_info_ptr->BUFFER_L.SIZE = buffer_size;

   io_info_ptr->BUFFER_R.DATA = (uint_32_ptr) _mem_alloc_system(buffer_size * sizeof(uint_32));
   if (io_info_ptr->BUFFER_R.DATA == NULL)
   {
      uint_32 errcode = _mem_free(io_info_ptr->BUFFER_L.DATA);
      if (errcode != MQX_OK)
      {
         return errcode;
      }
      io_info_ptr->BUFFER_L.DATA = NULL;
      io_info_ptr->BUFFER_L.SPACE = 0;
      io_info_ptr->BUFFER_L.SIZE = 0;
      return MQX_OUT_OF_MEMORY;
   }
   io_info_ptr->BUFFER_R.SPACE = buffer_size;
   io_info_ptr->BUFFER_R.SIZE = buffer_size;

   _mem_set_type (io_info_ptr->BUFFER_L.DATA, MEM_TYPE_IO_I2S_LEFT_BUFFER);
   _mem_set_type (io_info_ptr->BUFFER_R.DATA, MEM_TYPE_IO_I2S_RIGHT_BUFFER);

   io_i2s_dev_ptr->DEV_INFO_PTR = io_info_ptr;

   /*Interrupt vector setup*/
   vector = _bsp_get_i2s_vector(channel);
   io_info_ptr->OLD_ISR_DATA = _int_get_isr_data (vector);
   io_info_ptr->OLD_ISR = _int_install_isr (vector, _ki2s_int_isr, io_info_ptr);
   _bsp_int_init((IRQInterruptIndex)_bsp_get_i2s_vector(channel), io_info_ptr->INIT.LEVEL, 0, TRUE);

   return(I2S_OK);
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_int_deinit
* Returned Value   : MQX error code
* Comments         :
*    This function de-initializes an I2S device.
*
*END*********************************************************************/

uint_32 _ki2s_int_deinit
   (
      /* [IN] the initialization information for the device being opened */
      IO_I2S_DEVICE_STRUCT_PTR  io_i2s_dev_ptr,

      /* [IN] the address of the device specific information */
      KI2S_INFO_STRUCT_PTR   io_info_ptr
   )
{ /* Body */
   I2S_MemMapPtr          i2s_ptr;
   KI2S_INIT_STRUCT_PTR   i2s_init_ptr;
   _mqx_uint              errcode;
   _mqx_uint              result = 0;

   if ((NULL == io_info_ptr) || (NULL == io_i2s_dev_ptr))
   {
      return I2S_ERROR_INVALID_PARAMETER;
   }

   i2s_ptr = io_info_ptr->I2S_PTR;

   /*SSI disable*/
   i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK);

   /* Clear the SSI events */
   i2s_ptr->ISR = 0x00;

   /* Install original vectors */
   i2s_init_ptr = (KI2S_INIT_STRUCT_PTR)(io_i2s_dev_ptr->DEV_INIT_DATA_PTR);
   _bsp_int_init((IRQInterruptIndex)_bsp_get_i2s_vector(i2s_init_ptr->CHANNEL), i2s_init_ptr->LEVEL, 0, TRUE);
   _int_install_isr (_bsp_get_i2s_vector (i2s_init_ptr->CHANNEL), io_info_ptr->OLD_ISR, io_info_ptr->OLD_ISR_DATA);
   /* Destroy event */
   result = _lwevent_destroy(io_info_ptr->EVENT_IO_FINISHED);
   if (result != MQX_OK)
   {
      #ifdef I2S_DEBUG
      printf("i2s_rx - Error: unable to destroy event_int_done.\n");
      #endif
      _task_set_error(result);
   }
   /* Free buffers and info struct */
   errcode = _mem_free (io_info_ptr->BUFFER_L.DATA);
   if (errcode != MQX_OK)
   {
      return errcode;
   }
   io_info_ptr->BUFFER_L.IN = 0;
   io_info_ptr->BUFFER_L.OUT = 0;
   io_info_ptr->BUFFER_L.DATA = NULL;
   io_info_ptr->BUFFER_L.SPACE = 0;
   io_info_ptr->BUFFER_L.SIZE = 0;

   errcode = _mem_free (io_info_ptr->BUFFER_R.DATA);
   if (errcode != MQX_OK)
   {
      return errcode;
   }
   io_info_ptr->BUFFER_R.IN = 0;
   io_info_ptr->BUFFER_R.OUT = 0;
   io_info_ptr->BUFFER_R.DATA = NULL;
   io_info_ptr->BUFFER_R.SPACE = 0;
   io_info_ptr->BUFFER_R.SIZE = 0;

   if (io_info_ptr->TX_DUMMY)
   {
      errcode = _mem_free (io_info_ptr->DUMMY.DATA);
      if (errcode != MQX_OK)
      {
         return errcode;
      }
      io_info_ptr->DUMMY.DATA = NULL;
   }

   if(io_info_ptr->EVENT_IO_FINISHED) {
      errcode = _mem_free (io_info_ptr->EVENT_IO_FINISHED);
      if (errcode != MQX_OK)
      {
         return errcode;
      }
      io_info_ptr->EVENT_IO_FINISHED = NULL;
   }

   errcode = _mem_free (io_i2s_dev_ptr->DEV_INFO_PTR);
   if (errcode != MQX_OK)
   {
      return errcode;
   }
   io_i2s_dev_ptr->DEV_INFO_PTR = NULL;
   return I2S_OK;
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_int_rx
* Returned Value   : number of bytes read
* Comments         :
*   Returns the number of bytes received.
*   Reads the data into provided array when data is available.
*
*END*********************************************************************/

uint_32 _ki2s_int_rx
   (
      /* [IN] the address of the device specific information */
      IO_I2S_DEVICE_STRUCT_PTR   io_i2s_dev_ptr,

      /* [IN] The array to copy data into */
      char_ptr                      buffer,

    /* [IN] number of bytes to read */
      uint_32                       length
   )
{ /* Body */
   KI2S_INFO_STRUCT_PTR             io_info_ptr   = io_i2s_dev_ptr->DEV_INFO_PTR;
   I2S_MemMapPtr                    i2s_ptr = io_info_ptr->I2S_PTR;
   KI2S_BUFFER_PTR                  buffer_l = &(io_info_ptr->BUFFER_L);
   KI2S_BUFFER_PTR                  buffer_r = &(io_info_ptr->BUFFER_R);
   I2S_STATISTICS_STRUCT_PTR        stats = &(io_info_ptr->STATS);
   AUDIO_DATA_FORMAT_PTR            io_format = &(io_info_ptr->IO_FORMAT);
   LWEVENT_STRUCT_PTR               event_int_done = io_info_ptr->EVENT_IO_FINISHED;
   _mqx_uint                        result = MQX_OK;
   uint_32                          i = 0;

   /* disable transmitter interrupts */
   i2s_ptr->IER &= ~(
      I2S_IER_TUE0EN_MASK | /* Transmit underrun error 0*/
      I2S_IER_TFE0EN_MASK | /* Transmit FIFO0 empty */
      I2S_IER_TUE1EN_MASK | /* Transmit underrun error 0*/
      I2S_IER_TFE1EN_MASK /* Transmit FIFO0 empty */
      );

   i2s_ptr->IER |= (
      I2S_IER_ROE0EN_MASK | /* Receive overrun error 0*/
      I2S_IER_RFF0EN_MASK | /* Receive FIFO0 full */
      I2S_IER_ROE1EN_MASK | /* Receive overrun error 0*/
      I2S_IER_RFF1EN_MASK   /* Receive FIFO0 full */
      );
   io_info_ptr->IO_MODE = I2S_IO_READ;
   /* Init stats and buffers to default values */
    stats->PACKETS_REQUESTED_L += length/(io_format->CHANNELS * io_format->SIZE);
    switch (io_format->CHANNELS)
   {
      case 1:
         io_info_ptr->DATA_L = buffer;
         io_info_ptr->DATA_R = buffer;
      break;

      default:
         io_info_ptr->DATA_L = buffer;
          io_info_ptr->DATA_R = buffer + io_format->SIZE;
          stats->PACKETS_REQUESTED_R += length/(io_format->CHANNELS * io_format->SIZE);
      break;
   }

   io_info_ptr->ONTHEWAY = TRUE;

   while(io_info_ptr->ONTHEWAY)
   {
       if (io_info_ptr->FIRST_IO) /* enable receiver and interrupts in case of first run to get some data */
      {
         io_info_ptr->FIRST_IO = FALSE;
         i2s_ptr->IER |= (I2S_IER_RIE_MASK); /* Receive interrupts enable */
         i2s_ptr->CR |= (I2S_CR_RE_MASK); /* Enable receiver */
      }
      /* Wait for receive finish */
      result = _lwevent_wait_ticks(event_int_done, 1, TRUE, 0);
      if (result != MQX_OK)
      {
         #ifdef I2S_DEBUG
          printf("i2s_rx - Error: Wait for event_int_done failed.\n");
         #endif
         _task_set_error(result);
      }
        /* All data received, disable receiver */
        if ((stats->PACKETS_QUEUED_L == stats->PACKETS_REQUESTED_L) &&
            (stats->PACKETS_QUEUED_R == stats->PACKETS_REQUESTED_R))
        {
            io_info_ptr->ONTHEWAY = FALSE;
        }
        while ((buffer_l->SPACE < buffer_l->SIZE) &&
               (stats->PACKETS_PROCESSED_L < stats->PACKETS_QUEUED_L) &&
               (stats->PACKETS_PROCESSED_L < stats->PACKETS_REQUESTED_L))
      {
         if (buffer_l->OUT >= buffer_l->SIZE)
         {
            buffer_l->OUT = 0;
         }
         /* Critical Section - disable interrupt */
         _int_disable();
         i2s_ptr->IER &= ~(I2S_IER_RFF0EN_MASK);
         _int_enable();
         _ki2s_format_out_data(buffer_l->DATA[buffer_l->OUT], io_info_ptr->DATA_L, io_format);
         buffer_l->OUT++;
            io_info_ptr->DATA_L += io_format->CHANNELS * io_format->SIZE;
         stats->PACKETS_PROCESSED_L++;
         buffer_l->SPACE++;
         /* End of critical section*/
         i2s_ptr->IER |= (I2S_IER_RFF0EN_MASK);
      }

      while ((buffer_r->SPACE < buffer_r->SIZE) &&
             (stats->PACKETS_PROCESSED_R < stats->PACKETS_QUEUED_R) &&
             (stats->PACKETS_PROCESSED_R < stats->PACKETS_REQUESTED_R))
      {
         if (buffer_r->OUT >= buffer_r->SIZE)
         {
            buffer_r->OUT = 0;
         }
         /* Critical Section - disable interrupt */
         _int_disable();
         i2s_ptr->IER &= ~(I2S_IER_RFF1EN_MASK);
         _int_enable();
         *(io_info_ptr->DATA_R) = buffer_r->DATA[buffer_r->OUT];
         _ki2s_format_out_data(buffer_r->DATA[buffer_r->OUT], io_info_ptr->DATA_R, io_format);
         buffer_r->OUT++;
            io_info_ptr->DATA_R += io_format->CHANNELS * io_format->SIZE;
         stats->PACKETS_PROCESSED_R++;
         buffer_r->SPACE++;
         /* End of critical section*/
         i2s_ptr->IER |= (I2S_IER_RFF1EN_MASK);
      }
   }
    return((stats->PACKETS_PROCESSED_R * io_format->SIZE) + (stats->PACKETS_PROCESSED_L * io_format->SIZE));
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_int_tx
* Returned Value   : number of bytes written
* Comments         :
*   Returns the number of bytes written.
*   Writes the data provided into transmition buffer if available.
*
*END*********************************************************************/

uint_32 _ki2s_int_tx
   (
      /* [IN] the address of the device specific information */
      IO_I2S_DEVICE_STRUCT_PTR    io_i2s_dev_ptr,

      /* [IN] The array to be read from */
      char_ptr                       buffer,

      /* [IN] number of packets to output */
      uint_32                         length
   )
{ /* Body */
   KI2S_INFO_STRUCT_PTR       io_info_ptr   = io_i2s_dev_ptr->DEV_INFO_PTR;
   I2S_MemMapPtr              i2s_ptr = io_info_ptr->I2S_PTR;
   KI2S_BUFFER_PTR            buffer_l = &(io_info_ptr->BUFFER_L);
   KI2S_BUFFER_PTR            buffer_r = &(io_info_ptr->BUFFER_R);
   I2S_STATISTICS_STRUCT_PTR  stats = &(io_info_ptr->STATS);
   AUDIO_DATA_FORMAT_PTR      io_format = &(io_info_ptr->IO_FORMAT);
   LWEVENT_STRUCT_PTR         event_int_done = io_info_ptr->EVENT_IO_FINISHED;
   _mqx_uint                  result = MQX_OK;
   pointer                    ram_function = NULL;

   uint_32 (* FormatInDataRunInRAM)(char_ptr,AUDIO_DATA_FORMAT_PTR);

   ram_function = (pointer)_ki2s_format_in_data_init_ram_function();

   FormatInDataRunInRAM = (uint_32 (*)(char_ptr,AUDIO_DATA_FORMAT_PTR))((uchar_ptr)ram_function + 1);

   i2s_ptr->IER |= (
      I2S_IER_TUE0EN_MASK | /* Transmit underrun error 0*/
      I2S_IER_TFE0EN_MASK | /* Transmit FIFO0 empty */

      I2S_IER_TUE1EN_MASK | /* Transmit underrun error 0*/
      I2S_IER_TFE1EN_MASK /* Transmit FIFO0 empty */

      );

   i2s_ptr->IER &= ~(

         I2S_IER_ROE0EN_MASK | /* Receive overrun error 0*/
         I2S_IER_RFF0EN_MASK | /* Receive FIFO0 full */
         I2S_IER_ROE1EN_MASK | /* Receive overrun error 0*/
         I2S_IER_RFF1EN_MASK   /* Receive FIFO0 full */
         );

   if ((buffer == NULL) && !(io_info_ptr->TX_DUMMY))
   {
      _task_set_error(I2S_ERROR_INVALID_PARAMETER);
      return (0);
   }
   else
   {
      switch (io_format->CHANNELS)
      {
         case 1:
            io_info_ptr->DATA_L = buffer;
            io_info_ptr->DATA_R = buffer;
         break;

         default:
            io_info_ptr->DATA_L = buffer;
            io_info_ptr->DATA_R = buffer + io_format->SIZE;
         break;
      }
   }

   io_info_ptr->IO_MODE = I2S_IO_WRITE;
   /* Init stats and buffers */
   stats->PACKETS_REQUESTED_R += length/io_format->CHANNELS/io_format->SIZE;
   stats->PACKETS_REQUESTED_L += length/io_format->CHANNELS/io_format->SIZE;
   if (io_info_ptr->TX_DUMMY)
   {
      stats->PACKETS_QUEUED_L = stats->PACKETS_REQUESTED_L;
      stats->PACKETS_QUEUED_R = stats->PACKETS_REQUESTED_R;
   }

   io_info_ptr->ONTHEWAY = TRUE;

   while(io_info_ptr->ONTHEWAY)
   {
      if (!(io_info_ptr->TX_DUMMY))
      {
         /* Copy data from input to left channel buffer */
         while ((buffer_l->SPACE > 0) && (stats->PACKETS_QUEUED_L < stats->PACKETS_REQUESTED_L))
         {
            if (buffer_l->IN >= buffer_l->SIZE)
            {
               buffer_l->IN = 0;
            }

            /* buffer access, critical section - disable interrupt with buffer access */
            i2s_ptr->IER &= ~(I2S_IER_TFE0EN_MASK);
            buffer_l->DATA[buffer_l->IN++] = FormatInDataRunInRAM(io_info_ptr->DATA_L, io_format);
            buffer_l->SPACE--;
            stats->PACKETS_QUEUED_L++;
            /* buffer only even samples */
            io_info_ptr->DATA_L += io_format->CHANNELS * io_format->SIZE;
         }

         /* Copy data from input to right channel buffer */
         while ((buffer_r->SPACE > 0) && (stats->PACKETS_QUEUED_R < stats->PACKETS_REQUESTED_R))
         {
            if (buffer_r->IN >= buffer_r->SIZE)
            {
               buffer_r->IN = 0;
            }

            /* buffer access, critical section - disable interrupt with buffer access */
            i2s_ptr->IER &= ~(I2S_IER_TFE1EN_MASK);
            buffer_r->DATA[buffer_r->IN++] = FormatInDataRunInRAM(io_info_ptr->DATA_R, io_format);
            buffer_r->SPACE--;
            stats->PACKETS_QUEUED_R++;
            /* buffer only odd samples */
            io_info_ptr->DATA_R += io_format->CHANNELS * io_format->SIZE;
         }
         /* critical section end - enable interrupt */
         i2s_ptr->IER |= (I2S_IER_TFE0EN_MASK | I2S_IER_TFE1EN_MASK);
      }
      if (io_info_ptr->FIRST_IO)
      {
         io_info_ptr->FIRST_IO = FALSE;
         i2s_ptr->IER |= (I2S_IER_TIE_MASK); /* Transmit interrupts enable*/
         i2s_ptr->CR |= (I2S_CR_TE_MASK); /* Enable transmitter */
      }

      /* If all requested packets are queued break and return from function */
      if ((stats->PACKETS_QUEUED_L == stats->PACKETS_REQUESTED_L) && (stats->PACKETS_QUEUED_R == stats->PACKETS_REQUESTED_R))
      {
         break;
      }

      /* Wait for transmit finish and make current task blocked */
      result = _lwevent_wait_ticks(event_int_done, 1, TRUE, 0);

      if (result != MQX_OK)
      {
         #ifdef I2S_DEBUG
          printf("i2s_tx - Error: Wait for event_int_done failed.\n");
         #endif
         _task_set_error(result);
      }
   }
   _mem_free(ram_function);
   /* return number of processed data */
   return((stats->PACKETS_PROCESSED_R * io_format->SIZE) + (stats->PACKETS_PROCESSED_L * io_format->SIZE));
} /* Endbody */


/*FUNCTION****************************************************************
*
* Function Name    :_ki2s_int_isr
* Returned Value   : none
*
*END*********************************************************************/
static void _ki2s_int_isr
   (
      pointer                  parameter
   )
{ /* Body */
   KI2S_INFO_STRUCT_PTR       io_info_ptr = parameter;
   I2S_MemMapPtr              i2s_ptr = io_info_ptr->I2S_PTR;
   KI2S_BUFFER_PTR            buffer_l = &(io_info_ptr->BUFFER_L);
   KI2S_BUFFER_PTR            buffer_r = &(io_info_ptr->BUFFER_R);
   I2S_STATISTICS_STRUCT_PTR  stats = &(io_info_ptr->STATS);
   KI2S_DUMMY_DATA_PTR        dummy = &(io_info_ptr->DUMMY);

   stats->INTERRUPTS++;

   if (io_info_ptr->IO_MODE == I2S_IO_READ)
   {
      /* Receive FIFO0 full */
      if (i2s_ptr->ISR & I2S_ISR_RFF0_MASK)
      {
         uint_8 i, data_n;
         /* Read value of Receive FIFO Counter0 */
         data_n = ((i2s_ptr->FCSR >> 12) & 0xF);
         if (stats->PACKETS_QUEUED_L + data_n >= stats->PACKETS_REQUESTED_L)
         {
            data_n = stats->PACKETS_REQUESTED_L - stats->PACKETS_QUEUED_L;
         }
         if (data_n > buffer_l->SPACE)
         {
            data_n = buffer_l->SPACE;
         }
         /* Copy data from left channel FIFO to buffer */
         for (i = 0; i < data_n; i++)
         {
            if (buffer_l->IN >= buffer_l->SIZE)
            {
               buffer_l->IN = 0;
            }
            buffer_l->DATA[buffer_l->IN++] = i2s_ptr->RX0;
         }
         stats->RX_PACKETS += data_n;
         stats->PACKETS_QUEUED_L += data_n;
         buffer_l->SPACE -= data_n;
      }

      /* Receive FIFO1 full */
      if (i2s_ptr->ISR & I2S_ISR_RFF1_MASK)
      {
         uint_8 i, data_n;
         /* Read value of Receive FIFO Counter1 */
         data_n = ((i2s_ptr->FCSR >> 28) & 0xF);
         if (stats->PACKETS_QUEUED_R + data_n >= stats->PACKETS_REQUESTED_R)
         {
            data_n = stats->PACKETS_REQUESTED_R - stats->PACKETS_QUEUED_R;
         }
         if (data_n > buffer_r->SPACE)
         {
            data_n = buffer_r->SPACE;
         }
         /* Copy data from right channel FIFO to buffer */
         for (i = 0; i < data_n; i++)
         {
            if (buffer_r->IN >= buffer_r->SIZE)
            {
               buffer_r->IN = 0;
            }
            buffer_r->DATA[buffer_r->IN++] = i2s_ptr->RX1;
         }
         stats->RX_PACKETS += data_n;
         stats->PACKETS_QUEUED_R += data_n;
         buffer_r->SPACE -= data_n;
      }

      /* Receiver overrun error 0 */
      if (i2s_ptr->ISR & I2S_ISR_ROE0_MASK)
      {
         #ifdef I2S_DEBUG
          printf("i2s_isr - Error: Receiver 0 overrun.\n");
         #endif
         stats->OVERRUNS_L++;
         /* Clear error flag */
         i2s_ptr->ISR |= (I2S_ISR_ROE0_MASK);
      }

      /* Receiver overrun error 1 */
      if (i2s_ptr->ISR & I2S_ISR_ROE1_MASK)
      {
         #ifdef I2S_DEBUG
           printf("i2s_isr - Error: Receiver 1 overrun.\n");
         #endif
         stats->OVERRUNS_R++;
         /* Clear error flag */
         i2s_ptr->ISR |= (I2S_ISR_ROE1_MASK);
      }
   }
   else
   {
      /* All data transmitted, disable transmitter */
      if ((stats->PACKETS_PROCESSED_L == stats->PACKETS_REQUESTED_L) &&
         (stats->PACKETS_PROCESSED_R == stats->PACKETS_REQUESTED_R) &&
         (i2s_ptr->ISR & I2S_ISR_TFE0_MASK) &&
         (i2s_ptr->ISR & I2S_ISR_TFE1_MASK))
      {
         i2s_ptr->CR &= ~(I2S_CR_TE_MASK);
         _int_disable();
         i2s_ptr->IER &= ~(I2S_IER_TIE_MASK);
         _int_enable();
         io_info_ptr->ONTHEWAY = FALSE;
         io_info_ptr->FIRST_IO = TRUE;
      }

      if ((stats->PACKETS_PROCESSED_L == stats->PACKETS_QUEUED_L) &&
          (stats->PACKETS_PROCESSED_R == stats->PACKETS_QUEUED_R))
      {
         i2s_ptr->IER &= ~(I2S_IER_TFE1EN_MASK | I2S_IER_TFE0EN_MASK);
      }

      /* Transmit FIFO0 empty - need more data for left channel*/
      if (i2s_ptr->ISR & I2S_ISR_TFE0_MASK)
      {
         uint_8 i, space;
         /* Read value of Transmit FIFO Counter0 */
         space = FIFO_WATERMARK_MAX - ((i2s_ptr->FCSR >> I2S_CHAR_BIT) & 0xF);
         /* Copy data from buffer to left channel FIFO */
         for (i = 0; ((i < space) && (stats->PACKETS_PROCESSED_L < stats->PACKETS_QUEUED_L)); i++)
         {
            if (io_info_ptr->TX_DUMMY)
            {
               i2s_ptr->TX0 = (*(dummy->DATA + dummy->INDEX_L)) & 0xFFFFFFFF;
               if (dummy->INDEX_L++ >= dummy->LENGTH)
               {
                  dummy->INDEX_L = 0;
               }
               stats->PACKETS_PROCESSED_L++;
            }
            else
            {
               if (buffer_l->OUT >= buffer_l->SIZE)
               {
                  buffer_l->OUT = 0;
               }
               i2s_ptr->TX0 = buffer_l->DATA[buffer_l->OUT++];
               stats->TX_PACKETS++;
               stats->PACKETS_PROCESSED_L++;
               buffer_l->SPACE++;

               }
         }
      }
      /* Transmit FIFO1 empty - need more data for right channel  */
      if (i2s_ptr->ISR & I2S_ISR_TFE1_MASK)
      {
         uint_8 i, space;
         /* Read value of Transmit FIFO Counter1 */
         space = FIFO_WATERMARK_MAX - ((i2s_ptr->FCSR >> (I2S_CHAR_BIT * 3)) & 0xF);

         /* Copy data from buffer to right channel FIFO */
         for (i = 0; ((i < space) && (stats->PACKETS_PROCESSED_R < stats->PACKETS_QUEUED_R)); i++)
         {
            if (io_info_ptr->TX_DUMMY)
            {
               i2s_ptr->TX1 = (*(dummy->DATA + dummy->INDEX_R)) & 0xFFFFFFFF;
               if (dummy->INDEX_R++ >= dummy->LENGTH)
               {
                  dummy->INDEX_R = 0;
               }
               stats->PACKETS_PROCESSED_R++;
            }
            else
            {
               if (buffer_r->OUT >= buffer_r->SIZE)
               {
                  buffer_r->OUT = 0;
               }
               i2s_ptr->TX1 = buffer_r->DATA[buffer_r->OUT++];
               stats->TX_PACKETS++;
               stats->PACKETS_PROCESSED_R++;
               buffer_r->SPACE++;

            }
         }
      }

      /* Check for transmit underrun error 0 */
      if (i2s_ptr->ISR & I2S_ISR_TUE0_MASK)
      {
         #ifdef I2S_DEBUG
          printf("i2s_isr - Error: Left channel underrun.\n");
         #endif
         stats->UNDERRUNS_L++;
         /* Clear error flag */
         i2s_ptr->ISR |= (I2S_ISR_TUE0_MASK);
      }

      /* Check for transmit underrun error 1 */
      if (i2s_ptr->ISR & I2S_ISR_TUE1_MASK)
      {
         #ifdef I2S_DEBUG
           printf("i2s_isr - Error: Right channel underrun.\n");
         #endif
         stats->UNDERRUNS_R++;
         /* Clear error flag */
         i2s_ptr->ISR |= (I2S_ISR_TUE1_MASK);
      }
   }
   if ((NULL == io_info_ptr->EVENT_IO_FINISHED) || (0 == io_info_ptr->EVENT_IO_FINISHED->VALID))
      return;

   if (_lwevent_set(io_info_ptr->EVENT_IO_FINISHED,0x01) != MQX_OK)
   {
      #ifdef I2S_DEBUG
      printf("i2s_isr - Error: Unable to set EVENT_IO_FINISHED.\n");
      #endif
   }
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_int_ioctl
* Returned Value   : MQX error code.
* Comments         :
*    This function performs miscellaneous services for
*    the I2S I/O device.
*
*END*********************************************************************/

uint_32 _ki2s_int_ioctl
   (
      /* [IN] the address of the device specific information */
      KI2S_INFO_STRUCT_PTR io_info_ptr,

      /* [IN] The command to perform */
      uint_32                     cmd,

      /* [IN] Parameters for the command */
      pointer                 param_ptr
   )
{ /* Body */
   uint_32              result = MQX_OK;
   I2S_MemMapPtr        i2s_ptr = io_info_ptr->I2S_PTR;
   uint_8               watermark = 0;
   KI2S_DUMMY_DATA_PTR  dummy = &io_info_ptr->DUMMY;
   SIM_MemMapPtr        sim = SIM_BASE_PTR;

   switch(cmd)
   {
      case IO_IOCTL_I2S_SET_MODE_MASTER:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MODE_MASTER: Error - Cannot chnage I2S mode while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK);
         i2s_ptr->CR |= (I2S_CR_I2SMODE(0x01)); /* Master mode */

         if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_EXT)
         {
            i2s_ptr->CR &= ~(I2S_CR_SYSCLKEN_MASK); /* Internal master clock disable */
         }
         else if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_INT)
         {
            i2s_ptr->CR |= (I2S_CR_SYSCLKEN_MASK); /* Master clock enable */
            io_info_ptr->MCLK_FREQ = BSP_SYSTEM_CLOCK / io_info_ptr->MCLK_DIV;
            io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
            /* The word length is fixed to 32 in I2S master mode and DC is 1
            ** thus frame sync frequency is always BCLK_FREQ/64
            */
            io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / 64;
         }

         i2s_ptr->TCR |= (
                             I2S_TCR_TXDIR_MASK | /* Bit clock generated internally */
                             I2S_TCR_TFDIR_MASK    /* Frame sync generated internally */
                             );
         i2s_ptr->CR |= (I2S_CR_I2SEN_MASK);
         io_info_ptr->MODE = I2S_MODE_MASTER;
         break;

      case IO_IOCTL_I2S_SET_MODE_SLAVE:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MODE_SLAVE: Error - Cannot chnage I2S mode while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK);
         i2s_ptr->CR |= (I2S_CR_I2SMODE(0x02)); /* I2S Slave mode */
         i2s_ptr->CR &= ~(I2S_CR_SYSCLKEN_MASK);  /* Master clock disabled */
         i2s_ptr->TCR &= ~(
                             I2S_TCR_TXDIR_MASK |  /* Bit clock generated externally */
                             I2S_TCR_TFDIR_MASK    /* Frame sync generated externally */
                            );
         i2s_ptr->CR |= (I2S_CR_I2SEN_MASK);
         io_info_ptr->MODE = I2S_MODE_SLAVE;
         break;

      case IO_IOCTL_I2S_SET_CLOCK_SOURCE_INT:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_INT: Error - Cannot chnage I2S clock source while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         /* You should not change clock source, when in slave mode */
         if (io_info_ptr->MODE == I2S_MODE_SLAVE)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_INT: Error - Cannot chnage I2S clock source while in slave mode.\n");
            #endif
            result = IO_ERROR_INVALID_IOCTL_CMD;
            break;
         }

         i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK); /* disable SSI - see table at top of this file */
         i2s_ptr->CR |= (I2S_CR_SYSCLKEN_MASK); /* Internal master clock enable */
         i2s_ptr->CR |= (I2S_CR_I2SEN_MASK);

         if (io_info_ptr->CHANNEL == 0)
         {
        	 SIM_SOPT2 |= SIM_SOPT2_I2SSRC(0);
         }
         io_info_ptr->CLOCK_SOURCE = I2S_CLK_INT;
         io_info_ptr->MCLK_FREQ = BSP_SYSTEM_CLOCK / io_info_ptr->MCLK_DIV;
         io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
         io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / io_info_ptr->DATA_BITS;
         break;

      case IO_IOCTL_I2S_SET_CLOCK_SOURCE_EXT:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_EXT: Error - Cannot chnage I2S clock source while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         if (io_info_ptr->MODE == I2S_MODE_SLAVE)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_CLOCK_SOURCE_EXT: Error - Cannot change I2S clock source while in slave mode.\n");
            #endif
            result = IO_ERROR_INVALID_IOCTL_CMD;
            break;
         }
         /*enable pin to receive external CLK signal*/
          i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK); /* disable SSI - see table at top of this file */
          i2s_ptr->CR &= ~(I2S_CR_SYSCLKEN_MASK); /* Internal master clock disable */
          i2s_ptr->CR |= (I2S_CR_I2SEN_MASK);

         if (io_info_ptr->CHANNEL == 0)
         {
        	 SIM_SOPT2 |= SIM_SOPT2_I2SSRC(3); //SSI0 clock source = SSI_CLKIN pin=
         }
         io_info_ptr->CLOCK_SOURCE = I2S_CLK_EXT;
         break;

      case IO_IOCTL_I2S_SET_DATA_BITS:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_DATA_BITS: Error - Cannot chnage I2S data bit length while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
         }
         else
         {
            uint_8 data_bits =  *((uint_8_ptr) param_ptr);
            if ((data_bits < 8) || (data_bits > 24) || (data_bits == 14) || (data_bits % 2))
            {
               #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_DATA_BITS: Error - Unsupported data word length.\n");
               #endif
               result = I2S_ERROR_WORD_LENGTH_UNSUPPORTED;
            }
            data_bits = (data_bits/2) - 1;
            i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK);
            i2s_ptr->TCCR |= (I2S_TCCR_WL((uint_32) data_bits));
            i2s_ptr->RCCR |= (I2S_RCCR_WL((uint_32) data_bits));
            i2s_ptr->CR |= (I2S_CR_I2SEN_MASK);
            io_info_ptr->DATA_BITS = *((uint_8_ptr) param_ptr);
            io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / io_info_ptr->DATA_BITS;
         }
         break;

      case IO_IOCTL_I2S_DISABLE_DEVICE:
         _int_disable();
         i2s_ptr->IER &= ~(
                           I2S_IER_TIE_MASK | /* Transmit interrupts disable */
                           I2S_IER_RIE_MASK   /* Receive interrupts disable */
                           );
         _int_enable();
         i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK); /*SSI disable*/
         break;

      case IO_IOCTL_I2S_ENABLE_DEVICE:
         i2s_ptr->IER |= (
                           I2S_IER_TIE_MASK | /* Transmit interrupts enable */
                           I2S_IER_RIE_MASK   /* Receive interrupts enable */
                           );
         i2s_ptr->CR |= (I2S_CR_I2SEN_MASK); /*SSI enable*/
         break;

      case IO_IOCTL_I2S_SET_MCLK_FREQ:
          if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MCLK_FREQ: Error - Cannot chnage I2S master clock frequency while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         else
         {
            uint_8 divider;
            uint_32 frequency;
            uint_32 fract, temp_fract, divide, temp_div, closest_mclk;
            boolean found;

            frequency = *((uint_32_ptr) param_ptr);

            if (io_info_ptr->CLOCK_SOURCE == I2S_CLK_INT)
            {
               divider =(uint_8)( BSP_SYSTEM_CLOCK / frequency);
               if ((divider < SSIDIV_MIN) || (divider > SSIDIV_MAX))
               {
                  #ifdef I2S_DEBUG
                   printf("_i2s_ioctl - IO_IOCTL_I2S_SET_MCLK_FREQ: Error - System clock divider value out of range.\n");
                  #endif
                  result = I2S_ERROR_DIVIDER_VALUE_INVALID;
               }
               else
               {
                  fract = 1;
                  divide = 1;
                  found = FALSE;
                  closest_mclk = 0;
                  
                  while (fract < 256)
                  {
                     while (divide < 4096)
                     {   
                        if (((BSP_SYSTEM_CLOCK / divide * fract) - frequency) == 0)
                        {
                           found = TRUE;
                           break;
                        }
                        else
                        {
                           if ((closest_mclk == 0) 
                                 || (((BSP_SYSTEM_CLOCK / divide * fract) % frequency) < (closest_mclk % frequency)))
                           {
                              temp_fract = fract;
                              temp_div = divide;
                              closest_mclk = BSP_SYSTEM_CLOCK / divide * fract;
                           }
                           
                        }
                        divide++;
                     }
                     if (found == TRUE)
                     {
                        break;
                     }
                     if (divide == 4096)
                     {
                        divide = 1;
                     }
                     fract++;
                  }
               
                  if (found == FALSE)
                  {
                     frequency = closest_mclk;
                     fract = temp_fract;
                     divide = temp_div;
                  }
                  
                  if (io_info_ptr->CHANNEL == 0)
                  {
                     sim->SCGC6 &= ~(SIM_SCGC6_I2S_MASK);
                     sim->CLKDIV2 &= ~SIM_CLKDIV2_I2SDIV_MASK;
                     sim->CLKDIV2 &= ~SIM_CLKDIV2_I2SFRAC_MASK;
                     sim->CLKDIV2 |= SIM_CLKDIV2_I2SDIV(divide-1) | SIM_CLKDIV2_I2SFRAC(fract-1);
                     sim->SCGC6 |= SIM_SCGC6_I2S_MASK;
                  }
                  io_info_ptr->MCLK_FREQ = frequency;
                  io_info_ptr->MCLK_DIV = divider;
               }
            }
            else
            {
               io_info_ptr->MCLK_FREQ = frequency;
            }
            if (io_info_ptr->MODE == I2S_MODE_MASTER)
            {
               io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / 64;
            }
            else
            {
               io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / (2 * io_info_ptr->DATA_BITS);
            }
            io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
         }
         break;

      case IO_IOCTL_I2S_SET_FS_FREQ:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_FS_FREQ: Error - Cannot chnage I2S sampling frequency while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         else
         {
            uint_32 frequency, bclk_frequency;
            uint_16 divider;

            frequency = *((uint_32_ptr) param_ptr);

            if (io_info_ptr->MODE == I2S_MODE_SLAVE)
            {
               bclk_frequency = frequency * io_info_ptr->DATA_BITS * 2;
               if ((bclk_frequency > (io_info_ptr->MCLK_FREQ / 2)) || (bclk_frequency > (BSP_BUS_CLOCK / 5)))
               {
                  #ifdef I2S_DEBUG
                    printf("_i2s_ioctl - IO_IOCTL_I2S_SET_FS_FREQ: Error - BCLK_FREQ > MCLK_FREQ/2 or BCLK_FREQ > BUS_CLOCK / 5.\n");
                  #endif
                  result = I2S_ERROR_FREQUENCY_INVALID;
                  break;
               }
               io_info_ptr->BCLK_FREQ = bclk_frequency;
               io_info_ptr->FS_FREQ = frequency;
               break;
            }

            bclk_frequency = frequency * 64;
            divider = io_info_ptr->MCLK_FREQ / (bclk_frequency * 2);
            /*
            ** valid divider values are:
            ** 1-256 in step of 1
            ** 257-512 in step of 2
            ** 513-2048 in step of 8
            ** 2049-4096 in step of 16
            */
            if (
               (divider < BCLK_DIV_MIN) || (divider > BCLK_DIV_MAX) ||
               (divider > 256) && (divider < 513) && (divider%2) ||
               (divider > 512) && (divider < 2049) && (divider%8) ||
               (divider > 2048) && (divider < BCLK_DIV_MAX+1) && (divider%16)
               )
            {
               #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_FS_FREQ: Error - Invalid BCLK divider value.\n");
               #endif
               result = I2S_ERROR_DIVIDER_VALUE_INVALID;
            }
            else
            {
               if (divider < 257)
               {
                  /* According to MCF5441X reference manual values of DIV2, PSR and PM must
                  ** not be changed simultaneously */
                  i2s_ptr->TCCR &= ~(I2S_TCCR_PM_MASK);
                  i2s_ptr->TCCR |= (I2S_TCCR_PM(divider - 1));
                  i2s_ptr->TCCR &= ~(I2S_TCCR_PSR_MASK);
                  i2s_ptr->TCCR &= ~(I2S_TCCR_DIV2_MASK);
               }
               else if ((divider > 256) && (divider < 513))
               {
                  divider = divider/2;
                  i2s_ptr->TCCR &= ~(I2S_TCCR_PM_MASK);
                  i2s_ptr->TCCR |= (I2S_TCCR_PM(divider - 1));
                  i2s_ptr->TCCR &= ~(I2S_TCCR_PSR_MASK);
                  i2s_ptr->TCCR |= (I2S_TCCR_DIV2_MASK);
               }
               else if ((divider > 512) && (divider < 2049))
               {
                  divider = divider/8;
                  i2s_ptr->TCCR &= ~(I2S_TCCR_PM_MASK);
                  i2s_ptr->TCCR |= (I2S_TCCR_PM(divider - 1));
                  i2s_ptr->TCCR |= (I2S_TCCR_PSR_MASK);
                  i2s_ptr->TCCR &= ~(I2S_TCCR_DIV2_MASK);
               }
               else if (divider > 2048)
               {
                  divider = divider/16;
                  i2s_ptr->TCCR &= ~(I2S_TCCR_PM_MASK);
                  i2s_ptr->TCCR |= (I2S_TCCR_PM(divider - 1));
                  i2s_ptr->TCCR |= (I2S_TCCR_PSR_MASK);
                  i2s_ptr->TCCR |= (I2S_TCCR_DIV2_MASK);
               }
               io_info_ptr->BCLK_DIV = divider * 2;
               io_info_ptr->BCLK_FREQ = io_info_ptr->MCLK_FREQ / io_info_ptr->BCLK_DIV;
               /* The word length is fixed to 32 in I2S master mode and DC is 1
               ** thus frame sync frequency is always BCLK_FREQ/64
               */
               io_info_ptr->FS_FREQ = io_info_ptr->BCLK_FREQ / 64;
            }

         }
         break;

      case IO_IOCTL_AUDIO_SET_IO_DATA_FORMAT:
         if(io_info_ptr->ONTHEWAY)
         {
            #ifdef I2S_DEBUG
             printf("_i2s_ioctl - IO_IOCTL_I2S_SET_IO_DATA_FORMAT: Error - Cannot change I/O data format while I/O actions are in progress.\n");
            #endif
            result = I2S_ERROR_DEVICE_BUSY;
            break;
         }
         if (param_ptr == NULL)
         {
            result = I2S_ERROR_INVALID_PARAMETER;
            break;
         }
         else
         {
            uint_8 aligment, endianity, bits, channels, size;
            endianity = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ENDIAN;
            bits = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->BITS;
            aligment = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ALIGNMENT;
            channels = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->CHANNELS;
            size = ((AUDIO_DATA_FORMAT_PTR) param_ptr)->SIZE;
            if ((aligment > AUDIO_ALIGNMENT_LEFT) || (bits < AUDIO_BIT_SIZE_MIN) ||
               (bits >AUDIO_BIT_SIZE_MAX) || (endianity > AUDIO_LITTLE_ENDIAN) ||
               (channels > 2) || (channels == 0))
            {
               #ifdef I2S_DEBUG
                printf("_i2s_ioctl - IO_IOCTL_I2S_SET_IO_DATA_FORMAT: Error - Invalid I/O data format.\n");
               #endif
               result = AUDIO_ERROR_INVALID_IO_FORMAT;
               break;
            }
            io_info_ptr->IO_FORMAT.ENDIAN = endianity;
            io_info_ptr->IO_FORMAT.BITS = bits;
            io_info_ptr->IO_FORMAT.ALIGNMENT = aligment;
            io_info_ptr->IO_FORMAT.CHANNELS = channels;
            io_info_ptr->IO_FORMAT.SIZE = size;
         }
         break;

      case IO_IOCTL_I2S_SET_TXFIFO_WATERMARK:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         watermark = *((uint_8_ptr) param_ptr);
         if (watermark > FIFO_WATERMARK_MAX)
         {
            result = I2S_ERROR_PARAM_OUT_OF_RANGE;
            break;
         }
         i2s_ptr->FCSR |= (
            I2S_FCSR_TFWM0(watermark) | /* Set transmit FIFO0 watermark */
            I2S_FCSR_TFWM1(watermark)  /* Set transmit FIFO1 watermark */
         );
         break;

      case IO_IOCTL_I2S_SET_RXFIFO_WATERMARK:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         watermark = *((uint_8_ptr) param_ptr);
         if (watermark > FIFO_WATERMARK_MAX)
         {
            result = I2S_ERROR_PARAM_OUT_OF_RANGE;
            break;
         }
         i2s_ptr->FCSR |= (
            I2S_FCSR_RFWM0(watermark) | /* Set receive FIFO0 watermark */
            I2S_FCSR_RFWM1(watermark)   /* Set reveive FIFO1 watermark */
         );
         break;

      case IO_IOCTL_I2S_TX_DUMMY_ON:
         if (io_info_ptr->TX_DUMMY == FALSE)
         {
            io_info_ptr->TX_DUMMY = TRUE;
            dummy->LENGTH = _ki2s_sine_wave(io_info_ptr->FS_FREQ, 440, &(dummy->DATA));
            dummy->INDEX_L = 0;
            dummy->INDEX_R = 0;
         }
         break;

      case IO_IOCTL_I2S_TX_DUMMY_OFF:
         if (io_info_ptr->TX_DUMMY == TRUE)
         {
            io_info_ptr->TX_DUMMY = FALSE;
            result = _mem_free(dummy->DATA);
            if (result == MQX_OK)
            {
               dummy->LENGTH = 0;
               dummy->DATA = NULL;
            }
         dummy->INDEX_L = 0;
         dummy->INDEX_R = 0;
         }
         break;

      case IO_IOCTL_I2S_GET_MODE:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((uint_8_ptr) param_ptr) = io_info_ptr->MODE;
         break;

      case IO_IOCTL_I2S_GET_CLOCK_SOURCE:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((uint_8_ptr) param_ptr) = io_info_ptr->CLOCK_SOURCE;
          break;

      case IO_IOCTL_I2S_GET_DATA_BITS:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((uint_8_ptr) param_ptr) = io_info_ptr->DATA_BITS;
         break;

      case IO_IOCTL_I2S_GET_MCLK_FREQ:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((uint_32_ptr) param_ptr) = io_info_ptr->MCLK_FREQ;
         break;

      case IO_IOCTL_I2S_GET_BCLK_FREQ:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((uint_32_ptr) param_ptr) = io_info_ptr->BCLK_FREQ;
         break;

      case IO_IOCTL_I2S_GET_TX_DUMMY:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((boolean*) param_ptr) = io_info_ptr->TX_DUMMY;
         break;

      case IO_IOCTL_I2S_GET_FS_FREQ:
         if (param_ptr == NULL)
         {
              result = I2S_ERROR_INVALID_PARAMETER;
              break;
         }
         *((uint_32_ptr) param_ptr) = io_info_ptr->FS_FREQ;
         break;

      case IO_IOCTL_I2S_GET_STATISTICS:
         if (param_ptr == NULL)
         {
            result = I2S_ERROR_INVALID_PARAMETER;
            break;
         }
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->INTERRUPTS = io_info_ptr->STATS.INTERRUPTS;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->UNDERRUNS_L = io_info_ptr->STATS.UNDERRUNS_L;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->UNDERRUNS_R = io_info_ptr->STATS.UNDERRUNS_R;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->OVERRUNS_L = io_info_ptr->STATS.OVERRUNS_L;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->OVERRUNS_R = io_info_ptr->STATS.OVERRUNS_R;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->RX_PACKETS = io_info_ptr->STATS.RX_PACKETS;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->TX_PACKETS = io_info_ptr->STATS.TX_PACKETS;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->PACKETS_PROCESSED_L = io_info_ptr->STATS.PACKETS_PROCESSED_L;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->PACKETS_QUEUED_L = io_info_ptr->STATS.PACKETS_QUEUED_L;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->PACKETS_REQUESTED_L = io_info_ptr->STATS.PACKETS_REQUESTED_L;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->PACKETS_PROCESSED_R = io_info_ptr->STATS.PACKETS_PROCESSED_R;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->PACKETS_QUEUED_R = io_info_ptr->STATS.PACKETS_QUEUED_R;
         ((I2S_STATISTICS_STRUCT_PTR) param_ptr)->PACKETS_REQUESTED_R = io_info_ptr->STATS.PACKETS_REQUESTED_R;
         break;

      case IO_IOCTL_AUDIO_GET_IO_DATA_FORMAT:
         if (param_ptr == NULL)
         {
            result = I2S_ERROR_INVALID_PARAMETER;
            break;
         }
         ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ENDIAN = io_info_ptr->IO_FORMAT.ENDIAN;
         ((AUDIO_DATA_FORMAT_PTR) param_ptr)->BITS = io_info_ptr->IO_FORMAT.BITS;
         ((AUDIO_DATA_FORMAT_PTR) param_ptr)->ALIGNMENT = io_info_ptr->IO_FORMAT.ALIGNMENT;
         ((AUDIO_DATA_FORMAT_PTR) param_ptr)->CHANNELS = io_info_ptr->IO_FORMAT.CHANNELS;
         break;

      case IO_IOCTL_I2S_GET_TXFIFO_WATERMARK:
         if (param_ptr == NULL)
         {
            result = I2S_ERROR_INVALID_PARAMETER;
            break;
         }
         *((uint_8_ptr) param_ptr) = i2s_ptr->FCSR & 0x000F;
         break;

      case IO_IOCTL_I2S_GET_RXFIFO_WATERMARK:
         if (param_ptr == NULL)
         {
            result = I2S_ERROR_INVALID_PARAMETER;
            break;
         }
         *((uint_8_ptr) param_ptr) = (i2s_ptr->FCSR & 0x00F0) >> 8;
         break;

      case IO_IOCTL_I2S_SET_CLK_ALWAYS_ENABLED_ON:
         i2s_ptr->CR &= ~(I2S_CR_RFRCLKDIS_MASK); /* Enable receiving clocks after receiver is disabled */
         i2s_ptr->CR &= ~(I2S_CR_TFRCLKDIS_MASK); /* Enable transmitting clocks after transmitter is disabled */
         io_info_ptr->CLK_ALWAYS_ON = TRUE;
         break;

      case IO_IOCTL_I2S_SET_CLK_ALWAYS_ENABLED_OFF:
         i2s_ptr->CR |= (I2S_CR_RFRCLKDIS_MASK); /* Disable receiving clocks after receiver is disabled */
         i2s_ptr->CR |= (I2S_CR_TFRCLKDIS_MASK); /* Disable transmitting clocks after transmitter is disabled */
         io_info_ptr->CLK_ALWAYS_ON = FALSE;
         break;

      case IO_IOCTL_I2S_GET_CLK_ALWAYS_ENABLED:
         if (param_ptr == NULL)
         {
            result = I2S_ERROR_INVALID_PARAMETER;
            break;
         }
         *((boolean*) param_ptr) = io_info_ptr->CLK_ALWAYS_ON;
         break;

      case IO_IOCTL_I2S_CLEAR_STATISTICS:
         io_info_ptr->STATS.INTERRUPTS = 0;
         io_info_ptr->STATS.UNDERRUNS_L = 0;
         io_info_ptr->STATS.UNDERRUNS_R = 0;
         io_info_ptr->STATS.OVERRUNS_L = 0;
         io_info_ptr->STATS.OVERRUNS_R = 0;
         io_info_ptr->STATS.RX_PACKETS = 0;
         io_info_ptr->STATS.TX_PACKETS = 0;
         io_info_ptr->STATS.PACKETS_PROCESSED_L = 0;
         io_info_ptr->STATS.PACKETS_QUEUED_L = 0;
         io_info_ptr->STATS.PACKETS_REQUESTED_L = 0;
         io_info_ptr->STATS.PACKETS_PROCESSED_R = 0;
         io_info_ptr->STATS.PACKETS_QUEUED_R = 0;
         io_info_ptr->STATS.PACKETS_REQUESTED_R = 0;
         break;

      default:
         result = IO_ERROR_INVALID_IOCTL_CMD;
         break;
   }
   return result;
} /* Endbody */

/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_format_in_data
* Returned Value   : formatted input data
* Comments         : Function performs I2S input data formatting
*
*END*********************************************************************/
static uint_32 _ki2s_format_in_data
   (
    /* [IN] data to format */
    char_ptr input,

    /* [IN] Requested format */
    AUDIO_DATA_FORMAT_PTR format
    )
{/* Body */
   uint_32 retval = 0;
   uint_8 i;
   uchar_ptr data_ptr;

   uchar_ptr   definition;

   uchar _PTR_ next_ptr;
   uchar _PTR_ b_ptr;
   uchar       c;

   for (i = 0; i < format->SIZE; i++)
   {
      retval |= ((((uint_32) *(input + i)) << (format->SIZE - 1 - i) * 8) & (0xFF) << (format->SIZE - 1 - i) * 8);
   }
   if (format->ENDIAN == AUDIO_LITTLE_ENDIAN)
   {
      /* convert data from Big to Little Endian */
      data_ptr = (uchar_ptr)&retval;
      c = data_ptr[0];
      data_ptr[0] = data_ptr[3];
      data_ptr[3] = c;
      c = data_ptr[1];
      data_ptr[1] = data_ptr[2];
      data_ptr[2] = c;
      data_ptr += 4;

      retval >>= sizeof(retval) * I2S_CHAR_BIT - format->BITS;
   }
   else
   {
      if ((format->ALIGNMENT == AUDIO_ALIGNMENT_RIGHT) && (format->BITS < (format->SIZE * 8)))
      {
         retval >>= (format->SIZE * I2S_CHAR_BIT) - format->BITS;
      }
   }
   retval = ~retval;
   return(retval);
}/* Endbody */


static void _ki2s_format_in_data_end (void)
{
}
/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_format_out_data
* Returned Value   : none
* Comments         : Function performs I2S output data formatting
*
*END*********************************************************************/
static void _ki2s_format_out_data
   (
    /* [IN] data to format */
    uint_32 input,

    /* [OUT] location for output */
    char_ptr output,

    /* [IN] Requested format */
    AUDIO_DATA_FORMAT_PTR format
    )
{
   uint_8 i;
   input = ~input;
   for(i = 0; i < format->SIZE; i++)
   {
      *(output + i) = (char) (input >> I2S_CHAR_BIT * (format->SIZE - i - 1) & 0xFF);
   }
   if (format->ENDIAN == AUDIO_BIG_ENDIAN)
   {
      if ((format->ALIGNMENT == AUDIO_ALIGNMENT_LEFT) && (format->BITS < (format->SIZE * 8)))
      {
         input <<= (format->SIZE * I2S_CHAR_BIT) - format->BITS;
      }
   }
   else
   {
      uchar def[2];
      def[0] = (uchar) format->SIZE;
      def[1] = 0;
      _mem_swap_endian(def, output);
   }

   return;
}

/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_sine_wave
* Returned Value   : samples number if successful, 0 otherwise
* Comments         :
*    This function generate a sine wave.
*
*END*********************************************************************/

uint_32 _ki2s_sine_wave(uint_32 fs, uint_32 freq, int_16_ptr* output)
{
    uint_32 samples, i;
    boolean over180 = 0;
    ieee_single period, step, angle = 0.0;
    period = 1 / (ieee_single) freq;
    samples = fs * (uint_32)period;
    if (samples == 0)
    {
      return(0);
    }
    *output = (int_16_ptr) _mem_alloc(sizeof(int_16) * samples);
    if (*output == NULL)
    {
       return(0);
    }
    step = 360.0 / (ieee_single) samples;
    for(i = 0; i < samples; i++)
    {
        if (angle > 180)
        {
            angle = angle - 180;
            over180 = TRUE;
        }
        *((*output) + i) =  BIT16_MAX * (4*(uint_32)angle*(180 - (uint_32)angle))/(40500 - (uint_32)angle * (180 - (uint_32)angle));
        if (over180)
        {
           *((*output) + i) = -(*((*output) + i));
        }
        angle += step;
    }
    return(samples);
}


/*FUNCTION****************************************************************
*
* Function Name    : _ki2s_hw_init
* Returned Value   : MQX error code
* Comments         :
*    This function initializes an I2S hardware control registers.
*
*END*********************************************************************/

static _mqx_int _ki2s_hw_init
(
   KI2S_INIT_STRUCT_PTR i2s_init_ptr,
   I2S_MemMapPtr i2s_ptr
)
{
   /* variables */
   uint_8 data_bits = i2s_init_ptr->DATA_BITS;
   uint_8 i2s_mode = i2s_init_ptr->MODE;
   uint_8 clk_source = i2s_init_ptr->CLOCK_SOURCE;
   uint_8 channel = i2s_init_ptr->CHANNEL;
   boolean tx_empty = i2s_init_ptr->TX_DUMMY;
   SIM_MemMapPtr   sim = SIM_BASE_PTR;

   data_bits = (data_bits/2) - 1;
   i2s_ptr->CR &= ~(I2S_CR_I2SEN_MASK); /* SSIn disable */
   i2s_ptr->CR |= (
      I2S_CR_SYN_MASK |    /* Set synchronous mode */
      I2S_CR_TCHEN_MASK    /* Turn dual channel mode ON */
      );
   i2s_ptr->TCR &= ~(I2S_TCR_TSHFD_MASK); /* Data transmitted MSB first */
   i2s_ptr->RCR &= ~(I2S_RCR_RSHFD_MASK); /* Data received MSB first */
   i2s_ptr->TCR |= (
      I2S_TCR_TSCKP_MASK | /* Data clocked out on falling edge of bit clock */
      I2S_TCR_TFSI_MASK  | /* Transmit frame sync is active low */
      I2S_TCR_TEFS_MASK  | /* Transmit frame sync is initiated one bit before the data transmits */
      I2S_TCR_TFEN0_MASK | /* Transmit FIFO0 enable */
      I2S_TCR_TFEN1_MASK   /* Transmit FIFO1 enable */
      );
   i2s_ptr->FCSR |= (
      I2S_FCSR_TFWM0(0x5) | /* Set transmit FIFO0 watermark to 5 */
      I2S_FCSR_TFWM1(0x5) | /* Set transmit FIFO1 watermark to 5 */
      I2S_FCSR_RFWM0(0x8) |
      I2S_FCSR_RFWM1(0x8)
      );
   i2s_ptr->RCR |= (
      I2S_RCR_RSCKP_MASK | /* Data latched on rising edge of bit clock */
      I2S_RCR_RFSI_MASK  | /* Receive frame sync is active low */
      I2S_RCR_REFS_MASK  | /* Receive frame sync is initiated one bit before the data is received */
      I2S_RCR_RFEN0_MASK | /* Receive FIFO0 enable */
      I2S_RCR_RFEN1_MASK   /* Receive FIFO1 enable */
      );

   /* Interrupts setup */
   i2s_ptr->IER |= (
      I2S_IER_TUE0EN_MASK | /* Transmit underrun error 0*/
      I2S_IER_TFE0EN_MASK | /* Transmit FIFO0 empty */
      I2S_IER_ROE0EN_MASK | /* Receive overrun error 0*/
      I2S_IER_RFF0EN_MASK | /* Receive FIFO0 full */
      I2S_IER_TUE1EN_MASK | /* Transmit underrun error 0*/
      I2S_IER_TFE1EN_MASK | /* Transmit FIFO0 empty */
      I2S_IER_ROE1EN_MASK | /* Receive overrun error 0*/
      I2S_IER_RFF1EN_MASK   /* Receive FIFO0 full */
      );
   i2s_ptr->IER &= ~(
      I2S_IER_RDR0EN_MASK | /* Receiver data ready 0 */
      I2S_IER_RDR1EN_MASK | /* Receiver data ready 1 */
      I2S_IER_TDE0EN_MASK | /* Transmit data empty 0 */
      I2S_IER_TDE1EN_MASK | /* Transmit data empty 1 */
      I2S_IER_RIE_MASK    | /* Receiver disable interrupts */
      I2S_IER_TIE_MASK      /* Transmitter disable interrupts */
      );

   /* clock source specific */
   switch (clk_source)
   {
      case I2S_CLK_INT:
         if (channel == 0)
         {
             /* At first Disable system clock to the I2S module */
             sim->SCGC6 &= ~(SIM_SCGC6_I2S_MASK);
             /* I2S clock source is system clock */
             sim->SOPT2 |= SIM_SOPT2_I2SSRC(0);
             /* Divide to get the 12.2880 MHz from 96MHz (96* (16/125)) */
             sim->CLKDIV2 |= SIM_CLKDIV2_I2SDIV(124) | SIM_CLKDIV2_I2SFRAC(15);
             sim->SCGC6 |= SIM_SCGC6_I2S_MASK;
         }
         break;
      case I2S_CLK_EXT:
         if (channel == 0)
         {
            /* At first Disable system clock to the I2S module */
            sim->SCGC6 &= ~(SIM_SCGC6_I2S_MASK);
            sim->SOPT2 |= SIM_SOPT2_I2SSRC(3);
            /* Divide to get the 12.2880 MHz from 96MHz (96* (16/125)) */
            sim->CLKDIV2 |= SIM_CLKDIV2_I2SDIV(124) | SIM_CLKDIV2_I2SFRAC(15);
            sim->SCGC6 |= SIM_SCGC6_I2S_MASK;
         }
         break;
      default:
         break;
   }
   /* mode specific */
   switch(i2s_mode)
   {
      case I2S_MODE_MASTER:
         i2s_ptr->CR |= (I2S_CR_I2SMODE(0x01));
         if (clk_source == I2S_CLK_EXT)
         {
            i2s_ptr->CR &= ~(I2S_CR_SYSCLKEN_MASK); /* Master clock disable */
         }
         else
         {
            i2s_ptr->CR |= (I2S_CR_SYSCLKEN_MASK);  /* Master clock enable */
         }
         i2s_ptr->TCR |= (
                           I2S_TCR_TXDIR_MASK | /* Bit clock generated internally */
                           I2S_TCR_TFDIR_MASK   /* Frame sync generated internally */
                             );
         break;
      case I2S_MODE_SLAVE:
         i2s_ptr->CR |= (I2S_CR_I2SMODE(0x02));
         i2s_ptr->CR &= ~(I2S_CR_SYSCLKEN_MASK); /* Master clock disabled */
         i2s_ptr->TCR &= ~(
            I2S_TCR_TXDIR_MASK | /* Bit clock generated externally */
            I2S_TCR_TFDIR_MASK   /* Frame sync generated externally */
            );
         break;
      default:
         break;
   }

   /* Set word length (valid data bits) */
   i2s_ptr->TCCR = (I2S_TCCR_WL((uint_32) data_bits));
   /* set frame rate 48 MHz*/
   i2s_ptr->TCCR |= (I2S_TCCR_DC(0x03));
   i2s_ptr->TCCR |= (I2S_TCCR_PM(0x01));
   /* Set word length (valid data bits) */
   i2s_ptr->RCCR |= (I2S_RCCR_WL((uint_32) data_bits));
   i2s_ptr->RCCR |= (I2S_TCCR_DC(0x03));
   i2s_ptr->RCCR |= (I2S_TCCR_PM(0x01));

   /* Enable receiving clocks after receiver is disabled */
   i2s_ptr->CR &= ~(I2S_CR_RFRCLKDIS_MASK);
   /* Enable transmitting clocks after transmitter is disabled */
   i2s_ptr->CR &= ~(I2S_CR_TFRCLKDIS_MASK);
   /* SSIn enable */
   i2s_ptr->CR |= (I2S_CR_I2SEN_MASK);

   return (MQX_OK);
}
/* EOF */
