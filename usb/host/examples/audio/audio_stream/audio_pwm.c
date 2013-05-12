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
* $FileName: audio_pwm.c$
* $Version : 
* $Date    : 
*
* Comments:
*
* @brief 
*
*****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "audio_pwm.h"

/*****************************************************************************
 * Constant and Macro's 
 *****************************************************************************/

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
void _pwm_init(void);

/****************************************************************************
 * Global Variables
 ****************************************************************************/
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)|| (defined BSP_TWRMCF51JF)   
volatile uint_16 *duty;
#else
volatile uint_8 *duty;
#endif

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/

/*****************************************************************************
 * Local Variables 
 *****************************************************************************/

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/******************************************************************************
 *
 *   @name        _pwm_init
 *
 *   @brief       This function initialyzes PWM.
 *
 *   @param       None
 *
 *   @return      None
 *****************************************************************************/
void _pwm_init()
{
#if (defined BSP_M52259DEMO) || (defined BSP_M52259EVB) || (defined BSP_TWRMCF52259)
   VMCF5225_PWM_STRUCT_PTR pwm = &(((VMCF5225_STRUCT_PTR)_PSP_GET_IPSBAR())->PWM);
   VMCF5225_GPIO_STRUCT_PTR gpio = &(((VMCF5225_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO);
#endif
#if (defined BSP_M52223EVB)
   VMCF5222_PWM_STRUCT_PTR pwm = &(((VMCF5222_STRUCT_PTR)_PSP_GET_IPSBAR())->PWM);
   VMCF5222_GPIO_STRUCT_PTR gpio = &(((VMCF5222_STRUCT_PTR)_PSP_GET_IPSBAR())->GPIO);
#endif
#if(defined BSP_TWRMCF52259)
   duty = &(pwm->PWMDTY[0]);

   /* Init IO for PWM */
   gpio->PTCPAR |= 0x03;

   pwm->PWME = 0;
   /* PCKA = 1: clock A rate = bus clk / 1 = 40MHz*/
   pwm->PWMPRCLK = 0x00;
      
   pwm->PWMPER[0] = 0xFF;
   pwm->PWMDTY[0] = 0x00;

   /* High at begin of period */
   pwm->PWMPOL = 0x2;
   /* chose clock source is A */
   pwm->PWMCLK = 0x00;
   /* No Concatenates */
   pwm->PWMCTL = 0x00;

   /* Enable PWM */
   pwm->PWME = 0x01;
#endif
#if(defined BSP_M52259DEMO)||(defined BSP_M52259EVB)||(defined BSP_M52223EVB)||(defined BSP_M52277EVB)
   /* update duty (channel 1) */
   duty = &(pwm->PWMDTY[1]);
#if(defined BSP_M52277EVB)
   /* Init IO for PWM */
   gpio->PAR_LCDL = 0x200;
#else
   /* Init IO for PWM */
   gpio->PTAPAR |= 0x03;
#endif
   /* Disable PWM */
   pwm->PWME = 0;
   
   /* PCKA = 1: clock A rate = bus clk / 1 = 40MHz*/
   pwm->PWMPRCLK = 0x00;

   pwm->PWMPER[1] = 0xFF;
   pwm->PWMDTY[1] = 0x00;
   
   /* High at begin of period */
   pwm->PWMPOL = 0x02;
   
   /* chose clock source is A */
   pwm->PWMCLK = 0x00;
   
   /* No Concatenates */
   pwm->PWMCTL = 0x00;

   /* Enable PWM */
   pwm->PWME = 0x02;
#endif

/* Initialize PWM module for CFV1 families */
#if (defined BSP_TWRMCF51JE)||(defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
#if (defined BSP_TWRMCF51JE)
   VMCF51JE_TPM_STRUCT_PTR tpm = &(((MCF51JE_STRUCT_PTR)_PSP_GET_MBAR())->TPM2);
   VMCF51JE_STRUCT_PTR reg_ptr = _PSP_GET_MBAR();
#endif
#if (defined BSP_TWRMCF51MM)
   VMCF51MM_TPM_STRUCT_PTR tpm = &(((MCF51MM_STRUCT_PTR)_PSP_GET_MBAR())->TPM2);
   VMCF51MM_STRUCT_PTR reg_ptr = _PSP_GET_MBAR();
#endif

#if (defined BSP_MCF51JMEVB)
   VMCF51JM_TPM2_STRUCT_PTR tpm = &(((MCF51JM_STRUCT_PTR)_PSP_GET_MBAR())->TPM2);
   VMCF51JM_STRUCT_PTR reg_ptr = _PSP_GET_MBAR();
   reg_ptr->SIMX.SCGC1 |= MCF51XX_SCGC1_TPM2_MASK;
#else
   reg_ptr->SIM.SCGC1 |= MCF51XX_SCGC1_TPM2_MASK;
#endif

   /* update duty */
   duty = (uint_16*)(&(tpm->TPMxC[0].TPMxCyV));
   tpm->TPMxSC = 0;
   tpm->TPMxMOD = 0xFF;
   tpm->TPMxC[0].TPMxCySC = 0x24;
   tpm->TPMxSC = 0x08;
#endif

/* Initialize PWM module for kinetis families */
#if (defined BSP_TWR_K40X256) || (defined BSP_TWR_K60N512) || (defined BSP_TWR_K53N512) || (defined BSP_TWR_K60D100M)
   duty = (uint_8 *)&(FTM0_C0V);
   /* FTM0_CH0 enable on PTA3 ****/
   PORTC_PCR1 = PORT_PCR_MUX(0x4); 

   /* Enable clock for FTM */
   SIM_SCGC6 |= SIM_SCGC6_FTM0_MASK;

   /* Set FTM mode */
   FTM0_MODE = (FTM_MODE_WPDIS_MASK | FTM_MODE_FTMEN_MASK);
   FTM0_MODE &= ~FTM_MODE_FTMEN_MASK;
   FTM0_SC = 0;
   FTM0_CNTIN = 0x00;

   /* Set FTM modular */
   FTM0_MOD = 0xFF;
   FTM0_QDCTRL &= ~(FTM_QDCTRL_QUADEN_MASK);
   FTM0_COMBINE &= ~(FTM_COMBINE_DECAPEN0_MASK \
      |FTM_COMBINE_DECAPEN1_MASK \
      |FTM_COMBINE_DECAPEN2_MASK \
      |FTM_COMBINE_DECAPEN3_MASK \
      |FTM_COMBINE_COMBINE0_MASK \
      |FTM_COMBINE_COMBINE1_MASK \
      |FTM_COMBINE_COMBINE2_MASK \
      |FTM_COMBINE_COMBINE3_MASK);
   FTM0_C0SC = 0x28;   

   /* Enable FTM */
   FTM0_SC = 0x08;  
#endif
#if (defined BSP_TWRMCF51JF)
   volatile MXC_MemMapPtr mxc;
   volatile FTM_MemMapPtr ftm1;
   
   mxc = MXC_BASE_PTR;
   ftm1 = FTM1_BASE_PTR;
   
   duty = (uint_16_ptr)&(ftm1->CHANNEL[0].CVH);
   
   mxc->PTAPF4 &= ~MXC_PTAPF4_A0(0xF);
   mxc->PTAPF4 |= MXC_PTAPF4_A0(0x4);
   
   /* Init counter value*/
   ftm1->CNTH = 0x00;
   ftm1->CNTL = 0x00;
   /* Set Modulo -> free run mode*/
   ftm1->MODH = 0x00;
   ftm1->MODL = 0xFF;
   
   /* Disable dual capture mode and combine channels */
   ftm1->COMBINE[0] &= ~(FTM_COMBINE_DECAPEN_MASK | FTM_COMBINE_COMBINE_MASK);
   
   /* Select channel 0 to generate PWM signal */
   ftm1->CHANNEL[0].CSC |= FTM_CSC_MSB_MASK;
   ftm1->CHANNEL[0].CSC  |= FTM_CSC_ELSB_MASK;
   ftm1->CHANNEL[0].CSC  &= ~FTM_CSC_ELSA_MASK;
   
   /* Start timer */
   ftm1->SC  = 0x08;
#endif
}
/* EOF */
