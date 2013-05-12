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
* $FileName: Security_Util.c$
* $Version : 3.8.14.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <string.h>
#include "security_public.h"
#include "security_private.h"

SEC_PARAMS SEC_Params = {0};

static boolean InputState[SEC_MAX_INPUTS] = {0};
static boolean LastInputState[SEC_MAX_INPUTS] = {0};
static boolean MotionState[3] = {0};

/* Event string table */
static char* SEC_Event_Tab[]={
   "System Started",
   "Motion Started",
   "Motion Stopped",
   "Door Opened",
   "Door Closed",
   "Window opened",
   "Window closed",
} ;
   
   
/*
 *  Set up initial parameters
 */
void SEC_InitializeParameters(void) 
{
   int i;
   SEC_Params.DoorStatus = CLOSED;
   SEC_Params.WindowStatus = CLOSED;
   
   /* Clear the log */
     for(i=0;i<MAX_QUEUE;i++)
     {
        SEC_Params.History[i].eventptr=NULL;
        SEC_Params.History[i].time.SECONDS=0;
        SEC_Params.History[i].time.MILLISECONDS=0;
     }  
   SEC_Params.HistoryIndex=0;
   
   /* Set first entry in log to System started.*/
   SEC_AddLog(SYSTEM_STARTED);
}


void SEC_Initialize(void)
{
   
    /* Initialize IO for switches and LEDs */
   SEC_InitializeIO();

   /* Initialize ADC for accelerometer and potentiometer */
   SEC_InitializeADC();

   /* Initialize parameters for security system */
   SEC_InitializeParameters();
}


/*
 * The following functions set the appropriate SEC_PARAMS structure 
 *   state, add the change to the log queue, and turn on or off
 *   the appropriate LED
 */
void SEC_SetDoorOpened(void)
{
   SEC_Params.DoorStatus = OPENED;
   SEC_AddLog(DOOR_OPENED);
   SEC_SetOutput(SEC_OPEN_OUTPUT, 1);
}

void SEC_SetDoorClosed(void)
{
   SEC_Params.DoorStatus = CLOSED;
   SEC_AddLog(DOOR_CLOSED);  
   SEC_SetOutput(SEC_OPEN_OUTPUT, 0);
}

void SEC_SetWindowOpened(void)
{
   SEC_Params.WindowStatus = OPENED;
   SEC_AddLog(WINDOW_OPENED);
   SEC_SetOutput(SEC_OPEN_OUTPUT, 1);
}

void SEC_SetWindowClosed(void)
{
   SEC_Params.WindowStatus = CLOSED;
   SEC_AddLog(WINDOW_CLOSED);
   SEC_SetOutput(SEC_OPEN_OUTPUT, 0);
}

void SEC_SetMovementStarted(void) 
{
  SEC_AddLog(MOTION_STARTED);
  SEC_Params.MovementStatus = MOVING;

}

void SEC_SetMovementStopped(void) 
{
  SEC_AddLog(MOTION_STOPPED);
  SEC_Params.MovementStatus = STOPPED;

}


/*
 * The following functions return the last read status values of the 
 *   security system inputs
 */
uint_32 SEC_GetDoorStatus(void) 
{
   return SEC_Params.DoorStatus;
}

uint_32 SEC_GetWindowStatus(void) 
{
   return SEC_Params.WindowStatus;
}

uint_32 SEC_GetMovementStatus(void) 
{
   return SEC_Params.MovementStatus;
}

_mqx_int SEC_GetGarageStatus(void) 
{
   return SEC_Params.GarageStatus;
}


/*
 * Add event string to log queue. 
 */
void SEC_AddLog(SEC_Event_t event)
{
     
    /* Add new event to ring buffer*/
    SEC_Params.History[SEC_Params.HistoryIndex].eventptr = SEC_Event_Tab[event];
    _time_get(&SEC_Params.History[SEC_Params.HistoryIndex++].time);
       
    if (SEC_Params.HistoryIndex >= MAX_QUEUE) 
       SEC_Params.HistoryIndex = 0; 
}


/*
 * Read in values through ADC of the accelerometer. Then determines if board is moving
 *  or tilted. Compares against accelerometer values that were recorded at bootup, and 
 *  returns 1 (MOVING) if values are outside of 'tolerance' bounds.
 */
