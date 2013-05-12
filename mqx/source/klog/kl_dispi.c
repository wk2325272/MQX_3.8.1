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
* $FileName: kl_dispi.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the display function for the Kernel Data Logging
*   facility.  This function reads and displays one kernel log entry.
*
*END************************************************************************/

#include <stdarg.h>
#include "mqx_inc.h"
#if MQX_KERNEL_LOGGING
#include "lwlog.h"
#include "klog.h"
#include "fio.h"

extern char _PTR_ _klog_get_function_name2_internal(uint_32);

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _klog_get_function_name
* Returned Value   : char _PTR_ function name
* Comments         :
*   This function returns the address of a function name string.
*
*END*----------------------------------------------------------------------*/

/* A macro to display the name for a KLOG function entry */
#define KLOG_NAME(x) case KLOG ## x : result = #x ; break

char _PTR_ _klog_get_function_name_internal
   (
      /* [IN] the function index name */
      uint_32 index
   )
{ /* Body */
   char _PTR_ result;

   switch (index) {

      KLOG_NAME(_task_ready);
      KLOG_NAME(_task_block);
      KLOG_NAME(_task_execute_scheduler);
      KLOG_NAME(_task_create);
      KLOG_NAME(_task_create_block);
      KLOG_NAME(_task_destroy);
      KLOG_NAME(_time_dequeue);


      KLOG_NAME(_sched_setscheduler);
      KLOG_NAME(_sched_yield);
      KLOG_NAME(_sched_setparam);
      KLOG_NAME(_sched_setparam_td);
      KLOG_NAME(_sched_boost_priority);
      KLOG_NAME(_sched_unboost_priority);
      KLOG_NAME(_task_exit_function);
      KLOG_NAME(_taskq_create);
      KLOG_NAME(_taskq_destroy);
      KLOG_NAME(_taskq_suspend);
      KLOG_NAME(_taskq_resume);
      KLOG_NAME(_sched_set_policy);
      KLOG_NAME(_sched_set_rr_interval);
      KLOG_NAME(_sched_set_rr_interval_ticks);
      KLOG_NAME(_taskq_suspend_task);
      KLOG_NAME(_task_set_priority);
      KLOG_NAME(_time_dequeue_td);
      KLOG_NAME(_task_abort);
      KLOG_NAME(_task_set_environment);
      KLOG_NAME(_task_enable_fp);
      KLOG_NAME(_task_disable_fp);
      KLOG_NAME(_task_set_parameter);
      KLOG_NAME(_task_stop_preemption);
      KLOG_NAME(_task_start_preemption);
      KLOG_NAME(_task_set_exception_handler);
      KLOG_NAME(_task_set_exit_handler);
      KLOG_NAME(_taskq_test);
      KLOG_NAME(_io_set_handle);
      KLOG_NAME(_task_restart);


      KLOG_NAME(_mqx_exit);
      KLOG_NAME(_int_default_isr);
      KLOG_NAME(_task_set_error);
      KLOG_NAME(_log_test);
      KLOG_NAME(_mqx_set_exit_handler);
      KLOG_NAME(_mqx_fatal_error);


      KLOG_NAME(_msgq_close);
      KLOG_NAME(_msgpool_create_system);
      KLOG_NAME(_msgpool_create);
      KLOG_NAME(_msgpool_destroy);
      KLOG_NAME(_msg_free);
      KLOG_NAME(_msg_alloc_system);
      KLOG_NAME(_msg_alloc);
      KLOG_NAME(_msgq_open);
      KLOG_NAME(_msgq_open_system);
      KLOG_NAME(_msgq_receive);
      KLOG_NAME(_msgq_receive_for);
      KLOG_NAME(_msgq_receive_ticks);
      KLOG_NAME(_msgq_receive_until);
      KLOG_NAME(_msgq_poll);
      KLOG_NAME(_msgq_get_count);
      KLOG_NAME(_msgq_send);
      KLOG_NAME(_msgq_send_blocked);
      KLOG_NAME(_msgq_send_queue);
      KLOG_NAME(_msgq_send_urgent);
      KLOG_NAME(_msgq_send_broadcast);
      KLOG_NAME(_msgq_set_notification_function);
      KLOG_NAME(_msgq_peek);
      KLOG_NAME(_msgq_send_priority);
      KLOG_NAME(_msg_create_component);
      KLOG_NAME(_msgpool_test);
      KLOG_NAME(_msgq_test);
      /* Start CR 1944 */
      KLOG_NAME(_lwmsgq_init);
      KLOG_NAME(_lwmsgq_send);
      KLOG_NAME(_lwmsgq_receive);
      /* End CR 1944 */
      

      KLOG_NAME(_int_install_kernel_isr);
      KLOG_NAME(_int_install_isr);
      KLOG_NAME(_int_install_default_isr);
      KLOG_NAME(_int_set_isr_data);
      KLOG_NAME(_int_set_exception_handler);

      default: result = _klog_get_function_name2_internal(index); break;

   } /* Endswitch */

   return result;

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _klog_get_function_name2_internal
* Returned Value   : char _PTR_ function name
* Comments         :
*   This function returns the address of a function name string.
*
*END*----------------------------------------------------------------------*/

char _PTR_ _klog_get_function_name2_internal
   (
      /* [IN] the function index name */
      uint_32 index
   )
{ /* Body */
   char _PTR_ result;

   switch (index) {

      KLOG_NAME(_mem_free);
      KLOG_NAME(_mem_alloc);
      KLOG_NAME(_mem_alloc_from);
      KLOG_NAME(_mem_test);
      KLOG_NAME(_mem_test_pool);
      KLOG_NAME(_mem_alloc_zero);
      KLOG_NAME(_mem_alloc_system);
      KLOG_NAME(_mem_alloc_system_from);
      KLOG_NAME(_mem_alloc_system_zero);
      KLOG_NAME(_mem_alloc_system_zero_from);
      KLOG_NAME(_mem_extend);
      KLOG_NAME(_mem_extend_pool);
      KLOG_NAME(_mem_transfer);
      KLOG_NAME(_mem_free_part);
      KLOG_NAME(_mem_create_pool);

      KLOG_NAME(_lwmem_alloc);
      KLOG_NAME(_lwmem_alloc_from);
      KLOG_NAME(_lwmem_alloc_zero);
      KLOG_NAME(_lwmem_alloc_zero_from);
      KLOG_NAME(_lwmem_create_pool);
      KLOG_NAME(_lwmem_free);
      KLOG_NAME(_lwmem_alloc_system);
      KLOG_NAME(_lwmem_alloc_system_from);
      KLOG_NAME(_lwmem_transfer);
      KLOG_NAME(_lwmem_alloc_system_zero);
      KLOG_NAME(_lwmem_alloc_system_zero_from);
    
      KLOG_NAME(_time_set);
      KLOG_NAME(_time_set_ticks);
      KLOG_NAME(_time_delay);
      KLOG_NAME(_time_delay_for);
      KLOG_NAME(_time_delay_ticks);
      KLOG_NAME(_time_delay_until);
      KLOG_NAME(_timer_create_component);
      KLOG_NAME(_timer_cancel);
      KLOG_NAME(_timer_start_oneshot_after);
      KLOG_NAME(_timer_start_oneshot_at);
      KLOG_NAME(_timer_start_periodic_every);
      KLOG_NAME(_timer_start_periodic_at);
      KLOG_NAME(_timer_test);
      KLOG_NAME(_timer_start_oneshot_after_ticks);
      KLOG_NAME(_timer_start_oneshot_at_ticks);
      KLOG_NAME(_timer_start_periodic_every_ticks);
      KLOG_NAME(_timer_start_periodic_at_ticks);

      KLOG_NAME(_lwtimer_create_periodic_queue);
      KLOG_NAME(_lwtimer_add_timer_to_queue);
      KLOG_NAME(_lwtimer_cancel_period);
      KLOG_NAME(_lwtimer_cancel_timer);
      KLOG_NAME(_lwtimer_test);

      KLOG_NAME(_event_create_component);
      KLOG_NAME(_event_create);
      KLOG_NAME(_event_destroy);
      KLOG_NAME(_event_open);
      KLOG_NAME(_event_close);
      KLOG_NAME(_event_wait_all);
      KLOG_NAME(_event_wait_all_ticks);
      KLOG_NAME(_event_wait_all_for);
      KLOG_NAME(_event_wait_all_until);
      KLOG_NAME(_event_wait_any);
      KLOG_NAME(_event_wait_any_ticks);
      KLOG_NAME(_event_wait_any_for);
      KLOG_NAME(_event_wait_any_until);
      KLOG_NAME(_event_set);
      KLOG_NAME(_event_clear);
      KLOG_NAME(_event_create_fast);
      KLOG_NAME(_event_open_fast);
      KLOG_NAME(_event_destroy_fast);
      KLOG_NAME(_event_test);


      KLOG_NAME(_lwevent_clear);
      KLOG_NAME(_lwevent_create);
      KLOG_NAME(_lwevent_destroy);
      KLOG_NAME(_lwevent_set);
      KLOG_NAME(_lwevent_test);
      KLOG_NAME(_lwevent_wait);
      KLOG_NAME(_lwevent_wait_for);
      KLOG_NAME(_lwevent_wait_until);
      KLOG_NAME(_lwevent_wait_ticks);


      KLOG_NAME(_name_create_component);
      KLOG_NAME(_name_add);
      KLOG_NAME(_name_delete);
      KLOG_NAME(_name_find);
      KLOG_NAME(_name_find_name);
      KLOG_NAME(_name_test);


      KLOG_NAME(_mutatr_init);
      KLOG_NAME(_mutatr_destroy);
      KLOG_NAME(_mutatr_set_sched_protocol);
      KLOG_NAME(_mutatr_set_priority_ceiling);


      KLOG_NAME(_mutex_create_component);
      KLOG_NAME(_mutex_init);
      KLOG_NAME(_mutex_set_priority_ceiling);
      KLOG_NAME(_mutex_destroy);
      KLOG_NAME(_mutex_lock);
      KLOG_NAME(_mutex_try_lock);
      KLOG_NAME(_mutex_unlock);


      KLOG_NAME(_mutatr_set_wait_protocol);
      KLOG_NAME(_mutatr_set_spin_limit);
      KLOG_NAME(_mutex_test);


      KLOG_NAME(_sem_create_component);
      KLOG_NAME(_sem_create);
      KLOG_NAME(_sem_destroy);
      KLOG_NAME(_sem_open);
      KLOG_NAME(_sem_close);
      KLOG_NAME(_sem_post);
      KLOG_NAME(_sem_create_fast);
      KLOG_NAME(_sem_open_fast);
      KLOG_NAME(_sem_destroy_fast);
      KLOG_NAME(_sem_test);
      KLOG_NAME(_sem_wait);
      KLOG_NAME(_sem_wait_for);
      KLOG_NAME(_sem_wait_ticks);
      KLOG_NAME(_sem_wait_until);
      KLOG_NAME(_lwsem_create);
      KLOG_NAME(_lwsem_wait);
      KLOG_NAME(_lwsem_post);
      KLOG_NAME(_lwsem_destroy);
      KLOG_NAME(_lwsem_test);
      KLOG_NAME(_lwsem_poll);
      KLOG_NAME(_lwsem_wait_ticks);
      KLOG_NAME(_lwsem_wait_for);
      KLOG_NAME(_lwsem_wait_until);


      KLOG_NAME(_watchdog_create_component);
      KLOG_NAME(_watchdog_stop);
      KLOG_NAME(_watchdog_start);
      KLOG_NAME(_watchdog_start_ticks);
      KLOG_NAME(_watchdog_test);


      KLOG_NAME(_partition_create);
      KLOG_NAME(_partition_create_at);
      KLOG_NAME(_partition_alloc);
      KLOG_NAME(_partition_alloc_zero);
      KLOG_NAME(_partition_alloc_system);
      KLOG_NAME(_partition_alloc_system_zero);
      KLOG_NAME(_partition_extend);
      KLOG_NAME(_partition_free);
      KLOG_NAME(_partition_test);
      KLOG_NAME(_partition_transfer);
      KLOG_NAME(_partition_destroy);
      KLOG_NAME(_partition_create_component);


      KLOG_NAME(_eds_create_component);
      KLOG_NAME(_eds_remove);

      default: result = "Unknown function"; break;

   } /* Endswitch */

   return result;

} /* Endbody */
#endif /* MQX_KERNEL_LOGGING */

/* EOF */
