/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SystemSetupData.c
*
* @brief  Defines the structs and information needed for the configuration and decoders usage 
*
* @version 1.0.60.0
* 
* @date Feb-6-2012
* 
* All the declarations of structures and information are bassed in the values
* provided by the user in the SystemSetup.h file                             
*
***********************************************************************************************************************/


#include "TSS_SystemSetup.h"
#include "TSS_API.h"
#include "TSS_SystemSetupVal.h"
#include "derivative.h" 

/* Prototypes */

UINT16 TSS_IIRFilterEquation(UINT16 u16CurrentCapSample, UINT8 u8ElecNum);
UINT16 TSS_ShieldEquation(UINT16 u16CurrentCapSample, UINT8 u8ElecNum); 

/* The section below declares all the structures needed for each decoder */

#if TSS_N_CONTROLS > 0
    #if (TSS_C0_TYPE == TSS_CT_KEYPAD)                  /* Validates the Controler Type */
        UINT8 TSS_C0_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE]; /* Declares Events Buffer for Keypad */
        TSS_CSKeypad TSS_C0_STRUCTURE = { {0u, TSS_C0_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C0_KEYPAD_EVENTS_BUFFER}; /* Declares the control KeyPad Structure */
        TSS_KeypadContext TSS_C0_STRUCTURE_CONTEXT;     /* Declares the context KeyPad Structure */
        #define C0_BUFFMASK_BIT 0u                      /* Defines 1 bit to determinate if is a KeyPad or Slider/Rotary*/
        #define C0_DECODER      TSS_KeypadDecoder       /* Defines a reference constant for the Decodet type function */
    #elif (TSS_C0_TYPE == TSS_CT_SLIDER)                /* Validates the Controler Type */
        TSS_CSSlider TSS_C0_STRUCTURE = { {0u, TSS_C0_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u}; /* Declares the control Slider Structure */
        TSS_SingleContext TSS_C0_STRUCTURE_CONTEXT;     /* Declares the context Slider Structure */
        #define C0_BUFFMASK_BIT 1u                      /* Defines 1 bit to determinate if is a KeyPad or Slider/Rotary */
        #define C0_DECODER      TSS_SliderDecoder       /* Defines a reference constant for the Decodet type functions */       
    #else /* TSS_CT_ROTARY */                           /* Validates the Controler Type */
        TSS_CSRotary TSS_C0_STRUCTURE = { {0u, TSS_C0_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u}; /* Declares the control Rotary Structure */
        TSS_SingleContext TSS_C0_STRUCTURE_CONTEXT;     /* Declares the context Rotary Structure */
        #define C0_BUFFMASK_BIT 1u                      /* Defines 1 bit to determinate if is a KeyPad or Slider/Rotary */
        #define C0_DECODER      TSS_SliderDecoder       /* Defines a reference constant for the Decodet type functions */
    #endif
    #define C0_BUFFMASK (UINT8)(C0_BUFFMASK_BIT << 0u)  /* Assign each CO_MASK_BIT a diferent position depending on the number of control*/ 
    void TSS_C0_CALLBACK(TSS_CONTROL_ID u8ControlId);            /* Declares the callback fucntion */
    UINT8 C0_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command); /* Declares the decoder type function */
#endif

/* Depending on the number of controls declared by the user the strcutures are created */
#if TSS_N_CONTROLS > 1
    #if (TSS_C1_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C1_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C1_STRUCTURE = { {1u, TSS_C1_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C1_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C1_STRUCTURE_CONTEXT;
        #define C1_BUFFMASK_BIT 0u
        #define C1_DECODER      TSS_KeypadDecoder
    #elif (TSS_C1_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C1_STRUCTURE = { {1u, TSS_C1_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C1_STRUCTURE_CONTEXT;
        #define C1_BUFFMASK_BIT 1u
        #define C1_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C1_STRUCTURE = { {1u, TSS_C1_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C1_STRUCTURE_CONTEXT;
        #define C1_BUFFMASK_BIT 1u
        #define C1_DECODER      TSS_SliderDecoder
    #endif
    #define C1_BUFFMASK (UINT8)(C1_BUFFMASK_BIT << 1u)
    void TSS_C1_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C1_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 2
    #if (TSS_C2_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C2_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C2_STRUCTURE = { {2u, TSS_C2_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C2_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C2_STRUCTURE_CONTEXT;
        #define C2_BUFFMASK_BIT 0u
        #define C2_DECODER      TSS_KeypadDecoder
    #elif (TSS_C2_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C2_STRUCTURE = { {2u, TSS_C2_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C2_STRUCTURE_CONTEXT;
        #define C2_BUFFMASK_BIT 1u
        #define C2_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C2_STRUCTURE = { {2u, TSS_C2_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C2_STRUCTURE_CONTEXT;
        #define C2_BUFFMASK_BIT 1u
        #define C2_DECODER      TSS_SliderDecoder
    #endif
    #define C2_BUFFMASK (UINT8)(C2_BUFFMASK_BIT << 2u)
    void TSS_C2_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C2_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 3
    #if (TSS_C3_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C3_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C3_STRUCTURE = { {3u, TSS_C3_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C3_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C3_STRUCTURE_CONTEXT;
        #define C3_BUFFMASK_BIT 0u
        #define C3_DECODER      TSS_KeypadDecoder
    #elif (TSS_C3_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C3_STRUCTURE = { {3u, TSS_C3_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C3_STRUCTURE_CONTEXT;
        #define C3_BUFFMASK_BIT 1u
        #define C3_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C3_STRUCTURE = { {3u, TSS_C3_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C3_STRUCTURE_CONTEXT;
        #define C3_BUFFMASK_BIT 1u
        #define C3_DECODER      TSS_SliderDecoder
    #endif
    #define C3_BUFFMASK (UINT8)(C3_BUFFMASK_BIT << 3u)
    void TSS_C3_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C3_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 4
    #if (TSS_C4_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C4_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C4_STRUCTURE = { {4u, TSS_C4_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C4_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C4_STRUCTURE_CONTEXT;
        #define C4_BUFFMASK_BIT 0u
        #define C4_DECODER      TSS_KeypadDecoder
    #elif (TSS_C4_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C4_STRUCTURE = { {4u, TSS_C4_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C4_STRUCTURE_CONTEXT;
        #define C4_BUFFMASK_BIT 1u
        #define C4_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C4_STRUCTURE = { {4u, TSS_C4_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C4_STRUCTURE_CONTEXT;
        #define C4_BUFFMASK_BIT 1u
        #define C4_DECODER      TSS_SliderDecoder
    #endif
    #define C4_BUFFMASK (UINT8)(C4_BUFFMASK_BIT << 4u)
    void TSS_C4_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C4_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 5
    #if (TSS_C5_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C5_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C5_STRUCTURE = { {5u, TSS_C5_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C5_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C5_STRUCTURE_CONTEXT;
        #define C5_BUFFMASK_BIT 0u
        #define C5_DECODER      TSS_KeypadDecoder
    #elif (TSS_C5_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C5_STRUCTURE = { {5u, TSS_C5_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C5_STRUCTURE_CONTEXT;
        #define C5_BUFFMASK_BIT 1u
        #define C5_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C5_STRUCTURE = { {5u, TSS_C5_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C5_STRUCTURE_CONTEXT;
        #define C5_BUFFMASK_BIT 1u
        #define C5_DECODER      TSS_SliderDecoder
    #endif
    #define C5_BUFFMASK (UINT8)(C5_BUFFMASK_BIT << 5u)
    void TSS_C5_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C5_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 6
    #if (TSS_C6_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C6_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C6_STRUCTURE = { {6u, TSS_C6_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C6_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C6_STRUCTURE_CONTEXT;
        #define C6_BUFFMASK_BIT 0u
        #define C6_DECODER      TSS_KeypadDecoder
    #elif (TSS_C6_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C6_STRUCTURE = { {6u, TSS_C6_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C6_STRUCTURE_CONTEXT;
        #define C6_BUFFMASK_BIT 1u
        #define C6_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C6_STRUCTURE = { {6u, TSS_C6_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C6_STRUCTURE_CONTEXT;
        #define C6_BUFFMASK_BIT 1u
        #define C6_DECODER      TSS_SliderDecoder
    #endif
    #define C6_BUFFMASK (UINT8)(C6_BUFFMASK_BIT << 6u)
    void TSS_C6_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C6_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 7
    #if (TSS_C7_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C7_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C7_STRUCTURE = { {7u, TSS_C7_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C7_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C7_STRUCTURE_CONTEXT;
        #define C7_BUFFMASK_BIT 0u
        #define C7_DECODER      TSS_KeypadDecoder
    #elif (TSS_C7_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C7_STRUCTURE = { {7u, TSS_C7_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C7_STRUCTURE_CONTEXT;
        #define C7_BUFFMASK_BIT 1u
        #define C7_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C7_STRUCTURE = { {7u, TSS_C7_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C7_STRUCTURE_CONTEXT;
        #define C7_BUFFMASK_BIT 1u
        #define C7_DECODER      TSS_SliderDecoder
    #endif
    #define C7_BUFFMASK (UINT8)(C7_BUFFMASK_BIT << 7u)
    void TSS_C7_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C7_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 8
    #if (TSS_C8_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C8_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C8_STRUCTURE = { {8u, TSS_C8_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C8_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C8_STRUCTURE_CONTEXT;
        #define C8_BUFFMASK_BIT 0u
        #define C8_DECODER      TSS_KeypadDecoder
    #elif (TSS_C8_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C8_STRUCTURE = { {8u, TSS_C8_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C8_STRUCTURE_CONTEXT;
        #define C8_BUFFMASK_BIT 1u
        #define C8_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C8_STRUCTURE = { {8u, TSS_C8_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C8_STRUCTURE_CONTEXT;
        #define C8_BUFFMASK_BIT 1u
        #define C8_DECODER      TSS_SliderDecoder
    #endif
    #define C8_BUFFMASK (UINT8)(C8_BUFFMASK_BIT << 0u)
    void TSS_C8_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C8_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 9
    #if (TSS_C9_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C9_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C9_STRUCTURE = { {9u, TSS_C9_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C9_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C9_STRUCTURE_CONTEXT;
        #define C9_BUFFMASK_BIT 0u
        #define C9_DECODER      TSS_KeypadDecoder
    #elif (TSS_C9_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C9_STRUCTURE = { {9u, TSS_C9_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C9_STRUCTURE_CONTEXT;
        #define C9_BUFFMASK_BIT 1u
        #define C9_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C9_STRUCTURE = { {9u, TSS_C9_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C9_STRUCTURE_CONTEXT;
        #define C9_BUFFMASK_BIT 1u
        #define C9_DECODER      TSS_SliderDecoder
    #endif
    #define C9_BUFFMASK (UINT8)(C9_BUFFMASK_BIT << 1u)
    void TSS_C9_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C9_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 10
    #if (TSS_C10_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C10_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C10_STRUCTURE = { {10u, TSS_C10_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C10_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C10_STRUCTURE_CONTEXT;
        #define C10_BUFFMASK_BIT 0u
        #define C10_DECODER      TSS_KeypadDecoder
    #elif (TSS_C10_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C10_STRUCTURE = { {10u, TSS_C10_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C10_STRUCTURE_CONTEXT;
        #define C10_BUFFMASK_BIT 1u
        #define C10_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C10_STRUCTURE = { {10u, TSS_C10_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C10_STRUCTURE_CONTEXT;
        #define C10_BUFFMASK_BIT 1u
        #define C10_DECODER      TSS_SliderDecoder
    #endif
    #define C10_BUFFMASK (UINT8)(C10_BUFFMASK_BIT << 2u)
    void TSS_C10_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C10_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 11
    #if (TSS_C11_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C11_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C11_STRUCTURE = { {11u, TSS_C11_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C11_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C11_STRUCTURE_CONTEXT;
        #define C11_BUFFMASK_BIT 0u
        #define C11_DECODER      TSS_KeypadDecoder
    #elif (TSS_C11_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C11_STRUCTURE = { {11u, TSS_C11_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C11_STRUCTURE_CONTEXT;
        #define C11_BUFFMASK_BIT 1u
        #define C11_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C11_STRUCTURE = { {11u, TSS_C11_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C11_STRUCTURE_CONTEXT;
        #define C11_BUFFMASK_BIT 1u
        #define C11_DECODER      TSS_SliderDecoder
    #endif
    #define C11_BUFFMASK (UINT8)(C11_BUFFMASK_BIT << 3u)
    void TSS_C11_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C11_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 12
    #if (TSS_C12_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C12_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C12_STRUCTURE = { {12u, TSS_C12_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C12_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C12_STRUCTURE_CONTEXT;
        #define C12_BUFFMASK_BIT 0u
        #define C12_DECODER      TSS_KeypadDecoder
    #elif (TSS_C12_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C12_STRUCTURE = { {12u, TSS_C12_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C12_STRUCTURE_CONTEXT;
        #define C12_BUFFMASK_BIT 1u
        #define C12_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C12_STRUCTURE = { {12u, TSS_C12_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C12_STRUCTURE_CONTEXT;
        #define C12_BUFFMASK_BIT 1u
        #define C12_DECODER      TSS_SliderDecoder
    #endif
    #define C12_BUFFMASK (UINT8)(C12_BUFFMASK_BIT << 4u)
    void TSS_C12_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C12_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 13
    #if (TSS_C13_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C13_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C13_STRUCTURE = { {13u, TSS_C13_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C13_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C13_STRUCTURE_CONTEXT;
        #define C13_BUFFMASK_BIT 0u
        #define C13_DECODER      TSS_KeypadDecoder
    #elif (TSS_C13_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C13_STRUCTURE = { {13u, TSS_C13_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C13_STRUCTURE_CONTEXT;
        #define C13_BUFFMASK_BIT 1u
        #define C13_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C13_STRUCTURE = { {13u, TSS_C13_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C13_STRUCTURE_CONTEXT;
        #define C13_BUFFMASK_BIT 1u
        #define C13_DECODER      TSS_SliderDecoder
    #endif
    #define C13_BUFFMASK (UINT8)(C13_BUFFMASK_BIT << 5u)
    void TSS_C13_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C13_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 14
    #if (TSS_C14_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C14_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C14_STRUCTURE = { {14u, TSS_C14_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C14_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C14_STRUCTURE_CONTEXT;
        #define C14_BUFFMASK_BIT 0u
        #define C14_DECODER      TSS_KeypadDecoder
    #elif (TSS_C14_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C14_STRUCTURE = { {14u, TSS_C14_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C14_STRUCTURE_CONTEXT;
        #define C14_BUFFMASK_BIT 1u
        #define C14_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C14_STRUCTURE = { {14u, TSS_C14_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C14_STRUCTURE_CONTEXT;
        #define C14_BUFFMASK_BIT 1u
        #define C14_DECODER      TSS_SliderDecoder
    #endif
    #define C14_BUFFMASK (UINT8)(C14_BUFFMASK_BIT << 6u)
    void TSS_C14_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C14_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif

#if TSS_N_CONTROLS > 15
    #if (TSS_C15_TYPE == TSS_CT_KEYPAD)
        UINT8 TSS_C15_KEYPAD_EVENTS_BUFFER[TSS_KEYPAD_EVENTS_BUFFER_SIZE];
        TSS_CSKeypad TSS_C15_STRUCTURE = { {15u, TSS_C15_TYPE}, TSS_KEYPAD_CONTCONF_INIT, 0u,0u, TSS_KEYPAD_EVENTS_INIT, 0u,0u,0u, TSS_C15_KEYPAD_EVENTS_BUFFER};
        TSS_KeypadContext TSS_C15_STRUCTURE_CONTEXT;
        #define C15_BUFFMASK_BIT 0u
        #define C15_DECODER      TSS_KeypadDecoder
    #elif (TSS_C15_TYPE == TSS_CT_SLIDER)
        TSS_CSSlider TSS_C15_STRUCTURE = { {15u, TSS_C15_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C15_STRUCTURE_CONTEXT;
        #define C15_BUFFMASK_BIT 1u
        #define C15_DECODER      TSS_SliderDecoder
    #else /* TSS_CT_ROTARY */
        TSS_CSRotary TSS_C15_STRUCTURE = { {15u, TSS_C15_TYPE}, TSS_SLIDER_CONTROL_INIT, TSS_SLIDER_DYN_INIT, TSS_SLIDER_STAT_INIT, TSS_SLIDER_EVENTS_INIT, 0u,0u};
        TSS_SingleContext TSS_C15_STRUCTURE_CONTEXT;
        #define C15_BUFFMASK_BIT 1u
        #define C15_DECODER      TSS_SliderDecoder
    #endif
    #define C15_BUFFMASK (UINT8)(C15_BUFFMASK_BIT << 7u)
    void TSS_C15_CALLBACK(TSS_CONTROL_ID u8ControlId);
    UINT8 C15_DECODER(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command);
#endif


/*
 *  The section below defines all the CX_BUFFMASK elements 
 *  that have not been defined with "0"
 */

#ifndef C0_BUFFMASK
    #define C0_BUFFMASK 0u
#endif
#ifndef C1_BUFFMASK
    #define C1_BUFFMASK 0u
#endif
#ifndef C2_BUFFMASK
    #define C2_BUFFMASK 0u
#endif
#ifndef C3_BUFFMASK
    #define C3_BUFFMASK 0u
#endif
#ifndef C4_BUFFMASK
    #define C4_BUFFMASK 0u
#endif
#ifndef C5_BUFFMASK
    #define C5_BUFFMASK 0u
#endif
#ifndef C6_BUFFMASK
    #define C6_BUFFMASK 0u
#endif
#ifndef C7_BUFFMASK
    #define C7_BUFFMASK 0u
#endif
/* 8 decoders can only be represented by a Byte, if more decoders are needed another Byte must be used */
#if TSS_N_CONTROLS > 7   
    #ifndef C8_BUFFMASK                  
        #define C8_BUFFMASK 0u
    #endif
    #ifndef C9_BUFFMASK
        #define C9_BUFFMASK 0u
    #endif
    #ifndef C10_BUFFMASK
        #define C10_BUFFMASK 0u
    #endif
    #ifndef C11_BUFFMASK
        #define C11_BUFFMASK 0u
    #endif
    #ifndef C12_BUFFMASK
        #define C12_BUFFMASK 0u
    #endif
    #ifndef C13_BUFFMASK
        #define C13_BUFFMASK 0u
    #endif
    #ifndef C14_BUFFMASK
        #define C14_BUFFMASK 0u
    #endif
    #ifndef C15_BUFFMASK
        #define C15_BUFFMASK 0u
    #endif
#endif

/*
 * The section below defines an array of pointers to 
 * all the callback functions defined by the user for each decoder
 * 
 */

#if TSS_N_CONTROLS != 0
/* Pointer to Callback functions */
void (* const tss_faCallback[TSS_N_CONTROLS])(TSS_CONTROL_ID u8ControlId) = 
{
    TSS_C0_CALLBACK,
    #if TSS_N_CONTROLS > 1
        TSS_C1_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 2
        TSS_C2_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 3
        TSS_C3_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 4
        TSS_C4_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 5
        TSS_C5_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 6
        TSS_C6_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 7
        TSS_C7_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 8
        TSS_C8_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 9
        TSS_C9_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 10
        TSS_C10_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 11
        TSS_C11_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 12
        TSS_C12_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 13
        TSS_C13_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 14
        TSS_C14_CALLBACK,
    #endif
    #if TSS_N_CONTROLS > 15
        TSS_C15_CALLBACK,
    #endif
};
#else          /* If there are no defined controlers a single element array es created */
    void (* const tss_faCallback[1])(TSS_CONTROL_ID u8ControlId);  
#endif


/*
 * The section below declares an array of pointers with the
 * direction of all the decoders controls context structures
 * created in the sections above
 */

/* Array of pointers to decoders controls context data */ 
#if TSS_N_CONTROLS != 0
UINT8 * const tss_acpsDecContext[TSS_N_CONTROLS] = {
    (UINT8 *)&TSS_C0_STRUCTURE_CONTEXT,
    #if TSS_N_CONTROLS > 1
        (UINT8 *)&TSS_C1_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 2
        (UINT8 *)&TSS_C2_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 3
        (UINT8 *)&TSS_C3_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 4
        (UINT8 *)&TSS_C4_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 5
        (UINT8 *)&TSS_C5_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 6
        (UINT8 *)&TSS_C6_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 7
        (UINT8 *)&TSS_C7_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 8
        (UINT8 *)&TSS_C8_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 9
        (UINT8 *)&TSS_C9_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 10
        (UINT8 *)&TSS_C10_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 11
        (UINT8 *)&TSS_C11_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 12
        (UINT8 *)&TSS_C12_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 13
        (UINT8 *)&TSS_C13_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 14
        (UINT8 *)&TSS_C14_STRUCTURE_CONTEXT,
    #endif
    #if TSS_N_CONTROLS > 15
        (UINT8 *)&TSS_C15_STRUCTURE_CONTEXT,
    #endif
};
#else        /* If there are no defined controlers a single element array es created */
    UINT8 * const tss_acpsDecContext[1];
#endif

/*
 * The section below declares an array of pointers with the
 * direction of all the decoders controls system structures
 * created in the sections above
 */

#if TSS_N_CONTROLS != 0
/* Array of pointers to controls structure data */ 
const UINT8 * const tss_acpsCSStructs[TSS_N_CONTROLS] = {
    (UINT8 *)&TSS_C0_STRUCTURE,
    #if TSS_N_CONTROLS > 1
        (UINT8 *)&TSS_C1_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 2
        (UINT8 *)&TSS_C2_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 3
        (UINT8 *)&TSS_C3_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 4
        (UINT8 *)&TSS_C4_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 5
        (UINT8 *)&TSS_C5_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 6
        (UINT8 *)&TSS_C6_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 7
        (UINT8 *)&TSS_C7_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 8
        (UINT8 *)&TSS_C8_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 9
        (UINT8 *)&TSS_C9_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 10
        (UINT8 *)&TSS_C10_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 11
        (UINT8 *)&TSS_C11_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 12
        (UINT8 *)&TSS_C12_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 13
        (UINT8 *)&TSS_C13_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 14
        (UINT8 *)&TSS_C14_STRUCTURE,
    #endif
    #if TSS_N_CONTROLS > 15
        (UINT8 *)&TSS_C15_STRUCTURE,
    #endif
};
#else          /* If there are no defined controlers a single element array es created */
    const UINT8 * const tss_acpsCSStructs[1] = 
    {
        NULL
    };
#endif

/*
 * The section below determines the number of elements assigned to the defined decoders.
 * This provides information to the decoders allowing to reserve the required memory space
 *  
 */

#ifdef TSS_C0_ELECTRODES
    #if TSS_N_CONTROLS == 0
        #define N_BUFF_ELEMENTS 0u   /* If there are no controls N_BUFF_ELEMENTS is 0 */
    #endif
    #if TSS_N_CONTROLS == 1
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 2
        #define N_BUFF_ELEMENTS ((UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES)
    #endif
    #if TSS_N_CONTROLS == 3
        #define N_BUFF_ELEMENTS ((UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES)
    #endif
    #if TSS_N_CONTROLS == 4
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 5
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 6
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 7
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 8
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 9
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 10
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 11
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 12
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 13
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES
        #endif
    #if TSS_N_CONTROLS == 14
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES + (UINT8)TSS_C13_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 15
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES + (UINT8)TSS_C13_ELECTRODES + (UINT8)TSS_C14_ELECTRODES
    #endif
    #if TSS_N_CONTROLS == 16
        #define N_BUFF_ELEMENTS (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES + (UINT8)TSS_C13_ELECTRODES + (UINT8)TSS_C14_ELECTRODES + (UINT8)TSS_C15_ELECTRODES
    #endif
#else
    #define N_BUFF_ELEMENTS 0u   /* If there are no elements assigned to any control N_BUFF_ELEMENTS is 0 */
#endif


/*
*
* The section below declares an array where strores the
* number of electrodes per decoder
*/


#if TSS_N_CONTROLS != 0
/* Sumatory of electrodes arranged by control */
const UINT8 tss_cau8CntrlElecBound[TSS_N_CONTROLS] = {
    (UINT8)TSS_C0_ELECTRODES,
    #if TSS_N_CONTROLS > 1
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 2
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 3
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 4
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 5
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 6
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 7
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 8
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 9
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 10
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 11
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 12
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 13
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES + (UINT8)TSS_C13_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 14
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES + (UINT8)TSS_C13_ELECTRODES + (UINT8)TSS_C14_ELECTRODES,
    #endif
    #if TSS_N_CONTROLS > 15
        (UINT8)TSS_C0_ELECTRODES + (UINT8)TSS_C1_ELECTRODES + (UINT8)TSS_C2_ELECTRODES + (UINT8)TSS_C3_ELECTRODES + (UINT8)TSS_C4_ELECTRODES + (UINT8)TSS_C5_ELECTRODES + (UINT8)TSS_C6_ELECTRODES + (UINT8)TSS_C7_ELECTRODES + (UINT8)TSS_C8_ELECTRODES + (UINT8)TSS_C9_ELECTRODES + (UINT8)TSS_C10_ELECTRODES + (UINT8)TSS_C11_ELECTRODES + (UINT8)TSS_C12_ELECTRODES + (UINT8)TSS_C13_ELECTRODES + (UINT8)TSS_C14_ELECTRODES + (UINT8)TSS_C15_ELECTRODES,
    #endif
};
#else
    const UINT8 tss_cau8CntrlElecBound[1] =
    {
        (TSS_N_ELECTRODES - 1)
    };
#endif

/*
 * The section below declares an array where all the 
 * CX_BUFFER_MASK are joint into a two elemments array
 */

#if TSS_N_CONTROLS != 0
    const UINT8 tss_cau8BuffMask[(((TSS_N_CONTROLS - 1) / 8) + 1)] = 
    {
        C0_BUFFMASK | C1_BUFFMASK | C2_BUFFMASK | C3_BUFFMASK | C4_BUFFMASK | C5_BUFFMASK | C6_BUFFMASK | C7_BUFFMASK,
        #if TSS_N_CONTROLS > 8
            C8_BUFFMASK | C9_BUFFMASK | C10_BUFFMASK | C11_BUFFMASK | C12_BUFFMASK | C13_BUFFMASK | C14_BUFFMASK | C15_BUFFMASK,
        #endif
    };
#else       /* If there are no defined controlers a single element array es created */
    const UINT8 tss_cau8BuffMask[1] = 
    {
        0x00u
    };
#endif


/*
 * The section below defines an array of pointers to 
 * all the decoders functions defined above
 * 
 */

#if TSS_N_CONTROLS != 0
/* Pointer to Decoders functions */
UINT8 (* const tss_faDecoders[TSS_N_CONTROLS])(UINT8 u8CtrlNum, const UINT8 *pu8Buffer, UINT8 u8Command) = 
{
    C0_DECODER,
    #if TSS_N_CONTROLS > 1
        C1_DECODER,
    #endif
    #if TSS_N_CONTROLS > 2
        C2_DECODER,
    #endif
    #if TSS_N_CONTROLS > 3
        C3_DECODER,
    #endif
    #if TSS_N_CONTROLS > 4
        C4_DECODER,
    #endif
    #if TSS_N_CONTROLS > 5
        C5_DECODER,
    #endif
    #if TSS_N_CONTROLS > 6
        C6_DECODER,
    #endif
    #if TSS_N_CONTROLS > 7
        C7_DECODER,
    #endif
    #if TSS_N_CONTROLS > 8
        C8_DECODER,
    #endif
    #if TSS_N_CONTROLS > 9
        C9_DECODER,
    #endif
    #if TSS_N_CONTROLS > 10
        C10_DECODER,
    #endif
    #if TSS_N_CONTROLS > 11
        C11_DECODER,
    #endif
    #if TSS_N_CONTROLS > 12
        C12_DECODER,
    #endif
    #if TSS_N_CONTROLS > 13
        C13_DECODER,
    #endif
    #if TSS_N_CONTROLS > 14
        C14_DECODER,
    #endif
    #if TSS_N_CONTROLS > 15
        C15_DECODER,
    #endif
};
#else          /* If there are no defined controlers a single element array es created */
    UINT8 (* const tss_faDecoders[1])(UINT8 u8CtrlNum, UINT8 *pu8Buffer, UINT8 u8Command);
#endif

/*
 * Initializes the Shield Config array
 */
#define TSS_SHIELD_NOT_USED         0xFF    

#if TSS_USE_SIGNAL_SHIELDING
/* Array of Shield Config values */ 
const UINT8 tss_cau8WShieldPairs[TSS_N_ELECTRODES] = {
    #if TSS_N_ELECTRODES > 0
      #ifdef TSS_E0_SHIELD_ELECTRODE
        TSS_E0_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 1
      #ifdef TSS_E1_SHIELD_ELECTRODE
        TSS_E1_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 2
      #ifdef TSS_E2_SHIELD_ELECTRODE
        TSS_E2_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 3
      #ifdef TSS_E3_SHIELD_ELECTRODE
        TSS_E3_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 4
      #ifdef TSS_E4_SHIELD_ELECTRODE
        TSS_E4_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 5
      #ifdef TSS_E5_SHIELD_ELECTRODE
        TSS_E5_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 6
      #ifdef TSS_E6_SHIELD_ELECTRODE
        TSS_E6_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 7
      #ifdef TSS_E7_SHIELD_ELECTRODE
        TSS_E7_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 8
      #ifdef TSS_E8_SHIELD_ELECTRODE
        TSS_E8_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 9
      #ifdef TSS_E9_SHIELD_ELECTRODE
        TSS_E9_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 10
      #ifdef TSS_E10_SHIELD_ELECTRODE
        TSS_E10_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 11
      #ifdef TSS_E11_SHIELD_ELECTRODE
        TSS_E11_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 12
      #ifdef TSS_E12_SHIELD_ELECTRODE
        TSS_E12_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 13
      #ifdef TSS_E13_SHIELD_ELECTRODE
        TSS_E13_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 14
      #ifdef TSS_E14_SHIELD_ELECTRODE
        TSS_E14_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 15
      #ifdef TSS_E15_SHIELD_ELECTRODE
        TSS_E15_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 16
      #ifdef TSS_E16_SHIELD_ELECTRODE
        TSS_E16_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 17
      #ifdef TSS_E17_SHIELD_ELECTRODE
        TSS_E17_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 18
      #ifdef TSS_E18_SHIELD_ELECTRODE
        TSS_E18_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 19
      #ifdef TSS_E19_SHIELD_ELECTRODE
        TSS_E19_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 20
      #ifdef TSS_E20_SHIELD_ELECTRODE
        TSS_E20_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 21
      #ifdef TSS_E21_SHIELD_ELECTRODE
        TSS_E21_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 22
      #ifdef TSS_E22_SHIELD_ELECTRODE
        TSS_E22_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 23
      #ifdef TSS_E23_SHIELD_ELECTRODE
        TSS_E23_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 24
      #ifdef TSS_E24_SHIELD_ELECTRODE
        TSS_E24_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 25
      #ifdef TSS_E25_SHIELD_ELECTRODE
        TSS_E25_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 26
      #ifdef TSS_E26_SHIELD_ELECTRODE
        TSS_E26_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 27
      #ifdef TSS_E27_SHIELD_ELECTRODE
        TSS_E27_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 28
      #ifdef TSS_E28_SHIELD_ELECTRODE
        TSS_E28_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 29
      #ifdef TSS_E29_SHIELD_ELECTRODE
        TSS_E29_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 30
      #ifdef TSS_E30_SHIELD_ELECTRODE
        TSS_E30_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 31
      #ifdef TSS_E31_SHIELD_ELECTRODE
        TSS_E31_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 32
      #ifdef TSS_E32_SHIELD_ELECTRODE
        TSS_E32_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 33
      #ifdef TSS_E33_SHIELD_ELECTRODE
        TSS_E33_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 34
      #ifdef TSS_E34_SHIELD_ELECTRODE
        TSS_E34_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 35
      #ifdef TSS_E35_SHIELD_ELECTRODE
        TSS_E35_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 36
      #ifdef TSS_E36_SHIELD_ELECTRODE
        TSS_E36_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 37
      #ifdef TSS_E37_SHIELD_ELECTRODE
        TSS_E37_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 38
      #ifdef TSS_E38_SHIELD_ELECTRODE
        TSS_E38_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 39
      #ifdef TSS_E39_SHIELD_ELECTRODE
        TSS_E39_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 40
      #ifdef TSS_E40_SHIELD_ELECTRODE
        TSS_E40_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 41
      #ifdef TSS_E41_SHIELD_ELECTRODE
        TSS_E41_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 42
      #ifdef TSS_E42_SHIELD_ELECTRODE
        TSS_E42_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 43
      #ifdef TSS_E43_SHIELD_ELECTRODE
        TSS_E43_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 44
      #ifdef TSS_E44_SHIELD_ELECTRODE
        TSS_E44_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 45
      #ifdef TSS_E45_SHIELD_ELECTRODE
        TSS_E45_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 46
      #ifdef TSS_E46_SHIELD_ELECTRODE
        TSS_E46_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 47
      #ifdef TSS_E47_SHIELD_ELECTRODE
        TSS_E47_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 48
      #ifdef TSS_E48_SHIELD_ELECTRODE
        TSS_E48_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 49
      #ifdef TSS_E49_SHIELD_ELECTRODE
        TSS_E49_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 50
      #ifdef TSS_E50_SHIELD_ELECTRODE
        TSS_E50_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 51
      #ifdef TSS_E51_SHIELD_ELECTRODE
        TSS_E51_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 52
      #ifdef TSS_E52_SHIELD_ELECTRODE
        TSS_E52_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 53
      #ifdef TSS_E53_SHIELD_ELECTRODE
        TSS_E53_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 54
      #ifdef TSS_E54_SHIELD_ELECTRODE
        TSS_E54_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 55
      #ifdef TSS_E55_SHIELD_ELECTRODE
        TSS_E55_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 56
      #ifdef TSS_E56_SHIELD_ELECTRODE
        TSS_E56_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 57
      #ifdef TSS_E57_SHIELD_ELECTRODE
        TSS_E57_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 58
      #ifdef TSS_E58_SHIELD_ELECTRODE
        TSS_E58_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 59
      #ifdef TSS_E59_SHIELD_ELECTRODE
        TSS_E59_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 60
      #ifdef TSS_E60_SHIELD_ELECTRODE
        TSS_E60_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 61
      #ifdef TSS_E61_SHIELD_ELECTRODE
        TSS_E61_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 62
      #ifdef TSS_E62_SHIELD_ELECTRODE
        TSS_E62_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
    #if TSS_N_ELECTRODES > 63
      #ifdef TSS_E63_SHIELD_ELECTRODE
        TSS_E63_SHIELD_ELECTRODE,
      #else
        TSS_SHIELD_NOT_USED,
      #endif
    #endif
};
#endif
    
/*
 * The section below defines an array of pointers to 
 * private dat for each control
 * 
 */
    
#if TSS_USE_CONTROL_PRIVATE_DATA    
    void * tss_apsControlPrivateData[TSS_N_CONTROLS] = 
    {
      NULL,
      #if TSS_N_CONTROLS > 1
          NULL,
      #endif
      #if TSS_N_CONTROLS > 2
          NULL,
      #endif
      #if TSS_N_CONTROLS > 3
          NULL,
      #endif
      #if TSS_N_CONTROLS > 4
          NULL,
      #endif
      #if TSS_N_CONTROLS > 5
          NULL,
      #endif
      #if TSS_N_CONTROLS > 6
          NULL,
      #endif
      #if TSS_N_CONTROLS > 7
          NULL,
      #endif
      #if TSS_N_CONTROLS > 8
          NULL,
      #endif
      #if TSS_N_CONTROLS > 9
          NULL,
      #endif
      #if TSS_N_CONTROLS > 10
          NULL,
      #endif
      #if TSS_N_CONTROLS > 11
          NULL,
      #endif
      #if TSS_N_CONTROLS > 12
          NULL,
      #endif
      #if TSS_N_CONTROLS > 13
          NULL,
      #endif
      #if TSS_N_CONTROLS > 14
          NULL,
      #endif
      #if TSS_N_CONTROLS > 15
          NULL,
      #endif
    };
#else
    void * tss_apsControlPrivateData[];
#endif    
    
/*
 * Declaration of main structures and arrays used by TSS 
 */

TSS_CSSystem tss_CSSys;
UINT8 tss_au8Sensitivity[TSS_N_ELECTRODES];
UINT8 tss_au8ElectrodeEnablers[((TSS_N_ELECTRODES - 1)/ 8) + 1];
UINT8 tss_au8ElectrodeStatus[((TSS_N_ELECTRODES - 1)/ 8) + 1];

UINT8 tss_au8ElectrodeMeasured[((TSS_N_ELECTRODES - 1)/ 8) + 1];

UINT16 tss_au16ElecBaseline[TSS_N_ELECTRODES];

/* Declaration of arrays used to store electrode behavior values */
INT8  tss_ai8CalPrevDelta[TSS_N_ELECTRODES];
UINT8 tss_au8DebounceCount[TSS_N_ELECTRODES];
UINT8 tss_au8TouchCount[TSS_N_ELECTRODES];
UINT8 tss_au8ReleaseCount[TSS_N_ELECTRODES];
UINT8 tss_au8ElecState[TSS_N_ELECTRODES];
UINT8 tss_au8Buffers[N_BUFF_ELEMENTS + 1u];                     /* The extra byte is because of the end of character */

/* Evaluate No of Controls */
#if TSS_N_CONTROLS != 0
    extern void TSS_ExecuteDecoders(void);
    void (* const tss_fExecuteDecoders) (void) = TSS_ExecuteDecoders;    
    
    extern UINT8 TSS_KeyDetectorDecoderDataInit(UINT8 *u8GrpCount, UINT8 *u8GrpByteSel, UINT8 *u8GrpBitSel, UINT8 u8ElecCount, UINT8 u8ElectrodeCommand);
    extern UINT8 TSS_KeyDetectorDecoderDataInitSeq(UINT8 *u8GrpCount, UINT8 *u8GrpByteSel, UINT8 *u8GrpBitSel, UINT8 u8ElecCount, UINT8 u8ElectrodeCommand);
    UINT8 (* const tss_fDecoderDataInit) (UINT8 *u8GrpCount, UINT8 *u8GrpByteSel, UINT8 *u8GrpBitSel, UINT8 u8ElecCount, UINT8 u8ElectrodeCommand) = TSS_KeyDetectorDecoderDataInit;  
    UINT8 (* const tss_fDecoderDataInitSeq) (UINT8 *u8GrpCount, UINT8 *u8GrpByteSel, UINT8 *u8GrpBitSel, UINT8 u8ElecCount, UINT8 u8ElectrodeCommand) = TSS_KeyDetectorDecoderDataInitSeq; 

    UINT8 tss_au8ControlScheduleCount[TSS_N_CONTROLS];
    UINT8 tss_au8ControlIdleCounter[TSS_N_CONTROLS];
    UINT8 tss_au8ControlIdleState[((TSS_N_CONTROLS - 1)/ 8) + 1];
    UINT8 tss_au8ControlChangeSignal[((TSS_N_CONTROLS - 1)/ 8) + 1];
    UINT8 tss_au8ControlEnablers[((TSS_N_CONTROLS - 1)/ 8) + 1];    /* Enablers for scaning */
#else
    void (* const tss_fExecuteDecoders) (void) = NULL;
    UINT8 (* const tss_fDecoderDataInit) (UINT8 *u8GrpCount, UINT8 *u8GrpByteSel, UINT8 *u8GrpBitSel, UINT8 u8ElecCount, UINT8 u8ElectrodeCommand) = NULL;  
    UINT8 (* const tss_fDecoderDataInitSeq) (UINT8 *u8GrpCount, UINT8 *u8GrpByteSel, UINT8 *u8GrpBitSel, UINT8 u8ElecCount, UINT8 u8ElectrodeCommand) = NULL; 
    
    UINT8 tss_au8ControlScheduleCount[1];
    UINT8 tss_au8ControlIdleCounter[1];
    UINT8 tss_au8ControlIdleState[1];
    UINT8 tss_au8ControlChangeSignal[1];
    UINT8 tss_au8ControlEnablers[1];                                /* Enablers for scaning */
#endif

const UINT8 tss_cu8NumElecs = (UINT8)TSS_N_ELECTRODES;
const UINT8 tss_cu8NumCtrls = (UINT8)TSS_N_CONTROLS;
const UINT8 tss_cu8ElecBitFldLn = (UINT8)(((UINT8)TSS_N_ELECTRODES - 1U) / 8U) + 1U;

#if TSS_N_CONTROLS != 0
    const UINT8 tss_cu8CtrlBitFldLn = (UINT8)(((UINT8)TSS_N_CONTROLS - 1U) / 8u) + 1u;
#else
    const UINT8 tss_cu8CtrlBitFldLn = 1U;
#endif

/*
 * The section below defines separation of the functions 
 * called in low level functions according to selected TSS features by user
 * 
 */

#if TSS_USE_IIR_FILTER
    extern void TSS_KeyDetectorCalculateIIRFilter(UINT8 u8ElecNum);
   
    void (* const tss_fCalculateIIRFilter) (UINT8 u8ElecNum) = TSS_KeyDetectorCalculateIIRFilter;
#else
    void (* const tss_fCalculateIIRFilter) (UINT8 u8ElecNum) = NULL;
#endif

#if TSS_USE_SIGNAL_SHIELDING
    extern void TSS_KeyDetectorCalculateShielding(UINT8 u8ElecNum); 
    
    void (* const tss_fCalculateShielding) (UINT8 u8ElecNum) = TSS_KeyDetectorCalculateShielding;  
#else 
    void (* const tss_fCalculateShielding) (UINT8 u8ElecNum) = NULL;
#endif
   
#if TSS_USE_SIGNAL_SHIELDING || TSS_USE_IIR_FILTER
    extern void TSS_KeyDetectorInitPrevCapSample(UINT8 u8ElecNum);

    void (* const tss_fInitPrevCapSample) (UINT8 u8ElecNum) = TSS_KeyDetectorInitPrevCapSample;    

    UINT16 tss_au16PrevCapSample[TSS_N_ELECTRODES];
#else
    void (* const tss_fInitPrevCapSample) (UINT8 u8ElecNum) = NULL;    

    UINT16 tss_au16PrevCapSample[]; /* Dummy */
#endif    

    
#if TSS_USE_DELTA_LOG
    extern void TSS_KeyDetectorFillDeltaLogBuffer(UINT8 u8ElecNum, INT8 i8Delta);
   
    void (* const tss_fFillDeltaLoglBuffer) (UINT8 u8ElecNum, INT8 i8Delta) = TSS_KeyDetectorFillDeltaLogBuffer;

    INT8 tss_ai8InstantDelta[TSS_N_ELECTRODES];
#else
    void (* const tss_fFillDeltaLoglBuffer) (UINT8 u8ElecNum, INT8 i8Delta) = NULL;

    INT8 tss_ai8InstantDelta[]; /* Dummy */
#endif

#if TSS_USE_SIGNAL_LOG 
    extern void TSS_KeyDetectorFillSignalLogBuffer(UINT8 u8ElecNum);
    
    void (* const tss_fFillSignalLoglBuffer) (UINT8 u8ElecNum) = TSS_KeyDetectorFillSignalLogBuffer;
        
    UINT16 tss_au16InstantSignal[TSS_N_ELECTRODES];
#else
    void (* const tss_fFillSignalLoglBuffer) (UINT8 u8ElecNum) = NULL;
    
    UINT16 tss_au16InstantSignal[]; /* Dummy */
#endif

#if TSS_USE_DATA_CORRUPTION_CHECK
    extern void TSS_DataCorruptionCheck(UINT8 u8Command);
    
    void (* const tss_fDataCorruptionCheck) (UINT8 u8Command) = TSS_DataCorruptionCheck;
    
    UINT8 tss_u8ConfCheckSum;
#else
    void (* const tss_fDataCorruptionCheck) (UINT8 u8Command) = NULL;

    UINT8 tss_u8ConfCheckSum; /* Dummy */
#endif

#if TSS_USE_FREEMASTER_GUI
    extern void TSS_FmstrCall(void);
    
    void (* const tss_fFmstrCall) (void) = TSS_FmstrCall;
#else
    void (* const tss_fFmstrCall) (void) = NULL;
#endif
    
#if TSS_USE_TRIGGER_FUNCTION
    extern UINT8 TSS_TriggerConfig(UINT8 u8Parameter, UINT8 u8Value);
    extern UINT8 TSS_TriggerControl(UINT8 u8Command);
    
    UINT8 (* const tss_fTriggerConfig) (UINT8 u8Parameter, UINT8 u8Value) = TSS_TriggerConfig;
    UINT8 (* const tss_fTriggerControl) (UINT8 u8Command) = TSS_TriggerControl;
#else
    UINT8 (* const tss_fTriggerConfig) (UINT8 u8Parameter, UINT8 u8Value) = NULL;
    UINT8 (* const tss_fTriggerControl) (UINT8 u8Command) = NULL;
#endif

#if TSS_LOWPOWER_USED
    extern UINT8 TSS_LowPowerConfig(UINT8 u8Parameter, UINT8 u8Value);
    
    UINT8 (* const tss_fLowPowerConfig) (UINT8 u8Parameter, UINT8 u8Value) = TSS_LowPowerConfig;
#else
    UINT8 (* const tss_fLowPowerConfig) (UINT8 u8Parameter, UINT8 u8Value) = NULL;
#endif

#ifdef TSS_ONFAULT_CALLBACK
    extern void TSS_ONFAULT_CALLBACK(UINT8 u8FaultElecNum);
  
    void (* const tss_fOnFault) (UINT8 u8FaultElecNum) = TSS_ONFAULT_CALLBACK;
#else 
    void (* const tss_fOnFault) (UINT8 u8FaultElecNum) = NULL;
#endif
    
#ifdef TSS_ONINIT_CALLBACK
    extern void TSS_ONINIT_CALLBACK(void);
  
    void (* const tss_fOnInit) (void) = TSS_ONINIT_CALLBACK;
#else 
    extern void TSS_fOnInit(void);
    
    void (* const tss_fOnInit) (void) = TSS_fOnInit;
#endif
    
#if TSS_USE_STUCK_KEY
    extern void TSS_KeyDetectorStuckKeyControl(UINT8 u8ElecNum, INT8 i8Delta, UINT8 u8RetVal);
    extern void TSS_KeyDetectorResetStuckKeyCounter(UINT8 u8ElecNum);
    
    void (* const tss_fStuckKeyControl) (UINT8 u8ElecNum, INT8 i8Delta, UINT8 u8RetVal) = TSS_KeyDetectorStuckKeyControl;
    void (* const tss_fResetStuckKeyCounter) (UINT8 u8ElecNum) = TSS_KeyDetectorResetStuckKeyCounter;
    
    UINT8 tss_au8StuckCount[TSS_N_ELECTRODES];    
#else
    void (* const tss_fStuckKeyControl) (UINT8 u8ElecNum, INT8 i8Delta, UINT8 u8RetVal) = NULL;
    void (* const tss_fResetStuckKeyCounter) (UINT8 u8ElecNum) = NULL;

    UINT8 tss_au8StuckCount[];  /* Dummy */
#endif

#if TSS_USE_NEGATIVE_BASELINE_DROP
    extern void TSS_KeyDetectorNegativeBaselineDrop(UINT8 u8ElecNum, INT8 i8Delta);

    void (* const tss_fNegativeBaselineDrop) (UINT8 u8ElecNum, INT8 i8Delta) = TSS_KeyDetectorNegativeBaselineDrop;
#else
    void (* const tss_fNegativeBaselineDrop) (UINT8 u8ElecNum, INT8 i8Delta) = NULL;
#endif
    
/***************************************************************************//*!
*
* @brief  Performs a calculation of new capacitance value by user defined 
*         IIR filter equation from the current capacitance value and 
*         previous capacitance value for a certain electrode 
*
* @param  u16CurrentCapSample - Current measured oversampled capacitance value
*         u8ElecNum - Defines electrode number
*
* @return New calculated capacitance value
*
* @remarks Array tss_au16PrevCapSample[] is used as global storage for 
*          backup of previous capacitance values. The parameter u8ElecNum of 
*          TSS_IIRFilterEquation() should be used as pointer on the currently 
*          used previous capacitance value e.g. tss_au16PrevCapSample[u8ElecNum].
*          The principle of IIR Filter is to modify u16CurrentCapSample by any 
*          weight of tss_au16PrevCapSample[u8ElecNum] and the calculated value is
*          returned to the library by the return value of TSS_IIRFilterEquation().
*          The user may edit the equation according to his requirements,
*          but he has to follow previous rules.
*          Warning: Design new equation with respect to unsigned integer value 
*                   range of variables and number of samples used by TSS.
*
****************************************************************************/
#if TSS_USE_IIR_FILTER
    UINT16 TSS_IIRFilterEquation(UINT16 u16CurrentCapSample, UINT8 u8ElecNum)
    {
      return (UINT16) ((u16CurrentCapSample + (3u * tss_au16PrevCapSample[u8ElecNum])) >> 2u);
    }
#else
    /* Dummy Function - just for relations, anyway removed from flash */
    UINT16 TSS_IIRFilterEquation(UINT16 u16CurrentCapSample, UINT8 u8ElecNum)
    {
      (void) u16CurrentCapSample;
      (void) u8ElecNum;
      
      return TSS_STATUS_OK; 
    }
#endif

/***************************************************************************//*!
*
* @brief  Performs a calculation of new capacitance value by user defined 
*         Shield equation from the defined shielding electrode delta value
*
* @param  u16CurrentCapSample - Current measured oversampled capacitance value
*         u8ElecNum - Defines electrode number
*
* @return New calculated capacitance value
*
* @remarks Array tss_au16PrevCapSample[] is used as global storage for 
*          backup of previous capacitance values. The parameter u8ElecNum of 
*          TSS_ShieldEquation() should be used as pointer on the currently 
*          used previous capacitance value e.g. tss_au16PrevCapSample[u8ElecNum].
*          The principle of Shielding routine is to modify u16CurrentCapSample by any 
*          delta from ceratin electrode. Calculated value is
*          returned to the library by the return value of TSS_ShieldEquation().
*          The user may edit the equation according to his requirements,
*          but he has to follow previous rules.
*          Warning: Design new equation with respect to unsigned integer value 
*                   range of variables and number of samples used by TSS.
*
****************************************************************************/
#if TSS_USE_SIGNAL_SHIELDING
    UINT16 TSS_ShieldEquation(UINT16 u16CurrentCapSample, UINT8 u8ElecNum) 
    {
      UINT16 u16result;
      UINT16 u16PrevSample, u16PrevBaseline;
      
      if (tss_cau8WShieldPairs[u8ElecNum] != TSS_SHIELD_NOT_USED) 
      {
        /* Calculate Previous Delta */
        u16PrevSample = tss_au16PrevCapSample[tss_cau8WShieldPairs[u8ElecNum]];
        u16PrevBaseline = tss_au16ElecBaseline[tss_cau8WShieldPairs[u8ElecNum]];
        if (u16PrevSample > u16PrevBaseline)
        {
          u16result = (u16CurrentCapSample - (u16PrevSample - u16PrevBaseline));
        } else {
          u16result = (u16CurrentCapSample + (u16PrevBaseline - u16PrevSample));        
        }
      } else {
        u16result = u16CurrentCapSample;
      }
      
      return u16result;
    }
#else
    /* Dummy Function - just for relations, anyway removed from flash */
    UINT16 TSS_ShieldEquation(UINT16 u16CurrentCapSample, UINT8 u8ElecNum)
    {
      (void) u16CurrentCapSample;
      (void) u8ElecNum;
      
      return TSS_STATUS_OK;
    } 
#endif
