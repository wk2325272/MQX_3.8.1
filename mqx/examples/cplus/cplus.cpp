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
* $FileName: cplus.cpp$
* $Version : 3.8.10.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the cplus example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <fio.h>

/* Task IDs */
#define CPLUS_TASK 5

extern void cplus_task(uint_32);

TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
    {CPLUS_TASK, cplus_task, 2000, 8, "cplus", MQX_AUTO_START_TASK, 0, 0},
    {0,          0,          0,    0, 0,       0,                   0, 0}
};

class HelloWorld {
private:
   int check_init;
   const char *id;
public:
   HelloWorld() {
      check_init = 0x1234567;
   }
   ~HelloWorld() {
      _io_printf("%s: deallocation\n",id);
   }
   void print(const char *x) {
      id = x;
      if (check_init == 0x1234567) {
         _io_printf("%s: Constructed OK\n",id);
      } else {
         _io_printf("%s: Constructor not called\n",id);
      }
   }
};

HelloWorld global;

/*TASK*-----------------------------------------------------
* 
* Task Name    : cplus_task
* Comments     :
*    This task prints the state of each object
*
*END*-----------------------------------------------------*/

void cplus_task
   (
      uint_32 initial_data
   )
{
   { // Scope for local to destruct
      HelloWorld local;
      HelloWorld *heap;

      global.print("global");
      local.print("local");
      heap = new HelloWorld;
      if (heap != 0) {
         heap->print("heap");
         delete heap;
      } else {
         _io_printf("heap: new failed\n");
      } /* Endif */
   } // local should destruct
   _io_fflush(stdout);
   _task_block();
}

/* EOF */
