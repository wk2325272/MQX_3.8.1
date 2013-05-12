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
* $FileName: sh_boot_tftp.c$
* $Version : 3.7.2.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the RTCS shell.
*
*END************************************************************************/

#include <ctype.h>
#include <string.h>
#include <rtcs.h>
#include "shell.h"
#include "sh_rtcs.h"
#include <math.h>

#pragma section code_type ".image_data"
__declspec(section ".image_data")  extern uchar_ptr _image_data; 
        

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  ChartoHex
*  Returned Value:  
*  Comments  :  convert a character to hex
*  Usage:  
*
*END*-----------------------------------------------------------------*/
uchar ChartoHex(uchar c) { 
 if (c>='0' && c<='9') return c-'0'; 
 if (c>='A' && c<='F') return c-'A'+10; 
 if (c>='a' && c<='f') return c-'a'+10; 
 return c=0;        /* not Hex digit  */
} 

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  StrtoHex
*  Returned Value:  
*  Comments  :  convert a String to Hexa
*  Usage:  
*
*END*-----------------------------------------------------------------*/
uint_32 StrtoHex(char_ptr hex) 
{ 
 uint_32 hexa_value=0;
 uint_8 n=0,i;
 char_ptr buff = NULL ;
 
strcpy(buff,hex);
while(*(hex++) !=0) {
  n++;
 }
for(i=0;i<n;i++){
  hexa_value += ChartoHex(buff[i])*pow(16,n-1-i); 
  }
return hexa_value;
 } 
     
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_TFTP_client
*  Returned Value:  none
*  Comments  :  SHELL utility to TFTP to or from a host
*  Usage:  tftp host get source [destination] [mode] 
*
*END*-----------------------------------------------------------------*/

int_32  Shell_bootloader_tftp_client(int_32 argc, char_ptr argv[] )
{
   _ip_address          hostaddr;
   char                 hostname[MAX_HOSTNAMESIZE];
   char_ptr             file_ptr;
   uint_32              tftp_handle, buffer_size,byte_number=0,bytes=0;
   uchar_ptr            buffer_ptr;
   TFTP_DATA_STRUCT     tftp_data;
   MQX_FILE_PTR         fd;
   int_32               error;
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;   
   int_32               i=0,address;
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
   
      if ((argc >= 3) && (argc <= 5))  {
         RTCS_resolve_ip_address( argv[1], &hostaddr, hostname, MAX_HOSTNAMESIZE ); 

         if (!hostaddr)  {
            printf("Unable to resolve host.\n");
            return_code = SHELL_EXIT_ERROR;
         } else  {
            tftp_data.SERVER   = hostaddr;
            tftp_data.FILENAME = argv[2];
            tftp_data.FILEMODE = argv[2];
            if (argc > 3)  {
               file_ptr = argv[3];
               if (argc > 4) {
                  tftp_data.FILEMODE = argv[4];
               } else {
              tftp_data.FILEMODE = argv[2];
               }
            } else {
               file_ptr = argv[2];
            }
              address = StrtoHex(argv[3]);      
             _image_data[0] = 0x00;
               printf("\nDownloading file %s from TFTP server: %s [%ld.%ld.%ld.%ld]\n",
               tftp_data.FILENAME,hostname, IPBYTES(hostaddr));
               tftp_handle = (*FT_TFTP->OPEN)( (pointer) &tftp_data );
               if ( tftp_handle != RTCS_OK )  {
                  printf("\nError opening file %s\n",tftp_data.FILENAME);
                  return_code = SHELL_EXIT_ERROR;
               } else  {
                  while (! (*FT_TFTP->EOFT)())  {
                
                    while(1) {
                      buffer_ptr = (*FT_TFTP->READ)( &buffer_size );
                     if (buffer_ptr != NULL)  {
                       for(i=0;i<(buffer_size);i++)
                       {
                        _image_data[i+byte_number]=buffer_ptr[i];
                        
                       }
                        byte_number += buffer_size;
                        if(byte_number&0x1000)
                        printf("#");
                     
                     } else {
                     
                     switch (buffer_size) {
                         case (RTCSERR_TFTP_ERROR + 1):
                            printf("\nFile %s not found\n", tftp_data.FILENAME);
                            break;
                         case (RTCSERR_TFTP_ERROR + 2):
                            printf("\nAccess violation\n");
                            break;
                         case (RTCSERR_TFTP_ERROR + 3):
                            printf("\nDisk full or allocation exceeded\n");
                            break;
                         case (RTCSERR_TFTP_ERROR + 4):
                            printf("\nIllegal TFTP operation\n");
                            break;
                         case (RTCSERR_TFTP_ERROR + 5):
                            printf("\nUnknown transfer ID\n");
                            break;
                         case (RTCSERR_TFTP_ERROR + 6):
                            printf("\nFile already exists\n");
                            break;
                         case (RTCSERR_TFTP_ERROR + 7):
                            printf("\nNo such user\n");
                            break;
                         default:
                            if(byte_number !=0) 
                               {
                                 printf("\nTransfer 0x%x byte success",byte_number);  
                                 if(MQX_OK == _bootloader_store_image(address, byte_number, _image_data,(uchar_ptr)tftp_data.FILEMODE)) {
                                    printf("\nWrite image to table Success\n");
                                    _bootloader_list_image();
                                 }
                                 else {
                                    printf("\nWrite image to table Fail\n");
                                 }
                                 break; 
                               }
                            else 
                            printf("\nError reading file %s\n", tftp_data.FILENAME);
                         } /* Endswitch */
                     break; 
                     }  
                          
                  }
                    
                  }
                
                error = (*FT_TFTP->CLOSE)();
              
               }
            tftp_handle = (*FT_TFTP->OPEN)( (pointer) &tftp_data );
            if ( tftp_handle != RTCS_OK )  {
               printf("\nError opening file %s\n",tftp_data.FILENAME);
               return_code = SHELL_EXIT_ERROR;
            } 
            error = (*FT_TFTP->CLOSE)();
         }
      } else  {
         printf("Error, %s invoked with incorrect number of arguments\n", argv[0]);
         print_usage = TRUE;
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <host> <source> <add> [<name>]\n", argv[0]);
      } else  {
         printf("Usage: %s <host> <source> <add> [<name>]\n", argv[0]);
         printf("   <host>    = TFTP server address or name\n");
         printf("   <source>  = Source file name\n");
         printf("   <add>     = Start address (Rom address in Linker file) \n");
         printf("   [<name>]  = Name of image will be stored in table \n");
      }
   }
   return return_code;
} /* Endbody */


/* EOF */
