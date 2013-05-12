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
* $FileName: init_i2s.c$
* $Version : 3.8.2.0$
* $Date    : Oct-7-2011$
*
* Comments:
*
*   This file contains the definition for the baud rate for the I2C
*   channel
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "i2s.h"
#include "i2s_ki2s.h"
#include "i2s_audio.h"

const AUDIO_DATA_FORMAT _bsp_audio_data_init = {
        AUDIO_LITTLE_ENDIAN,       /* Endian of input data */
        AUDIO_ALIGNMENT_LEFT,   /* Aligment of input data */
        8,                      /* Bit size of input data */
        1,                      /* Sample size in bytes */
        1                       /* Number of channels */
};
const KI2S_INIT_STRUCT _bsp_i2s0_init = {
   0,                      /* I2S channel      */
   BSP_I2S0_MODE,          /* I2S mode         */ 
   BSP_I2S0_DATA_BITS,     /* I2C data bits    */ 
   BSP_I2S0_CLOCK_SOURCE,  /* I2C clock source */
   FALSE,                  /* I2C data are Stereo */
   FALSE,                  /* I2C Transmit dummy data */
   BSP_I2S0_INT_LEVEL,     /* I2C interrupt level */
   BSP_I2S0_BUFFER_SIZE,   /* I2C TX buffer size */
   &_bsp_audio_data_init
};

/* EOF */
