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
* $FileName: ti_elapt.c$
* $Version : 3.8.6.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the functions for returning the amount of
*   time since the processor started running.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_get_elapsed_ticks
* Returned Value   : void
* Comments         : 
*   This function retrieves the number of ticks since
*   the processor started.  (without any time offset information)
*
*   ***  Keep in sync with ti_gett.c  ***
*
*END*----------------------------------------------------------------------*/

void _time_get_elapsed_ticks
    (
        /* [IN/OUT] the address where the time is to be put */
        MQX_TICK_STRUCT_PTR tick_ptr
    )
{
    KERNEL_DATA_STRUCT_PTR kernel_data;

#if MQX_CHECK_ERRORS
    if ( tick_ptr == NULL ) {
        return;
    }
#endif

#if MQX_ENABLE_USER_MODE && MQX_ENABLE_USER_STDAPI
    if (MQX_RUN_IN_USER_MODE) {
        _usr_time_get_elapsed_ticks(tick_ptr);
        return;
    }
#endif
   
    _GET_KERNEL_DATA(kernel_data);
    
    _INT_DISABLE();
    
    *tick_ptr = kernel_data->TIME;
    /* Start CR 1082*/
    if (kernel_data->GET_HWTICKS) {
        // The hardware clock may have counted passed it's reference
        // and have an interrupt pending.  Thus, HW_TICKS may exceed
        // kernel_data->HW_TICKS_PER_TICK and this tick_ptr may need
        // normalizing.  This is done in a moment.
        tick_ptr->HW_TICKS = (*kernel_data->GET_HWTICKS)
            (kernel_data->GET_HWTICKS_PARAM);
    } /* Endif */
    
    _INT_ENABLE(); // The timer ISR may go off and increment kernel_data->TIME
    
    // The tick_ptr->HW_TICKS value might exceed the
    // kernel_data->HW_TICKS_PER_TICK and need to be
    // normalized for the PSP.
    PSP_NORMALIZE_TICKS(tick_ptr);
    /* End CR 1082*/   
}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _usr_time_get_elapsed_ticks
* Returned Value   : void
* Comments         : 
*   This function retrieves the number of ticks since
*   the processor started.  (without any time offset information)
*
*   ***  Keep in sync with ti_gett.c  ***
*
*END*----------------------------------------------------------------------*/

void _usr_time_get_elapsed_ticks
    (
        /* [IN/OUT] the address where the time is to be put */
        MQX_TICK_STRUCT_PTR tick_ptr
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)tick_ptr, 0, 0, 0, 0};
    _mqx_api_call(MQX_API_TIME_GET_ELAPSED_TICKS, &params);
}

#endif // MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_get_elapsed_ticks_fast
* Returned Value   : void
* Comments         : 
*   This function retrieves the number of ticks since
*   the processor started.  (without any time offset information)
*
*   ***  Keep in sync with ti_elapt.c  ***
*
*   The only difference between _time_get_elapsed_ticks_fast and
*   _time_get_elapsed_ticks is that this one is supposed to be
*   called from code with interrupts DISABLED. Do not use this function
*   with interrupts ENABLED.
*
*END*----------------------------------------------------------------------*/

void _time_get_elapsed_ticks_fast
   (
      /* [IN/OUT] the address where the time is to be put */
      MQX_TICK_STRUCT_PTR tick_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

#if MQX_CHECK_ERRORS
   if ( tick_ptr == NULL ) {
      return;
   } /* Endif */
#endif

   _GET_KERNEL_DATA(kernel_data);

   *tick_ptr = kernel_data->TIME;
   /* Start CR 1082*/
   if (kernel_data->GET_HWTICKS) {
      // The hardware clock may have counted passed it's reference
      // and have an interrupt pending.  Thus, HW_TICKS may exceed
      // kernel_data->HW_TICKS_PER_TICK and this tick_ptr may need
      // normalizing.  This is done in a moment.
      tick_ptr->HW_TICKS = (*kernel_data->GET_HWTICKS)
         (kernel_data->GET_HWTICKS_PARAM);
   } /* Endif */

   // The tick_ptr->HW_TICKS value might exceed the
   // kernel_data->HW_TICKS_PER_TICK and need to be
   // normalized for the PSP.
   PSP_NORMALIZE_TICKS(tick_ptr);
   /* End CR 1082*/

} /* Endbody */
#endif

/* EOF */
