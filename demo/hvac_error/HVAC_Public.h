#ifndef __hvac_public_h__
#define __hvac_public_h__

/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
*************************************************************************** 
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: HVAC_Public.h$
* $Version : 3.0.6.0$
* $Date    : Jul-30-2009$
*
* Comments:
*
*   
*
*END************************************************************************/


// HVAC user interface
#define HVAC_TEMP_TOLERANCE   0    // in 1/10 degree 
#define HVAC_TEMP_SW_DELTA    5    // in 1/10 degree 

// ambient temperature change emulation
#define HVAC_TEMP_UPD_DELTA   1    // in 1/10 degree 
#define HVAC_TEMP_UPDATE_RATE 1    // in seconds

#define HVAC_TEMP_MINIMUM     0    // in Celsius
#define HVAC_TEMP_MAXIMUM     500  // in Celsius

typedef enum {
   HVAC_FAN_OUTPUT=0,
   HVAC_HEAT_OUTPUT,
   HVAC_COOL_OUTPUT,
   HVAC_ALIVE_OUTPUT,
   HVAC_MAX_OUTPUTS
} HVAC_Output_t;

typedef enum {
   HVAC_TEMP_UP_INPUT=0,
   HVAC_TEMP_DOWN_INPUT,
   HVAC_FAN_ON_INPUT,
   HVAC_HEAT_ON_INPUT,
   HVAC_AC_ON_INPUT,
   HVAC_MAX_INPUTS
} HVAC_Input_t;


typedef enum {
   HVAC_Off,
   HVAC_Cool,
   HVAC_Heat,
   HVAC_Auto
} HVAC_Mode_t;

typedef enum {
   Fan_Automatic,
   Fan_On
} FAN_Mode_t;

typedef enum {
   Fahrenheit,
   Celsius
} Temperature_Scale_t;

extern LWSEM_STRUCT Logging_init_sem;

extern void HVAC_SetDesiredTemperature(uint_32);
extern uint_32 HVAC_GetDesiredTemperature(void);

extern void HVAC_SetFanMode(FAN_Mode_t);
extern FAN_Mode_t HVAC_GetFanMode(void);

extern void HVAC_SetHVACMode(HVAC_Mode_t);
extern HVAC_Mode_t HVAC_GetHVACMode(void);
extern char_ptr HVAC_HVACModeName(HVAC_Mode_t mode);

extern uint_32 HVAC_ConvertCelsiusToDisplayTemp(uint_32 temp);
extern uint_32 HVAC_ConvertDisplayTempToCelsius(uint_32 display_temp);
extern void HVAC_SetTemperatureScale(Temperature_Scale_t);
extern Temperature_Scale_t HVAC_GetTemperatureScale(void);
extern char HVAC_GetTemperatureSymbol(void); 

extern uint_32 HVAC_GetAmbientTemperature(void);
extern uint_32 HVAC_GetActualTemperature(void);
extern void HVAC_ReadAmbientTemperature(void);

extern boolean HVAC_GetOutput(HVAC_Output_t);
extern char_ptr HVAC_GetOutputName(HVAC_Output_t);

extern void HVAC_LogCurrentState(void);



#endif
