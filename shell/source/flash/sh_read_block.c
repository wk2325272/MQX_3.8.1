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
* $FileName: sh_read_block.c$
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
#include <stdlib.h>


/*FUNCTION*-----------------------------------------------------
* 
* Task Name    : nandflash_open
* Comments     :
*    Open the flash device
*
*END*-----------------------------------------------------*/
MQX_FILE_PTR nandflash_open(char_ptr name) 
{
    
      MQX_FILE_PTR   nandflash_hdl;
    /* Open the flash device */
   
   nandflash_hdl = fopen(name, NULL);
    if (nandflash_hdl == NULL) {
        printf("\nUnable to open NAND flash device %s", name);
        _task_block();
    } /* Endif */
    printf("\nNAND Flash device %s opened", name);      
    return nandflash_hdl;
}

/*FUNCTION*-----------------------------------------------------
* 
* Task Name    : allocate_buffer
* Comments     :
*    Allocate the buffer
*
*END*-----------------------------------------------------*/
 char_ptr allocate_buffer(_mem_size buffer_size, char_ptr string) 
{
    char_ptr buffer_ptr;
    
    buffer_ptr = _mem_alloc_zero(buffer_size);
    if (buffer_ptr == NULL) {
        printf("\nFailed to get %s buffer", string);
        _task_block();
    } 
    return buffer_ptr;          
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    :   Shell_write
* Returned Value   :  int_32 error code
* Comments  :  Reads from a file .
*
*END*---------------------------------------------------------------------*/

int_32  Shell_read_block(int_32 argc, char_ptr argv[] )
{ /* Body */
   boolean           print_usage, shorthelp = FALSE;
   int_32            return_code = SHELL_EXIT_SUCCESS;
   int_16                   block,page_number,page;
    _mqx_uint      ID, num_blocks, virt_page_size=2048;
   char_ptr      write_buffer, read_buffer;
   _mem_size      read_write_size = 8;

    MQX_FILE_PTR   nandflash_hdl;
   _mqx_int       i;
   
   print_usage = Shell_check_help_request(argc, argv, &shorthelp );
   
  
   if (!print_usage)  {

   /* Open the flash device */
   nandflash_hdl = fopen("nandflash:", NULL);
   read_buffer = allocate_buffer(read_write_size*virt_page_size, "read");
   if(argc !=3)
     {
         printf("Error, invalid number of parameters\n");
         return_code = SHELL_EXIT_ERROR;
         print_usage=TRUE;
     }
     else {
       block = _io_atoi( argv[1]); 
       page  =_io_atoi( argv[2]);
       page_number  = block*256 + page;
       fseek(nandflash_hdl, page_number, IO_SEEK_SET);
       read(nandflash_hdl, read_buffer, read_write_size);
       printf("Reading from block %d :%s\n",block,read_buffer); 
       fclose(nandflash_hdl);
      }
 }
 if (print_usage)  {
      if (shorthelp)  {
         printf("%s <block> <page>\n", argv[0]);
      } else  {
         printf("Usage: %s <block> <page>\n", argv[0]);
         printf("   <block>      =  block to read \n");
         printf("   <page>       =  number of page\n");
      }
 }
  
   
} /* Endbody */

//#endif SHELLCFG_USES_NAND
/* EOF */
