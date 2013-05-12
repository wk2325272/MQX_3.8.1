/****************************************************************************
* 
*   This file contains MQX only stationery code.
*
****************************************************************************/


#include "mqx_inc.h"
QUEUE_STRUCT MQX_QUEUE_STRUCT;
LWSEM_STRUCT MQX_LWSEM_STRUCT;

QUEUE_ELEMENT_STRUCT MQX_QUEUE_ELEMENT_STRUCT;
TASK_QUEUE_STRUCT MQX_TASK_QUEUE_STRUCT;
KERNEL_DATA_STRUCT MQX_KERNEL_DATA_STRUCT;
TD_STRUCT MQX_TD_STRUCT;
READY_Q_STRUCT MQX_READY_Q_STRUCT;
PSP_STACK_START_STRUCT MQX_PSP_STACK_START_STRUCT;

#include <bsp.h>
#include <tfs.h>
#include <rtc.h>

#if defined BSP_M52259EVB
#include <flexcan.h>
#endif

#if defined BSP_SDCARD_SPI_CHANNEL
#include <sdcard.h>
#include <sdcard_spi.h>
#endif

#if MQX_USE_NAME
#include "name.h"
#include "name_prv.h"
NAME_STRUCT MQX_NAME_STRUCT;
NAME_COMPONENT_STRUCT MQX_NAME_COMPONENT_STRUCT;
#endif

#if MQX_USE_EVENTS
#include "event.h"
#include "evnt_prv.h"

EVENT_COMPONENT_STRUCT MQX_EVENT_COMPONENT_STRUCT;
EVENT_STRUCT MQX_EVENT_STRUCT;
EVENT_CONNECTION_STRUCT MQX_EVENT_CONNECTION_STRUCT; 
#endif

#include <fio.h>
MQX_FILE MQX_FILE_STRUCT;

#include <io.h>
IO_DEVICE_STRUCT MQX_IO_DEVICE_STRUCT;

#if MQX_USE_LOGS
#include <log.h>
#include <log_prv.h>

LOG_HEADER_STRUCT MQX_LOG_HEADER_STRUCT;
LOG_COMPONENT_STRUCT MQX_LOG_COMPONENT_STRUCT;
#endif


#if MQX_USE_LWEVENTS
#include <lwevent.h>
LWEVENT_STRUCT  MQX_LWEVENT_STRUCT;
#endif

#if MQX_USE_LWLOGS
#include <lwlog.h>
#include <lwlogprv.h>
LWLOG_ENTRY_STRUCT  MQX_LWLOG_ENTRY_STRUCT;
LWLOG_HEADER_STRUCT  MQX_LWLOG_HEADER_STRUCT;
LWLOG_COMPONENT_STRUCT  MQX_LWLOG_COMPONENT_STRUCT;
#endif


#if MQX_USE_LWMEM
#include <lwmem.h>
#include <lwmemprv.h>
LWMEM_POOL_STRUCT MQX_LWMEM_POOL_STRUCT;
LWMEM_BLOCK_STRUCT MQX_LWMEM_BLOCK_STRUCT;
#endif

#if MQX_USE_LWMSGQ
#include <lwmsgq.h>
LWMSGQ_STRUCT MQX_LWMSGQ_STRUCT;
#endif

#if MQX_USE_LWTIMER
#include <lwtimer.h>
LWTIMER_STRUCT MQX_LWTIMER_STRUCT;
LWTIMER_PERIOD_STRUCT MQX_LWTIMER_PERIOD_STRUCT;
#endif

#if MQX_USE_MESSAGES
#include <message.h>
#include <msg_prv.h>
MESSAGE_HEADER_STRUCT MQX_MESSAGE_HEADER_STRUCT;
MSGPOOL_BLOCK_STRUCT MQX_MSGPOOL_BLOCK_STRUCT;
MSGPOOL_STRUCT MQX_MSGPOOL_STRUCT;
INTERNAL_MESSAGE_STRUCT MQX_INTERNAL_MESSAGE_STRUCT;
MSGQ_STRUCT MQX_MSGQ_STRUCT;
MSG_COMPONENT_STRUCT MQX_MSG_COMPONENT_STRUCT;
#endif


#if MQX_USE_IPC
#include <ipc.h>
#include <ipc_pcb.h>
#include <mqx_ioc.h>
#endif

#if MQX_USE_MUTEXES
#include <mutex.h>
#include <mutx_prv.h>
MUTEX_ATTR_STRUCT MQX_MUTEX_ATTR_STRUCT;
MUTEX_STRUCT MQX_MUTEX_STRUCT;
MUTEX_COMPONENT_STRUCT MQX_MUTEX_COMPONENT_STRUCT;
#endif

#if MQX_USE_PARTITIONS
#include <part.h>
#include <part_prv.h>
PARTPOOL_BLOCK_STRUCT MQX_PARTPOOL_BLOCK_STRUCT;
PARTPOOL_STRUCT MQX_PARTPOOL_STRUCT;
INTERNAL_PARTITION_BLOCK_STRUCT MQX_INTERNAL_PARTITION_BLOCK_STRUCT;
PARTITION_COMPONENT_STRUCT MQX_PARTITION_COMPONENT_STRUCT;
#endif

#if MQX_USE_SEMAPHORES
#include <sem.h>
#include <sem_prv.h>
SEM_COMPONENT_STRUCT MQX_SEM_COMPONENT_STRUCT;
SEM_STRUCT MQX_SEM_STRUCT;
SEM_CONNECTION_STRUCT MQX_SEM_CONNECTION_STRUCT;
#endif

#if MQX_USE_TIMER
#include <timer.h>
#include <tim_prv.h>
TIMER_ENTRY_STRUCT MQX_TIMER_ENTRY_STRUCT;
TIMER_COMPONENT_STRUCT MQX_TIMER_COMPONENT_STRUCT;
#endif

#if MQX_USE_SW_WATCHDOGS
#include <watchdog.h>
#include <wdog_prv.h>
WATCHDOG_ALARM_TABLE_STRUCT MQX_WATCHDOG_ALARM_TABLE_STRUCT;
WATCHDOG_COMPONENT_STRUCT MQX_WATCHDOG_COMPONENT_STRUCT;
#endif


#include "main.h"


const TASK_TEMPLATE_STRUCT MQX_template_list[] = 
{ 
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   1500,  9,   "main", MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0,                 }
};


