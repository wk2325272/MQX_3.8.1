/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
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
* $FileName: audio_speaker.h$
* $Version : 
* $Date    : 
*
* Comments:
*
* @brief The file contains Macro's and functions needed by the audio speaker 
*        application
*
*****************************************************************************/

#ifndef _audio_speaker_h
#define _audio_speaker_h 1

#include "usb_descriptor.h"

#if (defined BSP_TWRMCF54418) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60D100M)
    #define I2S_DEVICE_INTERRUPT  "ii2s0:"    
    #if ! BSPCFG_ENABLE_II2S0
    #error This application requires BSPCFG_ENABLE_II2S0 defined non-zero in user_config.h. Please recompile libraries with this option.
    #endif
#if BSP_TWR_K53N512 || BSP_TWR_K40X256
   #define I2C_DEVICE_POLLED "i2c1:"
   
   #if ! BSPCFG_ENABLE_I2C1
   #error This application requires BSPCFG_ENABLE_I2C1 defined non-zero in user_config.h. Please recompile BSP with this option.
   #endif
#else
   #define I2C_DEVICE_POLLED "i2c0:"
   
   #if ! BSPCFG_ENABLE_I2C0
   #error This application requires BSPCFG_ENABLE_I2C0 defined non-zero in user_config.h. Please recompile BSP with this option.
   #endif
#endif
#endif

#ifdef I2S_DEVICE_INTERRUPT
#define AUDIO_DATA_SIZE   (64*100)
#else
#define AUDIO_DATA_SIZE   48
#endif

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define DATA_BUFF_SIZE                     (AUDIO_ENDPOINT_PACKET_SIZE)
#define AUDIO_FORMAT_SAMPLE_RATE           (AUDIO_FORMAT_SAMPLE_RATE_44K1)
#define AUDIO_I2S_FS_FREQ_DIV              (256)

#define USB_APP_ENUM_COMPLETE_EVENT_MASK   (0x01 << 0)
#define USB_APP_DATA_RECEIVED_EVENT_MASK   (0x01 << 1)
#define USB_APP_BUFFER0_FULL_EVENT_MASK    (0x01 << 2)
#define USB_APP_BUFFER1_FULL_EVENT_MASK    (0x01 << 3)

/*****************************************************************************
 * Global variables
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern void TestApp_Init(void);

#endif 


/* EOF */
