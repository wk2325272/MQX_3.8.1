/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorGPIO.c
*
* @brief  Contains functions to Perform the sensing of the electrodes and set the status for each electrode
*
* @version 1.0.23.0
* 
* @date Mar-12-2012
* 
*
***********************************************************************************************************************/

#include "TSS_SensorGPIO.h"

#if TSS_DETECT_METHOD(GPIO)

  #include "TSS_Timer.h"
  #include "TSS_GPIO.h"

  #if TSS_HCS08_MCU || TSS_CFV1_MCU             

    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(0),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(1),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(2),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(3),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(4),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(5),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(6),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(7),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(8),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(9),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(10),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(11),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(12),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(13),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(14),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(15),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(16),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(17),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(18),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(19),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(20),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(21),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(22),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(23),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(24),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(25),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(26),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(27),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(28),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(29),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(30),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(31),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(32),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(33),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(34),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(35),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(36),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(37),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(38),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(39),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(40),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(41),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(42),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(43),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(44),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(45),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(46),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(47),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(48),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(49),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(50),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(51),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(52),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(53),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(54),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(55),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(56),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(57),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(58),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(59),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(60),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(61),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(62),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(63),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
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
    UINT8 GPIO_SensorInit(void)
    {
        UINT8 u8result = TSS_INIT_STATUS_OK;

        #if TSS_DETECT_METHOD(GPIO)

        #endif
   
        return u8result;
    }
    
  #elif TSS_KINETIS_MCU

    /************************** Prototypes ***************************/

    UINT8 GPIO_MethodControl(UINT8 u8ElNum, UINT8 u8Command);     

    /**********************  Modules definition **********************/
    
    #if TSS_DETECT_PORT_METHOD(A,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTA_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTA_BASE_PTR, (UINT32*) PTA_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif
    #if TSS_DETECT_PORT_METHOD(B,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTB_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTB_BASE_PTR, (UINT32*) PTB_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif
    #if TSS_DETECT_PORT_METHOD(C,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTC_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTC_BASE_PTR, (UINT32*) PTC_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif
    #if TSS_DETECT_PORT_METHOD(D,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTD_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTD_BASE_PTR, (UINT32*) PTD_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif
    #if TSS_DETECT_PORT_METHOD(E,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTE_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTE_BASE_PTR, (UINT32*) PTE_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif  
    #if TSS_DETECT_PORT_METHOD(F,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTF_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTF_BASE_PTR, (UINT32*) PTF_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif  
    #if TSS_DETECT_PORT_METHOD(G,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTG_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTG_BASE_PTR, (UINT32*) PTG_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif  
    #if TSS_DETECT_PORT_METHOD(H,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTH_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTH_BASE_PTR, (UINT32*) PTH_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif  
    #if TSS_DETECT_PORT_METHOD(I,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTI_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTI_BASE_PTR, (UINT32*) PTI_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif  
    #if TSS_DETECT_PORT_METHOD(J,GPIO) 
      const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTJ_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT32*) TSS_HW_TIMER_PTR, (UINT32*) PORTJ_BASE_PTR, (UINT32*) PTJ_BASE_PTR, TSS_HW_TIMER_VECTOR-16}; 
    #endif  
        
    /********************* Method Memory Data Decoding Macros **********/
        
    #define TSS_GET_GPIO_METHOD_DATA_CONTEXT_RESULT(memory,res)     TSS_GPIO_PORT##res##_METHOD_##memory##DATA_CONTEXT
    #define TSS_GET_GPIO_METHOD_DATA_CONTEXT_CONV(memory,text)      TSS_GET_GPIO_METHOD_DATA_CONTEXT_RESULT(memory,text)
    #define TSS_GET_GPIO_METHOD_DATA_CONTEXT_TYPE(memory,elec)      TSS_GET_GPIO_METHOD_DATA_CONTEXT_CONV(memory,elec)

    #define TSS_GET_GPIO_METHOD_DATA_CONTEXT(memory,el)             TSS_GET_GPIO_METHOD_DATA_CONTEXT_TYPE(memory,TSS_E##el##_P)
        
    /***************** ROM & RAM Data definition ***********************/
  
    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,0), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(0), TSS_ELECTRODE_BIT_NUM(0),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,1), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(1), TSS_ELECTRODE_BIT_NUM(1),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,2), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(2), TSS_ELECTRODE_BIT_NUM(2),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,3), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(3), TSS_ELECTRODE_BIT_NUM(3),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,4), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(4), TSS_ELECTRODE_BIT_NUM(4),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,5), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(5), TSS_ELECTRODE_BIT_NUM(5),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,6), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(6), TSS_ELECTRODE_BIT_NUM(6),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,7), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(7), TSS_ELECTRODE_BIT_NUM(7),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,8), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(8), TSS_ELECTRODE_BIT_NUM(8),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,9), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(9), TSS_ELECTRODE_BIT_NUM(9),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,10), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(10), TSS_ELECTRODE_BIT_NUM(10),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,11), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(11), TSS_ELECTRODE_BIT_NUM(11),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,12), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(12), TSS_ELECTRODE_BIT_NUM(12),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,13), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(13), TSS_ELECTRODE_BIT_NUM(13),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,14), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(14), TSS_ELECTRODE_BIT_NUM(14),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,15), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(15), TSS_ELECTRODE_BIT_NUM(15),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,16), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(16), TSS_ELECTRODE_BIT_NUM(16),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,17), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(17), TSS_ELECTRODE_BIT_NUM(17),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,18), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(18), TSS_ELECTRODE_BIT_NUM(18),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,19), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(19), TSS_ELECTRODE_BIT_NUM(19),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,20), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(20), TSS_ELECTRODE_BIT_NUM(20),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,21), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(21), TSS_ELECTRODE_BIT_NUM(21),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,22), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(22), TSS_ELECTRODE_BIT_NUM(22),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,23), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(23), TSS_ELECTRODE_BIT_NUM(23),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,24), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(24), TSS_ELECTRODE_BIT_NUM(24),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,25), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(25), TSS_ELECTRODE_BIT_NUM(25),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,26), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(26), TSS_ELECTRODE_BIT_NUM(26),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,27), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(27), TSS_ELECTRODE_BIT_NUM(27),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,28), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(28), TSS_ELECTRODE_BIT_NUM(28),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,29), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(29), TSS_ELECTRODE_BIT_NUM(29),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,30), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(30), TSS_ELECTRODE_BIT_NUM(30),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,31), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(31), TSS_ELECTRODE_BIT_NUM(31),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,32), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(32), TSS_ELECTRODE_BIT_NUM(32),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,33), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(33), TSS_ELECTRODE_BIT_NUM(33),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,34), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(34), TSS_ELECTRODE_BIT_NUM(34),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,35), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(35), TSS_ELECTRODE_BIT_NUM(35),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,36), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(36), TSS_ELECTRODE_BIT_NUM(36),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,37), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(37), TSS_ELECTRODE_BIT_NUM(37),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,38), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(38), TSS_ELECTRODE_BIT_NUM(38),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,39), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(39), TSS_ELECTRODE_BIT_NUM(39),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,40), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(40), TSS_ELECTRODE_BIT_NUM(40),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,41), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(41), TSS_ELECTRODE_BIT_NUM(41),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,42), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(42), TSS_ELECTRODE_BIT_NUM(42),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,43), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(43), TSS_ELECTRODE_BIT_NUM(43),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,44), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(44), TSS_ELECTRODE_BIT_NUM(44),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,45), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(45), TSS_ELECTRODE_BIT_NUM(45),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,46), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(46), TSS_ELECTRODE_BIT_NUM(46),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,47), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(47), TSS_ELECTRODE_BIT_NUM(47),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,48), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(48), TSS_ELECTRODE_BIT_NUM(48),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,49), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(49), TSS_ELECTRODE_BIT_NUM(49),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,50), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(50), TSS_ELECTRODE_BIT_NUM(50),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,51), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(51), TSS_ELECTRODE_BIT_NUM(51),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,52), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(52), TSS_ELECTRODE_BIT_NUM(52),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,53), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(53), TSS_ELECTRODE_BIT_NUM(53),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,54), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(54), TSS_ELECTRODE_BIT_NUM(54),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,55), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(55), TSS_ELECTRODE_BIT_NUM(55),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,56), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(56), TSS_ELECTRODE_BIT_NUM(56),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,57), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(57), TSS_ELECTRODE_BIT_NUM(57),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,58), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(58), TSS_ELECTRODE_BIT_NUM(58),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,59), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(59), TSS_ELECTRODE_BIT_NUM(59),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,60), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(60), TSS_ELECTRODE_BIT_NUM(60),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,61), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(61), TSS_ELECTRODE_BIT_NUM(61),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,62), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(62), TSS_ELECTRODE_BIT_NUM(62),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif
  
    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,GPIO)
        const TSS_GPIO_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,63), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(63), TSS_ELECTRODE_BIT_NUM(63),
                                                                   #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                     TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                   #endif
                                                                   };
              TSS_GPIO_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
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
    UINT8 GPIO_MethodControl(UINT8 u8ElNum, UINT8 u8Command)
    {
      UINT8 u8result = TSS_INIT_STATUS_OK;
      UINT8 u8ElCounter;
      
      TSS_GPIO_METHOD_ROMDATA *psMethodROMDataStruct;
      TSS_GPIO_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;

      FTM_MemMapPtr psTimerStruct;
      PORT_MemMapPtr psPortStruct;
      RGPIO_MemMapPtr psRGPIOStruct;
      NVIC_MemMapPtr psNVICStruct = NVIC_BASE_PTR;

      /* Pointers Decoding */      
      psElectrodeROMDataStruct = (TSS_GPIO_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElNum]);
      psMethodROMDataStruct = (TSS_GPIO_METHOD_ROMDATA *) (psElectrodeROMDataStruct->gpio_cpsu32MethodROMData);
      
      psTimerStruct = (FTM_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu32Timer);
      psPortStruct = (PORT_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu32Port);
      psRGPIOStruct = (RGPIO_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu32RGPIO);

      /************* Do GPIO Init *******************/
      if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
      {    
        /* HW Timer Init */        
        psTimerStruct->SC = 0u; /* Reset Timer */
        psTimerStruct->CNT = 0u; /* Reset Counter */
        psTimerStruct->SC |= (TSS_SENSOR_PRESCALER & TSS_HW_TPMFTM_PRESCALE_MASK); /* Set Prescaler */
        psTimerStruct->SC |= TSS_HW_TPMFTM_IE; /* Enable Interrupt */
        psTimerStruct->MOD = TSS_SENSOR_TIMEOUT; /* Set MOD Register */
        /* Enable HW Timer Interrupt */
        psNVICStruct->ISER[psMethodROMDataStruct->gpio_u8TimerIRQNum / 32] = 1 << (psMethodROMDataStruct->gpio_u8TimerIRQNum % 32);

        for(u8ElCounter = 0; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
        {
          if ((UINT32**) psMethodROMDataStruct == (UINT32 **) ((UINT32 *)tss_acp8ElectrodeROMData[u8ElCounter])[0]) 
          {
            /* Noise Amplitude Filter Initialization */
            #if TSS_USE_NOISE_AMPLITUDE_FILTER
              ((TSS_GPIO_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElCounter]))->gpio_u16AmplitudeFilterBase = 0u;
            #endif
            
            /* Calculate Pointer to the Module */ 
            psElectrodeROMDataStruct = (TSS_GPIO_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]);
            /* Init GPIO settings */    
            psPortStruct->PCR[psElectrodeROMDataStruct->gpio_cu32PortBit] = 0u; /* Erase PCR */
            /* Set PIN as GPIO */
            psPortStruct->PCR[psElectrodeROMDataStruct->gpio_cu32PortBit] |= PORT_PCR_MUX(0x01u); /* Set GPIO function for PIN */
            /* Set PIN Strength if enabled */
            #if TSS_USE_GPIO_STRENGTH
              psPortStruct->PCR[psElectrodeROMDataStruct->gpio_cu32PortBit] |= PORT_PCR_DSE_MASK; /* Set PIN Strength*/
            #endif
            /* Set PIN SlewRate if enabled */
            #if TSS_USE_GPIO_SLEW_RATE
              psPortStruct->PCR[psElectrodeROMDataStruct->gpio_cu32PortBit] |= PORT_PCR_SRE_MASK; /* Set PIN SlewRate*/
            #endif
            /* Set Default PIN State */
            psRGPIOStruct->PDDR |= (1 << psElectrodeROMDataStruct->gpio_cu32PortBit); /* Set PIN to OUTPUT */
            #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
              psRGPIOStruct->PCOR = (1 << psElectrodeROMDataStruct->gpio_cu32PortBit);  /* Set LOG0 to OUTPUT pin */      
            #else
              psRGPIOStruct->PSOR = (1 << psElectrodeROMDataStruct->gpio_cu32PortBit);  /* Set LOG1 to OUTPUT pin */      
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
    UINT8 GPIO_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
    {
      UINT8 u8NSamp;
      UINT8 u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;   /* Sets default status */
      UINT16 u16CapSubSample;
      UINT8 u8FaultCnt;
      TSS_GPIO_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
      FTM_MemMapPtr psTimerStruct;
      RGPIO_MemMapPtr psRGPIOStruct;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        INT8 i8AmplitudeFilterDeltaLimitTemp;
        TSS_GPIO_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
      #endif

      if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
      {
        u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;           
      } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
      {  
        /* Pointers Decoding */      
        psElectrodeROMDataStruct = (TSS_GPIO_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
        psTimerStruct = (FTM_MemMapPtr) (((TSS_GPIO_METHOD_ROMDATA *) (psElectrodeROMDataStruct->gpio_cpsu32MethodROMData))->gpio_cpsu32Timer);
        psRGPIOStruct = (RGPIO_MemMapPtr) (((TSS_GPIO_METHOD_ROMDATA *) (psElectrodeROMDataStruct->gpio_cpsu32MethodROMData))->gpio_cpsu32RGPIO);
        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          psElectrodeRAMDataStruct = (TSS_GPIO_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
        #endif
       
        tss_psElectrodeTimertAdr = (UINT32 *) psTimerStruct;
        tss_psElectrodeRGPIOAdr = (UINT32 *) psRGPIOStruct;
        tss_u8ElectrodeBitNum = psElectrodeROMDataStruct->gpio_cu32PortBit;
        
        /* Initialization of variables */
        tss_u8HWTimerFlag = 0u;
        tss_u16CapSample = 0u;
        u8FaultCnt = 0u;
        u8NSamp = tss_CSSys.NSamples;

        #if TSS_USE_NOISE_AMPLITUDE_FILTER
          if (psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase == 0u) {
            i8AmplitudeFilterDeltaLimitTemp = 127;
          } else {
            i8AmplitudeFilterDeltaLimitTemp = psElectrodeROMDataStruct->gpio_ci8AmplitudeFilterDeltaLimit;
          }
        #endif

        /* Main oversampling measurement loop */
        while(u8NSamp)
        {
            psTimerStruct->SC &= TSS_HW_TPMFTM_OFF; /* Stop Timer */
            psTimerStruct->CNT = 0u; /* Reset Counter i.e. Reset Timer*/

            TSS_CLEAR_SAMPLE_INTERRUPTED_FLAG();        /* Clears the interrupt sample flag */

            u16CapSubSample = (psElectrodeROMDataStruct->gpio_fSampleElectrodeLow)();    /* Samples one electrode */
            
            if(!TSS_FAULT_DETECTED)                     /* Verifies that no fault has occur */
            {            
                if(!TSS_SAMPLE_INTERRUPTED)             /* Verifies that the sample has not been inturrupted*/
                {
                  #if TSS_USE_NOISE_AMPLITUDE_FILTER
                    if (TSS_u16NoiseAmplitudeFilter(u16CapSubSample, &(psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase), i8AmplitudeFilterDeltaLimitTemp)) 
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
              psRGPIOStruct->PDDR |= (1 << psElectrodeROMDataStruct->gpio_cu32PortBit); /* Set PIN to OUTPUT */
              psRGPIOStruct->PCOR = (1 << psElectrodeROMDataStruct->gpio_cu32PortBit);  /* Set LOG0 to OUTPUT pin */      

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
            if (psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase == 0u) {
              psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase = (UINT16) (tss_u16CapSample / tss_CSSys.NSamples);
            }
          } else {
            psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase = 0u; /* Initiate Amplitude Filter Base refresh */
          }
        #endif
      } else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
      {
        /* Return the same electrode number because there is no more electrodes in the module */
        u8ElectrodeStatus = u8ElecNum;
      } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
      {
        /* Do Nothing because the Command is no relevant for GPIO Method */
        u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;
      } 
      
      return u8ElectrodeStatus;   /* Return status code */ 
    }
#elif TSS_CFM_MCU

  /************************** Prototypes ***************************/

  UINT8 GPIO_MethodControl(UINT8 u8ElNum, UINT8 u8Command);     

  /**********************  Modules definition **********************/
  
  #if TSS_DETECT_PORT_METHOD(A,GPIO) 
    const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTA_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT8*) TSS_HW_TIMER_PTR, (UINT8*) PCTLA_BASE_PTR, (UINT8*) PTA_BASE_PTR, (UINT8*) &MXC_PTAPF1}; 
  #endif
  #if TSS_DETECT_PORT_METHOD(B,GPIO) 
    const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTB_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT8*) TSS_HW_TIMER_PTR, (UINT8*) PCTLB_BASE_PTR, (UINT8*) PTB_BASE_PTR, (UINT8*) &MXC_PTBPF1}; 
  #endif
  #if TSS_DETECT_PORT_METHOD(C,GPIO) 
    const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTC_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT8*) TSS_HW_TIMER_PTR, (UINT8*) PCTLC_BASE_PTR, (UINT8*) PTC_BASE_PTR, (UINT8*) &MXC_PTCPF1}; 
  #endif
  #if TSS_DETECT_PORT_METHOD(D,GPIO) 
    const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTD_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT8*) TSS_HW_TIMER_PTR, (UINT8*) PCTLD_BASE_PTR, (UINT8*) PTD_BASE_PTR, (UINT8*) &MXC_PTDPF1}; 
  #endif
  #if TSS_DETECT_PORT_METHOD(E,GPIO) 
    const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTE_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT8*) TSS_HW_TIMER_PTR, (UINT8*) PCTLE_BASE_PTR, (UINT8*) PTE_BASE_PTR, (UINT8*) &MXC_PTEPF1}; 
  #endif  
  #if TSS_DETECT_PORT_METHOD(F,GPIO) 
    const    TSS_GPIO_METHOD_ROMDATA TSS_GPIO_PORTF_METHOD_ROMDATA_CONTEXT = {GPIO_MethodControl, (UINT8*) TSS_HW_TIMER_PTR, (UINT8*) PCTLF_BASE_PTR, (UINT8*) PTF_BASE_PTR, (UINT8*) &MXC_PTFPF1}; 
  #endif  
      
  /********************* Method Memory Data Decoding Macros **********/
      
  #define TSS_GET_GPIO_METHOD_DATA_CONTEXT_RESULT(memory,res)     TSS_GPIO_PORT##res##_METHOD_##memory##DATA_CONTEXT
  #define TSS_GET_GPIO_METHOD_DATA_CONTEXT_CONV(memory,text)      TSS_GET_GPIO_METHOD_DATA_CONTEXT_RESULT(memory,text)
  #define TSS_GET_GPIO_METHOD_DATA_CONTEXT_TYPE(memory,elec)      TSS_GET_GPIO_METHOD_DATA_CONTEXT_CONV(memory,elec)

  #define TSS_GET_GPIO_METHOD_DATA_CONTEXT(memory,el)             TSS_GET_GPIO_METHOD_DATA_CONTEXT_TYPE(memory,TSS_E##el##_P)
      
  /***************** ROM & RAM Data definition ***********************/

  #if TSS_N_ELECTRODES > 0
    #if TSS_DETECT_EL_METHOD(0,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,0), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(0), TSS_ELECTRODE_BIT_NUM(0),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E0_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 1
    #if TSS_DETECT_EL_METHOD(1,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,1), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(1), TSS_ELECTRODE_BIT_NUM(1),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E1_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 2
    #if TSS_DETECT_EL_METHOD(2,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,2), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(2), TSS_ELECTRODE_BIT_NUM(2),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E2_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 3
    #if TSS_DETECT_EL_METHOD(3,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,3), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(3), TSS_ELECTRODE_BIT_NUM(3),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E3_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 4
    #if TSS_DETECT_EL_METHOD(4,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,4), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(4), TSS_ELECTRODE_BIT_NUM(4),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E4_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 5
    #if TSS_DETECT_EL_METHOD(5,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,5), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(5), TSS_ELECTRODE_BIT_NUM(5),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E5_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 6
    #if TSS_DETECT_EL_METHOD(6,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,6), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(6), TSS_ELECTRODE_BIT_NUM(6),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E6_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 7
    #if TSS_DETECT_EL_METHOD(7,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,7), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(7), TSS_ELECTRODE_BIT_NUM(7),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E7_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 8
    #if TSS_DETECT_EL_METHOD(8,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,8), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(8), TSS_ELECTRODE_BIT_NUM(8),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E8_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 9
    #if TSS_DETECT_EL_METHOD(9,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,9), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(9), TSS_ELECTRODE_BIT_NUM(9),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E9_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 10
    #if TSS_DETECT_EL_METHOD(10,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,10), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(10), TSS_ELECTRODE_BIT_NUM(10),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E10_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 11
    #if TSS_DETECT_EL_METHOD(11,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,11), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(11), TSS_ELECTRODE_BIT_NUM(11),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E11_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 12
    #if TSS_DETECT_EL_METHOD(12,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,12), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(12), TSS_ELECTRODE_BIT_NUM(12),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E12_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 13
    #if TSS_DETECT_EL_METHOD(13,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,13), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(13), TSS_ELECTRODE_BIT_NUM(13),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E13_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 14
    #if TSS_DETECT_EL_METHOD(14,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,14), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(14), TSS_ELECTRODE_BIT_NUM(14),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E14_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 15
    #if TSS_DETECT_EL_METHOD(15,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,15), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(15), TSS_ELECTRODE_BIT_NUM(15),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E15_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 16
    #if TSS_DETECT_EL_METHOD(16,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,16), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(16), TSS_ELECTRODE_BIT_NUM(16),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E16_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 17
    #if TSS_DETECT_EL_METHOD(17,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,17), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(17), TSS_ELECTRODE_BIT_NUM(17),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E17_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 18
    #if TSS_DETECT_EL_METHOD(18,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,18), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(18), TSS_ELECTRODE_BIT_NUM(18),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E18_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 19
    #if TSS_DETECT_EL_METHOD(19,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,19), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(19), TSS_ELECTRODE_BIT_NUM(19),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E19_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 20
    #if TSS_DETECT_EL_METHOD(20,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,20), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(20), TSS_ELECTRODE_BIT_NUM(20),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E20_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 21
    #if TSS_DETECT_EL_METHOD(21,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,21), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(21), TSS_ELECTRODE_BIT_NUM(21),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E21_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 22
    #if TSS_DETECT_EL_METHOD(22,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,22), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(22), TSS_ELECTRODE_BIT_NUM(22),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E22_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 23
    #if TSS_DETECT_EL_METHOD(23,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,23), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(23), TSS_ELECTRODE_BIT_NUM(23),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E23_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 24
    #if TSS_DETECT_EL_METHOD(24,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,24), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(24), TSS_ELECTRODE_BIT_NUM(24),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E24_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 25
    #if TSS_DETECT_EL_METHOD(25,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,25), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(25), TSS_ELECTRODE_BIT_NUM(25),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E25_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 26
    #if TSS_DETECT_EL_METHOD(26,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,26), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(26), TSS_ELECTRODE_BIT_NUM(26),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E26_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 27
    #if TSS_DETECT_EL_METHOD(27,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,27), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(27), TSS_ELECTRODE_BIT_NUM(27),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E27_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 28
    #if TSS_DETECT_EL_METHOD(28,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,28), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(28), TSS_ELECTRODE_BIT_NUM(28),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E28_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 29
    #if TSS_DETECT_EL_METHOD(29,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,29), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(29), TSS_ELECTRODE_BIT_NUM(29),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E29_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 30
    #if TSS_DETECT_EL_METHOD(30,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,30), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(30), TSS_ELECTRODE_BIT_NUM(30),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E30_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 31
    #if TSS_DETECT_EL_METHOD(31,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,31), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(31), TSS_ELECTRODE_BIT_NUM(31),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E31_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 32
    #if TSS_DETECT_EL_METHOD(32,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,32), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(32), TSS_ELECTRODE_BIT_NUM(32),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E32_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 33
    #if TSS_DETECT_EL_METHOD(33,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,33), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(33), TSS_ELECTRODE_BIT_NUM(33),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E33_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 34
    #if TSS_DETECT_EL_METHOD(34,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,34), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(34), TSS_ELECTRODE_BIT_NUM(34),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E34_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 35
    #if TSS_DETECT_EL_METHOD(35,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,35), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(35), TSS_ELECTRODE_BIT_NUM(35),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E35_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 36
    #if TSS_DETECT_EL_METHOD(36,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,36), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(36), TSS_ELECTRODE_BIT_NUM(36),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E36_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 37
    #if TSS_DETECT_EL_METHOD(37,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,37), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(37), TSS_ELECTRODE_BIT_NUM(37),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E37_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 38
    #if TSS_DETECT_EL_METHOD(38,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,38), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(38), TSS_ELECTRODE_BIT_NUM(38),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E38_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 39
    #if TSS_DETECT_EL_METHOD(39,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,39), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(39), TSS_ELECTRODE_BIT_NUM(39),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E39_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 40
    #if TSS_DETECT_EL_METHOD(40,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,40), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(40), TSS_ELECTRODE_BIT_NUM(40),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E40_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 41
    #if TSS_DETECT_EL_METHOD(41,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,41), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(41), TSS_ELECTRODE_BIT_NUM(41),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E41_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 42
    #if TSS_DETECT_EL_METHOD(42,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,42), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(42), TSS_ELECTRODE_BIT_NUM(42),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E42_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 43
    #if TSS_DETECT_EL_METHOD(43,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,43), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(43), TSS_ELECTRODE_BIT_NUM(43),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E43_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 44
    #if TSS_DETECT_EL_METHOD(44,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,44), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(44), TSS_ELECTRODE_BIT_NUM(44),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E44_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 45
    #if TSS_DETECT_EL_METHOD(45,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,45), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(45), TSS_ELECTRODE_BIT_NUM(45),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E45_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 46
    #if TSS_DETECT_EL_METHOD(46,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,46), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(46), TSS_ELECTRODE_BIT_NUM(46),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E46_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 47
    #if TSS_DETECT_EL_METHOD(47,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,47), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(47), TSS_ELECTRODE_BIT_NUM(47),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E47_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 48
    #if TSS_DETECT_EL_METHOD(48,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,48), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(48), TSS_ELECTRODE_BIT_NUM(48),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E48_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 49
    #if TSS_DETECT_EL_METHOD(49,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,49), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(49), TSS_ELECTRODE_BIT_NUM(49),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E49_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 50
    #if TSS_DETECT_EL_METHOD(50,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,50), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(50), TSS_ELECTRODE_BIT_NUM(50),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E50_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 51
    #if TSS_DETECT_EL_METHOD(51,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,51), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(51), TSS_ELECTRODE_BIT_NUM(51),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E51_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 52
    #if TSS_DETECT_EL_METHOD(52,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,52), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(52), TSS_ELECTRODE_BIT_NUM(52),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E52_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 53
    #if TSS_DETECT_EL_METHOD(53,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,53), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(53), TSS_ELECTRODE_BIT_NUM(53),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E53_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 54
    #if TSS_DETECT_EL_METHOD(54,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,54), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(54), TSS_ELECTRODE_BIT_NUM(54),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E54_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 55
    #if TSS_DETECT_EL_METHOD(55,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,55), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(55), TSS_ELECTRODE_BIT_NUM(55),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E55_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 56
    #if TSS_DETECT_EL_METHOD(56,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,56), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(56), TSS_ELECTRODE_BIT_NUM(56),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E56_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 57
    #if TSS_DETECT_EL_METHOD(57,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,57), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(57), TSS_ELECTRODE_BIT_NUM(57),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E57_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 58
    #if TSS_DETECT_EL_METHOD(58,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,58), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(58), TSS_ELECTRODE_BIT_NUM(58),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E58_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 59
    #if TSS_DETECT_EL_METHOD(59,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,59), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(59), TSS_ELECTRODE_BIT_NUM(59),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E59_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 60
    #if TSS_DETECT_EL_METHOD(60,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,60), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(60), TSS_ELECTRODE_BIT_NUM(60),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E60_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 61
    #if TSS_DETECT_EL_METHOD(61,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,61), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(61), TSS_ELECTRODE_BIT_NUM(61),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E61_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 62
    #if TSS_DETECT_EL_METHOD(62,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,62), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(62), TSS_ELECTRODE_BIT_NUM(62),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E62_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
    #endif
  #endif

  #if TSS_N_ELECTRODES > 63
    #if TSS_DETECT_EL_METHOD(63,GPIO)
      const TSS_GPIO_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { &TSS_GET_GPIO_METHOD_DATA_CONTEXT(ROM,63), TSS_GET_SAMPLE_ELECTRODE_LOW_ROUTINE(63), TSS_ELECTRODE_BIT_NUM(63),
                                                                 #if TSS_USE_NOISE_AMPLITUDE_FILTER
                                                                   TSS_E63_NOISE_AMPLITUDE_FILTER_HALF_SIZE,
                                                                 #endif
                                                                 };
            TSS_GPIO_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
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
  UINT8 GPIO_MethodControl(UINT8 u8ElNum, UINT8 u8Command)
  {
    UINT8 u8result = TSS_INIT_STATUS_OK;
    UINT8 u8ElCounter;
    typedef UINT8 * TSS_MXC_MemMapPtr;
    
    TSS_GPIO_METHOD_ROMDATA *psMethodROMDataStruct;
    TSS_GPIO_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;

    FTM_MemMapPtr psTimerStruct;
    PCTL_MemMapPtr psPCTLStruct;
    PT_MemMapPtr psPortStruct;
    TSS_MXC_MemMapPtr psMXCStruct;

    /* Pointers Decoding */      
    psElectrodeROMDataStruct = (TSS_GPIO_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElNum]);
    psMethodROMDataStruct = (TSS_GPIO_METHOD_ROMDATA *) (psElectrodeROMDataStruct->gpio_cpsu8MethodROMData);
    
    psTimerStruct = (FTM_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu8Timer);
    psPCTLStruct = (PCTL_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu8PCTL);
    psPortStruct = (PT_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu8Port);
    psMXCStruct = (TSS_MXC_MemMapPtr) (psMethodROMDataStruct->gpio_cpsu8MXC);
    
    /************* Do GPIO Init *******************/
    if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
    {    
      /* HW Timer Init */        
      psTimerStruct->SC = 0u; /* Reset Timer */
      psTimerStruct->CNT = 0u; /* Reset Counter */
      psTimerStruct->SC |= (TSS_SENSOR_PRESCALER & TSS_HW_TPMFTM_PRESCALE_MASK); /* Set Prescaler */
      psTimerStruct->SC |= TSS_HW_TPMFTM_IE; /* Enable Interrupt */
      psTimerStruct->MOD = TSS_SENSOR_TIMEOUT; /* Set MOD Register */

      for(u8ElCounter = 0; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
      {
        if ((UINT32**) psMethodROMDataStruct == (UINT32 **) ((UINT32 *)tss_acp8ElectrodeROMData[u8ElCounter])[0]) 
        {
          /* Noise Amplitude Filter Initialization */
          #if TSS_USE_NOISE_AMPLITUDE_FILTER
            ((TSS_GPIO_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElCounter]))->gpio_u16AmplitudeFilterBase = 0u;
          #endif
          
          /* Calculate Pointer to the Module */ 
          psElectrodeROMDataStruct = (TSS_GPIO_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]);
          /* Init MXC settings */    
          if (psElectrodeROMDataStruct->gpio_cu8PortBit % 2u)
          {	  
        	/* Erase PCR */
        	psMXCStruct[3u-(psElectrodeROMDataStruct->gpio_cu8PortBit / 2u)] &= 0x0Fu;
            /* Set PIN as GPIO */
            psMXCStruct[3u-(psElectrodeROMDataStruct->gpio_cu8PortBit / 2u)] |= 0x10u; /* Set GPIO function for PIN */
          } else {
        	/* Erase PCR */        	  
        	psMXCStruct[3u-(psElectrodeROMDataStruct->gpio_cu8PortBit / 2u)] &= 0xF0u; /* Erase PCR */
            /* Set PIN as GPIO */
            psMXCStruct[3u-(psElectrodeROMDataStruct->gpio_cu8PortBit / 2u)] |= 0x01u; /* Set GPIO function for PIN */
          } 	
          /* Set PIN Strength if enabled */
          #if TSS_USE_GPIO_STRENGTH
            psPCTLStruct->DS |= (1u << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set PIN Strength*/ 
          #endif
          /* Set PIN SlewRate if enabled */
          #if TSS_USE_GPIO_SLEW_RATE
            psPCTLStruct->SRE |= (1u << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set PIN SlewRate*/ 
          #endif
          /* Set Default PIN State */
          psPortStruct->DD |= (1u << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set PIN to OUTPUT */
          #if TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW
            psPortStruct->D &= ~(1u << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set LOG0 to OUTPUT pin */      
          #else
            psPortStruct->D |= (1u << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set LOG1 to OUTPUT pin */      
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
  UINT8 GPIO_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
  {
    UINT8 u8NSamp;
    UINT8 u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;   /* Sets default status */
    UINT16 u16CapSubSample;
    UINT8 u8FaultCnt;
    TSS_GPIO_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
    FTM_MemMapPtr psTimerStruct;
    PT_MemMapPtr psPortStruct;
    #if TSS_USE_NOISE_AMPLITUDE_FILTER
      INT8 i8AmplitudeFilterDeltaLimitTemp;
      TSS_GPIO_ELECTRODE_RAMDATA *psElectrodeRAMDataStruct;
    #endif

    if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
    {
      u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;           
    } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
    {  
      /* Pointers Decoding */      
      psElectrodeROMDataStruct = (TSS_GPIO_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
      psTimerStruct = (FTM_MemMapPtr) (((TSS_GPIO_METHOD_ROMDATA *) (psElectrodeROMDataStruct->gpio_cpsu8MethodROMData))->gpio_cpsu8Timer);
      psPortStruct = (PT_MemMapPtr) (((TSS_GPIO_METHOD_ROMDATA *) (psElectrodeROMDataStruct->gpio_cpsu8MethodROMData))->gpio_cpsu8Port);
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        psElectrodeRAMDataStruct = (TSS_GPIO_ELECTRODE_RAMDATA *) (tss_acp8ElectrodeRAMData[u8ElecNum]);
      #endif
     
      tss_psElectrodeTimertAdr = (UINT32 *) psTimerStruct;
      tss_psElectrodePortAdr = (UINT32 *) psPortStruct;
      tss_u8ElectrodeBitNum = psElectrodeROMDataStruct->gpio_cu8PortBit;
      
      /* Initialization of variables */
      tss_u8HWTimerFlag = 0u;
      tss_u16CapSample = 0u;
      u8FaultCnt = 0u;
      u8NSamp = tss_CSSys.NSamples;

      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        if (psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase == 0u) {
          i8AmplitudeFilterDeltaLimitTemp = 127;
        } else {
          i8AmplitudeFilterDeltaLimitTemp = psElectrodeROMDataStruct->gpio_ci8AmplitudeFilterDeltaLimit;
        }
      #endif

      /* Main oversampling measurement loop */
      while(u8NSamp)
      {
          psTimerStruct->SC &= TSS_HW_TPMFTM_OFF; /* Stop Timer */
          psTimerStruct->CNT = 0u; /* Reset Counter i.e. Reset Timer*/

          TSS_CLEAR_SAMPLE_INTERRUPTED_FLAG();        /* Clears the interrupt sample flag */

          u16CapSubSample = (psElectrodeROMDataStruct->gpio_fSampleElectrodeLow)();    /* Samples one electrode */          
          
          if(!TSS_FAULT_DETECTED)                     /* Verifies that no fault has occur */
          {            
              if(!TSS_SAMPLE_INTERRUPTED)             /* Verifies that the sample has not been inturrupted*/
              {
                #if TSS_USE_NOISE_AMPLITUDE_FILTER
                  if (TSS_u16NoiseAmplitudeFilter(u16CapSubSample, &(psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase), i8AmplitudeFilterDeltaLimitTemp)) 
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
            psPortStruct->DD |= (1 << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set PIN to OUTPUT */
            psPortStruct->D &= ~(1 << psElectrodeROMDataStruct->gpio_cu8PortBit); /* Set LOG0 to OUTPUT pin */      

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
          if (psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase == 0u) {
            psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase = (UINT16) (tss_u16CapSample / tss_CSSys.NSamples);
          }
        } else {
          psElectrodeRAMDataStruct->gpio_u16AmplitudeFilterBase = 0u; /* Initiate Amplitude Filter Base refresh */
        }
      #endif
    } else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
    {
      /* Return the same electrode number because there is no more electrodes in the module */
      u8ElectrodeStatus = u8ElecNum;
    } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
    {
      /* Do Nothing because the Command is no relevant for GPIO Method */
      u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;
    } 
    
    return u8ElectrodeStatus;   /* Return status code */ 
  }
  #else /* End of TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    
  
#endif /* End of TSS_DETECT_METHOD(GPIO) */
