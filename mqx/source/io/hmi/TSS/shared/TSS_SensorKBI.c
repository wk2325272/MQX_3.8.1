/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorKBI.c
*
* @brief  Contains functions to Perform the sensing ot the electrodes and set the status for each electrode
*
* @version 1.0.8.0
* 
* @date Dec-6-2011
* 
*
***********************************************************************************************************************/


#include "TSS_SensorKBI.h"

#if TSS_DETECT_METHOD(KBI)

  #include "TSS_Timer.h"
  #include "TSS_GPIO.h"

  #if TSS_HCS08_MCU || TSS_CFV1_MCU

    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(0),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(1),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(2),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(3),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(4),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(5),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(6),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(7),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(8),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(9),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(10),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(11),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(12),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(13),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(14),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(15),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(16),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(17),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(18),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(19),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(20),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(21),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(22),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(23),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(24),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(25),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(26),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(27),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(28),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(29),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(30),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(31),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(32),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(33),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(34),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(35),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(36),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(37),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(38),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(39),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(40),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(41),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(42),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(43),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(44),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(45),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(46),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(47),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(48),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(49),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(50),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(51),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(52),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(53),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(54),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(55),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(56),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(57),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(58),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(59),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(60),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(61),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(62),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(63),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
              
    /********************************* Prototypes ********************************/          
    
    void TSS_KBISetRisingEdge(void);              
              
    /***************************************************************************//*!
    *
    * @brief  Sets KBI pin to rising edge 
    *
    * @param  void
    *
    * @return void
    *
    * @remarks
    *
    ****************************************************************************/
    void TSS_KBISetRisingEdge(void)
    {
      #if TSS_N_ELECTRODES > 0
        #if (TSS_DETECT_EL_METHOD(0,KBI))
          TSS_SET_EL_KBI_EDG(0) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 1
        #if (TSS_DETECT_EL_METHOD(1,KBI))
          TSS_SET_EL_KBI_EDG(1) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 2
        #if (TSS_DETECT_EL_METHOD(2,KBI))
          TSS_SET_EL_KBI_EDG(2) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 3
        #if (TSS_DETECT_EL_METHOD(3,KBI))
          TSS_SET_EL_KBI_EDG(3) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 4
        #if (TSS_DETECT_EL_METHOD(4,KBI))
          TSS_SET_EL_KBI_EDG(4) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 5
        #if (TSS_DETECT_EL_METHOD(5,KBI))
          TSS_SET_EL_KBI_EDG(5) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 6
        #if (TSS_DETECT_EL_METHOD(6,KBI))
          TSS_SET_EL_KBI_EDG(6) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 7
        #if (TSS_DETECT_EL_METHOD(7,KBI))
          TSS_SET_EL_KBI_EDG(7) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 8
        #if (TSS_DETECT_EL_METHOD(8,KBI))
          TSS_SET_EL_KBI_EDG(8) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 9
        #if (TSS_DETECT_EL_METHOD(9,KBI))
          TSS_SET_EL_KBI_EDG(9) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      
      #if TSS_N_ELECTRODES > 10
        #if (TSS_DETECT_EL_METHOD(10,KBI))
          TSS_SET_EL_KBI_EDG(10) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 11
        #if (TSS_DETECT_EL_METHOD(11,KBI))
          TSS_SET_EL_KBI_EDG(11) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 12
        #if (TSS_DETECT_EL_METHOD(12,KBI))
          TSS_SET_EL_KBI_EDG(12) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 13
        #if (TSS_DETECT_EL_METHOD(13,KBI))
          TSS_SET_EL_KBI_EDG(13) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 14
        #if (TSS_DETECT_EL_METHOD(14,KBI))
          TSS_SET_EL_KBI_EDG(14) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 15
        #if (TSS_DETECT_EL_METHOD(15,KBI))
          TSS_SET_EL_KBI_EDG(15) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 16
        #if (TSS_DETECT_EL_METHOD(16,KBI))
          TSS_SET_EL_KBI_EDG(16) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 17
        #if (TSS_DETECT_EL_METHOD(17,KBI))
          TSS_SET_EL_KBI_EDG(17) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 18
        #if (TSS_DETECT_EL_METHOD(18,KBI))
          TSS_SET_EL_KBI_EDG(18) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 19
        #if (TSS_DETECT_EL_METHOD(19,KBI))
          TSS_SET_EL_KBI_EDG(19) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      
      #if TSS_N_ELECTRODES > 20
        #if (TSS_DETECT_EL_METHOD(20,KBI))
          TSS_SET_EL_KBI_EDG(20) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 21
        #if (TSS_DETECT_EL_METHOD(21,KBI))
          TSS_SET_EL_KBI_EDG(21) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 22
        #if (TSS_DETECT_EL_METHOD(22,KBI))
          TSS_SET_EL_KBI_EDG(22) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 23
        #if (TSS_DETECT_EL_METHOD(23,KBI))
          TSS_SET_EL_KBI_EDG(23) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 24
        #if (TSS_DETECT_EL_METHOD(24,KBI))
          TSS_SET_EL_KBI_EDG(24) = TSS_KBI_RISING_EDGE;
        #endif   
      #endif
      #if TSS_N_ELECTRODES > 25
        #if (TSS_DETECT_EL_METHOD(25,KBI))
          TSS_SET_EL_KBI_EDG(25) = TSS_KBI_RISING_EDGE;
        #endif  
      #endif
      #if TSS_N_ELECTRODES > 26
        #if (TSS_DETECT_EL_METHOD(26,KBI))
          TSS_SET_EL_KBI_EDG(26) = TSS_KBI_RISING_EDGE;
        #endif   
      #endif
      #if TSS_N_ELECTRODES > 27
        #if (TSS_DETECT_EL_METHOD(27,KBI))
          TSS_SET_EL_KBI_EDG(27) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 28
        #if (TSS_DETECT_EL_METHOD(28,KBI))
          TSS_SET_EL_KBI_EDG(28) = TSS_KBI_RISING_EDGE;
        #endif       
      #endif
      #if TSS_N_ELECTRODES > 29
        #if (TSS_DETECT_EL_METHOD(29,KBI))
          TSS_SET_EL_KBI_EDG(29) = TSS_KBI_RISING_EDGE;
        #endif      
      #endif
      
      #if TSS_N_ELECTRODES > 30
        #if (TSS_DETECT_EL_METHOD(30,KBI))
          TSS_SET_EL_KBI_EDG(30) = TSS_KBI_RISING_EDGE;
        #endif       
      #endif
      #if TSS_N_ELECTRODES > 31
        #if (TSS_DETECT_EL_METHOD(31,KBI))
          TSS_SET_EL_KBI_EDG(31) = TSS_KBI_RISING_EDGE;
        #endif        
      #endif
      #if TSS_N_ELECTRODES > 32
        #if (TSS_DETECT_EL_METHOD(32,KBI))
          TSS_SET_EL_KBI_EDG(32) = TSS_KBI_RISING_EDGE;
        #endif       
      #endif
      #if TSS_N_ELECTRODES > 33      
        #if (TSS_DETECT_EL_METHOD(33,KBI))
          TSS_SET_EL_KBI_EDG(33) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 34
        #if (TSS_DETECT_EL_METHOD(34,KBI))
          TSS_SET_EL_KBI_EDG(34) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 35
        #if (TSS_DETECT_EL_METHOD(35,KBI))
          TSS_SET_EL_KBI_EDG(35) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 36
        #if (TSS_DETECT_EL_METHOD(36,KBI))
          TSS_SET_EL_KBI_EDG(36) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 37
        #if (TSS_DETECT_EL_METHOD(37,KBI))
          TSS_SET_EL_KBI_EDG(37) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 38
        #if (TSS_DETECT_EL_METHOD(38,KBI))
          TSS_SET_EL_KBI_EDG(38) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 39
        #if (TSS_DETECT_EL_METHOD(39,KBI))
          TSS_SET_EL_KBI_EDG(39) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      
      #if TSS_N_ELECTRODES > 40
        #if (TSS_DETECT_EL_METHOD(40,KBI))
          TSS_SET_EL_KBI_EDG(40) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 41
        #if (TSS_DETECT_EL_METHOD(41,KBI))
          TSS_SET_EL_KBI_EDG(41) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 42
        #if (TSS_DETECT_EL_METHOD(42,KBI))
          TSS_SET_EL_KBI_EDG(42) = TSS_KBI_RISING_EDGE;
        #endif   
      #endif
      #if TSS_N_ELECTRODES > 43
        #if (TSS_DETECT_EL_METHOD(43,KBI))
          TSS_SET_EL_KBI_EDG(43) = TSS_KBI_RISING_EDGE;
        #endif    
      #endif
      #if TSS_N_ELECTRODES > 44
        #if (TSS_DETECT_EL_METHOD(44,KBI))
          TSS_SET_EL_KBI_EDG(44) = TSS_KBI_RISING_EDGE;
        #endif 
      #endif
      #if TSS_N_ELECTRODES > 45
        #if (TSS_DETECT_EL_METHOD(45,KBI))
          TSS_SET_EL_KBI_EDG(45) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 46
        #if (TSS_DETECT_EL_METHOD(46,KBI))
          TSS_SET_EL_KBI_EDG(46) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 47
        #if (TSS_DETECT_EL_METHOD(47,KBI))
          TSS_SET_EL_KBI_EDG(47) = TSS_KBI_RISING_EDGE;
        #endif    
      #endif
      #if TSS_N_ELECTRODES > 48
        #if (TSS_DETECT_EL_METHOD(48,KBI))
          TSS_SET_EL_KBI_EDG(48) = TSS_KBI_RISING_EDGE;
        #endif    
      #endif
      #if TSS_N_ELECTRODES > 49
        #if (TSS_DETECT_EL_METHOD(49,KBI))
          TSS_SET_EL_KBI_EDG(49) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      
      #if TSS_N_ELECTRODES > 50
        #if (TSS_DETECT_EL_METHOD(50,KBI))
          TSS_SET_EL_KBI_EDG(50) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 51
        #if (TSS_DETECT_EL_METHOD(51,KBI))
          TSS_SET_EL_KBI_EDG(51) = TSS_KBI_RISING_EDGE;
        #endif    
      #endif
      #if TSS_N_ELECTRODES > 52
        #if (TSS_DETECT_EL_METHOD(52,KBI))
          TSS_SET_EL_KBI_EDG(52) = TSS_KBI_RISING_EDGE;
        #endif      
      #endif
      #if TSS_N_ELECTRODES > 53
        #if (TSS_DETECT_EL_METHOD(53,KBI))
          TSS_SET_EL_KBI_EDG(53) = TSS_KBI_RISING_EDGE;
        #endif    
      #endif
      #if TSS_N_ELECTRODES > 54
        #if (TSS_DETECT_EL_METHOD(54,KBI))
          TSS_SET_EL_KBI_EDG(54) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 55
        #if (TSS_DETECT_EL_METHOD(55,KBI))
          TSS_SET_EL_KBI_EDG(55) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 56
        #if (TSS_DETECT_EL_METHOD(56,KBI))
          TSS_SET_EL_KBI_EDG(56) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 57
        #if (TSS_DETECT_EL_METHOD(57,KBI))
          TSS_SET_EL_KBI_EDG(57) = TSS_KBI_RISING_EDGE;
        #endif      
      #endif
      #if TSS_N_ELECTRODES > 58
        #if (TSS_DETECT_EL_METHOD(58,KBI))
          TSS_SET_EL_KBI_EDG(58) = TSS_KBI_RISING_EDGE;
        #endif      
      #endif
      #if TSS_N_ELECTRODES > 59
        #if (TSS_DETECT_EL_METHOD(59,KBI))
          TSS_SET_EL_KBI_EDG(59) = TSS_KBI_RISING_EDGE;
        #endif  
      #endif
      
      #if TSS_N_ELECTRODES > 60
        #if (TSS_DETECT_EL_METHOD(60,KBI))
          TSS_SET_EL_KBI_EDG(60) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 61
        #if (TSS_DETECT_EL_METHOD(61,KBI))
          TSS_SET_EL_KBI_EDG(61) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 62
        #if (TSS_DETECT_EL_METHOD(62,KBI))
          TSS_SET_EL_KBI_EDG(62) = TSS_KBI_RISING_EDGE;
        #endif     
      #endif
      #if TSS_N_ELECTRODES > 63
        #if (TSS_DETECT_EL_METHOD(63,KBI))
          TSS_SET_EL_KBI_EDG(63) = TSS_KBI_RISING_EDGE;
        #endif    
      #endif
    }

    /***************************************************************************//*!
    *
    * @brief  Initializes the modules for the sensing of the electrodes
    *
    * @param  Nothing
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/
    UINT8 KBI_SensorInit(void)
    {
      UINT8 u8result = TSS_INIT_STATUS_OK;

      /* KBI Initialization */
      #if TSS_DETECT_MODULE(KBI)
        TSS_KBI_ENABLE_INT(KBI);
      #endif
      #if TSS_DETECT_MODULE(KBI1)
        TSS_KBI_ENABLE_INT(KBI1);
      #endif
      #if TSS_DETECT_MODULE(KBI2)
        TSS_KBI_ENABLE_INT(KBI2);
      #endif
      #if TSS_DETECT_MODULE(KBI3)
        TSS_KBI_ENABLE_INT(KBI3);
      #endif

      #if TSS_DETECT_METHOD(KBI)
        TSS_KBISetRisingEdge();
      #endif
      
      return  u8result;  
    }
    
    /***************************************************************************//*!
    *
    * @brief  ISR for all used KBI modules
    *
    * @param  void
    *
    * @return void
    *
    * @remarks
    *
    ****************************************************************************/
    #if TSS_USE_PE_COMPONENT
      #if TSS_DETECT_MODULE(KBI)
        interrupt void TSS_KBIIsr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI);
        }
        /* 51AC256 vector number exception */
        #if !TSS_DETECT_MODULE(KBI1)
          interrupt void TSS_KBI1Isr(void)
          {
            TSS_HW_TIMER_STOP();
            TSS_KBI_ACK(KBI1);
          }
        #endif
      #endif
    
      #if TSS_DETECT_MODULE(KBI1)
        interrupt void TSS_KBI1Isr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI1);
        }
        /* Predicted Excpetion Solving */
        #if !TSS_DETECT_MODULE(KBI)
          interrupt void TSS_KBIIsr(void)
          {
            TSS_HW_TIMER_STOP();
            TSS_KBI_ACK(KBI);
          }
        #endif
      #endif

      #if TSS_DETECT_MODULE(KBI2)
        interrupt void TSS_KBI2Isr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI2);
        }
      #endif

      #if TSS_DETECT_MODULE(KBI3)
        interrupt void TSS_KBI3Isr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI3);
        }
      #endif
    
    #else /* !TSS_USE_PE_COMPONENT */

      #if TSS_DETECT_MODULE(KBI)
        interrupt VectorNumber_Vkeyboard void TSS_KBIIsr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI);
        }
      #endif

      #if TSS_DETECT_MODULE(KBI1)
        interrupt VectorNumber_Vkeyboard1 void TSS_KBI1Isr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI1);
        }
      #endif

      #if TSS_DETECT_MODULE(KBI2)
        interrupt VectorNumber_Vkeyboard2 void TSS_KBI2Isr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI2);
        }
      #endif

      #if TSS_DETECT_MODULE(KBI3)
        interrupt VectorNumber_Vkeyboard3 void TSS_KBI3Isr(void)
        {
          TSS_HW_TIMER_STOP();
          TSS_KBI_ACK(KBI3);
        }
      #endif
    
    #endif  
  
  #elif TSS_KINETIS_MCU || TSS_CFM_MCU

    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,KBI)
        const TSS_KBI_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = {
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #else
                                                                   0u
                                                                 #endif
                                                                 };
              TSS_KBI_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"
  #endif  
  
#endif /* End of TSS_DETECT_METHOD(KBI) */