static void structs() 
{
   
   _mem_free(& MQX_QUEUE_STRUCT);
   _mem_free(& MQX_LWSEM_STRUCT);

   _mem_free(& MQX_QUEUE_ELEMENT_STRUCT);
   _mem_free(& MQX_TASK_QUEUE_STRUCT);
   _mem_free(& MQX_KERNEL_DATA_STRUCT);
   _mem_free(& MQX_TD_STRUCT);
   _mem_free(& MQX_READY_Q_STRUCT);
   _mem_free(& MQX_PSP_STACK_START_STRUCT);

#if MQX_USE_NAME
   _mem_free(& MQX_NAME_STRUCT);
   _mem_free(& MQX_NAME_COMPONENT_STRUCT);
#endif

#if MQX_USE_EVENTS
   _mem_free(& MQX_EVENT_COMPONENT_STRUCT);
   _mem_free(& MQX_EVENT_STRUCT);
   _mem_free(& MQX_EVENT_CONNECTION_STRUCT);
#endif


  _mem_free(&  MQX_FILE_STRUCT);

  _mem_free(&  MQX_IO_DEVICE_STRUCT);

#if MQX_USE_LOGS
  _mem_free(&  MQX_LOG_HEADER_STRUCT);
  _mem_free(&  MQX_LOG_COMPONENT_STRUCT);
#endif

#if MQX_USE_LWEVENTS
  _mem_free(&   MQX_LWEVENT_STRUCT);
#endif

#if MQX_USE_LWLOGS
  _mem_free(&   MQX_LWLOG_ENTRY_STRUCT);
  _mem_free(&   MQX_LWLOG_HEADER_STRUCT);
  _mem_free(&   MQX_LWLOG_COMPONENT_STRUCT);
#endif


#if MQX_USE_LWMEM
  _mem_free(& MQX_LWMEM_POOL_STRUCT);
  _mem_free(& MQX_LWMEM_BLOCK_STRUCT);
#endif

#if MQX_USE_LWMSGQ
  _mem_free(& MQX_LWMSGQ_STRUCT);
#endif

#if MQX_USE_LWTIMER
  _mem_free(& MQX_LWTIMER_STRUCT);
  _mem_free(& MQX_LWTIMER_PERIOD_STRUCT);
#endif

#if MQX_USE_MESSAGES
  _mem_free(& MQX_MESSAGE_HEADER_STRUCT);
  _mem_free(& MQX_MSGPOOL_BLOCK_STRUCT);
  _mem_free(& MQX_MSGPOOL_STRUCT);
  _mem_free(& MQX_INTERNAL_MESSAGE_STRUCT);
  _mem_free(& MQX_MSGQ_STRUCT);
  _mem_free(& MQX_MSG_COMPONENT_STRUCT);
#endif

#if MQX_USE_MUTEXES
  _mem_free(& MQX_MUTEX_ATTR_STRUCT);
  _mem_free(& MQX_MUTEX_STRUCT);
  _mem_free(& MQX_MUTEX_COMPONENT_STRUCT);
#endif

#if MQX_USE_PARTITIONS
  _mem_free(& MQX_PARTPOOL_BLOCK_STRUCT);
  _mem_free(& MQX_PARTPOOL_STRUCT);
  _mem_free(& MQX_INTERNAL_PARTITION_BLOCK_STRUCT);
  _mem_free(& MQX_PARTITION_COMPONENT_STRUCT);
#endif

#if MQX_USE_SEMAPHORES
  _mem_free(& MQX_SEM_COMPONENT_STRUCT);
  _mem_free(& MQX_SEM_STRUCT);
  _mem_free(& MQX_SEM_CONNECTION_STRUCT);
#endif

#if MQX_USE_TIMER
  _mem_free(& MQX_TIMER_ENTRY_STRUCT);
  _mem_free(& MQX_TIMER_COMPONENT_STRUCT);
#endif

#if MQX_USE_SW_WATCHDOGS
  _mem_free(& MQX_WATCHDOG_ALARM_TABLE_STRUCT);
  _mem_free(& MQX_WATCHDOG_COMPONENT_STRUCT);
#endif

}

static void event(void)
{
#if MQX_USE_EVENTS
   IF_TINIEST(pointer  handle=0);
   IF_TINIEST(_mqx_uint value=0);

   IF_SMALL(_event_create_component(1, 1, 1));
   IF_SMALL(_event_create_fast(1));
   IF_SMALL(_event_create_fast_auto_clear(1));
   IF_SMALL(_event_open("e", &handle));
   IF_SMALL(_event_set(handle, 0));
   IF_SMALL(_event_clear(handle, 0));
   IF_SMALL(_event_wait_all(handle, 0, 0));

   IF_TYPICAL(_event_close(handle));
   IF_TYPICAL(_event_create("e"));
   IF_MAX(_event_create_auto_clear("e"));
   IF_TYPICAL(_event_destroy("e"));
   IF_MAX(_event_destroy_fast(1));
   IF_MAX(_event_get_value(handle, &value));
   IF_MAX(_event_get_wait_count(handle));
   IF_MAX(_event_open_fast(1, &handle));
   IF_MAX(_event_test(&handle));
   IF_MAX(_event_wait_all_for(handle, 0, NULL));
   IF_TYPICAL(_event_wait_all_ticks(handle, 0, 0));
   IF_MAX(_event_wait_all_until(handle, 0, NULL));
   IF_MAX(_event_wait_any(handle, 0, 0));
   IF_MAX(_event_wait_any_for(handle, 0, NULL));
   IF_TYPICAL(_event_wait_any_ticks(handle, 0, 0));
   IF_MAX(_event_wait_any_until(handle, 0, NULL));
#endif

}


