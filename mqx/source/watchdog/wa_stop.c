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
* $FileName: wa_stop.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for stopping the watchdog.
*   routines.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SW_WATCHDOGS
#include "watchdog.h"
#include "wdog_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _watchdog_stop
* Returned Value  : boolean TRUE if succeeded
* Comments        : stop the watchdog timer
*
*END*------------------------------------------------------------------*/

boolean _watchdog_stop
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE1(KLOG_watchdog_stop);

   td_ptr = kernel_data->ACTIVE_PTR;
   if (td_ptr->FLAGS & TASK_WATCHDOG_STARTED) {
      _INT_DISABLE();
      /* Start CR 333 */
      /* td_ptr->FLAGS &= ~(TASK_WATCHDOG_STARTED | TASK_WATCHDOG_RUNNING); */
      td_ptr->FLAGS &= ~TASK_WATCHDOG_RUNNING;
      /* End CR 333 */
      _INT_ENABLE();
      _KLOGX2(KLOG_watchdog_stop, TRUE);
      return(TRUE);
   } /* Endif */

   _KLOGX2(KLOG_watchdog_stop, FALSE);
   return(FALSE);
   
} /* Endbody */
#endif /* MQX_USE_SW_WATCHDOGS */

/* EOF */
