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

/******************************************************************************
* Definitions
*****************************************************************************/
#define AUDIO_SPEAKER_FREQUENCY         1000
#define AUDIO_TIMER_CLOCK               BSP_BUS_CLOCK

/******************************************************************************
* Global function prototypes
*****************************************************************************/
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
extern uint_32 cmt_init_freq(uint_32 tickfreq,uint_32 clk,boolean unmask_timer);
extern void DisableCmtInterrupt(void);
extern void EnableCmtInterrupt(void);
#endif

#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) ||\
 	(defined BSP_KWIKSTIK_K40X256) || (defined BSP_TWR_K70F120M) || (defined BSP_TWR_K60F120M) ||\
	(defined BSP_TWR_K20D72M) || (defined BSP_TWR_K60D100M)
extern void _kinetis_timer_clear_int(uint_32 timer);
extern void _kinetis_timer_mask_int(uint_32 timer);
extern void _kinetis_timer_unmask_int(uint_32 timer);
extern void _kinetis_timer_init_freq(_mqx_uint timer,uint_32 system_clock, uint_32 tickfreq, boolean unmask_timer);
#endif

#if (defined BSP_TWRMCF51JF)
extern uint_32 ftm_init_freq(_mqx_uint timer,uint_32 tickfreq,uint_32 clk,boolean unmask_timer);
extern void EnableFTMInterrupt(uint_32 timer);
extern void DisableFTMInterrupt(uint_32 timer);
#endif
void audio_timer_init(void);
#endif
