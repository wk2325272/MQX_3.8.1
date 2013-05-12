/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorTIC.c
*
* @brief  Contains functions to Perform the sensing ot the electrodes and set the status for each electrode
*
* @version 1.0.17.0
* 
* @date Mar-12-2012
* 
*
***********************************************************************************************************************/

#include "TSS_SensorTIC.h"

#if TSS_DETECT_METHOD(TIC)

  #include "TSS_Timer.h"
  #include "TSS_GPIO.h"

  #if TSS_HCS08_MCU || TSS_CFV1_MCU

    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(0),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(1),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(2),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(3),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(4),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(5),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(6),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(7),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(8),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(9),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(10),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(11),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(12),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(13),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(14),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(15),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(16),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(17),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(18),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(19),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(20),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(21),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(22),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(23),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(24),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(25),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(26),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(27),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(28),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(29),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(30),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(31),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(32),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(33),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(34),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(35),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(36),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(37),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(38),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(39),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(40),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(41),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(42),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(43),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(44),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(45),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(46),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(47),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(48),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(49),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(50),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(51),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(52),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(53),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(54),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(55),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(56),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(57),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(58),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(59),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(60),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(61),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(62),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(63),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
    
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
    UINT8 TIC_SensorInit(void)
    {
        UINT8 u8result = TSS_INIT_STATUS_OK;

        /* TPM Initialization */
        #if TSS_DETECT_MODULE(TPMTIC)
          TSS_TIC_TIMER_INIT(TPM);
        #endif  
        #if TSS_DETECT_MODULE(TPM0TIC)
          TSS_TIC_TIMER_INIT(TPM0);
        #endif
        #if TSS_DETECT_MODULE(TPM1TIC)
          TSS_TIC_TIMER_INIT(TPM1);
        #endif
        #if TSS_DETECT_MODULE(TPM2TIC)
          TSS_TIC_TIMER_INIT(TPM2);
        #endif
        #if TSS_DETECT_MODULE(TPM3TIC)
          TSS_TIC_TIMER_INIT(TPM3);
        #endif
        #if TSS_DETECT_MODULE(TPM4TIC)
          TSS_TIC_TIMER_INIT(TPM4);
        #endif
        /* FTM Initialization */
        #if TSS_DETECT_MODULE(FTMTIC)
          TSS_TIC_TIMER_INIT(FTM);
        #endif
        #if TSS_DETECT_MODULE(FTM0TIC)
          TSS_TIC_TIMER_INIT(FTM0);
        #endif
        #if TSS_DETECT_MODULE(FTM1TIC)
          TSS_TIC_TIMER_INIT(FTM1);
        #endif
        #if TSS_DETECT_MODULE(FTM2TIC)
          TSS_TIC_TIMER_INIT(FTM2);
        #endif
        #if TSS_DETECT_MODULE(FTM3TIC)
          TSS_TIC_TIMER_INIT(FTM3);
        #endif
        #if TSS_DETECT_MODULE(FTM4TIC)
          TSS_TIC_TIMER_INIT(FTM4);
        #endif

        return u8result;
    }

    /***************************************************************************//*!
    *
    * @brief  TPMx/FTMx Overflow Interrupt handlers
    *
    * @param  void
    *
    * @return void
    *
    * @remarks
    *
    ****************************************************************************/

    #if TSS_DETECT_MODULE(TPMTIC)
      #if !TSS_DETECT_HW_TIMER(TPM)               /* Don't use this Isr if TSS_HW_TIMER Isr is used */                    
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtpmovf
        #endif
        void TSS_TPMIsr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(TPM);
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(TPM0TIC)
      #if !TSS_DETECT_HW_TIMER(TPM0)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */                
        interrupt
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtpm0ovf
        #endif
        void TSS_TPM0Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(TPM0);
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(TPM1TIC)
      #if !TSS_DETECT_HW_TIMER(TPM1)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */            
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtpm1ovf
        #endif
        void TSS_TPM1Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(TPM1);          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(TPM2TIC)
      #if !TSS_DETECT_HW_TIMER(TPM2)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */        
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtpm2ovf
        #endif
        void TSS_TPM2Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(TPM2);          
          tss_u8HWTimerFlag = 1u;                   /* Set measurement Overflow Flag variable */
        }
      #endif    
    #endif

    #if TSS_DETECT_MODULE(TPM3TIC)
      #if !TSS_DETECT_HW_TIMER(TPM3)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */    
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtpm3ovf
        #endif
        void TSS_TPM3Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(TPM3);          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(TPM4TIC)
      #if !TSS_DETECT_HW_TIMER(TPM4)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */    
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtpm4ovf
        #endif
        void TSS_TPM4Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(TPM4);          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTMTIC)
      #if !TSS_DETECT_HW_TIMER(FTM)               /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vftmovf
        #endif
        void TSS_FTMIsr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(FTM);
          tss_u8HWTimerFlag = 1u;                  /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTM0TIC)
      #if !TSS_DETECT_HW_TIMER(FTM0)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */                
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vftm0ovf
        #endif
        void TSS_FTM0Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(FTM0);
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(FTM1TIC)
      #if !TSS_DETECT_HW_TIMER(FTM1)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */            
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vftm1ovf
        #endif
        void TSS_FTM1Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(FTM1);
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(FTM2TIC)
      #if !TSS_DETECT_HW_TIMER(FTM2)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */        
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vftm2ovf
        #endif
        void TSS_FTM2Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(FTM2);          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif    
    #endif

    #if TSS_DETECT_MODULE(FTM3TIC)
      #if !TSS_DETECT_HW_TIMER(FTM3)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */        
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vftm3ovf
        #endif
        void TSS_FTM3Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(FTM3);          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTM4TIC)
      #if !TSS_DETECT_HW_TIMER(FTM4)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */    
        interrupt 
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vftm4ovf
        #endif
        void TSS_FTM4Isr(void)
        {
          TSS_TIC_TIMER_CLEARFLAG(FTM4);
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif    
    #endif
  
  #elif TSS_KINETIS_MCU /* Kinetis */    

    /************************** Prototypes ***************************/
              
    UINT8 TIC_MethodControl(UINT8 u8ElNum, UINT8 u8Command);
  
    /**********************  Modules definition **********************/
    #if TSS_DETECT_MODULE(TPMTIC) 
      #if TSS_DETECT_PORT_MODULE(A,TPMTIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_TPM-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,TPMTIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_TPM-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,TPMTIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_TPM-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,TPMTIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_TPM-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,TPMTIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_TPM-16, INT_PORTE-16}; 
      #endif
    #endif

    #if TSS_DETECT_MODULE(TPM0TIC) 
      #if TSS_DETECT_PORT_MODULE(A,TPM0TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM0_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_TPM0-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,TPM0TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM0_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_TPM0-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,TPM0TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM0_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_TPM0-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,TPM0TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM0_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_TPM0-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,TPM0TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM0_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_TPM0-16, INT_PORTE-16}; 
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(TPM1TIC) 
      #if TSS_DETECT_PORT_MODULE(A,TPM1TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM1_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_TPM1-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,TPM1TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM1_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_TPM1-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,TPM1TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM1_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_TPM1-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,TPM1TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM1_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_TPM1-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,TPM1TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM1_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_TPM1-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(TPM2TIC) 
      #if TSS_DETECT_PORT_MODULE(A,TPM2TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM2_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_TPM2-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,TPM2TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM2_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_TPM2-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,TPM2TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM2_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_TPM2-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,TPM2TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM2_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_TPM2-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,TPM2TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM2_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_TPM2-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(TPM3TIC) 
      #if TSS_DETECT_PORT_MODULE(A,TPM3TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM3_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_TPM3-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,TPM3TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM3_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_TPM3-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,TPM3TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM3_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_TPM3-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,TPM3TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM3_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_TPM3-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,TPM3TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM3_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_TPM3-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(TPM4TIC) 
      #if TSS_DETECT_PORT_MODULE(A,TPM4TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM4_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_TPM4-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,TPM4TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM4_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_TPM4-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,TPM4TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM4_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_TPM4-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,TPM4TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM4_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_TPM4-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,TPM4TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) TPM4_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_TPM4-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(FTMTIC) 
      #if TSS_DETECT_PORT_MODULE(A,FTMTIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_FTM-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,FTMTIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_FTM-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,FTMTIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_FTM-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,FTMTIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_FTM-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,FTMTIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_FTM-16, INT_PORTE-16}; 
      #endif
    #endif
        
    #if TSS_DETECT_MODULE(FTM0TIC) 
      #if TSS_DETECT_PORT_MODULE(A,FTM0TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM0_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_FTM0-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,FTM0TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM0_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_FTM0-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,FTM0TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM0_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_FTM0-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,FTM0TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM0_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_FTM0-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,FTM0TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM0_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_FTM0-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(FTM1TIC) 
      #if TSS_DETECT_PORT_MODULE(A,FTM1TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM1_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_FTM1-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,FTM1TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM1_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_FTM1-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,FTM1TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM1_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_FTM1-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,FTM1TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM1_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_FTM1-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,FTM1TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM1_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_FTM1-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(FTM2TIC) 
      #if TSS_DETECT_PORT_MODULE(A,FTM2TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM2_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_FTM2-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,FTM2TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM2_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_FTM2-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,FTM2TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM2_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_FTM2-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,FTM2TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM2_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_FTM2-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,FTM2TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM2_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_FTM2-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(FTM3TIC) 
      #if TSS_DETECT_PORT_MODULE(A,FTM3TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM3_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_FTM3-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,FTM3TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM3_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_FTM3-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,FTM3TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM3_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_FTM3-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,FTM3TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM3_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_FTM3-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,FTM3TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM3_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_FTM3-16, INT_PORTE-16}; 
      #endif
    #endif
  
    #if TSS_DETECT_MODULE(FTM4TIC) 
      #if TSS_DETECT_PORT_MODULE(A,FTM4TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM4_BASE_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, INT_FTM4-16, INT_PORTA-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(B,FTM4TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM4_BASE_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, INT_FTM4-16, INT_PORTB-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(C,FTM4TIC) 
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM4_BASE_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, INT_FTM4-16, INT_PORTC-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(D,FTM4TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM4_BASE_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, INT_FTM4-16, INT_PORTD-16}; 
      #endif
      #if TSS_DETECT_PORT_MODULE(E,FTM4TIC)  
        const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT32*) FTM4_BASE_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, INT_FTM4-16, INT_PORTE-16}; 
      #endif
    #endif
  
    /********************* Method Memory Data Decoding Macros **********/
        
    #define TSS_GET_TIC_METHOD_DATA_CONTEXT_RESULT(memory,timername,res)     TSS_TIC_##timername##TIC_PORT##res##_METHOD_##memory##DATA_CONTEXT
    #define TSS_GET_TIC_METHOD_DATA_CONTEXT_CONV(memory,timername,text)      TSS_GET_TIC_METHOD_DATA_CONTEXT_RESULT(memory,timername,text)
    #define TSS_GET_TIC_METHOD_DATA_CONTEXT_TYPE(memory,timername,elec)      TSS_GET_TIC_METHOD_DATA_CONTEXT_CONV(memory,timername,elec)
  
    #define TSS_GET_TIC_METHOD_DATA_CONTEXT(memory,el)                       TSS_GET_TIC_METHOD_DATA_CONTEXT_TYPE(memory, TSS_TIC_EL_TIMER_NAME(el), TSS_E##el##_P)
        
    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,0), TSS_ELECTRODE_BIT_NUM(0), TSS_TIC_EL_TIMER_CHANNEL(0), TSS_TIC_EL_TIMER_MUX(0),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,1), TSS_ELECTRODE_BIT_NUM(1), TSS_TIC_EL_TIMER_CHANNEL(1), TSS_TIC_EL_TIMER_MUX(1),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,2), TSS_ELECTRODE_BIT_NUM(2), TSS_TIC_EL_TIMER_CHANNEL(2), TSS_TIC_EL_TIMER_MUX(2),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,3), TSS_ELECTRODE_BIT_NUM(3), TSS_TIC_EL_TIMER_CHANNEL(3), TSS_TIC_EL_TIMER_MUX(3),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,4), TSS_ELECTRODE_BIT_NUM(4), TSS_TIC_EL_TIMER_CHANNEL(4), TSS_TIC_EL_TIMER_MUX(4),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,5), TSS_ELECTRODE_BIT_NUM(5), TSS_TIC_EL_TIMER_CHANNEL(5), TSS_TIC_EL_TIMER_MUX(5),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,6), TSS_ELECTRODE_BIT_NUM(6), TSS_TIC_EL_TIMER_CHANNEL(6), TSS_TIC_EL_TIMER_MUX(6),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,7), TSS_ELECTRODE_BIT_NUM(7), TSS_TIC_EL_TIMER_CHANNEL(7), TSS_TIC_EL_TIMER_MUX(7),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,8), TSS_ELECTRODE_BIT_NUM(8), TSS_TIC_EL_TIMER_CHANNEL(8), TSS_TIC_EL_TIMER_MUX(8),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,9), TSS_ELECTRODE_BIT_NUM(9), TSS_TIC_EL_TIMER_CHANNEL(9), TSS_TIC_EL_TIMER_MUX(9),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,10), TSS_ELECTRODE_BIT_NUM(10), TSS_TIC_EL_TIMER_CHANNEL(10), TSS_TIC_EL_TIMER_MUX(10),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,11), TSS_ELECTRODE_BIT_NUM(11), TSS_TIC_EL_TIMER_CHANNEL(11), TSS_TIC_EL_TIMER_MUX(11),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,12), TSS_ELECTRODE_BIT_NUM(12), TSS_TIC_EL_TIMER_CHANNEL(12), TSS_TIC_EL_TIMER_MUX(12),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,13), TSS_ELECTRODE_BIT_NUM(13), TSS_TIC_EL_TIMER_CHANNEL(13), TSS_TIC_EL_TIMER_MUX(13),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,14), TSS_ELECTRODE_BIT_NUM(14), TSS_TIC_EL_TIMER_CHANNEL(14), TSS_TIC_EL_TIMER_MUX(14),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,15), TSS_ELECTRODE_BIT_NUM(15), TSS_TIC_EL_TIMER_CHANNEL(15), TSS_TIC_EL_TIMER_MUX(15),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,16), TSS_ELECTRODE_BIT_NUM(16), TSS_TIC_EL_TIMER_CHANNEL(16), TSS_TIC_EL_TIMER_MUX(16),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,17), TSS_ELECTRODE_BIT_NUM(17), TSS_TIC_EL_TIMER_CHANNEL(17), TSS_TIC_EL_TIMER_MUX(17),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,18), TSS_ELECTRODE_BIT_NUM(18), TSS_TIC_EL_TIMER_CHANNEL(18), TSS_TIC_EL_TIMER_MUX(18),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,19), TSS_ELECTRODE_BIT_NUM(19), TSS_TIC_EL_TIMER_CHANNEL(19), TSS_TIC_EL_TIMER_MUX(19),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,20), TSS_ELECTRODE_BIT_NUM(20), TSS_TIC_EL_TIMER_CHANNEL(20), TSS_TIC_EL_TIMER_MUX(20),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,21), TSS_ELECTRODE_BIT_NUM(21), TSS_TIC_EL_TIMER_CHANNEL(21), TSS_TIC_EL_TIMER_MUX(21),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,22), TSS_ELECTRODE_BIT_NUM(22), TSS_TIC_EL_TIMER_CHANNEL(22), TSS_TIC_EL_TIMER_MUX(22),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,23), TSS_ELECTRODE_BIT_NUM(23), TSS_TIC_EL_TIMER_CHANNEL(23), TSS_TIC_EL_TIMER_MUX(23),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,24), TSS_ELECTRODE_BIT_NUM(24), TSS_TIC_EL_TIMER_CHANNEL(24), TSS_TIC_EL_TIMER_MUX(24),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,25), TSS_ELECTRODE_BIT_NUM(25), TSS_TIC_EL_TIMER_CHANNEL(25), TSS_TIC_EL_TIMER_MUX(25),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,26), TSS_ELECTRODE_BIT_NUM(26), TSS_TIC_EL_TIMER_CHANNEL(26), TSS_TIC_EL_TIMER_MUX(26),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,27), TSS_ELECTRODE_BIT_NUM(27), TSS_TIC_EL_TIMER_CHANNEL(27), TSS_TIC_EL_TIMER_MUX(27),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,28), TSS_ELECTRODE_BIT_NUM(28), TSS_TIC_EL_TIMER_CHANNEL(28), TSS_TIC_EL_TIMER_MUX(28),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,29), TSS_ELECTRODE_BIT_NUM(29), TSS_TIC_EL_TIMER_CHANNEL(29), TSS_TIC_EL_TIMER_MUX(29),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,30), TSS_ELECTRODE_BIT_NUM(30), TSS_TIC_EL_TIMER_CHANNEL(30), TSS_TIC_EL_TIMER_MUX(30),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,31), TSS_ELECTRODE_BIT_NUM(31), TSS_TIC_EL_TIMER_CHANNEL(31), TSS_TIC_EL_TIMER_MUX(31),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,32), TSS_ELECTRODE_BIT_NUM(32), TSS_TIC_EL_TIMER_CHANNEL(32), TSS_TIC_EL_TIMER_MUX(32),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,33), TSS_ELECTRODE_BIT_NUM(33), TSS_TIC_EL_TIMER_CHANNEL(33), TSS_TIC_EL_TIMER_MUX(33),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,34), TSS_ELECTRODE_BIT_NUM(34), TSS_TIC_EL_TIMER_CHANNEL(34), TSS_TIC_EL_TIMER_MUX(34),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,35), TSS_ELECTRODE_BIT_NUM(35), TSS_TIC_EL_TIMER_CHANNEL(35), TSS_TIC_EL_TIMER_MUX(35),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,36), TSS_ELECTRODE_BIT_NUM(36), TSS_TIC_EL_TIMER_CHANNEL(36), TSS_TIC_EL_TIMER_MUX(36),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,37), TSS_ELECTRODE_BIT_NUM(37), TSS_TIC_EL_TIMER_CHANNEL(37), TSS_TIC_EL_TIMER_MUX(37),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,38), TSS_ELECTRODE_BIT_NUM(38), TSS_TIC_EL_TIMER_CHANNEL(38), TSS_TIC_EL_TIMER_MUX(38),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,39), TSS_ELECTRODE_BIT_NUM(39), TSS_TIC_EL_TIMER_CHANNEL(39), TSS_TIC_EL_TIMER_MUX(39),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,40), TSS_ELECTRODE_BIT_NUM(40), TSS_TIC_EL_TIMER_CHANNEL(40), TSS_TIC_EL_TIMER_MUX(40),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,41), TSS_ELECTRODE_BIT_NUM(41), TSS_TIC_EL_TIMER_CHANNEL(41), TSS_TIC_EL_TIMER_MUX(41),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,42), TSS_ELECTRODE_BIT_NUM(42), TSS_TIC_EL_TIMER_CHANNEL(42), TSS_TIC_EL_TIMER_MUX(42),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,43), TSS_ELECTRODE_BIT_NUM(43), TSS_TIC_EL_TIMER_CHANNEL(43), TSS_TIC_EL_TIMER_MUX(43),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,44), TSS_ELECTRODE_BIT_NUM(44), TSS_TIC_EL_TIMER_CHANNEL(44), TSS_TIC_EL_TIMER_MUX(44),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,45), TSS_ELECTRODE_BIT_NUM(45), TSS_TIC_EL_TIMER_CHANNEL(45), TSS_TIC_EL_TIMER_MUX(45),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,46), TSS_ELECTRODE_BIT_NUM(46), TSS_TIC_EL_TIMER_CHANNEL(46), TSS_TIC_EL_TIMER_MUX(46),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,47), TSS_ELECTRODE_BIT_NUM(47), TSS_TIC_EL_TIMER_CHANNEL(47), TSS_TIC_EL_TIMER_MUX(47),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,48), TSS_ELECTRODE_BIT_NUM(48), TSS_TIC_EL_TIMER_CHANNEL(48), TSS_TIC_EL_TIMER_MUX(48),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,49), TSS_ELECTRODE_BIT_NUM(49), TSS_TIC_EL_TIMER_CHANNEL(49), TSS_TIC_EL_TIMER_MUX(49),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,50), TSS_ELECTRODE_BIT_NUM(50), TSS_TIC_EL_TIMER_CHANNEL(50), TSS_TIC_EL_TIMER_MUX(50),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,51), TSS_ELECTRODE_BIT_NUM(51), TSS_TIC_EL_TIMER_CHANNEL(51), TSS_TIC_EL_TIMER_MUX(51),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,52), TSS_ELECTRODE_BIT_NUM(52), TSS_TIC_EL_TIMER_CHANNEL(52), TSS_TIC_EL_TIMER_MUX(52),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,53), TSS_ELECTRODE_BIT_NUM(53), TSS_TIC_EL_TIMER_CHANNEL(53), TSS_TIC_EL_TIMER_MUX(53),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,54), TSS_ELECTRODE_BIT_NUM(54), TSS_TIC_EL_TIMER_CHANNEL(54), TSS_TIC_EL_TIMER_MUX(54),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,55), TSS_ELECTRODE_BIT_NUM(55), TSS_TIC_EL_TIMER_CHANNEL(55), TSS_TIC_EL_TIMER_MUX(55),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,56), TSS_ELECTRODE_BIT_NUM(56), TSS_TIC_EL_TIMER_CHANNEL(56), TSS_TIC_EL_TIMER_MUX(56),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,57), TSS_ELECTRODE_BIT_NUM(57), TSS_TIC_EL_TIMER_CHANNEL(57), TSS_TIC_EL_TIMER_MUX(57),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,58), TSS_ELECTRODE_BIT_NUM(58), TSS_TIC_EL_TIMER_CHANNEL(58), TSS_TIC_EL_TIMER_MUX(58),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,59), TSS_ELECTRODE_BIT_NUM(59), TSS_TIC_EL_TIMER_CHANNEL(59), TSS_TIC_EL_TIMER_MUX(59),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,60), TSS_ELECTRODE_BIT_NUM(60), TSS_TIC_EL_TIMER_CHANNEL(60), TSS_TIC_EL_TIMER_MUX(60),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,61), TSS_ELECTRODE_BIT_NUM(61), TSS_TIC_EL_TIMER_CHANNEL(61), TSS_TIC_EL_TIMER_MUX(61),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,62), TSS_ELECTRODE_BIT_NUM(62), TSS_TIC_EL_TIMER_CHANNEL(62), TSS_TIC_EL_TIMER_MUX(62),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,TIC)
        const TSS_TIC_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,63), TSS_ELECTRODE_BIT_NUM(63), TSS_TIC_EL_TIMER_CHANNEL(63), TSS_TIC_EL_TIMER_MUX(63),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
              TSS_TIC_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
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
    UINT8 TIC_MethodControl(UINT8 u8ElNum, UINT8 u8Command)
    {
        UINT8 u8result = TSS_INIT_STATUS_OK;

        UINT8 u8ElCounter;
        
        TSS_TIC_METHOD_ROMDATA *psMethodROMDataStruct;
        TSS_TIC_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;

        FTM_MemMapPtr psTimerStruct;
        PORT_MemMapPtr psPortStruct;
        RGPIO_MemMapPtr psRGPIOStruct;
        NVIC_MemMapPtr psNVICStruct = NVIC_BASE_PTR;

        /* Pointers Decoding */      
        psElectrodeROMDataStruct = (TSS_TIC_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElNum]);
        psMethodROMDataStruct = (TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu32MethodROMData);
        
        psTimerStruct = (FTM_MemMapPtr) (psMethodROMDataStruct->tic_cpsu32Timer);
        psPortStruct = (PORT_MemMapPtr) (psMethodROMDataStruct->tic_cpsu32Port);
        psRGPIOStruct = (RGPIO_MemMapPtr) (psMethodROMDataStruct->tic_cpsu32RGPIO);

        /************* Do TIC Init *******************/
        if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
        {    
          /* Timer Init */        
          psTimerStruct->SC = 0u; /* Reset Timer */
          psTimerStruct->CNT = 0u; /* Reset Counter */
          psTimerStruct->SC |= (TSS_SENSOR_PRESCALER & TSS_TIC_TIMER_PRESCALE_MASK); /* Set Prescaler */
          psTimerStruct->SC |= TSS_TIC_TIMER_IE; /* Enable Interrupt */
          psTimerStruct->MOD = TSS_SENSOR_TIMEOUT; /* Set MOD Register */
          /* Enable Timer Interrupt */
          psNVICStruct->ISER[psMethodROMDataStruct->tic_u8TimerIRQNum / 32u] = 1 << (psMethodROMDataStruct->tic_u8TimerIRQNum % 32u);

          for(u8ElCounter = 0; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
          {
            if ((UINT32**) psMethodROMDataStruct == (UINT32 **) ((UINT32 *)tss_acp8ElectrodeROMData[u8ElCounter])[0]) 
            {
              /* Noise Amplitude Filter Initialization */
              #if TSS_USE_NOISE_AMPLITUDE_FILTER
                ((TSS_TIC_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElCounter]))->tic_u16AmplitudeFilterBase = 0u;
              #endif
              
              /* Calculate Pointer to the Module */ 
              psElectrodeROMDataStruct = (TSS_TIC_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]);
              /* Init TIC settings */    
              psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] = 0u; /* Erase PCR */
              /* Set PIN as GPIO */
              psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] |= PORT_PCR_MUX(0x01u); /* Set GPIO function for PIN */
              /* Set PIN Strength if enabled */
              #if TSS_USE_GPIO_STRENGTH
                psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] |= PORT_PCR_DSE_MASK; /* Set PIN Strength*/
              #endif
              /* Set PIN SlewRate if enabled */
              #if TSS_USE_GPIO_SLEW_RATE
                psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] |= PORT_PCR_SRE_MASK; /* Set PIN SlewRate*/
              #endif
              /* Set Default PIN State */
              psRGPIOStruct->PDDR |= (1u << psElectrodeROMDataStruct->tic_cu32PortBit); /* Set PIN to OUTPUT */
              #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
                psRGPIOStruct->PCOR = (1u << psElectrodeROMDataStruct->tic_cu32PortBit);  /* Set LOG0 to OUTPUT pin */      
              #else
                psRGPIOStruct->PSOR = (1u << psElectrodeROMDataStruct->tic_cu32PortBit);  /* Set LOG1 to OUTPUT pin */      
              #endif
            } 
          }
        } 
        /* Exit */                   
        return u8result;
    }

    /***************************************************************************//*!
    *
    * @brief  Performs a valid reading of one electrode stores the timer values 
    *         and returns a status code
    *
    * @param  u8ElecNum Electrode number to be scanned
    *
    * @return Status Code
    *
    * @remarks
    *
    ****************************************************************************/
    UINT8 TIC_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
    {
      UINT8 u8NSamp;
      UINT8 u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;   /* Sets default status */
      UINT16 u16CapSubSample;
      UINT8 u8FaultCnt;
      TSS_TIC_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
      FTM_MemMapPtr psTimerStruct;
      RGPIO_MemMapPtr psRGPIOStruct;
      PORT_MemMapPtr psPortStruct;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        INT8 i8AmplitudeFilterDeltaLimitTemp;
        TSS_TIC_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
      #endif

      if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
      {
        u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;           
      } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
      {  
        /* Pointers Decoding */      
        psElectrodeROMDataStruct = (TSS_TIC_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
        psTimerStruct = (FTM_MemMapPtr) (((TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu32MethodROMData))->tic_cpsu32Timer);
        psRGPIOStruct = (RGPIO_MemMapPtr) (((TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu32MethodROMData))->tic_cpsu32RGPIO);
        psPortStruct = (PORT_MemMapPtr) (((TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu32MethodROMData))->tic_cpsu32Port);
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          psElectrodeRAMDataStruct = (TSS_TIC_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
        #endif
       
        tss_psElectrodeTimertAdr = (UINT32 *) psTimerStruct;
        tss_psElectrodeRGPIOAdr = (UINT32 *) psRGPIOStruct;
        tss_u8ElectrodeBitNum = psElectrodeROMDataStruct->tic_cu32PortBit;
        
        /* Initialization of variables */
        tss_u8HWTimerFlag = 0u;
        tss_u16CapSample = 0u;
        u8FaultCnt = 0u;
        u8NSamp = tss_CSSys.NSamples;

        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase == 0u) {
            i8AmplitudeFilterDeltaLimitTemp = 127;
          } else {
            i8AmplitudeFilterDeltaLimitTemp = psElectrodeROMDataStruct->tic_ci8AmplitudeFilterDeltaLimit;
          }
        #endif

        /* Main oversampling measurement loop */
        while(u8NSamp)
        {
            TSS_CLEAR_SAMPLE_INTERRUPTED_FLAG();        /* Clears the interrupt sample flag */

            /********************** TIC Measure Electrode ************************/
            /* Discharge pin */
            psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] &= ~PORT_PCR_MUX_MASK; /* Erase MUX */     
            psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] |= PORT_PCR_MUX(0x01u); /* Set Pin to GPIO */
            psRGPIOStruct->PDDR |= (1 << psElectrodeROMDataStruct->tic_cu32PortBit); /* Set PIN to OUTPUT */     
            psRGPIOStruct->PCOR = (1 << psElectrodeROMDataStruct->tic_cu32PortBit);  /* Set LOG0 to OUTPUT pin */    
            /* Start Timer */
            psTimerStruct->SC |= TSS_TIC_TIMER_ON;
            /* Enable Timer Input Capture */
            psTimerStruct->CONTROLS[psElectrodeROMDataStruct->tic_cu32TimerChannel].CnSC = TSS_TIC_TIMER_CH_ICM_EN;
            psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] &= ~PORT_PCR_MUX_MASK; /* Erase MUX */ 
            psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] |= PORT_PCR_MUX(psElectrodeROMDataStruct->tic_cu8TimerChannelMUX);
            /* Timer Reset */
            psTimerStruct->CNT = 0x0000u;
            /* Measurement Loop */
            while(!(psTimerStruct->CONTROLS[psElectrodeROMDataStruct->tic_cu32TimerChannel].CnSC & TSS_TIC_TIMER_CH_FLAG) && !TSS_FAULT_DETECTED)
            {
              /* MISRA Rule 14.8 */
            }
            /* Charge pin */
            psRGPIOStruct->PDDR &= ~(1 << psElectrodeROMDataStruct->tic_cu32PortBit);  /* Set PIN to INPUT */ 
            /* Disable Timer Input Capture */
            psTimerStruct->CONTROLS[psElectrodeROMDataStruct->tic_cu32TimerChannel].CnSC = 0x00u;
            psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] &= ~PORT_PCR_MUX_MASK; /* Erase MUX */            
            psPortStruct->PCR[psElectrodeROMDataStruct->tic_cu32PortBit] |= PORT_PCR_MUX(0x01u); /* Set Pin to GPIO */
            /* Clear Timer Flag */
            psTimerStruct->SC &= TSS_TIC_TIMER_FLAG_ACK;
            /* Stop Timer */
            psTimerStruct->SC &= TSS_TIC_TIMER_OFF;
            /* Read Value */
            u16CapSubSample = (UINT16) psTimerStruct->CONTROLS[psElectrodeROMDataStruct->tic_cu32TimerChannel].CnV;   
            #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
              /* Discharge pin */
              psRGPIOStruct->PDDR |= (1 << psElectrodeROMDataStruct->tic_cu32PortBit); /* Set PIN to OUTPUT */     
              psRGPIOStruct->PCOR = (1 << psElectrodeROMDataStruct->tic_cu32PortBit);  /* Set LOG0 to OUTPUT pin */    
            #endif
            
            /******************** End of TIC Measure Electrode **********************/          
            
            if(!TSS_FAULT_DETECTED)                     /* Verifies that no fault has occur */
            {            
                if(!TSS_SAMPLE_INTERRUPTED)             /* Verifies that the sample has not been inturrupted*/
                {
                  #if TSS_USE_NOISE_AMPLITUDE_FILTER
                    if (TSS_u16NoiseAmplitudeFilter(u16CapSubSample, &(psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase), i8AmplitudeFilterDeltaLimitTemp)) 
                    {
                      u8FaultCnt++;                                      
                    } 
                    else 
                    { 
                      tss_u16CapSample += u16CapSubSample;
                      u8NSamp--;
                      u8FaultCnt = 0u;                 /* Restart Fault counter due long cyclic interrupts */
                    }
                    
                  #else
                    tss_u16CapSample += u16CapSubSample;
                    u8NSamp--;
                    u8FaultCnt = 0u;                   /* Restart Fault counter due long cyclic interrupts */
                  #endif  
                }
                else
                {    
                  /* Do nothing if interrupt occured and do sample again in next loop*/
                  u8FaultCnt++; 
                }
            }
            else
            {
              /* Set PIN to OUTPUT LOW State */
              psRGPIOStruct->PDDR |= (1 << psElectrodeROMDataStruct->tic_cu32PortBit); /* Set PIN to OUTPUT */
              psRGPIOStruct->PCOR = (1 << psElectrodeROMDataStruct->tic_cu32PortBit);  /* Set LOG0 to OUTPUT pin */      

              u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
              u8NSamp = 0u;                                  
            }
            
            if(u8FaultCnt >= TSS_FAULT_TIMEOUT) {
                u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
                u8NSamp = 0u; 
            }        
        }

        if ((tss_u16CapSample < TSS_KEYDETECT_CAP_LOWER_LIMIT) && (u8ElectrodeStatus != TSS_SAMPLE_ERROR_CHARGE_TIMEOUT)) 
        {
          u8ElectrodeStatus = TSS_SAMPLE_ERROR_SMALL_CAP;
        }
        
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (u8ElectrodeStatus == TSS_SAMPLE_STATUS_OK) {
            if (psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase == 0u) {
              psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase = (UINT16) (tss_u16CapSample / tss_CSSys.NSamples);
            }
          } else {
            psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase = 0u; /* Initiate Amplitude Filter Base refresh */
          }
        #endif
      } else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
      {
        /* Return the same electrode number because there is no more electrodes in the module */
        u8ElectrodeStatus = u8ElecNum;
      } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
      {
        /* Do Nothing because the Command is no relevant for TIC Method */
        u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;
      } 
      
      return u8ElectrodeStatus;   /* Return status code */ 
    }
              
    /***************************************************************************//*!
    *
    * @brief  TPMx/FTMx Overflow Interrupt handlers
    *
    * @param  void
    *
    * @return void
    *
    * @remarks
    *
    ****************************************************************************/

    #if TSS_DETECT_MODULE(TPMTIC)
      #if !TSS_DETECT_HW_TIMER(TPM)               /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_TPMIsr to vector INT_TPM  
        #endif
      
        void TSS_TPMIsr(void)
        {
          /* Clear TImer Flag */
          ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(TPM0TIC)
      #if !TSS_DETECT_HW_TIMER(TPM0)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_TPM0Isr to vector INT_TPM0  
        #endif
      
        void TSS_TPM0Isr(void)
        {
          /* Clear TImer Flag */
          ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(TPM1TIC)
      #if !TSS_DETECT_HW_TIMER(TPM1)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_TPM1Isr to vector INT_TPM1  
        #endif
      
        void TSS_TPM1Isr(void)
        {
          /* Clear TImer Flag */
          ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(TPM2TIC)                /* Don't use this Isr if TSS_HW_TIMER Isr is used */
      #if !TSS_DETECT_HW_TIMER(TPM2)    
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_TPM2Isr to vector INT_TPM2  
        #endif
      
        void TSS_TPM2Isr(void)
        {
          /* Clear TImer Flag */
          ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif    
    #endif

    #if TSS_DETECT_MODULE(TPM3TIC)                /* Don't use this Isr if TSS_HW_TIMER Isr is used */
      #if !TSS_DETECT_HW_TIMER(TPM3)    
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_TPM3Isr to vector INT_TPM3  
        #endif
      
        void TSS_TPM3Isr(void)
        {
          /* Clear TImer Flag */
          ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(TPM4TIC)
      #if !TSS_DETECT_HW_TIMER(TPM4)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_TPM4Isr to vector INT_TPM4  
        #endif
      
        void TSS_TPM4Isr(void)
        {
          /* Clear TImer Flag */
          ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTMTIC)
      #if !TSS_DETECT_HW_TIMER(FTM)               /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_FTMIsr to vector INT_FTM  
        #endif
      
        void TSS_FTMIsr(void)
        {
          /* Clear TImer Flag */
          ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTM0TIC)
      #if !TSS_DETECT_HW_TIMER(FTM0)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */  
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_FTM0Isr to vector INT_FTM0  
        #endif
      
        void TSS_FTM0Isr(void)
        {
          /* Clear TImer Flag */
          ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
            
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif    
    #endif
    
    #if TSS_DETECT_MODULE(FTM1TIC)
      #if !TSS_DETECT_HW_TIMER(FTM1)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_FTM1Isr to vector INT_FTM1  
        #endif
      
        void TSS_FTM1Isr(void)
        {
          /* Clear TImer Flag */
          ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
    
    #if TSS_DETECT_MODULE(FTM2TIC)
      #if !TSS_DETECT_HW_TIMER(FTM2)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_FTM2Isr to vector INT_FTM2  
        #endif
      
        void TSS_FTM2Isr(void)
        {
          /* Clear TImer Flag */
          ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTM3TIC)
      #if !TSS_DETECT_HW_TIMER(FTM3)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_FTM2Isr to vector INT_FTM3  
        #endif
      
        void TSS_FTM3Isr(void)
        {
          /* Clear TImer Flag */
          ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif

    #if TSS_DETECT_MODULE(FTM4TIC)
      #if !TSS_DETECT_HW_TIMER(FTM4)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - Please assign ISR function TSS_FTM2Isr to vector INT_FTM4  
        #endif
      
        void TSS_FTM4Isr(void)
        {
          /* Clear TImer Flag */
          ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
          
          tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
        }
      #endif
    #endif
        
  #elif TSS_CFM_MCU /* End of TSS_KINETIS_MCU */

    /************************** Prototypes ***************************/
			  
      UINT8 TIC_MethodControl(UINT8 u8ElNum, UINT8 u8Command);

      /**********************  Modules definition **********************/
      #if TSS_DETECT_MODULE(TPMTIC)
        #if TSS_DETECT_PORT_MODULE(A,TPMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,TPMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,TPMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,TPMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,TPMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,TPMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPMTIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM0TIC)
        #if TSS_DETECT_PORT_MODULE(A,TPM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM0_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,TPM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM0_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,TPM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM0_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,TPM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM0_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,TPM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM0_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,TPM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM0TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM0_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM1TIC)
        #if TSS_DETECT_PORT_MODULE(A,TPM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM1_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,TPM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM1_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,TPM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM1_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,TPM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM1_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,TPM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM1_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,TPM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM1TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM1_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM2TIC)
        #if TSS_DETECT_PORT_MODULE(A,TPM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM2_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,TPM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM2_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,TPM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM2_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,TPM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM2_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,TPM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM2_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,TPM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM2TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM2_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM3TIC)
        #if TSS_DETECT_PORT_MODULE(A,TPM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM3_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,TPM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM3_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,TPM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM3_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,TPM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM3_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,TPM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM3_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,TPM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM3TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM3_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM4TIC)
        #if TSS_DETECT_PORT_MODULE(A,TPM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM4_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,TPM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM4_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,TPM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM4_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,TPM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM4_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,TPM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM4_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,TPM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_TPM4TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) TPM4_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTMTIC)
        #if TSS_DETECT_PORT_MODULE(A,FTMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,FTMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,FTMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,FTMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,FTMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,FTMTIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTMTIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTM0TIC)
        #if TSS_DETECT_PORT_MODULE(A,FTM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM0_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,FTM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM0_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,FTM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM0_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,FTM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM0_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,FTM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM0_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,FTM0TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM0TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM0_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTM1TIC)
        #if TSS_DETECT_PORT_MODULE(A,FTM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM1_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,FTM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM1_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,FTM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM1_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,FTM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM1_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,FTM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM1_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,FTM1TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM1TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM1_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTM2TIC)
        #if TSS_DETECT_PORT_MODULE(A,FTM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM2_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,FTM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM2_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,FTM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM2_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,FTM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM2_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,FTM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM2_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,FTM2TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM2TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM2_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTM3TIC)
        #if TSS_DETECT_PORT_MODULE(A,FTM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM3_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,FTM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM3_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,FTM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM3_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,FTM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM3_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,FTM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM3_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,FTM3TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM3TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM3_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTM4TIC)
        #if TSS_DETECT_PORT_MODULE(A,FTM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTA_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM4_BASE_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(B,FTM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTB_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM4_BASE_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(C,FTM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTC_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM4_BASE_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(D,FTM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTD_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM4_BASE_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(E,FTM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTE_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM4_BASE_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1};
        #endif
        #if TSS_DETECT_PORT_MODULE(F,FTM4TIC)
              const    TSS_TIC_METHOD_ROMDATA TSS_TIC_FTM4TIC_PORTF_METHOD_ROMDATA_CONTEXT = {TIC_MethodControl, (UINT8*) FTM4_BASE_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1};
        #endif
      #endif

      /********************* Method Memory Data Decoding Macros **********/
              
      #define TSS_GET_TIC_METHOD_DATA_CONTEXT_RESULT(memory,timername,res)     TSS_TIC_##timername##TIC_PORT##res##_METHOD_##memory##DATA_CONTEXT
      #define TSS_GET_TIC_METHOD_DATA_CONTEXT_CONV(memory,timername,text)      TSS_GET_TIC_METHOD_DATA_CONTEXT_RESULT(memory,timername,text)
      #define TSS_GET_TIC_METHOD_DATA_CONTEXT_TYPE(memory,timername,elec)      TSS_GET_TIC_METHOD_DATA_CONTEXT_CONV(memory,timername,elec)

      #define TSS_GET_TIC_METHOD_DATA_CONTEXT(memory,el)                       TSS_GET_TIC_METHOD_DATA_CONTEXT_TYPE(memory, TSS_TIC_EL_TIMER_NAME(el), TSS_E##el##_P)
              
      /***************** ROM & RAM Data definition ***********************/

        #if TSS_N_ELECTRODES > 0
              #if TSS_DETECT_EL_METHOD(0,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,0), TSS_ELECTRODE_BIT_NUM(0), TSS_TIC_EL_TIMER_CHANNEL(0),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 1
              #if TSS_DETECT_EL_METHOD(1,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,1), TSS_ELECTRODE_BIT_NUM(1), TSS_TIC_EL_TIMER_CHANNEL(1),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 2
              #if TSS_DETECT_EL_METHOD(2,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,2), TSS_ELECTRODE_BIT_NUM(2), TSS_TIC_EL_TIMER_CHANNEL(2),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 3
              #if TSS_DETECT_EL_METHOD(3,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,3), TSS_ELECTRODE_BIT_NUM(3), TSS_TIC_EL_TIMER_CHANNEL(3),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 4
              #if TSS_DETECT_EL_METHOD(4,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,4), TSS_ELECTRODE_BIT_NUM(4), TSS_TIC_EL_TIMER_CHANNEL(4),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 5
              #if TSS_DETECT_EL_METHOD(5,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,5), TSS_ELECTRODE_BIT_NUM(5), TSS_TIC_EL_TIMER_CHANNEL(5),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 6
              #if TSS_DETECT_EL_METHOD(6,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,6), TSS_ELECTRODE_BIT_NUM(6), TSS_TIC_EL_TIMER_CHANNEL(6),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 7
              #if TSS_DETECT_EL_METHOD(7,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,7), TSS_ELECTRODE_BIT_NUM(7), TSS_TIC_EL_TIMER_CHANNEL(7),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 8
              #if TSS_DETECT_EL_METHOD(8,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,8), TSS_ELECTRODE_BIT_NUM(8), TSS_TIC_EL_TIMER_CHANNEL(8),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 9
              #if TSS_DETECT_EL_METHOD(9,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,9), TSS_ELECTRODE_BIT_NUM(9), TSS_TIC_EL_TIMER_CHANNEL(9),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 10
              #if TSS_DETECT_EL_METHOD(10,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,10), TSS_ELECTRODE_BIT_NUM(10), TSS_TIC_EL_TIMER_CHANNEL(10),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 11
              #if TSS_DETECT_EL_METHOD(11,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,11), TSS_ELECTRODE_BIT_NUM(11), TSS_TIC_EL_TIMER_CHANNEL(11),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 12
              #if TSS_DETECT_EL_METHOD(12,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,12), TSS_ELECTRODE_BIT_NUM(12), TSS_TIC_EL_TIMER_CHANNEL(12),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 13
              #if TSS_DETECT_EL_METHOD(13,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,13), TSS_ELECTRODE_BIT_NUM(13), TSS_TIC_EL_TIMER_CHANNEL(13),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 14
              #if TSS_DETECT_EL_METHOD(14,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,14), TSS_ELECTRODE_BIT_NUM(14), TSS_TIC_EL_TIMER_CHANNEL(14),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 15
              #if TSS_DETECT_EL_METHOD(15,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,15), TSS_ELECTRODE_BIT_NUM(15), TSS_TIC_EL_TIMER_CHANNEL(15),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 16
              #if TSS_DETECT_EL_METHOD(16,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,16), TSS_ELECTRODE_BIT_NUM(16), TSS_TIC_EL_TIMER_CHANNEL(16),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 17
              #if TSS_DETECT_EL_METHOD(17,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,17), TSS_ELECTRODE_BIT_NUM(17), TSS_TIC_EL_TIMER_CHANNEL(17),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 18
              #if TSS_DETECT_EL_METHOD(18,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,18), TSS_ELECTRODE_BIT_NUM(18), TSS_TIC_EL_TIMER_CHANNEL(18),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 19
              #if TSS_DETECT_EL_METHOD(19,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,19), TSS_ELECTRODE_BIT_NUM(19), TSS_TIC_EL_TIMER_CHANNEL(19),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 20
              #if TSS_DETECT_EL_METHOD(20,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,20), TSS_ELECTRODE_BIT_NUM(20), TSS_TIC_EL_TIMER_CHANNEL(20),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 21
              #if TSS_DETECT_EL_METHOD(21,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,21), TSS_ELECTRODE_BIT_NUM(21), TSS_TIC_EL_TIMER_CHANNEL(21),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 22
              #if TSS_DETECT_EL_METHOD(22,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,22), TSS_ELECTRODE_BIT_NUM(22), TSS_TIC_EL_TIMER_CHANNEL(22),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 23
              #if TSS_DETECT_EL_METHOD(23,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,23), TSS_ELECTRODE_BIT_NUM(23), TSS_TIC_EL_TIMER_CHANNEL(23),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 24
              #if TSS_DETECT_EL_METHOD(24,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,24), TSS_ELECTRODE_BIT_NUM(24), TSS_TIC_EL_TIMER_CHANNEL(24),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 25
              #if TSS_DETECT_EL_METHOD(25,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,25), TSS_ELECTRODE_BIT_NUM(25), TSS_TIC_EL_TIMER_CHANNEL(25),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 26
              #if TSS_DETECT_EL_METHOD(26,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,26), TSS_ELECTRODE_BIT_NUM(26), TSS_TIC_EL_TIMER_CHANNEL(26),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 27
              #if TSS_DETECT_EL_METHOD(27,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,27), TSS_ELECTRODE_BIT_NUM(27), TSS_TIC_EL_TIMER_CHANNEL(27),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 28
              #if TSS_DETECT_EL_METHOD(28,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,28), TSS_ELECTRODE_BIT_NUM(28), TSS_TIC_EL_TIMER_CHANNEL(28),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 29
              #if TSS_DETECT_EL_METHOD(29,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,29), TSS_ELECTRODE_BIT_NUM(29), TSS_TIC_EL_TIMER_CHANNEL(29),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 30
              #if TSS_DETECT_EL_METHOD(30,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,30), TSS_ELECTRODE_BIT_NUM(30), TSS_TIC_EL_TIMER_CHANNEL(30),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 31
              #if TSS_DETECT_EL_METHOD(31,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,31), TSS_ELECTRODE_BIT_NUM(31), TSS_TIC_EL_TIMER_CHANNEL(31),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 32
              #if TSS_DETECT_EL_METHOD(32,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,32), TSS_ELECTRODE_BIT_NUM(32), TSS_TIC_EL_TIMER_CHANNEL(32),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 33
              #if TSS_DETECT_EL_METHOD(33,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,33), TSS_ELECTRODE_BIT_NUM(33), TSS_TIC_EL_TIMER_CHANNEL(33),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 34
              #if TSS_DETECT_EL_METHOD(34,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,34), TSS_ELECTRODE_BIT_NUM(34), TSS_TIC_EL_TIMER_CHANNEL(34),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 35
              #if TSS_DETECT_EL_METHOD(35,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,35), TSS_ELECTRODE_BIT_NUM(35), TSS_TIC_EL_TIMER_CHANNEL(35),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 36
              #if TSS_DETECT_EL_METHOD(36,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,36), TSS_ELECTRODE_BIT_NUM(36), TSS_TIC_EL_TIMER_CHANNEL(36),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 37
              #if TSS_DETECT_EL_METHOD(37,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,37), TSS_ELECTRODE_BIT_NUM(37), TSS_TIC_EL_TIMER_CHANNEL(37),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 38
              #if TSS_DETECT_EL_METHOD(38,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,38), TSS_ELECTRODE_BIT_NUM(38), TSS_TIC_EL_TIMER_CHANNEL(38),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 39
              #if TSS_DETECT_EL_METHOD(39,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,39), TSS_ELECTRODE_BIT_NUM(39), TSS_TIC_EL_TIMER_CHANNEL(39),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 40
              #if TSS_DETECT_EL_METHOD(40,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,40), TSS_ELECTRODE_BIT_NUM(40), TSS_TIC_EL_TIMER_CHANNEL(40),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 41
              #if TSS_DETECT_EL_METHOD(41,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,41), TSS_ELECTRODE_BIT_NUM(41), TSS_TIC_EL_TIMER_CHANNEL(41),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 42
              #if TSS_DETECT_EL_METHOD(42,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,42), TSS_ELECTRODE_BIT_NUM(42), TSS_TIC_EL_TIMER_CHANNEL(42),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 43
              #if TSS_DETECT_EL_METHOD(43,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,43), TSS_ELECTRODE_BIT_NUM(43), TSS_TIC_EL_TIMER_CHANNEL(43),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 44
              #if TSS_DETECT_EL_METHOD(44,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,44), TSS_ELECTRODE_BIT_NUM(44), TSS_TIC_EL_TIMER_CHANNEL(44),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 45
              #if TSS_DETECT_EL_METHOD(45,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,45), TSS_ELECTRODE_BIT_NUM(45), TSS_TIC_EL_TIMER_CHANNEL(45),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 46
              #if TSS_DETECT_EL_METHOD(46,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,46), TSS_ELECTRODE_BIT_NUM(46), TSS_TIC_EL_TIMER_CHANNEL(46),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 47
              #if TSS_DETECT_EL_METHOD(47,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,47), TSS_ELECTRODE_BIT_NUM(47), TSS_TIC_EL_TIMER_CHANNEL(47),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 48
              #if TSS_DETECT_EL_METHOD(48,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,48), TSS_ELECTRODE_BIT_NUM(48), TSS_TIC_EL_TIMER_CHANNEL(48),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 49
              #if TSS_DETECT_EL_METHOD(49,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,49), TSS_ELECTRODE_BIT_NUM(49), TSS_TIC_EL_TIMER_CHANNEL(49),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 50
              #if TSS_DETECT_EL_METHOD(50,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,50), TSS_ELECTRODE_BIT_NUM(50), TSS_TIC_EL_TIMER_CHANNEL(50),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 51
              #if TSS_DETECT_EL_METHOD(51,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,51), TSS_ELECTRODE_BIT_NUM(51), TSS_TIC_EL_TIMER_CHANNEL(51),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 52
              #if TSS_DETECT_EL_METHOD(52,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,52), TSS_ELECTRODE_BIT_NUM(52), TSS_TIC_EL_TIMER_CHANNEL(52),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 53
              #if TSS_DETECT_EL_METHOD(53,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,53), TSS_ELECTRODE_BIT_NUM(53), TSS_TIC_EL_TIMER_CHANNEL(53),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 54
              #if TSS_DETECT_EL_METHOD(54,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,54), TSS_ELECTRODE_BIT_NUM(54), TSS_TIC_EL_TIMER_CHANNEL(54),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 55
              #if TSS_DETECT_EL_METHOD(55,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,55), TSS_ELECTRODE_BIT_NUM(55), TSS_TIC_EL_TIMER_CHANNEL(55),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 56
              #if TSS_DETECT_EL_METHOD(56,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,56), TSS_ELECTRODE_BIT_NUM(56), TSS_TIC_EL_TIMER_CHANNEL(56),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 57
              #if TSS_DETECT_EL_METHOD(57,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,57), TSS_ELECTRODE_BIT_NUM(57), TSS_TIC_EL_TIMER_CHANNEL(57),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 58
              #if TSS_DETECT_EL_METHOD(58,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,58), TSS_ELECTRODE_BIT_NUM(58), TSS_TIC_EL_TIMER_CHANNEL(58),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 59
              #if TSS_DETECT_EL_METHOD(59,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,59), TSS_ELECTRODE_BIT_NUM(59), TSS_TIC_EL_TIMER_CHANNEL(59),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 60
              #if TSS_DETECT_EL_METHOD(60,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,60), TSS_ELECTRODE_BIT_NUM(60), TSS_TIC_EL_TIMER_CHANNEL(60),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 61
              #if TSS_DETECT_EL_METHOD(61,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,61), TSS_ELECTRODE_BIT_NUM(61), TSS_TIC_EL_TIMER_CHANNEL(61),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 62
              #if TSS_DETECT_EL_METHOD(62,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,62), TSS_ELECTRODE_BIT_NUM(62), TSS_TIC_EL_TIMER_CHANNEL(62),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif
      
        #if TSS_N_ELECTRODES > 63
              #if TSS_DETECT_EL_METHOD(63,TIC)
                const TSS_TIC_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { &TSS_GET_TIC_METHOD_DATA_CONTEXT(ROM,63), TSS_ELECTRODE_BIT_NUM(63), TSS_TIC_EL_TIMER_CHANNEL(63),
                                                                                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                                                                                       TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                                                                                 #endif
                                                                                                                                 };
                              TSS_TIC_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
              #endif
        #endif

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
      UINT8 TIC_MethodControl(UINT8 u8ElNum, UINT8 u8Command)
      {
          UINT8 u8result = TSS_INIT_STATUS_OK;
          UINT8 u8ElCounter;
          typedef UINT8 * TSS_MXC_MemMapPtr;
          
          TSS_TIC_METHOD_ROMDATA *psMethodROMDataStruct;
          TSS_TIC_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;

          FTM_MemMapPtr psTimerStruct;
          PCTL_MemMapPtr psPCTLStruct;
          PT_MemMapPtr psPortStruct;
          TSS_MXC_MemMapPtr psMXCStruct;

          /* Pointers Decoding */      
          psElectrodeROMDataStruct = (TSS_TIC_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElNum]);
          psMethodROMDataStruct = (TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu8MethodROMData);
          
          psTimerStruct = (FTM_MemMapPtr) (psMethodROMDataStruct->tic_cpsu8Timer);
          psPCTLStruct = (PCTL_MemMapPtr) (psMethodROMDataStruct->tic_cpsu8PCTL);
          psPortStruct = (PT_MemMapPtr) (psMethodROMDataStruct->tic_cpsu8Port);
          psMXCStruct = (TSS_MXC_MemMapPtr) (psMethodROMDataStruct->tic_cpsu8MXC);
      
          /************* Do TIC Init *******************/
          if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
          {    
            /* Timer Init */        
            psTimerStruct->SC = 0u; /* Reset Timer */
            psTimerStruct->CNT = 0u; /* Reset Counter */
            psTimerStruct->SC |= (TSS_SENSOR_PRESCALER & TSS_TIC_TIMER_PRESCALE_MASK); /* Set Prescaler */
            psTimerStruct->SC |= TSS_TIC_TIMER_IE; /* Enable Interrupt */
            psTimerStruct->MOD = TSS_SENSOR_TIMEOUT; /* Set MOD Register */

            for(u8ElCounter = 0; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
            {
                if ((UINT32**) psMethodROMDataStruct == (UINT32 **) ((UINT32 *)tss_acp8ElectrodeROMData[u8ElCounter])[0]) 
                {
                  /* Noise Amplitude Filter Initialization */
                  #if TSS_USE_NOISE_AMPLITUDE_FILTER
                    ((TSS_TIC_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElCounter]))->tic_u16AmplitudeFilterBase = 0u;
                  #endif
                  
                  /* Calculate Pointer to the Module */ 
                  psElectrodeROMDataStruct = (TSS_TIC_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]);
                  /* Init MXC settings */    
                  if (psElectrodeROMDataStruct->tic_cu8PortBit % 2u)
                  {	  
                    /* Erase MXC */
                    psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] &= 0x0Fu;
                    /* Set PIN as GPIO */
                    psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] |= 0x10u; /* Set GPIO function for PIN */
                  } else {
                    /* Erase MXC */        	  
                    psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] &= 0xF0u;
                    /* Set PIN as GPIO */
                    psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] |= 0x01u; /* Set GPIO function for PIN */
                  } 	
                  /* Set PIN Strength if enabled */
                  #if TSS_USE_GPIO_STRENGTH
                    psPCTLStruct->DS |= (1u << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN Strength*/ 
                  #endif
                  /* Set PIN SlewRate if enabled */
                  #if TSS_USE_GPIO_SLEW_RATE
                    psPCTLStruct->SRE |= (1u << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN SlewRate*/ 
                  #endif
                  /* Set Default PIN State */
                  psPortStruct->DD |= (1u << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN to OUTPUT */
                  #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
                    psPortStruct->D &= ~(1u << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set LOG0 to OUTPUT pin */      
                  #else
                    psPortStruct->D |= (1u << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set LOG1 to OUTPUT pin */
                  #endif
                } 
            }
          } 
          /* Exit */                   
          return u8result;
      }

      /***************************************************************************//*!
      *
      * @brief  Performs a valid reading of one electrode stores the timer values 
      *         and returns a status code
      *
      * @param  u8ElecNum Electrode number to be scanned
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TIC_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
      {
        typedef UINT8 * TSS_MXC_MemMapPtr;
              
        UINT8 u8NSamp;
        UINT8 u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;   /* Sets default status */
        UINT16 u16CapSubSample;
        UINT8 u8FaultCnt;
        UINT8 u8MXCOffxet;
        TSS_TIC_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
        FTM_MemMapPtr psTimerStruct;
        PT_MemMapPtr psPortStruct;
        PCTL_MemMapPtr psPCTLStruct;
        TSS_MXC_MemMapPtr psMXCStruct;
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
              INT8 i8AmplitudeFilterDeltaLimitTemp;
              TSS_TIC_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
        #endif

        if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
        {
              u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;           
        } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
        {  
              /* Pointers Decoding */      
              psElectrodeROMDataStruct = (TSS_TIC_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
              psTimerStruct = (FTM_MemMapPtr) (((TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu8MethodROMData))->tic_cpsu8Timer);
              psPortStruct = (PT_MemMapPtr) (((TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu8MethodROMData))->tic_cpsu8Port);
              psPCTLStruct = (PCTL_MemMapPtr) (((TSS_TIC_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tic_cpsu8MethodROMData))->tic_cpsu8PCTL);
              psMXCStruct = (TSS_MXC_MemMapPtr) ((TSS_TIC_METHOD_ROMDATA *) ((psElectrodeROMDataStruct->tic_cpsu8MethodROMData))->tic_cpsu8MXC);		
              #if TSS_USE_NOISE_AMPLITUDE_FILTER
                psElectrodeRAMDataStruct = (TSS_TIC_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
              #endif
         
              tss_psElectrodeTimertAdr = (UINT32 *) psTimerStruct;
              tss_psElectrodePortAdr = (UINT32 *) psPortStruct;
              tss_psElectrodePCTLAdr = (UINT32 *) psPCTLStruct;
              tss_u8ElectrodeBitNum = psElectrodeROMDataStruct->tic_cu8PortBit;
              
              /* Initialization of variables */
              tss_u8HWTimerFlag = 0u;
              tss_u16CapSample = 0u;
              u8FaultCnt = 0u;
              u8NSamp = tss_CSSys.NSamples;

              #if TSS_USE_NOISE_AMPLITUDE_FILTER
                if (psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase == 0u) {
                      i8AmplitudeFilterDeltaLimitTemp = 127;
                } else {
                      i8AmplitudeFilterDeltaLimitTemp = psElectrodeROMDataStruct->tic_ci8AmplitudeFilterDeltaLimit;
                }
              #endif

              /* Main oversampling measurement loop */
              while(u8NSamp)
              {
                  TSS_CLEAR_SAMPLE_INTERRUPTED_FLAG();        /* Clears the interrupt sample flag */

                  /********************** TIC Measure Electrode ************************/
                  if (psElectrodeROMDataStruct->tic_cu8PortBit % 2u)
                  {	  
                    u8MXCOffxet = 4u;
                  } else {
                        u8MXCOffxet = 0u;			  
                  } 
                  /* Discharge pin */
                  psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] &= (0xF0u >> u8MXCOffxet); /* Erase MXC */
                  psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] |= (0x01u << u8MXCOffxet); /* Set GPIO function for PIN */
                  psPortStruct->DD |= (1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN to OUTPUT */
                  psPortStruct->D &= ~(1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set LOG0 to OUTPUT pin */      
                  /* Start Timer */
                  psTimerStruct->SC |= TSS_TIC_TIMER_ON;
                  /* Enable Timer Input Capture */
                  psTimerStruct->CHANNEL[psElectrodeROMDataStruct->tic_cu8TimerChannel].CnSC = TSS_TIC_TIMER_CH_ICM_EN;
                  /* Set MXC */        	  
                  psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] &= (0xF0u >> u8MXCOffxet);
                  psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] |= (0x04u << u8MXCOffxet); /* Set FTM channel function for PIN */
                  /* Timer Reset */
                  psTimerStruct->CNT = 0x0000u;
                  /* Measurement Loop */
                  while(!(psTimerStruct->CHANNEL[psElectrodeROMDataStruct->tic_cu8TimerChannel].CnSC & TSS_TIC_TIMER_CH_FLAG) && !TSS_FAULT_DETECTED)
                  {
                    /* MISRA Rule 14.8 */
                  }
                  /* Charge pin */
                  psPortStruct->DD &= ~(1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN to INPUT */  
                  /* Disable Timer Input Capture */
                  psTimerStruct->CHANNEL[psElectrodeROMDataStruct->tic_cu8TimerChannel].CnSC = 0x00u;
                  /* Set MXC */        	  
                  psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] &= (0xF0u >> u8MXCOffxet);
                  psMXCStruct[3u-(psElectrodeROMDataStruct->tic_cu8PortBit / 2u)] |= (0x01u << u8MXCOffxet); /* Set GPIO function for PIN */
                  /* Clear Timer Flag */
                  psTimerStruct->SC &= TSS_TIC_TIMER_FLAG_ACK;
                  /* Stop Timer */
                  psTimerStruct->SC &= TSS_TIC_TIMER_OFF;
                  /* Read Value */
                  u16CapSubSample = (UINT16) psTimerStruct->CHANNEL[psElectrodeROMDataStruct->tic_cu8TimerChannel].CnV;   
                  #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
                    /* Discharge pin */
                    psPortStruct->DD |= (1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN to OUTPUT */
                    psPortStruct->D &= ~(1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set LOG0 to OUTPUT pin */      
                  #endif
                  
                  /******************** End of TIC Measure Electrode **********************/          
                  
                  if(!TSS_FAULT_DETECTED)                     /* Verifies that no fault has occur */
                  {            
                    if(!TSS_SAMPLE_INTERRUPTED)             /* Verifies that the sample has not been inturrupted*/
                    {
                      #if TSS_USE_NOISE_AMPLITUDE_FILTER
                          if (TSS_u16NoiseAmplitudeFilter(u16CapSubSample, &(psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase), i8AmplitudeFilterDeltaLimitTemp)) 
                          {
                            u8FaultCnt++;                                      
                          } 
                          else 
                          { 
                            tss_u16CapSample += u16CapSubSample;
                            u8NSamp--;
                            u8FaultCnt = 0u;                 /* Restart Fault counter due long cyclic interrupts */
                          }
                            
                      #else
                          tss_u16CapSample += u16CapSubSample;
                          u8NSamp--;
                          u8FaultCnt = 0u;                   /* Restart Fault counter due long cyclic interrupts */
                      #endif  
                    }
                    else
                    {    
                      /* Do nothing if interrupt occured and do sample again in next loop*/
                      u8FaultCnt++; 
                    }
                  }
                  else
                  {
                    /* Set PIN to OUTPUT LOW State */
                    psPortStruct->DD |= (1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set PIN to OUTPUT */
                    psPortStruct->D &= ~(1 << psElectrodeROMDataStruct->tic_cu8PortBit); /* Set LOG0 to OUTPUT pin */      

                    u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
                    u8NSamp = 0u;                                  
                  }
                  
                  if(u8FaultCnt >= TSS_FAULT_TIMEOUT) {
                    u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT; /* If a fault has occurred sets the return status code error */
                    u8NSamp = 0u; 
                  }        
              }

              if ((tss_u16CapSample < TSS_KEYDETECT_CAP_LOWER_LIMIT) && (u8ElectrodeStatus != TSS_SAMPLE_ERROR_CHARGE_TIMEOUT)) 
              {
                u8ElectrodeStatus = TSS_SAMPLE_ERROR_SMALL_CAP;
              }
              
              #if TSS_USE_NOISE_AMPLITUDE_FILTER
                if (u8ElectrodeStatus == TSS_SAMPLE_STATUS_OK) {
                  if (psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase == 0u) {
                    psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase = (UINT16) (tss_u16CapSample / tss_CSSys.NSamples);
                  }
                } else {
                  psElectrodeRAMDataStruct->tic_u16AmplitudeFilterBase = 0u; /* Initiate Amplitude Filter Base refresh */
                }
              #endif
        } else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
        {
              /* Return the same electrode number because there is no more electrodes in the module */
              u8ElectrodeStatus = u8ElecNum;
        } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
        {
              /* Do Nothing because the Command is no relevant for TIC Method */
              u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;
        } 
        
        return u8ElectrodeStatus;   /* Return status code */ 
      }
                        
      /***************************************************************************//*!
      *
      * @brief  TPMx/FTMx Overflow Interrupt handlers
      *
      * @param  void
      *
      * @return void
      *
      * @remarks
      *
      ****************************************************************************/

      #if TSS_DETECT_MODULE(TPMTIC)
        #if !TSS_DETECT_HW_TIMER(TPM)               /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_TPMIsr to vector Vtpmfault_ovf  
              #endif
          interrupt
              void TSS_TPMIsr(void)
              {
                /* Clear TImer Flag */
                ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif

      #if TSS_DETECT_MODULE(TPM0TIC)
        #if !TSS_DETECT_HW_TIMER(TPM0)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_TPM0Isr to vector Vtpm0fault_ovf  
              #endif
          interrupt	  
              void TSS_TPM0Isr(void)
              {
                /* Clear TImer Flag */
                ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM1TIC)
        #if !TSS_DETECT_HW_TIMER(TPM1)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_TPM1Isr to vector Vtpm1fault_ovf  
              #endif
          interrupt	  
              void TSS_TPM1Isr(void)
              {
                /* Clear TImer Flag */
                ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(TPM2TIC)                /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if !TSS_DETECT_HW_TIMER(TPM2)    
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_TPM2Isr to vector Vtpm2fault_ovf  
              #endif
          interrupt	  
              void TSS_TPM2Isr(void)
              {
                /* Clear TImer Flag */
                ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif    
      #endif

      #if TSS_DETECT_MODULE(TPM3TIC)                /* Don't use this Isr if TSS_HW_TIMER Isr is used */
        #if !TSS_DETECT_HW_TIMER(TPM3)    
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_TPM3Isr to vector Vtpm3fault_ovf  
              #endif
          interrupt	  
              void TSS_TPM3Isr(void)
              {
                /* Clear TImer Flag */
                ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif

      #if TSS_DETECT_MODULE(TPM4TIC)
        #if !TSS_DETECT_HW_TIMER(TPM4)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_TPM4Isr to vector Vtpm4fault_ovf  
              #endif
          interrupt	  
              void TSS_TPM4Isr(void)
              {
                /* Clear TImer Flag */
                ((TPM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif

      #if TSS_DETECT_MODULE(FTMTIC)
        #if !TSS_DETECT_HW_TIMER(FTM)               /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_FTMIsr to vector Vftmfault_ovf  
              #endif
          interrupt	  
              void TSS_FTMIsr(void)
              {
                /* Clear TImer Flag */
                ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif

      #if TSS_DETECT_MODULE(FTM0TIC)
        #if !TSS_DETECT_HW_TIMER(FTM0)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */  
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_FTM0Isr to vector Vftm0fault_ovf  
              #endif
          interrupt
              void TSS_FTM0Isr(void)
              {
                /* Clear TImer Flag */
                ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                      
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif    
      #endif
      
      #if TSS_DETECT_MODULE(FTM1TIC)
        #if !TSS_DETECT_HW_TIMER(FTM1)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_FTM1Isr to vector Vftm1fault_ovf  
              #endif
          interrupt
              void TSS_FTM1Isr(void)
              {
                /* Clear TImer Flag */
                ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif
      
      #if TSS_DETECT_MODULE(FTM2TIC)
        #if !TSS_DETECT_HW_TIMER(FTM2)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_FTM2Isr to vector Vftm2fault_ovf  
              #endif
          interrupt
              void TSS_FTM2Isr(void)
              {
                /* Clear TImer Flag */
                ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif

      #if TSS_DETECT_MODULE(FTM3TIC)
        #if !TSS_DETECT_HW_TIMER(FTM3)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_FTM2Isr to vector Vftm3fault_ovf  
              #endif
          interrupt
              void TSS_FTM3Isr(void)
              {
                /* Clear TImer Flag */
                ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif

      #if TSS_DETECT_MODULE(FTM4TIC)
        #if !TSS_DETECT_HW_TIMER(FTM4)              /* Don't use this Isr if TSS_HW_TIMER Isr is used */
              #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
                #warning TSS - Please assign ISR function TSS_FTM2Isr to vector Vftm4fault_ovf  
              #endif
          interrupt
              void TSS_FTM4Isr(void)
              {
                /* Clear TImer Flag */
                ((FTM_MemMapPtr) tss_psElectrodeTimertAdr)->SC &= TSS_HW_TPMFTM_FLAG_ACK;         /* Clear Timer Flag */
                
                tss_u8HWTimerFlag = 1u;                 /* Set measurement Overflow Flag variable */
              }
        #endif
      #endif
  #else /* End of TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    
           
            
#endif /* End of TSS_DETECT_METHOD(TIC) */
