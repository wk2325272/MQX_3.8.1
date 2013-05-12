/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorTSIL.c
*
* @brief  Contains functions to Perform the sensing to the electrodes and set the status for each electrode
*
* @version 1.0.14.0
*
* @date Feb-23-2012
*
*
***********************************************************************************************************************/

#include "TSS_SensorTSIL.h"

#if TSS_DETECT_METHOD(TSIL)

  #if TSS_HCS08_MCU || TSS_CFV1_MCU

    #if TSS_USE_SIMPLE_LOW_LEVEL

      /***************** ROM & RAM Data definition *******************/
  
      #if TSS_N_ELECTRODES > 0
        #if TSS_DETECT_EL_METHOD(0,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(0) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 1
        #if TSS_DETECT_EL_METHOD(1,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(1) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 2
        #if TSS_DETECT_EL_METHOD(2,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(2) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 3
        #if TSS_DETECT_EL_METHOD(3,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(3) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 4
        #if TSS_DETECT_EL_METHOD(4,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(4) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 5
        #if TSS_DETECT_EL_METHOD(5,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(5) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 6
        #if TSS_DETECT_EL_METHOD(6,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(6) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 7
        #if TSS_DETECT_EL_METHOD(7,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(7) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 8
        #if TSS_DETECT_EL_METHOD(8,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(8) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 9
        #if TSS_DETECT_EL_METHOD(9,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(9) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 10
        #if TSS_DETECT_EL_METHOD(10,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(10) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 11
        #if TSS_DETECT_EL_METHOD(11,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(11) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 12
        #if TSS_DETECT_EL_METHOD(12,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(12) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 13
        #if TSS_DETECT_EL_METHOD(13,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(13) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 14
        #if TSS_DETECT_EL_METHOD(14,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(14) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 15
        #if TSS_DETECT_EL_METHOD(15,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(15) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 16
        #if TSS_DETECT_EL_METHOD(16,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(16) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 17
        #if TSS_DETECT_EL_METHOD(17,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(17) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 18
        #if TSS_DETECT_EL_METHOD(18,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(18) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 19
        #if TSS_DETECT_EL_METHOD(19,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(19) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 20
        #if TSS_DETECT_EL_METHOD(20,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(20) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 21
        #if TSS_DETECT_EL_METHOD(21,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(21) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 22
        #if TSS_DETECT_EL_METHOD(22,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(22) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 23
        #if TSS_DETECT_EL_METHOD(23,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(23) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 24
        #if TSS_DETECT_EL_METHOD(24,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(24) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 25
        #if TSS_DETECT_EL_METHOD(25,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(25) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 26
        #if TSS_DETECT_EL_METHOD(26,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(26) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 27
        #if TSS_DETECT_EL_METHOD(27,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(27) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 28
        #if TSS_DETECT_EL_METHOD(28,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(28) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 29
        #if TSS_DETECT_EL_METHOD(29,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(29) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 30
        #if TSS_DETECT_EL_METHOD(30,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(30) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 31
        #if TSS_DETECT_EL_METHOD(31,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(31) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 32
        #if TSS_DETECT_EL_METHOD(32,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(32) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 33
        #if TSS_DETECT_EL_METHOD(33,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(33) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 34
        #if TSS_DETECT_EL_METHOD(34,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(34) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 35
        #if TSS_DETECT_EL_METHOD(35,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(35) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 36
        #if TSS_DETECT_EL_METHOD(36,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(36) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 37
        #if TSS_DETECT_EL_METHOD(37,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(37) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 38
        #if TSS_DETECT_EL_METHOD(38,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(38) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 39
        #if TSS_DETECT_EL_METHOD(39,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(39) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 40
        #if TSS_DETECT_EL_METHOD(40,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(40) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 41
        #if TSS_DETECT_EL_METHOD(41,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(41) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 42
        #if TSS_DETECT_EL_METHOD(42,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(42) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 43
        #if TSS_DETECT_EL_METHOD(43,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(43) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 44
        #if TSS_DETECT_EL_METHOD(44,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(44) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 45
        #if TSS_DETECT_EL_METHOD(45,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(45) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 46
        #if TSS_DETECT_EL_METHOD(46,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(46) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 47
        #if TSS_DETECT_EL_METHOD(47,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(47) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 48
        #if TSS_DETECT_EL_METHOD(48,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(48) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 49
        #if TSS_DETECT_EL_METHOD(49,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(49) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 50
        #if TSS_DETECT_EL_METHOD(50,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(50) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 51
        #if TSS_DETECT_EL_METHOD(51,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(51) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 52
        #if TSS_DETECT_EL_METHOD(52,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(52) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 53
        #if TSS_DETECT_EL_METHOD(53,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(53) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 54
        #if TSS_DETECT_EL_METHOD(54,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(54) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 55
        #if TSS_DETECT_EL_METHOD(55,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(55) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 56
        #if TSS_DETECT_EL_METHOD(56,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(56) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 57
        #if TSS_DETECT_EL_METHOD(57,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(57) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 58
        #if TSS_DETECT_EL_METHOD(58,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(58) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 59
        #if TSS_DETECT_EL_METHOD(59,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(59) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 60
        #if TSS_DETECT_EL_METHOD(60,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(60) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 61
        #if TSS_DETECT_EL_METHOD(61,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(61) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 62
        #if TSS_DETECT_EL_METHOD(62,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(62) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      #if TSS_N_ELECTRODES > 63
        #if TSS_DETECT_EL_METHOD(63,TSIL)
            const TSS_TSIL_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { TSS_DETECT_EL_MODULE_ID(63) };
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
                
      /***************************************************************************//*!
      *                                 Types 
      ****************************************************************************/
  
      /* TSIL Recalibration Struct */    
                  
      typedef struct {
        UINT8 u8TSIPS;
        UINT8 u8TSIExtChrg;          
        UINT16 u16Distance;
      } TSIL_CALIBRATION;
                
      /************************** Prototypes ***************************/
      UINT8 TSIL_InitModule(void);
      UINT8 TSIL_SetNSamples(void);
      UINT8 TSIL_Recalibrate(void);
      UINT8 TSIL_InitTrigger(void);
      
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL General Initialization
      *
      * @param  psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_InitModule(void)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
  
        /* TSI_CS0 Settings */
        TSS_GET_TSIL_REGISTER(CS0) = 0; /* Erase CS0 - Turn OFF TSI*/ 
        
        /* TSI_CS2 Settings */
        TSS_GET_TSIL_REGISTER(CS2) = 0; /* Erase CS2 */
        /* Both OSC settings */
        #if TSS_TSIL_CS2_DELVOL != 0
          TSS_GET_TSIL_REGISTER(CS2) |= TSIL_CS2_DELVOL_FIT(TSS_TSIL_CS2_DELVOL);     /* Delta Voltage settings for ElOSC and RefOSC 0 - 300mV, 3 - 670mV */
        #endif
        /* TSI RefOSC definitions */
        TSS_GET_TSIL_REGISTER(CS2) |= TSIL_CS2_REFCHRG_FIT(TSS_TSIL_CS2_REFCHRG);   /* Reference Oscillator Charge Current 0 = 500 nA, 7 = 64uA */ 
     
        return u8result;
      }  
      
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL Number of Samples Settings
      *
      * @param  psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_SetNSamples(void)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
        UINT8 u8NSamples;
  
        TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module*/
        TSS_GET_TSIL_REGISTER(CS1) &= ~((UINT8)TSI_CS1_NSCN_MASK);      /* Erase NSCN */
        /* Number of Samples settings */
        u8NSamples = tss_CSSys.NSamples;
        TSS_GET_TSIL_REGISTER(CS1) |= TSIL_CS1_NSCN_FIT(u8NSamples-1u);  /* Number of Consecutive Scans per Electrode 0 = 1 scan, 31 = 32 scans*/
        
        return u8result;
      }  
      
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL Recalibration
      *
      * @param  *psMethodROMDataStruct - Specification of ROMDATA Struct
      *         psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      #if ((TSS_TSIL_EXTCHRG_RANGE == 1) && (TSS_TSIL_PS_RANGE == 1))  
        UINT8 TSIL_Recalibrate(void)
        {
          UINT8 u8result = TSS_INIT_STATUS_OK;

          TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module */
          TSS_GET_TSIL_REGISTER(CS1) &= ~((UINT8)TSI_CS1_PS_MASK);        /* Erase PS */
          TSS_GET_TSIL_REGISTER(CS1) |= TSIL_CS1_PS_FIT(TSS_TSIL_PS_LOW_LIMIT); /* Set Prescaler for Electrode OSC - Set by the user 0 = divide 1, 7 = divide 128 */
          TSS_GET_TSIL_REGISTER(CS2) &= ~((UINT8)TSI_CS2_EXTCHRG_MASK);   /* Erase ExtChrg */
          TSS_GET_TSIL_REGISTER(CS2) |= TSIL_CS2_EXTCHRG_FIT(TSS_TSIL_EXTCHRG_LOW_LIMIT); /* External OSC Charge Current 0= 500nA, 7 = 64uA */
    
          return u8result;
        }  
      #else
        UINT8 TSIL_Recalibrate(void)
        {
          TSIL_CALIBRATION FinalTSICalibration;
          TSIL_CALIBRATION TempTSICalibration;
          TSIL_CALIBRATION OldTSICalibration;
          UINT8 u8result = TSS_INIT_STATUS_OK;
          UINT8 u8ElCounter;
          UINT8 u8TSIPSTemp;
          UINT8 u8TSIExtChrgTemp;
          UINT8 u8CalibrationComplete;
          UINT16 u16CapSampleTemp;
          UINT8 u8Iteration;
          
          /* Save previous TSI Calibration */
          OldTSICalibration.u8TSIPS = (UINT8) ((TSS_GET_TSIL_REGISTER(CS1) & TSI_CS1_PS_MASK) >> TSI_CS1_PS_BITNUM);
          OldTSICalibration.u8TSIExtChrg = (UINT8) ((TSS_GET_TSIL_REGISTER(CS2) & TSI_CS2_EXTCHRG_MASK) >> TSI_CS2_EXTCHRG_BITNUM);
          /* Do Autocalibration */ 
          u8TSIPSTemp = TSS_TSIL_PS_LOW_LIMIT;
          FinalTSICalibration.u16Distance = 0xFFFFu;
          /* Set TSI registers for Single measurement */
          TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module */ 
          TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIIEN_MASK);    /* Disable Interrupts */
          TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_STM_MASK);    /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
          /* Check if all electrodes has proper Counter value */
          u8CalibrationComplete = 1u;
          
          do {
            u8TSIExtChrgTemp = TSS_TSIL_EXTCHRG_LOW_LIMIT;        
            u8Iteration = 1u;        
            TempTSICalibration.u16Distance = 0xFFFFu;
          #if (TSS_TSIL_EXTCHRG_RANGE > 1)  
            do {
              if (u8CalibrationComplete == 1u) {
                u8TSIExtChrgTemp += ((TSS_TSIL_EXTCHRG_RANGE / 2u) / u8Iteration);
              } else {
                u8TSIExtChrgTemp -= ((TSS_TSIL_EXTCHRG_RANGE / 2u) / u8Iteration);
              }
          #endif                
              u8Iteration <<= 1u;
              /* Check if all electrodes has proper Counter value */
              u8CalibrationComplete = 1u;
              /* Go through all enabled pins */
              for(u8ElCounter = 0u; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) 
              {
                if (TSS_ELEC_TYPE_CHK(TSS_TSIL_MAIN_MODULE_NAME) == ((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cu8ModuleID)
                {
                  if (tss_au8ElectrodeEnablers[u8ElCounter / 8] & (1<< (u8ElCounter % 8)))
                  {
                    /* Configure TSIL */
                    TSS_GET_TSIL_REGISTER(CS1) &= ~((UINT8)TSI_CS1_PS_MASK);      /* Erase PS */
                    TSS_GET_TSIL_REGISTER(CS1) |= TSIL_CS1_PS_FIT(u8TSIPSTemp);    /* Set Prescaler for Electrode OSC - Set by the user 0 = divide 1, 7 = divide 128 */
                    TSS_GET_TSIL_REGISTER(CS2) &= ~((UINT8)TSI_CS2_EXTCHRG_MASK); /* Erase ExtChrg */
                    TSS_GET_TSIL_REGISTER(CS2) |= TSIL_CS2_EXTCHRG_FIT(u8TSIExtChrgTemp); /* External OSC Charge Current 0= 500nA, 7 = 64uA */
                   
                    /* Read TSI Counter */
                    u16CapSampleTemp = tss_faSampleElectrodeLow[u8ElCounter]();             /* Samples one electrode */
                    if (u16CapSampleTemp < TSS_TSIL_RESOLUTION_VALUE) 
                    {
                      u8CalibrationComplete = 0u; /* Capacitance value is small */
                    }
                  }
                } 
              }
              /* Save the best option for this PS */
              if (u8CalibrationComplete == 1u)
              {
                TempTSICalibration.u8TSIExtChrg = u8TSIExtChrgTemp;
                TempTSICalibration.u8TSIPS = u8TSIPSTemp;
                TempTSICalibration.u16Distance = (u16CapSampleTemp - TSS_TSIL_RESOLUTION_VALUE);
              }
          #if (TSS_TSIL_EXTCHRG_RANGE > 1)              
            } while (((TSS_TSIL_EXTCHRG_RANGE / 2u) / u8Iteration) >= 1u);
          #endif                       
            /* Analyse distance of the found configuration */       
            if (TempTSICalibration.u16Distance != 0xFFFFu)
            {  
              if (FinalTSICalibration.u16Distance >= TempTSICalibration.u16Distance)
              {
                FinalTSICalibration.u8TSIPS = TempTSICalibration.u8TSIPS;
                FinalTSICalibration.u8TSIExtChrg = TempTSICalibration.u8TSIExtChrg;         
                FinalTSICalibration.u16Distance = TempTSICalibration.u16Distance;
              } else {
                u8TSIPSTemp = 8u; /* Escape if no better option found in the last loop */
              }
            }
          
            u8TSIPSTemp++; /* Increment PS current */
          } while (u8TSIPSTemp < (TSS_TSIL_PS_HIGH_LIMIT + 1u)); 
          
          /* Check if searching was succesful */
          if (FinalTSICalibration.u16Distance != 0xFFFFu)
          {  
            /* Set Final Found values */                
            TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module */
            TSS_GET_TSIL_REGISTER(CS1) &= ~((UINT8)TSI_CS1_PS_MASK);        /* Erase PS */
            TSS_GET_TSIL_REGISTER(CS1) |= TSIL_CS1_PS_FIT(FinalTSICalibration.u8TSIPS); /* Set Prescaler for Electrode OSC - Set by the user 0 = divide 1, 7 = divide 128 */
            TSS_GET_TSIL_REGISTER(CS2) &= ~((UINT8)TSI_CS2_EXTCHRG_MASK);   /* Erase ExtChrg */
            TSS_GET_TSIL_REGISTER(CS2) |= TSIL_CS2_EXTCHRG_FIT(FinalTSICalibration.u8TSIExtChrg); /* External OSC Charge Current 0= 500nA, 7 = 64uA */
            /* Indication of changed configuration */
            if ((OldTSICalibration.u8TSIPS != FinalTSICalibration.u8TSIPS) || (OldTSICalibration.u8TSIExtChrg != FinalTSICalibration.u8TSIExtChrg))
            {
              u8result |= TSS_INIT_STATUS_CALIBRATION_CHANGED;    
            }
          } else {
            /* If searching was unsucessful, report fault */
            u8result |= TSS_INIT_ERROR_RECALIB_FAULT;    
          }  
    
          return u8result;
        }
      #endif    
      
      /***************************************************************************//*!
      *
      * @brief  Control function for TSI Trigger Settings
      *
      * @param  *psMethodROMDataStruct - Specificcation of ROMDATA Struct
      *         psModuleStruct - Specification of TSI module
      *         *psMethodRAMDataStruct - Specificcation of RAMDATA Struct
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_InitTrigger(void)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
        
        TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIEN_MASK); /* Turn OFF TSI Module */
        /* Setup TSI */       
        TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_STM_MASK);     /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
        TSS_GET_TSIL_REGISTER(CS0) &= ~((UINT8)TSI_CS0_TSIIEN_MASK);  /* TSI Interrupt Disable (0 = disabled, 1 = enabled) */
        
        u8result |= TSS_INIT_STATUS_TRIGGER_SET;
  
        return u8result;
      }
      
      /***************************************************************************//*!
      *
      * @brief  Initializes the modules for the sensing of the electrodes
      *
      * @param  UINT8 u8ElNum - Processed Electrode Number  
      *         UINT8 u8Command - Command intended to process
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_MethodControl(UINT8 u8Command)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;

        /************* Do TSIL Init *******************/
        if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
        {    
          u8result |= TSIL_InitModule();
          u8result |= TSIL_SetNSamples();
          u8result |= TSIL_Recalibrate();  
          u8result |= TSIL_InitTrigger(); 
        } 
        /************* Set NSamples ***************/
        else if (u8Command == TSS_INIT_COMMAND_SET_NSAMPLES)
        {
          u8result |= TSIL_SetNSamples();
          u8result |= TSIL_Recalibrate();
          u8result |= TSIL_InitTrigger(); 
        }
        /*************** Do Auto calibration setting of TSIL EXTCHRG and TSIL PS **************/      
        else if (u8Command == TSS_INIT_COMMAND_RECALIBRATE)
        {
          u8result |= TSIL_Recalibrate();
          u8result |= TSIL_InitTrigger();        
        }      
        /************* Triggering Init ************/
        else if (u8Command == TSS_INIT_COMMAND_INIT_TRIGGER)
        {  
          u8result |= TSIL_InitTrigger();
        } 
        
        /* Exit */                   
                       
        return u8result;
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
      UINT8 TSIL_SensorInit(UINT8 u8Command)
      {
          UINT8 u8result = TSS_INIT_STATUS_OK;
  
          u8result = TSIL_MethodControl(u8Command);
          
          return u8result;
      }              
      
      /***************************************************************************//*!
      *
      * @brief  Performs a valid reading of one electrode stores the timer values 
      *         and returns a status code
      *
      * @param  u8ElecNum Electrode number to be scanned
      *         u8Command Requested command
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
      {
        UINT8 u8ElCounter;
        UINT8 u8ModuleID;
        UINT8 u8ElectrodeStatus;  

        u8ModuleID = ((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]))->tsil_cu8ModuleID;
        /* Module Control */
        if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
        {
          u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;
        } else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
        {
#ifdef _TSS_TEST
  _TSS_TSIGEN_ON;                              /* Start generator for test purpose */
#endif

        	tss_u16CapSample = tss_faSampleElectrodeLow[u8ElecNum]();             /* Samples one electrode */
#ifdef _TSS_TEST
  _TSS_TSIGEN_OFF;                              /* Stop generator for test purpose */
#endif
          
          if (tss_u16CapSample > TSS_TSIL_CAP_HIGH_LIMIT)
          {      
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;    /* Too High Capacity is detected */
          } else if (tss_u16CapSample > TSS_TSIL_RECALIBRATION_HIGH_THRESHOLD)
          {      
            u8ElectrodeStatus = TSS_SAMPLE_RECALIB_REQUEST_HICAP;   /* Re-calibration is needed */
          }else if (tss_u16CapSample < TSS_TSIL_CAP_LOW_LIMIT) 
          {  
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_SMALL_CAP;         /* Too Low Capacity is detected */
          } 
          else if (tss_u16CapSample < TSS_TSIL_RECALIBRATION_LOW_THRESHOLD) 
          {  
            u8ElectrodeStatus = TSS_SAMPLE_RECALIB_REQUEST_LOCAP;   /* Re-calibration is needed */
          } else {
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;
          }
  
        }
        else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
        {
          /* Find the next electrode within the electrode module */
          u8ElCounter = u8ElecNum;
          #if TSS_N_ELECTRODES > 1          
            do {
              if (u8ElCounter < (TSS_N_ELECTRODES-1)) 
              {
                u8ElCounter++;
              } else {
                u8ElCounter = 0u;
              }  
            } while (u8ModuleID != ((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cu8ModuleID);
          #endif
          /* Return found electrode number */
          u8ElectrodeStatus = u8ElCounter;        
    
        } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
        {
          u8ElectrodeStatus = TSIL_MethodControl(TSS_INIT_COMMAND_RECALIBRATE);
          /* Evaluate result */
          if (u8ElectrodeStatus & TSS_INIT_ERROR_RECALIB_FAULT) 
          {
            /* Fault happened, so disable actual electrode */
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;
          } else if (u8ElectrodeStatus & TSS_INIT_STATUS_CALIBRATION_CHANGED) 
          {
            /* Calibration changed */
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_CALIBRATION_CHANGED;
          } else 
          {
            /* Old calibration was correct */
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  
          }
        }          
    
        return u8ElectrodeStatus;   /* Return status code */ 
        
      }
    
    #else /* TSS_USE_SIMPLE_LOW_LEVEL == 0 */
              
      /************************ Prototypes **************************/              
  
      UINT8 TSIL_MethodControl(UINT8 u8ElNum, UINT8 u8Command);
    
      /*******************  Modules definition **********************/
      #if TSS_DETECT_MODULE(TSIL)
        UINT16 TSS_TSIL_CHANNEL_COUNTER_BUFFER[16]; 
        const TSS_TSIL_METHOD_ROMDATA TSS_TSIL_METHOD_ROMDATA_CONTEXT = {TSIL_MethodControl, (UINT8*) &TSI_CS0, TSS_TSIL_CHANNEL_COUNTER_BUFFER};
        volatile TSS_TSIL_METHOD_RAMDATA TSS_TSIL_METHOD_RAMDATA_CONTEXT = {TSIL_EOSF_STATE_NONE, 0u, TSIL_SCNIP_STATE_NONE};
      #endif
      #if TSS_DETECT_MODULE(TSIL0)
        UINT16 TSS_TSIL0_CHANNEL_COUNTER_BUFFER[16];        
        const TSS_TSIL_METHOD_ROMDATA TSS_TSIL0_METHOD_ROMDATA_CONTEXT = {TSIL_MethodControl, (UINT8*) &TSI0_CS0, TSS_TSIL0_CHANNEL_COUNTER_BUFFER};    
        volatile TSS_TSIL_METHOD_RAMDATA TSS_TSIL0_METHOD_RAMDATA_CONTEXT = {TSIL_EOSF_STATE_NONE, 0u, TSIL_SCNIP_STATE_NONE};
      #endif    
      #if TSS_DETECT_MODULE(TSIL1)
        UINT16 TSS_TSIL1_CHANNEL_COUNTER_BUFFER[16];        
        const TSS_TSIL_METHOD_ROMDATA TSS_TSIL1_METHOD_ROMDATA_CONTEXT = {TSIL_MethodControl, (UINT8*) &TSI1_CS0, TSS_TSIL1_CHANNEL_COUNTER_BUFFER};    
        volatile TSS_TSIL_METHOD_RAMDATA TSS_TSIL1_METHOD_RAMDATA_CONTEXT = {TSIL_EOSF_STATE_NONE, 0u, TSIL_SCNIP_STATE_NONE};
      #endif
  
      /***************** Method Memory Data Decoding Macros **********/
          
      #define TSS_GET_TSIL_METHOD_DATA_CONTEXT_RESULT(memory,res)       TSS_##res##_METHOD_##memory##DATA_CONTEXT
      #define TSS_GET_TSIL_METHOD_DATA_CONTEXT_CONV(memory,text)        TSS_GET_TSIL_METHOD_DATA_CONTEXT_RESULT(memory,text)
      #define TSS_GET_TSIL_METHOD_DATA_CONTEXT_TYPE(memory,module)      TSS_GET_TSIL_METHOD_DATA_CONTEXT_CONV(memory,module)
    
      #define TSS_GET_TSIL_METHOD_DATA_CONTEXT(memory,el)               TSS_GET_TSIL_METHOD_DATA_CONTEXT_TYPE(memory, TSS_TSIL_EL_METHOD_MODULE_NAME(el))
        
      /***************** ROM & RAM Data definition *******************/
    
      #if TSS_N_ELECTRODES > 0
        #if TSS_DETECT_EL_METHOD(0,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,0), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,0), TSS_TSIL_EL_CHANNEL_NUM(0)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 1
        #if TSS_DETECT_EL_METHOD(1,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,1), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,1), TSS_TSIL_EL_CHANNEL_NUM(1)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 2
        #if TSS_DETECT_EL_METHOD(2,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,2), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,2), TSS_TSIL_EL_CHANNEL_NUM(2)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 3
        #if TSS_DETECT_EL_METHOD(3,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,3), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,3), TSS_TSIL_EL_CHANNEL_NUM(3)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 4
        #if TSS_DETECT_EL_METHOD(4,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,4), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,4), TSS_TSIL_EL_CHANNEL_NUM(4)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 5
        #if TSS_DETECT_EL_METHOD(5,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,5), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,5), TSS_TSIL_EL_CHANNEL_NUM(5)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 6
        #if TSS_DETECT_EL_METHOD(6,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,6), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,6), TSS_TSIL_EL_CHANNEL_NUM(6)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 7
        #if TSS_DETECT_EL_METHOD(7,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,7), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,7), TSS_TSIL_EL_CHANNEL_NUM(7)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 8
        #if TSS_DETECT_EL_METHOD(8,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,8), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,8), TSS_TSIL_EL_CHANNEL_NUM(8)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 9
        #if TSS_DETECT_EL_METHOD(9,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,9), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,9), TSS_TSIL_EL_CHANNEL_NUM(9)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 10
        #if TSS_DETECT_EL_METHOD(10,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,10), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,10), TSS_TSIL_EL_CHANNEL_NUM(10)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 11
        #if TSS_DETECT_EL_METHOD(11,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,11), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,11), TSS_TSIL_EL_CHANNEL_NUM(11)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 12
        #if TSS_DETECT_EL_METHOD(12,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,12), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,12), TSS_TSIL_EL_CHANNEL_NUM(12)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 13
        #if TSS_DETECT_EL_METHOD(13,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,13), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,13), TSS_TSIL_EL_CHANNEL_NUM(13)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 14
        #if TSS_DETECT_EL_METHOD(14,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,14), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,14), TSS_TSIL_EL_CHANNEL_NUM(14)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 15
        #if TSS_DETECT_EL_METHOD(15,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,15), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,15), TSS_TSIL_EL_CHANNEL_NUM(15)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 16
        #if TSS_DETECT_EL_METHOD(16,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,16), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,16), TSS_TSIL_EL_CHANNEL_NUM(16)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 17
        #if TSS_DETECT_EL_METHOD(17,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,17), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,17), TSS_TSIL_EL_CHANNEL_NUM(17)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 18
        #if TSS_DETECT_EL_METHOD(18,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,18), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,18), TSS_TSIL_EL_CHANNEL_NUM(18)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 19
        #if TSS_DETECT_EL_METHOD(19,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,19), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,19), TSS_TSIL_EL_CHANNEL_NUM(19)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 20
        #if TSS_DETECT_EL_METHOD(20,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,20), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,20), TSS_TSIL_EL_CHANNEL_NUM(20)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 21
        #if TSS_DETECT_EL_METHOD(21,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,21), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,21), TSS_TSIL_EL_CHANNEL_NUM(21)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 22
        #if TSS_DETECT_EL_METHOD(22,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,22), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,22), TSS_TSIL_EL_CHANNEL_NUM(22)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 23
        #if TSS_DETECT_EL_METHOD(23,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,23), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,23), TSS_TSIL_EL_CHANNEL_NUM(23)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 24
        #if TSS_DETECT_EL_METHOD(24,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,24), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,24), TSS_TSIL_EL_CHANNEL_NUM(24)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 25
        #if TSS_DETECT_EL_METHOD(25,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,25), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,25), TSS_TSIL_EL_CHANNEL_NUM(25)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 26
        #if TSS_DETECT_EL_METHOD(26,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,26), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,26), TSS_TSIL_EL_CHANNEL_NUM(26)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 27
        #if TSS_DETECT_EL_METHOD(27,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,27), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,27), TSS_TSIL_EL_CHANNEL_NUM(27)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 28
        #if TSS_DETECT_EL_METHOD(28,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,28), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,28), TSS_TSIL_EL_CHANNEL_NUM(28)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 29
        #if TSS_DETECT_EL_METHOD(29,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,29), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,29), TSS_TSIL_EL_CHANNEL_NUM(29)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 30
        #if TSS_DETECT_EL_METHOD(30,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,30), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,30), TSS_TSIL_EL_CHANNEL_NUM(30)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 31
        #if TSS_DETECT_EL_METHOD(31,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,31), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,31), TSS_TSIL_EL_CHANNEL_NUM(31)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 32
        #if TSS_DETECT_EL_METHOD(32,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,32), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,32), TSS_TSIL_EL_CHANNEL_NUM(32)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 33
        #if TSS_DETECT_EL_METHOD(33,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,33), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,33), TSS_TSIL_EL_CHANNEL_NUM(33)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 34
        #if TSS_DETECT_EL_METHOD(34,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,34), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,34), TSS_TSIL_EL_CHANNEL_NUM(34)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 35
        #if TSS_DETECT_EL_METHOD(35,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,35), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,35), TSS_TSIL_EL_CHANNEL_NUM(35)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 36
        #if TSS_DETECT_EL_METHOD(36,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,36), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,36), TSS_TSIL_EL_CHANNEL_NUM(36)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 37
        #if TSS_DETECT_EL_METHOD(37,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,37), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,37), TSS_TSIL_EL_CHANNEL_NUM(37)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 38
        #if TSS_DETECT_EL_METHOD(38,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,38), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,38), TSS_TSIL_EL_CHANNEL_NUM(38)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 39
        #if TSS_DETECT_EL_METHOD(39,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,39), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,39), TSS_TSIL_EL_CHANNEL_NUM(39)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 40
        #if TSS_DETECT_EL_METHOD(40,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,40), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,40), TSS_TSIL_EL_CHANNEL_NUM(40)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 41
        #if TSS_DETECT_EL_METHOD(41,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,41), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,41), TSS_TSIL_EL_CHANNEL_NUM(41)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 42
        #if TSS_DETECT_EL_METHOD(42,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,42), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,42), TSS_TSIL_EL_CHANNEL_NUM(42)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 43
        #if TSS_DETECT_EL_METHOD(43,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,43), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,43), TSS_TSIL_EL_CHANNEL_NUM(43)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 44
        #if TSS_DETECT_EL_METHOD(44,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,44), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,44), TSS_TSIL_EL_CHANNEL_NUM(44)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 45
        #if TSS_DETECT_EL_METHOD(45,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,45), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,45), TSS_TSIL_EL_CHANNEL_NUM(45)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 46
        #if TSS_DETECT_EL_METHOD(46,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,46), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,46), TSS_TSIL_EL_CHANNEL_NUM(46)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 47
        #if TSS_DETECT_EL_METHOD(47,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,47), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,47), TSS_TSIL_EL_CHANNEL_NUM(47)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 48
        #if TSS_DETECT_EL_METHOD(48,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,48), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,48), TSS_TSIL_EL_CHANNEL_NUM(48)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 49
        #if TSS_DETECT_EL_METHOD(49,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,49), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,49), TSS_TSIL_EL_CHANNEL_NUM(49)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 50
        #if TSS_DETECT_EL_METHOD(50,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,50), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,50), TSS_TSIL_EL_CHANNEL_NUM(50)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 51
        #if TSS_DETECT_EL_METHOD(51,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,51), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,51), TSS_TSIL_EL_CHANNEL_NUM(51)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 52
        #if TSS_DETECT_EL_METHOD(52,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,52), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,52), TSS_TSIL_EL_CHANNEL_NUM(52)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 53
        #if TSS_DETECT_EL_METHOD(53,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,53), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,53), TSS_TSIL_EL_CHANNEL_NUM(53)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 54
        #if TSS_DETECT_EL_METHOD(54,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,54), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,54), TSS_TSIL_EL_CHANNEL_NUM(54)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 55
        #if TSS_DETECT_EL_METHOD(55,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,55), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,55), TSS_TSIL_EL_CHANNEL_NUM(55)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 56
        #if TSS_DETECT_EL_METHOD(56,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,56), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,56), TSS_TSIL_EL_CHANNEL_NUM(56)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 57
        #if TSS_DETECT_EL_METHOD(57,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,57), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,57), TSS_TSIL_EL_CHANNEL_NUM(57)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 58
        #if TSS_DETECT_EL_METHOD(58,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,58), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,58), TSS_TSIL_EL_CHANNEL_NUM(58)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 59
        #if TSS_DETECT_EL_METHOD(59,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,59), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,59), TSS_TSIL_EL_CHANNEL_NUM(59)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 60
        #if TSS_DETECT_EL_METHOD(60,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,60), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,60), TSS_TSIL_EL_CHANNEL_NUM(60)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 61
        #if TSS_DETECT_EL_METHOD(61,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,61), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,61), TSS_TSIL_EL_CHANNEL_NUM(61)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 62
        #if TSS_DETECT_EL_METHOD(62,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,62), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,62), TSS_TSIL_EL_CHANNEL_NUM(62)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
    
      #if TSS_N_ELECTRODES > 63
        #if TSS_DETECT_EL_METHOD(63,TSIL)
          const TSS_TSIL_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { &TSS_GET_TSIL_METHOD_DATA_CONTEXT(ROM,63), &TSS_GET_TSIL_METHOD_DATA_CONTEXT(RAM,63), TSS_TSIL_EL_CHANNEL_NUM(63)};
                TSS_TSIL_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
        #endif
      #endif
  
      /***************************************************************************//*!
      *                                 Types 
      ****************************************************************************/
  
      /* TSIL Recalibration Struct */    
                  
      typedef struct {
        UINT8 u8TSIPS;
        UINT8 u8TSIExtChrg;          
        UINT16 u16Distance;
      } TSIL_CALIBRATION;
                
      /************************** Prototypes ***************************/
  
      UINT8 TSIL_InitModule(TSS_TSIL_MemMapPtr psModuleStruct);
      UINT8 TSIL_SetNSamples(TSS_TSIL_MemMapPtr psModuleStruct);
      UINT8 TSIL_InitPEN(TSS_TSIL_METHOD_ROMDATA *psMethodROMDataStruct, TSS_TSIL_MemMapPtr psModuleStruct);
      UINT8 TSIL_Recalibrate(TSS_TSIL_MemMapPtr psModuleStruct);
      UINT8 TSIL_InitTrigger(TSS_TSIL_MemMapPtr psModuleStruct, TSS_TSIL_METHOD_RAMDATA *psMethodRAMDataStruct);
      UINT8 TSIL_ReadCounter(TSS_TSIL_ELECTRODE_ROMDATA *psElectrodeROMDataStruct, TSS_TSIL_METHOD_ROMDATA *psMethodROMDataStruct);
  
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL General Initialization
      *
      * @param  psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_InitModule(TSS_TSIL_MemMapPtr psModuleStruct)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
  
        /* TSI_CS0 Settings */
        psModuleStruct->CS0 = 0; /* Erase CS0 - Turn OFF TSI*/ 
        
        /* TSI_CS2 Settings */
        psModuleStruct->CS2 = 0; /* Erase CS2 */
        /* Both OSC settings */
        #if TSS_TSIL_CS2_DELVOL != 0
          psModuleStruct->CS2 |= TSIL_CS2_DELVOL_FIT(TSS_TSIL_CS2_DELVOL);            /* Delta Voltage settings for ElOSC and RefOSC 0 - 300mV, 3 - 670mV */
        #endif
        /* TSI RefOSC definitions */
        psModuleStruct->CS2 |= TSIL_CS2_REFCHRG_FIT(TSS_TSIL_CS2_REFCHRG);   /* Reference Oscillator Charge Current 0 = 500 nA, 7 = 64uA */ 
     
        return u8result;
      }  
  
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL Number of Samples Settings
      *
      * @param  psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_SetNSamples(TSS_TSIL_MemMapPtr psModuleStruct)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
        UINT8 u8NSamples;
  
        psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module*/
        psModuleStruct->CS1 &= ~((UINT8)TSI_CS1_NSCN_MASK);      /* Erase NSCN */
        /* Number of Samples settings */
        u8NSamples = tss_CSSys.NSamples;
        psModuleStruct->CS1 |= TSIL_CS1_NSCN_FIT(u8NSamples-1u);  /* Number of Consecutive Scans per Electrode 0 = 1 scan, 31 = 32 scans*/
        
        return u8result;
      }  
  
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL Pin Enable Settings
      *
      * @param  *psMethodROMDataStruct - Specificcation of ROMDATA Struct
      *         psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_InitPEN(TSS_TSIL_METHOD_ROMDATA *psMethodROMDataStruct, TSS_TSIL_MemMapPtr psModuleStruct)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
        UINT8 u8ElCounter;
        UINT8 u8ChannelNum;
        
        /* PEN Clearing */
        for(u8ElCounter = 0; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
        {
          if (psMethodROMDataStruct == (TSS_TSIL_METHOD_ROMDATA *) (((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cpsMethodROMData)) 
          {
            /***** Disable Electrode *****/
            if (!(tss_au8ElectrodeEnablers[u8ElCounter / 8] & (1<< (u8ElCounter % 8))))
            {  
              u8ChannelNum = ((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cu8ChannelNum;
              if ((psModuleStruct->PEN.Byte[u8ChannelNum >> 3u] & (1 << (u8ChannelNum & 0x07))) != 0u)
              {  
                psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK); /* Turn OFF TSI Module if TSI was enabled */     
                /* If the Electrode is enabled then disable Electrode */ 
                psModuleStruct->PEN.Byte[u8ChannelNum >> 3u] &= ~((UINT8)(1 << (u8ChannelNum & 0x07)));
              }  
            }
          } 
        }
        /* PEN Enabling - The loop must be performed twice because one PEN can be assigned to more TSS electrodes */
        for(u8ElCounter = 0; u8ElCounter < TSS_N_ELECTRODES; u8ElCounter++) /* Find all with the same module */
        {
          if (psMethodROMDataStruct == (TSS_TSIL_METHOD_ROMDATA *) (((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cpsMethodROMData))
          {
            /***** Enable Electrode *****/
            if (tss_au8ElectrodeEnablers[u8ElCounter / 8] & (1<< (u8ElCounter % 8)))
            {
              u8ChannelNum = ((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cu8ChannelNum;
              if ((psModuleStruct->PEN.Byte[u8ChannelNum >> 3u] & (1 << (u8ChannelNum & 0x07))) == 0u)
              {  
                psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK); /* Turn OFF TSI Module if TSI was enabled */     
                /* If the Electrode is enabled then disable Electrode */ 
                psModuleStruct->PEN.Byte[u8ChannelNum >> 3u] |= ((UINT8)(1 << (u8ChannelNum & 0x07)));
              }  
            }
          } 
        }
        
        return u8result;
      } 
      
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL Recalibration
      *
      * @param  *psMethodROMDataStruct - Specification of ROMDATA Struct
      *         psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      #if ((TSS_TSIL_EXTCHRG_RANGE == 1) && (TSS_TSIL_PS_RANGE == 1))  
        UINT8 TSIL_Recalibrate(TSS_TSIL_MemMapPtr psModuleStruct)
        {
          UINT8 u8result = TSS_INIT_STATUS_OK;
          
          psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module */
          psModuleStruct->CS1 &= ~((UINT8)TSI_CS1_PS_MASK);        /* Erase PS */
          psModuleStruct->CS1 |= TSIL_CS1_PS_FIT(TSS_TSIL_PS_LOW_LIMIT); /* Set Prescaler for Electrode OSC - Set by the user 0 = divide 1, 7 = divide 128 */
          psModuleStruct->CS2 &= ~((UINT8)TSI_CS2_EXTCHRG_MASK);   /* Erase ExtChrg */
          psModuleStruct->CS2 |= TSIL_CS2_EXTCHRG_FIT(TSS_TSIL_EXTCHRG_LOW_LIMIT); /* External OSC Charge Current 0= 500nA, 7 = 64uA */
    
          return u8result;
        }  
      #else
        UINT8 TSIL_Recalibrate(TSS_TSIL_MemMapPtr psModuleStruct)
        {
          TSIL_CALIBRATION FinalTSICalibration;
          TSIL_CALIBRATION TempTSICalibration;
          TSIL_CALIBRATION OldTSICalibration;
          UINT8 u8result = TSS_INIT_STATUS_OK;
          UINT8 u8ElCounter;
          UINT8 u8TSIPSTemp;
          UINT8 u8TSIExtChrgTemp;
          UINT8 u8CalibrationComplete;
          UINT16 u16CapSampleTemp;
          UINT8 u8Iteration;
          UINT16 u16PENtemp;
          
          /* Save previous TSI Calibration */
          OldTSICalibration.u8TSIPS = (UINT8) ((psModuleStruct->CS1 & TSI_CS1_PS_MASK) >> TSI_CS1_PS_BITNUM);
          OldTSICalibration.u8TSIExtChrg = (UINT8) ((psModuleStruct->CS2 & TSI_CS2_EXTCHRG_MASK) >> TSI_CS2_EXTCHRG_BITNUM);
          /* Do Autocalibration */ 
          u8TSIPSTemp = TSS_TSIL_PS_LOW_LIMIT;
          FinalTSICalibration.u16Distance = 0xFFFFu;
          /* Set TSI registers for Single measurement */
          psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK);  /* Turn OFF TSI Module */ 
          psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIIEN_MASK); /* Disable Interrupts */
          psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_STM_MASK);    /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
          psModuleStruct->CS0 |= TSI_CS0_TSIEN_MASK;            /* Turn On TSI Module */
          /* Check if all electrodes has proper Counter value */
          u8CalibrationComplete = 1u;
          /* Read enabled pins */
          u16PENtemp = ((((UINT16) psModuleStruct->PEN.Byte[1]) << 8u) | ((UINT16)psModuleStruct->PEN.Byte[0]));
          
          do {
            u8TSIExtChrgTemp = TSS_TSIL_EXTCHRG_LOW_LIMIT;        
            u8Iteration = 1u;        
            TempTSICalibration.u16Distance = 0xFFFFu;

          #if (TSS_TSIL_EXTCHRG_RANGE > 1)              
            do {
              if (u8CalibrationComplete == 1u) {
                u8TSIExtChrgTemp += ((TSS_TSIL_EXTCHRG_RANGE / 2u) / u8Iteration);
              } else {
                u8TSIExtChrgTemp -= ((TSS_TSIL_EXTCHRG_RANGE / 2u) / u8Iteration);
              }
          #endif  
              u8Iteration <<= 1u;
              /* Check if all electrodes has proper Counter value */
              u8CalibrationComplete = 1u;
              /* Go through all enabled pins */
              for(u8ElCounter = 0u; u8ElCounter < 16u; u8ElCounter++) 
              {
                if (u16PENtemp & (1u << u8ElCounter))
                {
                  /* Configure TSIL */
                  psModuleStruct->CS1 &= ~((UINT8)TSI_CS1_PS_MASK);      /* Erase PS */
                  psModuleStruct->CS1 |= TSIL_CS1_PS_FIT(u8TSIPSTemp);    /* Set Prescaler for Electrode OSC - Set by the user 0 = divide 1, 7 = divide 128 */
                  psModuleStruct->CS2 &= ~((UINT8)TSI_CS2_EXTCHRG_MASK); /* Erase ExtChrg */
                  psModuleStruct->CS2 |= TSIL_CS2_EXTCHRG_FIT(u8TSIExtChrgTemp); /* External OSC Charge Current 0= 500nA, 7 = 64uA */
                  /* Start Measurement */
                  psModuleStruct->CS3 &= ~((UINT8)TSI_CS3_TSICH_MASK);   /* Erase TSICH */
                  psModuleStruct->CS3 = TSIL_CS3_TSICH_FIT(u8ElCounter);  /* Set Channel */
                  psModuleStruct->CS0 |= TSI_CS0_SWTS_MASK;              /* Toggle SW trigger */
                  
                  #ifdef _TSS_TEST
                    _TSS_TSIGEN_ON;                                      /* Start generator for test purpose */
                  #endif
                  do {
                  /* Measurement Running */  
                  } while ((psModuleStruct->CS0 & TSI_CS0_EOSF_MASK) == 0u); /* If TSI End of Scan Flag is reported*/
                  #ifdef _TSS_TEST
                    _TSS_TSIGEN_OFF;                                     /* Stop generator */
                  #endif
                  /* Reset EOSF Flag */
                  psModuleStruct->CS0 |= TSI_CS0_EOSF_MASK;              /* Toggle EOSF flag */  
                  /* Wait for data ready */
                  asm ("NOP"); asm ("NOP"); 
                  /* Read TSI Counter */
                  u16CapSampleTemp = psModuleStruct->CNT.Word;
                  if (u16CapSampleTemp < TSS_TSIL_RESOLUTION_VALUE) 
                  {
                    u8CalibrationComplete = 0u; /* Capacitance value is small */
                  }
                } 
              }
              /* Save the best option for this PS */
              if (u8CalibrationComplete == 1u)
              {
                TempTSICalibration.u8TSIExtChrg = u8TSIExtChrgTemp;
                TempTSICalibration.u8TSIPS = u8TSIPSTemp;
                TempTSICalibration.u16Distance = (u16CapSampleTemp - TSS_TSIL_RESOLUTION_VALUE);
              }
          #if (TSS_TSIL_EXTCHRG_RANGE > 1)
            } while (((TSS_TSIL_EXTCHRG_RANGE / 2u) / u8Iteration) >= 1u);
          #endif           
            /* Analyse distance of the found configuration */       
            if (TempTSICalibration.u16Distance != 0xFFFFu)
            {  
              if (FinalTSICalibration.u16Distance >= TempTSICalibration.u16Distance)
              {
                FinalTSICalibration.u8TSIPS = TempTSICalibration.u8TSIPS;
                FinalTSICalibration.u8TSIExtChrg = TempTSICalibration.u8TSIExtChrg;         
                FinalTSICalibration.u16Distance = TempTSICalibration.u16Distance;
              } else {
                u8TSIPSTemp = 8u; /* Escape if no better option found in the last loop */
              }
            }
          
            u8TSIPSTemp++; /* Increment PS current */
          } while (u8TSIPSTemp < (TSS_TSIL_PS_HIGH_LIMIT + 1u)); 
          
          /* Check if searching was succesful */
          if (FinalTSICalibration.u16Distance != 0xFFFFu)
          {  
            /* Set Final Found values */                
            psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK);     /* Turn OFF TSI Module */
            psModuleStruct->CS1 &= ~((UINT8)TSI_CS1_PS_MASK);        /* Erase PS */
            psModuleStruct->CS1 |= TSIL_CS1_PS_FIT(FinalTSICalibration.u8TSIPS); /* Set Prescaler for Electrode OSC - Set by the user 0 = divide 1, 7 = divide 128 */
            psModuleStruct->CS2 &= ~((UINT8)TSI_CS2_EXTCHRG_MASK);   /* Erase ExtChrg */
            psModuleStruct->CS2 |= TSIL_CS2_EXTCHRG_FIT(FinalTSICalibration.u8TSIExtChrg); /* External OSC Charge Current 0= 500nA, 7 = 64uA */
            /* Indication of changed configuration */
            if ((OldTSICalibration.u8TSIPS != FinalTSICalibration.u8TSIPS) || (OldTSICalibration.u8TSIExtChrg != FinalTSICalibration.u8TSIExtChrg))
            {
              u8result |= TSS_INIT_STATUS_CALIBRATION_CHANGED;    
            }
          } else {
            /* If searching was unsucessful, report fault */
            u8result |= TSS_INIT_ERROR_RECALIB_FAULT;    
          }  
    
          return u8result;
        }  
      #endif
       
      /***************************************************************************//*!
      *
      * @brief  Control function for TSIL Trigger Settings
      *
      * @param  *psMethodROMDataStruct - Specificcation of ROMDATA Struct
      *         psModuleStruct - Specification of TSIL module
      *         *psMethodRAMDataStruct - Specification of RAMDATA Struct
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_InitTrigger(TSS_TSIL_MemMapPtr psModuleStruct, TSS_TSIL_METHOD_RAMDATA *psMethodRAMDataStruct)
      {
        UINT8 u8result = TSS_INIT_STATUS_OK;
        
        psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK); /* Turn OFF TSI Module */
        /* Setup TSI */       
        if (tss_CSSys.SystemTrigger.TriggerMode == TSS_TRIGGER_MODE_AUTO)
        {
          psModuleStruct->CS0 |= TSI_CS0_STM_MASK;     /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
        } else /* Always & SW Mode */
        {
          psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_STM_MASK);     /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
        }
        psModuleStruct->CS0 |= TSI_CS0_TSIIEN_MASK;            /* TSI Interrupt Enable (0 = disabled, 1 = enabled) */
        psMethodRAMDataStruct->tsil_eEndOfScanState = TSIL_EOSF_STATE_NONE;
        psMethodRAMDataStruct->tsil_eScanInProgressState = TSIL_SCNIP_STATE_NONE;
        
        u8result |= TSS_INIT_STATUS_TRIGGER_SET;
  
        return u8result;
      }  
      
      /***************************************************************************//*!
      *
      * @brief  Initializes the modules for the sensing of the electrodes
      *
      * @param  UINT8 u8ElNum - Processed Electrode Number  
      *         UINT8 u8Command - Command intended to process
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_MethodControl(UINT8 u8ElNum, UINT8 u8Command)
      {
          UINT8 u8result = TSS_INIT_STATUS_OK;
          TSS_TSIL_METHOD_ROMDATA *psMethodROMDataStruct;
          TSS_TSIL_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
          TSS_TSIL_MemMapPtr psModuleStruct;
          TSS_TSIL_METHOD_RAMDATA *psMethodRAMDataStruct;
  
          /* Pointers Decoding */      
          psElectrodeROMDataStruct = (TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElNum]);
          psMethodROMDataStruct = (TSS_TSIL_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tsil_cpsMethodROMData);
          psModuleStruct = (TSS_TSIL_MemMapPtr) (psMethodROMDataStruct->tsil_cpsModule);
          psMethodRAMDataStruct = (TSS_TSIL_METHOD_RAMDATA *) (psElectrodeROMDataStruct->tsil_cpsMethodRAMData);
  
          /************* SWTrigger ************/
          if (u8Command == TSS_INIT_COMMAND_SW_TRIGGER)
          {  
            if (psMethodRAMDataStruct->tsil_eScanInProgressState == TSIL_SCNIP_STATE_WAIT)
            {
              psMethodRAMDataStruct->tsil_eEndOfScanState = TSIL_EOSF_STATE_NONE; /* Reset EndOfScanState */
              psMethodRAMDataStruct->tsil_eScanInProgressState = TSIL_SCNIP_STATE_PROCESS; /* Set processing */
              psModuleStruct->CS0 |= TSI_CS0_SWTS_MASK; /* Toggle SW Trigger */
              u8result |= TSS_INIT_STATUS_TRIGGER_SET;
              #ifdef _TSS_TEST
                _TSS_TSIGEN_ON;                         /* Start generator for test purpose */
              #endif
            }
          } 
          /************* Do TSIL Init *******************/
          else if (u8Command == TSS_INIT_COMMAND_INIT_MODULES)
          {    
            u8result |= TSIL_InitModule(psModuleStruct);
            u8result |= TSIL_SetNSamples(psModuleStruct);
            u8result |= TSIL_InitPEN(psMethodROMDataStruct, psModuleStruct);
            u8result |= TSIL_Recalibrate(psModuleStruct);  
            u8result |= TSIL_InitTrigger(psModuleStruct, psMethodRAMDataStruct); 
          } 
          /************* Set NSamples ***************/
          else if (u8Command == TSS_INIT_COMMAND_SET_NSAMPLES)
          {
            u8result |= TSIL_SetNSamples(psModuleStruct);
            u8result |= TSIL_Recalibrate(psModuleStruct);
            u8result |= TSIL_InitTrigger(psModuleStruct, psMethodRAMDataStruct); 
          }
          /*************** Do Auto calibration setting of TSIL EXTCHRG and TSIL PS **************/      
          else if (u8Command == TSS_INIT_COMMAND_RECALIBRATE)
          {
            u8result |= TSIL_Recalibrate(psModuleStruct);
            u8result |= TSIL_InitTrigger(psModuleStruct, psMethodRAMDataStruct);        
          }      
          /************ Electrode Enablers & Low Power Thresholds & Low Power Electrode Specification *************/
          else if (u8Command == TSS_INIT_COMMAND_ENABLE_ELECTRODES)
          {  
            u8result |= TSIL_InitPEN(psMethodROMDataStruct, psModuleStruct);
          }
          /************* Triggering Init ************/
          else if (u8Command == TSS_INIT_COMMAND_INIT_TRIGGER)
          {  
            u8result |= TSIL_InitTrigger(psModuleStruct, psMethodRAMDataStruct);
          } 
        
        /* Exit */                   
                       
        return u8result;
      }
      
      /***************************************************************************//*!
      *
      * @brief  Reads TSIL capacitance counter and returns a status code
      *
      * @param  *psElectrodeROMDataStruct - Specificcation of electrode ROMDATA Struct
      *         psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_ReadCounter(TSS_TSIL_ELECTRODE_ROMDATA *psElectrodeROMDataStruct, TSS_TSIL_METHOD_ROMDATA *psMethodROMDataStruct)
      {
        UINT8 u8result;  
  
        /* Read TSIL Counter */
        tss_u16CapSample = ((UINT16 *) (psMethodROMDataStruct->tsil_cpu16ChannelCounterBuffer))[psElectrodeROMDataStruct->tsil_cu8ChannelNum];
        /* Evaluation of the Measured Value */
        if (tss_u16CapSample > TSS_TSIL_CAP_HIGH_LIMIT)
        {      
          u8result = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;    /* Too High Capacity is detected */
        } else if (tss_u16CapSample > TSS_TSIL_RECALIBRATION_HIGH_THRESHOLD)
        {      
          u8result = TSS_SAMPLE_RECALIB_REQUEST_HICAP;   /* Re-calibration is needed */
        }else if (tss_u16CapSample < TSS_TSIL_CAP_LOW_LIMIT) 
        {  
          u8result = TSS_SAMPLE_ERROR_SMALL_CAP;         /* Too Low Capacity is detected */
        } 
        else if (tss_u16CapSample < TSS_TSIL_RECALIBRATION_LOW_THRESHOLD) 
        {  
          u8result = TSS_SAMPLE_RECALIB_REQUEST_LOCAP;   /* Re-calibration is needed */
        } else {
          u8result = TSS_SAMPLE_STATUS_OK;
        }
        
        return u8result;
      }
  
  
      /***************************************************************************//*!
      *
      * @brief  Performs and evalautes measurement 
      *
      * @param  *psMethodROMDataStruct - Specification of ROMDATA Struct
      *         *psMethodRAMDataStruct - Specification of RAMDATA Struct
      *         psModuleStruct - Specification of TSIL module
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_HandleMeasurement(TSS_TSIL_METHOD_RAMDATA *psMethodRAMDataStruct, TSS_TSIL_MemMapPtr psModuleStruct, UINT8 u8ElChannel)
      {
        UINT8 u8result;
  
        /* Establish measurement */
        if (tss_CSSys.SystemTrigger.TriggerMode == TSS_TRIGGER_MODE_ALWAYS)
        {
          if (psMethodRAMDataStruct->tsil_eEndOfScanState != TSIL_EOSF_STATE_REGULAR)
          {
            /* Init measurement */
            psMethodRAMDataStruct->tsil_u8StartElectrode = u8ElChannel;
            /* Start First Measurement */ 
            psModuleStruct->CS3 &= ~((UINT8)TSI_CS3_TSICH_MASK);   /* Erase TSICH */
            psModuleStruct->CS3 = TSIL_CS3_TSICH_FIT(u8ElChannel);  /* Set Channel */
            /* Set Status Flags */            
            psMethodRAMDataStruct->tsil_eEndOfScanState = TSIL_EOSF_STATE_NONE; /* Reset EndOfScanState */
            psMethodRAMDataStruct->tsil_eScanInProgressState = TSIL_SCNIP_STATE_PROCESS; /* Set processing */
            psModuleStruct->CS0 |= TSI_CS0_SWTS_MASK;    /* Toggle SW trigger */
          } 
          u8result = TSS_SAMPLE_STATUS_PROCESSING;
          
        } else {  /* If AUTO & SW Mode */

          /* Init measurement */
          psMethodRAMDataStruct->tsil_u8StartElectrode = u8ElChannel;
          psMethodRAMDataStruct->tsil_eScanInProgressState = TSIL_SCNIP_STATE_WAIT;
          /* Start First Measurement */ 
          psModuleStruct->CS3 &= ~((UINT8)TSI_CS3_TSICH_MASK);   /* Erase TSICH */
          psModuleStruct->CS3 = TSIL_CS3_TSICH_FIT(u8ElChannel);  /* Set Channel */
          
          if (tss_CSSys.SystemTrigger.TriggerMode == TSS_TRIGGER_MODE_AUTO)
          {
            /* Evaluate Auto Trigger period */
            if (psMethodRAMDataStruct->tsil_eEndOfScanState == TSIL_EOSF_STATE_ERROR)
            {
              u8result = TSS_SAMPLE_ERROR_SMALL_AUTOTRG_PERIOD; /* Small AutoTriggerModuloValue */
              psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_TSIEN_MASK); /* Disable whole TSS due triggering crash */
              #ifdef _TSS_TEST
                 _TSS_TSIGEN_OFF;                                  /* Stop generator for test purpose */
              #endif
            } else {
              u8result = TSS_SAMPLE_STATUS_PROCESSING;
            }
            psMethodRAMDataStruct->tsil_eEndOfScanState = TSIL_EOSF_STATE_NONE; /* Reset EndOfScanState */
          } else {
            u8result = TSS_SAMPLE_STATUS_PROCESSING;
          }
        }
        #ifdef _TSS_TEST
          _TSS_TSIGEN_ON;                              /* Start generator for test purpose */
        #endif
  
        return u8result;
      }
      
      /***************************************************************************//*!
      *
      * @brief  Performs a valid reading of one electrode stores the timer values 
      *         and returns a status code
      *
      * @param  u8ElecNum Electrode number to be scanned
      *         u8Command Requested command
      *
      * @return Status Code
      *
      * @remarks
      *
      ****************************************************************************/
      UINT8 TSIL_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command)
      {
        UINT8 u8ElectrodeStatus;  
        UINT8 u8ElCounter;
        UINT8 u8ElChannel;
        
        TSS_TSIL_METHOD_ROMDATA *psMethodROMDataStruct;
        TSS_TSIL_ELECTRODE_ROMDATA *psElectrodeROMDataStruct;
        TSS_TSIL_MemMapPtr psModuleStruct;
        TSS_TSIL_METHOD_RAMDATA *psMethodRAMDataStruct;
        
        /* Pointers decoding */
        psElectrodeROMDataStruct = (TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElecNum]);
        psMethodROMDataStruct = (TSS_TSIL_METHOD_ROMDATA *) (psElectrodeROMDataStruct->tsil_cpsMethodROMData);
        psModuleStruct = (TSS_TSIL_MemMapPtr) (psMethodROMDataStruct->tsil_cpsModule);
        psMethodRAMDataStruct = (TSS_TSIL_METHOD_RAMDATA *) (psElectrodeROMDataStruct->tsil_cpsMethodRAMData);
        u8ElChannel = psElectrodeROMDataStruct->tsil_cu8ChannelNum;
        
        /* Module Control */
        if (u8Command == TSS_SAMPLE_COMMAND_RESTART) 
        {
          /* If TSIL is disabled then enable */
          if ((psModuleStruct->CS0 & TSI_CS0_TSIEN_MASK) == 0u)
          {
            psModuleStruct->CS0 |= TSI_CS0_TSIEN_MASK;     /* Turn ON TSI Module */
          }
          /* Handle Measurement */
          if (psMethodRAMDataStruct->tsil_eScanInProgressState == TSIL_SCNIP_STATE_NONE)
          {
            u8ElectrodeStatus = TSIL_HandleMeasurement(psMethodRAMDataStruct, psModuleStruct, u8ElChannel);
          } else {
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;
          }
        }
        else if (u8Command == TSS_SAMPLE_COMMAND_PROCESS) 
        {
          /* If TSIL is disabled then enable */
          if ((psModuleStruct->CS0 & TSI_CS0_TSIEN_MASK) == 0u)
          {
            psModuleStruct->CS0 |= TSI_CS0_TSIEN_MASK;     /* Turn ON TSI Module */
          }
          /* Handle Auto Trigger Mode */
          if (psMethodRAMDataStruct->tsil_eScanInProgressState == TSIL_SCNIP_STATE_NONE) /* TSIL scanning is not in progress */
          {
            /* If TSI End of Scan Flag is reported */
            if (psMethodRAMDataStruct->tsil_eEndOfScanState >= TSIL_EOSF_STATE_REGULAR)
            {
              psMethodRAMDataStruct->tsil_eEndOfScanState = TSIL_EOSF_STATE_PROCESS;
              #ifdef _TSS_TEST
                _TSS_TSIGEN_OFF;                                    /* Stop generator */
              #endif
              /* Read TSI Counter */
              u8ElectrodeStatus = TSIL_ReadCounter(psElectrodeROMDataStruct, psMethodROMDataStruct);
            }
            else 
            {
              u8ElectrodeStatus = TSIL_HandleMeasurement(psMethodRAMDataStruct, psModuleStruct, u8ElChannel);
            }
          } 
          else 
          {
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_PROCESSING;
          }
        }
        else if (u8Command == TSS_SAMPLE_COMMAND_GET_NEXT_ELECTRODE) 
        {
          /* Find the next electrode within the electrode module */
          u8ElCounter = u8ElecNum;
          #if TSS_N_ELECTRODES > 1
            do {
              if (u8ElCounter < (TSS_N_ELECTRODES-1)) 
              {
                u8ElCounter++;
              } else {
                u8ElCounter = 0u;
              }  
            } while (psMethodROMDataStruct != (TSS_TSIL_METHOD_ROMDATA *) (((TSS_TSIL_ELECTRODE_ROMDATA *) (tss_acp8ElectrodeROMData[u8ElCounter]))->tsil_cpsMethodROMData));
          #endif          
          /* Return found electrode number */
          u8ElectrodeStatus = u8ElCounter;        
    
        } else if (u8Command == TSS_SAMPLE_COMMAND_RECALIB) 
        {
          u8ElectrodeStatus = TSIL_MethodControl(u8ElecNum, TSS_INIT_COMMAND_RECALIBRATE); 
    
          if (u8ElectrodeStatus & TSS_INIT_ERROR_RECALIB_FAULT) 
          {
            /* Fault happened, so disable actual electrode */
            u8ElectrodeStatus = TSS_SAMPLE_ERROR_CHARGE_TIMEOUT;
          } else if (u8ElectrodeStatus & TSS_INIT_STATUS_CALIBRATION_CHANGED) 
          {
            /* Calibration changed */
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_CALIBRATION_CHANGED;
          } else 
          {
            /* Old calibration was correct */
            u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;  
          }
          
        } else if (u8Command == TSS_SAMPLE_COMMAND_ENABLE_ELECTRODE) 
        {
          u8ElectrodeStatus = TSIL_MethodControl(u8ElecNum, TSS_INIT_COMMAND_ENABLE_ELECTRODES); 
          
          u8ElectrodeStatus = TSS_SAMPLE_STATUS_OK;
        } 
    
        return u8ElectrodeStatus;   /* Return status code */ 
        
      }
      
      /***************************************************************************//*!
      *
      * @brief  TSIL ISR routines
      *
      * @remarks
      *
      ****************************************************************************/
  
      #if TSS_DETECT_MODULE(TSIL)
        interrupt   
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtsi
        #endif
        void TSS_TSIIsr(void)
        {
          TSS_TSIL_MemMapPtr psModuleStruct;
          UINT8 u8Channel; 
          UINT16 u16PEN;
          UINT8 u8Timeout;
  
          /* Pointers decoding */
          psModuleStruct = (TSS_TSIL_MemMapPtr) (TSS_TSIL_METHOD_ROMDATA_CONTEXT.tsil_cpsModule);
  
          /* Clear EOSF flag */                 
          psModuleStruct->CS0 |= TSI_CS0_EOSF_MASK;
          
          if (TSS_TSIL_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState == TSIL_EOSF_STATE_NONE)
          {
            /* Disable HW trigger */
            psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_STM_MASK);   /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
            /* Save Actual channel */
            u8Channel = ((psModuleStruct->CS3 & TSI_CS3_TSICH_MASK) >> TSI_CS3_TSICH_BITNUM);        
            /* Read Counter */ 
            ((UINT16 *)(TSS_TSIL_METHOD_ROMDATA_CONTEXT.tsil_cpu16ChannelCounterBuffer))[u8Channel] = psModuleStruct->CNT.Word;
            /* Read PEN */
            u16PEN = ((((UINT16) psModuleStruct->PEN.Byte[1]) << 8u) | ((UINT16)psModuleStruct->PEN.Byte[0]));
            /* Find next electrode */
            u8Timeout = 18u;
            do {
              if (u8Channel < 15u)
              {
                u8Channel++;
              } else { 
                u8Channel = 0u;
              }
              u8Timeout--;
            } while (((u16PEN & (UINT16)(1u << u8Channel)) == 0u) && (u8Timeout > 0u));
            /* Evaluate */
            if (u8Channel == TSS_TSIL_METHOD_RAMDATA_CONTEXT.tsil_u8StartElectrode)
            {
              #ifdef _TSS_TEST
                _TSS_TSIGEN_OFF;                                   /* Stop generator */
              #endif
              TSS_TSIL_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState = TSIL_EOSF_STATE_REGULAR;
              TSS_TSIL_METHOD_RAMDATA_CONTEXT.tsil_eScanInProgressState = TSIL_SCNIP_STATE_NONE;
              if (tss_CSSys.SystemTrigger.TriggerMode == TSS_TRIGGER_MODE_AUTO)
              { 
                /* Enable HW trigger for period measurement in the case of AUTO trigger */
                psModuleStruct->CS0 |= TSI_CS0_STM_MASK;   /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
              }
            } else {
              psModuleStruct->CS3 &= ~((UINT8)TSI_CS3_TSICH_MASK); /* Erase TSICH */
              psModuleStruct->CS3 = TSIL_CS3_TSICH_FIT(u8Channel); /* Set Channel */
              psModuleStruct->CS0 |= TSI_CS0_SWTS_MASK;            /* Toggle SW trigger */
            }
          } else {
            /* Indication of short autotrigger period */
            TSS_TSIL_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState = TSIL_EOSF_STATE_ERROR;
          } 

          /* Set Sample Interrupted flag, because TSI measures at background and it can interrupt sampling of GPIO based methods */
          TSS_SET_SAMPLE_INTERRUPTED();
        } 
      #endif
        
      #if TSS_DETECT_MODULE(TSIL0)
        interrupt   
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtsi0
        #endif
        void TSS_TSI0Isr(void)
        {
          TSS_TSIL_MemMapPtr psModuleStruct;
          UINT8 u8Channel; 
          UINT16 u16PEN;
          UINT8 u8Timeout;
          
          /* Pointers decoding */
          psModuleStruct = (TSS_TSIL_MemMapPtr) (TSS_TSIL0_METHOD_ROMDATA_CONTEXT.tsil_cpsModule);
      
          /* Clear EOSF flag */                 
          psModuleStruct->CS0 |= TSI_CS0_EOSF_MASK;
          
          if (TSS_TSIL0_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState == TSIL_EOSF_STATE_NONE)
          {
            /* Disable HW trigger */
            psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_STM_MASK);   /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
            /* Save Actual channel */
            u8Channel = ((psModuleStruct->CS3 & TSI_CS3_TSICH_MASK) >> TSI_CS3_TSICH_BITNUM);        
            /* Read Counter */ 
            ((UINT16 *)(TSS_TSIL0_METHOD_ROMDATA_CONTEXT.tsil_cpu16ChannelCounterBuffer))[u8Channel] = psModuleStruct->CNT.Word;
            /* Read PEN */
            u16PEN = ((((UINT16) psModuleStruct->PEN.Byte[1]) << 8u) | ((UINT16)psModuleStruct->PEN.Byte[0]));
            /* Find next electrode */        
            u8Timeout = 18u;
            do {
              if (u8Channel < 15u)
              {
                u8Channel++;
              } else { 
                u8Channel = 0u;
              }
              u8Timeout--;
            } while (((u16PEN & (UINT16)(1u << u8Channel)) == 0u) && (u8Timeout > 0u));
            /* Evaluate */
            if (u8Channel == TSS_TSIL0_METHOD_RAMDATA_CONTEXT.tsil_u8StartElectrode)
            {
              #ifdef _TSS_TEST
                _TSS_TSIGEN_OFF;                                   /* Stop generator */
              #endif
              TSS_TSIL0_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState = TSIL_EOSF_STATE_REGULAR;
              TSS_TSIL0_METHOD_RAMDATA_CONTEXT.tsil_eScanInProgressState = TSIL_SCNIP_STATE_NONE;
              if (tss_CSSys.SystemTrigger.TriggerMode == TSS_TRIGGER_MODE_AUTO)
              { 
                /* Enable HW trigger for period measurement in the case of AUTO trigger */
                psModuleStruct->CS0 |= TSI_CS0_STM_MASK;   /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
              }
            } else {
              psModuleStruct->CS3 &= ~((UINT8)TSI_CS3_TSICH_MASK); /* Erase TSICH */
              psModuleStruct->CS3 = TSIL_CS3_TSICH_FIT(u8Channel); /* Set Channel */
              psModuleStruct->CS0 |= TSI_CS0_SWTS_MASK;            /* Toggle SW trigger */
            }
          } else {
            /* Indication of short autotrigger period */
            TSS_TSIL0_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState = TSIL_EOSF_STATE_ERROR;
          } 
          /* Set Sample Interrupted flag, because TSIL measures at background and it can interrupt sampling of GPIO based methods */
          TSS_SET_SAMPLE_INTERRUPTED();
        } 
      #endif
        
      #if TSS_DETECT_MODULE(TSIL1)
        interrupt   
        #if !TSS_USE_PE_COMPONENT
          VectorNumber_Vtsi1
        #endif
        void TSS_TSI1Isr(void)
        {
          TSI_MemMapPtr psModuleStruct;
          UINT8 u8Channel; 
          UINT16 u16PEN;
          UINT8 u8Timeout;
      
          /* Pointers decoding */
          psModuleStruct = (TSS_TSIL_MemMapPtr) (TSS_TSIL1_METHOD_ROMDATA_CONTEXT.tsil_cpsModule);
      
          /* Clear EOSF flag */                 
          psModuleStruct->CS0 |= TSI_CS0_EOSF_MASK;
          
          if (TSS_TSIL1_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState == TSIL_EOSF_STATE_NONE)
          {
            /* Disable HW trigger */
            psModuleStruct->CS0 &= ~((UINT8)TSI_CS0_STM_MASK);   /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
            /* Save Actual channel */
            u8Channel = ((psModuleStruct->CS3 & TSI_CS3_TSICH_MASK) >> TSI_CS3_TSICH_BITNUM);        
            /* Read Counter */ 
            ((UINT16 *)(TSS_TSIL1_METHOD_ROMDATA_CONTEXT.tsil_cpu16ChannelCounterBuffer))[u8Channel] = psModuleStruct->CNT.Word;
            /* Read PEN */
            u16PEN = ((((UINT16) psModuleStruct->PEN.Byte[1]) << 8u) | ((UINT16)psModuleStruct->PEN.Byte[0]));
            /* Find next electrode */        
            u8Timeout = 18u;
            do {
              if (u8Channel < 15u)
              {
                u8Channel++;
              } else { 
                u8Channel = 0u;
              }
              u8Timeout--;
            } while (((u16PEN & (UINT16)(1u << u8Channel)) == 0u) && (u8Timeout > 0u));
            /* Evaluate */
            if (u8Channel == TSS_TSIL1_METHOD_RAMDATA_CONTEXT.tsil_u8StartElectrode)
            {
              #ifdef _TSS_TEST
                _TSS_TSIGEN_OFF;                                   /* Stop generator */
              #endif
              TSS_TSIL1_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState = TSIL_EOSF_STATE_REGULAR;
              TSS_TSIL1_METHOD_RAMDATA_CONTEXT.tsil_eScanInProgressState = TSIL_SCNIP_STATE_NONE;
              if (tss_CSSys.SystemTrigger.TriggerMode == TSS_TRIGGER_MODE_AUTO)
              { 
                /* Enable HW trigger for period measurement in the case of AUTO trigger */
                psModuleStruct->CS0 |= TSI_CS0_STM_MASK;   /* Set Scan Trigger Mode (0 = SW Trigger, 1 = HW Trigger) */
              }
            } else {
              psModuleStruct->CS3 &= ~((UINT8)TSI_CS3_TSICH_MASK); /* Erase TSICH */
              psModuleStruct->CS3 = TSIL_CS3_TSICH_FIT(u8Channel); /* Set Channel */
              psModuleStruct->CS0 |= TSI_CS0_SWTS_MASK;            /* Toggle SW trigger */
            }
          } else {
            /* Indication of short autotrigger period */
            TSS_TSIL1_METHOD_RAMDATA_CONTEXT.tsil_eEndOfScanState = TSIL_EOSF_STATE_ERROR;
          } 
          /* Set Sample Interrupted flag, because TSI measures at background and it can interrupt sampling of GPIO based methods */
          TSS_SET_SAMPLE_INTERRUPTED();
        } 
      #endif
    #endif /* End of TSS_USE_SIMPLE_LOW_LEVEL == 0 */      

  #elif TSS_KINETIS_MCU || TSS_CFM_MCU

    /***************** ROM & RAM Data definition *******************/

    #if TSS_N_ELECTRODES > 0
      #if TSS_DETECT_EL_METHOD(0,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 1
      #if TSS_DETECT_EL_METHOD(1,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 2
      #if TSS_DETECT_EL_METHOD(2,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 3
      #if TSS_DETECT_EL_METHOD(3,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 4
      #if TSS_DETECT_EL_METHOD(4,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 5
      #if TSS_DETECT_EL_METHOD(5,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 6
      #if TSS_DETECT_EL_METHOD(6,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 7
      #if TSS_DETECT_EL_METHOD(7,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 8
      #if TSS_DETECT_EL_METHOD(8,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 9
      #if TSS_DETECT_EL_METHOD(9,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 10
      #if TSS_DETECT_EL_METHOD(10,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 11
      #if TSS_DETECT_EL_METHOD(11,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 12
      #if TSS_DETECT_EL_METHOD(12,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 13
      #if TSS_DETECT_EL_METHOD(13,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 14
      #if TSS_DETECT_EL_METHOD(14,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 15
      #if TSS_DETECT_EL_METHOD(15,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 16
      #if TSS_DETECT_EL_METHOD(16,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 17
      #if TSS_DETECT_EL_METHOD(17,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 18
      #if TSS_DETECT_EL_METHOD(18,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 19
      #if TSS_DETECT_EL_METHOD(19,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 20
      #if TSS_DETECT_EL_METHOD(20,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 21
      #if TSS_DETECT_EL_METHOD(21,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 22
      #if TSS_DETECT_EL_METHOD(22,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 23
      #if TSS_DETECT_EL_METHOD(23,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 24
      #if TSS_DETECT_EL_METHOD(24,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 25
      #if TSS_DETECT_EL_METHOD(25,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 26
      #if TSS_DETECT_EL_METHOD(26,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 27
      #if TSS_DETECT_EL_METHOD(27,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 28
      #if TSS_DETECT_EL_METHOD(28,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 29
      #if TSS_DETECT_EL_METHOD(29,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 30
      #if TSS_DETECT_EL_METHOD(30,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 31
      #if TSS_DETECT_EL_METHOD(31,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 32
      #if TSS_DETECT_EL_METHOD(32,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 33
      #if TSS_DETECT_EL_METHOD(33,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 34
      #if TSS_DETECT_EL_METHOD(34,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 35
      #if TSS_DETECT_EL_METHOD(35,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 36
      #if TSS_DETECT_EL_METHOD(36,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 37
      #if TSS_DETECT_EL_METHOD(37,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 38
      #if TSS_DETECT_EL_METHOD(38,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 39
      #if TSS_DETECT_EL_METHOD(39,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 40
      #if TSS_DETECT_EL_METHOD(40,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 41
      #if TSS_DETECT_EL_METHOD(41,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 42
      #if TSS_DETECT_EL_METHOD(42,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 43
      #if TSS_DETECT_EL_METHOD(43,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 44
      #if TSS_DETECT_EL_METHOD(44,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 45
      #if TSS_DETECT_EL_METHOD(45,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 46
      #if TSS_DETECT_EL_METHOD(46,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 47
      #if TSS_DETECT_EL_METHOD(47,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 48
      #if TSS_DETECT_EL_METHOD(48,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 49
      #if TSS_DETECT_EL_METHOD(49,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 50
      #if TSS_DETECT_EL_METHOD(50,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 51
      #if TSS_DETECT_EL_METHOD(51,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 52
      #if TSS_DETECT_EL_METHOD(52,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 53
      #if TSS_DETECT_EL_METHOD(53,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 54
      #if TSS_DETECT_EL_METHOD(54,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 55
      #if TSS_DETECT_EL_METHOD(55,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 56
      #if TSS_DETECT_EL_METHOD(56,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 57
      #if TSS_DETECT_EL_METHOD(57,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 58
      #if TSS_DETECT_EL_METHOD(58,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 59
      #if TSS_DETECT_EL_METHOD(59,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 60
      #if TSS_DETECT_EL_METHOD(60,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 61
      #if TSS_DETECT_EL_METHOD(61,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 62
      #if TSS_DETECT_EL_METHOD(62,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

    #if TSS_N_ELECTRODES > 63
      #if TSS_DETECT_EL_METHOD(63,TSIL)
        const TSS_TSIL_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT = { NULL };
              TSS_TSIL_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT = { 0u };
      #endif
    #endif

  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"
  #endif

#endif /* End of TSS_DETECT_METHOD(TSIL) */

