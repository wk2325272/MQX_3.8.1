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
#ifndef _audio_timer_h
#define _audio_timer_h 1

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_class.h"

/******************************************************************************
 * Global functions
 *****************************************************************************/
void audio_timer_init(void);
/* CMT timer functions prototypes */
#if (defined BSP_TWRMCF51JE)||(defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
   extern uint_32 cmt_init_freq(uint_32 tickfreq,uint_32 clk,boolean unmask_timer);
   extern void DisableCmtInterrupt(void);
   extern void EnableCmtInterrupt(void);
#endif

/* PIT timer functions prototypes */
#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_KWIKSTIK_K40X256) || (defined BSP_TWR_K60D100M)
   #define AUDIO        1
   extern void _pit_clear_int(uint_32 timer);
   extern void _pit_mask_int(uint_32 timer);
   extern void _pit_unmask_int(uint_32 timer);
   extern void _pit_init_freq(_mqx_uint timer,uint_32 system_clock, uint_32 tickfreq, boolean unmask_timer);
#endif

/* FTM timer functions prototypes */
#if (defined BSP_TWRMCF51JF)
   #define AUDIO        0
   extern uint_32 ftm_init_freq(_mqx_uint timer,uint_32 tickfreq,uint_32 clk,boolean unmask_timer);
   extern void DisableFTMInterrupt(uint_32 timer);
   extern void EnableFTMInterrupt(uint_32 timer);
#endif

/******************************************************************************
 * MACRO'S
 *****************************************************************************/
/* Audio timer for mcf51je, mcf51jm, mcf51mm */ 
#if (defined BSP_TWRMCF51JE)||(defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
   #define _audio_timer_init_freq(a,b,c,d)   cmt_init_freq(b,c,d)
   
/* Audio timer for mcf51jf */    
#elif (defined BSP_TWRMCF51JF)
   #define AUDIO_TIMER              0
   #define _audio_timer_init_freq   ftm_init_freq
   
/* Audio timer for kinetis */   
#elif (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_KWIKSTIK_K40X256) || (defined BSP_TWR_K60D100M)
   #define AUDIO_TIMER              1
   #define _audio_timer_init_freq   _pit_init_freq

/* Audio timer for other families */
#else
   #define AUDIO_TIMER              BSP_LAST_TIMER
   #define _audio_timer_init_freq   _pit_init_freq
#endif

/* Audio timer frequency */
#define AUDIO_SPEAKER_FREQUENCY     (8*1000)

/* Audio timer clock */
#define AUDIO_TIMER_CLOCK           BSP_BUS_CLOCK

#endif
/* EOF */
