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
* $FileName: sh_i2c.c$
* $Version : 3.8.1.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains an I2C shell command.
*
*END************************************************************************/

#include <ctype.h>
#include <string.h>
#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include "shell.h"

#if BSPCFG_ENABLE_I2C0 || BSPCFG_ENABLE_I2C1 

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_i2c_read
*  Returned Value:  none
*  Comments  :  SHELL utility to read an I2C Device
*
*END*-----------------------------------------------------------------*/

int_32  Shell_i2c_read(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean              print_usage, shorthelp = FALSE;
   int_32               result,return_code = SHELL_EXIT_SUCCESS;
   uint_32              addr, bytes,i;
   MQX_FILE_PTR         fd;
   char_ptr             buf;
   
      
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
      if (argc==4)  {
         if ( Shell_parse_hexnum(argv[2], &addr) && Shell_parse_uint_32(argv[3], &bytes)) {

            fd = fopen(argv[1],0);
            buf = _mem_alloc(bytes);
            if ((fd!=NULL) && (buf !=NULL)) {
               if (I2C_OK != ioctl(fd, IO_IOCTL_I2C_SET_RX_REQUEST, &bytes))
               {
                  printf("ERROR during set rx request\n");
               } else {
                  result = fread(buf, bytes, 1, fd);
                  if (I2C_OK==result) {
                     for (i=0;i<bytes;i++) {
                        printf("%02x ", buf[i]);
                     }
                     printf("\n");
                  } else {
                     printf ("ERROR during read\n");
                  }
                  bytes=0;
                  if (I2C_OK != ioctl(fd, IO_IOCTL_FLUSH_OUTPUT, &bytes)) {
                     printf ("ERROR during flush\n");
                  }
                  if (I2C_OK != ioctl(fd, IO_IOCTL_I2C_STOP, NULL)) {
                     printf ("ERROR during stop\n");
                  }
               }
            }
            if (fd) fclose(fd);
            if (buf) _mem_free(buf);              
         } else {
            printf("Error, invalid parameter\n");
         }         
         
      } else {
         printf("Error, %s invoked with incorrect number of arguments\n", argv[0]);
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <device> <address> <bytes>\n", argv[0]);
      } else  {
         printf("Usage: %s <device> <address> <bytes>\n", argv[0]);
         printf("   <device>   = I2C device name (eg: i2c0:)\n");
         printf("   <address>  = i2c device address in hex\n");
         printf("   <bytes>    = bytes to read (decimal)\n");
      }
   }
   return return_code;
} /* Endbody */

#endif

/* EOF */
