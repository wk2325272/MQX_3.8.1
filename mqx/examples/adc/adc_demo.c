/**HEADER********************************************************************
* 
* Copyright (c) 2008-2009 Freescale Semiconductor;
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
* $FileName: adc_demo.c$
* $Version : 3.8.33.6$
* $Date    : May-2-2012$
*
* Comments:
*
*   This file contains the source for the ADC example program.
*   Two channels are running, one is running in loop, the second one
*   only once.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>

#if MQX_USE_LWEVENTS
#include <lwevent.h>
#endif

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if defined(BSP_M51EMDEMO)
    #define MY_ADC "adc1:" /* must be #1 as the inputs are wired to ADC 1 */
    #define MY_TRIGGER ADC_PDB_TRIGGER
    #if !BSPCFG_ENABLE_ADC1
        #error This application requires BSPCFG_ENABLE_ADC1 defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif /* BSPCFG_ENABLE_ADCx */
#elif defined(BSP_TWRMCF51AG)
    #define MY_ADC "adc:" /* must be #1 as the inputs are wired to ADC 1 */
    #define MY_TRIGGER ADC_PDB_TRIGGER
    #if !BSPCFG_ENABLE_ADC
        #error This application requires BSPCFG_ENABLE_ADC defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif /* BSPCFG_ENABLE_ADCx */
#elif   defined(BSP_TWR_K70F120M) || (BSP_TWR_K60N512) || defined(BSP_TWR_K40X256) || defined(BSP_TWR_K40D100M) ||\
        defined(BSP_TWR_K53N512) || defined(BSP_TWR_K60F120M) || defined(BSP_TWR_K60D100M) || defined(BSP_TWR_K20D72M)
    #define MY_ADC "adc1:" /* must be #1 as the inputs are wired to ADC 1 */
    #define MY_TRIGGER ADC_PDB_TRIGGER
    #if !BSPCFG_ENABLE_ADC1
        #error This application requires BSPCFG_ENABLE_ADC1 defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif /* BSPCFG_ENABLE_ADCx */
#elif defined(BSP_KWIKSTIK_K40X256) || defined(BSP_TWR_K20D50M)
    #define MY_ADC "adc0:" 
    #define MY_TRIGGER ADC_PDB_TRIGGER
    #if !BSPCFG_ENABLE_ADC0
    #error This application requires BSPCFG_ENABLE_ADC0 defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif    /* BSPCFG_ENABLE_ADCx */     
#elif defined(BSP_TWRMCF51JF) || defined(BSP_TWRMCF51QM)
    #define MY_ADC "adc:"
    #define MY_TRIGGER ADC_PDB_TRIGGER
    #if !BSPCFG_ENABLE_ADC
        #error This application requires BSPCFG_ENABLE_ADC defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif /* BSPCFG_ENABLE_ADCx */
#elif defined(BSP_TWRMCF51MM) || defined(BSP_TWRMCF51JE)
    #define MY_ADC "adc:"
    #define MY_TRIGGER ADC_PDB_TRIGGER
    #if !BSPCFG_ENABLE_ADC
        #error This application requires BSPCFG_ENABLE_ADC defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif /* BSPCFG_ENABLE_ADC */
#else /* defined BSP_xxx */
    #define MY_ADC "adc:"
    #define MY_TRIGGER ADC_TRIGGER_1
    #if !BSPCFG_ENABLE_ADC
        #error This application requires BSPCFG_ENABLE_ADC defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif /* BSPCFG_ENABLE_ADCx */
#endif /* defined BSP_xxx */

/* Task IDs */
#define MAIN_TASK 5

/* Function prototypes */
extern void main_task(uint_32);

const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {MAIN_TASK,     main_task, 1000,      8,        "Main",     MQX_AUTO_START_TASK,    0,      0           },
    {0}
};


/* ADC device init struct */
const ADC_INIT_STRUCT adc_init = {
    ADC_RESOLUTION_DEFAULT,     /* resolution */
};

#if MQX_USE_LWEVENTS
static LWEVENT_STRUCT evn;
#endif

/* Logical channel #1 init struct */
const ADC_INIT_CHANNEL_STRUCT adc_channel_param1 = 
{
#if defined(BSP_M51EMDEMO) 
    BSP_ADC_CH_POT1, /* physical ADC channel */
#elif   defined(BSP_TWR_K70F120M) || (BSP_TWR_K60N512) || defined(BSP_TWR_K40X256) ||\
        defined(BSP_TWR_K40D100M) || defined(BSP_TWR_K53N512) || defined(BSP_TWR_K60D100M)
    ADC1_SOURCE_ADPM1, /* physical ADC channel */
#elif defined(BSP_TWR_K20D72M)
    ADC1_SOURCE_AD19,
#elif defined(BSP_KWIKSTIK_K40X256) 
    ADC0_SOURCE_ADPM0, /* physical ADC channel */
#elif defined(BSP_TWRMCF51JF) || defined(BSP_TWRMCF51QM)
    ADC0_SOURCE_AD12, /* physical ADC channel */
#else
    BSP_ADC_CH_POT, /* physical ADC channel */
#endif
    ADC_CHANNEL_MEASURE_LOOP | ADC_CHANNEL_START_TRIGGERED, /* runs continuously after IOCTL trigger */
    10,             /* number of samples in one run sequence */
    0,              /* time offset from trigger point in us */
    300000,         /* period in us (= 0.3 sec) */
    0x10000,        /* scale range of result (not used now) */
    10,             /* circular buffer size (sample count) */
    MY_TRIGGER,     /* logical trigger ID that starts this ADC channel */
#if MQX_USE_LWEVENTS
    &evn,
    0x01            /* mask of event to be set */
#endif
};

