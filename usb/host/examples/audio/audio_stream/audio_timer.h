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
 **************************************************************************//*!
 *
 * @file audio_timer.h
 *
 * @author
 *
 * @version
 *
 * @date    
 *
 * @brief   
 *          
 *****************************************************************************/
#ifndef AUDIO_TIMER_H_
#define AUDIO_TIMER_H_


void audio_timer_init(void);

#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
   #define AUDIO_SPEAKER_FREQUENCY         2992
   #define AUDIO_MICROPHONE_FREQUENCY      374
#elif (defined BSP_TWRMCF51JF)
   #define AUDIO_SPEAKER_FREQUENCY     6000
   #define AUDIO_MICROPHONE_FREQUENCY   750
#else
#define AUDIO_SPEAKER_FREQUENCY         1000
#define AUDIO_MICROPHONE_FREQUENCY      8000
#endif

#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_TWR_K60D100M)
#define AUDIO_TIMER_CLOCK     BSP_BUS_CLOCK
#endif

#if (defined BSP_M52259DEMO)  || (defined BSP_M52259EVB) || (defined BSP_TWRMCF52259)\
   || (defined BSP_M52223EVB) || (defined BSP_TWRMCF54418)
#define AUDIO_TIMER_CLOCK     BSP_BUS_CLOCK
#endif

#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
extern void cmt_init_freq(uint_32 period);
extern void DisableCmtInterrupt(void);
extern void EnableCmtInterrupt(void);
#endif

#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_TWR_K60D100M)
extern void _kinetis_timer_clear_int(uint_32 timer);
extern void _kinetis_timer_mask_int(uint_32 timer);
extern void _kinetis_timer_unmask_int(uint_32 timer);
extern void _kinetis_timer_init_freq(_mqx_uint timer,uint_32 system_clock, uint_32 tickfreq, boolean unmask_timer);
#endif

#if (defined BSP_TWRMCF51JF)
   extern void ftm0_init_freq(uint_32 period);
   extern void DisableFTM0Interrupt(void);
   extern void EnableFTM0Interrupt(void);
#endif
#endif
