#ifndef _sgtl5000_h
#define _sgtl5000_h

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
**************************************************************************
*
* $FileName: sgtl5000.h$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the definitions of constants and structures
*   required for the SGTL5000 Low Power Stereo CODEC with Headphone Amplifier
*
*END************************************************************************/
#include <mqx.h>

#define CHIP_ID 0x0000
#define CHIP_DIG_POWER 0x0002
#define CHIP_CLK_CTRL 0x0004
#define CHIP_I2S_CTRL 0x0006
#define CHIP_SSS_CTRL 0x000A
#define CHIP_ADCDAC_CTRL 0x000E
#define CHIP_DAC_VOL 0x0010
#define CHIP_PAD_STRENGTH 0x0014
#define CHIP_ANA_ADC_CTRL 0x0020
#define CHIP_ANA_HP_CTRL 0x0022
#define CHIP_ANA_CTRL 0x0024
#define CHIP_LINREG_CTRL 0x0026
#define CHIP_REF_CTRL 0x0028
#define CHIP_MIC_CTRL 0x002A
#define CHIP_LINE_OUT_CTRL 0x002C
#define CHIP_LINE_OUT_VOL 0x002E
#define CHIP_ANA_POWER 0x0030
#define CHIP_PLL_CTRL 0x0032
#define CHIP_CLK_TOP_CTRL 0x0034
#define CHIP_ANA_STATUS 0x0036
#define CHIP_ANA_TEST2 0x003A
#define CHIP_SHORT_CTRL 0x003C
#define DAP_CONTROL 0x0100
#define DAP_PEQ 0x0102
#define DAP_BASS_ENHANCE 0x0104
#define DAP_BASS_ENHANCE_CTRL 0x0106
#define DAP_AUDIO_EQ 0x0108
#define DAP_SGTL_SURROUND 0x010A
#define DAP_FILTER_COEF_ACCESS 0x010C
#define DAP_COEF_WR_B0_MSB 0x010E
#define DAP_COEF_WR_B0_LSB 0x0110
#define DAP_AUDIO_EQ_BASS_BAND0 0x0116
#define DAP_AUDIO_EQ_BAND1 0x0118
#define DAP_AUDIO_EQ_BAND2 0x011A
#define DAP_AUDIO_EQ_BAND3 0x011C
#define DAP_AUDIO_EQ_TREBLE_BAND4 0x011E
#define DAP_MAIN_CHAN 0x0120
#define DAP_MIX_CHAN 0x0122
#define DAP_AVC_CTRL 0x0124
#define DAP_AVC_THRESHOLD 0x0126
#define DAP_AVC_ATTACK 0x0128
#define DAP_AVC_DECAY 0x012A
#define DAP_COEF_WR_B1_MSB 0x012C
#define DAP_COEF_WR_B1_LSB 0x012E
#define DAP_COEF_WR_B2_MSB 0x0130
#define DAP_COEF_WR_B2_LSB 0x0132
#define DAP_COEF_WR_A1_MSB 0x0134
#define DAP_COEF_WR_A1_LSB 0x0136
#define DAP_COEF_WR_A2_MSB 0x0138
#define DAP_COEF_WR_A2_LSB 0x013A
#define SGTL5000_I2C_ADDR 0x0A

#define SGTL5000_DEBUG 1


_mqx_int sgtl_WriteReg(uint_16 reg, uint_16 reg_val);
_mqx_int sgtl_ReadReg(uint_16 reg, uint_16_ptr dest_ptr);
_mqx_int sgtl_ModifyReg(uint_16 reg, uint_16 clr_mask, uint_16 set_mask);
_mqx_int sgtl_Init(void);
_mqx_int InitCodec(MQX_FILE_PTR i2s_ptr);
_mqx_int SetupCodec(MQX_FILE_PTR device);

#endif
/* EOF */