_mqx_int SEC_DetectMotion(void) 
{
    _mqx_int i,axis[3] = {0,0,0};
    _mqx_int board_status;
    _mqx_int tolerance;
  
    if(SEC_GetMovementStatus() == MOVING) {
        /* If board has already been detected as moving, lower 
        tolerance to prevent false positives of the board being still*/
        tolerance = 0x20;
    }
    else {
        /* If board is not moving already, then make tolerance higher to prevent false positives 
        of the board moving */
        tolerance = 0x40;
    }
    
    /* Read in axis values */
   board_status = STOPPED;
   for (i = 0; i < 3; i++) {
      axis[i] = ReadADC(ADC_ACCX + i);  
      if (abs(SEC_Params.last[i] - axis[i]) > tolerance ) {
        board_status = MOVING;
      }
      SEC_Params.last[i] = axis[i];
   }
  
    return board_status;
}


/*
 *  Set LED 1-3 states according to "water-level" around Y accelerometer axis
 */
void SEC_WaterLevel(void) 
{
  uint_32 result;
  uint_32 flat, comparelow, comparemid, comparehigh;
 
  /* Read in Y-axis value */
  result = SEC_Params.last[ACCY];
  
  flat = SEC_Params.flat;
  
  comparelow  = flat-0x50-0xa0-0xa0;
  comparemid  = flat-0x50-0xa0;
  comparehigh = flat-0x50;
  
  /* Based on value on Y-axis, determine which LED's to light up */
     
  result = ((result - comparelow  <= (flat+0x50)  - comparelow)  ? 0 : (1 << 0)) |
           ((result - comparemid  <= (flat+0xF0)  - comparemid)  ? 0 : (1 << 1)) |
           ((result - comparehigh <= (flat+0x190) - comparehigh) ? 0 : (1 << 2)) ;
           
  /* Update LED status only when change detected */            
  if (result != SEC_Params.WaterLevel) {
    SEC_SetOutput(SEC_LED1_OUTPUT,(result >> 0) & 1);
    SEC_SetOutput(SEC_LED2_OUTPUT,(result >> 1) & 1);
    SEC_SetOutput(SEC_LED3_OUTPUT,(result >> 2) & 1);
  }

  /* Remember new state */
  SEC_Params.WaterLevel = result;
}


/*
 *  Get current values of push buttons (SW2 and SW3), determine if board is
 *    moving or tilted, and read in value of potentiometer
 */
void Input_Poll(void) 
{
   SEC_Input_t   input;   

   /* Save previous input states */
   for (input=SEC_DOOR_INPUT;input<SEC_MAX_INPUTS;input++) 
   {
      LastInputState[input]=InputState[input];
      InputState[input]=SEC_GetInput(input);
   }
   
   /* See if state has changed */
   
   /* If 'Door' pushbutton has been pressed... */
   if(InputState[SEC_DOOR_INPUT] && !LastInputState[SEC_DOOR_INPUT])
   {
        SEC_SetDoorOpened();
   }
   /* If 'Door' pushbutton has been released... */   
   else if(!InputState[SEC_DOOR_INPUT] && LastInputState[SEC_DOOR_INPUT])
   {
        SEC_SetDoorClosed();
   }
   
   /* If 'Window' pushbutton has been pressed... */
   if(InputState[SEC_WINDOW_INPUT] && !LastInputState[SEC_WINDOW_INPUT])
   {
        SEC_SetWindowOpened();
   }    
   /* If 'Window' pushbutton has been released... */    
   else if(!InputState[SEC_WINDOW_INPUT] && LastInputState[SEC_WINDOW_INPUT])
   {
        SEC_SetWindowClosed();
   } 
   
   /* Read ADC value to get current value of potentiometer (displayed as "Light Status" */
   SEC_Params.GarageStatus=ReadADC(ADC_POT);

   /* Queue to prevent false positives of movement detection */
   MotionState[2]=MotionState[1];
   MotionState[1]=MotionState[0];
   MotionState[0]=SEC_DetectMotion();
   
   /* If change in state of motion, determine if motion has stopped */
   if(MotionState[2] && !MotionState[1] && !MotionState[0] && SEC_GetMovementStatus()==MOVING) 
   {
      SEC_SetMovementStopped();
   } 
   /* Or if it just started moving */   
   else if (!MotionState[2] && MotionState[1] && MotionState[0] && SEC_GetMovementStatus()==STOPPED) 
   {
      SEC_SetMovementStarted();
   }
   
   SEC_WaterLevel();

}

