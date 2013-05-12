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
* $FileName: dd_prof.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   Implements user callbacks for the Diab Compiler RTA profiling
*
*END************************************************************************/

/* never profile this code */
#pragma option -Xprof-all=0

#include <string.h>
#include <rta/proflib.h>
#include <rta/rtaenv.h>
#include "mqx_inc.h"
#include "mqx_str.h"
#include "profprv.h"
#include "rta_prv.h"

static RTA_PROFILE_INFO_PTR RTA_ptr = NULL;

/* Callbacks from RTALIB, required for MQX customization */

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rta_alloc
* Returned Value   : 
* Comments         : MQX must be fully initialized when this function is called
*
*END*-------------------------------------------------------------------------*/

void _PTR_ __rta_alloc
   (
      size_t   size
   )
{ /* Body */

   return _mem_alloc_system(size);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rta_free
* Returned Value   : 
* Comments         : MQX must be fully initialized when this function is called
*
*END*-------------------------------------------------------------------------*/

void __rta_free
   (
      void _PTR_ p
   )
{ /* Body */

   _mem_free(p);

} /* Endbody */


/*ISR*-------------------------------------------------------------------------
*
* ISR Name : __prof_timer_isr
* Comments :
*
*END*-------------------------------------------------------------------------*/

void __prof_timer_isr
   (
      /* [IN] The Data for this interrupt */
      pointer isr_data
   )
{ /* Body */
   RTA_PROFILE_INFO_PTR  rta_ptr = (RTA_PROFILE_INFO_PTR)isr_data;

   /* Call the profiler clock function to update the time */
   if (rta_ptr->PROFILING_ACTIVE) {
      __prof_clock();
   } /* Endif */

   /* Chain to previous notifier */
   (*rta_ptr->OLD_ISR)(rta_ptr->OLD_ISR_DATA);   

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __prof_user_init
* Returned Value   : 
* Comments         : MQX must be fully initialized when this function is called
*
*END*-------------------------------------------------------------------------*/

unsigned long __prof_user_init
   (
      int start_timer
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   uint_32                       timer_vector;

   if (start_timer == 1) {
      /* Need to see if we should start/restart the timer */
      if (RTA_ptr == NULL) {
         /* First time here, we need to do some initialization */

         /* Get a state structure */
         RTA_ptr = _mem_alloc_system_zero(sizeof(RTA_PROFILE_INFO_STRUCT));
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
         if (RTA_ptr == NULL){
            return(0xFFFFFFFF);
         } /* Endif */
#endif   

         RTA_ptr->PROFILING_ACTIVE = TRUE;

         _GET_KERNEL_DATA(kernel_data);
         timer_vector = kernel_data->SYSTEM_CLOCK_INT_NUMBER;

         /* Chain the new isr to the MQX timer interrupt */
         RTA_ptr->OLD_ISR_DATA = _int_get_isr_data(timer_vector);
         RTA_ptr->OLD_ISR = _int_install_isr(timer_vector, __prof_timer_isr, RTA_ptr);
      } else {
         /* Just need to reactivate profiling -- this will also execute if already enabled */
         RTA_ptr->PROFILING_ACTIVE = TRUE;
      } /* Endif */

      /* Remember how many times the timer has been started */
      RTA_ptr->NUM_STARTS++;

      /* Return the period in ns */
      return (_time_get_resolution() * 1000000);      
   } /* Endif */

   /* No timer started */
   return 0;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __prof_user_finish
* Returned Value   : 
* Comments         : We can't safely uninstall this interrupt, in case someone 
*                    (the watchdog) has chained to us.  So, we will just stop
*                    calling __prof_clock() from the ISR.
*
*END*-------------------------------------------------------------------------*/

void __prof_user_finish
   (
      int stop_timer
   )
{ /* Body */

   /* Only "stop" the timer if NUM_STARTS is back to zero */    
   _int_disable();
   RTA_ptr->NUM_STARTS--;
   if ((stop_timer == 1) && (RTA_ptr->NUM_STARTS == 0)) {
      RTA_ptr->PROFILING_ACTIVE = FALSE;
   } /* Endif */
   _int_enable();

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __prof_get_current_tid
* Returned Value   : 0 on success, -1 if not multitasking
* Comments         : Outputs a task identifier for the currently active task
*
*END*-------------------------------------------------------------------------*/

void __prof_get_current_tid
   (
      /* Pointer to a buffer to hold the task identifier */
      char * tid_ptr,

      /* Maximum size of the buffer */
      int    max
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   TASK_TEMPLATE_STRUCT_PTR      template_ptr;
   uint_32                       size;

   _GET_KERNEL_DATA(kernel_data);

   template_ptr = kernel_data->ACTIVE_PTR->TASK_TEMPLATE_PTR;

   /* How long is the name? */
   size = _strnlen(template_ptr->TASK_NAME, max);

   /* Copy the string name of the task into the buffer */
   if (size >= max) {
      size = max - 1;
   } /* Endif */   
   strncpy(tid_ptr, template_ptr->TASK_NAME, size);

   /* Terminate the copied string */
   tid_ptr[size] = '\0';

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __diab_atomic_enter
* Returned Value   : Unused
* Comments         : Disables interrupts, so operations are atomic
*
*END*-------------------------------------------------------------------------*/

int __diab_atomic_enter
   (
      void 
   )
{ /* Body */

   _int_disable();

   /* Unused return value */
   return 0; 

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __diab_atomic_restore
* Returned Value   : None
* Comments         : Enables interrupts, after a call to __diab_atomic_enter
*
*END*-------------------------------------------------------------------------*/

void __diab_atomic_restore
   (
      int lock_value
   )
{ /* Body */

   _int_enable();

} /* Endbody */


/* Additional functions required by MQX */
/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_initialize_context
* Returned Value   : 
* Comments         : Initializes the fields in PROFILE_CONTEXT_STRUCT
*
*END*-------------------------------------------------------------------------*/

void _profiler_initialize_context
   (
      /* [IN] The profile context for this task */
      PROFILE_CONTEXT_STRUCT_PTR    context_ptr
   )
{ /* Body */

   context_ptr->PROF_INFO_PTR_SAVE = NULL;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_save_context
* Returned Value   : 
* Comments         : Saves the value in __PROF_INFO_PTR into the active TD
*
*END*-------------------------------------------------------------------------*/

void _profiler_save_context
   (
      void 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   PROFILE_CONTEXT_STRUCT_PTR    context_ptr;

   _GET_KERNEL_DATA(kernel_data);

   context_ptr = kernel_data->ACTIVE_PTR->PROFILER_CONTEXT_PTR;
   if (context_ptr != NULL) {
      context_ptr->PROF_INFO_PTR_SAVE = __PROF_INFO_PTR;
   } /* Endif */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_restore_context
* Returned Value   : 
* Comments         : Restores the profiler context from the active TD into the
*                     __PROF_INFO_PTR 
*
*END*-------------------------------------------------------------------------*/

void _profiler_restore_context
   (
      void 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   PROFILE_CONTEXT_STRUCT_PTR    context_ptr;
   
   _GET_KERNEL_DATA(kernel_data);
   context_ptr = kernel_data->ACTIVE_PTR->PROFILER_CONTEXT_PTR;
   __PROF_INFO_PTR = context_ptr->PROF_INFO_PTR_SAVE;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_finish
* Returned Value   : 
* Comments         : Performs any profiler-specific cleanup that needs to 
*                    be done when a task is destroyed.
*
*END*-------------------------------------------------------------------------*/

void _profiler_finish
   (
      /* [IN] The profile context for this task */
      PROFILE_CONTEXT_STRUCT_PTR    context_ptr
   )
{ /* Body */

   __prof_finish();   

} /* Endbody */

/* EOF */