#if CSCFG_MAX && __CODEWARRIOR__ 
static void fio(...)
#else
static void fio(void)
#endif
{
#if MQX_USE_IO
   IF_TYPICAL(char     line[4]={0};)
   IF_TYPICAL(uint_32  i=0;)
   IF_SMALL(MQX_FILE_PTR f=NULL;)
   IF_MAX(va_list  varg;)
   IF_MAX(va_start(varg, f);)
   
   IF_SMALL(_io_fopen("d", ""));
   IF_SMALL(_io_fputc('c', f));
   IF_SMALL(_io_fgetc(f));
  
   IF_MAX(_io_clearerr(f));
   IF_TYPICAL(_io_fclose(f));
   IF_TYPICAL(_io_feof(f));
   IF_TYPICAL(_io_ferror(f));
   IF_MAX(_io_fflush(f));
   IF_MAX(_io_fgetline(f, line, 4));
   IF_MAX(_io_fgets(line, 4, f));
   IF_TYPICAL(_io_fprintf(f, "%x", 0 ));
   IF_MAX(_io_fputs(line, f));
   IF_TYPICAL(_io_fscanf(f, "%x", line ));
   IF_TYPICAL(_io_fseek(f, 0, 0));
   IF_TYPICAL(_io_fstatus(f));
   IF_TYPICAL(_io_ftell(f));
   IF_MAX(_io_fungetc(i, f));
   IF_TYPICAL(_io_ioctl(f, 1, &i));
   IF_TYPICAL(_io_printf( "%x" , 0 ));
   IF_TYPICAL(_io_read(f, line, 1));
   IF_TYPICAL(_io_scanf("%x" , line ));
   IF_TYPICAL(_io_sprintf(line , "%x" , 0 ));
   IF_TYPICAL(_io_sscanf(line , "%x" , line ));
   IF_MAX(_io_vprintf("%x", varg));
   IF_MAX(_io_vfprintf(f, "%x", varg));
   IF_MAX(_io_vsprintf(line, "%x", varg));
   IF_TYPICAL(_io_write(f, line, 1));

   IF_TYPICAL(_io_getchar());
   IF_TYPICAL(_io_getline(line, 4));
   IF_TYPICAL(_io_gets(line));
   IF_TYPICAL(_io_putchar('c'));
   IF_TYPICAL(_io_puts(line));
   IF_TYPICAL(_io_status());
   IF_MAX(_io_ungetc('c'));
   
#endif
}

static void isr(pointer p) {}
static void kisr(void) {}

static void interrupts()
{
#if MQX_USE_INTERRUPTS
   _mqx_max_type  vt=0;
   pointer        m=0;

   IF_TINIEST(_int_disable());
   IF_TINIEST(_int_enable());
   
   IF_TYPICAL(_int_default_isr(0));   
#if !MQX_SPARSE_ISR_TABLE
   IF_MAX(_int_exception_isr(0));
   IF_MAX(_int_install_exception_isr());
   IF_MAX(_int_set_exception_handler(1, NULL));
   IF_MAX(_int_get_exception_handler(1));
#endif
   IF_MAX(_int_get_default_isr());
   IF_MAX(_int_get_isr(0));
   IF_MAX(_int_get_isr_data(0));
   IF_MAX(_int_get_isr_depth());
   IF_MAX(_int_get_kernel_isr(0));
   IF_MAX(_int_get_vector_table());
#if MQX_EXIT_ENABLED
   IF_MAX(_int_get_previous_vector_table());
#endif
   IF_MAX(_int_init(0, 0));
   IF_MAX(_int_install_default_isr(_int_default_isr));
   IF_MAX(_int_install_kernel_isr(0, kisr));
   IF_TYPICAL(_int_install_isr(0, isr, 0));
   IF_MAX(_int_set_isr_data(1, m));
   IF_MAX(_int_set_vector_table(vt));
#if MQX_USE_IO
   IF_MAX(_int_install_unexpected_isr());
   IF_MAX(_int_unexpected_isr(0));
#endif
#endif
}

static void io(void) 
{
 #if MQX_USE_IO
   pointer m=0;

   /* BSP Should call _io_install_dev */
   IF_MAX(_io_get_handle(1));
   IF_TYPICAL(_io_init());
   IF_TYPICAL(_io_dev_install(NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, char_ptr, char_ptr))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, char_ptr, _mqx_int))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, char_ptr, _mqx_int))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, _mqx_uint, pointer))NULL,
      NULL));
   IF_TYPICAL(_io_dev_install_ext(NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, char _PTR_, char _PTR_))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, char_ptr,  _mqx_int))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, char_ptr,  _mqx_int))NULL,
      (_mqx_int (_CODE_PTR_)(MQX_FILE_PTR, _mqx_uint, pointer))NULL,
      (_mqx_int (_CODE_PTR_)(IO_DEVICE_STRUCT_PTR))NULL,
      NULL));
   IF_MAX(_io_dev_uninstall(NULL));
   IF_MAX(_io_set_handle(1, m));
#endif
}

#if MQX_USE_IPC
const IPC_ROUTING_STRUCT _ipc_routing_table[] = { { 0, 0, 0 } };
const IPC_PROTOCOL_INIT_STRUCT _ipc_init_table[] = { { 0, 0, 0 } };
#endif



static void ipc(void)
{
#if 0 // not supported in this version
#if MQX_USE_IPC
   _event_install_ipc_handler();
   _ipc_add_io_ipc_handler((_mqx_uint (_CODE_PTR_)(pointer))NULL,0);
   _ipc_add_ipc_handler((_mqx_uint (_CODE_PTR_)(pointer))NULL,0);
   _ipc_msg_route_add(0,0,0);
   _ipc_msg_processor_route_exists(0);
   _ipc_msg_route_remove(0,0,0);
   _ipc_task(0);
   _ipc_pcb_init(NULL, NULL);
#endif
#endif
}

static void klog(void)
{
#if MQX_KERNEL_LOGGING

   IF_SMALL(_klog_create(0, 0));
   IF_MAX(_klog_create_at(0, 0, NULL));
   IF_MAX(_klog_get_interrupt_stack_usage(NULL,NULL));
   IF_MAX(_klog_get_task_stack_usage(0, NULL,NULL));
   IF_SMALL(_klog_control(0, 0));
   IF_MAX(_klog_disable_logging_task(0));
   IF_MAX(_klog_display());
   IF_MAX(_klog_enable_logging_task(0));
   IF_MAX(_klog_show_stack_usage());
#endif
}


static void log(void)
{
#if MQX_USE_LOGS
   LOG_ENTRY_STRUCT l={0};
   _mqx_uint log=0;

   IF_SMALL(_log_create(0, 0, 0));
   IF_SMALL(_log_create_component());
   IF_MAX(_log_destroy(0));
   IF_TYPICAL(_log_disable(0));
   IF_TYPICAL(_log_enable(0));
   IF_SMALL(_log_read(0, 0, 0, &l));
   IF_TYPICAL(_log_reset(0));
   IF_MAX(_log_test(&log));
   IF_SMALL(_log_write(0, 1, 0));
#endif
}


