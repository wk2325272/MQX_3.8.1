/**HEADER***********************************************************************
*
* Copyright (c) 2008-2009 Freescale Semiconductor;
* All Rights Reserved
*
********************************************************************************
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
********************************************************************************
*
* $FileName: dac_demo.c$
* $Version : 3.8.9.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This example demonstrates how to use DAC device driver
*
*   1. It generates sine signal with period defined by GEN_SIGNAL_PERIOD on
*      DACO pin. The signal amplitude sweeps between minimal to maximal value.
*      It can be observed on A32 pin TWR-ELEV FUNCTIONAL
*      or TWR-PROTO board by scope.");
*   2. It also toggles by LED1 on board using LWGPIO driver to see
*      that application is running.
*
*END***************************************************************************/

#include <mqx.h>
#include <bsp.h>

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h.
/* Please recompile BSP with this option. */
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL.
/*
 * Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h
 * and recompile BSP with this option.
 */
#endif

#define TERMINAL_CURSOR_POSITION_MAX    (80)
#define DAC_INTERNAL_BUFFER_SIZE        (16)

#define PDB_PRESCALER                   (0)     /* 000 timer uses peripheral clock */

#define GEN_NUMBER_OF_SAMPLES           (256)
#define GEN_PEAK2PEAK                   (4096)
#define GEN_PEAK2PEAK_STEP              (64)
#define GEN_OFFSET                      (2048)
#define GEN_SIGNAL_PERIOD               (4000)  /* [us] signal period  */

/* Macro for scaling signal period */
#define GEN_SCALE_PERIOD(x)             (   (BSP_BUS_CLOCK)          \
                                          / ((PDB_PRESCALER) + 1)    \
                                          / (GEN_NUMBER_OF_SAMPLES)  \
                                          / (1000000 / (x)))

/* Function prototypes */
void DAC_BufferWattermarkCallBack(LDD_RTOS_TDeviceDataPtr DeviceData);
void DAC_BufferEndCallBack(LDD_RTOS_TDeviceDataPtr DeviceData);

/* Signal generator functions */
uint_16_ptr GEN_CreateTable(int_16_ptr table, uint_16 table_size, int_16 peak_peak, int_16 offset);
_mqx_uint   GEN_DestroyTable(uint_16_ptr table);
_mqx_int    GEN_SetSignalPeriod(uint_32 pdb_prescaler, uint_32 dac_interval);


/* Task IDs             */
#define MAIN_TASK           5

/* Task prototypes      */
void main_task(uint_32);

/* Task template list   */
const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
    /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
    {MAIN_TASK,     main_task,  1500,     8,        "Main",     MQX_AUTO_START_TASK,    0,      0           },
    {0}
};


const LDD_RTOS_TDeviceData  DAC_DeviceData =
{
/* DAC device number                    */  DAC_1,
/* DAC reference selection              */  DAC_PDD_V_REF_EXT,
/* DAC trigger mode                     */  DAC_PDD_HW_TRIGGER,
/* DAC buffer mode                      */  LDD_DAC_BUFFER_NORMAL_MODE,
/* DAC buffer start callback            */  NULL,
/* DAC buffer watermark callback        */  DAC_BufferWattermarkCallBack,
/* DAC buffer end callback              */  DAC_BufferEndCallBack
};


/*******************************************************************************
 * Sine 12bit table with 256 samples.
 * The positive value is not saturated to 4095.
 * Saturation needs to be handled in driver
 ******************************************************************************/

