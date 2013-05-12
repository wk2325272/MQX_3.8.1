#ifndef __security_private_h__
#define __security_private_h__

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
* $FileName: Security_private.h$
* $Version : 3.8.B.1$
* $Date    : Jun-22-2012$
*
* Comments:
*
*
*
*END************************************************************************/

#define MAX_QUEUE            10

/* Defines for ADC channels */
#define ACCX    0
#define ACCY    1
#define ACCZ    2
#define ADC_POT     0
#define ADC_ACCX    1
#define ADC_ACCY    2
#define ADC_ACCZ    3

#if (defined(BSP_TWR_K70F120M) || defined(BSP_TWR_K60N512) ||  defined(BSP_TWR_K53N512) || defined(BSP_TWR_K60F120M)) || defined(BSP_TWR_K60D100M)
    #if BSPCFG_ENABLE_ADC1
        #define ENABLE_ADC BSPCFG_ENABLE_ADC1
        #define MY_ADC "adc1:" /* must be #1 as the inputs are wired to ADC 1 */
        #define MY_TRIGGER ADC_PDB_TRIGGER
    #endif /* BSPCFG_ENABLE_ADCx */

#else /* defined BSP_xxx */
    #if BSPCFG_ENABLE_ADC
        #define ENABLE_ADC BSPCFG_ENABLE_ADC
        #define MY_ADC "adc:"
        #define MY_TRIGGER ADC_TRIGGER_1
    #endif /* BSPCFG_ENABLE_ADCx */
#endif /* defined BSP_xxx */

typedef struct {
   char_ptr       eventptr;
   TIME_STRUCT    time;
} SEC_HIST, * SEC_HIST_PTR;

typedef struct {
   uint_32     DoorStatus;
   uint_32     WindowStatus;
   uint_32     WaterLevel;
   _mqx_int    MovementStatus;
   _mqx_int    GarageStatus;
   _mqx_int    last[3];
   _mqx_int    flat;
   _mqx_int    HistoryIndex;
   SEC_HIST    History[MAX_QUEUE];
} SEC_PARAMS, * SEC_PARAMS_PTR;

 typedef enum {
   SEC_LED1_OUTPUT=0,
   SEC_LED2_OUTPUT,
   SEC_LED3_OUTPUT,
   SEC_OPEN_OUTPUT,
   SEC_MAX_OUTPUTS
} SEC_Output_t;

typedef enum {
   SEC_DOOR_INPUT=0,
   SEC_WINDOW_INPUT,
   SEC_MAX_INPUTS
} SEC_Input_t;


typedef enum {
   SYSTEM_STARTED = 0,
   MOTION_STARTED,
   MOTION_STOPPED,
   DOOR_OPENED,
   DOOR_CLOSED,
   WINDOW_OPENED,
   WINDOW_CLOSED
}SEC_Event_t;


extern SEC_PARAMS SEC_Params;

extern void SEC_InitializeADC(void);
extern void SEC_InitializeParameters(void);
extern boolean SEC_InitializeIO(void);

extern void SEC_ResetOutputs(void);
extern void SEC_SetOutput(SEC_Output_t,boolean);
extern void SEC_ToggleOutput(SEC_Output_t);
extern boolean SEC_GetInput(SEC_Input_t);
extern void SEC_AddLog(SEC_Event_t event);
extern boolean SEC_GetOutput(SEC_Output_t);
extern char_ptr SEC_GetOutputName(SEC_Output_t);
extern void Input_Poll(void);


extern _mqx_int ReadADC(_mqx_int channel);

#endif
