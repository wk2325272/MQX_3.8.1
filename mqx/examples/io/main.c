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
* $FileName: main.c$
* $Version : 3.8.8.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the I/O example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <string.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#if ! BSPCFG_ENABLE_TTYA
#error This application requires BSPCFG_ENABLE_TTYA defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#if ! BSPCFG_ENABLE_TTYB
#error This application requires BSPCFG_ENABLE_TTYB defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


extern void main_task(uint_32);


const TASK_TEMPLATE_STRUCT MQX_template_list[] = 
{
   /* Task Index,   Function,   Stack,  Priority, Name,   Attributes,          Param, Time Slice */
    { 10,           main_task,  2000,   8,        "Main", MQX_AUTO_START_TASK, 0,     0},
    { 0 }
};   

/*TASK*-----------------------------------------------------
* 
* Task Name    : main_task
* Comments     :
*    This task opens a second I/O channel and prints a message
* to it.
*
*END*-----------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
   pointer fh_ptr;

   printf("This is printed to the default device\n");
   fflush(stdout);

   if (!strcmp("ttya:", BSP_DEFAULT_IO_CHANNEL))
     fh_ptr = (pointer)fopen("ttyb:", BSP_DEFAULT_IO_OPEN_MODE);
   else
     fh_ptr = (pointer)fopen("ttya:", BSP_DEFAULT_IO_OPEN_MODE);
   if (fh_ptr == NULL) {
      printf("Cannot open the other IO device\n");
   } else {
      _io_set_handle(IO_STDOUT, fh_ptr);
      printf("This is printed to the other device\n");
   }
   
   fflush(stdout);
   if (fh_ptr != NULL)
      fclose(fh_ptr);
   
#if defined(BSPCFG_ENABLE_IODEBUG) && BSPCFG_ENABLE_IODEBUG 
   if(NULL == (fh_ptr = fopen("iodebug:", NULL))) { 
      printf("Cannot open the debug output\n"); 
   } else { 
      _io_set_handle(IO_STDOUT, fh_ptr); 
      printf("This is printed to the debug output\n"); 
   }
   
   fflush(stdout);
   if (fh_ptr != NULL)
      fclose(fh_ptr);
#endif
   
   _task_block();   

}

/* EOF */
