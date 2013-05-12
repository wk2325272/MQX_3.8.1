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
* $FileName: HVAC_Util.c$
* $Version : 3.8.12.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/


#include "hvac.h"

#include "Logging_public.h"
#include "hvac_public.h"
#include "hvac_private.h"

HVAC_PARAMS HVAC_Params = {0};

const char_ptr HVACModeName[] = {"Off", "Cool", "Heat", "Auto"};


void HVAC_InitializeParameters(void) 
{
   _lwevent_create(&HVAC_Params.Event, 0);
   HVAC_Params.HVACMode = HVAC_Auto;
   HVAC_Params.FanMode = Fan_Automatic;
   HVAC_Params.TemperatureScale = Celsius;
   HVAC_Params.DesiredTemperature = HVAC_DEFAULT_TEMP;
}


boolean HVAC_WaitParameters(int_32 timeout) 
{
    boolean catched;
    #if DEMOCFG_ENABLE_SWITCH_TASK
        // switch-sensing task is running, we can simply wait for the event or timeout
        _lwevent_wait_ticks(&HVAC_Params.Event, HVAC_PARAMS_CHANGED, TRUE, timeout);
        
    #else
        // poll the keys frequenly until the event or timeout expires
        uint_32 poll_wait = BSP_ALARM_FREQUENCY/20; // 50ms
        int_32  elapsed;
        MQX_TICK_STRUCT tickstart, ticknow;
        
        _time_get_elapsed_ticks(&tickstart);

        do
        {
            Switch_Poll();
            _lwevent_wait_ticks(&HVAC_Params.Event, HVAC_PARAMS_CHANGED, TRUE, poll_wait);

            _time_get_elapsed_ticks(&ticknow);
            elapsed = _time_diff_ticks_int32 (&ticknow, &tickstart, NULL);
            
        } while (elapsed < timeout);
        
    #endif          
    catched = (HVAC_Params.Event.VALUE & HVAC_PARAMS_CHANGED) != 0;
    _lwevent_clear(&HVAC_Params.Event, HVAC_PARAMS_CHANGED);
    return catched;
    
}


char_ptr HVAC_HVACModeName(HVAC_Mode_t mode)
{
   return HVACModeName[mode];
}

uint_32 HVAC_ConvertCelsiusToDisplayTemp(uint_32 temp)
{
   uint_32  display_temp;
   
   if (HVAC_Params.TemperatureScale ==  Celsius) {
      display_temp = temp;
   } else {
      display_temp = temp*9/5+320;
   }
   
   return display_temp;
}

uint_32 HVAC_ConvertDisplayTempToCelsius(uint_32 display_temp)
{
   uint_32  temp;
   
   if (HVAC_Params.TemperatureScale ==  Celsius) {
      temp = display_temp;
   } else {
      temp = (display_temp-320)*5/9;
   }
   
   return temp;
}
     

uint_32 HVAC_GetDesiredTemperature(void) {
   return HVAC_ConvertCelsiusToDisplayTemp(HVAC_Params.DesiredTemperature);
}


void HVAC_SetDesiredTemperature(uint_32 temp)
{
   HVAC_Params.DesiredTemperature = HVAC_ConvertDisplayTempToCelsius(temp);
   _lwevent_set(&HVAC_Params.Event, HVAC_PARAMS_CHANGED);
}


FAN_Mode_t HVAC_GetFanMode(void) {
   return HVAC_Params.FanMode;
}


void HVAC_SetFanMode(FAN_Mode_t mode)
{
   HVAC_Params.FanMode = mode;
   _lwevent_set(&HVAC_Params.Event, HVAC_PARAMS_CHANGED);
}


HVAC_Mode_t HVAC_GetHVACMode(void) 
{
   return HVAC_Params.HVACMode;
}


void HVAC_SetHVACMode(HVAC_Mode_t mode)
{
   HVAC_Params.HVACMode = mode;
   _lwevent_set(&HVAC_Params.Event, HVAC_PARAMS_CHANGED);
}


uint_32 HVAC_GetActualTemperature(void) {
   return HVAC_ConvertCelsiusToDisplayTemp(HVAC_State.ActualTemperature);
}


Temperature_Scale_t HVAC_GetTemperatureScale(void) 
{
   return HVAC_Params.TemperatureScale;
}


void HVAC_SetTemperatureScale(Temperature_Scale_t scale)
{
   HVAC_Params.TemperatureScale = scale;
   _lwevent_set(&HVAC_Params.Event, HVAC_PARAMS_CHANGED);
}

char HVAC_GetTemperatureSymbol(void) 
{
   return (HVAC_Params.TemperatureScale==Celsius)?'C':'F';
}




void HVAC_LogCurrentState(void) 
{
   char     Message[LOG_MESSAGE_SIZE];
   uint_32  temp_d,temp_a,output,i;

   temp_d = HVAC_GetDesiredTemperature();
   temp_a = HVAC_GetActualTemperature();
   
   i=sprintf(Message,"mode: %s, set:%2d.%1d %c temp:%2d.%1d %c Fan: %s ",
      HVAC_HVACModeName(HVAC_GetHVACMode()), 
      temp_d/10, temp_d%10, HVAC_GetTemperatureSymbol(), 
      temp_a/10, temp_a%10, HVAC_GetTemperatureSymbol(),
      HVAC_GetFanMode()==Fan_Automatic?"Auto":"On  ");

   for(output=0;output<HVAC_MAX_OUTPUTS;output++) {
      i+=sprintf(&Message[i],"%s %s, ", HVAC_GetOutputName((HVAC_Output_t)output), HVAC_GetOutput((HVAC_Output_t)output)?"On":"Off");
   }
   sprintf(&Message[i],"\n");

   Log(Message);

}

/* EOF */
