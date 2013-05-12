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
* $FileName: Security_IO.c$
* $Version : 3.8.26.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <string.h>
#include "security_private.h"

#if defined(BSP_BUTTON1)
   #define DOOR_STATE      BSP_BUTTON1
#endif
#if defined(BSP_BUTTON2)
   #define WINDOW_STATE    BSP_BUTTON2
#endif
   #define BUTTONS   1
#if (!defined BSP_BUTTON1 && !defined BSP_BUTTON2)
   #warning "Buttons are not available in this BSP"
#else 
      #define BUTTONS         1
#endif 

#ifdef BSP_LED1
   #define LED_1       BSP_LED1
#endif
#ifdef BSP_LED2
   #define LED_2       BSP_LED2
#endif
#ifdef BSP_LED3
   #define LED_3       BSP_LED3
#endif
#ifdef BSP_LED4
   #define LED_4       BSP_LED4
#endif

#if defined BSP_ADC_CH_ACCEL_X && defined BSP_ADC_CH_ACCEL_Y && defined BSP_ADC_CH_ACCEL_Z
    #define ADC_CH_COUNT    4
#else
    #define ADC_CH_COUNT    1
#endif /* defined BSP_ADC_CH_ACCEL_X && defined BSP_ADC_CH_ACCEL_Y && defined BSP_ADC_CH_ACCEL_Z */

const char_ptr SEC_OutputName[] = { "LED1","LED2","Entry Open","Movement Detected" };


boolean SEC_OutputState[SEC_MAX_OUTPUTS] ={0};
static boolean input_port=0, output_port=0;
#ifdef LED_1
LWGPIO_STRUCT led1;
#endif
#ifdef LED_2
LWGPIO_STRUCT led2;
#endif
#ifdef LED_3
LWGPIO_STRUCT led3;
#endif
#ifdef LED_4
LWGPIO_STRUCT led4;
#endif
#ifdef BSP_BUTTON1
LWGPIO_STRUCT button1;
#endif
#ifdef BSP_BUTTON2
LWGPIO_STRUCT button2;
#endif
  