static void lwevent(void) 
{
#if MQX_USE_LWEVENTS
   IF_SMALL(_lwevent_clear(NULL, 0));
   IF_SMALL(_lwevent_create(NULL, 0));
   IF_MAX(_lwevent_destroy(NULL));
   IF_SMALL(_lwevent_set(NULL, 0));
   IF_MAX(_lwevent_test(NULL, NULL));
   IF_MAX(_lwevent_wait_for(NULL, 0, 0, 0));
   IF_SMALL(_lwevent_wait_ticks(NULL, 0, 0, 0));
   IF_MAX(_lwevent_wait_until(NULL, 0, 0, 0));
#endif
}

static void lwlog(void)
{
#if MQX_USE_LWLOGS
   LWLOG_ENTRY_STRUCT l={0};
   _mqx_uint lwlog=0;

   IF_MAX(_lwlog_calculate_size(0));
   IF_SMALL(_lwlog_create(0, 0, 0));
   IF_MAX(_lwlog_create_at(0, 0, 0, NULL));
   IF_SMALL(_lwlog_create_component());
   IF_MAX(_lwlog_destroy(0));
   IF_TYPICAL(_lwlog_disable(0));
   IF_TYPICAL(_lwlog_enable(0));
   IF_SMALL(_lwlog_read(0, 0, &l));
   IF_TYPICAL(_lwlog_reset(0));
   IF_MAX(_lwlog_test(&lwlog));
   IF_SMALL(_lwlog_write(1,1,2,3,4,5,6,7));

#endif
}

static void lwmem(void) 
{
#if MQX_USE_LWMEM
   _lwmem_pool_id p=0;
   _task_id       t=0;
   pointer        td_ptr=0;

   IF_SMALL(_lwmem_alloc(0));
   IF_TYPICAL(_lwmem_alloc_zero(0));
   IF_MAX(_lwmem_alloc_from(p, 0));
   IF_MAX(_lwmem_alloc_zero_from(p, 0));
   IF_TYPICAL(_lwmem_alloc_system(0));
   IF_TYPICAL(_lwmem_alloc_system_zero(0));
   IF_MAX(_lwmem_alloc_system_from(p, 0));
   IF_MAX(_lwmem_alloc_system_zero_from(p, 0));
   IF_SMALL(_lwmem_create_pool(NULL, NULL, 0));
   IF_SMALL(_lwmem_free(NULL));
   IF_MAX(_lwmem_get_size(NULL));
   IF_MAX(_lwmem_set_default_pool(p));
   IF_MAX(_lwmem_test(&p, NULL));
   IF_MAX(_lwmem_transfer(p, t, t));
#endif
}


static void lwmsgq(void) 
{
#if MQX_USE_LWMSGQ
   IF_SMALL(_lwmsgq_init(NULL, 0, 0));
   IF_SMALL(_lwmsgq_send(NULL, NULL, 0));
   IF_SMALL(_lwmsgq_receive(NULL, NULL, 0, 0, NULL));      
#endif
}

static void lwsem(void)
{
   LWSEM_STRUCT lwsem={0};
   pointer      p=0;
   
   IF_TINIEST(_lwsem_create(&lwsem, 1));
   IF_MAX(_lwsem_destroy(&lwsem));
   IF_TYPICAL(_lwsem_poll(&lwsem));
   IF_TINIEST(_lwsem_post(&lwsem));
   IF_MAX(_lwsem_test(&p, &p));
   IF_TINIEST(_lwsem_wait(&lwsem));
   IF_TYPICAL(_lwsem_wait_ticks(&lwsem, 1));
   IF_MAX(_lwsem_wait_for(&lwsem, NULL));
   IF_MAX(_lwsem_wait_until(&lwsem, NULL));   
}

static void lwtimer(void) 
{
#if MQX_USE_LWTIMER
   IF_SMALL(_lwtimer_add_timer_to_queue(NULL, NULL, 0,
      (void (_CODE_PTR_)(pointer))NULL, NULL));
   IF_MAX(_lwtimer_cancel_period(NULL));
   IF_MAX(_lwtimer_cancel_timer(NULL));
   IF_SMALL(_lwtimer_create_periodic_queue(NULL, 0, 0));
   IF_MAX(_lwtimer_test(NULL, NULL));
#endif
}

static void mem(void)
{
#if MQX_USE_MEM
   _mem_pool_id   mpid=0;
   pointer        m=NULL;
   _task_id       t=0;
   
   IF_SMALL(_mem_alloc(0));
   IF_MAX(_mem_alloc_from(mpid, 0));
   IF_TYPICAL(_mem_alloc_zero(0));
   IF_MAX(_mem_alloc_zero_from(mpid,0));
   IF_TYPICAL(_mem_alloc_system(0));
   IF_MAX(_mem_alloc_system_from(mpid,0));
   IF_TYPICAL(_mem_alloc_system_zero(0));
   IF_MAX(_mem_alloc_system_zero_from(mpid,0));
   IF_TYPICAL(_mem_copy(m, m, 0));
   IF_MAX(_mem_create_pool(NULL, 0));
   IF_MAX(_mem_extend(m, 0));
   IF_MAX(_mem_extend_pool(mpid, m, 0));
   IF_SMALL(_mem_free(m));
   IF_MAX(_mem_free_part(m, 0));
   IF_MAX(_mem_get_error());
   IF_MAX(_mem_get_error_pool(mpid));
   IF_MAX(_mem_get_highwater());
   IF_MAX(_mem_get_highwater_pool(mpid));
   IF_MAX(_mem_get_size(m));
   IF_MAX(_mem_sum_ip(0, 0, m));
   IF_MAX(_mem_swap_endian(0, 0));
   IF_MAX(_mem_swap_endian_len(0, 0, 0));
   IF_MAX(_mem_test());
   IF_MAX(_mem_test_all(&mpid));
   IF_MAX(_mem_test_pool(mpid));
   IF_MAX(_mem_test_and_set(0));
   IF_MAX(_mem_transfer(m, t, t));
   IF_TYPICAL(_mem_zero(m, 0));

#endif
}

