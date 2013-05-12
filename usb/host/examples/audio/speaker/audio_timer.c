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
* @file audio_timer.c
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

/******************************************************************************
* Includes
*****************************************************************************/
#include <string.h>
#include <mqx.h>

#include <bsp.h>

#include <usb.h>
#include <usb_types.h>
#include <host_main.h>
#include <host_common.h>

#include <usb_host_audio.h>

#include "audio_speaker.h"
#include "audio_timer.h"

/******************************************************************************
* Local functions
*****************************************************************************/
#if (defined BSP_TWRMCF51JE)||(defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB) || (defined BSP_TWRMCF51JF)
static void ClearCmtInterrupt(void);
#endif

/******************************************************************************
* Global functions
*****************************************************************************/
void audio_timer_init(void);
void AUDIO_TIMER_ISR(void);
extern void usb_host_audio_tr_callback(_usb_pipe_handle ,pointer ,uchar_ptr ,uint_32 ,uint_32 );

/******************************************************************************
* Global variable
*****************************************************************************/
extern volatile AUDIO_CONTROL_DEVICE_STRUCT audio_control;
extern volatile AUDIO_STREAM_DEVICE_STRUCT audio_stream;
extern uint_8 wav_buff[MAX_ISO_PACKET_SIZE];
extern uint_8 device_direction;
extern uint_32 packet_size;
extern volatile boolean trCallBack;

/******************************************************************************
*   @name        pit1_init
*
*   @brief       This function init PIT0
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void audio_timer_init(void)
{
    if(_int_install_isr(AUDIO_INT,
                (void (_CODE_PTR_)(pointer))AUDIO_TIMER_ISR, NULL) == NULL)
    {
        return ;
    } /* Endif */
#if (defined BSP_TWR_K40X256)  || (defined BSP_TWR_K60N512) ||\
	(defined BSP_TWR_K53N512)  || (defined BSP_KWIKSTIK_K40X256)||\
	(defined BSP_TWR_K70F120M) || (defined BSP_TWR_K60F120M) ||\
	(defined BSP_TWR_K20D72M)  || (defined BSP_TWR_K60D100M)
    _bsp_int_init(AUDIO_INT, 2, 0, TRUE);
#endif
}