boolean SEC_InitializeIO(void) 
{

    /* Init Gpio for Leds as output to drive LEDs (LED10 - LED13) */
#ifdef LED_1
       output_port = lwgpio_init(&led1, LED_1, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED1 failed.\n");
       }
       lwgpio_set_functionality(&led1, BSP_LED1_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led1, LWGPIO_VALUE_LOW);
#endif
    
#ifdef LED_2
       output_port = lwgpio_init(&led2, LED_2, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED2 failed.\n");
    
       }
       lwgpio_set_functionality(&led2, BSP_LED2_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led2, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_3
       output_port = lwgpio_init(&led3, LED_3, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED3 failed.\n");
       }
       lwgpio_set_functionality(&led3, BSP_LED3_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led3, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_4
       output_port = lwgpio_init(&led4, LED_4, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED4 failed.\n");
       }
       lwgpio_set_functionality(&led4, BSP_LED4_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led4, LWGPIO_VALUE_LOW);
#endif

#ifdef BSP_BUTTON1
        /* Open and set port DD as input to read value from switches */
           input_port = lwgpio_init(&button1, DOOR_STATE, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE);
           if(!input_port)
           {
               printf("Initializing LW GPIO for button1 as input failed.\n");
               _task_block();
           }    
           lwgpio_set_functionality(&button1 ,BSP_BUTTON1_MUX_GPIO);
           lwgpio_set_attribute(&button1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
#endif
           
#ifdef BSP_BUTTON2
           input_port = lwgpio_init(&button2, WINDOW_STATE, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE);
           if(!input_port)
           {
               printf("Initializing LW GPIO for button2 as input failed.\n");
               _task_block();
           }
           lwgpio_set_functionality(&button2, BSP_BUTTON2_MUX_GPIO);
           lwgpio_set_attribute(&button2, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
     
#endif
    return (input_port!=0) && (output_port!=0);
}
     
      
void SEC_ResetOutputs(void) 
{
   SEC_Output_t i;
   
   for (i=SEC_LED1_OUTPUT;i<SEC_MAX_OUTPUTS;i++) {
      SEC_SetOutput(i,FALSE);
   }
}

  
void SEC_SetOutput(SEC_Output_t signal,boolean state) 
{    
   if (SEC_OutputState[signal] != state) {
      SEC_OutputState[signal] = state;
      if (output_port) {
         switch (signal) {
#ifdef LED_1
             case SEC_LED1_OUTPUT:
                (state) ? lwgpio_set_value(&led1, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led1, LWGPIO_VALUE_LOW);            	
                break;
#endif
#ifdef LED_2
             case SEC_LED2_OUTPUT:
                (state) ? lwgpio_set_value(&led2, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led2, LWGPIO_VALUE_LOW);            	
                break;
#endif
#ifdef LED_3
             case SEC_LED3_OUTPUT:
                (state) ? lwgpio_set_value(&led3, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led3, LWGPIO_VALUE_LOW);            	
                break;
#endif
#ifdef LED_4
             case SEC_OPEN_OUTPUT:
                (state) ? lwgpio_set_value(&led4, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led4, LWGPIO_VALUE_LOW);            	
                break;
#endif
         }
      }
   }
}
 
boolean SEC_GetOutput(SEC_Output_t signal)
{
   return SEC_OutputState[signal];
} 

char_ptr SEC_GetOutputName(SEC_Output_t signal)
{
   return SEC_OutputName[signal];
}  
   
boolean SEC_GetInput(SEC_Input_t signal)
{
   boolean  value=FALSE;
   
#if BUTTONS
   if (input_port) {
      switch (signal) {
#ifdef BSP_BUTTON1
         case SEC_DOOR_INPUT:
            value = !lwgpio_get_value(&button1);
             break; 
#endif
#ifdef BSP_BUTTON2
         case SEC_WINDOW_INPUT:
            value = !lwgpio_get_value(&button2);
            break;
#endif
      }
   }
#endif /* BUTTONS */

  return value;
}

const ADC_INIT_STRUCT adc_init = {
    ADC_RESOLUTION_DEFAULT,     /* resolution */
};

const ADC_INIT_CHANNEL_STRUCT adc_ch_param[ADC_CH_COUNT] = {
    {
        BSP_ADC_CH_POT,
        ADC_CHANNEL_MEASURE_LOOP | ADC_CHANNEL_START_NOW,
        10,            /* number of samples in one run sequence */
        0,             /* time offset from trigger point in ns */
        50000,         /* period in us (50ms) */
        0x10000,       /* scale range of result (not used now) */
        1,             /* circular buffer size (sample count) */
        MY_TRIGGER     /* logical trigger ID that starts this ADC channel */
    },
#if defined BSP_ADC_CH_ACCEL_X && defined BSP_ADC_CH_ACCEL_Y && defined BSP_ADC_CH_ACCEL_Z
    {
        BSP_ADC_CH_ACCEL_X,
        ADC_CHANNEL_MEASURE_LOOP | ADC_CHANNEL_START_NOW,
        10,            /* number of samples in one run sequence */
        0,             /* time offset from trigger point in ns */
        50000,     /* period in us (50ms) */
        0x10000,       /* scale range of result (not used now) */
        1,          /* circular buffer size (sample count) */
        MY_TRIGGER  /* logical trigger ID that starts this ADC channel */
    },
    {
        BSP_ADC_CH_ACCEL_Y,
        ADC_CHANNEL_MEASURE_LOOP | ADC_CHANNEL_START_NOW,
        10,            /* number of samples in one run sequence */
        0,             /* time offset from trigger point in ns */
        50000,     /* period in us (50ms) */
        0x10000,       /* scale range of result (not used now) */
        1,          /* circular buffer size (sample count) */
        MY_TRIGGER  /* logical trigger ID that starts this ADC channel */
    },
    {
        BSP_ADC_CH_ACCEL_Z,
        ADC_CHANNEL_MEASURE_LOOP | ADC_CHANNEL_START_NOW,
        10,            /* number of samples in one run sequence */
        0,             /* time offset from trigger point in ns */
        50000,         /* period in us (50ms) */
        0x10000,       /* scale range of result (not used now) */
        1,             /* circular buffer size (sample count) */
        MY_TRIGGER     /* logical trigger ID that starts this ADC channel */
    }
#endif /* defined BSP_ADC_CH_ACCEL_X && defined BSP_ADC_CH_ACCEL_Y && defined BSP_ADC_CH_ACCEL_Z */
};

static MQX_FILE_PTR fd_adc, fd_ch[ADC_CH_COUNT];

/*
 *  Setup ADC module to read in accelerometer and potentiometer values
 */   
void SEC_InitializeADC(void) {
    _mqx_int i;
    char dev_name[10];
    fd_adc = fopen(MY_ADC, (const char*)&adc_init);
    if (NULL == fd_adc) {    
        printf("ADC device open failed\n");
        _task_block();
    }
    
    for (i = 0; i < ADC_CH_COUNT; i++) {
        sprintf(dev_name, "%s%d", MY_ADC, i);
        fd_ch[i] = fopen(dev_name, (const char*)&adc_ch_param[i]);
        if (NULL == fd_ch[i]) {    
            printf("%s%d channel open failed\n", MY_ADC, i);
            _task_block();
        }
    }
    _time_delay (100);

#if ADC_CH_COUNT > 1
    for (i = 0; i < 3; i++) {
      SEC_Params.last[i] = ReadADC(ADC_ACCX + i);
    }
    
    SEC_Params.flat=SEC_Params.last[ACCY];
#endif

    _time_delay (200);
}

/*
 *  Read in ADC value on the channel given
 */
_mqx_int ReadADC(_mqx_int channel) {
    ADC_RESULT_STRUCT adc_res;
    _mqx_int read_bytes;

    if (channel < ADC_CH_COUNT) {
        read_bytes = read(fd_ch[channel], &adc_res, sizeof(adc_res));
        if (read_bytes == 0)
            adc_res.result = 0; 
    }
    else
        adc_res.result = 0; 
    
    return (_mqx_int) adc_res.result;
}