static void msg_func(pointer p){}
static void message(void)
{
#if MQX_USE_MESSAGES
   _pool_id  p=0;
   _queue_id q=0;
   MESSAGE_HEADER_STRUCT_PTR m=0;
   uchar     c[4]={0};
   pointer   n=0;
   void (_CODE_PTR_ _PTR_ f)(pointer)=NULL;

   IF_SMALL(_msg_alloc(p));
   IF_MAX(_msg_alloc_system(0));
   IF_MAX(_msg_available(p));
   IF_SMALL(_msg_create_component());
   IF_SMALL(_msg_free(m));
   IF_MAX(_msg_swap_endian_data(c, m));
   IF_MAX(_msg_swap_endian_header(m));

   IF_SMALL(_msgpool_create(0, 0, 0, 0));
   IF_MAX(_msgpool_create_system(0, 0, 0, 0));
   IF_MAX(_msgpool_destroy(p));
   IF_MAX(_msgpool_test(&n, &n));

   IF_MAX(_msgq_close(q));
   IF_MAX(_msgq_get_count(q));
   IF_MAX(_msgq_get_id(0, 0));
   IF_MAX(_msgq_get_owner(q));
   IF_MAX(_msgq_get_notification_function(q, f, &n));
   IF_SMALL(_msgq_open(0, 0));
   IF_MAX(_msgq_open_system(0, 0, msg_func, 0));
   IF_MAX(_msgq_poll(q));
   IF_MAX(_msgq_peek(q));
   IF_MAX(_msgq_receive(q, 0));
   IF_MAX(_msgq_receive_for(q, NULL));
   IF_SMALL(_msgq_receive_ticks(q, 0));
   IF_MAX(_msgq_receive_until(q, NULL));
   IF_SMALL(_msgq_send(m));
   IF_MAX(_msgq_send_broadcast(m, &q, p));
   IF_MAX(_msgq_send_queue(m, q));
   IF_MAX(_msgq_send_urgent(m));
   IF_MAX(_msgq_send_priority(m, 1));
   IF_MAX(_msgq_set_notification_function(q, (void (_CODE_PTR_)(pointer))NULL, m));
   IF_MAX(_msgq_test(&n, &n));
#endif
}


static void mmu(void) 
{
    #ifdef __mmu_h__
    if (PSP_HAS_MMU) {
       _task_id t;

       _mmu_add_region(NULL, 0, 0);
       _mmu_disable();
       _mmu_enable();
       _mmu_init(NULL);
       _mmu_add_vregion(NULL, NULL, 0, 0);
       _mmu_get_vpage_size();
       _mmu_get_vmem_attributes(NULL, NULL, NULL, NULL, NULL);
       _mmu_set_vmem_attributes(NULL, 0, 0);
       _mmu_vinit(0, NULL);
       _mmu_venable();
       _mmu_vdisable();
       _mmu_vtop(NULL, NULL);
       _mmu_add_vcontext(t, NULL, 0, 0);
       _mmu_create_vcontext(t);
       _mmu_create_vtask(0, 0, NULL, NULL, 0, 0);
       _mmu_destroy_vcontext(t);
    } /* Endif */
    #endif
}

static void mqx(void)
{
   IF_MAX(_mqx_exit(1));
   IF_MAX(_mqx_fatal_error(0));
#if MQX_KD_HAS_COUNTER
   IF_MAX(_mqx_get_counter());
#endif
   IF_MAX(_mqx_get_cpu_type());
   IF_MAX(_mqx_get_initialization());
   IF_MAX(_mqx_get_kernel_data());
   IF_MAX(_mqx_get_exit_handler());
   IF_MAX(_mqx_get_system_task_id());
#if  MQX_TAD_RESERVED_ENABLE
   IF_MAX(_mqx_get_tad_data(NULL));
   IF_MAX(_mqx_set_tad_data(NULL, NULL));
#endif

   //IF_MAX(_mqx_set_cpu_type(0));
   IF_MAX(_mqx_set_exit_handler((void (_CODE_PTR_)(void))NULL));

#if MQX_IO_COMPONENT_DESTRUCTION
   IF_MAX(_mqx_get_io_component_cleanup(0));
   IF_MAX(_mqx_set_io_component_cleanup(0, NULL));
#endif
#if MQX_USE_IO_COMPONENTS
   IF_MAX(_mqx_get_io_component_handle(0));
   IF_MAX(_mqx_set_io_component_handle(0, NULL));
#endif
}


static void mutex(void)
{
#if MQX_USE_MUTEXES
   MUTEX_ATTR_STRUCT a={0};
   MUTEX_STRUCT      m={0};
   pointer           mp=0;
   _mqx_uint          p=0;

   IF_MAX(_mutatr_destroy(&a));
   IF_MAX(_mutatr_get_priority_ceiling(&a, &p));
   IF_MAX(_mutatr_get_sched_protocol(&a, &p));
   IF_MAX(_mutatr_get_spin_limit(&a, &p));
   IF_MAX(_mutatr_get_wait_protocol(&a, &p));
   IF_MAX(_mutatr_get_priority_ceiling(&a, &p));
   IF_TYPICAL(_mutatr_set_sched_protocol(&a, p));
   IF_MAX(_mutatr_set_spin_limit(&a, p));
   IF_TYPICAL(_mutatr_set_wait_protocol(&a, p));
   IF_SMALL(_mutatr_init(&a));

   IF_SMALL(_mutex_create_component());
   IF_MAX(_mutex_destroy(&m));
   IF_MAX(_mutex_get_priority_ceiling(&m, &p));
   IF_MAX(_mutex_get_wait_count(&m));
   IF_SMALL(_mutex_init(&m, &a));
   IF_SMALL(_mutex_lock(&m));
   IF_MAX(_mutex_set_priority_ceiling(&m, p, &p));
   IF_MAX(_mutex_test(&mp));
   IF_TYPICAL(_mutex_try_lock(&m));
   IF_SMALL(_mutex_unlock(&m));
#endif

}

static void name(void)
{
#if MQX_USE_NAME
   _mqx_max_type value=0;
   pointer    mp=0;
   char    s[4]={0};

   IF_TYPICAL(_name_create_component(0, 0, 0));
   IF_TYPICAL(_name_add("", 0));
   IF_TYPICAL(_name_delete(""));
   IF_TYPICAL(_name_find("", &value));
   IF_MAX(_name_find_by_number(0, s));
   IF_MAX(_name_test(&mp, &mp));
#endif

}


