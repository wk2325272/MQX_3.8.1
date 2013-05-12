/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorCTS.h
*
* @brief  Functions Prototypes, constants, variables and macros for the sensing of electrodes 
*
* @version 1.0.31.0
* 
* @date Feb-3-2012
* 
*
***********************************************************************************************************************/


#ifndef __TSS_SENSORCTS_H__
  #define __TSS_SENSORCTS_H__

  #include "TSS_Sensor.h"

  #if TSS_HCS08_MCU || TSS_CFV1_MCU

    typedef INT8 (* TSS_CTS_FNC) (INT8 i8CTS_arg);

    /* Electrode Data */
    typedef struct {
      const UINT8 cts_cu8ModuleID;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        const INT8 cts_i8AmplitudeFilterDeltaLimit;
      #endif
      const TSS_CTS_FNC cts_cps8CTSLowRoutine; 
    } TSS_CTS_ELECTRODE_ROMDATA;    
    
    typedef struct {
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        INT8 cts_i8AmplitudeFilterBase;
      #endif
      INT8 cts_i8turbo;
    } TSS_CTS_ELECTRODE_RAMDATA;

  #elif TSS_KINETIS_MCU || TSS_CFM_MCU

    typedef INT16 (* TSS_CTS_FNC) (INT16 i16CTS_arg);

    /* Method Data */
    typedef struct {
      const TSS_METHOD_CONTROL cts_fMethodControl;
    } TSS_CTS_METHOD_ROMDATA;

    /* Electrode Data */        
    typedef struct {
      const TSS_CTS_METHOD_ROMDATA * const cts_cpsu32MethodROMData; 
      const TSS_CTS_FNC cts_fSampleElectrodeLow; 
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        const INT8 cts_i8AmplitudeFilterDeltaLimit;
      #endif
    } TSS_CTS_ELECTRODE_ROMDATA;

    typedef struct {
      INT16 cts_i16turbo;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        UINT16 cts_u16AmplitudeFilterBase;
      #endif
    } TSS_CTS_ELECTRODE_RAMDATA;
    
  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    

  /************************ Function prototypes *********************/ 

  extern UINT8 CTS_SensorInit(void);
  /*
  function:   CTS_SensorInit
  brief:      Initializes modules for the sensing of the electrodes.
  param:      Void                           
  return:     Status code                              
  */

  extern UINT8 CTS_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command);
  /*
  function:   CTS_SampleElectrode
  brief:      Performs a valid reading of one electrode, stores the timer values and returns a status code
  param:      u8ElecNum   -  Number of electrode to be scanned
              u8Command - Measurement command
  return:     Status code                              
  */ 

  /************************ CTS Specfic Settings *********************/    
  #define TSS_CTS_FAULT_TIMEOUT               128U         
  #define TSS_CTS_SIGNAL_OFFSET               20U
  #define TSS_CTS_RECALIB_COUNTER_TIMEOUT     5U
  #define TSS_CTS_RECALIB_HIGH_LIMIT          (0x7FFFu - 0xFFu)
  #define TSS_CTS_MIN_TURBO_SIGNAL            5
  
  /*********************************************************************************
  *    Determination if CTS method is used for later conditional CTS module 
  *    initialization
  *********************************************************************************/
  
  #define TSS_CTS_USED           (TSS_DETECT_EL_MODULE(0,CTS) || TSS_DETECT_EL_MODULE(1,CTS) || TSS_DETECT_EL_MODULE(2,CTS) || TSS_DETECT_EL_MODULE(3,CTS) || TSS_DETECT_EL_MODULE(4,CTS) || TSS_DETECT_EL_MODULE(5,CTS) || TSS_DETECT_EL_MODULE(6,CTS) || TSS_DETECT_EL_MODULE(7,CTS) || TSS_DETECT_EL_MODULE(8,CTS) || TSS_DETECT_EL_MODULE(9,CTS) || TSS_DETECT_EL_MODULE(10,CTS) || TSS_DETECT_EL_MODULE(11,CTS) || TSS_DETECT_EL_MODULE(12,CTS) || TSS_DETECT_EL_MODULE(13,CTS) || TSS_DETECT_EL_MODULE(14,CTS) || TSS_DETECT_EL_MODULE(15,CTS) || TSS_DETECT_EL_MODULE(16,CTS) || TSS_DETECT_EL_MODULE(17,CTS) || TSS_DETECT_EL_MODULE(18,CTS) || TSS_DETECT_EL_MODULE(19,CTS) || TSS_DETECT_EL_MODULE(20,CTS) || TSS_DETECT_EL_MODULE(21,CTS) || TSS_DETECT_EL_MODULE(22,CTS) || TSS_DETECT_EL_MODULE(23,CTS) || TSS_DETECT_EL_MODULE(24,CTS) || TSS_DETECT_EL_MODULE(25,CTS) || TSS_DETECT_EL_MODULE(26,CTS) || TSS_DETECT_EL_MODULE(27,CTS) || TSS_DETECT_EL_MODULE(28,CTS) || TSS_DETECT_EL_MODULE(29,CTS) || TSS_DETECT_EL_MODULE(30,CTS) || TSS_DETECT_EL_MODULE(31,CTS) || TSS_DETECT_EL_MODULE(32,CTS) || TSS_DETECT_EL_MODULE(33,CTS) || TSS_DETECT_EL_MODULE(34,CTS) || TSS_DETECT_EL_MODULE(35,CTS) || TSS_DETECT_EL_MODULE(36,CTS) || TSS_DETECT_EL_MODULE(37,CTS) || TSS_DETECT_EL_MODULE(38,CTS) || TSS_DETECT_EL_MODULE(39,CTS) || TSS_DETECT_EL_MODULE(40,CTS) || TSS_DETECT_EL_MODULE(41,CTS) || TSS_DETECT_EL_MODULE(42,CTS) || TSS_DETECT_EL_MODULE(43,CTS) || TSS_DETECT_EL_MODULE(44,CTS) || TSS_DETECT_EL_MODULE(45,CTS) || TSS_DETECT_EL_MODULE(46,CTS) || TSS_DETECT_EL_MODULE(47,CTS) || TSS_DETECT_EL_MODULE(48,CTS) || TSS_DETECT_EL_MODULE(49,CTS) || TSS_DETECT_EL_MODULE(50,CTS) || TSS_DETECT_EL_MODULE(51,CTS) || TSS_DETECT_EL_MODULE(52,CTS) || TSS_DETECT_EL_MODULE(53,CTS) || TSS_DETECT_EL_MODULE(54,CTS) || TSS_DETECT_EL_MODULE(55,CTS) || TSS_DETECT_EL_MODULE(56,CTS) || TSS_DETECT_EL_MODULE(57,CTS) || TSS_DETECT_EL_MODULE(58,CTS) || TSS_DETECT_EL_MODULE(59,CTS) || TSS_DETECT_EL_MODULE(60,CTS) || TSS_DETECT_EL_MODULE(61,CTS) || TSS_DETECT_EL_MODULE(62,CTS) || TSS_DETECT_EL_MODULE(63,CTS))
  
  #define TSS_CTS_METHOD_USED    (TSS_DETECT_EL_METHOD(0,CTS) || TSS_DETECT_EL_METHOD(1,CTS) || TSS_DETECT_EL_METHOD(2,CTS) || TSS_DETECT_EL_METHOD(3,CTS) || TSS_DETECT_EL_METHOD(4,CTS) || TSS_DETECT_EL_METHOD(5,CTS) || TSS_DETECT_EL_METHOD(6,CTS) || TSS_DETECT_EL_METHOD(7,CTS) || TSS_DETECT_EL_METHOD(8,CTS) || TSS_DETECT_EL_METHOD(9,CTS) || TSS_DETECT_EL_METHOD(10,CTS) || TSS_DETECT_EL_METHOD(11,CTS) || TSS_DETECT_EL_METHOD(12,CTS) || TSS_DETECT_EL_METHOD(13,CTS) || TSS_DETECT_EL_METHOD(14,CTS) || TSS_DETECT_EL_METHOD(15,CTS) || TSS_DETECT_EL_METHOD(16,CTS) || TSS_DETECT_EL_METHOD(17,CTS) || TSS_DETECT_EL_METHOD(18,CTS) || TSS_DETECT_EL_METHOD(19,CTS) || TSS_DETECT_EL_METHOD(20,CTS) || TSS_DETECT_EL_METHOD(21,CTS) || TSS_DETECT_EL_METHOD(22,CTS) || TSS_DETECT_EL_METHOD(23,CTS) || TSS_DETECT_EL_METHOD(24,CTS) || TSS_DETECT_EL_METHOD(25,CTS) || TSS_DETECT_EL_METHOD(26,CTS) || TSS_DETECT_EL_METHOD(27,CTS) || TSS_DETECT_EL_METHOD(28,CTS) || TSS_DETECT_EL_METHOD(29,CTS) || TSS_DETECT_EL_METHOD(30,CTS) || TSS_DETECT_EL_METHOD(31,CTS) || TSS_DETECT_EL_METHOD(32,CTS) || TSS_DETECT_EL_METHOD(33,CTS) || TSS_DETECT_EL_METHOD(34,CTS) || TSS_DETECT_EL_METHOD(35,CTS) || TSS_DETECT_EL_METHOD(36,CTS) || TSS_DETECT_EL_METHOD(37,CTS) || TSS_DETECT_EL_METHOD(38,CTS) || TSS_DETECT_EL_METHOD(39,CTS) || TSS_DETECT_EL_METHOD(40,CTS) || TSS_DETECT_EL_METHOD(41,CTS) || TSS_DETECT_EL_METHOD(42,CTS) || TSS_DETECT_EL_METHOD(43,CTS) || TSS_DETECT_EL_METHOD(44,CTS) || TSS_DETECT_EL_METHOD(45,CTS) || TSS_DETECT_EL_METHOD(46,CTS) || TSS_DETECT_EL_METHOD(47,CTS) || TSS_DETECT_EL_METHOD(48,CTS) || TSS_DETECT_EL_METHOD(49,CTS) || TSS_DETECT_EL_METHOD(50,CTS) || TSS_DETECT_EL_METHOD(51,CTS) || TSS_DETECT_EL_METHOD(52,CTS) || TSS_DETECT_EL_METHOD(53,CTS) || TSS_DETECT_EL_METHOD(54,CTS) || TSS_DETECT_EL_METHOD(55,CTS) || TSS_DETECT_EL_METHOD(56,CTS) || TSS_DETECT_EL_METHOD(57,CTS) || TSS_DETECT_EL_METHOD(58,CTS) || TSS_DETECT_EL_METHOD(59,CTS) || TSS_DETECT_EL_METHOD(60,CTS) || TSS_DETECT_EL_METHOD(61,CTS) || TSS_DETECT_EL_METHOD(62,CTS) || TSS_DETECT_EL_METHOD(63,CTS))

  /**************** ROM & RAM Data prototypes ********************/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,CTS))
      extern const TSS_CTS_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT;
      extern       TSS_CTS_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT;
    #endif
  #endif
      
 /* 
  * Handler Body of the measurement method 
  */

  #if TSS_HCS08_MCU || TSS_CFV1_MCU
    #define  TSS_CTS_SAMPLE_ELECTRODE_LOW(el)      ;       /* Nothing */
  #elif TSS_KINETIS_MCU || TSS_CFM_MCU
    #define  TSS_CTS_SAMPLE_ELECTRODE_LOW(el)      ;       /* Nothing */
  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    
 
  /*********** Sample Electrode Low Level Routine assignment *******/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,CTS))
      #define TSS_E0_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(0)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,CTS))
      #define TSS_E1_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(1)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,CTS))
      #define TSS_E2_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(2)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,CTS))
      #define TSS_E3_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(3)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,CTS))
      #define TSS_E4_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(4)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,CTS))
      #define TSS_E5_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(5)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,CTS))
      #define TSS_E6_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(6)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,CTS))
      #define TSS_E7_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(7)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,CTS))
      #define TSS_E8_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(8)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,CTS))
      #define TSS_E9_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(9)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,CTS))
      #define TSS_E10_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(10)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,CTS))
      #define TSS_E11_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(11)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,CTS))
      #define TSS_E12_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(12)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,CTS))
      #define TSS_E13_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(13)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,CTS))
      #define TSS_E14_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(14)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,CTS))
      #define TSS_E15_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(15)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,CTS))
      #define TSS_E16_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(16)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,CTS))
      #define TSS_E17_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(17)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,CTS))
      #define TSS_E18_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(18)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,CTS))
      #define TSS_E19_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(19)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,CTS))
      #define TSS_E20_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(20)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,CTS))
      #define TSS_E21_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(21)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,CTS))
      #define TSS_E22_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(22)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,CTS))
      #define TSS_E23_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(23)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,CTS))
      #define TSS_E24_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(24)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,CTS))
      #define TSS_E25_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(25)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,CTS))
      #define TSS_E26_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(26)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,CTS))
      #define TSS_E27_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(27)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,CTS))
      #define TSS_E28_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(28)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,CTS))
      #define TSS_E29_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(29)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,CTS))
      #define TSS_E30_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(30)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,CTS))
      #define TSS_E31_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(31)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,CTS))
      #define TSS_E32_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(32)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,CTS))
      #define TSS_E33_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(33)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,CTS))
      #define TSS_E34_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(34)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,CTS))
      #define TSS_E35_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(35)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,CTS))
      #define TSS_E36_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(36)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,CTS))
      #define TSS_E37_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(37)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,CTS))
      #define TSS_E38_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(38)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,CTS))
      #define TSS_E39_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(39)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,CTS))
      #define TSS_E40_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(40)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,CTS))
      #define TSS_E41_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(41)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,CTS))
      #define TSS_E42_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(42)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,CTS))
      #define TSS_E43_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(43)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,CTS))
      #define TSS_E44_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(44)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,CTS))
      #define TSS_E45_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(45)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,CTS))
      #define TSS_E46_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(46)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,CTS))
      #define TSS_E47_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(47)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,CTS))
      #define TSS_E48_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(48)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,CTS))
      #define TSS_E49_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(49)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,CTS))
      #define TSS_E50_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(50)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,CTS))
      #define TSS_E51_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(51)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,CTS))
      #define TSS_E52_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(52)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,CTS))
      #define TSS_E53_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(53)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,CTS))
      #define TSS_E54_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(54)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,CTS))
      #define TSS_E55_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(55)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,CTS))
      #define TSS_E56_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(56)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,CTS))
      #define TSS_E57_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(57)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,CTS))
      #define TSS_E58_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(58)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,CTS))
      #define TSS_E59_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(59)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,CTS))
      #define TSS_E60_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(60)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,CTS))
      #define TSS_E61_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(61)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,CTS))
      #define TSS_E62_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(62)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,CTS))
      #define TSS_E63_SAMPLE_ELECTRODE_LOW()   TSS_CTS_SAMPLE_ELECTRODE_LOW(63)
    #endif
  #endif

  /*************** Sample Electrode Routine assignment *************/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,CTS))
      #define TSS_E0_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,CTS))
      #define TSS_E1_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,CTS))
      #define TSS_E2_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,CTS))
      #define TSS_E3_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,CTS))
      #define TSS_E4_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,CTS))
      #define TSS_E5_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,CTS))
      #define TSS_E6_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,CTS))
      #define TSS_E7_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,CTS))
      #define TSS_E8_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,CTS))
      #define TSS_E9_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,CTS))
      #define TSS_E10_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,CTS))
      #define TSS_E11_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,CTS))
      #define TSS_E12_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,CTS))
      #define TSS_E13_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,CTS))
      #define TSS_E14_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,CTS))
      #define TSS_E15_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,CTS))
      #define TSS_E16_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,CTS))
      #define TSS_E17_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,CTS))
      #define TSS_E18_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,CTS))
      #define TSS_E19_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,CTS))
      #define TSS_E20_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,CTS))
      #define TSS_E21_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,CTS))
      #define TSS_E22_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,CTS))
      #define TSS_E23_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,CTS))
      #define TSS_E24_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,CTS))
      #define TSS_E25_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,CTS))
      #define TSS_E26_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,CTS))
      #define TSS_E27_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,CTS))
      #define TSS_E28_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,CTS))
      #define TSS_E29_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,CTS))
      #define TSS_E30_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,CTS))
      #define TSS_E31_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,CTS))
      #define TSS_E32_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,CTS))
      #define TSS_E33_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,CTS))
      #define TSS_E34_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,CTS))
      #define TSS_E35_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,CTS))
      #define TSS_E36_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,CTS))
      #define TSS_E37_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,CTS))
      #define TSS_E38_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,CTS))
      #define TSS_E39_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,CTS))
      #define TSS_E40_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,CTS))
      #define TSS_E41_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,CTS))
      #define TSS_E42_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,CTS))
      #define TSS_E43_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,CTS))
      #define TSS_E44_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,CTS))
      #define TSS_E45_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,CTS))
      #define TSS_E46_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,CTS))
      #define TSS_E47_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,CTS))
      #define TSS_E48_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,CTS))
      #define TSS_E49_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,CTS))
      #define TSS_E50_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,CTS))
      #define TSS_E51_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,CTS))
      #define TSS_E52_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,CTS))
      #define TSS_E53_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,CTS))
      #define TSS_E54_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,CTS))
      #define TSS_E55_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,CTS))
      #define TSS_E56_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,CTS))
      #define TSS_E57_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,CTS))
      #define TSS_E58_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,CTS))
      #define TSS_E59_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,CTS))
      #define TSS_E60_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,CTS))
      #define TSS_E61_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,CTS))
      #define TSS_E62_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,CTS))
      #define TSS_E63_SAMPLE_ELECTRODE()    CTS_SampleElectrode
    #endif
  #endif
      
  /************** TSS_SystemSetup.h Settings validation ***************/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,CTS))
      #ifndef TSS_E0_P
        #error TSS - TSS_E0_P is not defined
      #endif
      #ifndef TSS_E0_B
        #error TSS - TSS_E0_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,CTS))
      #ifndef TSS_E1_P
        #error TSS - TSS_E1_P is not defined
      #endif
      #ifndef TSS_E1_B
        #error TSS - TSS_E1_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,CTS))
      #ifndef TSS_E2_P
        #error TSS - TSS_E2_P is not defined
      #endif
      #ifndef TSS_E2_B
        #error TSS - TSS_E2_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,CTS))
      #ifndef TSS_E3_P
        #error TSS - TSS_E3_P is not defined
      #endif
      #ifndef TSS_E3_B
        #error TSS - TSS_E3_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,CTS))
      #ifndef TSS_E4_P
        #error TSS - TSS_E4_P is not defined
      #endif
      #ifndef TSS_E4_B
        #error TSS - TSS_E4_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,CTS))
      #ifndef TSS_E5_P
        #error TSS - TSS_E5_P is not defined
      #endif
      #ifndef TSS_E5_B
        #error TSS - TSS_E5_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,CTS))
      #ifndef TSS_E6_P
        #error TSS - TSS_E6_P is not defined
      #endif
      #ifndef TSS_E6_B
        #error TSS - TSS_E6_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,CTS))
      #ifndef TSS_E7_P
        #error TSS - TSS_E7_P is not defined
      #endif
      #ifndef TSS_E7_B
        #error TSS - TSS_E7_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,CTS))
      #ifndef TSS_E8_P
        #error TSS - TSS_E8_P is not defined
      #endif
      #ifndef TSS_E8_B
        #error TSS - TSS_E8_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,CTS))
      #ifndef TSS_E9_P
        #error TSS - TSS_E9_P is not defined
      #endif
      #ifndef TSS_E9_B
        #error TSS - TSS_E9_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,CTS))
      #ifndef TSS_E10_P
        #error TSS - TSS_E10_P is not defined
      #endif
      #ifndef TSS_E10_B
        #error TSS - TSS_E10_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,CTS))
      #ifndef TSS_E11_P
        #error TSS - TSS_E11_P is not defined
      #endif
      #ifndef TSS_E11_B
        #error TSS - TSS_E11_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,CTS))
      #ifndef TSS_E12_P
        #error TSS - TSS_E12_P is not defined
      #endif
      #ifndef TSS_E12_B
        #error TSS - TSS_E12_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,CTS))
      #ifndef TSS_E13_P
        #error TSS - TSS_E13_P is not defined
      #endif
      #ifndef TSS_E13_B
        #error TSS - TSS_E13_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,CTS))
      #ifndef TSS_E14_P
        #error TSS - TSS_E14_P is not defined
      #endif
      #ifndef TSS_E14_B
        #error TSS - TSS_E14_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,CTS))
      #ifndef TSS_E15_P
        #error TSS - TSS_E15_P is not defined
      #endif
      #ifndef TSS_E15_B
        #error TSS - TSS_E15_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,CTS))
      #ifndef TSS_E16_P
        #error TSS - TSS_E16_P is not defined
      #endif
      #ifndef TSS_E16_B
        #error TSS - TSS_E16_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,CTS))
      #ifndef TSS_E17_P
        #error TSS - TSS_E17_P is not defined
      #endif
      #ifndef TSS_E17_B
        #error TSS - TSS_E17_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,CTS))
      #ifndef TSS_E18_P
        #error TSS - TSS_E18_P is not defined
      #endif
      #ifndef TSS_E18_B
        #error TSS - TSS_E18_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,CTS))
      #ifndef TSS_E19_P
        #error TSS - TSS_E19_P is not defined
      #endif
      #ifndef TSS_E19_B
        #error TSS - TSS_E19_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,CTS))
      #ifndef TSS_E20_P
        #error TSS - TSS_E20_P is not defined
      #endif
      #ifndef TSS_E20_B
        #error TSS - TSS_E20_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,CTS))
      #ifndef TSS_E21_P
        #error TSS - TSS_E21_P is not defined
      #endif
      #ifndef TSS_E21_B
        #error TSS - TSS_E21_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,CTS))
      #ifndef TSS_E22_P
        #error TSS - TSS_E22_P is not defined
      #endif
      #ifndef TSS_E22_B
        #error TSS - TSS_E22_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,CTS))
      #ifndef TSS_E23_P
        #error TSS - TSS_E23_P is not defined
      #endif
      #ifndef TSS_E23_B
        #error TSS - TSS_E23_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,CTS))
      #ifndef TSS_E24_P
        #error TSS - TSS_E24_P is not defined
      #endif
      #ifndef TSS_E24_B
        #error TSS - TSS_E24_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,CTS))
      #ifndef TSS_E25_P
        #error TSS - TSS_E25_P is not defined
      #endif
      #ifndef TSS_E25_B
        #error TSS - TSS_E25_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,CTS))
      #ifndef TSS_E26_P
        #error TSS - TSS_E26_P is not defined
      #endif
      #ifndef TSS_E26_B
        #error TSS - TSS_E26_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,CTS))
      #ifndef TSS_E27_P
        #error TSS - TSS_E27_P is not defined
      #endif
      #ifndef TSS_E27_B
        #error TSS - TSS_E27_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,CTS))
      #ifndef TSS_E28_P
        #error TSS - TSS_E28_P is not defined
      #endif
      #ifndef TSS_E28_B
        #error TSS - TSS_E28_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,CTS))
      #ifndef TSS_E29_P
        #error TSS - TSS_E29_P is not defined
      #endif
      #ifndef TSS_E29_B
        #error TSS - TSS_E29_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,CTS))
      #ifndef TSS_E30_P
        #error TSS - TSS_E30_P is not defined
      #endif
      #ifndef TSS_E30_B
        #error TSS - TSS_E30_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,CTS))
      #ifndef TSS_E31_P
        #error TSS - TSS_E31_P is not defined
      #endif
      #ifndef TSS_E31_B
        #error TSS - TSS_E31_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,CTS))
      #ifndef TSS_E32_P
        #error TSS - TSS_E32_P is not defined
      #endif
      #ifndef TSS_E32_B
        #error TSS - TSS_E32_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,CTS))
      #ifndef TSS_E33_P
        #error TSS - TSS_E33_P is not defined
      #endif
      #ifndef TSS_E33_B
        #error TSS - TSS_E33_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,CTS))
      #ifndef TSS_E34_P
        #error TSS - TSS_E34_P is not defined
      #endif
      #ifndef TSS_E34_B
        #error TSS - TSS_E34_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,CTS))
      #ifndef TSS_E35_P
        #error TSS - TSS_E35_P is not defined
      #endif
      #ifndef TSS_E35_B
        #error TSS - TSS_E35_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,CTS))
      #ifndef TSS_E36_P
        #error TSS - TSS_E36_P is not defined
      #endif
      #ifndef TSS_E36_B
        #error TSS - TSS_E36_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,CTS))
      #ifndef TSS_E37_P
        #error TSS - TSS_E37_P is not defined
      #endif
      #ifndef TSS_E37_B
        #error TSS - TSS_E37_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,CTS))
      #ifndef TSS_E38_P
        #error TSS - TSS_E38_P is not defined
      #endif
      #ifndef TSS_E38_B
        #error TSS - TSS_E38_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,CTS))
      #ifndef TSS_E39_P
        #error TSS - TSS_E39_P is not defined
      #endif
      #ifndef TSS_E39_B
        #error TSS - TSS_E39_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,CTS))
      #ifndef TSS_E40_P
        #error TSS - TSS_E40_P is not defined
      #endif
      #ifndef TSS_E40_B
        #error TSS - TSS_E40_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,CTS))
      #ifndef TSS_E41_P
        #error TSS - TSS_E41_P is not defined
      #endif
      #ifndef TSS_E41_B
        #error TSS - TSS_E41_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,CTS))
      #ifndef TSS_E42_P
        #error TSS - TSS_E42_P is not defined
      #endif
      #ifndef TSS_E42_B
        #error TSS - TSS_E42_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,CTS))
      #ifndef TSS_E43_P
        #error TSS - TSS_E43_P is not defined
      #endif
      #ifndef TSS_E43_B
        #error TSS - TSS_E43_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,CTS))
      #ifndef TSS_E44_P
        #error TSS - TSS_E44_P is not defined
      #endif
      #ifndef TSS_E44_B
        #error TSS - TSS_E44_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,CTS))
      #ifndef TSS_E45_P
        #error TSS - TSS_E45_P is not defined
      #endif
      #ifndef TSS_E45_B
        #error TSS - TSS_E45_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,CTS))
      #ifndef TSS_E46_P
        #error TSS - TSS_E46_P is not defined
      #endif
      #ifndef TSS_E46_B
        #error TSS - TSS_E46_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,CTS))
      #ifndef TSS_E47_P
        #error TSS - TSS_E47_P is not defined
      #endif
      #ifndef TSS_E47_B
        #error TSS - TSS_E47_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,CTS))
      #ifndef TSS_E48_P
        #error TSS - TSS_E48_P is not defined
      #endif
      #ifndef TSS_E48_B
        #error TSS - TSS_E48_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,CTS))
      #ifndef TSS_E49_P
        #error TSS - TSS_E49_P is not defined
      #endif
      #ifndef TSS_E49_B
        #error TSS - TSS_E49_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,CTS))
      #ifndef TSS_E50_P
        #error TSS - TSS_E50_P is not defined
      #endif
      #ifndef TSS_E50_B
        #error TSS - TSS_E50_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,CTS))
      #ifndef TSS_E51_P
        #error TSS - TSS_E51_P is not defined
      #endif
      #ifndef TSS_E51_B
        #error TSS - TSS_E51_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,CTS))
      #ifndef TSS_E52_P
        #error TSS - TSS_E52_P is not defined
      #endif
      #ifndef TSS_E52_B
        #error TSS - TSS_E52_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,CTS))
      #ifndef TSS_E53_P
        #error TSS - TSS_E53_P is not defined
      #endif
      #ifndef TSS_E53_B
        #error TSS - TSS_E53_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,CTS))
      #ifndef TSS_E54_P
        #error TSS - TSS_E54_P is not defined
      #endif
      #ifndef TSS_E54_B
        #error TSS - TSS_E54_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,CTS))
      #ifndef TSS_E55_P
        #error TSS - TSS_E55_P is not defined
      #endif
      #ifndef TSS_E55_B
        #error TSS - TSS_E55_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,CTS))
      #ifndef TSS_E56_P
        #error TSS - TSS_E56_P is not defined
      #endif
      #ifndef TSS_E56_B
        #error TSS - TSS_E56_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,CTS))
      #ifndef TSS_E57_P
        #error TSS - TSS_E57_P is not defined
      #endif
      #ifndef TSS_E57_B
        #error TSS - TSS_E57_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,CTS))
      #ifndef TSS_E58_P
        #error TSS - TSS_E58_P is not defined
      #endif
      #ifndef TSS_E58_B
        #error TSS - TSS_E58_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,CTS))
      #ifndef TSS_E59_P
        #error TSS - TSS_E59_P is not defined
      #endif
      #ifndef TSS_E59_B
        #error TSS - TSS_E59_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,CTS))
      #ifndef TSS_E60_P
        #error TSS - TSS_E60_P is not defined
      #endif
      #ifndef TSS_E60_B
        #error TSS - TSS_E60_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,CTS))
      #ifndef TSS_E61_P
        #error TSS - TSS_E61_P is not defined
      #endif
      #ifndef TSS_E61_B
        #error TSS - TSS_E61_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,CTS))
      #ifndef TSS_E62_P
        #error TSS - TSS_E62_P is not defined
      #endif
      #ifndef TSS_E62_B
        #error TSS - TSS_E62_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,CTS))
      #ifndef TSS_E63_P
        #error TSS - TSS_E63_P is not defined
      #endif
      #ifndef TSS_E63_B
        #error TSS - TSS_E63_B is not defined
      #endif
    #endif
  #endif

  /* Other Validation */
      
  #if TSS_DETECT_METHOD(CTS)
    #if ((TSS_HCS08_MCU == 1) || (TSS_CFV1_MCU == 1))
      #if (TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW == 1)
          #error "TSS - Function TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW is not supported for CTS method on HCS08 and CFV1 mcu."
      #endif
    #endif
  #endif
      
  #if TSS_DETECT_METHOD(CTS)
    #if (TSS_HCS08_MCU == 1)
      #if (TSS_GPIO_VERSION == 2)
        #error "TSS - CTS method is not supported on this MCU"
      #endif
    #endif
  #endif
  
  /*******************************************************
  ******        Determine what is needed             *****
  *******************************************************/

  #if TSS_DETECT_METHOD(CTS)
    /* HW Timer */  
    #ifndef TSS_HW_TIMER_USED
      #define TSS_HW_TIMER_USED      0
    #endif
    /* GPIO */        
    #ifndef TSS_HW_GPIO_USED
      #define TSS_HW_GPIO_USED       1
    #else
      #undef TSS_HW_GPIO_USED
      #define TSS_HW_GPIO_USED       1
    #endif  
  #endif  

  /*******************************************************************************
  ****  CTS Low defines 
  *******************************************************************************/

  /* CTS passing arguments ... */
  #define cCTS_ARG_TURBO      (-1)  /* to get turbo value in Sens-loop ratio */
  #define cCTS_ARG_RESET      (-2)  /* initialize port io */
  #define cCTS_ARG_RATIO      (-3)  /* to get CTS_RATIO */
    
  /* CTS returning errors ... */
  #define cCTS_ERROR_LoCAP    ( 0)
  #define cCTS_ERROR_ExISR    (-1)
  #define cCTS_ERROR_LoPIN    (-2)
  #define cCTS_ERROR_HiPIN    (-3)
  #define cCTS_ERROR_HiCAP    (-128)
  
  /* CTS returning errors ... */
  #define CTS_RATIO               0x7E  /* High_NIB=Turbo-loop, Low_NIB=Sens-loop */
  #define ExISR_ENABLED           1     /* default is 1, ex-post ISR detection */
  #define CTS_KINETIS_ASM_LOOPS   0     /* default is 1, asm code keep constant execution bus-cycles */

  /*******************************************************************************
  ****    CTS MCU platform - SELECTOR
  *******************************************************************************/
  //C08,S08,RS08
  //C12,S12,S12X,XGATE
  //DSC
  //MCF(V1..V4)
  //MPC(55..58)


  /*M08[CSR] -------------------------------------------------------------------*/

  #if (defined(__HC08__) || defined(__RS08__))
      

      /*CTS_PLATFORM*/
              #define __M08__       //[HC08|HCS08|RS08]


      //ASSEMBLER/COMPILER            [  COMBINATION  ]   ->  LIB/__DEF__      (SELECTOR
      //================================================================================================
      //   -C08  /-C08                [HC08|HCS08|____]          /__M08CS__    (__HC08__               
      //   -Cs08 /-Cs08               [____|HCS08|____]   ->  S08/__M08S__     (__HCS08__              
      //   -Csr08/empty               [____|_____|RS08]   ->  R08/__M08R__     (__RS08__               
      //                              [HC08|_____|____]   ->  C08/__M08C__     (__HC08__  && !__HCS08__
      //------------------------------------------------------------------------------------------------
      //                              [____|HCS08|RS08]          /__M08SR__    (__HCS08__ ||  __RS08__ 
      //                              [HC08|HCS08|RS08]          /__M08__      (__HC08__  ||  __RS08__ 
              
      /*LIBRARY*/
          #if (defined(__HC08__) && !defined(__HCS08__))
              #define __M08C__      //[HC08|_____|____]
          #elif (defined(__HCS08__))
              #define __M08S__      //[____|HCS08|____]
          #elif (defined(__RS08__))
              #define __M08R__      //[____|_____|RS08]
          #endif
          
      /*BASIC_COMBINATION*/
          #if (defined(__HC08__))
              #define __M08CS__     //[HC08|HCS08|____]
          #endif
          #if (defined(__HCS08__) || defined(__RS08__))
              #define __M08SR__     //[____|HCS08|RS08]        
          #endif
              
      /*CW coding*/
          #define __asf     //NA
          #define __asi asm 
          #define __aso
          
          #define __BGND //??? 
              
          #define __CW_COMPILER__    __VERSION__  // __VERSION_STR__
          #define _ctsabi  //NA
              
          #ifdef __M08CS__
              #define __DCX_EXPANSION_ENABLED__
              #define __DCB   DCB
              #define __DCW   DCW
              #define __DCL   DCL
          #else //M08R
              //EXPANSION: NO .. but char in string (macro defined) can be indexed (MACRO_STRING:idx)
              #define __DCX_EXPANSION_DISABLED__ //NO expansion
              #define __DCB   DC.B
              #define __DCW   DC.W
              #define __DCL   DC.L
          #endif
              
  #endif /*M08*/
      
      
  /*M12[CSXE] ------------------------------------------------------------------*/

  #if  (defined(__HC12__))
      //-Cpu(CPU12|HCS12|HCS12X|HCS12XE)
   

      /*CTS_PLATFORM*/            
              #define __M12__       //[HC12|HCS12|HCS12X|HCS12XE]


      //  ASSEMBLER/COMPILER          [ COMBINATION             ] ->  LIB/__DEF__     (SELECTOR
      //============================================================================================================
      //-CpuCPU12  /-CpuCPU12         [HC12|HCS12|HCS12X|HCS12XE]        /__M12__     (__HC12__           
      //-CpuHCS12  /-CpuHCS12         [____|HCS12|HCS12X|HCS12XE]        /__M12SXE__  (__HCS12__
      //-CpuHCS12X /-CpuHCS12X        [____|_____|HCS12X|HCS12XE]        /__M12XE__   (__HCS12X__
      //-CpuHCS12XE/-CpuHCS12XE       [____|_____|______|HCS12XE] ->  E12/__M12E__    (__HCS12XE__
      //                              [____|_____|HCS12X|_______] ->  X12/__M12X__    (__HCS12X__ && !__HCS12XE__
      //                              [____|HCS12|______|_______] ->  S12/__M12S__    (__HCS12__ && !__HCS12X__ && !__HCS12XE__
      //                              [HC12|_____|______|_______] ->  C12/__M12C__    (__HC12__ && !__HCS12__ && !__HCS12X__ && !__HCS12XE__
      //------------------------------------------------------------------------------------------------------------
      //                              [HC12|HCS12|______|_______]        /__M12CS__   (custom
      //                              [HC12|_____|HCS12X|_______]        /__M12CX__   (custom
      //                              [____|HCS12|HCS12X|_______]        /__M12SX__   (custom
      //                              [HC12|HCS12|HCS12X|_______]        /__M12CSX__  (custom
      //                              [HC12|_____|______|HCS12XE]        /__M12CE__   (custom
      //                              [____|HCS12|______|HCS12XE]        /__M12SE__   (custom
      //                              [HC12|HCS12|______|HCS12XE]        /__M12CSE__  (custom
      //                              [HC12|_____|HCS12X|HCS12XE]        /__M12CXE__  (custom


      /*LIBRARY*/
          #if (defined(__HC12__) && !defined(__HCS12__)  && !defined(__HCS12X__)  && !defined(__HCS12XE__))
              #define __M12C__      //[HC12|_____|______|_______]
          #elif  (defined(__HCS12__)  && !defined(__HCS12X__)  && !defined(__HCS12XE__))
              #define __M12S__      //[____|HCS12|______|_______]
          #elif  (defined(__HCS12X__)  && !defined(__HCS12XE__))
              #define __M12X__      //[____|_____|HCS12X|_______]
          #elif  (defined(__HCS12XE__))
              #define __M12E__      //[____|_____|______|HCS12XE]
          #endif

      /*BASIC_COMBINATION*/
          #if (defined(__HCS12__))
              #define __M12SXE__    //[____|HCS12|HCS12X|HCS12XE]
          #endif
          #if (defined(__HCS12X__))
              #define __M12XE__     //[____|_____|HCS12X|HCS12XE]
          #endif

      /*CW coding*/
          #define __asf     //NA
          #define __asi asm 
          #define __aso 
          
          #define __BGND //???
              
          #define __CW_COMPILER__    __VERSION__  // __VERSION_STR__
          #define _ctsabi  //NA
              
          #define __DCX_EXPANSION_ENABLED__
          #define __DCB   DC.B
          #define __DCW   DC.W
          #define __DCL   DC.L
              
  #endif /*M12*/


  #if (defined(__XGATE__))
     
      #ifdef XGATE_MSG_ENABLED
      #warning "\n\
      Note that CTS routines could be interrupted by priority 4-7 ... only on XE version of XGATE core.\n\
      XD version is OK because this XGATE has no ability interrupt executed trigger routines anyway!!!"
      #endif
              #define __MXG__                                 //XGATE
        
              
      /*CW coding*/
          #define __asf     //NA
          #define __asi asm 
          #define __aso 
          
          #define __BGND asm BGND    //"Enter Background Debug Mode" == hard stop
              
          #define __CW_COMPILER__    __VERSION__  // __VERSION_STR__
          #define _ctsabi  //NA
              
          #define __DCX_EXPANSION_ENABLED__
          #define __DCB   DC.B
          #define __DCW   DC.W
          #define __DCL   DC.L
  #endif


  /*M56, MFC, DSC  ------------------------------------------------------------------*/
  /*
  DSC56800E
  =========
  DSP56F80x
  DSP56F82x
  DSP5685x
  MC56F800x
  MC56F801x
  MC56F802x
  MC56F803x
  MC56F81xx
  MC56F83xx
  */
  #if (defined(__m56800__) || defined(__m56800E__))
      #error "Sorry ... this MCU is not supported by CTS at this time!!!"
  #endif


  /*MCF[1234] ------------------------------------------------------------------*/

  #if  (defined(__COLDFIRE__))
      
      #define __MCF__
      
      #if !(__COLDFIRE__ == __MCF51QE__ || __COLDFIRE__ == __MCF51AC__ || __COLDFIRE__ == __MCF51JM__)
      #endif
      
          #define __MCF1__
          #define __FLEXIS__

          #define __asf asm         //asm FUNCTION
          #define __asi asm{        //asm( asm{
          #define __aso     }       //    )    }
    
          #define __BGND
          
          #define __CW_COMPILER__     __BACKENDVERSION__         // __MWERKS__  __CWCC__
          #define _ctsabi            __declspec(register_abi)    //standard_abi (the default), compact_abi
              
          #define __DCX_EXPANSION_ENABLED__
          #define __DCB   DC.B
          #define __DCW   DC.W
          #define __DCL   DC.L

  #endif


  /*i.MX and others ------------------------------------------------------------------*/
  #if  (defined(__MC68K__) || defined(__INTEL__) || defined(__POWERPC__) || defined(__MIPS__) || defined (__MCORE__) || defined (_ENTERPRISE_C_) || defined(__PPCGEKKO__) || defined(__PPC_EABI__) || defined (__SH__))
      #error "Sorry ... this MCU is not supported by CTS at this time!!!"
  #endif


  /*******************************************************************************
  ****    CTS platform - ILOPs
  *******************************************************************************/
      #define ILOP_M08CS      asm DCB $8d       // all HC08 & HCS08 (HCS08 instruction) 
      #define ILOP_M08C       asm DCB $32       // all RS08 & HC08 (HC08 instruction)
      #define ILOP_M08R       asm DC.B $32      // all RS08 & HC08 (RS08 instruction)
      #define ILOP_M12        asm DC.W $1832    // TRAP($18)<<8 | unimplemented opcode
      #define ILOP_MXG        asm DC.W $1001    // see AN3555 or XGATE specs (Instruction Coding)
      #define ILOP_MCF1       asm(DC.W 0x4AFC)  // ILLEGAL, Take Illegal Instruction Trap


  /*******************************************************************************
  ****    CTS platform - DEFINES
  *******************************************************************************/

  /*CTS_M08*--------------------------------------------------------------------*/

      #ifdef __M08__
          #define __CTS_M08__
      
      /*C = H111, (not)H121 bacause no pullup*/
          #ifdef __M08C__ 
              #define __CTS_M08C__
              #define CTS_PLATFORM_STR    "CTS_M08C"
              #define CTS_ILOP            ILOP_M08CS
          #endif
          
      /*S = H111,H112*/
          #ifdef __M08S__
              #define __CTS_M08S__
              #define CTS_PLATFORM_STR    "CTS_M08S"
              #define CTS_ILOP            ILOP_M08CS
          #endif
          
      /*R = H112,H122*/
          #ifdef __M08R__
              #define __CTS_M08R__
              #define CTS_PLATFORM_STR    "CTS_M08R"
              #define CTS_ILOP            ILOP_M08R
          #endif
          
      #endif /*__M08__*/



  /*CTS_M12*--------------------------------------------------------------------*/
      //be carefule about known code-ready-prediction issue when the code is
      //located close to the end of (PageN) and prediction mechanism pre-read
      //the code which will be on (PageN+1) so the next execution will be related
      //to (PageN) and the the right page (PageN+1)
      
      #ifdef __M12__
          #define __CTS_M12__
          
      /*C = H222*/
          #ifdef __M12C__ 
              #define __CTS_M12C__
              #define CTS_PLATFORM_STR    "CTS_M12C"
              #define CTS_ILOP            ILOP_M12
          #endif
          
      /*S = H222*/
          #ifdef __M12S__
              #define __CTS_M08S__
              #define CTS_PLATFORM_STR    "CTS_M12S"
              #define CTS_ILOP            ILOP_M12
          #endif
          
      /*X = H222*/
          #ifdef __M12X__
              #define __CTS_M12X__
              #define CTS_PLATFORM_STR    "CTS_M12X"
              #define CTS_ILOP            ILOP_M12
          #endif
          
      /*E = H222*/
          #ifdef __M12E__
              #define __CTS_M12E__
              #define CTS_PLATFORM_STR    "CTS_M12E"
              #define CTS_ILOP            ILOP_M12
          #endif

      
      #endif /*__M12__*/
      
      /*D,R = H222*/
      #ifdef __MXG__
          //recognize XD vs. XE versions!!!
              #define __CTS_MXG__
              
              #define CTS_PLATFORM_STR    "CTS_MXG" //D,E
              #define CTS_ILOP            ILOP_MXG
      #endif


  /*CTS_M56*--------------------------------------------------------------------*/
      #ifdef __M56__
          #define __CTS_M56__
          
      /*1 = H222*/
          #ifdef __M56__ 
              #define __CTS_M56__
              #define CTS_PLATFORM_STR    "CTS_M56"
              #define CTS_ILOP            ILOP_M56
          #endif
      
      #endif /*__MCF__*/
      

  /*CTS_MCF*--------------------------------------------------------------------*/

      #ifdef __MCF__ //__M68__
          #define __CTS_MCF__
          
      /*1 = H444*/
          #ifdef __MCF1__ 
              #define __CTS_MCF1__
              #define CTS_PLATFORM_STR    "CTS_MCF1"
              #define CTS_ILOP            ILOP_MCF1
          #endif
      
      #endif /*__MCF__*/
      

  /*******************************************************************************
  ****    CTS platform - HARDWARE
  *******************************************************************************/


  #if defined(__M08SR__) || defined(__MCF1__)
  //RS08[A-E],S08[A-L],MCF1[A-J]
  //A
      #define __CTS_A_p__ 0
      #define __CTS_A0t__ defined(PTADD_PTADD0) && defined(PTAPE_PTAPE0)
      #define __CTS_A1t__ defined(PTADD_PTADD1) && defined(PTAPE_PTAPE1)
      #define __CTS_A2t__ defined(PTADD_PTADD2) && defined(PTAPE_PTAPE2)
      #define __CTS_A3t__ defined(PTADD_PTADD3) && defined(PTAPE_PTAPE3)
      #define __CTS_A4t__ defined(PTADD_PTADD4) && defined(PTAPE_PTAPE4)
      #define __CTS_A5t__ defined(PTADD_PTADD5) && defined(PTAPE_PTAPE5)
      #define __CTS_A6t__ defined(PTADD_PTADD6) && defined(PTAPE_PTAPE6)
      #define __CTS_A7t__ defined(PTADD_PTADD7) && defined(PTAPE_PTAPE7)
  //B
      #define __CTS_B_p__ 0
      #define __CTS_B0t__ defined(PTBDD_PTBDD0) && defined(PTBPE_PTBPE0)
      #define __CTS_B1t__ defined(PTBDD_PTBDD1) && defined(PTBPE_PTBPE1)
      #define __CTS_B2t__ defined(PTBDD_PTBDD2) && defined(PTBPE_PTBPE2)
      #define __CTS_B3t__ defined(PTBDD_PTBDD3) && defined(PTBPE_PTBPE3)
      #define __CTS_B4t__ defined(PTBDD_PTBDD4) && defined(PTBPE_PTBPE4)
      #define __CTS_B5t__ defined(PTBDD_PTBDD5) && defined(PTBPE_PTBPE5)
      #define __CTS_B6t__ defined(PTBDD_PTBDD6) && defined(PTBPE_PTBPE6)
      #define __CTS_B7t__ defined(PTBDD_PTBDD7) && defined(PTBPE_PTBPE7)
  //C
      #define __CTS_C_p__ 0
      #define __CTS_C0t__ defined(PTCDD_PTCDD0) && defined(PTCPE_PTCPE0)
      #define __CTS_C1t__ defined(PTCDD_PTCDD1) && defined(PTCPE_PTCPE1)
      #define __CTS_C2t__ defined(PTCDD_PTCDD2) && defined(PTCPE_PTCPE2)
      #define __CTS_C3t__ defined(PTCDD_PTCDD3) && defined(PTCPE_PTCPE3)
      #define __CTS_C4t__ defined(PTCDD_PTCDD4) && defined(PTCPE_PTCPE4)
      #define __CTS_C5t__ defined(PTCDD_PTCDD5) && defined(PTCPE_PTCPE5)
      #define __CTS_C6t__ defined(PTCDD_PTCDD6) && defined(PTCPE_PTCPE6)
      #define __CTS_C7t__ defined(PTCDD_PTCDD7) && defined(PTCPE_PTCPE7)
  //D
      #define __CTS_D_p__ 0
      #define __CTS_D0t__ defined(PTDDD_PTDDD0) && defined(PTDPE_PTDPE0)
      #define __CTS_D1t__ defined(PTDDD_PTDDD1) && defined(PTDPE_PTDPE1)
      #define __CTS_D2t__ defined(PTDDD_PTDDD2) && defined(PTDPE_PTDPE2)
      #define __CTS_D3t__ defined(PTDDD_PTDDD3) && defined(PTDPE_PTDPE3)
      #define __CTS_D4t__ defined(PTDDD_PTDDD4) && defined(PTDPE_PTDPE4)
      #define __CTS_D5t__ defined(PTDDD_PTDDD5) && defined(PTDPE_PTDPE5)
      #define __CTS_D6t__ defined(PTDDD_PTDDD6) && defined(PTDPE_PTDPE6)
      #define __CTS_D7t__ defined(PTDDD_PTDDD7) && defined(PTDPE_PTDPE7)
  //E
      #define __CTS_E_p__ 0
      #define __CTS_E0t__ defined(PTEDD_PTEDD0) && defined(PTEPE_PTEPE0)
      #define __CTS_E1t__ defined(PTEDD_PTEDD1) && defined(PTEPE_PTEPE1)
      #define __CTS_E2t__ defined(PTEDD_PTEDD2) && defined(PTEPE_PTEPE2)
      #define __CTS_E3t__ defined(PTEDD_PTEDD3) && defined(PTEPE_PTEPE3)
      #define __CTS_E4t__ defined(PTEDD_PTEDD4) && defined(PTEPE_PTEPE4)
      #define __CTS_E5t__ defined(PTEDD_PTEDD5) && defined(PTEPE_PTEPE5)
      #define __CTS_E6t__ defined(PTEDD_PTEDD6) && defined(PTEPE_PTEPE6)
      #define __CTS_E7t__ defined(PTEDD_PTEDD7) && defined(PTEPE_PTEPE7)
  //F
      #define __CTS_F_p__ 0
      #define __CTS_F0t__ defined(PTFDD_PTFDD0) && defined(PTFPE_PTFPE0)
      #define __CTS_F1t__ defined(PTFDD_PTFDD1) && defined(PTFPE_PTFPE1)
      #define __CTS_F2t__ defined(PTFDD_PTFDD2) && defined(PTFPE_PTFPE2)
      #define __CTS_F3t__ defined(PTFDD_PTFDD3) && defined(PTFPE_PTFPE3)
      #define __CTS_F4t__ defined(PTFDD_PTFDD4) && defined(PTFPE_PTFPE4)
      #define __CTS_F5t__ defined(PTFDD_PTFDD5) && defined(PTFPE_PTFPE5)
      #define __CTS_F6t__ defined(PTFDD_PTFDD6) && defined(PTFPE_PTFPE6)
      #define __CTS_F7t__ defined(PTFDD_PTFDD7) && defined(PTFPE_PTFPE7)
  //G
      #define __CTS_G_p__ 0
      #define __CTS_G0t__ defined(PTGDD_PTGDD0) && defined(PTGPE_PTGPE0)
      #define __CTS_G1t__ defined(PTGDD_PTGDD1) && defined(PTGPE_PTGPE1)
      #define __CTS_G2t__ defined(PTGDD_PTGDD2) && defined(PTGPE_PTGPE2)
      #define __CTS_G3t__ defined(PTGDD_PTGDD3) && defined(PTGPE_PTGPE3)
      #define __CTS_G4t__ defined(PTGDD_PTGDD4) && defined(PTGPE_PTGPE4)
      #define __CTS_G5t__ defined(PTGDD_PTGDD5) && defined(PTGPE_PTGPE5)
      #define __CTS_G6t__ defined(PTGDD_PTGDD6) && defined(PTGPE_PTGPE6)
      #define __CTS_G7t__ defined(PTGDD_PTGDD7) && defined(PTGPE_PTGPE7)
  //H
      #define __CTS_H_p__ 0
      #define __CTS_H0t__ defined(PTHDD_PTHDD0) && defined(PTHPE_PTHPE0)
      #define __CTS_H1t__ defined(PTHDD_PTHDD1) && defined(PTHPE_PTHPE1)
      #define __CTS_H2t__ defined(PTHDD_PTHDD2) && defined(PTHPE_PTHPE2)
      #define __CTS_H3t__ defined(PTHDD_PTHDD3) && defined(PTHPE_PTHPE3)
      #define __CTS_H4t__ defined(PTHDD_PTHDD4) && defined(PTHPE_PTHPE4)
      #define __CTS_H5t__ defined(PTHDD_PTHDD5) && defined(PTHPE_PTHPE5)
      #define __CTS_H6t__ defined(PTHDD_PTHDD6) && defined(PTHPE_PTHPE6)
      #define __CTS_H7t__ defined(PTHDD_PTHDD7) && defined(PTHPE_PTHPE7)
  //J
      #define __CTS_I_p__ 0
      #define __CTS_I0t__ defined(PTIDD_PTIDD0) && defined(PTIPE_PTIPE0)
      #define __CTS_I1t__ defined(PTIDD_PTIDD1) && defined(PTIPE_PTIPE1)
      #define __CTS_I2t__ defined(PTIDD_PTIDD2) && defined(PTIPE_PTIPE2)
      #define __CTS_I3t__ defined(PTIDD_PTIDD3) && defined(PTIPE_PTIPE3)
      #define __CTS_I4t__ defined(PTIDD_PTIDD4) && defined(PTIPE_PTIPE4)
      #define __CTS_I5t__ defined(PTIDD_PTIDD5) && defined(PTIPE_PTIPE5)
      #define __CTS_I6t__ defined(PTIDD_PTIDD6) && defined(PTIPE_PTIPE6)
      #define __CTS_I7t__ defined(PTIDD_PTIDD7) && defined(PTIPE_PTIPE7)
  //J
      #define __CTS_J_p__ 0
      #define __CTS_J0t__ defined(PTJDD_PTJDD0) && defined(PTJPE_PTJPE0)
      #define __CTS_J1t__ defined(PTJDD_PTJDD1) && defined(PTJPE_PTJPE1)
      #define __CTS_J2t__ defined(PTJDD_PTJDD2) && defined(PTJPE_PTJPE2)
      #define __CTS_J3t__ defined(PTJDD_PTJDD3) && defined(PTJPE_PTJPE3)
      #define __CTS_J4t__ defined(PTJDD_PTJDD4) && defined(PTJPE_PTJPE4)
      #define __CTS_J5t__ defined(PTJDD_PTJDD5) && defined(PTJPE_PTJPE5)
      #define __CTS_J6t__ defined(PTJDD_PTJDD6) && defined(PTJPE_PTJPE6)
      #define __CTS_J7t__ defined(PTJDD_PTJDD7) && defined(PTJPE_PTJPE7)
  //K
      #define __CTS_K_p__ 0
      #define __CTS_K0t__ defined(PTKDD_PTKDD0) && defined(PTKPE_PTKPE0)
      #define __CTS_K1t__ defined(PTKDD_PTKDD1) && defined(PTKPE_PTKPE1)
      #define __CTS_K2t__ defined(PTKDD_PTKDD2) && defined(PTKPE_PTKPE2)
      #define __CTS_K3t__ defined(PTKDD_PTKDD3) && defined(PTKPE_PTKPE3)
      #define __CTS_K4t__ defined(PTKDD_PTKDD4) && defined(PTKPE_PTKPE4)
      #define __CTS_K5t__ defined(PTKDD_PTKDD5) && defined(PTKPE_PTKPE5)
      #define __CTS_K6t__ defined(PTKDD_PTKDD6) && defined(PTKPE_PTKPE6)
      #define __CTS_K7t__ defined(PTKDD_PTKDD7) && defined(PTKPE_PTKPE7)
  //L
      #define __CTS_L_p__ 0
      #define __CTS_L0t__ defined(PTLDD_PTLDD0) && defined(PTLPE_PTLPE0)
      #define __CTS_L1t__ defined(PTLDD_PTLDD1) && defined(PTLPE_PTLPE1)
      #define __CTS_L2t__ defined(PTLDD_PTLDD2) && defined(PTLPE_PTLPE2)
      #define __CTS_L3t__ defined(PTLDD_PTLDD3) && defined(PTLPE_PTLPE3)
      #define __CTS_L4t__ defined(PTLDD_PTLDD4) && defined(PTLPE_PTLPE4)
      #define __CTS_L5t__ defined(PTLDD_PTLDD5) && defined(PTLPE_PTLPE5)
      #define __CTS_L6t__ defined(PTLDD_PTLDD6) && defined(PTLPE_PTLPE6)
      #define __CTS_L7t__ defined(PTLDD_PTLDD7) && defined(PTLPE_PTLPE7)
  #endif /*S08,RS08,MCF1*/


  #if defined(__M08C__)
      //C08[A-E]
      #define __CTS_A_p__ 0
      #define __CTS_A0t__ defined(DDRA_DDRA0) && defined(PTAPUE_PTAPUE1)
      #define __CTS_A1t__ defined(DDRA_DDRA1) && defined(PTAPUE_PTAPUE1)
      #define __CTS_A2t__ defined(DDRA_DDRA2) && defined(PTAPUE_PTAPUE2)
      #define __CTS_A3t__ defined(DDRA_DDRA3) && defined(PTAPUE_PTAPUE3)
      #define __CTS_A4t__ defined(DDRA_DDRA4) && defined(PTAPUE_PTAPUE4)
      #define __CTS_A5t__ defined(DDRA_DDRA5) && defined(PTAPUE_PTAPUE5)
      #define __CTS_A6t__ defined(DDRA_DDRA6) && defined(PTAPUE_PTAPUE6)
      #define __CTS_A7t__ defined(DDRA_DDRA7) && defined(PTAPUE_PTAPUE7)
      
      #define __CTS_B_p__ 0
      #define __CTS_B0t__ defined(DDRB_DDRB0) && defined(PTBPUE_PTBPUE0)
      #define __CTS_B1t__ defined(DDRB_DDRB1) && defined(PTBPUE_PTBPUE1)
      #define __CTS_B2t__ defined(DDRB_DDRB2) && defined(PTBPUE_PTBPUE2)
      #define __CTS_B3t__ defined(DDRB_DDRB3) && defined(PTBPUE_PTBPUE3)
      #define __CTS_B4t__ defined(DDRB_DDRB4) && defined(PTBPUE_PTBPUE4)
      #define __CTS_B5t__ defined(DDRB_DDRB5) && defined(PTBPUE_PTBPUE5)
      #define __CTS_B6t__ defined(DDRB_DDRB6) && defined(PTBPUE_PTBPUE6)
      #define __CTS_B7t__ defined(DDRB_DDRB7) && defined(PTBPUE_PTBPUE7)

      #define __CTS_C_p__ 0
      #define __CTS_C0t__ defined(DDRC_DDRC0) && defined(PTCPUE_PTCPUE0)
      #define __CTS_C1t__ defined(DDRC_DDRC1) && defined(PTCPUE_PTCPUE1)
      #define __CTS_C2t__ defined(DDRC_DDRC2) && defined(PTCPUE_PTCPUE2)
      #define __CTS_C3t__ defined(DDRC_DDRC3) && defined(PTCPUE_PTCPUE3)
      #define __CTS_C4t__ defined(DDRC_DDRC4) && defined(PTCPUE_PTCPUE4)
      #define __CTS_C5t__ defined(DDRC_DDRC5) && defined(PTCPUE_PTCPUE5)
      #define __CTS_C6t__ defined(DDRC_DDRC6) && defined(PTCPUE_PTCPUE6)
      #define __CTS_C7t__ defined(DDRC_DDRC7) && defined(PTCPUE_PTCPUE7)

      #define __CTS_D_p__ 0
      #define __CTS_D0t__ defined(DDRD_DDRD0) && defined(PTDPUE_PTDPUE0)
      #define __CTS_D1t__ defined(DDRD_DDRD1) && defined(PTDPUE_PTDPUE1)
      #define __CTS_D2t__ defined(DDRD_DDRD2) && defined(PTDPUE_PTDPUE2)
      #define __CTS_D3t__ defined(DDRD_DDRD3) && defined(PTDPUE_PTDPUE3)
      #define __CTS_D4t__ defined(DDRD_DDRD4) && defined(PTDPUE_PTDPUE4)
      #define __CTS_D5t__ defined(DDRD_DDRD5) && defined(PTDPUE_PTDPUE5)
      #define __CTS_D6t__ defined(DDRD_DDRD6) && defined(PTDPUE_PTDPUE6)
      #define __CTS_D7t__ defined(DDRD_DDRD7) && defined(PTDPUE_PTDPUE7)

      #define __CTS_E_p__ 0
      #define __CTS_E0t__ defined(DDRE_DDRE0) && defined(PTEPUE_PTEPUE0)
      #define __CTS_E1t__ defined(DDRE_DDRE1) && defined(PTEPUE_PTEPUE1)
      #define __CTS_E2t__ defined(DDRE_DDRE2) && defined(PTEPUE_PTEPUE2)
      #define __CTS_E3t__ defined(DDRE_DDRE3) && defined(PTEPUE_PTEPUE3)
      #define __CTS_E4t__ defined(DDRE_DDRE4) && defined(PTEPUE_PTEPUE4)
      #define __CTS_E5t__ defined(DDRE_DDRE5) && defined(PTEPUE_PTEPUE5)
      #define __CTS_E6t__ defined(DDRE_DDRE6) && defined(PTEPUE_PTEPUE6)
      #define __CTS_E7t__ defined(DDRE_DDRE7) && defined(PTEPUE_PTEPUE7)
  #endif /*C08*/


  #if defined(__M12__) || defined(__MXG__)
  //M12(MXG)[]
  //PE-global
  //A:0
      #define __CTS_A_p__ 0
      #define __CTS_A0t__ defined(DDRA_DDRA0) && defined(PUCR_PUPAE)
      #define __CTS_A1t__ defined(DDRA_DDRA1) && defined(PUCR_PUPAE)
      #define __CTS_A2t__ defined(DDRA_DDRA2) && defined(PUCR_PUPAE)
      #define __CTS_A3t__ defined(DDRA_DDRA3) && defined(PUCR_PUPAE)
      #define __CTS_A4t__ defined(DDRA_DDRA4) && defined(PUCR_PUPAE)
      #define __CTS_A5t__ defined(DDRA_DDRA5) && defined(PUCR_PUPAE)
      #define __CTS_A6t__ defined(DDRA_DDRA6) && defined(PUCR_PUPAE)
      #define __CTS_A7t__ defined(DDRA_DDRA7) && defined(PUCR_PUPAE)
  //B:1
      #define __CTS_B_p__ 0
      #define __CTS_B0t__ defined(DDRB_DDRB0) && defined(PUCR_PUPBE)
      #define __CTS_B1t__ defined(DDRB_DDRB1) && defined(PUCR_PUPBE)
      #define __CTS_B2t__ defined(DDRB_DDRB2) && defined(PUCR_PUPBE)
      #define __CTS_B3t__ defined(DDRB_DDRB3) && defined(PUCR_PUPBE)
      #define __CTS_B4t__ defined(DDRB_DDRB4) && defined(PUCR_PUPBE)
      #define __CTS_B5t__ defined(DDRB_DDRB5) && defined(PUCR_PUPBE)
      #define __CTS_B6t__ defined(DDRB_DDRB6) && defined(PUCR_PUPBE)
      #define __CTS_B7t__ defined(DDRB_DDRB7) && defined(PUCR_PUPBE)
  //C:2
      #define __CTS_C_p__ 0
      #define __CTS_C0t__ defined(DDRC_DDRC0) && defined(PUCR_PUPCE)
      #define __CTS_C1t__ defined(DDRC_DDRC1) && defined(PUCR_PUPCE)
      #define __CTS_C2t__ defined(DDRC_DDRC2) && defined(PUCR_PUPCE)
      #define __CTS_C3t__ defined(DDRC_DDRC3) && defined(PUCR_PUPCE)
      #define __CTS_C4t__ defined(DDRC_DDRC4) && defined(PUCR_PUPCE)
      #define __CTS_C5t__ defined(DDRC_DDRC5) && defined(PUCR_PUPCE)
      #define __CTS_C6t__ defined(DDRC_DDRC6) && defined(PUCR_PUPCE)
      #define __CTS_C7t__ defined(DDRC_DDRC7) && defined(PUCR_PUPCE)
  //D:3
      #define __CTS_D_p__ 0
      #define __CTS_D0t__ defined(DDRD_DDRD0) && defined(PUCR_PUPDE)
      #define __CTS_D1t__ defined(DDRD_DDRD1) && defined(PUCR_PUPDE)
      #define __CTS_D2t__ defined(DDRD_DDRD2) && defined(PUCR_PUPDE)
      #define __CTS_D3t__ defined(DDRD_DDRD3) && defined(PUCR_PUPDE)
      #define __CTS_D4t__ defined(DDRD_DDRD4) && defined(PUCR_PUPDE)
      #define __CTS_D5t__ defined(DDRD_DDRD5) && defined(PUCR_PUPDE)
      #define __CTS_D6t__ defined(DDRD_DDRD6) && defined(PUCR_PUPDE)
      #define __CTS_D7t__ defined(DDRD_DDRD7) && defined(PUCR_PUPDE)
  //E:4
      #define __CTS_E_p__ 0
      #define __CTS_E0t__ defined(DDRE_DDRE0) && defined(PUCR_PUPEE)
      #define __CTS_E1t__ defined(DDRE_DDRE1) && defined(PUCR_PUPEE)
      #define __CTS_E2t__ defined(DDRE_DDRE2) && defined(PUCR_PUPEE)
      #define __CTS_E3t__ defined(DDRE_DDRE3) && defined(PUCR_PUPEE)
      #define __CTS_E4t__ defined(DDRE_DDRE4) && defined(PUCR_PUPEE)
      #define __CTS_E5t__ defined(DDRE_DDRE5) && defined(PUCR_PUPEE)
      #define __CTS_E6t__ defined(DDRE_DDRE6) && defined(PUCR_PUPEE)
      #define __CTS_E7t__ defined(DDRE_DDRE7) && defined(PUCR_PUPEE)
  //K:7
      #define __CTS_K_p__ 0
      #define __CTS_K0t__ defined(DDRK_DDRK0) && defined(PUCR_PUPKE)
      #define __CTS_K1t__ defined(DDRK_DDRK1) && defined(PUCR_PUPKE)
      #define __CTS_K2t__ defined(DDRK_DDRK2) && defined(PUCR_PUPKE)
      #define __CTS_K3t__ defined(DDRK_DDRK3) && defined(PUCR_PUPKE)
      #define __CTS_K4t__ defined(DDRK_DDRK4) && defined(PUCR_PUPKE)
      #define __CTS_K5t__ defined(DDRK_DDRK5) && defined(PUCR_PUPKE)
      #define __CTS_K6t__ defined(DDRK_DDRK6) && defined(PUCR_PUPKE)
      #define __CTS_K7t__ defined(DDRK_DDRK7) && defined(PUCR_PUPKE)

  #if !defined(__M12C__) 
  //H,J,L,M,P,Q,R,S,T,U,V,W
  //H
      #define __CTS_H_p__ defined(PPSH)
      #define __CTS_H0t__ defined(DDRH_DDRH0) && defined(PERH_PERH0)
      #define __CTS_H1t__ defined(DDRH_DDRH1) && defined(PERH_PERH1)
      #define __CTS_H2t__ defined(DDRH_DDRH2) && defined(PERH_PERH2)
      #define __CTS_H3t__ defined(DDRH_DDRH3) && defined(PERH_PERH3)
      #define __CTS_H4t__ defined(DDRH_DDRH4) && defined(PERH_PERH4)
      #define __CTS_H5t__ defined(DDRH_DDRH5) && defined(PERH_PERH5)
      #define __CTS_H6t__ defined(DDRH_DDRH6) && defined(PERH_PERH6)
      #define __CTS_H7t__ defined(DDRH_DDRH7) && defined(PERH_PERH7)
  //J
      #define __CTS_J_p__ defined(PPSJ)
      #define __CTS_J0t__ defined(DDRJ_DDRJ0) && defined(PERJ_PERJ0)
      #define __CTS_J1t__ defined(DDRJ_DDRJ1) && defined(PERJ_PERJ1)
      #define __CTS_J2t__ defined(DDRJ_DDRJ2) && defined(PERJ_PERJ2)
      #define __CTS_J3t__ defined(DDRJ_DDRJ3) && defined(PERJ_PERJ3)
      #define __CTS_J4t__ defined(DDRJ_DDRJ4) && defined(PERJ_PERJ4)
      #define __CTS_J5t__ defined(DDRJ_DDRJ5) && defined(PERJ_PERJ5)
      #define __CTS_J6t__ defined(DDRJ_DDRJ6) && defined(PERJ_PERJ6)
      #define __CTS_J7t__ defined(DDRJ_DDRJ7) && defined(PERJ_PERJ7)
  //L
      #define __CTS_L_p__ defined(PPSL)
      #define __CTS_L0t__ defined(DDRL_DDRL0) && defined(PERL_PERL0)
      #define __CTS_L1t__ defined(DDRL_DDRL1) && defined(PERL_PERL1)
      #define __CTS_L2t__ defined(DDRL_DDRL2) && defined(PERL_PERL2)
      #define __CTS_L3t__ defined(DDRL_DDRL3) && defined(PERL_PERL3)
      #define __CTS_L4t__ defined(DDRL_DDRL4) && defined(PERL_PERL4)
      #define __CTS_L5t__ defined(DDRL_DDRL5) && defined(PERL_PERL5)
      #define __CTS_L6t__ defined(DDRL_DDRL6) && defined(PERL_PERL6)
      #define __CTS_L7t__ defined(DDRL_DDRL7) && defined(PERL_PERL7)
  //M
      #define __CTS_M_p__ defined(PPSM)
      #define __CTS_M0t__ defined(DDRM_DDRM0) && defined(PERM_PERM0)
      #define __CTS_M1t__ defined(DDRM_DDRM1) && defined(PERM_PERM1)
      #define __CTS_M2t__ defined(DDRM_DDRM2) && defined(PERM_PERM2)
      #define __CTS_M3t__ defined(DDRM_DDRM3) && defined(PERM_PERM3)
      #define __CTS_M4t__ defined(DDRM_DDRM4) && defined(PERM_PERM4)
      #define __CTS_M5t__ defined(DDRM_DDRM5) && defined(PERM_PERM5)
      #define __CTS_M6t__ defined(DDRM_DDRM6) && defined(PERM_PERM6)
      #define __CTS_M7t__ defined(DDRM_DDRM7) && defined(PERM_PERM7)
  //P
      #define __CTS_P_p__ defined(PPSP)
      #define __CTS_P0t__ defined(DDRP_DDRP0) && defined(PERP_PERP0)
      #define __CTS_P1t__ defined(DDRP_DDRP1) && defined(PERP_PERP1)
      #define __CTS_P2t__ defined(DDRP_DDRP2) && defined(PERP_PERP2)
      #define __CTS_P3t__ defined(DDRP_DDRP3) && defined(PERP_PERP3)
      #define __CTS_P4t__ defined(DDRP_DDRP4) && defined(PERP_PERP4)
      #define __CTS_P5t__ defined(DDRP_DDRP5) && defined(PERP_PERP5)
      #define __CTS_P6t__ defined(DDRP_DDRP6) && defined(PERP_PERP6)
      #define __CTS_P7t__ defined(DDRP_DDRP7) && defined(PERP_PERP7)
  //Q
      #define __CTS_Q_p__ defined(PPSQ)
      #define __CTS_Q0t__ defined(DDRQ_DDRQ0) && defined(PERQ_PERQ0)
      #define __CTS_Q1t__ defined(DDRQ_DDRQ1) && defined(PERQ_PERQ1)
      #define __CTS_Q2t__ defined(DDRQ_DDRQ2) && defined(PERQ_PERQ2)
      #define __CTS_Q3t__ defined(DDRQ_DDRQ3) && defined(PERQ_PERQ3)
      #define __CTS_Q4t__ defined(DDRQ_DDRQ4) && defined(PERQ_PERQ4)
      #define __CTS_Q5t__ defined(DDRQ_DDRQ5) && defined(PERQ_PERQ5)
      #define __CTS_Q6t__ defined(DDRQ_DDRQ6) && defined(PERQ_PERQ6)
      #define __CTS_Q7t__ defined(DDRQ_DDRQ7) && defined(PERQ_PERQ7)
  //R
      #define __CTS_R_p__ defined(PPSR)
      #define __CTS_R0t__ defined(DDRR_DDRR0) && defined(PERR_PERR0)
      #define __CTS_R1t__ defined(DDRR_DDRR1) && defined(PERR_PERR1)
      #define __CTS_R2t__ defined(DDRR_DDRR2) && defined(PERR_PERR2)
      #define __CTS_R3t__ defined(DDRR_DDRR3) && defined(PERR_PERR3)
      #define __CTS_R4t__ defined(DDRR_DDRR4) && defined(PERR_PERR4)
      #define __CTS_R5t__ defined(DDRR_DDRR5) && defined(PERR_PERR5)
      #define __CTS_R6t__ defined(DDRR_DDRR6) && defined(PERR_PERR6)
      #define __CTS_R7t__ defined(DDRR_DDRR7) && defined(PERR_PERR7)
  //S
      #define __CTS_S_p__ defined(PPSS)
      #define __CTS_S0t__ defined(DDRS_DDRS0) && defined(PERS_PERS0)
      #define __CTS_S1t__ defined(DDRS_DDRS1) && defined(PERS_PERS1)
      #define __CTS_S2t__ defined(DDRS_DDRS2) && defined(PERS_PERS2)
      #define __CTS_S3t__ defined(DDRS_DDRS3) && defined(PERS_PERS3)
      #define __CTS_S4t__ defined(DDRS_DDRS4) && defined(PERS_PERS4)
      #define __CTS_S5t__ defined(DDRS_DDRS5) && defined(PERS_PERS5)
      #define __CTS_S6t__ defined(DDRS_DDRS6) && defined(PERS_PERS6)
      #define __CTS_S7t__ defined(DDRS_DDRS7) && defined(PERS_PERS7)
  //T
      #define __CTS_T_p__ defined(PPST)
      #define __CTS_T0t__ defined(DDRT_DDRT0) && defined(PERT_PERT0)
      #define __CTS_T1t__ defined(DDRT_DDRT1) && defined(PERT_PERT1)
      #define __CTS_T2t__ defined(DDRT_DDRT2) && defined(PERT_PERT2)
      #define __CTS_T3t__ defined(DDRT_DDRT3) && defined(PERT_PERT3)
      #define __CTS_T4t__ defined(DDRT_DDRT4) && defined(PERT_PERT4)
      #define __CTS_T5t__ defined(DDRT_DDRT5) && defined(PERT_PERT5)
      #define __CTS_T6t__ defined(DDRT_DDRT6) && defined(PERT_PERT6)
      #define __CTS_T7t__ defined(DDRT_DDRT7) && defined(PERT_PERT7)
  //U
      #define __CTS_U_p__ defined(PPSU)
      #define __CTS_U0t__ defined(DDRU_DDRU0) && defined(PERU_PERU0)
      #define __CTS_U1t__ defined(DDRU_DDRU1) && defined(PERU_PERU1)
      #define __CTS_U2t__ defined(DDRU_DDRU2) && defined(PERU_PERU2)
      #define __CTS_U3t__ defined(DDRU_DDRU3) && defined(PERU_PERU3)
      #define __CTS_U4t__ defined(DDRU_DDRU4) && defined(PERU_PERU4)
      #define __CTS_U5t__ defined(DDRU_DDRU5) && defined(PERU_PERU5)
      #define __CTS_U6t__ defined(DDRU_DDRU6) && defined(PERU_PERU6)
      #define __CTS_U7t__ defined(DDRU_DDRU7) && defined(PERU_PERU7)
  //V
      #define __CTS_V_p__ defined(PPSV)
      #define __CTS_V0t__ defined(DDRV_DDRV0) && defined(PERV_PERV0)
      #define __CTS_V1t__ defined(DDRV_DDRV1) && defined(PERV_PERV1)
      #define __CTS_V2t__ defined(DDRV_DDRV2) && defined(PERV_PERV2)
      #define __CTS_V3t__ defined(DDRV_DDRV3) && defined(PERV_PERV3)
      #define __CTS_V4t__ defined(DDRV_DDRV4) && defined(PERV_PERV4)
      #define __CTS_V5t__ defined(DDRV_DDRV5) && defined(PERV_PERV5)
      #define __CTS_V6t__ defined(DDRV_DDRV6) && defined(PERV_PERV6)
      #define __CTS_V7t__ defined(DDRV_DDRV7) && defined(PERV_PERV7)
  //W
      #define __CTS_W_p__ defined(PPSW)
      #define __CTS_W0t__ defined(DDRW_DDRW0) && defined(PERW_PERW0)
      #define __CTS_W1t__ defined(DDRW_DDRW1) && defined(PERW_PERW1)
      #define __CTS_W2t__ defined(DDRW_DDRW2) && defined(PERW_PERW2)
      #define __CTS_W3t__ defined(DDRW_DDRW3) && defined(PERW_PERW3)
      #define __CTS_W4t__ defined(DDRW_DDRW4) && defined(PERW_PERW4)
      #define __CTS_W5t__ defined(DDRW_DDRW5) && defined(PERW_PERW5)
      #define __CTS_W6t__ defined(DDRW_DDRW6) && defined(PERW_PERW6)
      #define __CTS_W7t__ defined(DDRW_DDRW7) && defined(PERW_PERW7)

  #endif /*!C12*/

  #endif /*M12,XGT*/

  /*******************************************************************************
  ****    CTS platform - pull-up/-down 1x polarity && 8x test redefines
  *******************************************************************************/

  //_A
  #ifndef             __CTS_A_p__
      #define         __CTS_A_p__ 0
  #endif
  #ifndef             __CTS_A0t__
      #define         __CTS_A0t__ 0
  #endif
  #ifndef             __CTS_A1t__
      #define         __CTS_A1t__ 0
  #endif
  #ifndef             __CTS_A2t__
      #define         __CTS_A2t__ 0
  #endif
  #ifndef             __CTS_A3t__
      #define         __CTS_A3t__ 0
  #endif
  #ifndef             __CTS_A4t__
      #define         __CTS_A4t__ 0
  #endif
  #ifndef             __CTS_A5t__
      #define         __CTS_A5t__ 0
  #endif
  #ifndef             __CTS_A6t__
      #define         __CTS_A6t__ 0
  #endif
  #ifndef             __CTS_A7t__
      #define         __CTS_A7t__ 0
  #endif
  //_B
  #ifndef             __CTS_B_p__
      #define         __CTS_B_p__ 0
  #endif
  #ifndef             __CTS_B0t__
      #define         __CTS_B0t__ 0
  #endif
  #ifndef             __CTS_B1t__
      #define         __CTS_B1t__ 0
  #endif
  #ifndef             __CTS_B2t__
      #define         __CTS_B2t__ 0
  #endif
  #ifndef             __CTS_B3t__
      #define         __CTS_B3t__ 0
  #endif
  #ifndef             __CTS_B4t__
      #define         __CTS_B4t__ 0
  #endif
  #ifndef             __CTS_B5t__
      #define         __CTS_B5t__ 0
  #endif
  #ifndef             __CTS_B6t__
      #define         __CTS_B6t__ 0
  #endif
  #ifndef             __CTS_B7t__
      #define         __CTS_B7t__ 0
  #endif
  //_C
  #ifndef             __CTS_C_p__
      #define         __CTS_C_p__ 0
  #endif
  #ifndef             __CTS_C0t__
      #define         __CTS_C0t__ 0
  #endif
  #ifndef             __CTS_C1t__
      #define         __CTS_C1t__ 0
  #endif
  #ifndef             __CTS_C2t__
      #define         __CTS_C2t__ 0
  #endif
  #ifndef             __CTS_C3t__
      #define         __CTS_C3t__ 0
  #endif
  #ifndef             __CTS_C4t__
      #define         __CTS_C4t__ 0
  #endif
  #ifndef             __CTS_C5t__
      #define         __CTS_C5t__ 0
  #endif
  #ifndef             __CTS_C6t__
      #define         __CTS_C6t__ 0
  #endif
  #ifndef             __CTS_C7t__
      #define         __CTS_C7t__ 0
  #endif
  //_D
  #ifndef             __CTS_D_p__
      #define         __CTS_D_p__ 0
  #endif
  #ifndef             __CTS_D0t__
      #define         __CTS_D0t__ 0
  #endif
  #ifndef             __CTS_D1t__
      #define         __CTS_D1t__ 0
  #endif
  #ifndef             __CTS_D2t__
      #define         __CTS_D2t__ 0
  #endif
  #ifndef             __CTS_D3t__
      #define         __CTS_D3t__ 0
  #endif
  #ifndef             __CTS_D4t__
      #define         __CTS_D4t__ 0
  #endif
  #ifndef             __CTS_D5t__
      #define         __CTS_D5t__ 0
  #endif
  #ifndef             __CTS_D6t__
      #define         __CTS_D6t__ 0
  #endif
  #ifndef             __CTS_D7t__
      #define         __CTS_D7t__ 0
  #endif
  //_E
  #ifndef             __CTS_E_p__
      #define         __CTS_E_p__ 0
  #endif
  #ifndef             __CTS_E0t__
      #define         __CTS_E0t__ 0
  #endif
  #ifndef             __CTS_E1t__
      #define         __CTS_E1t__ 0
  #endif
  #ifndef             __CTS_E2t__
      #define         __CTS_E2t__ 0
  #endif
  #ifndef             __CTS_E3t__
      #define         __CTS_E3t__ 0
  #endif
  #ifndef             __CTS_E4t__
      #define         __CTS_E4t__ 0
  #endif
  #ifndef             __CTS_E5t__
      #define         __CTS_E5t__ 0
  #endif
  #ifndef             __CTS_E6t__
      #define         __CTS_E6t__ 0
  #endif
  #ifndef             __CTS_E7t__
      #define         __CTS_E7t__ 0
  #endif
  //_F
  #ifndef             __CTS_F_p__
      #define         __CTS_F_p__ 0
  #endif
  #ifndef             __CTS_F0t__
      #define         __CTS_F0t__ 0
  #endif
  #ifndef             __CTS_F1t__
      #define         __CTS_F1t__ 0
  #endif
  #ifndef             __CTS_F2t__
      #define         __CTS_F2t__ 0
  #endif
  #ifndef             __CTS_F3t__
      #define         __CTS_F3t__ 0
  #endif
  #ifndef             __CTS_F4t__
      #define         __CTS_F4t__ 0
  #endif
  #ifndef             __CTS_F5t__
      #define         __CTS_F5t__ 0
  #endif
  #ifndef             __CTS_F6t__
      #define         __CTS_F6t__ 0
  #endif
  #ifndef             __CTS_F7t__
      #define         __CTS_F7t__ 0
  #endif
  //_G
  #ifndef             __CTS_G_p__
      #define         __CTS_G_p__ 0
  #endif
  #ifndef             __CTS_G0t__
      #define         __CTS_G0t__ 0
  #endif
  #ifndef             __CTS_G1t__
      #define         __CTS_G1t__ 0
  #endif
  #ifndef             __CTS_G2t__
      #define         __CTS_G2t__ 0
  #endif
  #ifndef             __CTS_G3t__
      #define         __CTS_G3t__ 0
  #endif
  #ifndef             __CTS_G4t__
      #define         __CTS_G4t__ 0
  #endif
  #ifndef             __CTS_G5t__
      #define         __CTS_G5t__ 0
  #endif
  #ifndef             __CTS_G6t__
      #define         __CTS_G6t__ 0
  #endif
  #ifndef             __CTS_G7t__
      #define         __CTS_G7t__ 0
  #endif
  //_H
  #ifndef             __CTS_H_p__
      #define         __CTS_H_p__ 0
  #endif
  #ifndef             __CTS_H0t__
      #define         __CTS_H0t__ 0
  #endif
  #ifndef             __CTS_H1t__
      #define         __CTS_H1t__ 0
  #endif
  #ifndef             __CTS_H2t__
      #define         __CTS_H2t__ 0
  #endif
  #ifndef             __CTS_H3t__
      #define         __CTS_H3t__ 0
  #endif
  #ifndef             __CTS_H4t__
      #define         __CTS_H4t__ 0
  #endif
  #ifndef             __CTS_H5t__
      #define         __CTS_H5t__ 0
  #endif
  #ifndef             __CTS_H6t__
      #define         __CTS_H6t__ 0
  #endif
  #ifndef             __CTS_H7t__
      #define         __CTS_H7t__ 0
  #endif
  //_I
  #ifndef             __CTS_I_p__
      #define         __CTS_I_p__ 0
  #endif
  #ifndef             __CTS_I0t__
      #define         __CTS_I0t__ 0
  #endif
  #ifndef             __CTS_I1t__
      #define         __CTS_I1t__ 0
  #endif
  #ifndef             __CTS_I2t__
      #define         __CTS_I2t__ 0
  #endif
  #ifndef             __CTS_I3t__
      #define         __CTS_I3t__ 0
  #endif
  #ifndef             __CTS_I4t__
      #define         __CTS_I4t__ 0
  #endif
  #ifndef             __CTS_I5t__
      #define         __CTS_I5t__ 0
  #endif
  #ifndef             __CTS_I6t__
      #define         __CTS_I6t__ 0
  #endif
  #ifndef             __CTS_I7t__
      #define         __CTS_I7t__ 0
  #endif
  //_J
  #ifndef             __CTS_J_p__
      #define         __CTS_J_p__ 0
  #endif
  #ifndef             __CTS_J0t__
      #define         __CTS_J0t__ 0
  #endif
  #ifndef             __CTS_J1t__
      #define         __CTS_J1t__ 0
  #endif
  #ifndef             __CTS_J2t__
      #define         __CTS_J2t__ 0
  #endif
  #ifndef             __CTS_J3t__
      #define         __CTS_J3t__ 0
  #endif
  #ifndef             __CTS_J4t__
      #define         __CTS_J4t__ 0
  #endif
  #ifndef             __CTS_J5t__
      #define         __CTS_J5t__ 0
  #endif
  #ifndef             __CTS_J6t__
      #define         __CTS_J6t__ 0
  #endif
  #ifndef             __CTS_J7t__
      #define         __CTS_J7t__ 0
  #endif
  //_K
  #ifndef             __CTS_K_p__
      #define         __CTS_K_p__ 0
  #endif
  #ifndef             __CTS_K0t__
      #define         __CTS_K0t__ 0
  #endif
  #ifndef             __CTS_K1t__
      #define         __CTS_K1t__ 0
  #endif
  #ifndef             __CTS_K2t__
      #define         __CTS_K2t__ 0
  #endif
  #ifndef             __CTS_K3t__
      #define         __CTS_K3t__ 0
  #endif
  #ifndef             __CTS_K4t__
      #define         __CTS_K4t__ 0
  #endif
  #ifndef             __CTS_K5t__
      #define         __CTS_K5t__ 0
  #endif
  #ifndef             __CTS_K6t__
      #define         __CTS_K6t__ 0
  #endif
  #ifndef             __CTS_K7t__
      #define         __CTS_K7t__ 0
  #endif
  //_L
  #ifndef             __CTS_L_p__
      #define         __CTS_L_p__ 0
  #endif
  #ifndef             __CTS_L0t__
      #define         __CTS_L0t__ 0
  #endif
  #ifndef             __CTS_L1t__
      #define         __CTS_L1t__ 0
  #endif
  #ifndef             __CTS_L2t__
      #define         __CTS_L2t__ 0
  #endif
  #ifndef             __CTS_L3t__
      #define         __CTS_L3t__ 0
  #endif
  #ifndef             __CTS_L4t__
      #define         __CTS_L4t__ 0
  #endif
  #ifndef             __CTS_L5t__
      #define         __CTS_L5t__ 0
  #endif
  #ifndef             __CTS_L6t__
      #define         __CTS_L6t__ 0
  #endif
  #ifndef             __CTS_L7t__
      #define         __CTS_L7t__ 0
  #endif
  //_M
  #ifndef             __CTS_M_p__
      #define         __CTS_M_p__ 0
  #endif
  #ifndef             __CTS_M0t__
      #define         __CTS_M0t__ 0
  #endif
  #ifndef             __CTS_M1t__
      #define         __CTS_M1t__ 0
  #endif
  #ifndef             __CTS_M2t__
      #define         __CTS_M2t__ 0
  #endif
  #ifndef             __CTS_M3t__
      #define         __CTS_M3t__ 0
  #endif
  #ifndef             __CTS_M4t__
      #define         __CTS_M4t__ 0
  #endif
  #ifndef             __CTS_M5t__
      #define         __CTS_M5t__ 0
  #endif
  #ifndef             __CTS_M6t__
      #define         __CTS_M6t__ 0
  #endif
  #ifndef             __CTS_M7t__
      #define         __CTS_M7t__ 0
  #endif
  //_N
  #ifndef             __CTS_N_p__
      #define         __CTS_N_p__ 0
  #endif
  #ifndef             __CTS_N0t__
      #define         __CTS_N0t__ 0
  #endif
  #ifndef             __CTS_N1t__
      #define         __CTS_N1t__ 0
  #endif
  #ifndef             __CTS_N2t__
      #define         __CTS_N2t__ 0
  #endif
  #ifndef             __CTS_N3t__
      #define         __CTS_N3t__ 0
  #endif
  #ifndef             __CTS_N4t__
      #define         __CTS_N4t__ 0
  #endif
  #ifndef             __CTS_N5t__
      #define         __CTS_N5t__ 0
  #endif
  #ifndef             __CTS_N6t__
      #define         __CTS_N6t__ 0
  #endif
  #ifndef             __CTS_N7t__
      #define         __CTS_N7t__ 0
  #endif
  //_P
  #ifndef             __CTS_P_p__
      #define         __CTS_P_p__ 0
  #endif
  #ifndef             __CTS_P0t__
      #define         __CTS_P0t__ 0
  #endif
  #ifndef             __CTS_P1t__
      #define         __CTS_P1t__ 0
  #endif
  #ifndef             __CTS_P2t__
      #define         __CTS_P2t__ 0
  #endif
  #ifndef             __CTS_P3t__
      #define         __CTS_P3t__ 0
  #endif
  #ifndef             __CTS_P4t__
      #define         __CTS_P4t__ 0
  #endif
  #ifndef             __CTS_P5t__
      #define         __CTS_P5t__ 0
  #endif
  #ifndef             __CTS_P6t__
      #define         __CTS_P6t__ 0
  #endif
  #ifndef             __CTS_P7t__
      #define         __CTS_P7t__ 0
  #endif
  //_Q
  #ifndef             __CTS_Q_p__
      #define         __CTS_Q_p__ 0
  #endif
  #ifndef             __CTS_Q0t__
      #define         __CTS_Q0t__ 0
  #endif
  #ifndef             __CTS_Q1t__
      #define         __CTS_Q1t__ 0
  #endif
  #ifndef             __CTS_Q2t__
      #define         __CTS_Q2t__ 0
  #endif
  #ifndef             __CTS_Q3t__
      #define         __CTS_Q3t__ 0
  #endif
  #ifndef             __CTS_Q4t__
      #define         __CTS_Q4t__ 0
  #endif
  #ifndef             __CTS_Q5t__
      #define         __CTS_Q5t__ 0
  #endif
  #ifndef             __CTS_Q6t__
      #define         __CTS_Q6t__ 0
  #endif
  #ifndef             __CTS_Q7t__
      #define         __CTS_Q7t__ 0
  #endif
  //_R
  #ifndef             __CTS_R_p__
      #define         __CTS_R_p__ 0
  #endif
  #ifndef             __CTS_R0t__
      #define         __CTS_R0t__ 0
  #endif
  #ifndef             __CTS_R1t__
      #define         __CTS_R1t__ 0
  #endif
  #ifndef             __CTS_R2t__
      #define         __CTS_R2t__ 0
  #endif
  #ifndef             __CTS_R3t__
      #define         __CTS_R3t__ 0
  #endif
  #ifndef             __CTS_R4t__
      #define         __CTS_R4t__ 0
  #endif
  #ifndef             __CTS_R5t__
      #define         __CTS_R5t__ 0
  #endif
  #ifndef             __CTS_R6t__
      #define         __CTS_R6t__ 0
  #endif
  #ifndef             __CTS_R7t__
      #define         __CTS_R7t__ 0
  #endif
  //_S
  #ifndef             __CTS_S_p__
      #define         __CTS_S_p__ 0
  #endif
  #ifndef             __CTS_S0t__
      #define         __CTS_S0t__ 0
  #endif
  #ifndef             __CTS_S1t__
      #define         __CTS_S1t__ 0
  #endif
  #ifndef             __CTS_S2t__
      #define         __CTS_S2t__ 0
  #endif
  #ifndef             __CTS_S3t__
      #define         __CTS_S3t__ 0
  #endif
  #ifndef             __CTS_S4t__
      #define         __CTS_S4t__ 0
  #endif
  #ifndef             __CTS_S5t__
      #define         __CTS_S5t__ 0
  #endif
  #ifndef             __CTS_S6t__
      #define         __CTS_S6t__ 0
  #endif
  #ifndef             __CTS_S7t__
      #define         __CTS_S7t__ 0
  #endif
  //_T
  #ifndef             __CTS_T_p__
      #define         __CTS_T_p__ 0
  #endif
  #ifndef             __CTS_T0t__
      #define         __CTS_T0t__ 0
  #endif
  #ifndef             __CTS_T1t__
      #define         __CTS_T1t__ 0
  #endif
  #ifndef             __CTS_T2t__
      #define         __CTS_T2t__ 0
  #endif
  #ifndef             __CTS_T3t__
      #define         __CTS_T3t__ 0
  #endif
  #ifndef             __CTS_T4t__
      #define         __CTS_T4t__ 0
  #endif
  #ifndef             __CTS_T5t__
      #define         __CTS_T5t__ 0
  #endif
  #ifndef             __CTS_T6t__
      #define         __CTS_T6t__ 0
  #endif
  #ifndef             __CTS_T7t__
      #define         __CTS_T7t__ 0
  #endif
  //_U
  #ifndef             __CTS_U_p__
      #define         __CTS_U_p__ 0
  #endif
  #ifndef             __CTS_U0t__
      #define         __CTS_U0t__ 0
  #endif
  #ifndef             __CTS_U1t__
      #define         __CTS_U1t__ 0
  #endif
  #ifndef             __CTS_U2t__
      #define         __CTS_U2t__ 0
  #endif
  #ifndef             __CTS_U3t__
      #define         __CTS_U3t__ 0
  #endif
  #ifndef             __CTS_U4t__
      #define         __CTS_U4t__ 0
  #endif
  #ifndef             __CTS_U5t__
      #define         __CTS_U5t__ 0
  #endif
  #ifndef             __CTS_U6t__
      #define         __CTS_U6t__ 0
  #endif
  #ifndef             __CTS_U7t__
      #define         __CTS_U7t__ 0
  #endif
  //_V
  #ifndef             __CTS_V_p__
      #define         __CTS_V_p__ 0
  #endif
  #ifndef             __CTS_V0t__
      #define         __CTS_V0t__ 0
  #endif
  #ifndef             __CTS_V1t__
      #define         __CTS_V1t__ 0
  #endif
  #ifndef             __CTS_V2t__
      #define         __CTS_V2t__ 0
  #endif
  #ifndef             __CTS_V3t__
      #define         __CTS_V3t__ 0
  #endif
  #ifndef             __CTS_V4t__
      #define         __CTS_V4t__ 0
  #endif
  #ifndef             __CTS_V5t__
      #define         __CTS_V5t__ 0
  #endif
  #ifndef             __CTS_V6t__
      #define         __CTS_V6t__ 0
  #endif
  #ifndef             __CTS_V7t__
      #define         __CTS_V7t__ 0
  #endif
  //_W
  #ifndef             __CTS_W_p__
      #define         __CTS_W_p__ 0
  #endif
  #ifndef             __CTS_W0t__
      #define         __CTS_W0t__ 0
  #endif
  #ifndef             __CTS_W1t__
      #define         __CTS_W1t__ 0
  #endif
  #ifndef             __CTS_W2t__
      #define         __CTS_W2t__ 0
  #endif
  #ifndef             __CTS_W3t__
      #define         __CTS_W3t__ 0
  #endif
  #ifndef             __CTS_W4t__
      #define         __CTS_W4t__ 0
  #endif
  #ifndef             __CTS_W5t__
      #define         __CTS_W5t__ 0
  #endif
  #ifndef             __CTS_W6t__
      #define         __CTS_W6t__ 0
  #endif
  #ifndef             __CTS_W7t__
      #define         __CTS_W7t__ 0
  #endif





  /*******************************************************************************
  ****    CTS platform - BLUE detection
  *******************************************************************************/
  //_A
  #if                 __CTS_A0t__
      #define         __CTS_A0H__
      #if             __CTS_A_p__
          #define     __CTS_A0L__
      #endif
  #endif
  #if                 __CTS_A1t__
      #define         __CTS_A1H__
      #if             __CTS_A_p__
          #define     __CTS_A1L__
      #endif
  #endif
  #if                 __CTS_A2t__
      #define         __CTS_A2H__
      #if             __CTS_A_p__
          #define     __CTS_A2L__
      #endif
  #endif
  #if                 __CTS_A3t__
      #define         __CTS_A3H__
      #if             __CTS_A_p__
          #define     __CTS_A3L__
      #endif
  #endif
  #if                 __CTS_A4t__
      #define         __CTS_A4H__
      #if             __CTS_A_p__
          #define     __CTS_A4L__
      #endif
  #endif
  #if                 __CTS_A5t__
      #define         __CTS_A5H__
      #if             __CTS_A_p__
          #define     __CTS_A5L__
      #endif
  #endif
  #if                 __CTS_A6t__
      #define         __CTS_A6H__
      #if             __CTS_A_p__
          #define     __CTS_A6L__
      #endif
  #endif
  #if                 __CTS_A7t__
      #define         __CTS_A7H__
      #if             __CTS_A_p__
          #define     __CTS_A7L__
      #endif
  #endif


  //_B
  #if                 __CTS_B0t__
      #define         __CTS_B0H__
      #if             __CTS_B_p__
          #define     __CTS_B0L__
      #endif
  #endif
  #if                 __CTS_B1t__
      #define         __CTS_B1H__
      #if             __CTS_B_p__
          #define     __CTS_B1L__
      #endif
  #endif
  #if                 __CTS_B2t__
      #define         __CTS_B2H__
      #if             __CTS_B_p__
          #define     __CTS_B2L__
      #endif
  #endif
  #if                 __CTS_B3t__
      #define         __CTS_B3H__
      #if             __CTS_B_p__
          #define     __CTS_B3L__
      #endif
  #endif
  #if                 __CTS_B4t__
      #define         __CTS_B4H__
      #if             __CTS_B_p__
          #define     __CTS_B4L__
      #endif
  #endif
  #if                 __CTS_B5t__
      #define         __CTS_B5H__
      #if             __CTS_B_p__
          #define     __CTS_B5L__
      #endif
  #endif
  #if                 __CTS_B6t__
      #define         __CTS_B6H__
      #if             __CTS_B_p__
          #define     __CTS_B6L__
      #endif
  #endif
  #if                 __CTS_B7t__
      #define         __CTS_B7H__
      #if             __CTS_B_p__
          #define     __CTS_B7L__
      #endif
  #endif


  //_C
  #if                 __CTS_C0t__
      #define         __CTS_C0H__
      #if             __CTS_C_p__
          #define     __CTS_C0L__
      #endif
  #endif
  #if                 __CTS_C1t__
      #define         __CTS_C1H__
      #if             __CTS_C_p__
          #define     __CTS_C1L__
      #endif
  #endif
  #if                 __CTS_C2t__
      #define         __CTS_C2H__
      #if             __CTS_C_p__
          #define     __CTS_C2L__
      #endif
  #endif
  #if                 __CTS_C3t__
      #define         __CTS_C3H__
      #if             __CTS_C_p__
          #define     __CTS_C3L__
      #endif
  #endif
  #if                 __CTS_C4t__
      #define         __CTS_C4H__
      #if             __CTS_C_p__
          #define     __CTS_C4L__
      #endif
  #endif
  #if                 __CTS_C5t__
      #define         __CTS_C5H__
      #if             __CTS_C_p__
          #define     __CTS_C5L__
      #endif
  #endif
  #if                 __CTS_C6t__
      #define         __CTS_C6H__
      #if             __CTS_C_p__
          #define     __CTS_C6L__
      #endif
  #endif
  #if                 __CTS_C7t__
      #define         __CTS_C7H__
      #if             __CTS_C_p__
          #define     __CTS_C7L__
      #endif
  #endif


  //_D
  #if                 __CTS_D0t__
      #define         __CTS_D0H__
      #if             __CTS_D_p__
          #define     __CTS_D0L__
      #endif
  #endif
  #if                 __CTS_D1t__
      #define         __CTS_D1H__
      #if             __CTS_D_p__
          #define     __CTS_D1L__
      #endif
  #endif
  #if                 __CTS_D2t__
      #define         __CTS_D2H__
      #if             __CTS_D_p__
          #define     __CTS_D2L__
      #endif
  #endif
  #if                 __CTS_D3t__
      #define         __CTS_D3H__
      #if             __CTS_D_p__
          #define     __CTS_D3L__
      #endif
  #endif
  #if                 __CTS_D4t__
      #define         __CTS_D4H__
      #if             __CTS_D_p__
          #define     __CTS_D4L__
      #endif
  #endif
  #if                 __CTS_D5t__
      #define         __CTS_D5H__
      #if             __CTS_D_p__
          #define     __CTS_D5L__
      #endif
  #endif
  #if                 __CTS_D6t__
      #define         __CTS_D6H__
      #if             __CTS_D_p__
          #define     __CTS_D6L__
      #endif
  #endif
  #if                 __CTS_D7t__
      #define         __CTS_D7H__
      #if             __CTS_D_p__
          #define     __CTS_D7L__
      #endif
  #endif


  //_E
  #if                 __CTS_E0t__
      #define         __CTS_E0H__
      #if             __CTS_E_p__
          #define     __CTS_E0L__
      #endif
  #endif
  #if                 __CTS_E1t__
      #define         __CTS_E1H__
      #if             __CTS_E_p__
          #define     __CTS_E1L__
      #endif
  #endif
  #if                 __CTS_E2t__
      #define         __CTS_E2H__
      #if             __CTS_E_p__
          #define     __CTS_E2L__
      #endif
  #endif
  #if                 __CTS_E3t__
      #define         __CTS_E3H__
      #if             __CTS_E_p__
          #define     __CTS_E3L__
      #endif
  #endif
  #if                 __CTS_E4t__
      #define         __CTS_E4H__
      #if             __CTS_E_p__
          #define     __CTS_E4L__
      #endif
  #endif
  #if                 __CTS_E5t__
      #define         __CTS_E5H__
      #if             __CTS_E_p__
          #define     __CTS_E5L__
      #endif
  #endif
  #if                 __CTS_E6t__
      #define         __CTS_E6H__
      #if             __CTS_E_p__
          #define     __CTS_E6L__
      #endif
  #endif
  #if                 __CTS_E7t__
      #define         __CTS_E7H__
      #if             __CTS_E_p__
          #define     __CTS_E7L__
      #endif
  #endif


  //_F
  #if                 __CTS_F0t__
      #define         __CTS_F0H__
      #if             __CTS_F_p__
          #define     __CTS_F0L__
      #endif
  #endif
  #if                 __CTS_F1t__
      #define         __CTS_F1H__
      #if             __CTS_F_p__
          #define     __CTS_F1L__
      #endif
  #endif
  #if                 __CTS_F2t__
      #define         __CTS_F2H__
      #if             __CTS_F_p__
          #define     __CTS_F2L__
      #endif
  #endif
  #if                 __CTS_F3t__
      #define         __CTS_F3H__
      #if             __CTS_F_p__
          #define     __CTS_F3L__
      #endif
  #endif
  #if                 __CTS_F4t__
      #define         __CTS_F4H__
      #if             __CTS_F_p__
          #define     __CTS_F4L__
      #endif
  #endif
  #if                 __CTS_F5t__
      #define         __CTS_F5H__
      #if             __CTS_F_p__
          #define     __CTS_F5L__
      #endif
  #endif
  #if                 __CTS_F6t__
      #define         __CTS_F6H__
      #if             __CTS_F_p__
          #define     __CTS_F6L__
      #endif
  #endif
  #if                 __CTS_F7t__
      #define         __CTS_F7H__
      #if             __CTS_F_p__
          #define     __CTS_F7L__
      #endif
  #endif


  //_G
  #if                 __CTS_G0t__
      #define         __CTS_G0H__
      #if             __CTS_G_p__
          #define     __CTS_G0L__
      #endif
  #endif
  #if                 __CTS_G1t__
      #define         __CTS_G1H__
      #if             __CTS_G_p__
          #define     __CTS_G1L__
      #endif
  #endif
  #if                 __CTS_G2t__
      #define         __CTS_G2H__
      #if             __CTS_G_p__
          #define     __CTS_G2L__
      #endif
  #endif
  #if                 __CTS_G3t__
      #define         __CTS_G3H__
      #if             __CTS_G_p__
          #define     __CTS_G3L__
      #endif
  #endif
  #if                 __CTS_G4t__
      #define         __CTS_G4H__
      #if             __CTS_G_p__
          #define     __CTS_G4L__
      #endif
  #endif
  #if                 __CTS_G5t__
      #define         __CTS_G5H__
      #if             __CTS_G_p__
          #define     __CTS_G5L__
      #endif
  #endif
  #if                 __CTS_G6t__
      #define         __CTS_G6H__
      #if             __CTS_G_p__
          #define     __CTS_G6L__
      #endif
  #endif
  #if                 __CTS_G7t__
      #define         __CTS_G7H__
      #if             __CTS_G_p__
          #define     __CTS_G7L__
      #endif
  #endif


  //_H
  #if                 __CTS_H0t__
      #define         __CTS_H0H__
      #if             __CTS_H_p__
          #define     __CTS_H0L__
      #endif
  #endif
  #if                 __CTS_H1t__
      #define         __CTS_H1H__
      #if             __CTS_H_p__
          #define     __CTS_H1L__
      #endif
  #endif
  #if                 __CTS_H2t__
      #define         __CTS_H2H__
      #if             __CTS_H_p__
          #define     __CTS_H2L__
      #endif
  #endif
  #if                 __CTS_H3t__
      #define         __CTS_H3H__
      #if             __CTS_H_p__
          #define     __CTS_H3L__
      #endif
  #endif
  #if                 __CTS_H4t__
      #define         __CTS_H4H__
      #if             __CTS_H_p__
          #define     __CTS_H4L__
      #endif
  #endif
  #if                 __CTS_H5t__
      #define         __CTS_H5H__
      #if             __CTS_H_p__
          #define     __CTS_H5L__
      #endif
  #endif
  #if                 __CTS_H6t__
      #define         __CTS_H6H__
      #if             __CTS_H_p__
          #define     __CTS_H6L__
      #endif
  #endif
  #if                 __CTS_H7t__
      #define         __CTS_H7H__
      #if             __CTS_H_p__
          #define     __CTS_H7L__
      #endif
  #endif

  //_I
  #if                 __CTS_I0t__
      #define         __CTS_I0H__
      #if             __CTS_I_p__
          #define     __CTS_I0L__
      #endif
  #endif
  #if                 __CTS_I1t__
      #define         __CTS_I1H__
      #if             __CTS_I_p__
          #define     __CTS_I1L__
      #endif
  #endif
  #if                 __CTS_I2t__
      #define         __CTS_I2H__
      #if             __CTS_I_p__
          #define     __CTS_I2L__
      #endif
  #endif
  #if                 __CTS_I3t__
      #define         __CTS_I3H__
      #if             __CTS_I_p__
          #define     __CTS_I3L__
      #endif
  #endif
  #if                 __CTS_I4t__
      #define         __CTS_I4H__
      #if             __CTS_I_p__
          #define     __CTS_I4L__
      #endif
  #endif
  #if                 __CTS_I5t__
      #define         __CTS_I5H__
      #if             __CTS_I_p__
          #define     __CTS_I5L__
      #endif
  #endif
  #if                 __CTS_I6t__
      #define         __CTS_I6H__
      #if             __CTS_I_p__
          #define     __CTS_I6L__
      #endif
  #endif
  #if                 __CTS_I7t__
      #define         __CTS_I7H__
      #if             __CTS_I_p__
          #define     __CTS_I7L__
      #endif
  #endif
  
  //_J
  #if                 __CTS_J0t__
      #define         __CTS_J0H__
      #if             __CTS_J_p__
          #define     __CTS_J0L__
      #endif
  #endif
  #if                 __CTS_J1t__
      #define         __CTS_J1H__
      #if             __CTS_J_p__
          #define     __CTS_J1L__
      #endif
  #endif
  #if                 __CTS_J2t__
      #define         __CTS_J2H__
      #if             __CTS_J_p__
          #define     __CTS_J2L__
      #endif
  #endif
  #if                 __CTS_J3t__
      #define         __CTS_J3H__
      #if             __CTS_J_p__
          #define     __CTS_J3L__
      #endif
  #endif
  #if                 __CTS_J4t__
      #define         __CTS_J4H__
      #if             __CTS_J_p__
          #define     __CTS_J4L__
      #endif
  #endif
  #if                 __CTS_J5t__
      #define         __CTS_J5H__
      #if             __CTS_J_p__
          #define     __CTS_J5L__
      #endif
  #endif
  #if                 __CTS_J6t__
      #define         __CTS_J6H__
      #if             __CTS_J_p__
          #define     __CTS_J6L__
      #endif
  #endif
  #if                 __CTS_J7t__
      #define         __CTS_J7H__
      #if             __CTS_J_p__
          #define     __CTS_J7L__
      #endif
  #endif


  //_K
  #if                 __CTS_K0t__
      #define         __CTS_K0H__
      #if             __CTS_K_p__
          #define     __CTS_K0L__
      #endif
  #endif
  #if                 __CTS_K1t__
      #define         __CTS_K1H__
      #if             __CTS_K_p__
          #define     __CTS_K1L__
      #endif
  #endif
  #if                 __CTS_K2t__
      #define         __CTS_K2H__
      #if             __CTS_K_p__
          #define     __CTS_K2L__
      #endif
  #endif
  #if                 __CTS_K3t__
      #define         __CTS_K3H__
      #if             __CTS_K_p__
          #define     __CTS_K3L__
      #endif
  #endif
  #if                 __CTS_K4t__
      #define         __CTS_K4H__
      #if             __CTS_K_p__
          #define     __CTS_K4L__
      #endif
  #endif
  #if                 __CTS_K5t__
      #define         __CTS_K5H__
      #if             __CTS_K_p__
          #define     __CTS_K5L__
      #endif
  #endif
  #if                 __CTS_K6t__
      #define         __CTS_K6H__
      #if             __CTS_K_p__
          #define     __CTS_K6L__
      #endif
  #endif
  #if                 __CTS_K7t__
      #define         __CTS_K7H__
      #if             __CTS_K_p__
          #define     __CTS_K7L__
      #endif
  #endif


  //_L
  #if                 __CTS_L0t__
      #define         __CTS_L0H__
      #if             __CTS_L_p__
          #define     __CTS_L0L__
      #endif
  #endif
  #if                 __CTS_L1t__
      #define         __CTS_L1H__
      #if             __CTS_L_p__
          #define     __CTS_L1L__
      #endif
  #endif
  #if                 __CTS_L2t__
      #define         __CTS_L2H__
      #if             __CTS_L_p__
          #define     __CTS_L2L__
      #endif
  #endif
  #if                 __CTS_L3t__
      #define         __CTS_L3H__
      #if             __CTS_L_p__
          #define     __CTS_L3L__
      #endif
  #endif
  #if                 __CTS_L4t__
      #define         __CTS_L4H__
      #if             __CTS_L_p__
          #define     __CTS_L4L__
      #endif
  #endif
  #if                 __CTS_L5t__
      #define         __CTS_L5H__
      #if             __CTS_L_p__
          #define     __CTS_L5L__
      #endif
  #endif
  #if                 __CTS_L6t__
      #define         __CTS_L6H__
      #if             __CTS_L_p__
          #define     __CTS_L6L__
      #endif
  #endif
  #if                 __CTS_L7t__
      #define         __CTS_L7H__
      #if             __CTS_L_p__
          #define     __CTS_L7L__
      #endif
  #endif


  //_M
  #if                 __CTS_M0t__
      #define         __CTS_M0H__
      #if             __CTS_M_p__
          #define     __CTS_M0L__
      #endif
  #endif
  #if                 __CTS_M1t__
      #define         __CTS_M1H__
      #if             __CTS_M_p__
          #define     __CTS_M1L__
      #endif
  #endif
  #if                 __CTS_M2t__
      #define         __CTS_M2H__
      #if             __CTS_M_p__
          #define     __CTS_M2L__
      #endif
  #endif
  #if                 __CTS_M3t__
      #define         __CTS_M3H__
      #if             __CTS_M_p__
          #define     __CTS_M3L__
      #endif
  #endif
  #if                 __CTS_M4t__
      #define         __CTS_M4H__
      #if             __CTS_M_p__
          #define     __CTS_M4L__
      #endif
  #endif
  #if                 __CTS_M5t__
      #define         __CTS_M5H__
      #if             __CTS_M_p__
          #define     __CTS_M5L__
      #endif
  #endif
  #if                 __CTS_M6t__
      #define         __CTS_M6H__
      #if             __CTS_M_p__
          #define     __CTS_M6L__
      #endif
  #endif
  #if                 __CTS_M7t__
      #define         __CTS_M7H__
      #if             __CTS_M_p__
          #define     __CTS_M7L__
      #endif
  #endif


  //_N
  #if                 __CTS_N0t__
      #define         __CTS_N0H__
      #if             __CTS_N_p__
          #define     __CTS_N0L__
      #endif
  #endif
  #if                 __CTS_N1t__
      #define         __CTS_N1H__
      #if             __CTS_N_p__
          #define     __CTS_N1L__
      #endif
  #endif
  #if                 __CTS_N2t__
      #define         __CTS_N2H__
      #if             __CTS_N_p__
          #define     __CTS_N2L__
      #endif
  #endif
  #if                 __CTS_N3t__
      #define         __CTS_N3H__
      #if             __CTS_N_p__
          #define     __CTS_N3L__
      #endif
  #endif
  #if                 __CTS_N4t__
      #define         __CTS_N4H__
      #if             __CTS_N_p__
          #define     __CTS_N4L__
      #endif
  #endif
  #if                 __CTS_N5t__
      #define         __CTS_N5H__
      #if             __CTS_N_p__
          #define     __CTS_N5L__
      #endif
  #endif
  #if                 __CTS_N6t__
      #define         __CTS_N6H__
      #if             __CTS_N_p__
          #define     __CTS_N6L__
      #endif
  #endif
  #if                 __CTS_N7t__
      #define         __CTS_N7H__
      #if             __CTS_N_p__
          #define     __CTS_N7L__
      #endif
  #endif


  //_O
  //_P
  #if                 __CTS_P0t__
      #define         __CTS_P0H__
      #if             __CTS_P_p__
          #define     __CTS_P0L__
      #endif
  #endif
  #if                 __CTS_P1t__
      #define         __CTS_P1H__
      #if             __CTS_P_p__
          #define     __CTS_P1L__
      #endif
  #endif
  #if                 __CTS_P2t__
      #define         __CTS_P2H__
      #if             __CTS_P_p__
          #define     __CTS_P2L__
      #endif
  #endif
  #if                 __CTS_P3t__
      #define         __CTS_P3H__
      #if             __CTS_P_p__
          #define     __CTS_P3L__
      #endif
  #endif
  #if                 __CTS_P4t__
      #define         __CTS_P4H__
      #if             __CTS_P_p__
          #define     __CTS_P4L__
      #endif
  #endif
  #if                 __CTS_P5t__
      #define         __CTS_P5H__
      #if             __CTS_P_p__
          #define     __CTS_P5L__
      #endif
  #endif
  #if                 __CTS_P6t__
      #define         __CTS_P6H__
      #if             __CTS_P_p__
          #define     __CTS_P6L__
      #endif
  #endif
  #if                 __CTS_P7t__
      #define         __CTS_P7H__
      #if             __CTS_P_p__
          #define     __CTS_P7L__
      #endif
  #endif


  //_Q
  #if                 __CTS_Q0t__
      #define         __CTS_Q0H__
      #if             __CTS_Q_p__
          #define     __CTS_Q0L__
      #endif
  #endif
  #if                 __CTS_Q1t__
      #define         __CTS_Q1H__
      #if             __CTS_Q_p__
          #define     __CTS_Q1L__
      #endif
  #endif
  #if                 __CTS_Q2t__
      #define         __CTS_Q2H__
      #if             __CTS_Q_p__
          #define     __CTS_Q2L__
      #endif
  #endif
  #if                 __CTS_Q3t__
      #define         __CTS_Q3H__
      #if             __CTS_Q_p__
          #define     __CTS_Q3L__
      #endif
  #endif
  #if                 __CTS_Q4t__
      #define         __CTS_Q4H__
      #if             __CTS_Q_p__
          #define     __CTS_Q4L__
      #endif
  #endif
  #if                 __CTS_Q5t__
      #define         __CTS_Q5H__
      #if             __CTS_Q_p__
          #define     __CTS_Q5L__
      #endif
  #endif
  #if                 __CTS_Q6t__
      #define         __CTS_Q6H__
      #if             __CTS_Q_p__
          #define     __CTS_Q6L__
      #endif
  #endif
  #if                 __CTS_Q7t__
      #define         __CTS_Q7H__
      #if             __CTS_Q_p__
          #define     __CTS_Q7L__
      #endif
  #endif


  //_R
  #if                 __CTS_R0t__
      #define         __CTS_R0H__
      #if             __CTS_R_p__
          #define     __CTS_R0L__
      #endif
  #endif
  #if                 __CTS_R1t__
      #define         __CTS_R1H__
      #if             __CTS_R_p__
          #define     __CTS_R1L__
      #endif
  #endif
  #if                 __CTS_R2t__
      #define         __CTS_R2H__
      #if             __CTS_R_p__
          #define     __CTS_R2L__
      #endif
  #endif
  #if                 __CTS_R3t__
      #define         __CTS_R3H__
      #if             __CTS_R_p__
          #define     __CTS_R3L__
      #endif
  #endif
  #if                 __CTS_R4t__
      #define         __CTS_R4H__
      #if             __CTS_R_p__
          #define     __CTS_R4L__
      #endif
  #endif
  #if                 __CTS_R5t__
      #define         __CTS_R5H__
      #if             __CTS_R_p__
          #define     __CTS_R5L__
      #endif
  #endif
  #if                 __CTS_R6t__
      #define         __CTS_R6H__
      #if             __CTS_R_p__
          #define     __CTS_R6L__
      #endif
  #endif
  #if                 __CTS_R7t__
      #define         __CTS_R7H__
      #if             __CTS_R_p__
          #define     __CTS_R7L__
      #endif
  #endif


  //_S
  #if                 __CTS_S0t__
      #define         __CTS_S0H__
      #if             __CTS_S_p__
          #define     __CTS_S0L__
      #endif
  #endif
  #if                 __CTS_S1t__
      #define         __CTS_S1H__
      #if             __CTS_S_p__
          #define     __CTS_S1L__
      #endif
  #endif
  #if                 __CTS_S2t__
      #define         __CTS_S2H__
      #if             __CTS_S_p__
          #define     __CTS_S2L__
      #endif
  #endif
  #if                 __CTS_S3t__
      #define         __CTS_S3H__
      #if             __CTS_S_p__
          #define     __CTS_S3L__
      #endif
  #endif
  #if                 __CTS_S4t__
      #define         __CTS_S4H__
      #if             __CTS_S_p__
          #define     __CTS_S4L__
      #endif
  #endif
  #if                 __CTS_S5t__
      #define         __CTS_S5H__
      #if             __CTS_S_p__
          #define     __CTS_S5L__
      #endif
  #endif
  #if                 __CTS_S6t__
      #define         __CTS_S6H__
      #if             __CTS_S_p__
          #define     __CTS_S6L__
      #endif
  #endif
  #if                 __CTS_S7t__
      #define         __CTS_S7H__
      #if             __CTS_S_p__
          #define     __CTS_S7L__
      #endif
  #endif


  //_T
  #if                 __CTS_T0t__
      #define         __CTS_T0H__
      #if             __CTS_T_p__
          #define     __CTS_T0L__
      #endif
  #endif
  #if                 __CTS_T1t__
      #define         __CTS_T1H__
      #if             __CTS_T_p__
          #define     __CTS_T1L__
      #endif
  #endif
  #if                 __CTS_T2t__
      #define         __CTS_T2H__
      #if             __CTS_T_p__
          #define     __CTS_T2L__
      #endif
  #endif
  #if                 __CTS_T3t__
      #define         __CTS_T3H__
      #if             __CTS_T_p__
          #define     __CTS_T3L__
      #endif
  #endif
  #if                 __CTS_T4t__
      #define         __CTS_T4H__
      #if             __CTS_T_p__
          #define     __CTS_T4L__
      #endif
  #endif
  #if                 __CTS_T5t__
      #define         __CTS_T5H__
      #if             __CTS_T_p__
          #define     __CTS_T5L__
      #endif
  #endif
  #if                 __CTS_T6t__
      #define         __CTS_T6H__
      #if             __CTS_T_p__
          #define     __CTS_T6L__
      #endif
  #endif
  #if                 __CTS_T7t__
      #define         __CTS_T7H__
      #if             __CTS_T_p__
          #define     __CTS_T7L__
      #endif
  #endif


  //_U
  #if                 __CTS_U0t__
      #define         __CTS_U0H__
      #if             __CTS_U_p__
          #define     __CTS_U0L__
      #endif
  #endif
  #if                 __CTS_U1t__
      #define         __CTS_U1H__
      #if             __CTS_U_p__
          #define     __CTS_U1L__
      #endif
  #endif
  #if                 __CTS_U2t__
      #define         __CTS_U2H__
      #if             __CTS_U_p__
          #define     __CTS_U2L__
      #endif
  #endif
  #if                 __CTS_U3t__
      #define         __CTS_U3H__
      #if             __CTS_U_p__
          #define     __CTS_U3L__
      #endif
  #endif
  #if                 __CTS_U4t__
      #define         __CTS_U4H__
      #if             __CTS_U_p__
          #define     __CTS_U4L__
      #endif
  #endif
  #if                 __CTS_U5t__
      #define         __CTS_U5H__
      #if             __CTS_U_p__
          #define     __CTS_U5L__
      #endif
  #endif
  #if                 __CTS_U6t__
      #define         __CTS_U6H__
      #if             __CTS_U_p__
          #define     __CTS_U6L__
      #endif
  #endif
  #if                 __CTS_U7t__
      #define         __CTS_U7H__
      #if             __CTS_U_p__
          #define     __CTS_U7L__
      #endif
  #endif



  //_V
  #if                 __CTS_V0t__
      #define         __CTS_V0H__
      #if             __CTS_V_p__
          #define     __CTS_V0L__
      #endif
  #endif
  #if                 __CTS_V1t__
      #define         __CTS_V1H__
      #if             __CTS_V_p__
          #define     __CTS_V1L__
      #endif
  #endif
  #if                 __CTS_V2t__
      #define         __CTS_V2H__
      #if             __CTS_V_p__
          #define     __CTS_V2L__
      #endif
  #endif
  #if                 __CTS_V3t__
      #define         __CTS_V3H__
      #if             __CTS_V_p__
          #define     __CTS_V3L__
      #endif
  #endif
  #if                 __CTS_V4t__
      #define         __CTS_V4H__
      #if             __CTS_V_p__
          #define     __CTS_V4L__
      #endif
  #endif
  #if                 __CTS_V5t__
      #define         __CTS_V5H__
      #if             __CTS_V_p__
          #define     __CTS_V5L__
      #endif
  #endif
  #if                 __CTS_V6t__
      #define         __CTS_V6H__
      #if             __CTS_V_p__
          #define     __CTS_V6L__
      #endif
  #endif
  #if                 __CTS_V7t__
      #define         __CTS_V7H__
      #if             __CTS_V_p__
          #define     __CTS_V7L__
      #endif
  #endif


  //_W
  #if                 __CTS_W0t__
      #define         __CTS_W0H__
      #if             __CTS_W_p__
          #define     __CTS_W0L__
      #endif
  #endif
  #if                 __CTS_W1t__
      #define         __CTS_W1H__
      #if             __CTS_W_p__
          #define     __CTS_W1L__
      #endif
  #endif
  #if                 __CTS_W2t__
      #define         __CTS_W2H__
      #if             __CTS_W_p__
          #define     __CTS_W2L__
      #endif
  #endif
  #if                 __CTS_W3t__
      #define         __CTS_W3H__
      #if             __CTS_W_p__
          #define     __CTS_W3L__
      #endif
  #endif
  #if                 __CTS_W4t__
      #define         __CTS_W4H__
      #if             __CTS_W_p__
          #define     __CTS_W4L__
      #endif
  #endif
  #if                 __CTS_W5t__
      #define         __CTS_W5H__
      #if             __CTS_W_p__
          #define     __CTS_W5L__
      #endif
  #endif
  #if                 __CTS_W6t__
      #define         __CTS_W6H__
      #if             __CTS_W_p__
          #define     __CTS_W6L__
      #endif
  #endif
  #if                 __CTS_W7t__
      #define         __CTS_W7H__
      #if             __CTS_W_p__
          #define     __CTS_W7L__
      #endif
  #endif

  /*******************************************************************************
  *    ptrf pfCTSType
  *******************************************************************************/
  typedef INT8   (*pfCTS_LowType)(INT8 CTS_arg);

  /*******************************************************************************
  *    INT8 CTS_A0Hs(INT8 CTS_arg);  << High-side-safe
  *    INT8 CTS_A0Hf(INT8 CTS_arg);  << High-side-fast
  *    INT8 CTS_A0Ls(INT8 CTS_arg);  << Low-side-fast (RS08, S12)
  *    INT8 CTS_A0Lf(INT8 CTS_arg);  << Low-side-fast (RS08, S12)
  *******************************************************************************/

  //_A
  #ifdef          __CTS_A0H__
      _ctsabi INT8 CTS_A0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A0Hf(INT8 CTS_arg);
      #ifdef      __CTS_A0L__
      _ctsabi INT8 CTS_A0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A1H__
      _ctsabi INT8 CTS_A1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A1Hf(INT8 CTS_arg);
      #ifdef      __CTS_A1L__
      _ctsabi INT8 CTS_A1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A2H__
      _ctsabi INT8 CTS_A2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A2Hf(INT8 CTS_arg);
      #ifdef      __CTS_A2L__
      _ctsabi INT8 CTS_A2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A3H__
      _ctsabi INT8 CTS_A3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A3Hf(INT8 CTS_arg);
      #ifdef      __CTS_A3L__
      _ctsabi INT8 CTS_A3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A4H__
      _ctsabi INT8 CTS_A4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A4Hf(INT8 CTS_arg);
      #ifdef      __CTS_A4L__
      _ctsabi INT8 CTS_A4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A5H__
      _ctsabi INT8 CTS_A5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A5Hf(INT8 CTS_arg);
      #ifdef      __CTS_A5L__
      _ctsabi INT8 CTS_A5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A6H__
      _ctsabi INT8 CTS_A6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A6Hf(INT8 CTS_arg);
      #ifdef      __CTS_A6L__
      _ctsabi INT8 CTS_A6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_A7H__
      _ctsabi INT8 CTS_A7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A7Hf(INT8 CTS_arg);
      #ifdef      __CTS_A7L__
      _ctsabi INT8 CTS_A7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_A7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_B
  #ifdef          __CTS_B0H__
      _ctsabi INT8 CTS_B0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B0Hf(INT8 CTS_arg);
      #ifdef      __CTS_B0L__
      _ctsabi INT8 CTS_B0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B1H__
      _ctsabi INT8 CTS_B1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B1Hf(INT8 CTS_arg);
      #ifdef      __CTS_B1L__
      _ctsabi INT8 CTS_B1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B2H__
      _ctsabi INT8 CTS_B2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B2Hf(INT8 CTS_arg);
      #ifdef      __CTS_B2L__
      _ctsabi INT8 CTS_B2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B3H__
      _ctsabi INT8 CTS_B3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B3Hf(INT8 CTS_arg);
      #ifdef      __CTS_B3L__
      _ctsabi INT8 CTS_B3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B4H__
      _ctsabi INT8 CTS_B4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B4Hf(INT8 CTS_arg);
      #ifdef      __CTS_B4L__
      _ctsabi INT8 CTS_B4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B5H__
      _ctsabi INT8 CTS_B5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B5Hf(INT8 CTS_arg);
      #ifdef      __CTS_B5L__
      _ctsabi INT8 CTS_B5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B6H__
      _ctsabi INT8 CTS_B6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B6Hf(INT8 CTS_arg);
      #ifdef      __CTS_B6L__
      _ctsabi INT8 CTS_B6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_B7H__
      _ctsabi INT8 CTS_B7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B7Hf(INT8 CTS_arg);
      #ifdef      __CTS_B7L__
      _ctsabi INT8 CTS_B7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_B7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_C
  #ifdef          __CTS_C0H__
      _ctsabi INT8 CTS_C0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C0Hf(INT8 CTS_arg);
      #ifdef      __CTS_C0L__
      _ctsabi INT8 CTS_C0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C1H__
      _ctsabi INT8 CTS_C1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C1Hf(INT8 CTS_arg);
      #ifdef      __CTS_C1L__
      _ctsabi INT8 CTS_C1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C2H__
      _ctsabi INT8 CTS_C2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C2Hf(INT8 CTS_arg);
      #ifdef      __CTS_C2L__
      _ctsabi INT8 CTS_C2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C3H__
      _ctsabi INT8 CTS_C3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C3Hf(INT8 CTS_arg);
      #ifdef      __CTS_C3L__
      _ctsabi INT8 CTS_C3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C4H__
      _ctsabi INT8 CTS_C4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C4Hf(INT8 CTS_arg);
      #ifdef      __CTS_C4L__
      _ctsabi INT8 CTS_C4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C5H__
      _ctsabi INT8 CTS_C5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C5Hf(INT8 CTS_arg);
      #ifdef      __CTS_C5L__
      _ctsabi INT8 CTS_C5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C6H__
      _ctsabi INT8 CTS_C6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C6Hf(INT8 CTS_arg);
      #ifdef      __CTS_C6L__
      _ctsabi INT8 CTS_C6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_C7H__
      _ctsabi INT8 CTS_C7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C7Hf(INT8 CTS_arg);
      #ifdef      __CTS_C7L__
      _ctsabi INT8 CTS_C7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_C7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_D
  #ifdef          __CTS_D0H__
      _ctsabi INT8 CTS_D0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D0Hf(INT8 CTS_arg);
      #ifdef      __CTS_D0L__
      _ctsabi INT8 CTS_D0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D1H__
      _ctsabi INT8 CTS_D1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D1Hf(INT8 CTS_arg);
      #ifdef      __CTS_D1L__
      _ctsabi INT8 CTS_D1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D2H__
      _ctsabi INT8 CTS_D2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D2Hf(INT8 CTS_arg);
      #ifdef      __CTS_D2L__
      _ctsabi INT8 CTS_D2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D3H__
      _ctsabi INT8 CTS_D3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D3Hf(INT8 CTS_arg);
      #ifdef      __CTS_D3L__
      _ctsabi INT8 CTS_D3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D4H__
      _ctsabi INT8 CTS_D4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D4Hf(INT8 CTS_arg);
      #ifdef      __CTS_D4L__
      _ctsabi INT8 CTS_D4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D5H__
      _ctsabi INT8 CTS_D5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D5Hf(INT8 CTS_arg);
      #ifdef      __CTS_D5L__
      _ctsabi INT8 CTS_D5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D6H__
      _ctsabi INT8 CTS_D6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D6Hf(INT8 CTS_arg);
      #ifdef      __CTS_D6L__
      _ctsabi INT8 CTS_D6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_D7H__
      _ctsabi INT8 CTS_D7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D7Hf(INT8 CTS_arg);
      #ifdef      __CTS_D7L__
      _ctsabi INT8 CTS_D7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_D7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_E
  #ifdef          __CTS_E0H__
      _ctsabi INT8 CTS_E0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E0Hf(INT8 CTS_arg);
      #ifdef      __CTS_E0L__
      _ctsabi INT8 CTS_E0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E1H__
      _ctsabi INT8 CTS_E1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E1Hf(INT8 CTS_arg);
      #ifdef      __CTS_E1L__
      _ctsabi INT8 CTS_E1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E2H__
      _ctsabi INT8 CTS_E2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E2Hf(INT8 CTS_arg);
      #ifdef      __CTS_E2L__
      _ctsabi INT8 CTS_E2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E3H__
      _ctsabi INT8 CTS_E3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E3Hf(INT8 CTS_arg);
      #ifdef      __CTS_E3L__
      _ctsabi INT8 CTS_E3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E4H__
      _ctsabi INT8 CTS_E4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E4Hf(INT8 CTS_arg);
      #ifdef      __CTS_E4L__
      _ctsabi INT8 CTS_E4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E5H__
      _ctsabi INT8 CTS_E5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E5Hf(INT8 CTS_arg);
      #ifdef      __CTS_E5L__
      _ctsabi INT8 CTS_E5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E6H__
      _ctsabi INT8 CTS_E6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E6Hf(INT8 CTS_arg);
      #ifdef      __CTS_E6L__
      _ctsabi INT8 CTS_E6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_E7H__
      _ctsabi INT8 CTS_E7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E7Hf(INT8 CTS_arg);
      #ifdef      __CTS_E7L__
      _ctsabi INT8 CTS_E7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_E7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_F
  #ifdef          __CTS_F0H__
      _ctsabi INT8 CTS_F0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F0Hf(INT8 CTS_arg);
      #ifdef      __CTS_F0L__
      _ctsabi INT8 CTS_F0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F1H__
      _ctsabi INT8 CTS_F1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F1Hf(INT8 CTS_arg);
      #ifdef      __CTS_F1L__
      _ctsabi INT8 CTS_F1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F2H__
      _ctsabi INT8 CTS_F2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F2Hf(INT8 CTS_arg);
      #ifdef      __CTS_F2L__
      _ctsabi INT8 CTS_F2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F3H__
      _ctsabi INT8 CTS_F3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F3Hf(INT8 CTS_arg);
      #ifdef      __CTS_F3L__
      _ctsabi INT8 CTS_F3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F4H__
      _ctsabi INT8 CTS_F4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F4Hf(INT8 CTS_arg);
      #ifdef      __CTS_F4L__
      _ctsabi INT8 CTS_F4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F5H__
      _ctsabi INT8 CTS_F5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F5Hf(INT8 CTS_arg);
      #ifdef      __CTS_F5L__
      _ctsabi INT8 CTS_F5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F6H__
      _ctsabi INT8 CTS_F6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F6Hf(INT8 CTS_arg);
      #ifdef      __CTS_F6L__
      _ctsabi INT8 CTS_F6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_F7H__
      _ctsabi INT8 CTS_F7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F7Hf(INT8 CTS_arg);
      #ifdef      __CTS_F7L__
      _ctsabi INT8 CTS_F7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_F7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_G
  #ifdef          __CTS_G0H__
      _ctsabi INT8 CTS_G0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G0Hf(INT8 CTS_arg);
      #ifdef      __CTS_G0L__
      _ctsabi INT8 CTS_G0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G1H__
      _ctsabi INT8 CTS_G1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G1Hf(INT8 CTS_arg);
      #ifdef      __CTS_G1L__
      _ctsabi INT8 CTS_G1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G2H__
      _ctsabi INT8 CTS_G2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G2Hf(INT8 CTS_arg);
      #ifdef      __CTS_G2L__
      _ctsabi INT8 CTS_G2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G3H__
      _ctsabi INT8 CTS_G3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G3Hf(INT8 CTS_arg);
      #ifdef      __CTS_G3L__
      _ctsabi INT8 CTS_G3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G4H__
      _ctsabi INT8 CTS_G4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G4Hf(INT8 CTS_arg);
      #ifdef      __CTS_G4L__
      _ctsabi INT8 CTS_G4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G5H__
      _ctsabi INT8 CTS_G5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G5Hf(INT8 CTS_arg);
      #ifdef      __CTS_G5L__
      _ctsabi INT8 CTS_G5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G6H__
      _ctsabi INT8 CTS_G6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G6Hf(INT8 CTS_arg);
      #ifdef      __CTS_G6L__
      _ctsabi INT8 CTS_G6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_G7H__
      _ctsabi INT8 CTS_G7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G7Hf(INT8 CTS_arg);
      #ifdef      __CTS_G7L__
      _ctsabi INT8 CTS_G7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_G7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_H
  #ifdef          __CTS_H0H__
      _ctsabi INT8 CTS_H0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H0Hf(INT8 CTS_arg);
      #ifdef      __CTS_H0L__
      _ctsabi INT8 CTS_H0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H1H__
      _ctsabi INT8 CTS_H1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H1Hf(INT8 CTS_arg);
      #ifdef      __CTS_H1L__
      _ctsabi INT8 CTS_H1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H2H__
      _ctsabi INT8 CTS_H2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H2Hf(INT8 CTS_arg);
      #ifdef      __CTS_H2L__
      _ctsabi INT8 CTS_H2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H3H__
      _ctsabi INT8 CTS_H3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H3Hf(INT8 CTS_arg);
      #ifdef      __CTS_H3L__
      _ctsabi INT8 CTS_H3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H4H__
      _ctsabi INT8 CTS_H4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H4Hf(INT8 CTS_arg);
      #ifdef      __CTS_H4L__
      _ctsabi INT8 CTS_H4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H5H__
      _ctsabi INT8 CTS_H5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H5Hf(INT8 CTS_arg);
      #ifdef      __CTS_H5L__
      _ctsabi INT8 CTS_H5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H6H__
      _ctsabi INT8 CTS_H6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H6Hf(INT8 CTS_arg);
      #ifdef      __CTS_H6L__
      _ctsabi INT8 CTS_H6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_H7H__
      _ctsabi INT8 CTS_H7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H7Hf(INT8 CTS_arg);
      #ifdef      __CTS_H7L__
      _ctsabi INT8 CTS_H7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_H7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_I
  #ifdef          __CTS_I0H__
      _ctsabi INT8 CTS_I0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I0Hf(INT8 CTS_arg);
      #ifdef      __CTS_I0L__
      _ctsabi INT8 CTS_I0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I1H__
      _ctsabi INT8 CTS_I1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I1Hf(INT8 CTS_arg);
      #ifdef      __CTS_I1L__
      _ctsabi INT8 CTS_I1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I2H__
      _ctsabi INT8 CTS_I2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I2Hf(INT8 CTS_arg);
      #ifdef      __CTS_I2L__
      _ctsabi INT8 CTS_I2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I3H__
      _ctsabi INT8 CTS_I3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I3Hf(INT8 CTS_arg);
      #ifdef      __CTS_I3L__
      _ctsabi INT8 CTS_I3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I4H__
      _ctsabi INT8 CTS_I4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I4Hf(INT8 CTS_arg);
      #ifdef      __CTS_I4L__
      _ctsabi INT8 CTS_I4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I5H__
      _ctsabi INT8 CTS_I5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I5Hf(INT8 CTS_arg);
      #ifdef      __CTS_I5L__
      _ctsabi INT8 CTS_I5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I6H__
      _ctsabi INT8 CTS_I6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I6Hf(INT8 CTS_arg);
      #ifdef      __CTS_I6L__
      _ctsabi INT8 CTS_I6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_I7H__
      _ctsabi INT8 CTS_I7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I7Hf(INT8 CTS_arg);
      #ifdef      __CTS_I7L__
      _ctsabi INT8 CTS_I7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_I7Lf(INT8 CTS_arg);
      #endif
  #endif

  //_J
  #ifdef          __CTS_J0H__
      _ctsabi INT8 CTS_J0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J0Hf(INT8 CTS_arg);
      #ifdef      __CTS_J0L__
      _ctsabi INT8 CTS_J0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J1H__
      _ctsabi INT8 CTS_J1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J1Hf(INT8 CTS_arg);
      #ifdef      __CTS_J1L__
      _ctsabi INT8 CTS_J1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J2H__
      _ctsabi INT8 CTS_J2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J2Hf(INT8 CTS_arg);
      #ifdef      __CTS_J2L__
      _ctsabi INT8 CTS_J2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J3H__
      _ctsabi INT8 CTS_J3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J3Hf(INT8 CTS_arg);
      #ifdef      __CTS_J3L__
      _ctsabi INT8 CTS_J3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J4H__
      _ctsabi INT8 CTS_J4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J4Hf(INT8 CTS_arg);
      #ifdef      __CTS_J4L__
      _ctsabi INT8 CTS_J4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J5H__
      _ctsabi INT8 CTS_J5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J5Hf(INT8 CTS_arg);
      #ifdef      __CTS_J5L__
      _ctsabi INT8 CTS_J5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J6H__
      _ctsabi INT8 CTS_J6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J6Hf(INT8 CTS_arg);
      #ifdef      __CTS_J6L__
      _ctsabi INT8 CTS_J6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_J7H__
      _ctsabi INT8 CTS_J7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J7Hf(INT8 CTS_arg);
      #ifdef      __CTS_J7L__
      _ctsabi INT8 CTS_J7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_J7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_K
  #ifdef          __CTS_K0H__
      _ctsabi INT8 CTS_K0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K0Hf(INT8 CTS_arg);
      #ifdef      __CTS_K0L__
      _ctsabi INT8 CTS_K0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K1H__
      _ctsabi INT8 CTS_K1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K1Hf(INT8 CTS_arg);
      #ifdef      __CTS_K1L__
      _ctsabi INT8 CTS_K1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K2H__
      _ctsabi INT8 CTS_K2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K2Hf(INT8 CTS_arg);
      #ifdef      __CTS_K2L__
      _ctsabi INT8 CTS_K2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K3H__
      _ctsabi INT8 CTS_K3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K3Hf(INT8 CTS_arg);
      #ifdef      __CTS_K3L__
      _ctsabi INT8 CTS_K3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K4H__
      _ctsabi INT8 CTS_K4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K4Hf(INT8 CTS_arg);
      #ifdef      __CTS_K4L__
      _ctsabi INT8 CTS_K4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K5H__
      _ctsabi INT8 CTS_K5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K5Hf(INT8 CTS_arg);
      #ifdef      __CTS_K5L__
      _ctsabi INT8 CTS_K5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K6H__
      _ctsabi INT8 CTS_K6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K6Hf(INT8 CTS_arg);
      #ifdef      __CTS_K6L__
      _ctsabi INT8 CTS_K6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_K7H__
      _ctsabi INT8 CTS_K7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K7Hf(INT8 CTS_arg);
      #ifdef      __CTS_K7L__
      _ctsabi INT8 CTS_K7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_K7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_L
  #ifdef          __CTS_L0H__
      _ctsabi INT8 CTS_L0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L0Hf(INT8 CTS_arg);
      #ifdef      __CTS_L0L__
      _ctsabi INT8 CTS_L0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L1H__
      _ctsabi INT8 CTS_L1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L1Hf(INT8 CTS_arg);
      #ifdef      __CTS_L1L__
      _ctsabi INT8 CTS_L1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L2H__
      _ctsabi INT8 CTS_L2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L2Hf(INT8 CTS_arg);
      #ifdef      __CTS_L2L__
      _ctsabi INT8 CTS_L2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L3H__
      _ctsabi INT8 CTS_L3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L3Hf(INT8 CTS_arg);
      #ifdef      __CTS_L3L__
      _ctsabi INT8 CTS_L3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L4H__
      _ctsabi INT8 CTS_L4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L4Hf(INT8 CTS_arg);
      #ifdef      __CTS_L4L__
      _ctsabi INT8 CTS_L4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L5H__
      _ctsabi INT8 CTS_L5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L5Hf(INT8 CTS_arg);
      #ifdef      __CTS_L5L__
      _ctsabi INT8 CTS_L5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L6H__
      _ctsabi INT8 CTS_L6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L6Hf(INT8 CTS_arg);
      #ifdef      __CTS_L6L__
      _ctsabi INT8 CTS_L6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_L7H__
      _ctsabi INT8 CTS_L7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L7Hf(INT8 CTS_arg);
      #ifdef      __CTS_L7L__
      _ctsabi INT8 CTS_L7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_L7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_M
  #ifdef          __CTS_M0H__
      _ctsabi INT8 CTS_M0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M0Hf(INT8 CTS_arg);
      #ifdef      __CTS_M0L__
      _ctsabi INT8 CTS_M0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M1H__
      _ctsabi INT8 CTS_M1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M1Hf(INT8 CTS_arg);
      #ifdef      __CTS_M1L__
      _ctsabi INT8 CTS_M1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M2H__
      _ctsabi INT8 CTS_M2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M2Hf(INT8 CTS_arg);
      #ifdef      __CTS_M2L__
      _ctsabi INT8 CTS_M2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M3H__
      _ctsabi INT8 CTS_M3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M3Hf(INT8 CTS_arg);
      #ifdef      __CTS_M3L__
      _ctsabi INT8 CTS_M3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M4H__
      _ctsabi INT8 CTS_M4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M4Hf(INT8 CTS_arg);
      #ifdef      __CTS_M4L__
      _ctsabi INT8 CTS_M4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M5H__
      _ctsabi INT8 CTS_M5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M5Hf(INT8 CTS_arg);
      #ifdef      __CTS_M5L__
      _ctsabi INT8 CTS_M5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M6H__
      _ctsabi INT8 CTS_M6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M6Hf(INT8 CTS_arg);
      #ifdef      __CTS_M6L__
      _ctsabi INT8 CTS_M6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_M7H__
      _ctsabi INT8 CTS_M7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M7Hf(INT8 CTS_arg);
      #ifdef      __CTS_M7L__
      _ctsabi INT8 CTS_M7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_M7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_N
  #ifdef          __CTS_N0H__
      _ctsabi INT8 CTS_N0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N0Hf(INT8 CTS_arg);
      #ifdef      __CTS_N0L__
      _ctsabi INT8 CTS_N0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N1H__
      _ctsabi INT8 CTS_N1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N1Hf(INT8 CTS_arg);
      #ifdef      __CTS_N1L__
      _ctsabi INT8 CTS_N1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N2H__
      _ctsabi INT8 CTS_N2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N2Hf(INT8 CTS_arg);
      #ifdef      __CTS_N2L__
      _ctsabi INT8 CTS_N2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N3H__
      _ctsabi INT8 CTS_N3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N3Hf(INT8 CTS_arg);
      #ifdef      __CTS_N3L__
      _ctsabi INT8 CTS_N3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N4H__
      _ctsabi INT8 CTS_N4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N4Hf(INT8 CTS_arg);
      #ifdef      __CTS_N4L__
      _ctsabi INT8 CTS_N4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N5H__
      _ctsabi INT8 CTS_N5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N5Hf(INT8 CTS_arg);
      #ifdef      __CTS_N5L__
      _ctsabi INT8 CTS_N5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N6H__
      _ctsabi INT8 CTS_N6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N6Hf(INT8 CTS_arg);
      #ifdef      __CTS_N6L__
      _ctsabi INT8 CTS_N6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_N7H__
      _ctsabi INT8 CTS_N7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N7Hf(INT8 CTS_arg);
      #ifdef      __CTS_N7L__
      _ctsabi INT8 CTS_N7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_N7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_O
  //_P
  #ifdef          __CTS_P0H__
      _ctsabi INT8 CTS_P0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P0Hf(INT8 CTS_arg);
      #ifdef      __CTS_P0L__
      _ctsabi INT8 CTS_P0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P1H__
      _ctsabi INT8 CTS_P1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P1Hf(INT8 CTS_arg);
      #ifdef      __CTS_P1L__
      _ctsabi INT8 CTS_P1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P2H__
      _ctsabi INT8 CTS_P2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P2Hf(INT8 CTS_arg);
      #ifdef      __CTS_P2L__
      _ctsabi INT8 CTS_P2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P3H__
      _ctsabi INT8 CTS_P3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P3Hf(INT8 CTS_arg);
      #ifdef      __CTS_P3L__
      _ctsabi INT8 CTS_P3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P4H__
      _ctsabi INT8 CTS_P4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P4Hf(INT8 CTS_arg);
      #ifdef      __CTS_P4L__
      _ctsabi INT8 CTS_P4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P5H__
      _ctsabi INT8 CTS_P5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P5Hf(INT8 CTS_arg);
      #ifdef      __CTS_P5L__
      _ctsabi INT8 CTS_P5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P6H__
      _ctsabi INT8 CTS_P6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P6Hf(INT8 CTS_arg);
      #ifdef      __CTS_P6L__
      _ctsabi INT8 CTS_P6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_P7H__
      _ctsabi INT8 CTS_P7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P7Hf(INT8 CTS_arg);
      #ifdef      __CTS_P7L__
      _ctsabi INT8 CTS_P7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_P7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_Q
  #ifdef          __CTS_Q0H__
      _ctsabi INT8 CTS_Q0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q0Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q0L__
      _ctsabi INT8 CTS_Q0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q1H__
      _ctsabi INT8 CTS_Q1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q1Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q1L__
      _ctsabi INT8 CTS_Q1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q2H__
      _ctsabi INT8 CTS_Q2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q2Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q2L__
      _ctsabi INT8 CTS_Q2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q3H__
      _ctsabi INT8 CTS_Q3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q3Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q3L__
      _ctsabi INT8 CTS_Q3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q4H__
      _ctsabi INT8 CTS_Q4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q4Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q4L__
      _ctsabi INT8 CTS_Q4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q5H__
      _ctsabi INT8 CTS_Q5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q5Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q5L__
      _ctsabi INT8 CTS_Q5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q6H__
      _ctsabi INT8 CTS_Q6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q6Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q6L__
      _ctsabi INT8 CTS_Q6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_Q7H__
      _ctsabi INT8 CTS_Q7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q7Hf(INT8 CTS_arg);
      #ifdef      __CTS_Q7L__
      _ctsabi INT8 CTS_Q7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_Q7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_R
  #ifdef          __CTS_R0H__
      _ctsabi INT8 CTS_R0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R0Hf(INT8 CTS_arg);
      #ifdef      __CTS_R0L__
      _ctsabi INT8 CTS_R0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R1H__
      _ctsabi INT8 CTS_R1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R1Hf(INT8 CTS_arg);
      #ifdef      __CTS_R1L__
      _ctsabi INT8 CTS_R1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R2H__
      _ctsabi INT8 CTS_R2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R2Hf(INT8 CTS_arg);
      #ifdef      __CTS_R2L__
      _ctsabi INT8 CTS_R2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R3H__
      _ctsabi INT8 CTS_R3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R3Hf(INT8 CTS_arg);
      #ifdef      __CTS_R3L__
      _ctsabi INT8 CTS_R3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R4H__
      _ctsabi INT8 CTS_R4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R4Hf(INT8 CTS_arg);
      #ifdef      __CTS_R4L__
      _ctsabi INT8 CTS_R4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R5H__
      _ctsabi INT8 CTS_R5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R5Hf(INT8 CTS_arg);
      #ifdef      __CTS_R5L__
      _ctsabi INT8 CTS_R5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R6H__
      _ctsabi INT8 CTS_R6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R6Hf(INT8 CTS_arg);
      #ifdef      __CTS_R6L__
      _ctsabi INT8 CTS_R6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_R7H__
      _ctsabi INT8 CTS_R7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R7Hf(INT8 CTS_arg);
      #ifdef      __CTS_R7L__
      _ctsabi INT8 CTS_R7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_R7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_S
  #ifdef          __CTS_S0H__
      _ctsabi INT8 CTS_S0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S0Hf(INT8 CTS_arg);
      #ifdef      __CTS_S0L__
      _ctsabi INT8 CTS_S0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S1H__
      _ctsabi INT8 CTS_S1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S1Hf(INT8 CTS_arg);
      #ifdef      __CTS_S1L__
      _ctsabi INT8 CTS_S1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S2H__
      _ctsabi INT8 CTS_S2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S2Hf(INT8 CTS_arg);
      #ifdef      __CTS_S2L__
      _ctsabi INT8 CTS_S2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S3H__
      _ctsabi INT8 CTS_S3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S3Hf(INT8 CTS_arg);
      #ifdef      __CTS_S3L__
      _ctsabi INT8 CTS_S3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S4H__
      _ctsabi INT8 CTS_S4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S4Hf(INT8 CTS_arg);
      #ifdef      __CTS_S4L__
      _ctsabi INT8 CTS_S4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S5H__
      _ctsabi INT8 CTS_S5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S5Hf(INT8 CTS_arg);
      #ifdef      __CTS_S5L__
      _ctsabi INT8 CTS_S5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S6H__
      _ctsabi INT8 CTS_S6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S6Hf(INT8 CTS_arg);
      #ifdef      __CTS_S6L__
      _ctsabi INT8 CTS_S6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_S7H__
      _ctsabi INT8 CTS_S7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S7Hf(INT8 CTS_arg);
      #ifdef      __CTS_S7L__
      _ctsabi INT8 CTS_S7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_S7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_T
  #ifdef          __CTS_T0H__
      _ctsabi INT8 CTS_T0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T0Hf(INT8 CTS_arg);
      #ifdef      __CTS_T0L__
      _ctsabi INT8 CTS_T0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T1H__
      _ctsabi INT8 CTS_T1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T1Hf(INT8 CTS_arg);
      #ifdef      __CTS_T1L__
      _ctsabi INT8 CTS_T1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T2H__
      _ctsabi INT8 CTS_T2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T2Hf(INT8 CTS_arg);
      #ifdef      __CTS_T2L__
      _ctsabi INT8 CTS_T2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T3H__
      _ctsabi INT8 CTS_T3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T3Hf(INT8 CTS_arg);
      #ifdef      __CTS_T3L__
      _ctsabi INT8 CTS_T3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T4H__
      _ctsabi INT8 CTS_T4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T4Hf(INT8 CTS_arg);
      #ifdef      __CTS_T4L__
      _ctsabi INT8 CTS_T4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T5H__
      _ctsabi INT8 CTS_T5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T5Hf(INT8 CTS_arg);
      #ifdef      __CTS_T5L__
      _ctsabi INT8 CTS_T5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T6H__
      _ctsabi INT8 CTS_T6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T6Hf(INT8 CTS_arg);
      #ifdef      __CTS_T6L__
      _ctsabi INT8 CTS_T6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_T7H__
      _ctsabi INT8 CTS_T7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T7Hf(INT8 CTS_arg);
      #ifdef      __CTS_T7L__
      _ctsabi INT8 CTS_T7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_T7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_U
  #ifdef          __CTS_U0H__
      _ctsabi INT8 CTS_U0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U0Hf(INT8 CTS_arg);
      #ifdef      __CTS_U0L__
      _ctsabi INT8 CTS_U0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U1H__
      _ctsabi INT8 CTS_U1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U1Hf(INT8 CTS_arg);
      #ifdef      __CTS_U1L__
      _ctsabi INT8 CTS_U1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U2H__
      _ctsabi INT8 CTS_U2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U2Hf(INT8 CTS_arg);
      #ifdef      __CTS_U2L__
      _ctsabi INT8 CTS_U2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U3H__
      _ctsabi INT8 CTS_U3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U3Hf(INT8 CTS_arg);
      #ifdef      __CTS_U3L__
      _ctsabi INT8 CTS_U3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U4H__
      _ctsabi INT8 CTS_U4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U4Hf(INT8 CTS_arg);
      #ifdef      __CTS_U4L__
      _ctsabi INT8 CTS_U4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U5H__
      _ctsabi INT8 CTS_U5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U5Hf(INT8 CTS_arg);
      #ifdef      __CTS_U5L__
      _ctsabi INT8 CTS_U5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U6H__
      _ctsabi INT8 CTS_U6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U6Hf(INT8 CTS_arg);
      #ifdef      __CTS_U6L__
      _ctsabi INT8 CTS_U6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_U7H__
      _ctsabi INT8 CTS_U7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U7Hf(INT8 CTS_arg);
      #ifdef      __CTS_U7L__
      _ctsabi INT8 CTS_U7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_U7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_V
  #ifdef          __CTS_V0H__
      _ctsabi INT8 CTS_V0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V0Hf(INT8 CTS_arg);
      #ifdef      __CTS_V0L__
      _ctsabi INT8 CTS_V0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V1H__
      _ctsabi INT8 CTS_V1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V1Hf(INT8 CTS_arg);
      #ifdef      __CTS_V1L__
      _ctsabi INT8 CTS_V1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V2H__
      _ctsabi INT8 CTS_V2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V2Hf(INT8 CTS_arg);
      #ifdef      __CTS_V2L__
      _ctsabi INT8 CTS_V2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V3H__
      _ctsabi INT8 CTS_V3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V3Hf(INT8 CTS_arg);
      #ifdef      __CTS_V3L__
      _ctsabi INT8 CTS_V3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V4H__
      _ctsabi INT8 CTS_V4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V4Hf(INT8 CTS_arg);
      #ifdef      __CTS_V4L__
      _ctsabi INT8 CTS_V4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V5H__
      _ctsabi INT8 CTS_V5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V5Hf(INT8 CTS_arg);
      #ifdef      __CTS_V5L__
      _ctsabi INT8 CTS_V5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V6H__
      _ctsabi INT8 CTS_V6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V6Hf(INT8 CTS_arg);
      #ifdef      __CTS_V6L__
      _ctsabi INT8 CTS_V6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_V7H__
      _ctsabi INT8 CTS_V7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V7Hf(INT8 CTS_arg);
      #ifdef      __CTS_V7L__
      _ctsabi INT8 CTS_V7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_V7Lf(INT8 CTS_arg);
      #endif
  #endif


  //_W
  #ifdef          __CTS_W0H__
      _ctsabi INT8 CTS_W0HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W0Hf(INT8 CTS_arg);
      #ifdef      __CTS_W0L__
      _ctsabi INT8 CTS_W0LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W0Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W1H__
      _ctsabi INT8 CTS_W1HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W1Hf(INT8 CTS_arg);
      #ifdef      __CTS_W1L__
      _ctsabi INT8 CTS_W1LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W1Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W2H__
      _ctsabi INT8 CTS_W2HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W2Hf(INT8 CTS_arg);
      #ifdef      __CTS_W2L__
      _ctsabi INT8 CTS_W2LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W2Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W3H__
      _ctsabi INT8 CTS_W3HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W3Hf(INT8 CTS_arg);
      #ifdef      __CTS_W3L__
      _ctsabi INT8 CTS_W3LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W3Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W4H__
      _ctsabi INT8 CTS_W4HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W4Hf(INT8 CTS_arg);
      #ifdef      __CTS_W4L__
      _ctsabi INT8 CTS_W4LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W4Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W5H__
      _ctsabi INT8 CTS_W5HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W5Hf(INT8 CTS_arg);
      #ifdef      __CTS_W5L__
      _ctsabi INT8 CTS_W5LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W5Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W6H__
      _ctsabi INT8 CTS_W6HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W6Hf(INT8 CTS_arg);
      #ifdef      __CTS_W6L__
      _ctsabi INT8 CTS_W6LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W6Lf(INT8 CTS_arg);
      #endif
  #endif
  #ifdef          __CTS_W7H__
      _ctsabi INT8 CTS_W7HS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W7Hf(INT8 CTS_arg);
      #ifdef      __CTS_W7L__
      _ctsabi INT8 CTS_W7LS(INT8 CTS_arg);
      _ctsabi INT8 CTS_W7Lf(INT8 CTS_arg);
      #endif
  #endif

#endif /*__TSS_SENSORCTS_H__*/
