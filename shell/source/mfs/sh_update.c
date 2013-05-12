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
* $FileName: sh_exit.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the exit command.
*
*END************************************************************************/


#include <string.h>
#include <mqx.h>
#include <fio.h>
#include <mfs.h>
#include <shell.h>
#include <sh_prv.h>



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name :  Shell_update
*  Returned Value:  none
*  Comments  :  
*  Usage:   by dx 2013.03.18
*
*END*-----------------------------------------------------------------*/

int_32  Shell_update(int_32 argc, char_ptr argv[] )
{
   boolean              print_usage, shorthelp = FALSE;
   int_32               return_code = SHELL_EXIT_SUCCESS;
   SHELL_CONTEXT_PTR    shell_ptr = Shell_get_context( argv ); 
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   char_ptr                   name_ptr = NULL;
   MQX_FILE_PTR               fs_ptr;
   if (!print_usage)  {
     if(argc >= 2)
     {
       if (strcmp(argv[1], "flush") == 0) 
       {
        if (argc>=3) 
        {
            name_ptr = argv[2];
        } else
        {
            name_ptr = Shell_get_current_filesystem_name(argv);
        }
         if (_io_validate_device(name_ptr)) 
         {
            //fs_ptr = fopen(name_ptr,0);
            fs_ptr = _io_get_fs_by_name(name_ptr);
            if (fs_ptr == NULL)  
            {
            printf("Error, file system %s not found\n", argv[1] );
            return_code = SHELL_EXIT_ERROR;
            }
            if (fs_ptr == NULL )  
            {
               printf("Error, unable to access file system\n" );
               return_code = SHELL_EXIT_ERROR;
            } else 
            {
               ioctl(fs_ptr, IO_IOCTL_FLUSH_OUTPUT, NULL);
              // fclose(fs_ptr);   
            } 
         } 
        
       }
       
     }else
     {
      printf("Error, invalid number of parameters\n");
      return_code = SHELL_EXIT_ERROR;
      print_usage=TRUE;
     }     
   }
   if (print_usage)  {
      if (shorthelp)  {
          printf("Usage: %s flush <drive:> \n", argv[0]);
      } else  {
        printf("Usage: %s flush <drive:> \n", argv[0]);
      }
   }
   return return_code;
} /* Endbody */

/* EOF */
