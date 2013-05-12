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
* $FileName: ta_exit.c$
* $Version : 3.8.5.0$
* $Date    : Sep-21-2011$
*
* Comments:
*
*   This file contains the function called when a task exits.
*   This may be caused by a call to _task_abort, or by returning
*   from the root function of the task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_exit_function_internal
* Returned Value   : void
* Comments         : 
*    This function executes the exit handler for the current task,
* then destroys the current task
*
*END*----------------------------------------------------------------------*/

/* Dummy function so debuggers will display stack correctly */
extern void _task_exiting_function_internal(void);  /* Dummy prototype */
void _task_exiting_function_internal(void){}

void _task_exit_function_internal
   (
      void
   )
{
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_task_exit_function, kernel_data->ACTIVE_PTR->TASK_ID);

   td_ptr = kernel_data->ACTIVE_PTR;
#if MQX_TASK_DESTRUCTION

#if MQX_HAS_EXIT_HANDLER
   if (td_ptr->EXIT_HANDLER_PTR != NULL) {
      (*td_ptr->EXIT_HANDLER_PTR)();
   }
#endif   
   (void)_task_destroy(MQX_NULL_TASK_ID); /* Never returns */
#else
   td_ptr->STATE = DYING;
   while(TRUE) {
      _task_block();
   }
#endif
}
