/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
* All Rights Reserved

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
* $FileName: sh_mem.c$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the MQX shell command.
*
*END************************************************************************/


#include <ctype.h>
#include <string.h>
#include <mqx.h>
#include <fio.h>
#include <tad.h>
#include "shell.h"


void dump_mem (pointer addr, uint_32 len) 
{
   int i;
   for (i=0;i < len;i++) 
   {
      if (i%8 == 0) 
      {
         printf("\n0x%08X: ",addr);
      }
      printf("%08X ",*(uint_32 *)addr);
      ((uint_32 *)addr)++;
   }
   printf("\n");
}
         

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_md
*  Returned Value:  none
*  Comments  :  SHELL utility to display memory.
*
*END*-----------------------------------------------------------------*/

int_32 Shell_md (int_32 argc, char_ptr argv[])
{ /* Body */
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;
   uint_32 *            current_addr;
   uint_32              addr = 0;

   print_usage = Shell_check_help_request (argc, argv, &shorthelp);

   if (! print_usage)  
   {
      if (argc == 2) 
      {
         if (1 == sscanf(argv[1], "0x%8x", &addr))
         {
            current_addr = (uint_32 *)addr; 
            printf("\nAt: 0x%08X",current_addr);
            dump_mem(current_addr,8*24);
         } 
         else 
         {
            printf("Error, bad address format %s\n", argv[1]);
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
         }
      } 
      else 
      {
         printf("Error, %s invoked with incorrect number of arguments\n", argv[0]);
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
   }
   
   if (print_usage)  
   {
      if (shorthelp)  
      {
         printf("%s [address]\n", argv[0]);
      } 
      else  
      {
         printf("Usage: %s [address]\n", argv[0]);
         printf("   address = memory address to dump in hex format (e.g. 0x41F)\n");
      }
   }
   return return_code;
} /* Endbody */

/* EOF */