static void part(void)
{
#if MQX_USE_PARTITIONS
   _partition_id p=0;
   pointer       m=0;
   _task_id      t=0;

   IF_SMALL(_partition_alloc(p));
   IF_TYPICAL(_partition_alloc_system(p));
   IF_TYPICAL(_partition_alloc_system_zero(p));
   IF_TYPICAL(_partition_alloc_zero(p));
   IF_MAX(_partition_calculate_size(0,0));
   IF_MAX(_partition_calculate_blocks(0,0));
   IF_SMALL(_partition_create_component());
   IF_SMALL(_partition_create(0, 0, 0, 0));
   IF_MAX(_partition_create_at(0, 0, 0));
   IF_MAX(_partition_destroy(p));
   IF_MAX(_partition_extend(p, 0, 0));
   IF_SMALL(_partition_free(m));
   IF_MAX(_partition_get_free_blocks(p));
   IF_MAX(_partition_get_max_used_blocks(p));
   IF_MAX(_partition_get_total_blocks(p));
   IF_MAX(_partition_get_block_size(p));
   IF_MAX(_partition_get_total_size(p));
   IF_MAX(_partition_test(&p, &m, &m));
   IF_MAX(_partition_transfer(m, t));
#endif
}


static void psp(void)
{
#if defined _CACHE_ENABLE
   IF_TYPICAL(_CACHE_ENABLE(0));
#endif
#if defined _CACHE_ENABLE
   IF_TYPICAL(_CACHE_DISABLE());
#endif
#if defined _DCACHE_ENABLE
   IF_TYPICAL(_DCACHE_ENABLE(0));
#endif
#if defined _DCACHE_ENABLE
   IF_TYPICAL(_DCACHE_DISABLE());
#endif
   IF_TYPICAL(_DCACHE_FLUSH());
   IF_TYPICAL(_DCACHE_FLUSH_LINE(NULL));
   IF_TYPICAL(_DCACHE_FLUSH_MBYTES(NULL, 0));
   IF_TYPICAL(_DCACHE_INVALIDATE());
   IF_TYPICAL(_DCACHE_INVALIDATE_LINE(NULL));       
   IF_TYPICAL(_DCACHE_INVALIDATE_MBYTES(NULL, 0));  

   IF_MAX(_ICACHE_INVALIDATE());
   IF_MAX(_ICACHE_INVALIDATE_LINE(NULL));
   IF_MAX(_ICACHE_INVALIDATE_MBYTES(NULL, 0));
}



static void queues(void)
{
   QUEUE_STRUCT q={0};
   pointer      m=0;
   
   IF_SMALL(_queue_dequeue(&q));
   IF_SMALL(_queue_enqueue(&q, m));
   IF_TYPICAL(_queue_get_size(&q));
   IF_SMALL(_queue_init(&q, 1));
   IF_MAX(_queue_insert(&q, m, m));
   IF_TYPICAL(_queue_is_empty(&q));
   IF_MAX(_queue_head(&q));
   IF_MAX(_queue_next(&q, m));
   IF_MAX(_queue_unlink(&q, m));
   IF_MAX(_queue_test(&q, m));
}

static void sem(void)
{
#if MQX_USE_SEMAPHORES
   pointer sem=NULL;

   IF_MAX(_sem_close(sem));
   IF_MAX(_sem_create("", 0, 0));
   IF_SMALL(_sem_create_fast(1, 0, 0));
   IF_SMALL(_sem_create_component(0, 0, 0));
   IF_MAX(_sem_destroy("", 0));
   IF_TYPICAL(_sem_destroy_fast(1, 0));
   IF_MAX(_sem_get_wait_count(sem));
   IF_MAX(_sem_open("", &sem));
   IF_SMALL(_sem_open_fast(1, &sem));
   IF_SMALL(_sem_post(sem));
   IF_MAX(_sem_test(&sem));
   IF_MAX(_sem_get_value(sem));
   IF_SMALL(_sem_wait(sem, 0));
   IF_TYPICAL(_sem_wait_for(sem, NULL));
   IF_TYPICAL(_sem_wait_ticks(sem, 0));
   IF_MAX(_sem_wait_until(sem, NULL));
#endif
}


static void sched(void)
{
   _mqx_uint         p=0;
    uint_32          u=0;
   _task_id          t=0;
   MQX_TICK_STRUCT   ticks={0};
   
   IF_MAX(_sched_get_max_priority(0));
   IF_MAX(_sched_get_min_priority(0));
   IF_MAX(_sched_get_policy(t, &p));
#if MQX_HAS_TIME_SLICE
   IF_MAX(_sched_get_rr_interval(t, &u));
   IF_MAX(_sched_get_rr_interval_ticks(t, &ticks));
   IF_MAX(_sched_set_policy(t, 1));
   IF_MAX(_sched_set_rr_interval(t, 1));
   IF_MAX(_sched_set_rr_interval_ticks(t, &ticks, &ticks));
#endif
   IF_TYPICAL(_sched_yield());
}

static void string(void)
{
   // All functions under 'string' are pulled in by other functions   
}


