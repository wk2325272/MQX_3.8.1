/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
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
*
**************************************************************************
*
* $FileName: sgtl5000.c$
* $Version : 3.8.2.1$
* $Date    : May-22-2012$
*
* Comments:
*
*   This file contains functions for the SGTL5000
*
*END************************************************************************/

#include <mqx.h>
#include <fio.h>
#include <bsp.h>
#include <i2c.h>
#include "sgtl5000.h"
#include "audio_speaker.h"

#ifdef I2S_DEVICE_INTERRUPT

MQX_FILE_PTR fd = NULL;

/*FUNCTION****************************************************************
* 
* Function Name    : sgtl_WriteReg
* Returned Value   : MQX error code
* Comments         :
*    Writes a value to the entire register. All
*    bit-fields of the register will be written.
*
*END*********************************************************************/
_mqx_int sgtl_Init(void)
{
	uint_32 param;
	I2C_STATISTICS_STRUCT stats;
#if (defined BSP_TWRMCF54418)
	VMCF5441_STRUCT_PTR pReg = _PSP_GET_IPSBAR();
	pReg->GPIO.SRCR_CANI2C |= 0x03;
#endif
	if (fd == NULL)
	{
	   fd = fopen (I2C_DEVICE_POLLED, NULL);
	}
	if (fd == NULL) 
	{
		printf ("ERROR: Unable to open I2C driver!\n");
		return(-9);
	}
	param = 200000;
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_BAUD, &param))
	{
		return(-1);
	}
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_MASTER_MODE, NULL))
	{
		return(-2);
	}
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_CLEAR_STATISTICS, NULL))
	{
		return(-3);
	}
	param = SGTL5000_I2C_ADDR;
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_DESTINATION_ADDRESS, &param))
	{
		return(-4);
	}
	
	/* Initiate start and send I2C bus address */
	fwrite (&param, 1, 0, fd);
	
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_GET_STATISTICS, (pointer)&stats))
	{
		return(-5);
	}
   /* Stop I2C transfer */
 	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
 	{
 		return(-6);
 	}
	/* Check ack (device exists) */
	if (I2C_OK == ioctl (fd, IO_IOCTL_FLUSH_OUTPUT, &param))
	{
	    if ((param) || (stats.TX_NAKS)) 
		{
	    	return(-7);
		}
	}
	else
	{
		return(-8);
	}
	return(MQX_OK);
}

/*FUNCTION****************************************************************
* 
* Function Name    : sgtl_WriteReg
* Returned Value   : MQX error code
* Comments         :
*    Writes a value to the entire register. All
*    bit-fields of the register will be written.
*
*END*********************************************************************/
_mqx_int sgtl_WriteReg(uint_16 reg, uint_16 reg_val)
{
	uint_8 buffer[4];
	uint_32 result;
	buffer[0] = (uint_8)((reg >> 8) & 0xFF);
	buffer[1] =	(uint_8)(reg & 0xFF);
	buffer[2] =	(uint_8)((reg_val >> 8) & 0xFF);
	buffer[3] =	(uint_8)(reg_val & 0xFF);
	result = write(fd, buffer, 4); 
	if (4 != result)
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_WriteReg: Error - write to address ");
		 printf("0x%04X failed.\n", reg);
		#endif
		return(-1);
	} 
	result = fflush (fd);
	if (MQX_OK != result)
	{
		return(-3);
	} 
	/* Stop I2C transfer */
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
	{
		return(-2);
	}
	result = 0;
	return(MQX_OK);
}

/*FUNCTION****************************************************************
* 
* Function Name    : sgtl_WriteReg
* Returned Value   : MQX error code
* Comments         :
*    Reads value of register. 
*
*END*********************************************************************/
_mqx_int sgtl_ReadReg(uint_16 reg, uint_16_ptr dest_ptr)
{
	uint_8 buffer[2];
	uint_32 result, param;
	buffer[0] = (uint_8)((reg >> 8) & 0xFF);
	buffer[1] =	(uint_8)(reg & 0xFF);
	result = write(fd, buffer, 2);
	if (2 != result)
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_ReadReg: Error - SGTL not responding.\n");
		#endif
		return(-1);
	}
	result = fflush (fd);
	if (MQX_OK != result)
	{
		return(-6);
	} 
	/* Send repeated start */
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_REPEATED_START, NULL))
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_ReadReg: Error - unable to send repeated start.\n");
		#endif
		return(-2);
	}
	/* Set read request */
	param = 2;
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_SET_RX_REQUEST, &param))
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_ReadReg: Error - unable to set number of bytes requested.\n");
		#endif
		return(-3);
	}
	result = 0;
	/* Read all data */ 
	result = read (fd, buffer, 2);		
	if (2 != result)
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_ReadReg: Error - SGTL not responding.\n");
		#endif
		return(-4);
	}
	result = fflush (fd);
	if (MQX_OK != result)
	{
		return(-7);
	} 
	*dest_ptr = (buffer[1] & 0xFFFF) | ((buffer[0] & 0xFFFF) << 8);
	/* Stop I2C transfer */
	if (I2C_OK != ioctl (fd, IO_IOCTL_I2C_STOP, NULL))
	{
		return(-5);
	}
	return (MQX_OK);
}

