/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
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
* $FileName: sh_ping.c$
* $Version : 3.7.10.0$
* $Date    : Feb-24-2011$
*
* Comments:
*
*   This file contains the RTCS shell.
*
*END************************************************************************/

#include <ctype.h>
#include <string.h>
#include <mqx.h>
#include "shell.h"
#if SHELLCFG_USES_RTCS

#include <rtcs.h>
#include "sh_rtcs.h"


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_ping
*  Returned Value:  none
*  Comments  :  SHELL utility to Ping a host
*
*END*-----------------------------------------------------------------*/

int_32  Shell_ping(int_32 argc, char_ptr argv[] )
{ /* Body */
   _ip_address          hostaddr;
   char                 hostname[MAX_HOSTNAMESIZE];
   uint_32              i;
   int_32               time, error;
   uint_16              pingid = 0;
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc == 2)  {
         memset (hostname, 0, sizeof (hostname));
         RTCS_resolve_ip_address( argv[1], &hostaddr, hostname, MAX_HOSTNAMESIZE ); 

         if (!hostaddr)  {
#if RTCSCFG_ENABLE_DNS | RTCSCFG_ENABLE_LWDNS
            printf("Unable to resolve host\n");
#else
            printf("Unable to resolve host - dns service disabled\n");
#endif            
            return_code = SHELL_EXIT_ERROR;

         } else  {
            printf("Pinging %s [%ld.%ld.%ld.%ld]:\n", hostname, IPBYTES(hostaddr));

            for (i=0; i<4; i++) {
               time = 5000; /* 5 seconds */
               error = RTCS_ping(hostaddr, (uint_32_ptr)&time, ++pingid);
               if (error == RTCSERR_ICMP_ECHO_TIMEOUT) {
                  printf("Request timed out\n");
               } else if (error) {
                  if(error == 0x1000515)
                  {
                     printf("Error 0x%04lX - illegal ip address\n", error);
                  }
                  else if( error == 0x1000510) 
                  {
                     printf("Error 0x%04lX - no route to host\n", error);
                  }
                  else
                  {
                     printf("Error 0x%04lX\n", error);
                  }
               } else {
                  if (time < 1)
                  {
                     printf("Reply from %ld.%ld.%ld.%ld: time<1ms\n", IPBYTES(hostaddr));
                  }
                  else
                  {
                     printf("Reply from %ld.%ld.%ld.%ld: time=%ldms\n", IPBYTES(hostaddr), time);
                  }
                  if ((time<1000) && (i<3)) 
                  {
                     RTCS_time_delay(1000-time);
                  } 
               } 
            } /* Endfor */
         }
      } else  {
         printf("Error, %s invoked with incorrect number of arguments\n", argv[0]);
         print_usage = TRUE;
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <host>\n", argv[0]);
      } else  {
         printf("Usage: %s <host>\n", argv[0]);
         printf("   <host>   = host ip address or name\n");
      }
   }
   return return_code;
} /* Endbody */

#endif /* SHELLCFG_USES_RTCS */

/* EOF */
