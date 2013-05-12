/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: HVAC_Private.h$
* $Version : 3.8.11.1$
* $Date    : Apr-25-2012$
*
* Comments:
*
*   
*
*END************************************************************************/
#ifndef __hvac_private_h__
#define __hvac_private_h__

#include "bsp.h"

#define HVAC_CONTROL_CYCLE_IN_SECONDS    1

#define HVAC_CONTROL_CYCLE_IN_TICKS      (HVAC_CONTROL_CYCLE_IN_SECONDS*BSP_ALARM_FREQUENCY)

#define HVAC_LOG_CYCLE_IN_CONTROL_CYCLES 15


#define HVAC_DEFAULT_TEMP     200   // in 1/10 degree C

#define HVAC_PARAMS_CHANGED  1    

#if defined(BSP_TWR_K70F120M) || defined(BSP_TWR_K60N512) || defined(BSP_TWR_K40X256) ||\
    defined(BSP_TWR_K40D100M) || defined(BSP_TWR_K60F120M) || defined(BSP_TWR_K60D100M)   
    #if BSPCFG_ENABLE_ADC1
        #define ENABLE_ADC BSPCFG_ENABLE_ADC1 
        #define MY_ADC "adc1:" /* must be #1 as the inputs are wired to ADC 1 */  
        #if defined (BSP_TWR_K40X256) || defined (BSP_TWR_K40D100M)
            #define ADC_CH_HEART BSP_ADC_CH_ACCEL_Y  
        #else
            #define ADC_CH_HEART  ADC1_SOURCE_ADPM1
        #endif 
        #define ADC_SOURCE0 ADC1_SOURCE_ADPM1
        #define MY_TRIGGER ADC_PDB_TRIGGER
    #endif /* BSPCFG_ENABLE_ADCx */

#else /* defined BSP_xxx */    
    #if BSPCFG_ENABLE_ADC
        #define ENABLE_ADC BSPCFG_ENABLE_ADC
        #define MY_ADC "adc:"    
        #define ADC_CH_HEART BSP_ADC_CH_POT
        #define ADC_SOURCE0 BSP_ADC_CH_POT
        #define MY_TRIGGER 0
    #endif /* BSPCFG_ENABLE_ADCx */
#endif /* defined BSP_xxx */


typedef struct {
   LWEVENT_STRUCT       Event;
   HVAC_Mode_t          HVACMode;
   FAN_Mode_t           FanMode;
   Temperature_Scale_t  TemperatureScale;
   uint_32              DesiredTemperature;
} HVAC_PARAMS, * HVAC_PARAMS_PTR;

typedef struct  {
   HVAC_Mode_t    HVACState;
   boolean        FanOn;
   uint_32        ActualTemperature;
} HVAC_STATE, * HVAC_STATE_PTR;


extern HVAC_STATE  HVAC_State;
extern HVAC_PARAMS HVAC_Params;

void HVAC_InitializeParameters(void); 
boolean HVAC_InitializeIO(void);
void HVAC_ResetOutputs(void);
void HVAC_SetOutput(HVAC_Output_t,boolean);
boolean HVAC_GetInput(HVAC_Input_t);
boolean HVAC_WaitParameters(int_32);
void HVAC_InitializeADC(void); 
_mqx_int ReadADC(void);

#endif