const int_16 sintable[] =
{
        0,   101,   201,   301,   401,   501,   601,   700,
      799,   897,   995,  1092,  1189,  1285,  1380,  1474,
     1567,  1660,  1751,  1842,  1931,  2019,  2106,  2191,
     2276,  2359,  2440,  2520,  2598,  2675,  2751,  2824,
     2896,  2967,  3035,  3102,  3166,  3229,  3290,  3349,
     3406,  3461,  3513,  3564,  3612,  3659,  3703,  3745,
     3784,  3822,  3857,  3889,  3920,  3948,  3973,  3996,
     4017,  4036,  4052,  4065,  4076,  4085,  4091,  4095,
     4096,  4095,  4091,  4085,  4076,  4065,  4052,  4036,
     4017,  3996,  3973,  3948,  3920,  3889,  3857,  3822,
     3784,  3745,  3703,  3659,  3612,  3564,  3513,  3461,
     3406,  3349,  3290,  3229,  3166,  3102,  3035,  2967,
     2896,  2824,  2751,  2675,  2598,  2520,  2440,  2359,
     2276,  2191,  2106,  2019,  1931,  1842,  1751,  1660,
     1567,  1474,  1380,  1285,  1189,  1092,   995,   897,
      799,   700,   601,   501,   401,   301,   201,   101,
        0,  -101,  -201,  -301,  -401,  -501,  -601,  -700,
     -799,  -897,  -995, -1092, -1189, -1285, -1380, -1474,
    -1567, -1660, -1751, -1842, -1931, -2019, -2106, -2191,
    -2276, -2359, -2440, -2520, -2598, -2675, -2751, -2824,
    -2896, -2967, -3035, -3102, -3166, -3229, -3290, -3349,
    -3406, -3461, -3513, -3564, -3612, -3659, -3703, -3745,
    -3784, -3822, -3857, -3889, -3920, -3948, -3973, -3996,
    -4017, -4036, -4052, -4065, -4076, -4085, -4091, -4095,
    -4096, -4095, -4091, -4085, -4076, -4065, -4052, -4036,
    -4017, -3996, -3973, -3948, -3920, -3889, -3857, -3822,
    -3784, -3745, -3703, -3659, -3612, -3564, -3513, -3461,
    -3406, -3349, -3290, -3229, -3166, -3102, -3035, -2967,
    -2896, -2824, -2751, -2675, -2598, -2520, -2440, -2359,
    -2276, -2191, -2106, -2019, -1931, -1842, -1751, -1660,
    -1567, -1474, -1380, -1285, -1189, -1092,  -995,  -897,
     -799,  -700,  -601,  -501,  -401,  -301,  -201,  -101,
};


LDD_TDeviceDataPtr  DAC_DevicePtr;
LDD_TError          DAC_Error;
uint_16             DAC_Watermark = LDD_DAC_BUFFER_WATERMARK_L4;

uint_16_ptr         GEN_BufferNewPtr;
uint_16_ptr         GEN_BufferCurrentPtr;
uint_16_ptr         GEN_BufferPtr;

int_16              GEN_SignalOffsetNew     = GEN_OFFSET;
int_16              GEN_Peak2PeakNew        = GEN_PEAK2PEAK;
int_16              GEN_Peak2PeakStep       = GEN_PEAK2PEAK_STEP;
int_16              GEN_SignalOffsetCurrent;
int_16              GEN_Peak2PeakCurrent;

uint_32             terminal_cursor_position     = 1;

/*TASK**************************************************************************
*
* Function Name   : main_task
*
* Input Params    : initial_data
*
* Returned Value  : None
*
* Comments        :
*
*END***************************************************************************/