/*FUNCTION****************************************************************
* 
* Function Name    : sgtl_ModifyReg
* Returned Value   : MQX error code
* Comments         :
*    Modifies value of register. Bits to set to zero are defined by first 
*	 mask, bits to be set to one are defined by second mask.
*
*END*********************************************************************/
_mqx_int sgtl_ModifyReg(uint_16 reg, uint_16 clr_mask, uint_16 set_mask)
{
	uint_16 reg_val = 0;
	if (MQX_OK != sgtl_ReadReg(reg, &reg_val))
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_ModifyReg: Error - cannot read from SGTL.\n");
		#endif
		return(-1);
	}
	reg_val &= clr_mask;
	reg_val |= set_mask;
	if (MQX_OK != sgtl_WriteReg(reg, reg_val))
	{
		#ifdef SGTL5000_DEBUG	
		 printf("sgtl_ModifyReg: Error - cannot write to SGTL.\n");
		#endif
		return(-2);
	}
	return(MQX_OK);	
}

/*FUNCTION****************************************************************
* 
* Function Name    : InitCodec
* Returned Value   : MQX error code
* Comments         :
*     
*
*END*********************************************************************/
_mqx_int InitCodec(MQX_FILE_PTR i2s_ptr)
{
   _mqx_uint errcode = 0;
   uint_32 fs_freq = AUDIO_FORMAT_SAMPLE_RATE;
   uint_32 mclk_freq = fs_freq * 256;
   if (i2s_ptr == NULL)
   {
      return(0xDEAD);
   }
   ioctl(i2s_ptr, IO_IOCTL_I2S_SET_CLK_ALWAYS_ENABLED_ON, NULL);
   ioctl(i2s_ptr, IO_IOCTL_I2S_SET_MCLK_FREQ, &mclk_freq);
   ioctl(i2s_ptr, IO_IOCTL_I2S_SET_FS_FREQ, &fs_freq);
   errcode = sgtl_Init();
   if (errcode != MQX_OK)
   {
      return (errcode);
   }
   //--------------- Power Supply Configuration----------------
   // NOTE: This next 2 Write calls is needed ONLY if VDDD is
   // internally driven by the chip
   // Configure VDDD level to 1.2V (bits 3:0)
   // Configure the charge pump to use the VDDIO rail (set bit 5 and bit 6)
   sgtl_WriteReg(CHIP_CLK_TOP_CTRL, 0x0800);
   // Enable internal oscillator 
   sgtl_WriteReg(CHIP_LINREG_CTRL, 0x0068); 
   // Power up internal linear regulator (Set bit 9)
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFFF, 0x0200);
   
   //------ Reference Voltage and Bias Current Configuration----------
   // NOTE: The value written in the next 2 Write calls is dependent
   // on the VDDA voltage value.
   // Set ground, ADC, DAC reference voltage (bits 8:4).
   // This example assumes VDDA = 3.3V. This should usually be set
   // to VDDA/2 or lower for best performance (maximum output swing at minimum THD).
   // 0x1F = 1.575V.
   // The bias current should be set to 50% of the nominal value (bits 3:1)
   sgtl_WriteReg(CHIP_REF_CTRL, 0x01FE);
   // Set LINEOUT reference voltage to 1.675V (bits 5:0) and bias current
   //(bits 11:8) to the recommended value of 0.36mA for 10kOhm load with 1nF
   //capacitance
   sgtl_WriteReg(CHIP_LINE_OUT_CTRL, 0x0323);
   
   //----------------Other Analog Block Configurations------------------
   // Configure slow ramp up rate to minimize pop (bit 0)
   sgtl_ModifyReg(CHIP_REF_CTRL, 0xFFFF, 0x0001);   
   // Enable short detect mode for headphone left/right
   // and center channel and set short detect current trip level
   // to 125mA
   sgtl_WriteReg(CHIP_SHORT_CTRL, 0x7706);
   // Enable Zero-cross detect if needed for HP_OUT (bit 5) and ADC (bit 1)
   sgtl_WriteReg(CHIP_ANA_CTRL, 0x0133);

   //----------------Power up Inputs/Outputs/Digital Blocks-------------
   // Power up desired digital blocks
   // I2S_IN (bit 0), DAP (bit 4), DAC (bit 5),
   // ADC (bit 6) are powered on
   sgtl_WriteReg(CHIP_DIG_POWER, 0x0071);
   // Power up LINEOUT, HP, ADC, DAC
