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
* $FileName: Security_Shell_Commands.c$
* $Version : 3.8.B.1$
* $Date    : Jun-22-2012$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include "security_public.h"
#include "security_private.h"
#include "security_Shell_Commands.h"
#include <shell.h>  


const SHELL_COMMAND_STRUCT Telnet_commands[] = {
   {  "status",     Shell_status},
   {  "displaylog", Shell_displaylog},
   {  "clearlog",   Shell_clearlog},
   {  "ledtoggle",  Shell_ledtoggle}, 
   {  "exit",       Shell_exit },  
   {  "help",       Shell_help }, 
   {  "?",          Shell_command_list },      
   { NULL,          NULL } 
};
   
void Telnetd_shell_fn(void) 
{  
  Shell(Telnet_commands,NULL);
}


static void print_usage_simple (boolean shorthelp, const char_ptr argv, const char_ptr longhelp) 
{
  if (shorthelp)  
  {
    printf("%s\n", argv);
  } 
  else  
  {
    printf("Usage: %s\n", argv);
    printf("   %s\n", longhelp);
  }
}


static void print_usage_led (boolean shorthelp, const char_ptr argv, const char_ptr longhelp) 
{
  if (shorthelp)  {
    printf("%s <LED number>\n", argv);
  } else  {
    printf("Usage: %s <LED number>\n", argv);
    printf("   %s\n", longhelp);
  }
}


/*
 *  Print out current value of push buttons, if board is moving or tiled, 
 *  the value of the potentiometer, and the elapsed time since bootup
 */
int_32  Shell_status(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   int pot;
        
    TIME_STRUCT time;
    DATE_STRUCT date;
    
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  
   {
      if (argc > 1) 
      {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } 
      else 
      {
        /* Print out 'Door' status */
        printf("Door (SW2):                      ");
        if(SEC_GetDoorStatus()==OPENED)
            printf("Open\n");
        else
            printf("Closed\n");
      
        /* Print out 'Window' status */ 
            printf("Window (SW3):                    ");
        if(SEC_GetWindowStatus()==OPENED)
            printf("Open\n");
        else
            printf("Closed\n");
        
        /* Print out movement/tilt status */
            printf("Motion Detection:                ");
        if(SEC_GetMovementStatus()==MOVING)
            printf("Movement\n");
        else
            printf("No Movement\n");
        
        /* Print out potentiometer percentage in 10% increments */
        pot = (SEC_Params.GarageStatus / 409) * 10;
        printf("Garage Door (Potentiometer R2):  %d%% Open\n",pot);

        /* Print out elapsed time since bootup */
            _time_get(&time);
            _time_to_date(&time,&date);   
            printf("Time Since Bootup:               %02d:%02d:%02d\n\n",date.HOUR,date.MINUTE,date.SECOND);    
        
      }
   }
   
   if (print_usage)  
   {
      print_usage_simple (shorthelp, argv[0], "Display security system status");
   }
   return return_code;
} 

/*
 *  Display the last MAX_QUEUE events that occured with a timestamp
 *  Button pushes and movement changes logged. Potentiometer changes are not logged.
 */
int_32  Shell_displaylog(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
        
    TIME_STRUCT time;
    DATE_STRUCT date;
    SEC_HIST_PTR hist;
    int i;

    
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  
   {
      if (argc > 1) 
      {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } 
      else 
      {

            _time_get_elapsed(&time);
            _time_to_date(&time,&date);   
            printf("Time Since Bootup: %02d:%02d:%02d\n",date.HOUR,date.MINUTE,date.SECOND);    
        
            /* Print out last MAX_QUEUE events */
            for(i = (MAX_QUEUE - 1);i >= 0;i--)
            {
           hist = &(SEC_Params.History[(i+SEC_Params.HistoryIndex)%MAX_QUEUE]);
               if(NULL != hist->eventptr) {
              _time_to_date(&hist->time,&date);
              printf("%02d:%02d:%02d %s\n", date.HOUR,date.MINUTE,date.SECOND,hist->eventptr);
               }
            }       
        
      }
   }
   
   if (print_usage)  
   {
      print_usage_simple (shorthelp, argv[0], "Display event log");
   }
   return return_code;
} 

/*
 *  Erase log. Clears all entries.
 */
int_32  Shell_clearlog(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   
   int i;

    
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  
   {
      if (argc > 1) 
      {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } 
      else 
      {     
        for(i=0;i<MAX_QUEUE;i++)
        {
           SEC_Params.History[i].eventptr=NULL;
           SEC_Params.History[i].time.SECONDS=0;
           SEC_Params.History[i].time.MILLISECONDS=0;
        }  
        SEC_Params.HistoryIndex=0;
        printf("Log Cleared\n");
        
      }
   }
   
   if (print_usage)  
   {
      print_usage_simple (shorthelp, argv[0], "Clear event log");
   }
   return return_code;
} 

/*
 *  Toggle  LED's. 
 */
int_32  Shell_ledtoggle(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  
   {
      if (argc !=2 ) 
      {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } 
      else 
      {
        /* Read in command line argument to determine LED to toggle */
        if(argv[1][0]=='1' && argv[1][1]=='\0')
          SEC_ToggleOutput(SEC_LED1_OUTPUT);
        else if(argv[1][0]=='2' && argv[1][1]=='\0')
          SEC_ToggleOutput(SEC_LED2_OUTPUT);
        else if(argv[1][0]=='3' && argv[1][1]=='\0')
          SEC_ToggleOutput(SEC_LED3_OUTPUT);
        else
          printf("Invalid Argument. Must be '1', '2' or '3'\n");
      }
   }
   
   if (print_usage)  {
      print_usage_led (shorthelp, argv[0], "<LED number> = Toggle LED1, LED2 or LED3");
   }
   return return_code;
} 
