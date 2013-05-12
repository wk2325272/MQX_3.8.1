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
* $FileName: wa_tdel.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function called when a task is destroyed
*   so that the watchdog component can release any owned resources.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SW_WATCHDOGS
#include "watchdog.h"
#include "wdog_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _watchdog_cleanup
* Returned Value  : none
* Comments        : 
*    Used during task destruction to clean up any watcdogs started by 
*   this task.
*
*END*------------------------------------------------------------------*/

void _watchdog_cleanup
   (
      /* [IN] the task being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR          kernel_data;
   WATCHDOG_COMPONENT_STRUCT_PTR   watchdog_component_ptr;
   WATCHDOG_ALARM_TABLE_STRUCT_PTR table_ptr;
   _mqx_uint                       i;

   _GET_KERNEL_DATA(kernel_data);

   watchdog_component_ptr = (WATCHDOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_WATCHDOG];
   if (watchdog_component_ptr == NULL) {
      return; /* No work to do */
   } /* Endif */

#if MQX_CHECK_VALIDITY
   if (watchdog_component_ptr->VALID != WATCHDOG_VALID) {
      return;
   } /* Endif */
#endif

   table_ptr = &watchdog_component_ptr->ALARM_ENTRIES;
   
   _INT_DISABLE();
   while (table_ptr != NULL) {
      for (i = 0; i < WATCHDOG_TABLE_SIZE; ++i) {
         if (table_ptr->TD_PTRS[i] == td_ptr) {
            table_ptr->TD_PTRS[i] = NULL;
            td_ptr->FLAGS &= ~(TASK_WATCHDOG_STARTED | TASK_WATCHDOG_RUNNING);
            _INT_ENABLE();
            return;
         } /* Endif */
      } /* Endfor */
      table_ptr = table_ptr->NEXT_TABLE_PTR;
   } /* Endwhile */
   _INT_ENABLE();

} /* Endbody */
#endif /* MQX_USE_SW_WATCHDOGS */

/* EOF */