static void task(void)
{
   _task_id  t=0;
   _mqx_uint p=0;
   pointer   m=0;
      
   IF_MAX(_task_abort(t));
   IF_TYPICAL(_task_block());
   IF_MAX(_task_check_stack());
   IF_TYPICAL(_task_create(0, 0, 0));
   IF_MAX(_task_create_at(0, 0, 0, NULL, 0));
   IF_MAX(_task_create_blocked(0, 0, 0));
   IF_TYPICAL(_task_destroy(t));
#if PSP_HAS_FPU
   IF_MAX(_task_disable_fp());
   IF_MAX(_task_enable_fp());
#endif
#if MQX_TD_HAS_PARENT
   IF_MAX(_task_get_creator());
#endif
#if MQX_HAS_TASK_ENVIRONMENT
   IF_MAX(_task_get_environment(t));
   IF_MAX(_task_set_environment(t, m));
#endif
#if MQX_HAS_EXCEPTION_HANDLER
   IF_MAX(_task_get_exception_handler(t));
   IF_MAX(_task_set_exception_handler(t, (void (_CODE_PTR_)(uint_32,pointer))NULL));
#endif
#if MQX_TD_HAS_ERROR_CODE
   IF_TYPICAL(_task_get_error());
   IF_MAX(_task_get_error_ptr());
   IF_MAX(_task_set_error(0));
#endif
#if MQX_HAS_EXIT_HANDLER
   IF_MAX(_task_get_exit_handler(t));
   IF_MAX(_task_set_exit_handler(t, (void (_CODE_PTR_)(void))NULL));
#endif
   IF_MAX(_task_get_id());
#if MQX_TD_HAS_TASK_TEMPLATE_PTR
   IF_MAX(_task_get_id_from_name(""));
   IF_MAX(_task_get_index_from_id(t));
   IF_MAX(_task_get_template_index(""));
   IF_MAX(_task_get_template_ptr(0));
   IF_MAX(_task_restart(t, NULL, 0));
#endif   
   IF_MAX(_task_get_parameter());
   IF_MAX(_task_get_parameter_for(t));
   IF_MAX(_task_get_processor(t));
#if MQX_HAS_DYNAMIC_PRIORITIES
   IF_MAX(_task_get_priority(t, &p));
   IF_MAX(_task_set_priority(t, p, &p));
#endif   
   IF_MAX(_task_get_td(t));
   IF_MAX(_task_ready(m));
   IF_MAX(_task_set_parameter(0));
   IF_MAX(_task_set_parameter_for(0, t));
   IF_MAX(_task_stop_preemption());
   IF_MAX(_task_start_preemption());

}

static void taskq(void)
{
    pointer  tq=0;
    _task_id t=0;
    
   IF_SMALL(_taskq_create(0));
   IF_MAX(_taskq_destroy(tq));
   IF_SMALL(_taskq_resume(tq, 0));
   IF_SMALL(_taskq_suspend(tq));
   IF_MAX(_taskq_suspend_task(t, tq));
   IF_MAX(_taskq_test(&tq, &tq));
   IF_TYPICAL(_taskq_get_value(tq));
}

static void time(void)
{
   pointer  m=0;
   _task_id t=0;
   TIME_STRUCT        time={0};   
   MQX_TICK_STRUCT    ticks={0};   
   MQX_XDATE_STRUCT   xdate={0}; 

   // All PSP_* time functions except this one are called by other APIs
   PSP_GET_ELAPSED_MILLISECONDS();
   
   IF_MAX(_time_add_day_to_ticks(NULL, 0));
   IF_MAX(_time_add_hour_to_ticks(NULL, 0));
   IF_MAX(_time_add_min_to_ticks(NULL, 0));
   IF_MAX(_time_add_msec_to_ticks(NULL, 0));
   IF_MAX(_time_add_nsec_to_ticks(NULL, 0));
   IF_MAX(_time_add_sec_to_ticks(NULL, 0));
   IF_MAX(_time_add_usec_to_ticks(NULL, 0));
   IF_MAX(_time_add_psec_to_ticks(NULL, 0));
   IF_TYPICAL(_time_delay(0));
   IF_SMALL(_time_delay_ticks(0));
   IF_MAX(_time_delay_for(NULL));
   IF_MAX(_time_delay_until(NULL));
   IF_MAX(_time_dequeue(t));
   IF_MAX(_time_dequeue_td(m));
   IF_TYPICAL(_time_diff(NULL, NULL, NULL));
   IF_MAX(_time_diff_days(NULL, NULL, NULL));
   IF_MAX(_time_diff_hours(NULL, NULL, NULL));
   IF_MAX(_time_diff_minutes(NULL, NULL, NULL));
   IF_MAX(_time_diff_milliseconds(NULL, NULL, NULL));
   IF_MAX(_time_diff_nanoseconds(NULL, NULL, NULL));
   IF_MAX(_time_diff_picoseconds(NULL, NULL, NULL));
   IF_MAX(_time_diff_seconds(NULL, NULL, NULL));
   IF_MAX(_time_diff_microseconds(NULL, NULL, NULL));
   IF_MAX(_time_diff_ticks(NULL, NULL, NULL));
   IF_TYPICAL(_time_from_date(m, &time));
   IF_TYPICAL(_time_get(&time));
   IF_TYPICAL(_time_get_elapsed(&time));
   IF_MAX(_time_get_elapsed_ticks(&ticks));
   IF_MAX(_time_get_hwticks());
   IF_MAX(_time_get_hwticks_per_tick());
   IF_MAX(_time_get_microseconds());
   IF_MAX(_time_get_nanoseconds());
   IF_MAX(_time_get_resolution());
   IF_MAX(_time_get_ticks(&ticks));
   IF_MAX(_time_get_ticks_per_sec());
   IF_MAX(_time_init_ticks(NULL, 0));
   IF_MAX(_time_normalize_xdate(NULL));
   IF_TYPICAL(_time_notify_kernel());
   IF_TYPICAL(_time_set(&time));
   IF_MAX(_time_set_hwticks_per_tick(0));
   IF_MAX(_time_set_resolution(0));
   IF_MAX(_time_set_ticks(&ticks));
   IF_MAX(_time_set_ticks_per_sec(0));
   IF_MAX(_time_set_timer_vector(0));
   IF_MAX(_time_ticks_to_xdate(&ticks, &xdate));
   IF_MAX(_time_to_date(&time, m));
   IF_MAX(_time_xdate_to_ticks(&xdate, &ticks));
   IF_MAX(_time_to_ticks(&time, &ticks));
   IF_MAX(_ticks_to_time(&ticks, &time));
}

static void timer_func(uint_32 x1, void* x2, MQX_TICK_STRUCT_PTR x3)
{
}

static void timer_func2(uint_32 x1, void* x2, uint_32 x3, uint_32 x4)
{
}

static void timer(void)
{
#if MQX_USE_TIMER
   TIME_STRUCT        time={0};   
   IF_TYPICAL(_timer_cancel(0));
   IF_SMALL(_timer_create_component(1, 4000));
   IF_MAX(_timer_start_oneshot_after(timer_func2, 0, 0, 0));
   IF_TYPICAL(_timer_start_oneshot_after_ticks(timer_func, 0, 0, 0));
   IF_MAX(_timer_start_oneshot_at(timer_func2, 0, 0, NULL));
   IF_MAX(_timer_start_oneshot_at_ticks(timer_func, 0, 0, NULL));
   IF_MAX(_timer_start_periodic_every(timer_func2, 0, 0, 0));
   IF_SMALL(_timer_start_periodic_every_ticks(timer_func, 0, 0, 0));
   IF_MAX(_timer_start_periodic_at(timer_func2, NULL, 0, &time, 0));
   IF_MAX(_timer_start_periodic_at_ticks(timer_func, 0, 0, NULL, 0));
   IF_MAX(_timer_test(NULL));
#endif
}