void main_task
(
    uint_32 initial_data
)
{

    LWGPIO_STRUCT led1;


    /* Print example functional description on terminal */
    puts("\n");
    puts("\n********************************************************************************");
    puts("\n*");
    puts("\n*   This example demonstrates how to use DAC device driver.");
    puts("\n*");
  printf("\n*   1. It generates sine signal with period = %d us on DACO pin.", GEN_SIGNAL_PERIOD);
    puts("\n*      The signal amplitude sweeps between minimal to maximal value.");
    puts("\n*      It can be observed on A32 pin TWR-ELEV FUNCTIONAL");
    puts("\n*      or TWR-PROTO board by scope.");
    puts("\n*   2. It also toggles by LED1 on board using LWGPIO driver");
    puts("\n*      to see that application is running.");
    puts("\n*");
    puts("\n********************************************************************************");
    puts("\n");

    puts("\nLWGPIO init for LED1...............");
    if (TRUE == lwgpio_init(&led1, BSP_LED1, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE))    {
        /* swich pin functionality (MUX) to GPIO mode */
        lwgpio_set_functionality(&led1, BSP_LED1_MUX_GPIO);
        /* write logical 1 to the pin */
        lwgpio_set_value(&led1, LWGPIO_VALUE_HIGH); /* set pin to 1 */
        puts("OK\n");
    }
    else {
        puts("failed, task is blocked\n");
        _task_block();
    }

    /* Init DAC device */
    puts("\nDAC device initialization..........");
    if (NULL == (DAC_DevicePtr = DAC_Init((const LDD_RTOS_TDeviceDataPtr)&DAC_DeviceData)))
    {
        puts("failed, task is blocked\n");
        _task_block();
    }
    puts("OK");

    puts("\n\n");

    /* Set signal period  */
    GEN_SetSignalPeriod(PDB_PRESCALER, GEN_SCALE_PERIOD(GEN_SIGNAL_PERIOD));

    /* Set buffer watermark */
    DAC_Error = DAC_SetBufferWatermark(DAC_DevicePtr, DAC_Watermark);

    /* Set signal parameters */
    GEN_Peak2PeakCurrent    = GEN_Peak2PeakNew;
    GEN_SignalOffsetCurrent = GEN_SignalOffsetNew;

    /* Create table in RAM memory */
    GEN_BufferCurrentPtr    = GEN_CreateTable( (const int_16_ptr)sintable,
                                                sizeof(sintable),
                                                GEN_Peak2PeakCurrent,
                                                GEN_SignalOffsetCurrent);

    /* Fill internal DAC buffer */
    DAC_Error     = DAC_SetBuffer(DAC_DevicePtr,
                                  GEN_BufferCurrentPtr,
                                  DAC_INTERNAL_BUFFER_SIZE,
                                  0);

    /* Keep current buffer pointer */
    GEN_BufferPtr = GEN_BufferCurrentPtr;
    GEN_BufferPtr += DAC_INTERNAL_BUFFER_SIZE;

    /* Enable DAC device and start signal generation */
    DAC_Error     = DAC_Enable(DAC_DevicePtr);

    while(1)
    {
        /* Suspend task for 125 milliseconds */
        _time_delay(125);

        /* Toggle LED1 on board to see that example is running */
        lwgpio_toggle_value(&led1);

        /* Sweep signal amplitude between minimal and maximal value */
        GEN_Peak2PeakNew +=  GEN_Peak2PeakStep;

        if (GEN_Peak2PeakNew < 0)   {
            GEN_Peak2PeakNew  = 0;
            GEN_Peak2PeakStep = -GEN_Peak2PeakStep;
            puts("\nReached minimal amplitude -> increasing amplitude\n");
            /* reset cursor position */
            terminal_cursor_position = 1;
        }
        else if (GEN_Peak2PeakNew > (GEN_PEAK2PEAK)) {
            GEN_Peak2PeakNew  = GEN_PEAK2PEAK;
            GEN_Peak2PeakStep = -GEN_Peak2PeakStep;
            puts("\nReached maximal amplitude -> decreasing amplitude\n");
            /* reset cursor position */
            terminal_cursor_position = 1;
        }

        /* Change signal amplitude if it differs from current one */
        if (GEN_Peak2PeakNew != GEN_Peak2PeakCurrent)
        {
            GEN_BufferNewPtr = GEN_CreateTable((const int_16_ptr)sintable,
                                                sizeof(sintable),
                                                GEN_Peak2PeakNew,
                                                GEN_SignalOffsetNew);

            GEN_DestroyTable(GEN_BufferCurrentPtr);
            GEN_Peak2PeakCurrent    = GEN_Peak2PeakNew;
            GEN_SignalOffsetCurrent = GEN_SignalOffsetNew;
            GEN_BufferCurrentPtr    = GEN_BufferNewPtr;
        }

        /* Print dot on console to see that application is running */
        if (terminal_cursor_position++ > TERMINAL_CURSOR_POSITION_MAX) {
            terminal_cursor_position = 1;
            puts("\n");
        }
        else {
            puts(".");
        }
    }
}

/*FUNCTION**********************************************************************
*
* Function Name   : DAC_BufferWattermarkCallBack
*
* Input Params    : DeviceData   - Device data structure pointer.
*
* Returned Value  : None
*
* Comments        : The DAC_BufferWattermarkCallBack is called from DAC driver
*                   when internal circular buffer read pointer
*                   reaches the Watermark.
*
*END***************************************************************************/