/* Logical channel #2 init struct */
const ADC_INIT_CHANNEL_STRUCT adc_channel_param2 = 
{
#if defined(BSP_M51EMDEMO) 
    BSP_ADC_CH_POT2, /* physical ADC channel */
#elif   defined(BSP_TWR_K70F120M) || (BSP_TWR_K60N512) || defined(BSP_TWR_K40X256) || defined(BSP_TWR_K40D100M) ||\
        defined(BSP_TWR_K53N512) || defined(BSP_TWR_K20D72M) || defined(BSP_TWR_K60F120M) || defined(BSP_TWR_K60D100M)
    ADC1_SOURCE_AD3,
#elif defined(BSP_KWIKSTIK_K40X256) 
    ADC0_SOURCE_AD19, /* physical ADC channel */
#elif defined(BSP_TWRMCF51MM) || defined(BSP_TWRMCF51JE) || defined(BSP_TWRMCF51AG)
    ADC_SOURCE_AD1, /* physical ADC channel */
#elif defined(BSP_MCF51AC)
    BSP_ADC_CH_ACCEL_Y, /* physical ADC channel */
#elif defined(BSP_TWRMCF51QM)||defined(BSP_TWRMCF51JF)
    ADC0_SOURCE_AD9, /* physical ADC channel */
#elif ADC_MAX_MODULES <= 2
    0,
#else
    ADC_SOURCE_AN1, /* physical ADC channel */
#endif
    ADC_CHANNEL_MEASURE_ONCE | ADC_CHANNEL_START_NOW, /* one sequence is sampled after fopen */
    15,             /* number of samples in one run sequence */
    150000,         /* time offset from trigger point in us */
    600000,         /* period in us (= 0.6 sec) */
    0x10000,        /* scale range of result (not used now) */
    15,             /* circular buffer size (sample count) */
    MY_TRIGGER,     /* logical trigger ID that starts this ADC channel */
#if MQX_USE_LWEVENTS
    NULL
#endif
};
/*TASK*-----------------------------------------------------
** 
** Task Name    : main_task
** Comments     :
**
*END*-----------------------------------------------------*/
void main_task
   (
      uint_32 initial_data
   )
{
    ADC_RESULT_STRUCT data;
    _mqx_int i;
    MQX_FILE_PTR f, f_ch1;
#if ADC_MAX_MODULES > 1
    MQX_FILE_PTR f_ch2;
#endif
    printf("\n\n-------------- Begin ADC example --------------\n\n");

#if MQX_USE_LWEVENTS
    if (_lwevent_create(&evn, 0) != MQX_OK) {
        printf("\nMake event failed!\n");
        _task_block();
    }
#endif


    printf("Opening ADC device ...");
    f = fopen(MY_ADC, (const char*)&adc_init);
    if(f != NULL)
    {    
        printf("done\n");
    }
    else
    {    
        printf("failed\n");
        _task_block();
    }


    printf("Opening channel #1 ...");
    f_ch1 = fopen(MY_ADC "first", (const char*)&adc_channel_param1);
    if(f_ch1 != NULL)
    {    
        printf("done, prepared to start by trigger\n");
    }
    else
    {    
        printf("failed\n");
        _task_block();
    }

#if ADC_MAX_MODULES > 1 
    printf("Opening channel #2 ...");
    f_ch2 = fopen(MY_ADC "second", (const char*)&adc_channel_param2); /* adc:2 is running now */
    if(f_ch2 != NULL)
    {    
        printf("done, one sequence started automatically\n");
    }
    else
    {    
        printf("failed\n");
        _task_block();
    }
#endif

    _time_delay(1000);
    
    printf("Triggering channel #1...");
    ioctl(f, ADC_IOCTL_FIRE_TRIGGER, (pointer) MY_TRIGGER);
    printf("triggered!\n");

    for(i = 0; ; i++) 
    {    
        /* channel 1 sample ready? */
        if (read(f_ch1, &data, sizeof(data) ))
            printf("ADC ch 1: %4d ", data.result);
        else
            printf("               ");

#if ADC_MAX_MODULES > 1
        /* channel 2 sample ready? */
        if (read(f_ch2, &data, sizeof(data) ))
            printf("ADC ch 2: %d\n", data.result);
        else
            printf("\n");
#else
        printf("\n");
#endif
        if (i == 37) {
            printf("Pausing channel #1...");
            if (IO_OK == ioctl (f_ch1, ADC_IOCTL_PAUSE_CHANNEL, NULL))
              printf("stopped!\n");
            else
              printf("failed!\n");
        }
        if (i == 53) {
            printf("Resuming channel #1...");
            if (IO_OK == ioctl (f_ch1, ADC_IOCTL_RESUME_CHANNEL, NULL))
              printf("resumed!\n");
            else
              printf("failed!\n");
        }

        _time_delay(100);
    }

#if MQX_USE_LWEVENTS
    printf("Waiting for event...");
    if (_lwevent_wait_ticks(&evn, 0x01, TRUE, 0) != MQX_OK) {
        printf("failed!\n");
    }
    else
        printf("succeed!\n");
#endif
    
    /* close all ADC channels */
    printf("Closing channels...");
    fclose(f_ch1);
#if ADC_MAX_MODULES > 1
    fclose(f_ch2);
#endif
    fclose(f);

    printf("done!\n");
    
    _task_block();
}  

/* EOF */