static void err_func(pointer p) { }


static void watchdog(void)
{
#if MQX_USE_SW_WATCHDOGS

   IF_TYPICAL(_watchdog_create_component(0, err_func));
   IF_TYPICAL(_watchdog_stop());
   IF_MAX(_watchdog_start(0));
   IF_TYPICAL(_watchdog_start_ticks(0));
   IF_MAX(_watchdog_test(NULL,NULL));
#endif
}



static void enet(void)
{
   IF_TYPICAL(ENET_initialize(0,0,0,0));
   IF_TYPICAL(ENET_open(0,0,0,0));
}


static void tfs(void)
{
   IF_TYPICAL(_io_tfs_install("tfs:", NULL));
}


static void flexcan(void)
{
#if defined BSP_M52259EVB
   IF_TYPICAL(_bsp_get_flexcan_base_address(0));
   IF_TYPICAL(_bsp_get_flexcan_vector (0,0,0));
   IF_TYPICAL(_bsp_flexcan_io_init(0));
   IF_TYPICAL(FLEXCAN_Softreset(0));
   IF_TYPICAL(FLEXCAN_Start(0));
   IF_TYPICAL(FLEXCAN_Get_msg_object(0,0));
   IF_TYPICAL(FLEXCAN_Select_mode(0,0));
   IF_TYPICAL(FLEXCAN_Select_clk(0,0));
   IF_TYPICAL(FLEXCAN_Initialize(0,0,0,0,0));
   IF_TYPICAL(FLEXCAN_Initialize_mailbox(0,0,0,0,0,0,0));
   IF_TYPICAL(FLEXCAN_Request_mailbox(0,0,0));
   IF_TYPICAL(FLEXCAN_Activate_mailbox(0,0,0));
   IF_TYPICAL(FLEXCAN_Lock_mailbox(0,0));
   IF_TYPICAL(FLEXCAN_Unlock_mailbox(0));
   IF_TYPICAL(FLEXCAN_Set_global_extmask(0,0));
   IF_TYPICAL(FLEXCAN_Set_buf14_extmask(0,0));
   IF_TYPICAL(FLEXCAN_Set_buf15_extmask(0,0));
   IF_TYPICAL(FLEXCAN_Set_global_stdmask(0,0));
   IF_TYPICAL(FLEXCAN_Set_buf14_stdmask(0,0));
   IF_TYPICAL(FLEXCAN_Set_buf15_stdmask(0,0));
   IF_TYPICAL(FLEXCAN_Tx_successful(0));
   IF_TYPICAL(FLEXCAN_Tx_mailbox(0,0,NULL));
   IF_TYPICAL(FLEXCAN_Rx_mailbox(0,0,NULL));
   IF_TYPICAL(FLEXCAN_Disable_mailbox(0,0));
   IF_TYPICAL(FLEXCAN_Request_message(0,0,0));
   IF_TYPICAL(FLEXCAN_Rx_message(0,0,NULL,0,NULL,NULL,0));
   IF_TYPICAL(FLEXCAN_Tx_message(0,0,0,0,0,NULL));
   IF_TYPICAL(FLEXCAN_Read(0,0,NULL));
   IF_TYPICAL(FLEXCAN_Write(0,0,0));
   IF_TYPICAL(FLEXCAN_Get_status(0,NULL));
   IF_TYPICAL(FLEXCAN_Update_message(0,NULL,0,0,0));
   IF_MAX(FLEXCAN_Int_enable(0,0));
   IF_MAX(FLEXCAN_Error_int_enable(0));
   IF_MAX(FLEXCAN_Int_disable(0,0));
   IF_MAX(FLEXCAN_Error_int_disable(0));
   IF_MAX(FLEXCAN_Install_isr(0,0,NULL));
   IF_MAX(FLEXCAN_Install_isr_err_int(0,NULL));
   IF_MAX(FLEXCAN_Install_isr_boff_int(0,NULL));
   IF_MAX(FLEXCAN_Install_isr_wake_int(0,NULL));
   IF_MAX(FLEXCAN_Int_status(0));
#endif
}


static void rtc(void)
{
   IF_TYPICAL(_bsp_rtc_io_init ());
   IF_TYPICAL(_rtc_init (0));
   IF_MAX(_rtc_int_install (NULL));
   IF_MAX(_rtc_int_enable (FALSE, 0));
   IF_MAX(_rtc_clear_requests (0));
   IF_MAX(_rtc_get_status ());
   IF_MAX(_rtc_set_time (NULL));
   IF_MAX(_rtc_get_time (NULL));
   IF_MAX(_rtc_set_alarm (NULL));
   IF_MAX(_rtc_get_alarm (NULL));
   IF_MAX(_rtc_set_stopwatch (0));
   IF_MAX(_rtc_get_stopwatch ());
   IF_TYPICAL(_rtc_time_to_mqx_time (NULL, NULL));
   IF_TYPICAL(_rtc_time_from_mqx_time (NULL, NULL));
   IF_TYPICAL(_rtc_sync_with_mqx (FALSE));
}

static void sdcard(void) 
{
#if defined BSP_SDCARD_SPI_CHANNEL
   IF_TYPICAL(_io_sdcard_install("sdcard:", (pointer)&_bsp_sdcard0_init, NULL));
#endif
}

/*TASK*-----------------------------------------------------
* 
* Task Name    : Main_task
* Comments     :
*
*END*-----------------------------------------------------*/

void Main_task(uint_32 initial_data)
{

   event();     
   fio();
   interrupts();
   io();
   ipc();       
   klog();      
   log();       
   lwevent();   
   lwlog();     
   lwmem();     
   lwmsgq();    
   lwsem();
   lwtimer();   
   mem();
   message();   
   mqx();
   mutex();     
   name();      
   part();      
   psp();
   queues();
   sem();       
   sched();
   string();    
   task();
   taskq();
   time();
   timer();     
   watchdog(); 
   
   enet();
   tfs();
   flexcan();
   rtc();   
   sdcard();
   
   structs();
 
} 

/* EOF */