void DAC_BufferWattermarkCallBack
(
    LDD_RTOS_TDeviceDataPtr DeviceData
)
{
    static vuint_16 DAC_BufferWattermarkCount;

    DAC_BufferWattermarkCount++;

    /* Copy data from buffer start to DAC_Watermark */
    DAC_Error = DAC_SetBuffer(DAC_DevicePtr,
                              GEN_BufferPtr,
                              DAC_INTERNAL_BUFFER_SIZE - (DAC_Watermark + 1),
                              0);

    GEN_BufferPtr += (DAC_INTERNAL_BUFFER_SIZE - (DAC_Watermark + 1));
}

/*FUNCTION**********************************************************************
*
* Function Name   : DAC_BufferEndCallBack
*
* Input Params    : DeviceData   - Device data structure pointer.
*
* Returned Value  : None
*
* Comments        : The DAC_BufferEndCallBack is called from DAC driver
*                   when internal circular buffer read pointer
*                   reaches end of buffer.
*                   The callback is registered in DAC_Init() function.
*
*END***************************************************************************/

void DAC_BufferEndCallBack
(
    LDD_RTOS_TDeviceDataPtr DeviceData
)
{
    static vuint_16 DAC_BufferEndCount;

    /* */
    DAC_BufferEndCount++;

    /* Copy data from watermark to end of buffer */
    DAC_Error = DAC_SetBuffer(DAC_DevicePtr,
                              GEN_BufferPtr,
                              (DAC_Watermark + 1),
                              DAC_INTERNAL_BUFFER_SIZE - (DAC_Watermark + 1));

    GEN_BufferPtr += (DAC_Watermark + 1);

    if (GEN_BufferPtr >= GEN_BufferCurrentPtr + GEN_NUMBER_OF_SAMPLES)
    {
        GEN_BufferPtr = GEN_BufferCurrentPtr;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name   : GEN_CreateTable()
*
* Input Params    : table_ptr   - pointer to source table to be rescaled.
*                   table_size  - the size of source table in bytes.
*                   peak_peak   - peak to peak value
*                   offset      - signal DC offset
*
* Returned Value  : output table pointer
*
* Comments        : Function creates table in RAM and return pointer
*   to table base address. The output data have rescaled by peak to peak value
*   and are shifted by DC offset.
*
*END***************************************************************************/

uint_16_ptr GEN_CreateTable
(
    int_16_ptr  table_ptr,
    uint_16     table_size,
    int_16      peak_peak,
    int_16      offset
)
{
    uint_32         i;
    /* Calculate number of samples in source table */
    uint_32         table_count = table_size / sizeof(uint_16);
    uint_16_ptr     out_table;
    int_32          temp;

    if (NULL == (out_table = _mem_alloc(table_size)))    {
        return NULL;
    }

    for (i = 0; i < table_count; i++)    {

        temp   = table_ptr[i];
        temp  *= peak_peak / 2;
        temp >>= 12;
        temp  += offset;

        /* positive saturation */
        if (temp > 4095) temp = 4095;
        /* negative saturation */
        if (temp < 0)    temp = 0;

        out_table[i] = (uint_16)temp;
    }

    return out_table;
}


/*FUNCTION**********************************************************************
*
* Function Name   : GEN_DestroyTable()
*
* Input Params    : table_ptr   - pointer to table to be destroyed
*
* Returned Value  :
*
* Comments        :
*   Function destroys the table pointed by table_ptr .
*
*END***************************************************************************/

_mqx_uint GEN_DestroyTable
(
    uint_16_ptr table_ptr
)
{
    return _mem_free(table_ptr);
}


/*FUNCTION**********************************************************************
*
* Function Name   : GEN_SetSignalPeriod()
*
* Input Params    : pdb_prescaler   - PDB prescaler
*                   dac_interval    - DAC interval
*
* Returned Value  : IO_OK
*
* Comments        : The function sets the PDB module for DAC triggering.
*
*END***************************************************************************/
#if defined (BSP_TWR_K53N512) ||defined (BSP_TWRMCF51JF) ||defined (BSP_TWRMCF51QM)
_mqx_int GEN_SetSignalPeriod
(
    uint_32 pdb_prescaler,
    uint_32 dac_interval
)
{
    /* Get PDB base pointer */
    PDB_MemMapPtr pdb_ptr = (PDB_MemMapPtr)PDB0_BASE_PTR;

    pdb_ptr->DAC[0].INT =  dac_interval - 1;

    /* Set prescaler */
    pdb_ptr->SC = (pdb_prescaler << PDB_SC_PRESCALER_SHIFT) | PDB_SC_CONT_MASK;
    /* Software trigger select */
    pdb_ptr->SC |= PDB_SC_TRGSEL(0x0F);
    /* Enable DAC trigger output */
    pdb_ptr->DAC[0].INTC = PDB_INTC_TOE_MASK;
    pdb_ptr->SC |= PDB_SC_LDOK_MASK;
    /* Enable PDB module */
    pdb_ptr->SC |= PDB_SC_PDBEN_MASK;
    /* Trigger */
    pdb_ptr->SC |= PDB_SC_SWTRIG_MASK;

    return IO_OK;
}
#else


#if   PSP_MQX_CPU_IS_MCF51JE
    #define PDB_STRUCT_PTR              MCF51JE_PDB_STRUCT_PTR
    #define DEVICE_STRUCT_PTR           VMCF51JE_STRUCT_PTR
    #define PDB_PDBC1_PRESCALER_BITNUM  MCF51JE_PDB_PDBC1_PRESCALER_BITNUM
    #define PDB_PDBC1_CONT              MCF51JE_PDB_PDBC1_CONT
    #define PDB_PDBC1_TRIGSEL_SWTRIG    MCF51JE_PDB_PDBC1_TRIGSEL_SWTRIG
    #define PDB_PDBSC_DACTOE            MCF51JE_PDB_PDBSC_DACTOE
    #define PDB_PDBSC_LDOK              MCF51JE_PDB_PDBSC_LDOK
    #define PDB_PDBSC_PDBEN             MCF51JE_PDB_PDBSC_PDBEN
    #define PDB_PDBC2_SWTRIG            MCF51JE_PDB_PDBC2_SWTRIG
#elif PSP_MQX_CPU_IS_MCF51MM
    #define PDB_STRUCT_PTR              MCF51MM_PDB_STRUCT_PTR
    #define DEVICE_STRUCT_PTR           VMCF51MM_STRUCT_PTR
    #define PDB_PDBC1_PRESCALER_BITNUM  MCF51MM_PDB_PDBC1_PRESCALER_BITNUM
    #define PDB_PDBC1_CONT              MCF51MM_PDB_PDBC1_CONT
    #define PDB_PDBC1_TRIGSEL_SWTRIG    MCF51MM_PDB_PDBC1_TRIGSEL_SWTRIG
    #define PDB_PDBSC_DACTOE            MCF51MM_PDB_PDBSC_DACTOE
    #define PDB_PDBSC_LDOK              MCF51MM_PDB_PDBSC_LDOK
    #define PDB_PDBSC_PDBEN             MCF51MM_PDB_PDBSC_PDBEN
    #define PDB_PDBC2_SWTRIG            MCF51MM_PDB_PDBC2_SWTRIG
#endif

_mqx_int GEN_SetSignalPeriod
(
    uint_32 pdb_prescaler,
    uint_32 dac_interval
)
{
    PDB_STRUCT_PTR  pdb_ptr;

    /* Get PDB base pointer */
    pdb_ptr = (PDB_STRUCT_PTR)&((DEVICE_STRUCT_PTR)BSP_IPSBAR)->PDB;

    pdb_ptr->DACINT =  dac_interval - 1;

    /* Set prescaler */
    pdb_ptr->PDBC1 = (   (pdb_prescaler << PDB_PDBC1_PRESCALER_BITNUM) | (PDB_PDBC1_CONT));

    /* Software trigger select */
    pdb_ptr->PDBC1 |= PDB_PDBC1_TRIGSEL_SWTRIG;
    /* Enable DAC trigger output */
    pdb_ptr->PDBSC |= PDB_PDBSC_DACTOE | PDB_PDBSC_LDOK;
    /* Enable PDB module */
    pdb_ptr->PDBSC |= PDB_PDBSC_PDBEN;

    /* Trigger */
    pdb_ptr->PDBC2 |= PDB_PDBC2_SWTRIG;

    return IO_OK;
}
#endif

/* EOF */
