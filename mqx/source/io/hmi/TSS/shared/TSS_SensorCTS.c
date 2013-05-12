/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorCTS.c
*
* @brief  Contains functions to Perform the sensing ot the electrodes and set the status for each electrode
*
* @version 1.0.55.0
* 
* @date Mar-12-2012
* 
*
***********************************************************************************************************************/

#include "TSS_SensorCTS.h"
 
#if TSS_DETECT_METHOD(CTS)

  #if TSS_HCS08_MCU || TSS_CFV1_MCU
    
    UINT8 CTS_GetTurbo(UINT8 u8ElecNum);
    
    /************************* Decode  Macros ************************/
    
    #define TSS_PORT(port,pin)            &CTS_##port##pin##HS
    #define TSS2CTS_CONV(port,pin)	  TSS_PORT(port,pin)
    #define CTS_LOW_ROUTINE(X)            TSS2CTS_CONV(TSS_##X##_P,TSS_##X##_B)

    /***************** ROM & RAM Data definition *********************/

    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(0), CTS_LOW_ROUTINE(E0),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(1), CTS_LOW_ROUTINE(E1),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(2), CTS_LOW_ROUTINE(E2),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(3), CTS_LOW_ROUTINE(E3),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(4), CTS_LOW_ROUTINE(E4),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(5), CTS_LOW_ROUTINE(E5),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(6), CTS_LOW_ROUTINE(E6),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(7), CTS_LOW_ROUTINE(E7),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(8), CTS_LOW_ROUTINE(E8),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(9), CTS_LOW_ROUTINE(E9),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(10), CTS_LOW_ROUTINE(E10),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(11), CTS_LOW_ROUTINE(E11),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(12), CTS_LOW_ROUTINE(E12),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(13), CTS_LOW_ROUTINE(E13),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(14), CTS_LOW_ROUTINE(E14),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(15), CTS_LOW_ROUTINE(E15),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(16), CTS_LOW_ROUTINE(E16),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(17), CTS_LOW_ROUTINE(E17),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(18), CTS_LOW_ROUTINE(E18),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(19), CTS_LOW_ROUTINE(E19),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(20), CTS_LOW_ROUTINE(E20),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(21), CTS_LOW_ROUTINE(E21),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(22), CTS_LOW_ROUTINE(E22),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(23), CTS_LOW_ROUTINE(E23),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(24), CTS_LOW_ROUTINE(E24),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(25), CTS_LOW_ROUTINE(E25),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(26), CTS_LOW_ROUTINE(E26),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(27), CTS_LOW_ROUTINE(E27),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(28), CTS_LOW_ROUTINE(E28),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(29), CTS_LOW_ROUTINE(E29),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(30), CTS_LOW_ROUTINE(E30),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(31), CTS_LOW_ROUTINE(E31),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(32), CTS_LOW_ROUTINE(E32),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(33), CTS_LOW_ROUTINE(E33),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(34), CTS_LOW_ROUTINE(E34),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(35), CTS_LOW_ROUTINE(E35),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(36), CTS_LOW_ROUTINE(E36),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(37), CTS_LOW_ROUTINE(E37),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(38), CTS_LOW_ROUTINE(E38),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(39), CTS_LOW_ROUTINE(E39),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(40), CTS_LOW_ROUTINE(E40),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(41), CTS_LOW_ROUTINE(E41),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(42), CTS_LOW_ROUTINE(E42),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(43), CTS_LOW_ROUTINE(E43),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(44), CTS_LOW_ROUTINE(E44),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(45), CTS_LOW_ROUTINE(E45),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(46), CTS_LOW_ROUTINE(E46),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(47), CTS_LOW_ROUTINE(E47),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(48), CTS_LOW_ROUTINE(E48),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(49), CTS_LOW_ROUTINE(E49),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(50), CTS_LOW_ROUTINE(E50),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(51), CTS_LOW_ROUTINE(E51),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(52), CTS_LOW_ROUTINE(E52),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(53), CTS_LOW_ROUTINE(E53),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(54), CTS_LOW_ROUTINE(E54),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(55), CTS_LOW_ROUTINE(E55),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(56), CTS_LOW_ROUTINE(E56),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(57), CTS_LOW_ROUTINE(E57),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(58), CTS_LOW_ROUTINE(E58),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(59), CTS_LOW_ROUTINE(E59),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(60), CTS_LOW_ROUTINE(E60),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(61), CTS_LOW_ROUTINE(E61),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(62), CTS_LOW_ROUTINE(E62),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(63), CTS_LOW_ROUTINE(E63),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { -127,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0,
                                                                 #endif
                                                                 };
      #endif
    #endif
    
    /***************************************************************************//*!
    *
    * @brief  Initializes the variables related to CTS Sensor   
    *
    * @param  Nothing
    *
    * @return Status Code
    *
    * @remarks  Set cts_i8Turbo array to default value and initiliaze Noise Amplitude Filter
    *
    ****************************************************************************/ 
    UINT8 CTS_SensorInit(void)
    {
        UINT8 u8counter = 0;

        TSS_CTS_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
        /* Pointers decoding */
        psElectrodeRAMDataStruct = (TSS_CTS_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8counter]);

        for (u8counter = 0; u8counter < TSS_N_ELECTRODES; u8counter++) {
        
          ((TSS_CTS_ELECTRODE_RAMDATA *) tss_acp8ElectrodeRAMData[u8counter])->cts_i8turbo = -127; /* Set default value */

          #if TSS_USE_NOISE_AMPLITUDE_FILTER
           /* Initializes the Noise Amplitude Filter variables */
            ((TSS_CTS_ELECTRODE_RAMDATA *) tss_acp8ElectrodeRAMData[u8counter])->cts_i8AmplitudeFilterBase = 0; /* Set default value */
          #endif
        }
        
        return TSS_INIT_STATUS_OK;
    }
    
    /***************************************************************************//*!
    *
    * @brief  Performs a determination of Turbo values for certain electrode  
    *
    * @param  u8ElecNum Required Electrode number
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/ 
    UINT8 CTS_GetTurbo(UINT8 u8ElecNum) 
    {
       UINT8 u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
       UINT8 u8Result = TSS_INIT_STATUS_OK;
       INT8 i8SampleTest = 0;
       INT8 i8TurboTemp = 0;
       INT8 i8OldTurboTemp;
       
       TSS_CTS_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
       TSS_CTS_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
      
       /* Pointers Decoding */      
       psElectrodeRAMDataStruct = (TSS_CTS_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
       psElectrodeROMDataStruct = (TSS_CTS_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);

       /* Determine first Turbo value */
       u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
       do {
          i8TurboTemp = (psElectrodeROMDataStruct->cts_cps8CTSLowRoutine) (cCTS_ARG_TURBO);
          u8FaultCnt--;
       } while ((i8TurboTemp < 0) && (u8FaultCnt > 0));
         
       if (u8FaultCnt > 0) { /* Success */
      
         /* Try maximize turbo speed-up */
         u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
         do {
           do {
              i8SampleTest = (psElectrodeROMDataStruct->cts_cps8CTSLowRoutine) (++i8TurboTemp);
           } while (i8SampleTest > TSS_CTS_MIN_TURBO_SIGNAL);     
  
           if (i8SampleTest == cCTS_ERROR_ExISR) /* Solving of situation when Interrupt occured */
           { 
             i8TurboTemp--;   
           }
  
           u8FaultCnt--; 
         } while ((i8SampleTest == cCTS_ERROR_ExISR) && (u8FaultCnt > 0));
         
         /* Timeout evaluation after measurement */
         if (u8FaultCnt > 0) 
         {
             /* Align turbo speed-up to first one appropriate */
             u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
             do {
               do {
                  i8SampleTest = (psElectrodeROMDataStruct->cts_cps8CTSLowRoutine) (--i8TurboTemp);
               } while ((i8SampleTest <= TSS_CTS_MIN_TURBO_SIGNAL) && (i8TurboTemp > 0));
               
               if (i8SampleTest == cCTS_ERROR_ExISR) {  /* Solving of situation when Interrupt occured */
                 i8TurboTemp++;   
               }
               
               u8FaultCnt--; 
             } while ((i8SampleTest == cCTS_ERROR_ExISR) && (u8FaultCnt > 0));
             
             if (u8FaultCnt > 0) {
                u8Result = TSS_INIT_STATUS_OK;
    
                /* Change Turbo value just when difference is more than 1, for prevention of oscillations */
                i8OldTurboTemp = psElectrodeRAMDataStruct->cts_i8turbo;
                if (i8OldTurboTemp != i8TurboTemp) 
                {
                  i8OldTurboTemp -= i8TurboTemp;
                  if ( (i8OldTurboTemp > 1) || (i8OldTurboTemp < (-1)) ) 
                  {
                    u8Result |= TSS_INIT_STATUS_CALIBRATION_CHANGED;
                    psElectrodeRAMDataStruct->cts_i8turbo = i8TurboTemp;
                  }
                }
             
             } else {
                u8Result |= TSS_INIT_ERROR_RECALIB_FAULT; /* If a fault has occurred sets the return status code error */
             } 
               
         } else {
            u8Result |= TSS_INIT_ERROR_RECALIB_FAULT; /* If a fault has occurred sets the return status code error */
         }
       
       } else {
         u8Result |= TSS_INIT_ERROR_RECALIB_FAULT; /* If a fault has occurred sets the return status code error */
       }
       
       return u8Result;
    }

    /***************************************************************************//*!
    *
    * @brief  Performs a valid reading of one Electrode stores the timer values 
    *         and returns a status code  
    *
    * @param  u8ElecNum Required Electrode number
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/
    UINT8 CTS_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
    {
      UINT8 u8NSamp;
      UINT8 u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  
      UINT8 u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
      UINT8 u8RecalibCounter = 0u;
      INT8 i8Sample = 0;

      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        INT8 i8AmplitudeFilterDeltaLimitTemp;
      #endif

      TSS_CTS_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
      TSS_CTS_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
      
      /* Pointers Decoding */      
      psElectrodeRAMDataStruct = (TSS_CTS_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
      psElectrodeROMDataStruct = (TSS_CTS_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
        
      if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
      {
        u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;           
      } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
      {  

        /* Intitialization of variables */
        tss_u16CapSample = TSS_CTS_SIGNAL_OFFSET;
        u8NSamp = tss_CSSys.NSamples;    
        
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (psElectrodeRAMDataStruct->cts_i8AmplitudeFilterBase == 0) {
            i8AmplitudeFilterDeltaLimitTemp = 127;
          } else {
            i8AmplitudeFilterDeltaLimitTemp = psElectrodeROMDataStruct->cts_i8AmplitudeFilterDeltaLimit;
          }
        #endif

        /* First Turbo Determination */
        if (psElectrodeRAMDataStruct->cts_i8turbo == -127) {
          u8ElectrodeStatus = CTS_GetTurbo(u8ElecNum);
        
          if (u8ElectrodeStatus & TSS_INIT_ERROR_RECALIB_FAULT) 
          {
            /* Turbo value wasn't determined so don't measure and disable actual electrode */
            u8NSamp = 0u;   
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;
          } else 
          {
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  /* New Turbo value was set so status is OK */
          }
        }

         /* Main measurement loop */
        while(u8NSamp > 0) 
        {                  

          i8Sample =(psElectrodeROMDataStruct->cts_cps8CTSLowRoutine) (psElectrodeRAMDataStruct->cts_i8turbo);   /* Perform measurement */        
        
          if (i8Sample > cCTS_ERROR_LoCAP)  {         /* Capacitance Signal Ok */

            #if TSS_USE_NOISE_AMPLITUDE_FILTER
              if (TSS_i8NoiseAmplitudeFilter(i8Sample, &(psElectrodeRAMDataStruct->cts_i8AmplitudeFilterBase), i8AmplitudeFilterDeltaLimitTemp)) 
              {
                u8FaultCnt--;                                      
              } 
              else 
              { 
                tss_u16CapSample += i8Sample;
                u8NSamp--;
                u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;           /* Restart Fault counter due long cyclic interrupts */
              }
            #else
              tss_u16CapSample += i8Sample;
              u8NSamp--;
              u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
            #endif  
   
            if (i8Sample >  TSS_CTS_RECALIB_HIGH_LIMIT) /* If Cap Signal Very High */
            {      
              if (u8RecalibCounter < TSS_CTS_RECALIB_COUNTER_TIMEOUT) 
              {
                u8RecalibCounter++;
              } else {
                u8ElectrodeStatus = TSS_SAMPLE_RECALIB_REQUEST_HICAP; /* If a recalibration is needed */
              }
            }

          } else if (i8Sample == cCTS_ERROR_LoCAP) {  /* Dynamic Recalibration in Keydetector */

            u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
            if (u8RecalibCounter < TSS_CTS_RECALIB_COUNTER_TIMEOUT) {
              u8RecalibCounter++;
            } else {
              u8NSamp = 0u;
              
              if (psElectrodeRAMDataStruct->cts_i8turbo < 2)  {
                u8ElectrodeStatus = TSS_KEYDETECT_ERROR_SMALL_CAP; /* If not possible decrease of Turbo value, Disable Electrode */             
              } else {
                u8ElectrodeStatus = TSS_SAMPLE_RECALIB_REQUEST_LOCAP; /* If a recalibration is possible, request */
              }
              
            }

          } else if (i8Sample == cCTS_ERROR_ExISR) {  /* Interrupt Occured */
            
            /* Do nothing because the measurement was corrupted by interrupt, and sample again in next loop */
            u8FaultCnt--;
            
          } else { /* if (-128) then Capacitance is extremely high, variable overflow */
            u8NSamp = 0u;
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */

          }
          
          if(u8FaultCnt == 0u) {
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
            u8NSamp = 0u; 
          }
           
        }
        
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (u8ElectrodeStatus == TSS_SAMPLE_STATUS_OK) {
            if (psElectrodeRAMDataStruct->cts_i8AmplitudeFilterBase == 0) {
              psElectrodeRAMDataStruct->cts_i8AmplitudeFilterBase = (INT8) (tss_u16CapSample / tss_CSSys.NSamples);
            }
          } else {
            psElectrodeRAMDataStruct->cts_i8AmplitudeFilterBase = 0; /* Initiate Amplitude Filter Base refresh */
          }
        #endif
      } else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
      {
        /* Return the same electrode number because there is no more electrodes in the module */
        u8ElectrodeStatus = u8ElecNum;

      } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
      {
        u8ElectrodeStatus = CTS_GetTurbo(u8ElecNum); 

        if (u8ElectrodeStatus & TSS_INIT_ERROR_RECALIB_FAULT) 
        {
          /* Fault happened, so disable actual electrode */
          u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;
        } else if (u8ElectrodeStatus & TSS_INIT_STATUS_CALIBRATION_CHANGED) 
        {
          /* New Turbo value was set so status is OK */
          u8ElectrodeStatus = TSS_SAMPLE_STATUS_CALIBRATION_CHANGED;
        } else 
        {
          /* Old Turbo value was correct */
          u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  
        }
      } 
        
      return u8ElectrodeStatus;   /* Return status code */
    }  

  #elif TSS_KINETIS_MCU || TSS_CFM_MCU
    
    /************************** Prototypes ***************************/

    UINT8 CTS_GetTurbo(UINT8 u8ElecNum);
    UINT8 CTS_MethodControl(UINT8 u8ElNum, UINT8 u8Command);

    INT16 CTS_A0HS  (INT16 CTS_arg);
    INT16 CTS_A1HS  (INT16 CTS_arg);
    INT16 CTS_A2HS  (INT16 CTS_arg);
    INT16 CTS_A3HS  (INT16 CTS_arg);
    INT16 CTS_A4HS  (INT16 CTS_arg);
    INT16 CTS_A5HS  (INT16 CTS_arg);
    INT16 CTS_A6HS  (INT16 CTS_arg);
    INT16 CTS_A7HS  (INT16 CTS_arg);
    #if TSS_KINETIS_MCU       
      INT16 CTS_A8HS  (INT16 CTS_arg);
      INT16 CTS_A9HS  (INT16 CTS_arg);
      INT16 CTS_A10HS (INT16 CTS_arg);
      INT16 CTS_A11HS (INT16 CTS_arg);
      INT16 CTS_A12HS (INT16 CTS_arg);
      INT16 CTS_A13HS (INT16 CTS_arg);
      INT16 CTS_A14HS (INT16 CTS_arg);
      INT16 CTS_A15HS (INT16 CTS_arg);
      INT16 CTS_A16HS (INT16 CTS_arg);
      INT16 CTS_A17HS (INT16 CTS_arg);
      INT16 CTS_A18HS (INT16 CTS_arg);
      INT16 CTS_A19HS (INT16 CTS_arg);
      INT16 CTS_A20HS (INT16 CTS_arg);
      INT16 CTS_A21HS (INT16 CTS_arg);
      INT16 CTS_A22HS (INT16 CTS_arg);
      INT16 CTS_A23HS (INT16 CTS_arg);
      INT16 CTS_A24HS (INT16 CTS_arg);
      INT16 CTS_A25HS (INT16 CTS_arg);
      INT16 CTS_A26HS (INT16 CTS_arg);
      INT16 CTS_A27HS (INT16 CTS_arg);
      INT16 CTS_A28HS (INT16 CTS_arg);
      INT16 CTS_A29HS (INT16 CTS_arg);
      INT16 CTS_A30HS (INT16 CTS_arg);
      INT16 CTS_A31HS (INT16 CTS_arg);
    #endif
    
    INT16 CTS_B0HS  (INT16 CTS_arg);
    INT16 CTS_B1HS  (INT16 CTS_arg);
    INT16 CTS_B2HS  (INT16 CTS_arg);
    INT16 CTS_B3HS  (INT16 CTS_arg);
    INT16 CTS_B4HS  (INT16 CTS_arg);
    INT16 CTS_B5HS  (INT16 CTS_arg);
    INT16 CTS_B6HS  (INT16 CTS_arg);
    INT16 CTS_B7HS  (INT16 CTS_arg);
    #if TSS_KINETIS_MCU       
      INT16 CTS_B8HS  (INT16 CTS_arg);
      INT16 CTS_B9HS  (INT16 CTS_arg);
      INT16 CTS_B10HS (INT16 CTS_arg);
      INT16 CTS_B11HS (INT16 CTS_arg);
      INT16 CTS_B12HS (INT16 CTS_arg);
      INT16 CTS_B13HS (INT16 CTS_arg);
      INT16 CTS_B14HS (INT16 CTS_arg);
      INT16 CTS_B15HS (INT16 CTS_arg);
      INT16 CTS_B16HS (INT16 CTS_arg);
      INT16 CTS_B17HS (INT16 CTS_arg);
      INT16 CTS_B18HS (INT16 CTS_arg);
      INT16 CTS_B19HS (INT16 CTS_arg);
      INT16 CTS_B20HS (INT16 CTS_arg);
      INT16 CTS_B21HS (INT16 CTS_arg);
      INT16 CTS_B22HS (INT16 CTS_arg);
      INT16 CTS_B23HS (INT16 CTS_arg);
      INT16 CTS_B24HS (INT16 CTS_arg);
      INT16 CTS_B25HS (INT16 CTS_arg);
      INT16 CTS_B26HS (INT16 CTS_arg);
      INT16 CTS_B27HS (INT16 CTS_arg);
      INT16 CTS_B28HS (INT16 CTS_arg);
      INT16 CTS_B29HS (INT16 CTS_arg);
      INT16 CTS_B30HS (INT16 CTS_arg);
      INT16 CTS_B31HS (INT16 CTS_arg);
    #endif	

    INT16 CTS_C0HS  (INT16 CTS_arg);
    INT16 CTS_C1HS  (INT16 CTS_arg);
    INT16 CTS_C2HS  (INT16 CTS_arg);
    INT16 CTS_C3HS  (INT16 CTS_arg);
    INT16 CTS_C4HS  (INT16 CTS_arg);
    INT16 CTS_C5HS  (INT16 CTS_arg);
    INT16 CTS_C6HS  (INT16 CTS_arg);
    INT16 CTS_C7HS  (INT16 CTS_arg);
    #if TSS_KINETIS_MCU       
      INT16 CTS_C8HS  (INT16 CTS_arg);
      INT16 CTS_C9HS  (INT16 CTS_arg);
      INT16 CTS_C10HS (INT16 CTS_arg);
      INT16 CTS_C11HS (INT16 CTS_arg);
      INT16 CTS_C12HS (INT16 CTS_arg);
      INT16 CTS_C13HS (INT16 CTS_arg);
      INT16 CTS_C14HS (INT16 CTS_arg);
      INT16 CTS_C15HS (INT16 CTS_arg);
      INT16 CTS_C16HS (INT16 CTS_arg);
      INT16 CTS_C17HS (INT16 CTS_arg);
      INT16 CTS_C18HS (INT16 CTS_arg);
      INT16 CTS_C19HS (INT16 CTS_arg);
      INT16 CTS_C20HS (INT16 CTS_arg);
      INT16 CTS_C21HS (INT16 CTS_arg);
      INT16 CTS_C22HS (INT16 CTS_arg);
      INT16 CTS_C23HS (INT16 CTS_arg);
      INT16 CTS_C24HS (INT16 CTS_arg);
      INT16 CTS_C25HS (INT16 CTS_arg);
      INT16 CTS_C26HS (INT16 CTS_arg);
      INT16 CTS_C27HS (INT16 CTS_arg);
      INT16 CTS_C28HS (INT16 CTS_arg);
      INT16 CTS_C29HS (INT16 CTS_arg);
      INT16 CTS_C30HS (INT16 CTS_arg);
      INT16 CTS_C31HS (INT16 CTS_arg);
    #endif		

    INT16 CTS_D0HS  (INT16 CTS_arg);
    INT16 CTS_D1HS  (INT16 CTS_arg);
    INT16 CTS_D2HS  (INT16 CTS_arg);
    INT16 CTS_D3HS  (INT16 CTS_arg);
    INT16 CTS_D4HS  (INT16 CTS_arg);
    INT16 CTS_D5HS  (INT16 CTS_arg);
    INT16 CTS_D6HS  (INT16 CTS_arg);
    INT16 CTS_D7HS  (INT16 CTS_arg);
    #if TSS_KINETIS_MCU       
      INT16 CTS_D8HS  (INT16 CTS_arg);
      INT16 CTS_D9HS  (INT16 CTS_arg);
      INT16 CTS_D10HS (INT16 CTS_arg);
      INT16 CTS_D11HS (INT16 CTS_arg);
      INT16 CTS_D12HS (INT16 CTS_arg);
      INT16 CTS_D13HS (INT16 CTS_arg);
      INT16 CTS_D14HS (INT16 CTS_arg);
      INT16 CTS_D15HS (INT16 CTS_arg);
      INT16 CTS_D16HS (INT16 CTS_arg);
      INT16 CTS_D17HS (INT16 CTS_arg);
      INT16 CTS_D18HS (INT16 CTS_arg);
      INT16 CTS_D19HS (INT16 CTS_arg);
      INT16 CTS_D20HS (INT16 CTS_arg);
      INT16 CTS_D21HS (INT16 CTS_arg);
      INT16 CTS_D22HS (INT16 CTS_arg);
      INT16 CTS_D23HS (INT16 CTS_arg);
      INT16 CTS_D24HS (INT16 CTS_arg);
      INT16 CTS_D25HS (INT16 CTS_arg);
      INT16 CTS_D26HS (INT16 CTS_arg);
      INT16 CTS_D27HS (INT16 CTS_arg);
      INT16 CTS_D28HS (INT16 CTS_arg);
      INT16 CTS_D29HS (INT16 CTS_arg);
      INT16 CTS_D30HS (INT16 CTS_arg);
      INT16 CTS_D31HS (INT16 CTS_arg);
    #endif		

    INT16 CTS_E0HS  (INT16 CTS_arg);
    INT16 CTS_E1HS  (INT16 CTS_arg);
    INT16 CTS_E2HS  (INT16 CTS_arg);
    INT16 CTS_E3HS  (INT16 CTS_arg);
    INT16 CTS_E4HS  (INT16 CTS_arg);
    INT16 CTS_E5HS  (INT16 CTS_arg);
    INT16 CTS_E6HS  (INT16 CTS_arg);
    INT16 CTS_E7HS  (INT16 CTS_arg);
    #if TSS_KINETIS_MCU       
      INT16 CTS_E8HS  (INT16 CTS_arg);
      INT16 CTS_E9HS  (INT16 CTS_arg);
      INT16 CTS_E10HS (INT16 CTS_arg);
      INT16 CTS_E11HS (INT16 CTS_arg);
      INT16 CTS_E12HS (INT16 CTS_arg);
      INT16 CTS_E13HS (INT16 CTS_arg);
      INT16 CTS_E14HS (INT16 CTS_arg);
      INT16 CTS_E15HS (INT16 CTS_arg);
      INT16 CTS_E16HS (INT16 CTS_arg);
      INT16 CTS_E17HS (INT16 CTS_arg);
      INT16 CTS_E18HS (INT16 CTS_arg);
      INT16 CTS_E19HS (INT16 CTS_arg);
      INT16 CTS_E20HS (INT16 CTS_arg);
      INT16 CTS_E21HS (INT16 CTS_arg);
      INT16 CTS_E22HS (INT16 CTS_arg);
      INT16 CTS_E23HS (INT16 CTS_arg);
      INT16 CTS_E24HS (INT16 CTS_arg);
      INT16 CTS_E25HS (INT16 CTS_arg);
      INT16 CTS_E26HS (INT16 CTS_arg);
      INT16 CTS_E27HS (INT16 CTS_arg);
      INT16 CTS_E28HS (INT16 CTS_arg);
      INT16 CTS_E29HS (INT16 CTS_arg);
      INT16 CTS_E30HS (INT16 CTS_arg);
      INT16 CTS_E31HS (INT16 CTS_arg);
    #endif

    INT16 CTS_F0HS  (INT16 CTS_arg);
    INT16 CTS_F1HS  (INT16 CTS_arg);
    INT16 CTS_F2HS  (INT16 CTS_arg);
    INT16 CTS_F3HS  (INT16 CTS_arg);
    INT16 CTS_F4HS  (INT16 CTS_arg);
    INT16 CTS_F5HS  (INT16 CTS_arg);
    INT16 CTS_F6HS  (INT16 CTS_arg);
    INT16 CTS_F7HS  (INT16 CTS_arg);
    #if TSS_KINETIS_MCU       
      INT16 CTS_F8HS  (INT16 CTS_arg);
      INT16 CTS_F9HS  (INT16 CTS_arg);
      INT16 CTS_F10HS (INT16 CTS_arg);
      INT16 CTS_F11HS (INT16 CTS_arg);
      INT16 CTS_F12HS (INT16 CTS_arg);
      INT16 CTS_F13HS (INT16 CTS_arg);
      INT16 CTS_F14HS (INT16 CTS_arg);
      INT16 CTS_F15HS (INT16 CTS_arg);
      INT16 CTS_F16HS (INT16 CTS_arg);
      INT16 CTS_F17HS (INT16 CTS_arg);
      INT16 CTS_F18HS (INT16 CTS_arg);
      INT16 CTS_F19HS (INT16 CTS_arg);
      INT16 CTS_F20HS (INT16 CTS_arg);
      INT16 CTS_F21HS (INT16 CTS_arg);
      INT16 CTS_F22HS (INT16 CTS_arg);
      INT16 CTS_F23HS (INT16 CTS_arg);
      INT16 CTS_F24HS (INT16 CTS_arg);
      INT16 CTS_F25HS (INT16 CTS_arg);
      INT16 CTS_F26HS (INT16 CTS_arg);
      INT16 CTS_F27HS (INT16 CTS_arg);
      INT16 CTS_F28HS (INT16 CTS_arg);
      INT16 CTS_F29HS (INT16 CTS_arg);
      INT16 CTS_F30HS (INT16 CTS_arg);
      INT16 CTS_F31HS (INT16 CTS_arg);
    #endif

    /************************* Decode  Macros ************************/
    
    #define TSS_PORT(port,pin)            &CTS_##port##pin##HS
    #define TSS2CTS_CONV(port,pin)	  TSS_PORT(port,pin)
    #define CTS_LOW_ROUTINE(X)            TSS2CTS_CONV(TSS_##X##_P,TSS_##X##_B)

    /**********************  Modules definition **********************/
    
    #if TSS_DETECT_MODULE(CTS) 
        const TSS_CTS_METHOD_ROMDATA TSS_CTS_METHOD_ROMDATA_CONTEXT = {CTS_MethodControl}; 
    #endif

    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E0),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E1),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E2),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E3),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E4),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E5),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E6),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E7),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E8),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E9),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E10),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E11),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E12),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E13),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E14),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E15),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E16),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E17),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E18),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E19),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E20),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E21),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E22),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E23),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E24),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E25),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E26),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E27),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E28),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E29),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E30),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E31),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E32),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E33),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E34),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E35),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E36),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E37),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E38),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E39),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E40),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E41),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E42),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E43),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E44),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E45),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E46),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E47),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E48),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E49),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E50),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E51),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E52),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E53),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E54),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E55),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E56),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E57),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E58),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E59),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E60),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E61),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E62),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,CTS)
        const TSS_CTS_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { &TSS_CTS_METHOD_ROMDATA_CONTEXT, CTS_LOW_ROUTINE(E63),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_CTS_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0,
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   0u,
                                                                 #endif
                                                                 };
      #endif
    #endif

    /************************ Macros Definition ***********************/  

    #if TSS_KINETIS_MCU 
        
      /* handling macros for PORT[A/B/C/D/E] */
      #define pPEon   (*ptr_port) |= PORT_PCR_PE_MASK;  /* Enable pull-up */
      #define pPEoff  (*ptr_port) &= ~PORT_PCR_PE_MASK;
      
      #define PORTA_pCLK    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK   /* enable bus-clock */
      #define PORTA_pOUT    GPIOA_PDDR  |= msk_pin  /* out */
      #define PORTA_pINP    GPIOA_PDDR  &= ~msk_pin /* inp */
      #define PORTA_pSET    GPIOA_PSOR  = msk_pin   /* set */
      #define PORTA_pCLR    GPIOA_PCOR  = msk_pin   /* clr */
      #define PORTA_qSET   (GPIOA_PDIR & msk_pin)   /* is set? */
      
      #define PORTB_pCLK    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK
      #define PORTB_pOUT    GPIOB_PDDR  |= msk_pin
      #define PORTB_pINP    GPIOB_PDDR  &= ~msk_pin
      #define PORTB_pSET    GPIOB_PSOR  = msk_pin
      #define PORTB_pCLR    GPIOB_PCOR  = msk_pin
      #define PORTB_qSET   (GPIOB_PDIR & msk_pin)
      
      #define PORTC_pCLK    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK
      #define PORTC_pOUT    GPIOC_PDDR  |= msk_pin
      #define PORTC_pINP    GPIOC_PDDR  &= ~msk_pin
      #define PORTC_pSET    GPIOC_PSOR  = msk_pin
      #define PORTC_pCLR    GPIOC_PCOR  = msk_pin
      #define PORTC_qSET   (GPIOC_PDIR & msk_pin)
      
      #define PORTD_pCLK    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK
      #define PORTD_pOUT    GPIOC_PDDR  |= msk_pin
      #define PORTD_pINP    GPIOC_PDDR  &= ~msk_pin
      #define PORTD_pSET    GPIOC_PSOR  = msk_pin
      #define PORTD_pCLR    GPIOC_PCOR  = msk_pin
      #define PORTD_qSET   (GPIOC_PDIR & msk_pin)
                              
      #define PORTE_pCLK    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK
      #define PORTE_pOUT    GPIOC_PDDR  |= msk_pin
      #define PORTE_pINP    GPIOC_PDDR  &= ~msk_pin
      #define PORTE_pSET    GPIOC_PSOR  = msk_pin
      #define PORTE_pCLR    GPIOC_PCOR  = msk_pin
      #define PORTE_qSET   (GPIOC_PDIR & msk_pin)

      #define PORTF_pCLK    SIM_SCGC5 |= SIM_SCGC5_PORTF_MASK
      #define PORTF_pOUT    GPIOC_PDDR  |= msk_pin
      #define PORTF_pINP    GPIOC_PDDR  &= ~msk_pin
      #define PORTF_pSET    GPIOC_PSOR  = msk_pin
      #define PORTF_pCLR    GPIOC_PCOR  = msk_pin
      #define PORTF_qSET   (GPIOC_PDIR & msk_pin)
      
      /* PORTA CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
                        
      INT16 CTS_ARM7M4T2_PORTA_HS (INT32 * ptr_port, INT32 msk_pin, INT16 CTS_arg)
      {
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTA_pSET;
        #endif
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm("PUSH   {r4} \n"            /* Save r4 */
              "MOV    r4,#(0x00) \n"
              "STR    r4,[sp,#(4*-2)] \n" /* Put ExISR mark to SP -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
              "POP    {r4}"               /* Recover r4 */
             );   
        #endif
        
        /* port init */
        PORTA_pCLK;
        (*ptr_port) = (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK);
        PORTA_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTA_qSET) return cCTS_ERROR_LoPIN;
        PORTA_pCLR;
        PORTA_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTA_qSET) return cCTS_ERROR_HiPIN;
        PORTA_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        if (CTS_arg) {
          pPEon;
          if (0 < CTS_arg) {
            #if 1==CTS_KINETIS_ASM_LOOPS
              asm("PORTA_TURBO_DEC: SUBS r2,r2,#1 \n SXTB r2,r2 \n CMP r2,#0 \n BNE.N PORTA_TURBO_DEC"); /* turbo loop = 7 */
            #else
              while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            #endif
            pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTA_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
            asm("PUSH   {r4} \n"              /* Save r4 */
                "LDR    r4,[sp,#(4*-2)] \n"   /* Load ExISR_pos mark from -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
                "CBZ    r4, PORTA_ExISR_UNDETECTED"); /* Check if mark was changed by interrupt */
          #if defined(__CWCC__)               /* If CodeWarrior used */
            asm("MOV    CTS_arg,#(-1)");      /* CTS_arg = cCTS_ERROR_ExISR; */
          #else 
            asm("MOV    r2,#(-1)");           /* r2 = CTS_arg = cCTS_ERROR_ExISR; */            
          #endif
            asm("PORTA_ExISR_UNDETECTED: \n"
                "POP   {r4}");                /* Recover r4 */
        #endif
        
        /* disable pull-up */
        pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTA_pCLR;
          PORTA_pOUT;
        #else
          PORTA_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      
      INT16 CTS_A0HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR0,  (1 <<  0), CTS_arg); }
      INT16 CTS_A1HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR1,  (1 <<  1), CTS_arg); }
      INT16 CTS_A2HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR2,  (1 <<  2), CTS_arg); }
      INT16 CTS_A3HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR3,  (1 <<  3), CTS_arg); }
      INT16 CTS_A4HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR4,  (1 <<  4), CTS_arg); }
      INT16 CTS_A5HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR5,  (1 <<  5), CTS_arg); }
      INT16 CTS_A6HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR6,  (1 <<  6), CTS_arg); }
      INT16 CTS_A7HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR7,  (1 <<  7), CTS_arg); }
      INT16 CTS_A8HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR8,  (1 <<  8), CTS_arg); }
      INT16 CTS_A9HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR9,  (1 <<  9), CTS_arg); }
      INT16 CTS_A10HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR10, (1 << 10), CTS_arg); }
      INT16 CTS_A11HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR11, (1 << 11), CTS_arg); }
      INT16 CTS_A12HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR12, (1 << 12), CTS_arg); }
      INT16 CTS_A13HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR13, (1 << 13), CTS_arg); }
      INT16 CTS_A14HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR14, (1 << 14), CTS_arg); }
      INT16 CTS_A15HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR15, (1 << 15), CTS_arg); }
      INT16 CTS_A16HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR16, (1 << 16), CTS_arg); }
      INT16 CTS_A17HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR17, (1 << 17), CTS_arg); }
      INT16 CTS_A18HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR18, (1 << 18), CTS_arg); }
      INT16 CTS_A19HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR19, (1 << 19), CTS_arg); }
      INT16 CTS_A20HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR20, (1 << 20), CTS_arg); }
      INT16 CTS_A21HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR21, (1 << 21), CTS_arg); }
      INT16 CTS_A22HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR22, (1 << 22), CTS_arg); }
      INT16 CTS_A23HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR23, (1 << 23), CTS_arg); }
      INT16 CTS_A24HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR24, (1 << 24), CTS_arg); }
      INT16 CTS_A25HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR25, (1 << 25), CTS_arg); }
      INT16 CTS_A26HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR26, (1 << 26), CTS_arg); }
      INT16 CTS_A27HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR27, (1 << 27), CTS_arg); }
      INT16 CTS_A28HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR28, (1 << 28), CTS_arg); }
      INT16 CTS_A29HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR29, (1 << 29), CTS_arg); }
      INT16 CTS_A30HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR30, (1 << 30), CTS_arg); }
      INT16 CTS_A31HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTA_HS ((INT32*)&PORTA_PCR31, (1 << 31), CTS_arg); }

      /* PORTB CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
      #pragma optimization_level 0
      #else
      #pragma optimize=none
      #endif
      
      INT16 CTS_ARM7M4T2_PORTB_HS (INT32 * ptr_port, INT32 msk_pin, INT16 CTS_arg)
      {
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTB_pSET;
        #endif
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm("PUSH   {r4} \n"            /* Save r4 */
              "MOV    r4,#(0x00) \n"
              "STR    r4,[sp,#(4*-2)] \n" /* Put ExISR mark to SP -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
              "POP    {r4}"               /* Recover r4 */
             );   
        #endif

        /* port init */
        PORTB_pCLK;
        (*ptr_port) = (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK);
        PORTB_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTB_qSET) return cCTS_ERROR_LoPIN;
        PORTB_pCLR;
        PORTB_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTB_qSET) return cCTS_ERROR_HiPIN;
        PORTB_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        if (CTS_arg) {
          pPEon;
          if (0 < CTS_arg) {
            #if 1==CTS_KINETIS_ASM_LOOPS
              asm("PORTB_TURBO_DEC: SUBS r2,r2,#1 \n SXTB r2,r2 \n CMP r2,#0 \n BNE.N PORTB_TURBO_DEC"); /* turbo loop = 7 */
            #else
              while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            #endif
            pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTB_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
            asm("PUSH   {r4} \n"              /* Save r4 */
                "LDR    r4,[sp,#(4*-2)] \n"   /* Load ExISR_pos mark from -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
                "CBZ    r4, PORTB_ExISR_UNDETECTED"); /* Check if mark was changed by interrupt */
          #if defined(__CWCC__)               /* If CodeWarrior used */
            asm("MOV    CTS_arg,#(-1)");      /* CTS_arg = cCTS_ERROR_ExISR; */
          #else 
            asm("MOV    r2,#(-1)");           /* r2 = CTS_arg = cCTS_ERROR_ExISR; */            
          #endif
            asm("PORTB_ExISR_UNDETECTED: \n"
                "POP   {r4}");                /* Recover r4 */
        #endif
        
        /* disable pull-up */
        pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTB_pCLR;
          PORTB_pOUT;
        #else
          PORTB_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      
      INT16 CTS_B0HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR0,  (1 <<  0), CTS_arg); }
      INT16 CTS_B1HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR1,  (1 <<  1), CTS_arg); }
      INT16 CTS_B2HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR2,  (1 <<  2), CTS_arg); }
      INT16 CTS_B3HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR3,  (1 <<  3), CTS_arg); }
      INT16 CTS_B4HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR4,  (1 <<  4), CTS_arg); }
      INT16 CTS_B5HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR5,  (1 <<  5), CTS_arg); }
      INT16 CTS_B6HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR6,  (1 <<  6), CTS_arg); }
      INT16 CTS_B7HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR7,  (1 <<  7), CTS_arg); }
      INT16 CTS_B8HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR8,  (1 <<  8), CTS_arg); }
      INT16 CTS_B9HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR9,  (1 <<  9), CTS_arg); }
      INT16 CTS_B10HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR10, (1 << 10), CTS_arg); }
      INT16 CTS_B11HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR11, (1 << 11), CTS_arg); }
      INT16 CTS_B12HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR12, (1 << 12), CTS_arg); }
      INT16 CTS_B13HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR13, (1 << 13), CTS_arg); }
      INT16 CTS_B14HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR14, (1 << 14), CTS_arg); }
      INT16 CTS_B15HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR15, (1 << 15), CTS_arg); }
      INT16 CTS_B16HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR16, (1 << 16), CTS_arg); }
      INT16 CTS_B17HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR17, (1 << 17), CTS_arg); }
      INT16 CTS_B18HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR18, (1 << 18), CTS_arg); }
      INT16 CTS_B19HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR19, (1 << 19), CTS_arg); }
      INT16 CTS_B20HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR20, (1 << 20), CTS_arg); }
      INT16 CTS_B21HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR21, (1 << 21), CTS_arg); }
      INT16 CTS_B22HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR22, (1 << 22), CTS_arg); }
      INT16 CTS_B23HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR23, (1 << 23), CTS_arg); }
      INT16 CTS_B24HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR24, (1 << 24), CTS_arg); }
      INT16 CTS_B25HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR25, (1 << 25), CTS_arg); }
      INT16 CTS_B26HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR26, (1 << 26), CTS_arg); }
      INT16 CTS_B27HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR27, (1 << 27), CTS_arg); }
      INT16 CTS_B28HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR28, (1 << 28), CTS_arg); }
      INT16 CTS_B29HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR29, (1 << 29), CTS_arg); }
      INT16 CTS_B30HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR30, (1 << 30), CTS_arg); }
      INT16 CTS_B31HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTB_HS ((INT32*)&PORTB_PCR31, (1 << 31), CTS_arg); }
      
      /* PORTC CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      INT16 CTS_ARM7M4T2_PORTC_HS (INT32 * ptr_port, INT32 msk_pin, INT16 CTS_arg)
      {
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTC_pSET;
        #endif

        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm("PUSH   {r4} \n"            /* Save r4 */            
              "MOV    r4,#(0x00) \n"
              "STR    r4,[sp,#(4*-2)] \n" /* Put ExISR mark to SP -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
              "POP    {r4}"               /* Recover r4 */
             );   
        #endif
        
        /* port init */
        PORTC_pCLK;
        (*ptr_port) = (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK);
        PORTC_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTC_qSET) return cCTS_ERROR_LoPIN;
        PORTC_pCLR;
        PORTC_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTC_qSET) return cCTS_ERROR_HiPIN;
        PORTC_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        if (CTS_arg) {
          pPEon;
          if (0 < CTS_arg) {
            #if 1==CTS_KINETIS_ASM_LOOPS
              asm("PORTC_TURBO_DEC: SUBS r2,r2,#1 \n SXTB r2,r2 \n CMP r2,#0 \n BNE.N PORTC_TURBO_DEC"); /* turbo loop = 7 */
            #else
              while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            #endif
            pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTC_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
            asm("PUSH   {r4} \n"              /* Save r4 */
                "LDR    r4,[sp,#(4*-2)] \n"   /* Load ExISR_pos mark from -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
                "CBZ    r4, PORTC_ExISR_UNDETECTED"); /* Check if mark was changed by interrupt */
          #if defined(__CWCC__)               /* If CodeWarrior used */
            asm("MOV    CTS_arg,#(-1)");      /* CTS_arg = cCTS_ERROR_ExISR; */
          #else 
            asm("MOV    r2,#(-1)");           /* r2 = CTS_arg = cCTS_ERROR_ExISR; */            
          #endif
            asm("PORTC_ExISR_UNDETECTED: \n"
                "POP   {r4}");                /* Recover r4 */
        #endif
        
        /* disable pull-up */
        pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTC_pCLR;
          PORTC_pOUT;
        #else
          PORTC_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      
      INT16 CTS_C0HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR0,  (1 <<  0), CTS_arg); }
      INT16 CTS_C1HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR1,  (1 <<  1), CTS_arg); }
      INT16 CTS_C2HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR2,  (1 <<  2), CTS_arg); }
      INT16 CTS_C3HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR3,  (1 <<  3), CTS_arg); }
      INT16 CTS_C4HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR4,  (1 <<  4), CTS_arg); }
      INT16 CTS_C5HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR5,  (1 <<  5), CTS_arg); }
      INT16 CTS_C6HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR6,  (1 <<  6), CTS_arg); }
      INT16 CTS_C7HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR7,  (1 <<  7), CTS_arg); }
      INT16 CTS_C8HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR8,  (1 <<  8), CTS_arg); }
      INT16 CTS_C9HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR9,  (1 <<  9), CTS_arg); }
      INT16 CTS_C10HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR10, (1 << 10), CTS_arg); }
      INT16 CTS_C11HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR11, (1 << 11), CTS_arg); }
      INT16 CTS_C12HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR12, (1 << 12), CTS_arg); }
      INT16 CTS_C13HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR13, (1 << 13), CTS_arg); }
      INT16 CTS_C14HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR14, (1 << 14), CTS_arg); }
      INT16 CTS_C15HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR15, (1 << 15), CTS_arg); }
      INT16 CTS_C16HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR16, (1 << 16), CTS_arg); }
      INT16 CTS_C17HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR17, (1 << 17), CTS_arg); }
      INT16 CTS_C18HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR18, (1 << 18), CTS_arg); }
      INT16 CTS_C19HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR19, (1 << 19), CTS_arg); }
      INT16 CTS_C20HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR20, (1 << 20), CTS_arg); }
      INT16 CTS_C21HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR21, (1 << 21), CTS_arg); }
      INT16 CTS_C22HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR22, (1 << 22), CTS_arg); }
      INT16 CTS_C23HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR23, (1 << 23), CTS_arg); }
      INT16 CTS_C24HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR24, (1 << 24), CTS_arg); }
      INT16 CTS_C25HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR25, (1 << 25), CTS_arg); }
      INT16 CTS_C26HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR26, (1 << 26), CTS_arg); }
      INT16 CTS_C27HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR27, (1 << 27), CTS_arg); }
      INT16 CTS_C28HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR28, (1 << 28), CTS_arg); }
      INT16 CTS_C29HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR29, (1 << 29), CTS_arg); }
      INT16 CTS_C30HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR30, (1 << 30), CTS_arg); }
      INT16 CTS_C31HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTC_HS ((INT32*)&PORTC_PCR31, (1 << 31), CTS_arg); }
      
      /* PORTD CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      INT16 CTS_ARM7M4T2_PORTD_HS (INT32 * ptr_port, INT32 msk_pin, INT16 CTS_arg)
      {
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTD_pSET;
        #endif

        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm("PUSH   {r4} \n"            /* Save r4 */            
              "MOV    r4,#(0x00) \n"
              "STR    r4,[sp,#(4*-2)] \n" /* Put ExISR mark to SP -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
              "POP    {r4}"               /* Recover r4 */
             );   
        #endif
        
        /* port init */
        PORTD_pCLK;
        (*ptr_port) = (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK);
        PORTD_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTD_qSET) return cCTS_ERROR_LoPIN;
        PORTD_pCLR;
        PORTD_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTD_qSET) return cCTS_ERROR_HiPIN;
        PORTD_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        if (CTS_arg) {
          pPEon;
          if (0 < CTS_arg) {
            #if 1==CTS_KINETIS_ASM_LOOPS
              asm("PORTD_TURBO_DEC: SUBS r2,r2,#1 \n SXTB r2,r2 \n CMP r2,#0 \n BNE.N PORTD_TURBO_DEC"); /* turbo loop = 7 */
            #else
              while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            #endif
            pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTD_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
            asm("PUSH   {r4} \n"              /* Save r4 */
                "LDR    r4,[sp,#(4*-2)] \n"   /* Load ExISR_pos mark from -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
                "CBZ    r4, PORTD_ExISR_UNDETECTED"); /* Check if mark was changed by interrupt */
          #if defined(__CWCC__)               /* If CodeWarrior used */
            asm("MOV    CTS_arg,#(-1)");      /* CTS_arg = cCTS_ERROR_ExISR; */
          #else 
            asm("MOV    r2,#(-1)");           /* r2 = CTS_arg = cCTS_ERROR_ExISR; */            
          #endif
            asm("PORTD_ExISR_UNDETECTED: \n"
                "POP   {r4}");                /* Recover r4 */
        #endif
        
        /* disable pull-up */
        pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTD_pCLR;
          PORTD_pOUT;
        #else
          PORTD_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      
      INT16 CTS_D0HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR0,  (1 <<  0), CTS_arg); }
      INT16 CTS_D1HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR1,  (1 <<  1), CTS_arg); }
      INT16 CTS_D2HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR2,  (1 <<  2), CTS_arg); }
      INT16 CTS_D3HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR3,  (1 <<  3), CTS_arg); }
      INT16 CTS_D4HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR4,  (1 <<  4), CTS_arg); }
      INT16 CTS_D5HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR5,  (1 <<  5), CTS_arg); }
      INT16 CTS_D6HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR6,  (1 <<  6), CTS_arg); }
      INT16 CTS_D7HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR7,  (1 <<  7), CTS_arg); }
      INT16 CTS_D8HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR8,  (1 <<  8), CTS_arg); }
      INT16 CTS_D9HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR9,  (1 <<  9), CTS_arg); }
      INT16 CTS_D10HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR10, (1 << 10), CTS_arg); }
      INT16 CTS_D11HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR11, (1 << 11), CTS_arg); }
      INT16 CTS_D12HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR12, (1 << 12), CTS_arg); }
      INT16 CTS_D13HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR13, (1 << 13), CTS_arg); }
      INT16 CTS_D14HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR14, (1 << 14), CTS_arg); }
      INT16 CTS_D15HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR15, (1 << 15), CTS_arg); }
      INT16 CTS_D16HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR16, (1 << 16), CTS_arg); }
      INT16 CTS_D17HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR17, (1 << 17), CTS_arg); }
      INT16 CTS_D18HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR18, (1 << 18), CTS_arg); }
      INT16 CTS_D19HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR19, (1 << 19), CTS_arg); }
      INT16 CTS_D20HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR20, (1 << 20), CTS_arg); }
      INT16 CTS_D21HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR21, (1 << 21), CTS_arg); }
      INT16 CTS_D22HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR22, (1 << 22), CTS_arg); }
      INT16 CTS_D23HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR23, (1 << 23), CTS_arg); }
      INT16 CTS_D24HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR24, (1 << 24), CTS_arg); }
      INT16 CTS_D25HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR25, (1 << 25), CTS_arg); }
      INT16 CTS_D26HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR26, (1 << 26), CTS_arg); }
      INT16 CTS_D27HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR27, (1 << 27), CTS_arg); }
      INT16 CTS_D28HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR28, (1 << 28), CTS_arg); }
      INT16 CTS_D29HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR29, (1 << 29), CTS_arg); }
      INT16 CTS_D30HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR30, (1 << 30), CTS_arg); }
      INT16 CTS_D31HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTD_HS ((INT32*)&PORTD_PCR31, (1 << 31), CTS_arg); }
      
      /* PORTE CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      INT16 CTS_ARM7M4T2_PORTE_HS (INT32 * ptr_port, INT32 msk_pin, INT16 CTS_arg)
      {
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTE_pSET;
        #endif

        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm("PUSH   {r4} \n"            /* Save r4 */
              "MOV    r4,#(0x00) \n"
              "STR    r4,[sp,#(4*-2)] \n" /* Put ExISR mark to SP -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
              "POP    {r4}"               /* Recover r4 */
             );   
        #endif
        
        /* port init */
        PORTE_pCLK;
        (*ptr_port) = (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK);
        PORTE_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTE_qSET) return cCTS_ERROR_LoPIN;
        PORTE_pCLR;
        PORTE_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTE_qSET) return cCTS_ERROR_HiPIN;
        PORTE_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        if (CTS_arg) {
          pPEon;
          if (0 < CTS_arg) {
            #if 1==CTS_KINETIS_ASM_LOOPS
              asm("PORTE_TURBO_DEC: SUBS r2,r2,#1 \n SXTB r2,r2 \n CMP r2,#0 \n BNE.N PORTE_TURBO_DEC"); /* turbo loop = 7 */
            #else
              while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            #endif
            pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTE_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
            asm("PUSH   {r4} \n"              /* Save r4 */
                "LDR    r4,[sp,#(4*-2)] \n"   /* Load ExISR_pos mark from -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
                "CBZ    r4, PORTE_ExISR_UNDETECTED"); /* Check if mark was changed by interrupt */
          #if defined(__CWCC__)               /* If CodeWarrior used */
            asm("MOV    CTS_arg,#(-1)");      /* CTS_arg = cCTS_ERROR_ExISR; */
          #else 
            asm("MOV    r2,#(-1)");           /* r2 = CTS_arg = cCTS_ERROR_ExISR; */            
          #endif
            asm("PORTE_ExISR_UNDETECTED: \n"
                "POP   {r4}");                /* Recover r4 */
        #endif
        
        /* disable pull-up */
        pPEoff;
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTE_pCLR;
          PORTE_pOUT;          
        #else
          PORTE_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      
      INT16 CTS_E0HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR0,  (1 <<  0), CTS_arg); }
      INT16 CTS_E1HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR1,  (1 <<  1), CTS_arg); }
      INT16 CTS_E2HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR2,  (1 <<  2), CTS_arg); }
      INT16 CTS_E3HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR3,  (1 <<  3), CTS_arg); }
      INT16 CTS_E4HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR4,  (1 <<  4), CTS_arg); }
      INT16 CTS_E5HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR5,  (1 <<  5), CTS_arg); }
      INT16 CTS_E6HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR6,  (1 <<  6), CTS_arg); }
      INT16 CTS_E7HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR7,  (1 <<  7), CTS_arg); }
      INT16 CTS_E8HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR8,  (1 <<  8), CTS_arg); }
      INT16 CTS_E9HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR9,  (1 <<  9), CTS_arg); }
      INT16 CTS_E10HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR10, (1 << 10), CTS_arg); }
      INT16 CTS_E11HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR11, (1 << 11), CTS_arg); }
      INT16 CTS_E12HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR12, (1 << 12), CTS_arg); }
      INT16 CTS_E13HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR13, (1 << 13), CTS_arg); }
      INT16 CTS_E14HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR14, (1 << 14), CTS_arg); }
      INT16 CTS_E15HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR15, (1 << 15), CTS_arg); }
      INT16 CTS_E16HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR16, (1 << 16), CTS_arg); }
      INT16 CTS_E17HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR17, (1 << 17), CTS_arg); }
      INT16 CTS_E18HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR18, (1 << 18), CTS_arg); }
      INT16 CTS_E19HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR19, (1 << 19), CTS_arg); }
      INT16 CTS_E20HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR20, (1 << 20), CTS_arg); }
      INT16 CTS_E21HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR21, (1 << 21), CTS_arg); }
      INT16 CTS_E22HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR22, (1 << 22), CTS_arg); }
      INT16 CTS_E23HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR23, (1 << 23), CTS_arg); }
      INT16 CTS_E24HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR24, (1 << 24), CTS_arg); }
      INT16 CTS_E25HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR25, (1 << 25), CTS_arg); }
      INT16 CTS_E26HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR26, (1 << 26), CTS_arg); }
      INT16 CTS_E27HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR27, (1 << 27), CTS_arg); }
      INT16 CTS_E28HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR28, (1 << 28), CTS_arg); }
      INT16 CTS_E29HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR29, (1 << 29), CTS_arg); }
      INT16 CTS_E30HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR30, (1 << 30), CTS_arg); }
      INT16 CTS_E31HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTE_HS ((INT32*)&PORTE_PCR31, (1 << 31), CTS_arg); }

      #ifdef PORTF_PCR0
      
  		  /* PORTF CTS sensor */
  		  #if defined(__CWCC__)  /* If Code Warrior used */ 
  			#pragma optimization_level 0
  		  #else
  			#pragma optimize=none
  		  #endif
  		  
  		  INT16 CTS_ARM7M4T2_PORTF_HS (INT32 * ptr_port, INT32 msk_pin, INT16 CTS_arg)
  		  {
  			#if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
  			  PORTF_pSET;
  			#endif
  	
  			if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
  			
  			/* ExISR_INIT */
  			#if 1==ExISR_ENABLED
  			  asm("PUSH   {r4} \n"            /* Save r4 */
  				  "MOV    r4,#(0x00) \n"
  				  "STR    r4,[sp,#(4*-2)] \n" /* Put ExISR mark to SP -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
  				  "POP    {r4}"               /* Recover r4 */
  				 );   
  			#endif
  			
  			/* port init */
  			PORTF_pCLK;
  			(*ptr_port) = (PORT_PCR_MUX(1) | PORT_PCR_PS_MASK);
  			PORTF_pINP;
  			if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
  		  
  			/* HiTest -> set Low */
  			if (!PORTF_qSET) return cCTS_ERROR_LoPIN;
  			PORTF_pCLR;
  			PORTF_pOUT;
  		  
  			/* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
  			asm("nop \n nop \n nop \n nop \n nop");
  			
  			/* LoTest */
  			if (PORTF_qSET) return cCTS_ERROR_HiPIN;
  			PORTF_pINP;
  			
  			/* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
  			if (CTS_arg) {
  			  pPEon;
  			  if (0 < CTS_arg) {
  				#if 1==CTS_KINETIS_ASM_LOOPS
  				  asm("PORTF_TURBO_DEC: SUBS r2,r2,#1 \n SXTB r2,r2 \n CMP r2,#0 \n BNE.N PORTF_TURBO_DEC"); /* turbo loop = 7 */
  				#else
  				  while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
  				#endif
  				pPEoff;
  			  } else CTS_arg = 0;
  			}
  			
  			/* Sens-loop waits for LOG1 */
  			while ((!PORTF_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
  		  
  			/* ExISR_Test -> return -1; */
  			#if 1==ExISR_ENABLED
  				asm("PUSH   {r4} \n"              /* Save r4 */
  					"LDR    r4,[sp,#(4*-2)] \n"   /* Load ExISR_pos mark from -4*2 (Originally -4*3 but PUSH {r4} cause offset 4) */
  					"CBZ    r4, PORTF_ExISR_UNDETECTED"); /* Check if mark was changed by interrupt */
  			  #if defined(__CWCC__)               /* If CodeWarrior used */
  				asm("MOV    CTS_arg,#(-1)");      /* CTS_arg = cCTS_ERROR_ExISR; */
  			  #else 
  				asm("MOV    r2,#(-1)");           /* r2 = CTS_arg = cCTS_ERROR_ExISR; */            
  			  #endif
  				asm("PORTF_ExISR_UNDETECTED: \n"
  					"POP   {r4}");                /* Recover r4 */
  			#endif
  			
  			/* disable pull-up */
  			pPEoff;
  			/* Set Default PIN State */
  			#if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
  			  PORTF_pCLR;
  			  PORTF_pOUT;          
  			#else
  			  PORTF_pINP;
  			#endif
  		  
  			/* ret */
  			return CTS_arg;
  		  }
        
  		  INT16 CTS_F0HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR0,  (1 <<  0), CTS_arg); }
  		  INT16 CTS_F1HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR1,  (1 <<  1), CTS_arg); }
  		  INT16 CTS_F2HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR2,  (1 <<  2), CTS_arg); }
  		  INT16 CTS_F3HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR3,  (1 <<  3), CTS_arg); }
  		  INT16 CTS_F4HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR4,  (1 <<  4), CTS_arg); }
  		  INT16 CTS_F5HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR5,  (1 <<  5), CTS_arg); }
  		  INT16 CTS_F6HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR6,  (1 <<  6), CTS_arg); }
  		  INT16 CTS_F7HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR7,  (1 <<  7), CTS_arg); }
  		  INT16 CTS_F8HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR8,  (1 <<  8), CTS_arg); }
  		  INT16 CTS_F9HS  (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR9,  (1 <<  9), CTS_arg); }
  		  INT16 CTS_F10HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR10, (1 << 10), CTS_arg); }
  		  INT16 CTS_F11HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR11, (1 << 11), CTS_arg); }
  		  INT16 CTS_F12HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR12, (1 << 12), CTS_arg); }
  		  INT16 CTS_F13HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR13, (1 << 13), CTS_arg); }
  		  INT16 CTS_F14HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR14, (1 << 14), CTS_arg); }
  		  INT16 CTS_F15HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR15, (1 << 15), CTS_arg); }
  		  INT16 CTS_F16HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR16, (1 << 16), CTS_arg); }
  		  INT16 CTS_F17HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR17, (1 << 17), CTS_arg); }
  		  INT16 CTS_F18HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR18, (1 << 18), CTS_arg); }
  		  INT16 CTS_F19HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR19, (1 << 19), CTS_arg); }
  		  INT16 CTS_F20HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR20, (1 << 20), CTS_arg); }
  		  INT16 CTS_F21HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR21, (1 << 21), CTS_arg); }
  		  INT16 CTS_F22HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR22, (1 << 22), CTS_arg); }
  		  INT16 CTS_F23HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR23, (1 << 23), CTS_arg); }
  		  INT16 CTS_F24HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR24, (1 << 24), CTS_arg); }
  		  INT16 CTS_F25HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR25, (1 << 25), CTS_arg); }
  		  INT16 CTS_F26HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR26, (1 << 26), CTS_arg); }
  		  INT16 CTS_F27HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR27, (1 << 27), CTS_arg); }
  		  INT16 CTS_F28HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR28, (1 << 28), CTS_arg); }
  		  INT16 CTS_F29HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR29, (1 << 29), CTS_arg); }
  		  INT16 CTS_F30HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR30, (1 << 30), CTS_arg); }
  		  INT16 CTS_F31HS (INT16 CTS_arg) { return CTS_ARM7M4T2_PORTF_HS ((INT32*)&PORTF_PCR31, (1 << 31), CTS_arg); }
		  
      #endif
      
    #elif TSS_CFM_MCU  /* End of TSS_KINETIS_MCU */
      
      /*************************** Prototypes ******************************/
      
      INT16 CTS_CFM_PORTA_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg);
      INT16 CTS_CFM_PORTB_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg);
      INT16 CTS_CFM_PORTC_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg);
      INT16 CTS_CFM_PORTD_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg);
      INT16 CTS_CFM_PORTE_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg);
      INT16 CTS_CFM_PORTF_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg);
      
      /***************************** Macros ********************************/
      
      #define PORTA_pCLK    SIM_SCGC6 |= SIM_SCGC6_PORTA_MASK  /* enable bus-clock */
      #define PORTA_pPEon   PCTLA_PUS |= msk_pin; PCTLA_PUE |= msk_pin /* Select pull-up, Enable pull-up */
      #define PORTA_pPEoff  PCTLA_PUE &= ~msk_pin /* Disable pull-up */
      #define PORTA_pOUT    PTA_DD  |= msk_pin  /* out */
      #define PORTA_pINP    PTA_DD  &= ~msk_pin /* inp */
      #define PORTA_pSET    PTA_D  |= msk_pin   /* set */
      #define PORTA_pCLR    PTA_D  &= ~msk_pin  /* clr */
      #define PORTA_qSET   (PTA_PV & msk_pin)   /* is set? */
      
      #define PORTB_pCLK    SIM_SCGC6 |= SIM_SCGC6_PORTB_MASK  /* enable bus-clock */
      #define PORTB_pPEon   PCTLB_PUS |= msk_pin; PCTLB_PUE |= msk_pin /* Select pull-up, Enable pull-up */
      #define PORTB_pPEoff  PCTLB_PUE &= ~msk_pin /* Disable pull-up */
      #define PORTB_pOUT    PTB_DD  |= msk_pin  /* out */
      #define PORTB_pINP    PTB_DD  &= ~msk_pin /* inp */
      #define PORTB_pSET    PTB_D  |= msk_pin   /* set */
      #define PORTB_pCLR    PTB_D  &= ~msk_pin  /* clr */
      #define PORTB_qSET   (PTB_PV & msk_pin)   /* is set? */
      
      #define PORTC_pCLK    SIM_SCGC6 |= SIM_SCGC6_PORTC_MASK  /* enable bus-clock */
      #define PORTC_pPEon   PCTLC_PUS |= msk_pin; PCTLC_PUE |= msk_pin /* Select pull-up, Enable pull-up */
      #define PORTC_pPEoff  PCTLC_PUE &= ~msk_pin /* Disable pull-up */
      #define PORTC_pOUT    PTC_DD  |= msk_pin  /* out */
      #define PORTC_pINP    PTC_DD  &= ~msk_pin /* inp */
      #define PORTC_pSET    PTC_D  |= msk_pin   /* set */
      #define PORTC_pCLR    PTC_D  &= ~msk_pin  /* clr */
      #define PORTC_qSET   (PTC_PV & msk_pin)   /* is set? */
      
      #define PORTD_pCLK    SIM_SCGC6 |= SIM_SCGC6_PORTD_MASK  /* enable bus-clock */
      #define PORTD_pPEon   PCTLD_PUS |= msk_pin; PCTLD_PUE |= msk_pin /* Select pull-up, Enable pull-up */
      #define PORTD_pPEoff  PCTLD_PUE &= ~msk_pin /* Disable pull-up */
      #define PORTD_pOUT    PTD_DD  |= msk_pin  /* out */
      #define PORTD_pINP    PTD_DD  &= ~msk_pin /* inp */
      #define PORTD_pSET    PTD_D  |= msk_pin   /* set */
      #define PORTD_pCLR    PTD_D  &= ~msk_pin  /* clr */
      #define PORTD_qSET   (PTD_PV & msk_pin)   /* is set? */
      
      #define PORTE_pCLK    SIM_SCGC6 |= SIM_SCGC6_PORTE_MASK  /* enable bus-clock */
      #define PORTE_pPEon   PCTLE_PUS |= msk_pin; PCTLE_PUE |= msk_pin /* Select pull-up, Enable pull-up */
      #define PORTE_pPEoff  PCTLE_PUE &= ~msk_pin /* Disable pull-up */
      #define PORTE_pOUT    PTE_DD  |= msk_pin  /* out */
      #define PORTE_pINP    PTE_DD  &= ~msk_pin /* inp */
      #define PORTE_pSET    PTE_D  |= msk_pin   /* set */
      #define PORTE_pCLR    PTE_D  &= ~msk_pin  /* clr */
      #define PORTE_qSET   (PTE_PV & msk_pin)   /* is set? */
      
      #define PORTF_pCLK    SIM_SCGC6 |= SIM_SCGC6_PORTF_MASK  /* enable bus-clock */
      #define PORTF_pPEon   PCTLF_PUS |= msk_pin; PCTLF_PUE |= msk_pin /* Select pull-up, Enable pull-up */
      #define PORTF_pPEoff  PCTLF_PUE &= ~msk_pin /* Disable pull-up */
      #define PORTF_pOUT    PTF_DD  |= msk_pin  /* out */
      #define PORTF_pINP    PTF_DD  &= ~msk_pin /* inp */
      #define PORTF_pSET    PTF_D  |= msk_pin   /* set */
      #define PORTF_pCLR    PTF_D  &= ~msk_pin  /* clr */
      #define PORTF_qSET   (PTF_PV & msk_pin)   /* is set? */
      
      /* PORTA CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      #ifdef SIM_SCGC6_PORTA_MASK
      INT16 CTS_CFM_PORTA_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg)
      {
        UINT8 msk_pin;

        msk_pin = 1 << pin_num;
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm{ BSET %5,-5(sp); }
        #endif
        
        /* port init */
        PORTA_pCLK;
        if (pin_num % 2u)
        {	  
          (*ptr_port) &= 0x0Fu; /* Erase PCR */
          (*ptr_port) |= 0x10u; /* Set GPIO function for PIN */
        } else {
          (*ptr_port) &= 0xF0u; /* Erase PCR */
          (*ptr_port) |= 0x01u; /* Set GPIO function for PIN */
        }
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTA_pSET;
        #endif
        PORTA_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTA_qSET) return cCTS_ERROR_LoPIN;
        PORTA_pCLR;
        PORTA_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTA_qSET) return cCTS_ERROR_HiPIN;
        PORTA_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        #if defined(__CWCC__)  /* If Code Warrior used */ 
          #pragma optimization_level 4
        #else
          #pragma optimize=speed
        #endif
        if (CTS_arg) {
              PORTA_pPEon;
              if (0 < CTS_arg) {
                while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
                PORTA_pPEoff;
              } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTA_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
          #if defined(__CWCC__)  /* If Code Warrior used */ 
            #pragma optimization_level 0
          #else
            #pragma optimize=none
          #endif
          asm {   BTST.B %5, -5(sp);
            BNE.B PORTA_ExISR_UNDETECTED;
            MOVE.W %-1, CTS_arg;
            PORTA_ExISR_UNDETECTED:
          }
        #endif
        
        /* disable pull-up */
        PORTA_pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTA_pCLR;
          PORTA_pOUT;          
        #else
          PORTA_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      #endif
      
      #ifdef MXC_PTAPF4
        INT16 CTS_A0HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF4, 0u, CTS_arg); }
        INT16 CTS_A1HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF4, 1u, CTS_arg); }
      #endif
      #ifdef MXC_PTAPF3
        INT16 CTS_A2HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF3, 2u, CTS_arg); }
        INT16 CTS_A3HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF3, 3u, CTS_arg); }
      #endif
      #ifdef MXC_PTAPF2
        INT16 CTS_A4HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF2, 4u, CTS_arg); }
        INT16 CTS_A5HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF2, 5u, CTS_arg); }
      #endif
      #ifdef MXC_PTAPF1
        INT16 CTS_A6HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF1, 6u, CTS_arg); }
        INT16 CTS_A7HS  (INT16 CTS_arg) { return CTS_CFM_PORTA_HS ((UINT8*)&MXC_PTAPF1, 7u, CTS_arg); }
      #endif
      
      /* PORTB CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      #ifdef SIM_SCGC6_PORTB_MASK
      INT16 CTS_CFM_PORTB_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg)
      {
        UINT8 msk_pin;

        msk_pin = 1 << pin_num;
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm{ BSET %5,-5(sp); }
        #endif
        
        /* port init */
        PORTB_pCLK;
        if (pin_num % 2u)
        {	  
          (*ptr_port) &= 0x0Fu; /* Erase PCR */
          (*ptr_port) |= 0x10u; /* Set GPIO function for PIN */
        } else {
          (*ptr_port) &= 0xF0u; /* Erase PCR */
          (*ptr_port) |= 0x01u; /* Set GPIO function for PIN */
        }
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTB_pSET;
        #endif
        PORTB_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTB_qSET) return cCTS_ERROR_LoPIN;
        PORTB_pCLR;
        PORTB_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTB_qSET) return cCTS_ERROR_HiPIN;
        PORTB_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        #if defined(__CWCC__)  /* If Code Warrior used */ 
          #pragma optimization_level 4
        #else
          #pragma optimize=speed
        #endif
        if (CTS_arg) {
          PORTB_pPEon;
          if (0 < CTS_arg) {
            while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            PORTB_pPEoff;
          } else CTS_arg = 0;
        }
        /* Sens-loop waits for LOG1 */
        while ((!PORTB_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
        
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
          #if defined(__CWCC__)  /* If Code Warrior used */ 
            #pragma optimization_level 0
          #else
            #pragma optimize=none
          #endif
          asm {   BTST.B %5, -5(sp);
            BNE.B PORTA_ExISR_UNDETECTED;
            MOVE.W %-1, CTS_arg;
            PORTA_ExISR_UNDETECTED:
          }
        #endif
        
        /* disable pull-up */
        PORTB_pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTB_pCLR;
          PORTB_pOUT;
        #else
          PORTB_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      #endif
      
      #ifdef MXC_PTBPF4
        INT16 CTS_B0HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF4, 0u, CTS_arg); }
        INT16 CTS_B1HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF4, 1u, CTS_arg); }
      #endif  
      #ifdef MXC_PTBPF3        
        INT16 CTS_B2HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF3, 2u, CTS_arg); }
        INT16 CTS_B3HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF3, 3u, CTS_arg); }
      #endif  
      #ifdef MXC_PTBPF2        
        INT16 CTS_B4HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF2, 4u, CTS_arg); }
        INT16 CTS_B5HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF2, 5u, CTS_arg); }
      #endif  
      #ifdef MXC_PTBPF1        
        INT16 CTS_B6HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF1, 6u, CTS_arg); }
        INT16 CTS_B7HS  (INT16 CTS_arg) { return CTS_CFM_PORTB_HS ((UINT8*)&MXC_PTBPF1, 7u, CTS_arg); }
      #endif  
      
      /* PORTC CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      #ifdef SIM_SCGC6_PORTC_MASK
      INT16 CTS_CFM_PORTC_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg)
      {
        UINT8 msk_pin;

        msk_pin = 1 << pin_num;
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm{ BSET %5,-5(sp); }
        #endif
        
        /* port init */
        PORTC_pCLK;
        if (pin_num % 2u)
        {	  
          (*ptr_port) &= 0x0Fu; /* Erase PCR */
          (*ptr_port) |= 0x10u; /* Set GPIO function for PIN */
        } else {
          (*ptr_port) &= 0xF0u; /* Erase PCR */
          (*ptr_port) |= 0x01u; /* Set GPIO function for PIN */
        }
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTC_pSET;
        #endif
        PORTC_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTC_qSET) return cCTS_ERROR_LoPIN;
        PORTC_pCLR;
        PORTC_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTC_qSET) return cCTS_ERROR_HiPIN;
        PORTC_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        #if defined(__CWCC__)  /* If Code Warrior used */ 
          #pragma optimization_level 4
        #else
          #pragma optimize=speed
        #endif
        if (CTS_arg) {
          PORTC_pPEon;
          if (0 < CTS_arg) {
            while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            PORTC_pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTC_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
          #if defined(__CWCC__)  /* If Code Warrior used */ 
            #pragma optimization_level 0
          #else
            #pragma optimize=none
          #endif
          asm {   BTST.B %5, -5(sp);
            BNE.B PORTA_ExISR_UNDETECTED;
            MOVE.W %-1, CTS_arg;
            PORTA_ExISR_UNDETECTED:
          }
        #endif
        
        /* disable pull-up */
        PORTC_pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTC_pCLR;
          PORTC_pOUT;
        #else
          PORTC_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      #endif
      
      #ifdef MXC_PTCPF4
        INT16 CTS_C0HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF4, 0u, CTS_arg); }
        INT16 CTS_C1HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF4, 1u, CTS_arg); }
      #endif
      #ifdef MXC_PTCPF3        
        INT16 CTS_C2HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF3, 2u, CTS_arg); }
        INT16 CTS_C3HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF3, 3u, CTS_arg); }
      #endif
      #ifdef MXC_PTCPF2        
        INT16 CTS_C4HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF2, 4u, CTS_arg); }
        INT16 CTS_C5HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF2, 5u, CTS_arg); }
      #endif
      #ifdef MXC_PTCPF1        
        INT16 CTS_C6HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF1, 6u, CTS_arg); }
        INT16 CTS_C7HS  (INT16 CTS_arg) { return CTS_CFM_PORTC_HS ((UINT8*)&MXC_PTCPF1, 7u, CTS_arg); }
      #endif
      
      /* PORTD CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
      
      #ifdef SIM_SCGC6_PORTD_MASK        
      INT16 CTS_CFM_PORTD_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg)
      {
        UINT8 msk_pin;

        msk_pin = 1 << pin_num;
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm{ BSET %5,-5(sp); } /* Put indication data to stack */
        #endif
        
        /* port init */
        PORTD_pCLK;
        if (pin_num % 2u)
        {	  
          (*ptr_port) &= 0x0Fu; /* Erase PCR */
          (*ptr_port) |= 0x10u; /* Set GPIO function for PIN */
        } else {
          (*ptr_port) &= 0xF0u; /* Erase PCR */
          (*ptr_port) |= 0x01u; /* Set GPIO function for PIN */
        }
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTD_pSET;
        #endif
        PORTD_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTD_qSET) return cCTS_ERROR_LoPIN;
        PORTD_pCLR;
        PORTD_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTD_qSET) return cCTS_ERROR_HiPIN;
        PORTD_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        #if defined(__CWCC__)  /* If Code Warrior used */ 
          #pragma optimization_level 4
        #else
          #pragma optimize=speed
        #endif
        if (CTS_arg) {
          PORTD_pPEon;
          if (0 < CTS_arg) {
            while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            PORTD_pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTD_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
          #if defined(__CWCC__)  /* If Code Warrior used */ 
            #pragma optimization_level 0
          #else
            #pragma optimize=none
          #endif
          asm {   BTST.B %5, -5(sp);
            BNE.B PORTA_ExISR_UNDETECTED;
            MOVE.W %-1, CTS_arg;
            PORTA_ExISR_UNDETECTED:
          }
        #endif
        
        /* disable pull-up */
        PORTD_pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTD_pCLR;
          PORTD_pOUT;
        #else
          PORTD_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      #endif
      
      #ifdef MXC_PTDPF4
        INT16 CTS_D0HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF4, 0u, CTS_arg); }
        INT16 CTS_D1HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF4, 1u, CTS_arg); }
      #endif
      #ifdef MXC_PTDPF3
        INT16 CTS_D2HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF3, 2u, CTS_arg); }
        INT16 CTS_D3HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF3, 3u, CTS_arg); }
      #endif
      #ifdef MXC_PTDPF2
        INT16 CTS_D4HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF2, 4u, CTS_arg); }
        INT16 CTS_D5HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF2, 5u, CTS_arg); }
      #endif
      #ifdef MXC_PTDPF1
        INT16 CTS_D6HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF1, 6u, CTS_arg); }
        INT16 CTS_D7HS  (INT16 CTS_arg) { return CTS_CFM_PORTD_HS ((UINT8*)&MXC_PTDPF1, 7u, CTS_arg); }
      #endif
      
      /* PORTE CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif
                        
      #ifdef SIM_SCGC6_PORTE_MASK
      INT16 CTS_CFM_PORTE_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg)
      {
        UINT8 msk_pin;

        msk_pin = 1 << pin_num;
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm{ BSET %5,-5(sp); } /* Put indication data to stack */
        #endif
        
        /* port init */
        PORTE_pCLK;
        if (pin_num % 2u)
        {	  
          (*ptr_port) &= 0x0Fu; /* Erase PCR */
          (*ptr_port) |= 0x10u; /* Set GPIO function for PIN */
        } else {
          (*ptr_port) &= 0xF0u; /* Erase PCR */
          (*ptr_port) |= 0x01u; /* Set GPIO function for PIN */
        }
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTE_pSET;
        #endif
        PORTE_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTE_qSET) return cCTS_ERROR_LoPIN;
        PORTE_pCLR;
        PORTE_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTE_qSET) return cCTS_ERROR_HiPIN;
        PORTE_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        #if defined(__CWCC__)  /* If Code Warrior used */ 
          #pragma optimization_level 4
        #else
          #pragma optimize=speed
        #endif
        if (CTS_arg) {
          PORTE_pPEon;
          if (0 < CTS_arg) {
            while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            PORTE_pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTE_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
          #if defined(__CWCC__)  /* If Code Warrior used */ 
            #pragma optimization_level 0
          #else
            #pragma optimize=none
          #endif
          asm {   BTST.B %5, -5(sp);
            BNE.B PORTA_ExISR_UNDETECTED;
            MOVE.W %-1, CTS_arg;
            PORTA_ExISR_UNDETECTED:
          }
        #endif
        
        /* disable pull-up */
        PORTE_pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTE_pCLR;
          PORTE_pOUT;
        #else
          PORTE_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      #endif
      
      #ifdef MXC_PTEPF4
        INT16 CTS_E0HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF4, 0u, CTS_arg); }
        INT16 CTS_E1HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF4, 1u, CTS_arg); }
      #endif
      #ifdef MXC_PTEPF3
        INT16 CTS_E2HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF3, 2u, CTS_arg); }
        INT16 CTS_E3HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF3, 3u, CTS_arg); }
      #endif
      #ifdef MXC_PTEPF2
        INT16 CTS_E4HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF2, 4u, CTS_arg); }
        INT16 CTS_E5HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF2, 5u, CTS_arg); }
      #endif
      #ifdef MXC_PTEPF1
        INT16 CTS_E6HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF1, 6u, CTS_arg); }
        INT16 CTS_E7HS  (INT16 CTS_arg) { return CTS_CFM_PORTE_HS ((UINT8*)&MXC_PTEPF1, 7u, CTS_arg); }
      #endif
      
      /* PORTF CTS sensor */
      #if defined(__CWCC__)  /* If Code Warrior used */ 
        #pragma optimization_level 0
      #else
        #pragma optimize=none
      #endif

      #ifdef SIM_SCGC6_PORTF_MASK
      INT16 CTS_CFM_PORTF_HS (UINT8 * ptr_port, UINT8 pin_num, INT16 CTS_arg)
      {
        UINT8 msk_pin;

        msk_pin = 1 << pin_num;
        
        if (cCTS_ARG_RATIO == CTS_arg) return CTS_RATIO;
        
        /* ExISR_INIT */
        #if 1==ExISR_ENABLED
          asm{ BSET %5,-5(sp); } /* Put indication data to stack */
        #endif
        
        /* port init */
        PORTF_pCLK;
        if (pin_num % 2u)
        {	  
          (*ptr_port) &= 0x0Fu; /* Erase PCR */
          (*ptr_port) |= 0x10u; /* Set GPIO function for PIN */
        } else {
          (*ptr_port) &= 0xF0u; /* Erase PCR */
          (*ptr_port) |= 0x01u; /* Set GPIO function for PIN */
        }
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTF_pSET;
        #endif
        PORTF_pINP;
        if (cCTS_ARG_RESET == CTS_arg) return CTS_RATIO;
      
        /* HiTest -> set Low */
        if (!PORTF_qSET) return cCTS_ERROR_LoPIN;
        PORTF_pCLR;
        PORTF_pOUT;
      
        /* need to wait 5x nop for higher parasitic capacitances to proceed with the next LoTest without error */
        asm("nop \n nop \n nop \n nop \n nop");
        
        /* LoTest */
        if (PORTF_qSET) return cCTS_ERROR_HiPIN;
        PORTF_pINP;
        
        /* if CTS_arg > 0 then use pull-up for turbo-control or -1 for sens-loop with PE enabled */
        #if defined(__CWCC__)  /* If Code Warrior used */ 
          #pragma optimization_level 4
        #else
          #pragma optimize=speed
        #endif
        if (CTS_arg) {
          PORTF_pPEon;
          if (0 < CTS_arg) {
            while (--CTS_arg){}; /* turbo loop = 7 or 8 depends on optimalization level */
            PORTF_pPEoff;
          } else CTS_arg = 0;
        }
        
        /* Sens-loop waits for LOG1 */
        while ((!PORTF_qSET) && (0 < ++CTS_arg)){}; /* sens loop = 14  for none-optimalization */
      
        /* ExISR_Test -> return -1; */
        #if 1==ExISR_ENABLED
          #if defined(__CWCC__)  /* If Code Warrior used */ 
            #pragma optimization_level 0
          #else
            #pragma optimize=none
          #endif
          asm {   BTST.B %5, -5(sp); /* Check if indication data are still in the stack */
            BNE.B PORTA_ExISR_UNDETECTED;
            MOVE.W %-1, CTS_arg;
            PORTA_ExISR_UNDETECTED:
          }
        #endif
        
        /* disable pull-up */
        PORTF_pPEoff;
        
        /* Set Default PIN State */
        #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
          PORTF_pCLR;
          PORTF_pOUT;
        #else
          PORTF_pINP;
        #endif
      
        /* ret */
        return CTS_arg;
      }
      #endif
      
      #ifdef MXC_PTFPF4      
        INT16 CTS_F0HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF4, 0u, CTS_arg); }
        INT16 CTS_F1HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF4, 1u, CTS_arg); }
      #endif
      #ifdef MXC_PTFPF3      
        INT16 CTS_F2HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF3, 2u, CTS_arg); }
        INT16 CTS_F3HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF3, 3u, CTS_arg); }
      #endif
      #ifdef MXC_PTFPF2      
        INT16 CTS_F4HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF2, 4u, CTS_arg); }
        INT16 CTS_F5HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF2, 5u, CTS_arg); }
      #endif
      #ifdef MXC_PTFPF1      
        INT16 CTS_F6HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF1, 6u, CTS_arg); }
        INT16 CTS_F7HS  (INT16 CTS_arg) { return CTS_CFM_PORTF_HS ((UINT8*)&MXC_PTFPF1, 7u, CTS_arg); }
      #endif

    #endif /* End of TSS_CFM_MCU */
		
    /***************************************************************************//*!
    *
    * @brief  Performs a determination of Turbo values for certain electrode  
    *
    * @param  u8ElecNum Required Electrode number
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/ 
    UINT8 CTS_GetTurbo(UINT8 u8ElecNum) 
    {
    
       UINT8 u8Result = TSS_INIT_STATUS_OK;      
       UINT8 u8FaultCnt;
       INT16 i16SampleTest = 0;
       INT16 i16TurboTemp = 0;
       
       TSS_CTS_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
       TSS_CTS_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;

       /* Pointers Decoding */      
       psElectrodeROMDataStruct = (TSS_CTS_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
       psElectrodeRAMDataStruct = (TSS_CTS_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);

       /* Determine first Turbo value */
       u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
       do {
          i16TurboTemp = (psElectrodeROMDataStruct->cts_fSampleElectrodeLow) (cCTS_ARG_TURBO);

          u8FaultCnt--;
       } while ((i16TurboTemp < 0) && (u8FaultCnt > 0));
         
       if (u8FaultCnt > 0) /* Success */
       { 
         /* Calculate real turbo value from cts loop ratio */    
         i16TurboTemp *= (((psElectrodeROMDataStruct->cts_fSampleElectrodeLow) (cCTS_ARG_RATIO)) & 0x0F);
         i16TurboTemp /= (((psElectrodeROMDataStruct->cts_fSampleElectrodeLow) (cCTS_ARG_RATIO)) >> 4);
         /* Try maximize turbo speed-up */
         u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
         do {
           do {
              i16SampleTest = (psElectrodeROMDataStruct->cts_fSampleElectrodeLow) (++i16TurboTemp);
           } while (i16SampleTest > TSS_CTS_MIN_TURBO_SIGNAL);     
  
           if (i16SampleTest == cCTS_ERROR_ExISR) { /* Solving of situation when Interrupt occured */
             i16TurboTemp--;   
           }
  
           u8FaultCnt--; 
         } while ((i16SampleTest == cCTS_ERROR_ExISR) && (u8FaultCnt > 0));
         
         /* Timeout evaluation after measurement */
         if (u8FaultCnt > 0) 
         {
             /* Align turbo speed-up to first one appropriate */
             u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
             do {
               do {
                  i16SampleTest = (psElectrodeROMDataStruct->cts_fSampleElectrodeLow) (--i16TurboTemp);
               } while ((i16SampleTest <= TSS_CTS_MIN_TURBO_SIGNAL) && (i16TurboTemp > 0));
               
               if (i16SampleTest == cCTS_ERROR_ExISR) {  /* Solving of situation when Interrupt occured */
                 i16TurboTemp++;   
               }
               
               u8FaultCnt--; 
             } while ((i16SampleTest == cCTS_ERROR_ExISR) && (u8FaultCnt > 0));
             
             if (u8FaultCnt > 0) 
             {
                u8Result = TSS_INIT_STATUS_OK;
    
                /* Change Turbo value just when difference is more than 1, for prevention of oscillations */
                if (psElectrodeRAMDataStruct->cts_i16turbo != i16TurboTemp) {
                  if ( ((psElectrodeRAMDataStruct->cts_i16turbo - i16TurboTemp) > 1) || ((psElectrodeRAMDataStruct->cts_i16turbo - i16TurboTemp) < (-1)) ) {
                    u8Result |= TSS_INIT_STATUS_CALIBRATION_CHANGED;
                    psElectrodeRAMDataStruct->cts_i16turbo = i16TurboTemp;
                  }
                }
             
             } else {
                u8Result |= TSS_INIT_ERROR_RECALIB_FAULT; /* If a fault has occurred sets the return status code error */
             } 
               
         } else {
            u8Result |= TSS_INIT_ERROR_RECALIB_FAULT; /* If a fault has occurred sets the return status code error */
         }
       
       } else {
         u8Result |= TSS_INIT_ERROR_RECALIB_FAULT; /* If a fault has occurred sets the return status code error */
       }

       return u8Result;        
    }

    /***************************************************************************//*!
    *
    * @brief  Performs a determination of Turbo values for certain electrode  
    *
    * @param  u8ElecNum Required Electrode number
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/ 
    UINT8 CTS_MethodControl(UINT8 u8ElecNum, UINT8 u8Command) 
    {
      UINT8 u8Result = TSS_INIT_STATUS_OK;      
      UINT8 u8ElCounter;
      
      TSS_CTS_METHOD_ROMDATA *psMethodROMDataStruct;
      TSS_CTS_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
       
      /* Pointers Decoding */      
      psElectrodeROMDataStruct = (TSS_CTS_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
      psMethodROMDataStruct = (TSS_CTS_METHOD_ROMDATA *) (psElectrodeROMDataStruct->cts_cpsu32MethodROMData);
      
      if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
      {    
          for(u8ElCounter = 0u; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
          {
            if ((UINT32**) psMethodROMDataStruct == (UINT32 **) ((UINT32 *)tss_acp8ElectrodeROMData[u8ElCounter])[0]) 
            {
              #if TSS_USE_NOISE_AMPLITUDE_FILTER
                 /* Noise Amplitude Filter Initialization */              
                ((TSS_CTS_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElCounter]))->cts_u16AmplitudeFilterBase = 0;
              #endif                
              u8Result |= CTS_GetTurbo(u8ElCounter);
            } 
          }

      }
            
      if (u8Command == TSS_INIT_COMMAND_RECALIBRATE)
      {    
         u8Result |= CTS_GetTurbo(u8ElecNum);
      }

      return u8Result;
    }

    /***************************************************************************//*!
    *
    * @brief  Performs a valid reading of one Electrode stores the timer values 
    *         and returns a status code  
    *
    * @param  u8ElecNum Required Electrode number
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/
    UINT8 CTS_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
    {
        UINT8 u8NSamp;
        UINT8 u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  
        UINT8 u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
        UINT8 u8RecalibCounter = 0u;
        INT16 i16Sample = 0;

        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          INT8 i8AmplitudeFilterDeltaLimitTemp;
        #endif

        TSS_CTS_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
        TSS_CTS_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
        
        /* Pointers Decoding */      
        psElectrodeRAMDataStruct = (TSS_CTS_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
        psElectrodeROMDataStruct = (TSS_CTS_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
        
      if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
      {
        u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;           
      } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
      {  

        /* Intitialization of variables */
        tss_u16CapSample = TSS_CTS_SIGNAL_OFFSET;
        u8NSamp = tss_CSSys.NSamples;    
        
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (psElectrodeRAMDataStruct->cts_u16AmplitudeFilterBase == 0) 
          {
            i8AmplitudeFilterDeltaLimitTemp = 127;
          } else {
            i8AmplitudeFilterDeltaLimitTemp = psElectrodeROMDataStruct->cts_i8AmplitudeFilterDeltaLimit;
          }
        #endif

         /* Main measurement loop */
        while(u8NSamp > 0) 
        {                  

          i16Sample =(psElectrodeROMDataStruct->cts_fSampleElectrodeLow) (psElectrodeRAMDataStruct->cts_i16turbo);   /* Perform measurement */        
        
          if (i16Sample > cCTS_ERROR_LoCAP)               /* Capacitance Signal Ok */
          {
            #if TSS_USE_NOISE_AMPLITUDE_FILTER
              if (TSS_u16NoiseAmplitudeFilter((UINT16) i16Sample, &(psElectrodeRAMDataStruct->cts_u16AmplitudeFilterBase), i8AmplitudeFilterDeltaLimitTemp)) 
              {
                u8FaultCnt--;                                      
              } 
              else 
              { 
                tss_u16CapSample += i16Sample;
                u8NSamp--;
                u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;      /* Restart Fault counter due long cyclic interrupts */
              }
            #else
              tss_u16CapSample += i16Sample;
              u8NSamp--;
              u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
            #endif  
   
            if (i16Sample >  TSS_CTS_RECALIB_HIGH_LIMIT) /* If Cap Signal Very High */
            {      
              if (u8RecalibCounter < TSS_CTS_RECALIB_COUNTER_TIMEOUT) 
              {
                u8RecalibCounter++;
              } else {
                u8ElectrodeStatus = TSS_SAMPLE_RECALIB_REQUEST_HICAP; /* If a recalibration is needed */
              }
            }

          } else if (i16Sample == cCTS_ERROR_LoCAP)     /* Dynamic Recalibration in Keydetector */
          {

            u8FaultCnt = TSS_CTS_FAULT_TIMEOUT;
            if (u8RecalibCounter < TSS_CTS_RECALIB_COUNTER_TIMEOUT) {
              u8RecalibCounter++;
            } else {
              u8NSamp = 0u;
              
              if (psElectrodeRAMDataStruct->cts_i16turbo < 2)  
              {
                u8ElectrodeStatus = TSS_KEYDETECT_ERROR_SMALL_CAP; /* If not possible decrease of Turbo value, Disable Electrode */             
              } else {
                u8ElectrodeStatus = TSS_SAMPLE_RECALIB_REQUEST_LOCAP; /* If a recalibration is possible, request */
              }
              
            }

          } else if (i16Sample == cCTS_ERROR_ExISR)     /* Interrupt Occured */
          {
            /* Do nothing because the measurement was corrupted by interrupt, and sample again in next loop */
            u8FaultCnt--;
            
          } else { /* if (-128) then Capacitance is extremely high, variable overflow */
            u8NSamp = 0u;
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
          }
          
          if(u8FaultCnt == 0u) 
          {
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
            u8NSamp = 0u; 
          }
           
        }
        
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (u8ElectrodeStatus == TSS_SAMPLE_STATUS_OK) 
          {
            if (psElectrodeRAMDataStruct->cts_u16AmplitudeFilterBase == 0) 
            {
              psElectrodeRAMDataStruct->cts_u16AmplitudeFilterBase = (INT8) (tss_u16CapSample / tss_CSSys.NSamples);
            }
          } else {
            psElectrodeRAMDataStruct->cts_u16AmplitudeFilterBase = 0; /* Initiate Amplitude Filter Base refresh */
          }
        #endif
      } else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
      {
        /* Return the same electrode number because there is no more electrodes in the module */
        u8ElectrodeStatus = u8ElecNum;

      } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
      {
        u8ElectrodeStatus = CTS_MethodControl(u8ElecNum, TSS_INIT_COMMAND_RECALIBRATE); 
        
        if (u8ElectrodeStatus & TSS_INIT_ERROR_RECALIB_FAULT) 
        {
          /* Fault happened, so disable actual electrode */
          u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;
        } else if (u8ElectrodeStatus & TSS_INIT_STATUS_CALIBRATION_CHANGED) 
        {
          /* New Turbo value was set so status is OK */
          u8ElectrodeStatus = TSS_SAMPLE_STATUS_CALIBRATION_CHANGED;
        } else 
        {
          /* Old Turbo value was correct */
          u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  
        }
      } 
        
      return u8ElectrodeStatus;   /* Return status code */
    }  
  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    

#endif /* End of TSS_DETECT_METHOD(CTS) */
