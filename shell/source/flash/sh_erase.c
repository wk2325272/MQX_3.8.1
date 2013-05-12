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
* $FileName: sh_erase.c$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the source for an MFS shell function.
*
*END************************************************************************/

#include <string.h>
#include <mqx.h>
#include <fio.h>
#include <mfs.h>
#include <shell.h>
#include <sh_prv.h>

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_write
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_erase(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   uint_32           block;

   MQX_FILE_PTR      fd;
   char_ptr          abs_path;   
   boolean           cache_enabled=TRUE;
   SHELL_CONTEXT_PTR    shell_ptr = Shell_get_context( argv );
   char                 buf[SHELL_BLOCK_SIZE];
   
   MQX_FILE_PTR   nandflash_hdl;
    
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );

   if (!print_usage)  {
   
    nandflash_hdl = fopen("nandflash:", NULL);
   
    if ((argc < 2) || (argc >= 3)) {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
      }
      
      block = _io_atoi( argv[1]);
            
      if(MQX_OK != ioctl(nandflash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK, (void*)block))
         printf("\n\nErasing block %d failed.",block);
      else
         printf("\n\nErasing block %d passed.\n",block);
   }
      
 if (print_usage)  {
      if (shorthelp)  {
         printf("%s <block>\n", argv[0]);
      } else  {
         printf("Usage: %s <block> ", argv[0]);
         printf("   <block>      = number of block to erase\n");
      
      }
   }     
} /* Endbody */

/* EOF */
