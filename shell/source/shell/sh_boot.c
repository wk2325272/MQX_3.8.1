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
* $FileName: sh_boot.c$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <fio.h>
#include <mfs.h>
#include <shell.h>
#include <sh_prv.h>
#include <stdlib.h>
#include <nandbootloader.h>

extern boolean _auto_boot;
extern uint_32 _image_index;

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_list_image
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_list_image(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;

   print_usage = Shell_check_help_request(argc, argv, &shorthelp );


   if (!print_usage)  {
      if (argc != 1) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
      else { 
         _bootloader_list_image();
      }
   }

   if (print_usage)  {
      if (shorthelp)  {
         printf("%s\n", argv[0]);
      } else  {
         printf("Usage: %s ", argv[0]);
         printf("   <%s> to list image table\n",argv[0]);
      }
   }

} /* Endbody */
/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_del_image
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_del_image(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;
   uint_32             index, block, result;
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   
  
   if (!print_usage)  {
      if ((argc < 2) || (argc >= 3)) {     
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
      else 
      {
         index = _io_atoi(argv[1]);
         _bootloader_del_image(index);
      }
   }
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <index> \n", argv[0]);
      } else  {
         printf("Usage: %s <index> \n", argv[0]);
         printf("  <index> = index of image to delete\n");
      }
   }
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_store_image
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_store_image(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;
   uint_32             addr, size, result;
   uchar               buffer[] = {1,2,3,4,5,6,7,8,9,0};
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   
   
   if (!print_usage)  {
      if (argc!=2) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
      else {
         addr = _io_atoi(argv[1]);
         size = _io_atoi(argv[2]);
         _bootloader_store_image(addr, size, buffer,(uchar_ptr)"default");
      }
   }
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s <add> <size> \n", argv[0]);
      } else  {
         printf("Usage: %s <add> <size>\n", argv[0]);
         printf("  <add> =  addrees begin to store");
         printf("  <size>=  size image to store   ");
         
      }
   }
}

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_go_image
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_go_image(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
  
   if (!print_usage)  {
      _auto_boot = FALSE;
      _image_index = _io_atoi(argv[1]);
      if(_bootloader_check_image(FALSE,_image_index)) {
         printf("\nStarting application at image:%d\n",_image_index);
         _mqx_exit(0);
      }
      else {
         printf("\nError no image found !\n");
         print_usage = TRUE;
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<index>] \n", argv[0]);
      } else  {
         printf("Usage: %s [<index>] \n", argv[0]);
         printf("   <index> = image index in memory table \n");
      }
   }
}


/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_del_table
* Returned Value : 
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_del_table(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;
   char_ptr            read_buffer;
   MQX_FILE_PTR        nandflash_hdl;
   IMAGE_INFO_PTR      image;
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   
  
   if (!print_usage)  {
      if (argc !=1) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
      else
      {
         if(IO_ERROR == _bootloader_del_table())
            printf ("Delete table error\n ");
         else  
            return MQX_OK; 
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s \n", argv[0]);
      } else  {
         printf("Usage: %s ", argv[0]);
         printf("   <%s> Delete table in memory \n",argv[0]);
      }
   }
} 

/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_reset
* Returned Value : 
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_reset(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;
   char_ptr            read_buffer;
   MQX_FILE_PTR        nandflash_hdl;
   IMAGE_INFO_PTR      image;
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   
  
   if (!print_usage)  {
      if (argc !=1) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
      else
      {
         _bootloader_reset();
         return MQX_OK; 
      }
   }
   
   if (print_usage)  {
      if (shorthelp)  {
         printf("%s \n", argv[0]);
      } else  {
         printf("Usage: %s ", argv[0]);
         printf("   <%s> booloader reset  \n",argv[0]);
      }
   }
} 



/*TASK*-----------------------------------------------------------------
*
* Function Name  : Shell_bootloader_set_default
* Returned Value : 
* Comments       :
*
*END------------------------------------------------------------------*/

int_32  Shell_bootloader_set_default(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean             print_usage, shorthelp = FALSE;
   int_32              return_code = SHELL_EXIT_SUCCESS;
    
   int_8               index;
    
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   
  
   if (!print_usage)  {
      if (argc != 2) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage = TRUE;
      }
      else
      {
         index = _io_atoi( argv[1]);
         if (IO_ERROR ==_bootloader_set_default(index))
            printf ("Set default error\n ");
      }
   }

   if (print_usage)  {
      if (shorthelp)  {
         printf("%s [<index>]", argv[0]);
      } else  {
         printf("Usage: %s ", argv[0]);
         printf("   <%s> image index to boot\n",argv[0]);
      }
   }
} /* Endbody */
  
/* EOF */
