/***********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   TSS_API.h
*
* @brief  This header defines the structs, constants, Types and registers of the TSS library 
*
* @version 1.0.55.0
* 
* @date Mar-6-2012
* 
*
***********************************************************************************************************************/


#ifndef __TSS_API_H__
  #define __TSS_API_H__

  #include "TSS_SystemSetup.h"  
  #include "TSS_StatusCodes.h" /* Defines The Return Status Codes Used By The Library */
  #include "TSS_DataTypes.h"   /* Defines The Data Types */
  #include "TSS_Sensor.h"      /* Includes TSS Sensor */
  
  /*******************************************************
  ******************* Version Control ********************
  *******************************************************/
  #define __TSS__             2
  #define __TSS_MINOR__       6
  #define __TSS_PATCHLEVEL__  0

  /* Version = VVRRPP */
  #if defined(__TSS__)
  #if defined(__TSS_PATCHLEVEL__)
  #define __TSS_VERSION__ ( __TSS__ * 10000 \
                          + __TSS_MINOR__ *100 \
                          + __TSS_PATCHLEVEL__)
  #else
  #define __TSS_VERSION__ ( __TSS__ * 10000 \
                          + __TSS_MINOR__ *100 \
                          )
  #endif
  #endif 
  /*******************************************************
   ****************** Types Definitions ******************
   *******************************************************/

  /* Electrode Control Types */
  #define TSS_CT_KEYPAD               1u
  #define TSS_CT_SLIDER               2u
  #define TSS_CT_ROTARY               3u

  /*******************************************************
   *********************** Macros ************************
   *******************************************************/

  /* Keypad Decoder Macros */

  #define TSS_KEYPAD_BUFFER_READ(destvar,kpcsStruct)      destvar = (kpcsStruct).BufferPtr[(kpcsStruct).BufferReadIndex]; \
                                                          (kpcsStruct).BufferReadIndex = (UINT8) (((kpcsStruct).BufferReadIndex + 1) & 0x0Fu)
  #define TSS_KEYPAD_BUFFER_EMPTY(kpcsStruct)             ((kpcsStruct).BufferReadIndex == (kpcsStruct).BufferWriteIndex)

  /*******************************************************
   ************* Data structures enumerations ************
   *******************************************************/

  enum TSS_CSSystem_REGISTERS{
      System_Faults_Register = 0,
      System_SystemConfig_Register = 1,
      System_NSamples_Register = 4,
      System_DCTrackerRate_Register = 5,
      System_ResponseTime_Register = 6,
      System_StuckKeyTimeout_Register = 7,
      System_LowPowerScanPeriod_Register = 8,
      System_LowPowerElectrode_Register = 9,
      System_LowPowerElectrodeSensitivity_Register = 10,
      System_SystemTrigger_Register = 11,
      System_AutoTriggerModuloValue_Register = 12,      
      System_Sensitivity_Register = 13,                                                    
      System_ElectrodeEnablers_Register = System_Sensitivity_Register + TSS_N_ELECTRODES, 
      System_ElectrodeStatus_Register = System_ElectrodeEnablers_Register + (((TSS_N_ELECTRODES - 1) / 8) + 1),
      System_ConfigCheckSum_Register = System_ElectrodeStatus_Register + (((TSS_N_ELECTRODES - 1) / 8) + 1)
  };

  enum TSS_CSKeypad_REGISTERS{
      Keypad_ControlId_Register = 0,
      Keypad_ControlConfig_Register = 1,
      Keypad_BufferReadIndex_Register = 2,
      Keypad_BufferWriteIndex_Register = 3,
      Keypad_Events_Register = 4,
      Keypad_MaxTouches_Register = 5,
      Keypad_AutoRepeatRate_Register = 6,
      Keypad_AutoRepeatStart_Register = 7,
      Keypad_BufferPtr_Register = 8
  };

  enum TSS_CSSlider_REGISTERS{
      Slider_ControlId_Register = 0,
      Slider_ControlConfig_Register = 1,
      Slider_DynamicStatus_Register = 2,
      Slider_StaticStatus_Register = 3,
      Slider_Events_Register = 4,
      Slider_AutoRepeatRate_Register = 5,
      Slider_MovementTimeout_Register = 6
  };

  enum TSS_CSRotary_REGISTERS{
      Rotary_ControlId_Register = 0,
      Rotary_ControlConfig_Register = 1,
      Rotary_DynamicStatus_Register = 2,
      Rotary_StaticStatus_Register = 3,
      Rotary_Events_Register = 4,
      Rotary_AutoRepeatRate_Register = 5,
      Rotary_MovementTimeout_Register = 6
  };

  /*******************************************************
   *********** Library Constants Definitions *************
   *******************************************************/
   
  /* System Register Constants */
  #define TSS_SYSTEM_EN_BIT            7u     /* Bit 7 at SystemConfig */
  #define TSS_SWI_EN_BIT               6u     /* Bit 6 at SystemConfig */
  #define TSS_DC_TRACKER_EN_BIT        5u     /* Bit 5 at SystemConfig */
  #define TSS_STUCK_KEY_EN_BIT         4u     /* Bit 4 at SystemConfig */
  #define TSS_HW_RECA_START_BIT        3u     /* Bit 3 at SystemConfig */  
  #define TSS_LOWPOWER_EN_BIT          2u     /* Bit 2 at SystemConfig */
  #define TSS_SYSTEM_RESET_BIT         1u     /* Bit 1 at SystemConfig */
  #define TSS_MANUAL_RECA_START_BIT    0u     /* Bit 0 at SystemConfig */

  #define TSS_SWTRIGGER_BIT            2u     /* Bit 0-1 at SystemTrigger */
  #define TSS_TRIGGER_MODE_BIT         0u     /* Bit 2 at SystemTrigger */  
  
  #define TSS_SYSTEM_EN_MASK           ((UINT8)(1u << TSS_SYSTEM_EN_BIT))
  #define TSS_SWI_EN_MASK              ((UINT8)(1u << TSS_SWI_EN_BIT))
  #define TSS_DC_TRACKER_EN_MASK       ((UINT8)(1u << TSS_DC_TRACKER_EN_BIT))
  #define TSS_STUCK_KEY_EN_MASK        ((UINT8)(1u << TSS_STUCK_KEY_EN_BIT))
  #define TSS_HW_RECA_START_MASK       ((UINT8)(1u << TSS_HW_RECA_START_BIT))  
  #define TSS_LOWPOWER_EN_MASK         ((UINT8)(1u << TSS_LOWPOWER_EN_BIT))
  #define TSS_SYSTEM_RESET_MASK        ((UINT8)(1u << TSS_SYSTEM_RESET_BIT))
  #define TSS_MANUAL_RECA_START_MASK   ((UINT8)(1u << TSS_MANUAL_RECA_START_BIT))
  
  #define TSS_SWTRIGGER_MASK           ((UINT8)(1u << TSS_SWTRIGGER_BIT))
  #define TSS_TRIGGER_MODE_MASK        ((UINT8)(3u << TSS_TRIGGER_MODE_BIT))
  
  #define TSS_TRIGGER_MODE_AUTO        0u
  #define TSS_TRIGGER_MODE_ALWAYS      1u
  #define TSS_TRIGGER_MODE_SW          2u

  /* KeyPad Constants */
  #define TSS_KEYPAD_CONTROL_EN_BIT              7u     /* Bit 7 at Keypad Control Config */
  #define TSS_KEYPAD_CALLBACK_EN_BIT             6u     /* Bit 6 at Keypad Control Config */
  #define TSS_KEYPAD_IDLE_EN_BIT                 5u     /* Bit 5 at Keypad Control Config */

  #define TSS_KEYPAD_MAXTOUCHES_FLAG_BIT         7u     /* Bit 7 at Keypad Events */
  #define TSS_KEYPAD_BUFFER_OVF_FLAG_BIT         6u     /* Bit 6 at Keypad Events */
  #define TSS_KEYPAD_MAXTOUCHES_EN_BIT           4u     /* Bit 4 at Keypad Events */
  #define TSS_KEYPAD_BUFFER_OVF_EN_BIT           3u     /* Bit 3 at Keypad Events */
  #define TSS_KEYPAD_AUTOREPEAT_EN_BIT           2u     /* Bit 2 at Keypad Events */
  #define TSS_KEYPAD_RELEASE_EVENT_EN_BIT        1u     /* Bit 1 at Keypad Events */
  #define TSS_KEYPAD_TOUCH_EVENT_EN_BIT          0u     /* Bit 0 at Keypad Events */

  #define TSS_KEYPAD_CONTROL_EN_MASK             ((UINT8)(1u << TSS_KEYPAD_CONTROL_EN_BIT))
  #define TSS_KEYPAD_CALLBACK_EN_MASK            ((UINT8)(1u << TSS_KEYPAD_CALLBACK_EN_BIT))
  #define TSS_KEYPAD_IDLE_EN_MASK                ((UINT8)(1u << TSS_KEYPAD_IDLE_EN_BIT)) 

  #define TSS_KEYPAD_MAXTOUCHES_FLAG_MASK        ((UINT8)(1u << TSS_KEYPAD_MAXTOUCHES_FLAG_BIT))
  #define TSS_KEYPAD_BUFFER_OVF_FLAG_MASK        ((UINT8)(1u << TSS_KEYPAD_BUFFER_OVF_FLAG_BIT))
  #define TSS_KEYPAD_MAXTOUCHES_EN_MASK          ((UINT8)(1u << TSS_KEYPAD_MAXTOUCHES_EN_BIT))
  #define TSS_KEYPAD_BUFFER_OVF_EN_MASK          ((UINT8)(1u << TSS_KEYPAD_BUFFER_OVF_EN_BIT))
  #define TSS_KEYPAD_AUTOREPEAT_EN_MASK          ((UINT8)(1u << TSS_KEYPAD_AUTOREPEAT_EN_BIT))
  #define TSS_KEYPAD_RELEASE_EVENT_EN_MASK       ((UINT8)(1u << TSS_KEYPAD_RELEASE_EVENT_EN_BIT))
  #define TSS_KEYPAD_TOUCH_EVENT_EN_MASK         ((UINT8)(1u << TSS_KEYPAD_TOUCH_EVENT_EN_BIT))

  #define TSS_KEYPAD_BUFFER_EVENT_FLAG_BIT       7u

  #define TSS_KEYPAD_BUFFER_EVENT_FLAG_MASK      ((UINT8)(1u << TSS_KEYPAD_BUFFER_EVENT_FLAG_BIT))
  #define TSS_KEYPAD_BUFFER_RELEASE_FLAG         ((UINT8)(1u << TSS_KEYPAD_BUFFER_EVENT_FLAG_BIT))
  #define TSS_KEYPAD_BUFFER_TOUCH_FLAG           0
  
  #define TSS_KEYPAD_BUFFER_KEY_MASK             0x0F  
  
   /* Slider Constants */
  #define TSS_SLIDER_DYN_EN_BIT                   7u     /* Bit 7 at Slider Control Config */
  #define TSS_SLIDER_CALLBACK_EN_BIT              6u     /* Bit 6 at Slider Control Config */
  #define TSS_SLIDER_IDLE_SCAN                    5u     /* Bit 5 at Slider Control Config */

  #define TSS_SLIDER_RELEASE_EN_BIT               4u     /* Bit 4 at Slider Events */
  #define TSS_SLIDER_HOLD_AUTOREPEAT_EN_BIT       3u     /* Bit 3 at Slider Events */
  #define TSS_SLIDER_HOLD_EN_BIT                  2u     /* Bit 2 at Slider Events */
  #define TSS_SLIDER_MOVEMENT_EVENT_EN_BIT        1u     /* Bit 1 at Slider Events */
  #define TSS_SLIDER_INITIAL_TOUCH_EVENT_EN_BIT   0u     /* Bit 0 at Slider Events */

  #define TSS_SLIDER_DYN_EN_MASK                  ((UINT8)(1u << TSS_SLIDER_DYN_EN_BIT))
  #define TSS_SLIDER_CALLBACK_EN_MASK             ((UINT8)(1u << TSS_SLIDER_CALLBACK_EN_BIT))
  #define TSS_SLIDER_IDLE_EN_MASK                 ((UINT8)(1u << TSS_SLIDER_IDLE_SCAN)) 

  #define TSS_SLIDER_RELEASE_EVENT_EN_MASK        ((UINT8)(1u << TSS_SLIDER_RELEASE_EN_BIT))
  #define TSS_SLIDER_AUTOREPEAT_EN_MASK           ((UINT8)(1u << TSS_SLIDER_HOLD_AUTOREPEAT_EN_BIT))
  #define TSS_SLIDER_HOLD_EN_MASK                 ((UINT8)(1u << TSS_SLIDER_HOLD_EN_BIT))
  #define TSS_SLIDER_MOVEMENT_EVENT_EN_MASK       ((UINT8)(1u << TSS_SLIDER_MOVEMENT_EVENT_EN_BIT))
  #define TSS_SLIDER_INITIAL_TOUCH_EVENT_EN_MASK  ((UINT8)(1u << TSS_SLIDER_INITIAL_TOUCH_EVENT_EN_BIT))

   /* Rotary Constants */
  #define TSS_ROTARY_CONTROL_EN_BIT               7u     /* Bit 7 at Slider Control Config */
  #define TSS_ROTARY_CALLBACK_EN_BIT              6u     /* Bit 6 at Slider Control Config */
  #define TSS_ROTARY_IDLE_SCAN                    5u     /* Bit 5 at Slider Control Config */

  #define TSS_ROTARY_RELEASE_EN_BIT               4u     /* Bit 4 at Slider Events */
  #define TSS_ROTARY_HOLD_AUTOREPEAT_EN_BIT       3u     /* Bit 3 at Slider Events */
  #define TSS_ROTARY_HOLD_EN_BIT                  2u     /* Bit 2 at Slider Events */
  #define TSS_ROTARY_MOVEMENT_EVENT_EN_BIT        1u     /* Bit 1 at Slider Events */
  #define TSS_ROTARY_INITIAL_TOUCH_EVENT_EN_BIT   0u     /* Bit 0 at Slider Events */

  #define TSS_ROTARY_CONTROL_EN_MASK              ((UINT8)(1u << TSS_ROTARY_CONTROL_EN_BIT))
  #define TSS_ROTARY_CALLBACK_EN_MASK             ((UINT8)(1u << TSS_ROTARY_CALLBACK_EN_BIT))
  #define TSS_ROTARY_IDLE_EN_MASK                 ((UINT8)(1u << TSS_ROTARY_IDLE_SCAN)) 

  #define TSS_ROTARY_RELEASE_EVENT_EN_MASK        ((UINT8)(1u << TSS_ROTARY_RELEASE_EN_BIT))
  #define TSS_ROTARY_AUTOREPEAT_EN_MASK           ((UINT8)(1u << TSS_ROTARY_HOLD_AUTOREPEAT_EN_BIT))
  #define TSS_ROTARY_HOLD_EN_MASK                 ((UINT8)(1u << TSS_ROTARY_HOLD_EN_BIT))
  #define TSS_ROTARY_MOVEMENT_EVENT_EN_MASK       ((UINT8)(1u << TSS_ROTARY_MOVEMENT_EVENT_EN_BIT))
  #define TSS_ROTARY_INITIAL_TOUCH_EVENT_EN_MASK  ((UINT8)(1u << TSS_ROTARY_INITIAL_TOUCH_EVENT_EN_BIT))

  /* System Setup Constants */
  
  #define TSS_KEYPAD_EVENTS_BUFFER_SIZE            16u
  #define TSS_KEYPAD_CONTCONF_INIT                 {0u,0u,0u,0u}
  #define TSS_KEYPAD_EVENTS_INIT                   {0u,0u,0u,0u,0u,0u,0u}
  
  #define TSS_SLIDER_CONTROL_INIT                  {0u,0u,0u,0u}
  #define TSS_SLIDER_DYN_INIT                      {0u,0u,0u}
  #define TSS_SLIDER_STAT_INIT                     {0u,0u,0u}
  #define TSS_SLIDER_EVENTS_INIT                   {0u,0u,0u,0u,0u}
  
  #define TSS_ASLIDER_CONTROL_INIT                 {0u,0u,0u,0u}
  #define TSS_ASLIDER_DYN_INIT                     {0u,0u,0u}
  #define TSS_ASLIDER_EVENTS_INIT                  {0u,0u,0u,0u,0u,0u,0u}

  /*******************************************************
   ************* Data structures definitions *************
   *******************************************************/

  /***** Definition of CS structure's register types *****/
  #ifdef __HCS08__
    #pragma MESSAGE DISABLE C1106 /* WARNING C1106: Non-standard bitfield type */
  #endif
  /******* Definition of system's CS structure type ******/

  typedef struct{
      volatile const TSS_SYSTEM_FAULTS Faults;
      volatile const TSS_SYSTEM_SYSCONF SystemConfig;
      volatile const UINT16 Reserved;
      volatile const UINT8 NSamples;
      volatile const UINT8 DCTrackerRate;
      volatile const UINT8 ResponseTime;
      volatile const UINT8 StuckKeyTimeout;
      volatile const UINT8 LowPowerScanPeriod;         
      volatile const UINT8 LowPowerElectrode;    
      volatile const UINT8 LowPowerElectrodeSensitivity; 
      volatile const TSS_SYSTEM_TRIGGER SystemTrigger;
      volatile const UINT8 AutoTriggerModuloValue;    
  } TSS_CSSystem;

  /****** Definitions of decoder's CS structure types ****/

  /* Keypad CS structure */
  typedef struct {                                /* MAIN Struct for KeyPad Decoder */
      const TSS_CONTROL_ID ControlId;
      const TSS_KEYPAD_CONTCONF ControlConfig;
            UINT8 BufferReadIndex;                /* KeyPad Buffer Read Index     */
      const UINT8 BufferWriteIndex;               /* KeyPad Buffer Write Index    */
      const TSS_KEYPAD_EVENTS Events;
      const UINT8 MaxTouches;
      const UINT8 AutoRepeatRate;
      const UINT8 AutoRepeatStart;
      const UINT8 * const BufferPtr;              /* KeyPad Buffer pointer        */
  } TSS_CSKeypad;

  /* Slider CS structure */
  typedef struct{
      const TSS_CONTROL_ID ControlId;
      const TSS_SLIDER_CONTROL ControlConfig;
      const TSS_SLIDER_DYN DynamicStatus;
      const TSS_SLIDER_STAT StaticStatus;
      const TSS_SLIDER_EVENTS Events;
      const UINT8 AutoRepeatRate;
      const UINT8 MovementTimeout;
  } TSS_CSSlider;

  /* Public Rotary CS structure */
  typedef struct{
      const TSS_CONTROL_ID ControlId;
      const TSS_SLIDER_CONTROL ControlConfig;
      const TSS_SLIDER_DYN DynamicStatus;
      const TSS_SLIDER_STAT StaticStatus;
      const TSS_SLIDER_EVENTS Events;
      const UINT8 AutoRepeatRate;
      const UINT8 MovementTimeout;
  } TSS_CSRotary;

  /*******************************************************
   ********* Control's CS Structures Prototypes **********
   *******************************************************/
   
  /*
  *  This section includes the extern control structures that will ve used. 
  *  Only the Structures needed are compiled according with the number of declared
  *  controls in SystemSetup.h.
  */

  #if TSS_N_CONTROLS > 0                          /* Includes one extern control estructure per decoder declared */
      #if (TSS_C0_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C0_STRUCTURE;   /* If a KeyPad control is used includes the KeyPad struct*/
      #elif (TSS_C0_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C0_STRUCTURE;   /* If a Slider control is used includes the Slider struct*/
      #else
          extern TSS_CSRotary TSS_C0_STRUCTURE;   /* If a Rotary control is used incluldes the Rotary struct*/
      #endif
  #endif

  #if TSS_N_CONTROLS > 1
      #if (TSS_C1_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C1_STRUCTURE;
      #elif (TSS_C1_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C1_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C1_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 2
      #if (TSS_C2_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C2_STRUCTURE;
      #elif (TSS_C2_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C2_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C2_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 3
      #if (TSS_C3_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C3_STRUCTURE;
      #elif (TSS_C3_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C3_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C3_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 4
      #if (TSS_C4_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C4_STRUCTURE;
      #elif (TSS_C4_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C4_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C4_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 5
      #if (TSS_C5_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C5_STRUCTURE;
      #elif (TSS_C5_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C5_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C5_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 6
      #if (TSS_C6_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C6_STRUCTURE;
      #elif (TSS_C6_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C6_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C6_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 7
      #if (TSS_C7_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C7_STRUCTURE;
      #elif (TSS_C7_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C7_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C7_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 8
      #if (TSS_C8_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C8_STRUCTURE;
      #elif (TSS_C8_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C8_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C8_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 9
      #if (TSS_C9_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C9_STRUCTURE;
      #elif (TSS_C9_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C9_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C9_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 10
      #if (TSS_C10_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C10_STRUCTURE;
      #elif (TSS_C10_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C10_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C10_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 11
      #if (TSS_C11_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C11_STRUCTURE;
      #elif (TSS_C11_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C11_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C11_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 12
      #if (TSS_C12_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C12_STRUCTURE;
      #elif (TSS_C12_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C12_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C12_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 13
      #if (TSS_C13_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C13_STRUCTURE;
      #elif (TSS_C13_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C13_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C13_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 14
      #if (TSS_C14_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C14_STRUCTURE;
      #elif (TSS_C14_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C14_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C14_STRUCTURE;
      #endif
  #endif

  #if TSS_N_CONTROLS > 15
      #if (TSS_C15_TYPE == TSS_CT_KEYPAD)
          extern TSS_CSKeypad TSS_C15_STRUCTURE;
      #elif (TSS_C15_TYPE == TSS_CT_SLIDER)
          extern TSS_CSSlider TSS_C15_STRUCTURE;
      #else
          extern TSS_CSRotary TSS_C15_STRUCTURE;
      #endif
  #endif

  /*******************************************************
   ********** TSS Library Function Prototypes ************
   *******************************************************/
   
  UINT8 TSS_Task(void);
  /*
  function    TSS_Task
  brief:      Performs all the task related to the TSS library  
  param:      void                            
  return:     Status Code (TSS_STATUS_OK, TSS_STATUS_PROCESSING)                             
  */

  UINT8 TSS_TaskSeq(void);
  /*
  function    TSS_TaskSeq
  brief:      Performs all the task related to the TSS library  
  param:      void                            
  return:     Status Code (TSS_STATUS_OK, TSS_STATUS_PROCESSING)                             
  */

  void TSS_Init(void);
  /*
  function:   TSSInit   
  brief:      Initializes all the hardware required for the TSS library
  param:      void
  return:     void
  */

  UINT8 TSS_SetSystemConfig(UINT8 u8Parameter, UINT8 u8Value);
  /*
  function:   TSS_SetSystemConfig
  brief:      Configures one parameter of the TSS library
  param:      u8Parameter - Parameter that will be modified
              u8Value     - New value of the parameter                            
  return:     Status code                             
  */
  
  UINT8 TSS_GetSystemConfig(UINT8 u8Parameter);
  /*
  function:   TSS_GetSystemConfig
  brief:      Read one parameter of the proximity library
  param:      u8Parameter - Requested Parameter for reading
  return:     Desired result Value
  */

  UINT8 TSS_SetKeypadConfig(TSS_CONTROL_ID u8ControlId, UINT8 u8Parameter, UINT8 u8Value);
  /*
  function:   TSS_SetKeypadConfig 
  brief:      Configures one parameter of the KeyPad control
  param:      u8ControlId - Control identifier
              u8Parameter - Parameter that will be modified
              u8Value     - New value of the parameter                            
  return:     Status code                             
  */
  
  UINT8 TSS_GetKeypadConfig(TSS_CONTROL_ID u8CtrlId, UINT8 u8Parameter);
  /*
  function    TSS_SetKeypadConfig
  brief:      Configurates a parameter of a Keypad Decoder   
  param:      u8Ctrld     - Control Identifier
  param:      i8Parameter - Parameter that will be modified
  param:      u8Value     - New value                      
  return:     Read Value                             
  */ 

  UINT8 TSS_SetSliderConfig(TSS_CONTROL_ID u8ControlId, UINT8 u8Parameter, UINT8 u8Value);
  /*
  function:   TSS_SetSliderConfig
  brief:      Configures one parameter of the Slider control
  param:      u8ControlId - Control identifier
              u8Parameter - Parameter that will be modified
              u8Value     - New value of the parameter                            
  return:     Status code                              
  */
  
  UINT8 TSS_GetSliderConfig(TSS_CONTROL_ID u8CtrlId, UINT8 u8Parameter);
  /*
  function    TSS_GetSliderConfig
  brief:      Read a parameter of a Slider Decoder   
  param:      u8Ctrld     - Control Identifier
  param:      i8Parameter - Parameter that will be modified
  return:     Read Value                             
  */

  #define TSS_SetRotaryConfig(u8ControlId, u8Parameter, u8Value) TSS_SetSliderConfig(u8ControlId, u8Parameter, u8Value)
  /* The same function is used for Slider and Rotary configuration */

  #define TSS_GetRotaryConfig(u8ControlId, u8Parameter) TSS_GetSliderConfig(u8ControlId, u8Parameter)
  /* The same function is used for Slider and Rotary configuration */

  /**************** Back Compatibility Macros ****************/
  
  #define TSS_SystemConfig(u8Parameter, u8Value) TSS_SetSystemConfig(u8Parameter, u8Value)
  #define TSS_KeypadConfig(u8ControlId, u8Parameter, u8Value) TSS_SetKeypadConfig(u8ControlId, u8Parameter, u8Value)
  #define TSS_SliderConfig(u8ControlId, u8Parameter, u8Value) TSS_SetSliderConfig(u8ControlId, u8Parameter, u8Value)
  #define TSS_RotaryConfig(u8ControlId, u8Parameter, u8Value) TSS_SetRotaryConfig(u8ControlId, u8Parameter, u8Value)    
  
  /***********************************************************
   **************** Default Values definition ****************
   ***********************************************************/  

  #ifndef TSS_USE_PE_COMPONENT
    #define TSS_USE_PE_COMPONENT                 0
  #endif

  #ifndef TSS_USE_MQX
    #define TSS_USE_MQX                          0
  #endif
  
  #ifndef TSS_USE_IIR_FILTER
    #define TSS_USE_IIR_FILTER                   0
  #endif    

  #ifndef TSS_USE_DELTA_LOG
    #define TSS_USE_DELTA_LOG                    0
  #endif
  
  #ifndef TSS_USE_SIGNAL_LOG
    #define TSS_USE_SIGNAL_LOG                   0
  #endif
  
  #ifndef TSS_USE_DATA_CORRUPTION_CHECK
    #define TSS_USE_DATA_CORRUPTION_CHECK        1
  #endif

  #ifndef TSS_USE_FREEMASTER_GUI
     #define TSS_USE_FREEMASTER_GUI              0
  #endif

  #ifndef TSS_ENABLE_DIAGNOSTIC_MESSAGES
     #define TSS_ENABLE_DIAGNOSTIC_MESSAGES      0
  #endif

  #ifndef TSS_USE_CONTROL_PRIVATE_DATA
     #define TSS_USE_CONTROL_PRIVATE_DATA        0
  #endif

  #ifndef TSS_USE_SIGNAL_SHIELDING
     #define TSS_USE_SIGNAL_SHIELDING            0
  #endif
  
  #ifndef TSS_USE_STUCK_KEY
     #define TSS_USE_STUCK_KEY                   1
  #endif

  #ifndef TSS_USE_NEGATIVE_BASELINE_DROP
     #define TSS_USE_NEGATIVE_BASELINE_DROP      1
  #endif
  
  /***********************************************************
   ******* Extern Contants and Variables Declaration *********
   ***********************************************************/

  extern volatile UINT8 tss_u8SampleIntFlag;            /* Extern variable for Sample Interrupted Flag */
  extern const UINT8 * const tss_acpsCSStructs[];       /* Extern Constant Array to store the structures addresses */

  extern TSS_CSSystem tss_CSSys;                        /* System Control Extern Struct */
  extern UINT8 tss_au8Sensitivity[TSS_N_ELECTRODES];
  extern UINT8 tss_au8ElectrodeEnablers[((TSS_N_ELECTRODES - 1)/ 8) + 1];
  extern UINT8 tss_au8ElectrodeStatus[((TSS_N_ELECTRODES - 1)/ 8) + 1];

  #if TSS_USE_DELTA_LOG 
      extern INT8 tss_ai8InstantDelta[TSS_N_ELECTRODES];
  #endif
  
  #if TSS_USE_SIGNAL_LOG 
      extern UINT16 tss_au16InstantSignal[TSS_N_ELECTRODES];
  #endif

  #define TSS_SET_SAMPLE_INTERRUPTED()       tss_u8SampleIntFlag = 1u;
  
#endif /* __TSS_API_H__ */
