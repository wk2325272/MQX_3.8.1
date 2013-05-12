/**********************************************************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************************************************//*!
*
* @file   TSS_DataTypes.h
*
* @brief  Header file that define Types, Structs Types and Constants of the TSS library
*
* @version 1.0.14.0
* 
* @date Jan-30-2012
* 
* These Types, Structs Types and Constants are used by the User Application Level and also internally in the Library
*
***********************************************************************************************************************/


#ifndef __TSS_DATA_TYPES_H__
  #define __TSS_DATA_TYPES_H__

  /********************************************************************************/
  /* Standard Definitions:                                                        */
  /* These defines allow for easier porting to other compilers. if porting change */
  /* these defines to the required values for the chosen compiler.                */
  /********************************************************************************/

  typedef unsigned char   UINT8;      /* unsigned 8 bit definition */
  typedef unsigned short  UINT16;     /* unsigned 16 bit definition */
  typedef unsigned long   UINT32;     /* unsigned 32 bit definition */
  typedef signed char     INT8;       /* signed 8 bit definition */
  typedef short           INT16;      /* signed 16 bit definition */
  typedef long int        INT32;      /* signed 32 bit definition */

  #ifndef NULL
    #define NULL          ((void *)0) /* NULL as a generic pointer */
  #endif
  
  #ifdef __HCS08__
    #pragma MESSAGE DISABLE C1106       /* WARNING C1106: Non-standard bitfield type */
  #endif

  /************************************************************************/
  /******************* Data Type structures definitions *******************/
  /************************************************************************/

  typedef struct {                    /* Struct for KeyPad Decoder */
                                      /* This struct is for bit-fields only */
      UINT8 ControlNumber     :4;
      UINT8 ControlType       :4;
  } TSS_CONTROL_ID;

  typedef struct {                    /* Struct for KeyPad Decoder */
      UINT8 IdleScanRate      :5;     /* This struct is for bit-fields only */
      UINT8 IdleEnabler       :1;
      UINT8 CallbackEn        :1;
      UINT8 ControlEn         :1;
  } TSS_KEYPAD_CONTCONF;

  typedef struct {                    /* Struct for KeyPad Decoder */
      UINT8 KeyNumber         :6;     /* This struct is for bit-fields only */
      UINT8                   :1;
      UINT8 Eventtype         :1;
  } TSS_KEYPAD_BUFFPTR;

  typedef struct {                    /* Struct for KeyPad Decoder */
      UINT8 IndexValue        :4;     /* This struct is for bit-fields only */
      UINT8                   :1;
      UINT8                   :1;
      UINT8                   :1;
      UINT8                   :1;
  } TSS_KEYPAD_BUFFIDX;

  typedef struct {                    /* Struct for KeyPad Decoder */
      UINT8 TouchEventEn      :1;     /* This struct is for bit-fields only */
      UINT8 ReleaseEventEn    :1;
      UINT8 AutoRepeatEventEn :1;
      UINT8 BufferFullOvfEn   :1;
      UINT8 KeysExceededEn    :1;
      UINT8                   :1;
      UINT8 BufferOvfFlag     :1;
      UINT8 MaxKeysFlag       :1;
  } TSS_KEYPAD_EVENTS;

  typedef struct {                    /* Struct for Slider Decoder */
      UINT8 IdleScanRate      :5;     /* This struct is for bit-fields only */
      UINT8 IdleEnabler       :1;
      UINT8 CallbackEnabler   :1;
      UINT8 ControlEnabler    :1;
  } TSS_SLIDER_CONTROL;

  typedef struct {                    /* Struct for Slider Decoder */
      UINT8 Displacement      :4;     /* This struct is for bit-fields only */
      UINT8                   :1;
      UINT8                   :1;
      UINT8 Direction         :1;
      UINT8 Movement          :1;
  } TSS_SLIDER_DYN;

  typedef struct {                    /* Struct for Slider Decoder */
      UINT8 Position          :5;     /* This struct is for bit-fields only */
      UINT8                   :1;
      UINT8 InvalidPos        :1;
      UINT8 Touch             :1;
  } TSS_SLIDER_STAT;

  typedef struct{                     /* Struct for Slider Decoder */
      UINT8 InitialTouchEn    :1;     /* This struct is for bit-fields only */
      UINT8 MovementEn        :1;
      UINT8 HoldEn            :1;
      UINT8 AutoRepeatEn      :1;
      UINT8 ReleaseEnabler    :1;
      UINT8                   :1;
      UINT8                   :1;
      UINT8                   :1;
  } TSS_SLIDER_EVENTS;

  typedef struct {                    /* Struct for System Setup */
      UINT8 ChargeTimeout      :1;     /* This struct is for bit-fields only */
      UINT8 SmallCapacitor     :1;
      UINT8 DataCorruption     :1;
      UINT8 SmallTriggerPeriod :1;
      UINT8                    :1;
      UINT8                    :1;
      UINT8                    :1;
      UINT8                    :1;
  } TSS_SYSTEM_FAULTS;

  typedef struct {                    /* Struct for System Setup */
      UINT8 ManRecalStarter   :1;     /* This struct is for bit-fields only */
      UINT8 SystemReset       :1;
      UINT8 LowPowerEn        :1;
      UINT8 HWRecalStarter    :1;
      UINT8 StuckKeyEn        :1;
      UINT8 DCTrackerEn       :1;
      UINT8 SWIEn             :1;
      UINT8 SystemEn          :1;
  } TSS_SYSTEM_SYSCONF;
  
    typedef struct {                  /* Struct for Triggering setup */
      UINT8 TriggerMode       :2;
      UINT8 SWTrigger         :1;
      UINT8 Reserved          :5;
  } TSS_SYSTEM_TRIGGER;  

  /******************************************************************/
  /****** Definition of control's context data for each decoder *****/
  /******************************************************************/

  /* Struct for the context of a KeyPad Context*/
  typedef struct{
      INT8  i8KeyCount;
      UINT8 u8LastTouch;
      UINT8 u8NextBufElement;
      UINT8 u8State;
  } TSS_KeypadContext;

  typedef struct{                     /* Struct for a single variable*/
      UINT8 u8SingleVal;
  } TSS_SingleContext;

  /********************************************************************/

#endif /* __TSS_DATA_TYPES_H__*/
