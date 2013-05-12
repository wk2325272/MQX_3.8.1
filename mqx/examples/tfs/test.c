/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: test.c$
* $Version : 3.8.16.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   Test application for trivial file system (TFS)
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <tfs.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


extern const TFS_DIR_ENTRY tfs_data[];


extern void main_task(uint_32);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index, Function,     Stack, Priority,  Name,     Attributes,          Param, Time Slice */
    { 10L,        main_task,    1400L, 8L,        "Main",   MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/*TASK*-------------------------------------------------------------------
* 
* Task Name : main_task
* Comments  :
*
*END*----------------------------------------------------------------------*/


void main_task
   (
      uint_32 dummy
   )
{
   MQX_FILE_PTR                 file1;
   MQX_FILE_PTR                 file2;
   int_32                       result;
   int_32                       i;
   uchar                        buffer[20];
   uint_32                      param;
   uchar_ptr                    name;
     
   result = _io_tfs_install ("tfs:", tfs_data);
   printf ("\ninstall returned: %08x", result); 
     
   file1 = fopen("tfs:\\INDEX.html", "r");
   printf ("\nfopen1 returned: %p", file1); 

   file2 = fopen("tfs:/", "r");
   printf ("\nfopen2 returned: %p", file2); 

   file2 = fopen("tfs:/readme.txt", "r");
   printf ("\nfopen2 returned: %p", file2); 

   result = fread(buffer, 1, 20, file1);
   printf ("\nfread1 returned: %d and\n", result);
   for (i=0;i<result;i++) printf ("%c",buffer[i]);

   result = ioctl(file1, IO_IOCTL_TFS_GET_ATTRIBUTES, &param);
   printf ("\nioctl1 get flags returned: %08x and %08x", result, param);

   result = ioctl(file2, IO_IOCTL_TFS_GET_NAME, &name);
   printf ("\nioctl2 get name returned: %08x and %s", result, name);

   result = ioctl(file2, IO_IOCTL_TFS_GET_LENGTH, &param);
   printf ("\nioctl2 get length returned: %08x and %d", result, param);

   result = fwrite(buffer, 1, 10, file2);
   printf ("\nfwrite2 returned: %d", result);

   result = ioctl(file2, IO_IOCTL_TFS_GET_LAST_ERROR, NULL);
   printf ("\nioctl2 get last error returned: %08x", result);

   result = fclose(file1);
   printf ("\nfclose1 returned: %d", result);

   result = fread(buffer, 1, 10, file2);
   printf ("\nfread2 returned: %d and ", result);
   for (i=0;i<result;i++) printf ("%c",buffer[i]);

   result = fread(buffer, 1, 10, file2);
   printf ("\nfread2 returned: %d and ", result);
   for (i=0;i<result;i++) printf ("%c",buffer[i]);

   result = fseek (file2, 0, IO_SEEK_END);
   printf ("\nfseek2 to end returned: %08x", result);

   result = fread(buffer, 1, 10, file2);
   printf ("\nfread2 returned: %d and ", result);
   for (i=0;i<result;i++) printf ("%c",buffer[i]);

   result = fseek (file2, 0, IO_SEEK_SET);
   printf ("\nfseek2 to start returned: %08x", result);

   printf ("\nfread2 complete file:\n");
   do
   {
        result = fread(buffer, 1, 4, file2);
        for (i=0;i<result;i++) printf ("%c",buffer[i]);
   } while (result != 0);

   result = fclose(file2);
   printf ("\nfclose2 returned: %d", result);

   _task_block();
 
} /* Endbody */

/* EOF */