//   sgtl_WriteReg(CHIP_ANA_POWER, 0x3AFF);
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFFF, 0x3A00);
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFFF, 0x000F);
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFFF, 0x0010);
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFFF, 0x0020);
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFBF, 0x0000);
   sgtl_ModifyReg(CHIP_ANA_POWER, 0xFFFF, 0x0080);
   //--------------------Set LINEOUT Volume Level-----------------------
   // Set the LINEOUT volume level based on voltage reference (VAG)
   // values using this formula
   // Value = (int)(40*log(VAG_VAL/LO_VAGCNTRL) + 15)
   // Assuming VAG_VAL and LO_VAGCNTRL is set to 1.575V and 1.675V respectively, the
   // left LO volume (bits 12:8) and right LO volume (bits 4:0) value should be set
   // to 14
   sgtl_WriteReg(CHIP_LINE_OUT_VOL, 0x1414);
   
   // Configure SYS_FS clock to 44.1kHz
   // Configure MCLK_FREQ to 256*Fs
   sgtl_ModifyReg(CHIP_CLK_CTRL, 0xFFF4, 0x0004);
   // Configure the I2S clocks in master mode
   // NOTE: I2S LRCLK is same as the system sample clock
   sgtl_ModifyReg(CHIP_I2S_CTRL, 0xFFFF, 0x0030);
   // Example 1: I2S_IN -> DAC -> LINEOUT, HP_OUT
   // Route I2S_IN to DAC
   sgtl_ModifyReg(CHIP_SSS_CTRL, 0xFFDF, 0x0010);   
   // Select DAC as the input to HP_OUT
   sgtl_ModifyReg(CHIP_ANA_CTRL, 0xFFBF, 0x0000);
   
   //---------------- Input Volume Control---------------------
   // Configure ADC left and right analog volume to desired default.
   // Example shows volume of 0dB
   sgtl_WriteReg(CHIP_ANA_ADC_CTRL, 0x0000);
   // Configure MIC gain if needed. Example shows gain of 20dB
//   sgtl_ModifyReg(CHIP_MIC_CTRL, 0xFFFF, 0x0001); // bits 1:0
   
   //---------------- Volume and Mute Control---------------------
   // Configure HP_OUT left and right volume to minimum, unmute
   // HP_OUT and ramp the volume up to desired volume.
   sgtl_WriteReg(CHIP_ANA_HP_CTRL, 0x7F7F);
   // Code assumes that left and right volumes are set to same value
   sgtl_WriteReg(CHIP_ANA_HP_CTRL, 0x0000);
   // Unmute HP
   sgtl_ModifyReg(CHIP_ANA_CTRL, 0xFFEF, 0x0000); // bit 5
   // LINEOUT and DAC volume control
   sgtl_ModifyReg(CHIP_ANA_CTRL, 0xFEFF, 0x0000); // bit 8
   // Configure DAC left and right digital volume. Example shows
   // volume of 0dB
   sgtl_WriteReg(CHIP_DAC_VOL, 0x6464);
//   Modify CHIP_ADCDAC_CTRL->DAC_MUTE_LEFT 0x0000 // bit 2
//   Modify CHIP_ADCDAC_CTRL->DAC_MUTE_RIGHT 0x0000 // bit 3
   sgtl_ModifyReg(CHIP_ADCDAC_CTRL, 0xFFF3, 0x0000);
   // Unmute ADC
   sgtl_ModifyReg(CHIP_ANA_CTRL, 0xFFFE, 0x0000); // bit 0
   return (0);
}

/*FUNCTION****************************************************************
* 
* Function Name    : InitCodec
* Returned Value   : MQX error code
* Comments         :
*     
*
*END*********************************************************************/
_mqx_int SetupCodec(MQX_FILE_PTR device)
{ 
   uint_32 mclk_freq;
   uint_32 fs_freq;
   uint_8 mode;
   uint_32 multiple;
   _mqx_int errcode = 0;
   uint_16 mask_ones, mask_zeros;
   
   ioctl(device, IO_IOCTL_I2S_GET_MCLK_FREQ, &mclk_freq);
   ioctl(device, IO_IOCTL_I2S_GET_FS_FREQ, &fs_freq);
   multiple = mclk_freq/fs_freq;
   /* Convert "multiple" to value required for SGTL5000 CHIP_CLK_CTRL register (bits 1:0)*/
   mask_ones = (multiple/128) - 2;
   if (mask_ones > 0x2)
   {
      return (-1);
   }  
   /* Add to mask bits required to setup sampling frequency (bits 3:2) */
   errcode = sgtl_ModifyReg(CHIP_CLK_CTRL, 0xFFF0, AUDIO_FORMAT_SAMPLE_RATE);
   if (errcode != MQX_OK)
   {
      return(-3);
   }
   /* Setup I2S mode in codec */
   ioctl(device, IO_IOCTL_I2S_GET_MODE, &mode);
   if (mode == I2S_MODE_MASTER)
   {
      mask_ones = 0x0000;
      mask_zeros = 0xFF7F;   
   }
   else
   {
      mask_ones = 0x0080;
      mask_zeros = 0xFFFF;
   }
   errcode = sgtl_ModifyReg(CHIP_I2S_CTRL, mask_zeros, mask_ones);
   if (errcode != MQX_OK)
   {
      return(-4);
   }
   /* rise volume */
   sgtl_WriteReg(CHIP_DAC_VOL, 0x3C3C);
   return(0);
}

#endif /* I2S_DEVICE_INTERRUPT */
/* EOF */
