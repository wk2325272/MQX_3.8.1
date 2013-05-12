/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_SensorPTI.h
*
* @brief  Functions Prototypes, constants, variables and macros for the sensing of electrodes
*
* @version 1.0.16.0
* 
* @date Dec-15-2011
* 
*
***********************************************************************************************************************/

#ifndef __TSS_SENSORPTI_H__
  #define __TSS_SENSORPTI_H__

  #include "TSS_Sensor.h"

  /***************************** Types ***************************/

  #if TSS_HCS08_MCU || TSS_CFV1_MCU

    /* Electrode Data */
    typedef struct {
      const UINT8 pti_cu8ModuleID;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        const INT8 pti_ci8AmplitudeFilterDeltaLimit;
      #endif
    } TSS_PTI_ELECTRODE_ROMDATA;    
    
    typedef struct {
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        UINT16 pti_u16AmplitudeFilterBase;
      #else
        UINT8 u8Dummy;
      #endif
    } TSS_PTI_ELECTRODE_RAMDATA;

  #elif TSS_KINETIS_MCU
    
    /* Method Data */
    typedef struct {
      const TSS_METHOD_CONTROL pti_fMethodControl;
      const UINT32 * const pti_cpsu32Timer;
      const UINT32 * const pti_cpsu32Port;
      const UINT32 * const pti_cpsu32RGPIO;
      const UINT8 pti_u8TimerIRQNum;    
      const UINT8 pti_u8PortIRQNum;        
    } TSS_PTI_METHOD_ROMDATA;

    /* Electrode Data */    
    typedef struct {
      const TSS_PTI_METHOD_ROMDATA * const pti_cpsu32MethodROMData; 
      const UINT32 pti_cu32PortBit;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        const INT8 pti_ci8AmplitudeFilterDeltaLimit;
      #endif
    } TSS_PTI_ELECTRODE_ROMDATA;    

    typedef struct {
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        UINT16 pti_u16AmplitudeFilterBase;
      #else
        UINT8 u8Dummy;
      #endif
    } TSS_PTI_ELECTRODE_RAMDATA;
    
  #elif TSS_CFM_MCU

    /* Method Data */
    typedef struct {
      const TSS_METHOD_CONTROL pti_fMethodControl;
      const UINT8 * const pti_cpsu8Timer;
      const UINT8 * const pti_cpsu8PCTL;
      const UINT8 * const pti_cpsu8Port;
      const UINT8 * const pti_cpsu8MXC;
    } TSS_PTI_METHOD_ROMDATA;

    /* Electrode Data */
    typedef struct {
      const TSS_PTI_METHOD_ROMDATA * const pti_cpsu8MethodROMData; 
      const UINT8 pti_cu8PortBit;
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        const INT8 pti_ci8AmplitudeFilterDeltaLimit;
      #endif  
    } TSS_PTI_ELECTRODE_ROMDATA;    
  
    typedef struct {
      #if TSS_USE_NOISE_AMPLITUDE_FILTER
        UINT16 pti_u16AmplitudeFilterBase;
      #else
      UINT8 u8Dummy;        
      #endif  
    } TSS_PTI_ELECTRODE_RAMDATA;

  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    

  /******************** Functions prototypes **********************/
  
  extern UINT8 PTI_SensorInit(void);
  /*
  function:   PTI_SensorInit
  brief:      Initializes modules for the sensing of the electrodes.
  param:      Void                           
  return:     Status code                              
  */

  extern UINT8 PTI_SampleElectrode(UINT8 u8ElecNum, UINT8 u8Command);
  /*
  function:   PTI_SampleElectrode
  brief:      Performs a valid reading of one electrode, stores the timer values and returns a status code
  param:      u8ElecNum - Number of electrode to be scanned
              u8Command - Measurement command
  return:     Status code                              
  */

  /*********************************************************************************
   *    Determination what PTI module is used for later conditional PTI module 
   *    initialization
   *********************************************************************************/

  #define TSS_PTI_USED           (TSS_DETECT_EL_MODULE(0,PTI) || TSS_DETECT_EL_MODULE(1,PTI) || TSS_DETECT_EL_MODULE(2,PTI) || TSS_DETECT_EL_MODULE(3,PTI) || TSS_DETECT_EL_MODULE(4,PTI) || TSS_DETECT_EL_MODULE(5,PTI) || TSS_DETECT_EL_MODULE(6,PTI) || TSS_DETECT_EL_MODULE(7,PTI) || TSS_DETECT_EL_MODULE(8,PTI) || TSS_DETECT_EL_MODULE(9,PTI) || TSS_DETECT_EL_MODULE(10,PTI) || TSS_DETECT_EL_MODULE(11,PTI) || TSS_DETECT_EL_MODULE(12,PTI) || TSS_DETECT_EL_MODULE(13,PTI) || TSS_DETECT_EL_MODULE(14,PTI) || TSS_DETECT_EL_MODULE(15,PTI) || TSS_DETECT_EL_MODULE(16,PTI) || TSS_DETECT_EL_MODULE(17,PTI) || TSS_DETECT_EL_MODULE(18,PTI) || TSS_DETECT_EL_MODULE(19,PTI) || TSS_DETECT_EL_MODULE(20,PTI) || TSS_DETECT_EL_MODULE(21,PTI) || TSS_DETECT_EL_MODULE(22,PTI) || TSS_DETECT_EL_MODULE(23,PTI) || TSS_DETECT_EL_MODULE(24,PTI) || TSS_DETECT_EL_MODULE(25,PTI) || TSS_DETECT_EL_MODULE(26,PTI) || TSS_DETECT_EL_MODULE(27,PTI) || TSS_DETECT_EL_MODULE(28,PTI) || TSS_DETECT_EL_MODULE(29,PTI) || TSS_DETECT_EL_MODULE(30,PTI) || TSS_DETECT_EL_MODULE(31,PTI) || TSS_DETECT_EL_MODULE(32,PTI) || TSS_DETECT_EL_MODULE(33,PTI) || TSS_DETECT_EL_MODULE(34,PTI) || TSS_DETECT_EL_MODULE(35,PTI) || TSS_DETECT_EL_MODULE(36,PTI) || TSS_DETECT_EL_MODULE(37,PTI) || TSS_DETECT_EL_MODULE(38,PTI) || TSS_DETECT_EL_MODULE(39,PTI) || TSS_DETECT_EL_MODULE(40,PTI) || TSS_DETECT_EL_MODULE(41,PTI) || TSS_DETECT_EL_MODULE(42,PTI) || TSS_DETECT_EL_MODULE(43,PTI) || TSS_DETECT_EL_MODULE(44,PTI) || TSS_DETECT_EL_MODULE(45,PTI) || TSS_DETECT_EL_MODULE(46,PTI) || TSS_DETECT_EL_MODULE(47,PTI) || TSS_DETECT_EL_MODULE(48,PTI) || TSS_DETECT_EL_MODULE(49,PTI) || TSS_DETECT_EL_MODULE(50,PTI) || TSS_DETECT_EL_MODULE(51,PTI) || TSS_DETECT_EL_MODULE(52,PTI) || TSS_DETECT_EL_MODULE(53,PTI) || TSS_DETECT_EL_MODULE(54,PTI) || TSS_DETECT_EL_MODULE(55,PTI) || TSS_DETECT_EL_MODULE(56,PTI) || TSS_DETECT_EL_MODULE(57,PTI) || TSS_DETECT_EL_MODULE(58,PTI) || TSS_DETECT_EL_MODULE(59,PTI) || TSS_DETECT_EL_MODULE(60,PTI) || TSS_DETECT_EL_MODULE(61,PTI) || TSS_DETECT_EL_MODULE(62,PTI) || TSS_DETECT_EL_MODULE(63,PTI))

  #define TSS_PTI_METHOD_USED    (TSS_DETECT_EL_METHOD(0,PTI) || TSS_DETECT_EL_METHOD(1,PTI) || TSS_DETECT_EL_METHOD(2,PTI) || TSS_DETECT_EL_METHOD(3,PTI) || TSS_DETECT_EL_METHOD(4,PTI) || TSS_DETECT_EL_METHOD(5,PTI) || TSS_DETECT_EL_METHOD(6,PTI) || TSS_DETECT_EL_METHOD(7,PTI) || TSS_DETECT_EL_METHOD(8,PTI) || TSS_DETECT_EL_METHOD(9,PTI) || TSS_DETECT_EL_METHOD(10,PTI) || TSS_DETECT_EL_METHOD(11,PTI) || TSS_DETECT_EL_METHOD(12,PTI) || TSS_DETECT_EL_METHOD(13,PTI) || TSS_DETECT_EL_METHOD(14,PTI) || TSS_DETECT_EL_METHOD(15,PTI) || TSS_DETECT_EL_METHOD(16,PTI) || TSS_DETECT_EL_METHOD(17,PTI) || TSS_DETECT_EL_METHOD(18,PTI) || TSS_DETECT_EL_METHOD(19,PTI) || TSS_DETECT_EL_METHOD(20,PTI) || TSS_DETECT_EL_METHOD(21,PTI) || TSS_DETECT_EL_METHOD(22,PTI) || TSS_DETECT_EL_METHOD(23,PTI) || TSS_DETECT_EL_METHOD(24,PTI) || TSS_DETECT_EL_METHOD(25,PTI) || TSS_DETECT_EL_METHOD(26,PTI) || TSS_DETECT_EL_METHOD(27,PTI) || TSS_DETECT_EL_METHOD(28,PTI) || TSS_DETECT_EL_METHOD(29,PTI) || TSS_DETECT_EL_METHOD(30,PTI) || TSS_DETECT_EL_METHOD(31,PTI) || TSS_DETECT_EL_METHOD(32,PTI) || TSS_DETECT_EL_METHOD(33,PTI) || TSS_DETECT_EL_METHOD(34,PTI) || TSS_DETECT_EL_METHOD(35,PTI) || TSS_DETECT_EL_METHOD(36,PTI) || TSS_DETECT_EL_METHOD(37,PTI) || TSS_DETECT_EL_METHOD(38,PTI) || TSS_DETECT_EL_METHOD(39,PTI) || TSS_DETECT_EL_METHOD(40,PTI) || TSS_DETECT_EL_METHOD(41,PTI) || TSS_DETECT_EL_METHOD(42,PTI) || TSS_DETECT_EL_METHOD(43,PTI) || TSS_DETECT_EL_METHOD(44,PTI) || TSS_DETECT_EL_METHOD(45,PTI) || TSS_DETECT_EL_METHOD(46,PTI) || TSS_DETECT_EL_METHOD(47,PTI) || TSS_DETECT_EL_METHOD(48,PTI) || TSS_DETECT_EL_METHOD(49,PTI) || TSS_DETECT_EL_METHOD(50,PTI) || TSS_DETECT_EL_METHOD(51,PTI) || TSS_DETECT_EL_METHOD(52,PTI) || TSS_DETECT_EL_METHOD(53,PTI) || TSS_DETECT_EL_METHOD(54,PTI) || TSS_DETECT_EL_METHOD(55,PTI) || TSS_DETECT_EL_METHOD(56,PTI) || TSS_DETECT_EL_METHOD(57,PTI) || TSS_DETECT_EL_METHOD(58,PTI) || TSS_DETECT_EL_METHOD(59,PTI) || TSS_DETECT_EL_METHOD(60,PTI) || TSS_DETECT_EL_METHOD(61,PTI) || TSS_DETECT_EL_METHOD(62,PTI) || TSS_DETECT_EL_METHOD(63,PTI))

  /**************** ROM & RAM Data prototypes ********************/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E0_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E0_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E1_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E1_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E2_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E2_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E3_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E3_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E4_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E4_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E5_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E5_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E6_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E6_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E7_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E7_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E8_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E8_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E9_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E9_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E10_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E10_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E11_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E11_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E12_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E12_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E13_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E13_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E14_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E14_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E15_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E15_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E16_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E16_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E17_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E17_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E18_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E18_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E19_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E19_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E20_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E20_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E21_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E21_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E22_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E22_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E23_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E23_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E24_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E24_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E25_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E25_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E26_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E26_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E27_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E27_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E28_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E28_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E29_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E29_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E30_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E30_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E31_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E31_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E32_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E32_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E33_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E33_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E34_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E34_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E35_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E35_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E36_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E36_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E37_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E37_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E38_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E38_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E39_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E39_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E40_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E40_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E41_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E41_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E42_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E42_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E43_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E43_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E44_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E44_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E45_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E45_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E46_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E46_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E47_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E47_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E48_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E48_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E49_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E49_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E50_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E50_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E51_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E51_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E52_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E52_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E53_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E53_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E54_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E54_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E55_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E55_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E56_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E56_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E57_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E57_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E58_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E58_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E59_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E59_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E60_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E60_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E61_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E61_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E62_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E62_RAMDATA_CONTEXT;
    #endif
  #endif

  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,PTI))
      extern const TSS_PTI_ELECTRODE_ROMDATA TSS_E63_ROMDATA_CONTEXT;
      extern       TSS_PTI_ELECTRODE_RAMDATA TSS_E63_RAMDATA_CONTEXT;
    #endif
  #endif

  /*******************************************************
  ******        Determine what is needed             *****
  *******************************************************/

  #if TSS_DETECT_METHOD(PTI)
    /* HW Timer */  
    #ifndef TSS_HW_TIMER_USED
      #define TSS_HW_TIMER_USED      1
    #else
      #undef TSS_HW_TIMER_USED
      #define TSS_HW_TIMER_USED      1
    #endif
    /* GPIO */        
    #ifndef TSS_HW_GPIO_USED
      #define TSS_HW_GPIO_USED       1
    #else
      #undef TSS_HW_GPIO_USED
      #define TSS_HW_GPIO_USED       1
    #endif  
  #endif  
      
  /* 
  * Handler Body of the measurement method 
  */
  #if TSS_HCS08_MCU || TSS_CFV1_MCU

    #define TSS_PTI_SAMPLE_ELECTRODE_LOW(el)  TSS_SAVE_ELECTRODE_PORT(el);          /* Save the electrode port */\
                                              TSS_ELECTRODE_DISCHARGE(el);\
                                              TSS_HW_TIMER_START();\
                                              TSS_PTI_PIN_ENABLE(el);\
                                              TSS_GOTO_WAIT();\
                                              TSS_ELECTRODE_CHARGE(el);             /* Charges the required electrode for high state */\
                                              TSS_PTI_PIN_DISABLE(el);\
                                              TSS_HW_TIMER_STOP();                  /* For sure if KBI doesn't appear */\
                                              TSS_ELECTRODE_DEFAULT(el);\
                                              u16Result =  TSS_HW_TIMER_GET_CNT();

  #elif TSS_KINETIS_MCU || TSS_CFM_MCU      

    #define TSS_PTI_SAMPLE_ELECTRODE_LOW(el)    ;    /* Nothing */

  #else /* End of TSS_KINETIS_MCU || TSS_CFM_MCU */
    #error "TSS - Not supported MCU used"  
  #endif    

  /*********** Sample Electrode Low Level Routine assignment ******/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,PTI))
      #define TSS_E0_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(0)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,PTI))
      #define TSS_E1_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(1)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,PTI))
      #define TSS_E2_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(2)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,PTI))
      #define TSS_E3_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(3)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,PTI))
      #define TSS_E4_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(4)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,PTI))
      #define TSS_E5_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(5)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,PTI))
      #define TSS_E6_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(6)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,PTI))
      #define TSS_E7_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(7)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,PTI))
      #define TSS_E8_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(8)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,PTI))
      #define TSS_E9_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(9)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,PTI))
      #define TSS_E10_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(10)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,PTI))
      #define TSS_E11_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(11)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,PTI))
      #define TSS_E12_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(12)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,PTI))
      #define TSS_E13_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(13)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,PTI))
      #define TSS_E14_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(14)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,PTI))
      #define TSS_E15_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(15)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,PTI))
      #define TSS_E16_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(16)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,PTI))
      #define TSS_E17_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(17)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,PTI))
      #define TSS_E18_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(18)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,PTI))
      #define TSS_E19_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(19)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,PTI))
      #define TSS_E20_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(20)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,PTI))
      #define TSS_E21_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(21)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,PTI))
      #define TSS_E22_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(22)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,PTI))
      #define TSS_E23_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(23)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,PTI))
      #define TSS_E24_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(24)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,PTI))
      #define TSS_E25_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(25)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,PTI))
      #define TSS_E26_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(26)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,PTI))
      #define TSS_E27_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(27)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,PTI))
      #define TSS_E28_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(28)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,PTI))
      #define TSS_E29_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(29)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,PTI))
      #define TSS_E30_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(30)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,PTI))
      #define TSS_E31_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(31)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,PTI))
      #define TSS_E32_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(32)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,PTI))
      #define TSS_E33_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(33)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,PTI))
      #define TSS_E34_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(34)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,PTI))
      #define TSS_E35_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(35)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,PTI))
      #define TSS_E36_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(36)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,PTI))
      #define TSS_E37_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(37)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,PTI))
      #define TSS_E38_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(38)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,PTI))
      #define TSS_E39_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(39)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,PTI))
      #define TSS_E40_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(40)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,PTI))
      #define TSS_E41_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(41)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,PTI))
      #define TSS_E42_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(42)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,PTI))
      #define TSS_E43_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(43)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,PTI))
      #define TSS_E44_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(44)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,PTI))
      #define TSS_E45_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(45)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,PTI))
      #define TSS_E46_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(46)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,PTI))
      #define TSS_E47_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(47)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,PTI))
      #define TSS_E48_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(48)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,PTI))
      #define TSS_E49_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(49)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,PTI))
      #define TSS_E50_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(50)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,PTI))
      #define TSS_E51_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(51)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,PTI))
      #define TSS_E52_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(52)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,PTI))
      #define TSS_E53_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(53)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,PTI))
      #define TSS_E54_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(54)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,PTI))
      #define TSS_E55_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(55)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,PTI))
      #define TSS_E56_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(56)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,PTI))
      #define TSS_E57_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(57)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,PTI))
      #define TSS_E58_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(58)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,PTI))
      #define TSS_E59_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(59)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,PTI))
      #define TSS_E60_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(60)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,PTI))
      #define TSS_E61_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(61)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,PTI))
      #define TSS_E62_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(62)
    #endif
  #endif
  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,PTI))
      #define TSS_E63_SAMPLE_ELECTRODE_LOW()    TSS_PTI_SAMPLE_ELECTRODE_LOW(63)
    #endif
  #endif

  /**************** Sample Electrode Routine assignment ***********/

  #if (TSS_KINETIS_MCU == 0) && (TSS_CFM_MCU == 0)
    #define PTI_SampleElectrode            UNI_SampleElectrode
  #endif 

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,PTI))
      #define TSS_E0_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,PTI))
      #define TSS_E1_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,PTI))
      #define TSS_E2_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,PTI))
      #define TSS_E3_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,PTI))
      #define TSS_E4_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,PTI))
      #define TSS_E5_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,PTI))
      #define TSS_E6_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,PTI))
      #define TSS_E7_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,PTI))
      #define TSS_E8_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,PTI))
      #define TSS_E9_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,PTI))
      #define TSS_E10_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,PTI))
      #define TSS_E11_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,PTI))
      #define TSS_E12_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,PTI))
      #define TSS_E13_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,PTI))
      #define TSS_E14_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,PTI))
      #define TSS_E15_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,PTI))
      #define TSS_E16_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,PTI))
      #define TSS_E17_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,PTI))
      #define TSS_E18_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,PTI))
      #define TSS_E19_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,PTI))
      #define TSS_E20_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,PTI))
      #define TSS_E21_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,PTI))
      #define TSS_E22_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,PTI))
      #define TSS_E23_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,PTI))
      #define TSS_E24_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,PTI))
      #define TSS_E25_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,PTI))
      #define TSS_E26_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,PTI))
      #define TSS_E27_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,PTI))
      #define TSS_E28_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,PTI))
      #define TSS_E29_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,PTI))
      #define TSS_E30_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,PTI))
      #define TSS_E31_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,PTI))
      #define TSS_E32_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,PTI))
      #define TSS_E33_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,PTI))
      #define TSS_E34_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,PTI))
      #define TSS_E35_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,PTI))
      #define TSS_E36_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,PTI))
      #define TSS_E37_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,PTI))
      #define TSS_E38_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,PTI))
      #define TSS_E39_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,PTI))
      #define TSS_E40_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,PTI))
      #define TSS_E41_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,PTI))
      #define TSS_E42_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,PTI))
      #define TSS_E43_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,PTI))
      #define TSS_E44_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,PTI))
      #define TSS_E45_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,PTI))
      #define TSS_E46_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,PTI))
      #define TSS_E47_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,PTI))
      #define TSS_E48_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,PTI))
      #define TSS_E49_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,PTI))
      #define TSS_E50_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,PTI))
      #define TSS_E51_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,PTI))
      #define TSS_E52_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,PTI))
      #define TSS_E53_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,PTI))
      #define TSS_E54_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,PTI))
      #define TSS_E55_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,PTI))
      #define TSS_E56_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,PTI))
      #define TSS_E57_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,PTI))
      #define TSS_E58_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,PTI))
      #define TSS_E59_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,PTI))
      #define TSS_E60_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,PTI))
      #define TSS_E61_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,PTI))
      #define TSS_E62_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,PTI))
      #define TSS_E63_SAMPLE_ELECTRODE()    PTI_SampleElectrode
    #endif
  #endif
      
  /************** TSS_SystemSetup.h Settings validation ***************/

  #if TSS_N_ELECTRODES > 0
    #if (TSS_DETECT_EL_METHOD(0,PTI))
      #ifndef TSS_E0_P
        #error TSS - TSS_E0_P is not defined
      #endif
      #ifndef TSS_E0_B
        #error TSS - TSS_E0_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 1
    #if (TSS_DETECT_EL_METHOD(1,PTI))
      #ifndef TSS_E1_P
        #error TSS - TSS_E1_P is not defined
      #endif
      #ifndef TSS_E1_B
        #error TSS - TSS_E1_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 2
    #if (TSS_DETECT_EL_METHOD(2,PTI))
      #ifndef TSS_E2_P
        #error TSS - TSS_E2_P is not defined
      #endif
      #ifndef TSS_E2_B
        #error TSS - TSS_E2_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 3
    #if (TSS_DETECT_EL_METHOD(3,PTI))
      #ifndef TSS_E3_P
        #error TSS - TSS_E3_P is not defined
      #endif
      #ifndef TSS_E3_B
        #error TSS - TSS_E3_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 4
    #if (TSS_DETECT_EL_METHOD(4,PTI))
      #ifndef TSS_E4_P
        #error TSS - TSS_E4_P is not defined
      #endif
      #ifndef TSS_E4_B
        #error TSS - TSS_E4_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 5
    #if (TSS_DETECT_EL_METHOD(5,PTI))
      #ifndef TSS_E5_P
        #error TSS - TSS_E5_P is not defined
      #endif
      #ifndef TSS_E5_B
        #error TSS - TSS_E5_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 6
    #if (TSS_DETECT_EL_METHOD(6,PTI))
      #ifndef TSS_E6_P
        #error TSS - TSS_E6_P is not defined
      #endif
      #ifndef TSS_E6_B
        #error TSS - TSS_E6_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 7
    #if (TSS_DETECT_EL_METHOD(7,PTI))
      #ifndef TSS_E7_P
        #error TSS - TSS_E7_P is not defined
      #endif
      #ifndef TSS_E7_B
        #error TSS - TSS_E7_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 8
    #if (TSS_DETECT_EL_METHOD(8,PTI))
      #ifndef TSS_E8_P
        #error TSS - TSS_E8_P is not defined
      #endif
      #ifndef TSS_E8_B
        #error TSS - TSS_E8_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 9
    #if (TSS_DETECT_EL_METHOD(9,PTI))
      #ifndef TSS_E9_P
        #error TSS - TSS_E9_P is not defined
      #endif
      #ifndef TSS_E9_B
        #error TSS - TSS_E9_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 10
    #if (TSS_DETECT_EL_METHOD(10,PTI))
      #ifndef TSS_E10_P
        #error TSS - TSS_E10_P is not defined
      #endif
      #ifndef TSS_E10_B
        #error TSS - TSS_E10_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 11
    #if (TSS_DETECT_EL_METHOD(11,PTI))
      #ifndef TSS_E11_P
        #error TSS - TSS_E11_P is not defined
      #endif
      #ifndef TSS_E11_B
        #error TSS - TSS_E11_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 12
    #if (TSS_DETECT_EL_METHOD(12,PTI))
      #ifndef TSS_E12_P
        #error TSS - TSS_E12_P is not defined
      #endif
      #ifndef TSS_E12_B
        #error TSS - TSS_E12_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 13
    #if (TSS_DETECT_EL_METHOD(13,PTI))
      #ifndef TSS_E13_P
        #error TSS - TSS_E13_P is not defined
      #endif
      #ifndef TSS_E13_B
        #error TSS - TSS_E13_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 14
    #if (TSS_DETECT_EL_METHOD(14,PTI))
      #ifndef TSS_E14_P
        #error TSS - TSS_E14_P is not defined
      #endif
      #ifndef TSS_E14_B
        #error TSS - TSS_E14_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 15
    #if (TSS_DETECT_EL_METHOD(15,PTI))
      #ifndef TSS_E15_P
        #error TSS - TSS_E15_P is not defined
      #endif
      #ifndef TSS_E15_B
        #error TSS - TSS_E15_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 16
    #if (TSS_DETECT_EL_METHOD(16,PTI))
      #ifndef TSS_E16_P
        #error TSS - TSS_E16_P is not defined
      #endif
      #ifndef TSS_E16_B
        #error TSS - TSS_E16_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 17
    #if (TSS_DETECT_EL_METHOD(17,PTI))
      #ifndef TSS_E17_P
        #error TSS - TSS_E17_P is not defined
      #endif
      #ifndef TSS_E17_B
        #error TSS - TSS_E17_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 18
    #if (TSS_DETECT_EL_METHOD(18,PTI))
      #ifndef TSS_E18_P
        #error TSS - TSS_E18_P is not defined
      #endif
      #ifndef TSS_E18_B
        #error TSS - TSS_E18_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 19
    #if (TSS_DETECT_EL_METHOD(19,PTI))
      #ifndef TSS_E19_P
        #error TSS - TSS_E19_P is not defined
      #endif
      #ifndef TSS_E19_B
        #error TSS - TSS_E19_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 20
    #if (TSS_DETECT_EL_METHOD(20,PTI))
      #ifndef TSS_E20_P
        #error TSS - TSS_E20_P is not defined
      #endif
      #ifndef TSS_E20_B
        #error TSS - TSS_E20_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 21
    #if (TSS_DETECT_EL_METHOD(21,PTI))
      #ifndef TSS_E21_P
        #error TSS - TSS_E21_P is not defined
      #endif
      #ifndef TSS_E21_B
        #error TSS - TSS_E21_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 22
    #if (TSS_DETECT_EL_METHOD(22,PTI))
      #ifndef TSS_E22_P
        #error TSS - TSS_E22_P is not defined
      #endif
      #ifndef TSS_E22_B
        #error TSS - TSS_E22_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 23
    #if (TSS_DETECT_EL_METHOD(23,PTI))
      #ifndef TSS_E23_P
        #error TSS - TSS_E23_P is not defined
      #endif
      #ifndef TSS_E23_B
        #error TSS - TSS_E23_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 24
    #if (TSS_DETECT_EL_METHOD(24,PTI))
      #ifndef TSS_E24_P
        #error TSS - TSS_E24_P is not defined
      #endif
      #ifndef TSS_E24_B
        #error TSS - TSS_E24_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 25
    #if (TSS_DETECT_EL_METHOD(25,PTI))
      #ifndef TSS_E25_P
        #error TSS - TSS_E25_P is not defined
      #endif
      #ifndef TSS_E25_B
        #error TSS - TSS_E25_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 26
    #if (TSS_DETECT_EL_METHOD(26,PTI))
      #ifndef TSS_E26_P
        #error TSS - TSS_E26_P is not defined
      #endif
      #ifndef TSS_E26_B
        #error TSS - TSS_E26_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 27
    #if (TSS_DETECT_EL_METHOD(27,PTI))
      #ifndef TSS_E27_P
        #error TSS - TSS_E27_P is not defined
      #endif
      #ifndef TSS_E27_B
        #error TSS - TSS_E27_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 28
    #if (TSS_DETECT_EL_METHOD(28,PTI))
      #ifndef TSS_E28_P
        #error TSS - TSS_E28_P is not defined
      #endif
      #ifndef TSS_E28_B
        #error TSS - TSS_E28_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 29
    #if (TSS_DETECT_EL_METHOD(29,PTI))
      #ifndef TSS_E29_P
        #error TSS - TSS_E29_P is not defined
      #endif
      #ifndef TSS_E29_B
        #error TSS - TSS_E29_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 30
    #if (TSS_DETECT_EL_METHOD(30,PTI))
      #ifndef TSS_E30_P
        #error TSS - TSS_E30_P is not defined
      #endif
      #ifndef TSS_E30_B
        #error TSS - TSS_E30_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 31
    #if (TSS_DETECT_EL_METHOD(31,PTI))
      #ifndef TSS_E31_P
        #error TSS - TSS_E31_P is not defined
      #endif
      #ifndef TSS_E31_B
        #error TSS - TSS_E31_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 32
    #if (TSS_DETECT_EL_METHOD(32,PTI))
      #ifndef TSS_E32_P
        #error TSS - TSS_E32_P is not defined
      #endif
      #ifndef TSS_E32_B
        #error TSS - TSS_E32_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 33
    #if (TSS_DETECT_EL_METHOD(33,PTI))
      #ifndef TSS_E33_P
        #error TSS - TSS_E33_P is not defined
      #endif
      #ifndef TSS_E33_B
        #error TSS - TSS_E33_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 34
    #if (TSS_DETECT_EL_METHOD(34,PTI))
      #ifndef TSS_E34_P
        #error TSS - TSS_E34_P is not defined
      #endif
      #ifndef TSS_E34_B
        #error TSS - TSS_E34_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 35
    #if (TSS_DETECT_EL_METHOD(35,PTI))
      #ifndef TSS_E35_P
        #error TSS - TSS_E35_P is not defined
      #endif
      #ifndef TSS_E35_B
        #error TSS - TSS_E35_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 36
    #if (TSS_DETECT_EL_METHOD(36,PTI))
      #ifndef TSS_E36_P
        #error TSS - TSS_E36_P is not defined
      #endif
      #ifndef TSS_E36_B
        #error TSS - TSS_E36_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 37
    #if (TSS_DETECT_EL_METHOD(37,PTI))
      #ifndef TSS_E37_P
        #error TSS - TSS_E37_P is not defined
      #endif
      #ifndef TSS_E37_B
        #error TSS - TSS_E37_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 38
    #if (TSS_DETECT_EL_METHOD(38,PTI))
      #ifndef TSS_E38_P
        #error TSS - TSS_E38_P is not defined
      #endif
      #ifndef TSS_E38_B
        #error TSS - TSS_E38_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 39
    #if (TSS_DETECT_EL_METHOD(39,PTI))
      #ifndef TSS_E39_P
        #error TSS - TSS_E39_P is not defined
      #endif
      #ifndef TSS_E39_B
        #error TSS - TSS_E39_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 40
    #if (TSS_DETECT_EL_METHOD(40,PTI))
      #ifndef TSS_E40_P
        #error TSS - TSS_E40_P is not defined
      #endif
      #ifndef TSS_E40_B
        #error TSS - TSS_E40_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 41
    #if (TSS_DETECT_EL_METHOD(41,PTI))
      #ifndef TSS_E41_P
        #error TSS - TSS_E41_P is not defined
      #endif
      #ifndef TSS_E41_B
        #error TSS - TSS_E41_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 42
    #if (TSS_DETECT_EL_METHOD(42,PTI))
      #ifndef TSS_E42_P
        #error TSS - TSS_E42_P is not defined
      #endif
      #ifndef TSS_E42_B
        #error TSS - TSS_E42_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 43
    #if (TSS_DETECT_EL_METHOD(43,PTI))
      #ifndef TSS_E43_P
        #error TSS - TSS_E43_P is not defined
      #endif
      #ifndef TSS_E43_B
        #error TSS - TSS_E43_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 44
    #if (TSS_DETECT_EL_METHOD(44,PTI))
      #ifndef TSS_E44_P
        #error TSS - TSS_E44_P is not defined
      #endif
      #ifndef TSS_E44_B
        #error TSS - TSS_E44_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 45
    #if (TSS_DETECT_EL_METHOD(45,PTI))
      #ifndef TSS_E45_P
        #error TSS - TSS_E45_P is not defined
      #endif
      #ifndef TSS_E45_B
        #error TSS - TSS_E45_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 46
    #if (TSS_DETECT_EL_METHOD(46,PTI))
      #ifndef TSS_E46_P
        #error TSS - TSS_E46_P is not defined
      #endif
      #ifndef TSS_E46_B
        #error TSS - TSS_E46_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 47
    #if (TSS_DETECT_EL_METHOD(47,PTI))
      #ifndef TSS_E47_P
        #error TSS - TSS_E47_P is not defined
      #endif
      #ifndef TSS_E47_B
        #error TSS - TSS_E47_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 48
    #if (TSS_DETECT_EL_METHOD(48,PTI))
      #ifndef TSS_E48_P
        #error TSS - TSS_E48_P is not defined
      #endif
      #ifndef TSS_E48_B
        #error TSS - TSS_E48_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 49
    #if (TSS_DETECT_EL_METHOD(49,PTI))
      #ifndef TSS_E49_P
        #error TSS - TSS_E49_P is not defined
      #endif
      #ifndef TSS_E49_B
        #error TSS - TSS_E49_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 50
    #if (TSS_DETECT_EL_METHOD(50,PTI))
      #ifndef TSS_E50_P
        #error TSS - TSS_E50_P is not defined
      #endif
      #ifndef TSS_E50_B
        #error TSS - TSS_E50_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 51
    #if (TSS_DETECT_EL_METHOD(51,PTI))
      #ifndef TSS_E51_P
        #error TSS - TSS_E51_P is not defined
      #endif
      #ifndef TSS_E51_B
        #error TSS - TSS_E51_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 52
    #if (TSS_DETECT_EL_METHOD(52,PTI))
      #ifndef TSS_E52_P
        #error TSS - TSS_E52_P is not defined
      #endif
      #ifndef TSS_E52_B
        #error TSS - TSS_E52_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 53
    #if (TSS_DETECT_EL_METHOD(53,PTI))
      #ifndef TSS_E53_P
        #error TSS - TSS_E53_P is not defined
      #endif
      #ifndef TSS_E53_B
        #error TSS - TSS_E53_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 54
    #if (TSS_DETECT_EL_METHOD(54,PTI))
      #ifndef TSS_E54_P
        #error TSS - TSS_E54_P is not defined
      #endif
      #ifndef TSS_E54_B
        #error TSS - TSS_E54_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 55
    #if (TSS_DETECT_EL_METHOD(55,PTI))
      #ifndef TSS_E55_P
        #error TSS - TSS_E55_P is not defined
      #endif
      #ifndef TSS_E55_B
        #error TSS - TSS_E55_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 56
    #if (TSS_DETECT_EL_METHOD(56,PTI))
      #ifndef TSS_E56_P
        #error TSS - TSS_E56_P is not defined
      #endif
      #ifndef TSS_E56_B
        #error TSS - TSS_E56_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 57
    #if (TSS_DETECT_EL_METHOD(57,PTI))
      #ifndef TSS_E57_P
        #error TSS - TSS_E57_P is not defined
      #endif
      #ifndef TSS_E57_B
        #error TSS - TSS_E57_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 58
    #if (TSS_DETECT_EL_METHOD(58,PTI))
      #ifndef TSS_E58_P
        #error TSS - TSS_E58_P is not defined
      #endif
      #ifndef TSS_E58_B
        #error TSS - TSS_E58_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 59
    #if (TSS_DETECT_EL_METHOD(59,PTI))
      #ifndef TSS_E59_P
        #error TSS - TSS_E59_P is not defined
      #endif
      #ifndef TSS_E59_B
        #error TSS - TSS_E59_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 60
    #if (TSS_DETECT_EL_METHOD(60,PTI))
      #ifndef TSS_E60_P
        #error TSS - TSS_E60_P is not defined
      #endif
      #ifndef TSS_E60_B
        #error TSS - TSS_E60_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 61
    #if (TSS_DETECT_EL_METHOD(61,PTI))
      #ifndef TSS_E61_P
        #error TSS - TSS_E61_P is not defined
      #endif
      #ifndef TSS_E61_B
        #error TSS - TSS_E61_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 62
    #if (TSS_DETECT_EL_METHOD(62,PTI))
      #ifndef TSS_E62_P
        #error TSS - TSS_E62_P is not defined
      #endif
      #ifndef TSS_E62_B
        #error TSS - TSS_E62_B is not defined
      #endif
    #endif
  #endif
  #if TSS_N_ELECTRODES > 63
    #if (TSS_DETECT_EL_METHOD(63,PTI))
      #ifndef TSS_E63_P
        #error TSS - TSS_E63_P is not defined
      #endif
      #ifndef TSS_E63_B
        #error TSS - TSS_E63_B is not defined
      #endif
    #endif
  #endif

  /* Warning if PE Micro will be influenced by the method */
  #if TSS_DETECT_METHOD(PTI)
     #if TSS_USE_FREEMASTER_GUI
        #if TSS_ENABLE_DIAGNOSTIC_MESSAGES
          #warning TSS - The connection with FreeMASTER GUI via PE Micro BDM Multilink may work not properly with PTI method.
        #endif
     #endif
  #endif
      
  /* Other Validation */
      
  #if TSS_DETECT_METHOD(PTI)
    #if (TSS_HCS08_MCU == 1)
      #if (TSS_GPIO_VERSION == 2)
        #error "TSS - PTI method is not supported on this MCU"
      #endif
    #endif
  #endif
      
  
  /*********************************************************************************
  *                                 PTI Control Macros
  *********************************************************************************/
  #define TSS_PTI_FALLING_EDGE              0x00U         /* Falling Edge/Low Level  */
  #define TSS_PTI_RISING_EDGE               0x01U         /* Rising Edge/High Level  */
    
  #define TSS_PTI_TO_PE(port, pin)          PT##port##PS_PT##port##PS##pin
  #define TSS_GET_PTI_PE(port, pin)         TSS_PTI_TO_PE(port, pin)
  #define TSS_PTI_PIN_CONV(port, pin)       TSS_GET_PTI_PE(port, pin)
      
  #define TSS_PTI_PIN_ENABLE(X)             (TSS_PTI_PIN_CONV(TSS_E##X##_P, TSS_E##X##_B) = 1u)
  #define TSS_PTI_PIN_DISABLE(X)            (TSS_PTI_PIN_CONV(TSS_E##X##_P, TSS_E##X##_B) = 0u)  
  
  #define TSS_PTI_TO_EDG(port, pin)         PT##port##ES_PT##port##ES##pin
  #define TSS_PTI_EDG_CONV(port, pin)       TSS_PTI_TO_EDG(port, pin)
  #define TSS_SET_PTI_EDG(X)                TSS_PTI_EDG_CONV(TSS_E##X##_P, TSS_E##X##_B)

  #define TSS_PTI_TO_IE(reg)                PT##reg##SC_PT##reg##IE
  #define TSS_PTI_ENABLE_INT(num)           (TSS_PTI_TO_IE(num) = 1u)
  
  #define TSS_PTI_TO_ACK(reg)               PT##reg##SC_PT##reg##ACK
  #define TSS_PTI_ACK(num)                  (TSS_PTI_TO_ACK(num) = 1u)

  /*******************************************************
  **** Indication of not supported MCUs and features ****
  *******************************************************/
  
  #if TSS_CFV1_MCU    
    #if (defined(_MCF51AG128_H) || defined(_MCF51AG96_H))
      #if (TSS_DETECT_PORT_METHOD(A,PTI) || TSS_DETECT_PORT_METHOD(B,PTI) || TSS_DETECT_PORT_METHOD(C,PTI) || TSS_DETECT_PORT_METHOD(D,PTI) || TSS_DETECT_PORT_METHOD(J,PTI))
        #error "TSI - Pin Interrupt method is not supported yet by TSS for MCF51AG family"
      #endif  
    #endif
  #endif

#endif /*__TSS_SENSORPTI_H__*/
