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
* $FileName: Switch_Task.c$
* $Version : 3.8.11.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/

#include "hvac.h"
#include "hvac_public.h"
#include "hvac_private.h"

static boolean InputState[HVAC_MAX_INPUTS] = {0};
static boolean LastInputState[HVAC_MAX_INPUTS] = {0};

void Switch_Poll(void) 
{
   HVAC_Input_t   input;

   // Save previous input states
   for (input=(HVAC_Input_t)0;input<HVAC_MAX_INPUTS;input++) {
      LastInputState[input]=InputState[input];
      InputState[input]=HVAC_GetInput(input);
   }

   // Check each input for a low-high transition (button press)
   if (InputState[HVAC_TEMP_UP_INPUT]) {
      if (!LastInputState[HVAC_TEMP_UP_INPUT] && !InputState[HVAC_TEMP_DOWN_INPUT]) {
         if (HVAC_GetDesiredTemperature() < HVAC_ConvertCelsiusToDisplayTemp(HVAC_TEMP_MAXIMUM - HVAC_TEMP_SW_DELTA)) 
         {
            HVAC_SetDesiredTemperature(HVAC_GetDesiredTemperature()+HVAC_TEMP_SW_DELTA);
         }
      }
   } else if (InputState[HVAC_TEMP_DOWN_INPUT] && !LastInputState[HVAC_TEMP_DOWN_INPUT] ) {
      if (HVAC_GetDesiredTemperature() > HVAC_ConvertCelsiusToDisplayTemp(HVAC_TEMP_MINIMUM + HVAC_TEMP_SW_DELTA)) 
      {
         HVAC_SetDesiredTemperature(HVAC_GetDesiredTemperature()-HVAC_TEMP_SW_DELTA);
      }
   }

   if (InputState[HVAC_FAN_ON_INPUT] & !LastInputState[HVAC_FAN_ON_INPUT] ) {
      HVAC_SetFanMode(HVAC_GetFanMode()==Fan_On?Fan_Automatic:Fan_On);
   }

   if (InputState[HVAC_HEAT_ON_INPUT] & !LastInputState[HVAC_HEAT_ON_INPUT] ) {
      HVAC_SetHVACMode(HVAC_Heat);
   } else if (InputState[HVAC_AC_ON_INPUT] & !LastInputState[HVAC_AC_ON_INPUT] ) {
      HVAC_SetHVACMode(HVAC_Cool);
   } else if (LastInputState[HVAC_HEAT_ON_INPUT] | LastInputState[HVAC_AC_ON_INPUT]) {
      HVAC_SetHVACMode(HVAC_Off);
   }

   HVAC_ReadAmbientTemperature();
}

#if DEMOCFG_ENABLE_SWITCH_TASK
void Switch_Task(uint_32 param)
{
 
   while (TRUE) {
      Switch_Poll();
      
      // Delay 50 ms 
      _time_delay(50);
   }
}
#endif