#if(defined BSP_TWRMCF51JE)||(defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
/******************************************************************************
*   @name        cmt_init_freq
*
* Function Name    : cmt_init_freq
* Returned Value   :
* Comments         : Initialize timer module
*
*
*   @comment
*
*******************************************************************************/
uint_32 cmt_init_freq
(
/* [IN] ticks per second */
uint_32   tickfreq,

/* [IN] input clock speed in Hz */
uint_32   clk,

/* [IN] unmask the timer after initializing */
boolean   unmask_timer
)
{
    /* CMT memory map */
#if (defined BSP_TWRMCF51JE)
    VMCF51JE_CMT_STRUCT_PTR cmt = &(((MCF51JE_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
#if (defined BSP_TWRMCF51MM)
    VMCF51MM_CMT_STRUCT_PTR cmt = &(((MCF51MM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif

#if (defined BSP_MCF51JMEVB)
    VMCF51JM_CMT_STRUCT_PTR cmt = &(((MCF51JM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
    uint_32 rate;
    uint_8 prescale = 0;
    /* Calculate prescale */
    rate = (clk / (8*tickfreq));
    while (rate > (0xFFFF+1)) {
        prescale += 1;
        clk >>= 1;
        rate = (clk / tickfreq);
    }
    /* CMTMSC: EOCF=0,CMTDIV1=0,CMTDIV0=0,EXSPC=0,BASE=0,FSK=0,EOCIE=0,MCGEN=0 */
    cmt->CMTMSC = 0x00;
    /* CMTOC: IROL=0,CMTPOL=0,IROPEN=0,??=0,??=0,??=0,??=0,??=0 */
    cmt->CMTOC = 0x00;
    /* CMTCG1: PH7=0,PH6=0,PH5=0,PH4=0,PH3=0,PH2=0,PH1=0,PH0=0,PL7=0,PL6=0,PL5=0,PL4=0,PL3=0,PL2=0,PL1=0,PL0=0 */
    cmt->CMTCGH1 = 0x00;
    cmt->CMTCGL1 = 0x00;
    /* CMTCG2: SH7=0,SH6=0,SH5=0,SH4=0,SH3=0,SH2=0,SH1=0,SH0=0,SL7=0,SL6=0,SL5=0,SL4=0,SL3=0,SL2=0,SL1=0,SL0=0 */
    cmt->CMTCGH2 = 0x00;
    cmt->CMTCGL2 = 0x00;
    /* CMTCMD12: MB15=0,MB14=0,MB13=0,MB12=0,MB11=0,MB10=0,MB9=0,MB8=1,MB7=0,MB6=1,MB5=1,MB4=1,MB3=0,MB2=1,MB1=1,MB0=0 */
    cmt->CMTCMD1 = (uint_8)((rate & 0xFF00)>>8);
    cmt->CMTCMD2 = (uint_8)(rate & 0x00FF);
    /* CMTCMD34: SB15=0,SB14=0,SB13=0,SB12=0,SB11=0,SB10=0,SB9=0,SB8=0,SB7=0,SB6=0,SB5=0,SB4=0,SB3=0,SB2=0,SB1=0,SB0=0 */
    cmt->CMTCMD3 = 0x00;
    cmt->CMTCMD4 = 0x00;
    /* CMTMSC: EOCF=0,CMTDIV1=0,CMTDIV0=0,EXSPC=0,BASE=0,FSK=0,EOCIE=1,MCGEN=1 */
    cmt->CMTMSC |= 0x01;
    cmt->CMTMSC |= prescale<<5;

    if(unmask_timer)
        EnableCmtInterrupt();
    return rate;
}

/******************************************************************************
*   @name        EnableCmtInterrupt
*
*   @brief       This function is called to enable CMT interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void EnableCmtInterrupt(void)
{
#if (defined BSP_TWRMCF51JE)
    VMCF51JE_CMT_STRUCT_PTR cmt = &(((MCF51JE_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif

#if (defined BSP_TWRMCF51MM)
    VMCF51MM_CMT_STRUCT_PTR cmt = &(((MCF51MM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif

#if (defined BSP_MCF51JMEVB)
    VMCF51JM_CMT_STRUCT_PTR cmt = &(((MCF51JM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
    /* Enable Timer Interrupt */
    cmt->CMTMSC |= 0x02;
    return;
}

/******************************************************************************
*   @name        DisableCmtInterrupt
*
*   @brief       This function is called to disable CMT interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void DisableCmtInterrupt(void)
{
#if(defined BSP_TWRMCF51JE)
    VMCF51JE_CMT_STRUCT_PTR cmt = &(((MCF51JE_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
#if(defined BSP_TWRMCF51MM)
    VMCF51MM_CMT_STRUCT_PTR cmt = &(((MCF51MM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
#if(defined BSP_MCF51JMEVB)
    VMCF51JM_CMT_STRUCT_PTR cmt = &(((MCF51JM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
    /* Disable Timer Interrupt */
    cmt->CMTMSC &= ~(0x02);
    return;
}

/******************************************************************************
*   @name        ClearCmtInterrupt
*
*   @brief       This function is called to clear CMT interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
static void ClearCmtInterrupt(void)
{
#if(defined BSP_TWRMCF51JE)
    VMCF51JE_CMT_STRUCT_PTR cmt = &(((MCF51JE_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
#if(defined BSP_TWRMCF51MM)
    VMCF51MM_CMT_STRUCT_PTR cmt = &(((MCF51MM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
#if(defined BSP_MCF51JMEVB)
    VMCF51JM_CMT_STRUCT_PTR cmt = &(((MCF51JM_STRUCT_PTR)_PSP_GET_MBAR())->CMT);
#endif
    /* Clear Timer Interrupt */
    (void)cmt->CMTMSC;
    (void)cmt->CMTCMD2;
    return;
}
#endif

#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) ||\
    (defined BSP_KWIKSTIK_K40X256) || (defined BSP_TWR_K70F120M) || (defined BSP_TWR_K60F120M) ||\
	(defined BSP_TWR_K20D72M) || (defined BSP_TWR_K60D100M)
/******************************************************************************
*   @name        _pit_init_freq
*
*   @brief       This function will set up a timer interrupt
*
*   @return      he clock rate for the timer (ns per hw tick)
*
*   @comment
*
*******************************************************************************/
void _kinetis_timer_init_freq
(
/* [IN] the timer to initialize */
_mqx_uint timer,

/* [IN] ticks per second */
uint_32   tickfreq,

/* [IN] the system clock speed in MHz */
uint_32   system_clock,

/* [IN] Unmask the timer after initializing */
boolean   unmask_timer
)
{ /* Body */

    if(tickfreq != 0)
    {
        /* Enable PIT Module Clock */
        SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

        /* Configure PIT */
        PIT_MCR = ~(PIT_MCR_FRZ_MASK |PIT_MCR_MDIS_MASK);

        /* Timer counter value */
        PIT_LDVAL_REG(PIT_BASE_PTR,timer) = system_clock/tickfreq;

        /* Mask PIT interrupt flag */
        PIT_TFLG_REG(PIT_BASE_PTR,timer)= PIT_TFLG_TIF_MASK;

    }
    if (unmask_timer)
    {
        _kinetis_timer_unmask_int(timer);
    }
}

/******************************************************************************
*   @name        _kinetis_timer_mask_int
*
*   @brief       This function enables PIT timer interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void _kinetis_timer_mask_int(uint_32 timer)
{
    PIT_TCTRL_REG(PIT_BASE_PTR,timer)&=~PIT_TCTRL_TEN_MASK;
    PIT_TCTRL_REG(PIT_BASE_PTR,timer)&=~PIT_TCTRL_TIE_MASK;
}

/******************************************************************************
*   @name        _kinetis_timer_unmask_int
*
*   @brief       This function disables PIT timer interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void _kinetis_timer_unmask_int(uint_32 timer)
{
    PIT_TCTRL_REG(PIT_BASE_PTR,timer)|= PIT_TCTRL_TEN_MASK;
    PIT_TCTRL_REG(PIT_BASE_PTR,timer)|= PIT_TCTRL_TIE_MASK;
}

/******************************************************************************
*   @name        _kinetis_timer_clear_int
*
*   @brief       This function clears PIT interrupt flag
*
*   @return      None
*
*   @comment
*
*
*******************************************************************************/
void _kinetis_timer_clear_int(uint_32 timer)
{
    PIT_TFLG_REG(PIT_BASE_PTR,timer) |= PIT_TFLG_TIF_MASK;
}
#endif

#if(defined BSP_TWRMCF51JF)
/******************************************************************************
*   @name        ftm_init_freq
*
*   @brief       This function initializes FTM0 timer module
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
uint_32 ftm_init_freq
(
/* [IN] the timer to initialize */
_mqx_uint timer,

/* [IN] ticks per second */
uint_32   tickfreq,

/* [IN] input clock speed in Hz */
uint_32   clk,

/* [IN] unmask the timer after initializing */
boolean   unmask_timer
)
{
    uint_32 rate;
    uint_8 prescale = 0;
    /* FTM memory map */
    volatile FTM_MemMapPtr ftm;
    /* Select FTM module */
    if(timer >1)
        printf("\nInvalid parameter");
    else if(timer)
        ftm = FTM1_BASE_PTR;
    else
        ftm = FTM0_BASE_PTR;
    /* FTM count */
    ftm->CNT = 0x0000;

    rate = (clk / tickfreq);
    while (rate > (0xFFFF+1)) {
        prescale += 1;
        clk >>= 1;
        rate = (clk / tickfreq);
    }
    ftm->MOD = (uint_16)(rate & 0x0000FFFF);
    ftm->SC &= ~FTM_SC_TOF_MASK;
    /* Start timer */
    ftm->SC |= 0x08;
    ftm->SC |= prescale;
    if(unmask_timer)
    EnableFTMInterrupt(timer);
    return rate;
}

/******************************************************************************
*   @name        EnableFTM0Interrupt
*
*   @brief       This function is called to enable FTM0 interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void EnableFTMInterrupt(uint_32 timer)
{
    volatile FTM_MemMapPtr ftm;
    /* Select FTM timer */
    if(timer >1)
        printf("\nInvalid parameter");
    else if(timer)
        ftm = FTM1_BASE_PTR;
    else
        ftm = FTM0_BASE_PTR;
    /* Enable FTM timer */
    ftm->SC |= FTM_SC_TOIE_MASK;
}

/******************************************************************************
*   @name        DisableFTM0Interrupt
*
*   @brief       This function is called to disable FTM0 interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void DisableFTMInterrupt(uint_32 timer)
{
    volatile FTM_MemMapPtr ftm;
    /* Select FTM timer */
    if(timer >1)
        printf("\nInvalid parameter");
    else if(timer)
        ftm = FTM1_BASE_PTR;
    else
    ftm = FTM0_BASE_PTR;

    /* Disable Timer Interrupt */
    ftm->SC &= ~FTM_SC_TOIE_MASK;
}

/******************************************************************************
*   @name        ClearFTM0Interrupt
*
*   @brief       This function is called to FTM0 interrupt
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
static void ClearFTMInterrupt(uint_32 timer)
{
    volatile FTM_MemMapPtr ftm;
    if (timer >1)
        printf("\nInvalid parameter");
    else if (timer)
        ftm = FTM1_BASE_PTR;
    else
        ftm = FTM0_BASE_PTR;
    /* Clear Timer Interrupt */
    ftm->SC &= ~FTM_SC_TOF_MASK;
    return;
}
#endif
/******************************************************************************
*   @name        AUDIO_TIMER_ISR
*
*   @brief       This routine services RTC Interrupt
*
*   @param       None
*
*   @return      None
*
******************************************************************************
* Services Programmable Interrupt Timer 0. If a Timer Object expires, then
* removes the object from Timer Queue and Calls the callback function
* (if registered)
*****************************************************************************/
static void AUDIO_TIMER_ISR(void)
{
    _audio_clear_int(AUDIO_TIMER);
    if(device_direction == IN_DEVICE)
    {
        if (USB_DEVICE_INTERFACED == audio_stream.DEV_STATE)
        {
            /* Send data */
            usb_audio_send_data((CLASS_CALL_STRUCT_PTR)&audio_control.CLASS_INTF,
            (CLASS_CALL_STRUCT_PTR)&audio_stream.CLASS_INTF, usb_host_audio_tr_callback,
            NULL, packet_size, (uchar_ptr)wav_buff);
        }
    }
#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_TWR_K70F120M) || (defined BSP_KWIKSTIK_K40X256) || (defined BSP_TWR_K60F120M) || (defined BSP_TWR_K20D72M)
    /* Enable PIT interrupt neeed due to errata on Kinetis PIT */
    PIT_TCTRL_REG(PIT_BASE_PTR,AUDIO_TIMER) |= PIT_TCTRL_TIE_MASK;
#endif
}
/* EOF */
