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
* $FileName: HVAC_Shell_Commands.c$
* $Version : 3.8.11.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/


#include "hvac.h"
#include <string.h>
#include <shell.h>

#include "hvac_public.h"
#include "HVAC_Shell_Commands.h"



/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_temp
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_temp(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   uint_32           temp,temp_fract;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 2) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         if (argc == 2) {
            temp_fract = 0;
            if (sscanf(argv[1],"%d.%d",&temp,&temp_fract)>=1) {
               if (temp_fract<10) {
                  HVAC_SetDesiredTemperature(temp*10+temp_fract);
               } else {
                  printf("Invalid temperature specified, format is dd.d\n");
               }
            } else {
               printf("Invalid temperature specified\n");
            } 
         }

         temp  = HVAC_GetDesiredTemperature();
         printf("Desired Temperature is %d.%1d %c\n", temp/10, temp%10, HVAC_GetTemperatureSymbol());
         temp  = HVAC_GetActualTemperature();
         printf("Actual Temperature is %d.%1d %c\n", temp/10, temp%10, HVAC_GetTemperatureSymbol());
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<temperature>]\n", argv[0]);
      } else  {
         printf("Usage: %s [<temperature>]\n", argv[0]);
         printf("   <temperature> = desired temperature (degrees)\n");
      }
   }
   return return_code;
} 


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_scale
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_scale(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   uint_32           temp;
 
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 2) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         if (argc == 2) {
            if (*argv[1] == 'f') {
               HVAC_SetTemperatureScale(Fahrenheit);
            } else if (*argv[1] == 'c') {
               HVAC_SetTemperatureScale(Celsius);
            } else {
               printf("Invalid temperature scale\n");
            } 
         }

         if (HVAC_GetTemperatureScale() == Fahrenheit) {
            printf("Temperature scale is Fahrenheit\n");
         } else {
            printf("Temperature scale is Celsius\n");
         }         
         temp  = HVAC_GetDesiredTemperature();
         printf("Desired Temperature is %d.%1d %c\n", temp/10, temp%10, HVAC_GetTemperatureSymbol());
         temp  = HVAC_GetActualTemperature();
         printf("Actual Temperature is %d.%1d %c\n", temp/10, temp%10, HVAC_GetTemperatureSymbol());
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [c | f]\n", argv[0]);
      } else  {
         printf("Usage: %s [c | f]\n", argv[0]);
         printf("   c = switch to Celsius scale\n");
         printf("   f = switch to Fahrenheit scale\n");
      }
   }
   return return_code;
} 

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_fan
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_fan(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   FAN_Mode_t        fan;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 2) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         if (argc == 2) {
            if (strcmp(argv[1],"on")==0) {
               HVAC_SetFanMode(Fan_On);
            } else if (strcmp(argv[1],"off")==0) {
               HVAC_SetFanMode(Fan_Automatic);
            } else {
               printf("Invalid fan mode specified\n");
            } 
         }

         fan  = HVAC_GetFanMode();
         printf("Fan mode is %s\n", fan == Fan_Automatic ? "Automatic" : "On");
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<mode>]\n", argv[0]);
      } else  {
         printf("Usage: %s [<mode>]\n", argv[0]);
         printf("   <mode> = on or off (off = automatic mode)\n");
      }
   }
   return return_code;
} 

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_hvac
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_hvac(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 2) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         if (argc == 2) {
            if (strcmp(argv[1],"off")==0) {
               HVAC_SetHVACMode(HVAC_Off);
            } else if (strcmp(argv[1],"cool")==0) {
               HVAC_SetHVACMode(HVAC_Cool);
            } else if (strcmp(argv[1],"heat")==0) {
               HVAC_SetHVACMode(HVAC_Heat);
            } else if (strcmp(argv[1],"auto")==0) {
               HVAC_SetHVACMode(HVAC_Auto);
            } else {
               printf("Invalid hvac mode specified\n");
            } 
         }

         printf("HVAC mode is %s\n", HVAC_HVACModeName(HVAC_GetHVACMode()));
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<mode>]\n", argv[0]);
      } else  {
         printf("Usage: %s [<mode>]\n", argv[0]);
         printf("   <mode> = off, cool, heat or auto\n");
      }
   }
   return return_code;
} 

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_info
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_info(int_32 argc, char_ptr argv[] )
{
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   uint_32           temp;
   FAN_Mode_t        fan;
   HVAC_Output_t     output;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc > 1) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      } else {
         printf("HVAC mode:    %s\n",  HVAC_HVACModeName(HVAC_GetHVACMode()));
         temp  = HVAC_GetDesiredTemperature();
         printf("Desired Temp: %d.%1d %c\n", temp/10, temp%10, HVAC_GetTemperatureSymbol());
         temp  = HVAC_GetActualTemperature();
         printf("Actual Temp:  %d.%1d %c\n", temp/10, temp%10, HVAC_GetTemperatureSymbol());
         fan  = HVAC_GetFanMode();
         printf("Fan mode:     %s\n", fan == Fan_Automatic ? "Automatic" : "On");

         for(output=(HVAC_Output_t)0;output<HVAC_MAX_OUTPUTS;output++) {
            printf("%12s: %s\n", HVAC_GetOutputName(output), HVAC_GetOutput(output) ? "On" : "Off");
         }
       }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s \n", argv[0]);
      } else  {
         printf("Usage: %s \n", argv[0]);
      }
   }
   return return_code;
} 

#if DEMOCFG_ENABLE_USB_FILESYSTEM

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_log
*  Returned Value:  none
*  Comments  :  SHELL utility to type a file
*
*END*-----------------------------------------------------------------*/

int_32  Shell_log(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;
   MQX_FILE_PTR         fd;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc == 1)  {
         
         fd = fopen(LOG_FILE, "r");
         if (fd)  {
            while (!feof(fd) ) {
               fputc(fgetc(fd), stdout);
            } 
            fclose(fd);
            printf("\n");
         } else  {
            printf("Error, unable to open file %s.\n", LOG_FILE );
            return_code = SHELL_EXIT_ERROR;
         }
      } else  {
         printf("Error, %s invoked with incorrect number of arguments\n", argv[0]);
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s \n", argv[0]);
      } else  {
         printf("Usage: %s \n", argv[0]);
      }
   }
   return return_code;
} /* Endbody */

#endif
/* EOF*/
