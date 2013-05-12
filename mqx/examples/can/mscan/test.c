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
* $Version : 3.6.6.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the source for the MSCAN example program.
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include <log.h>
#include <klog.h>
#include <mscan.h>
#include <lwevent.h>
#include "test.h"


#define TEST_MODULE MSCAN_ADDRESS_2


TASK_TEMPLATE_STRUCT MQX_template_list[] = {
   { MAIN_TASK, Main_Task, 2000L, 8L, "Main", MQX_AUTO_START_TASK, 0, 0 },
   { TX_TASK,   Tx_Task,   1500L, 7L, "TX",   0,                   0, 0 },
   { RX_TASK,   Rx_Task,   1500L, 7L, "RX",   0,                   0, 0 },
   { 0L,        0L,        0L,    0L, 0L,     0L,                  0L,0L}
};


LWEVENT_STRUCT event;
char    recv_data[9];
uint_32 recv_id;
uint_32 recv_len;
uint_32 recv_format;
uint_32 recv_hit;
uint_32 recv_result;


/*FUNCTION*-----------------------------------------------------------
*
* Function Name : app_mscan_isr
* Comments :
* This function demonstrates user defined mscan interrupt service routine.
*
*END*-----------------------------------------------------------*/

void app_mscan_isr
   (
   /* [IN] MSCAN base address */
   pointer mscan_addr
   )
{
   VMSCAN_REG_STRUCT_PTR    mscan_reg_ptr = (VMSCAN_REG_STRUCT_PTR) mscan_addr;
   vuint_8                  tx_buff;
   uint_8                   mask;
   
 
   /* MSCAN receive interrupt */
   if ((mscan_reg_ptr->CANRFLG & MSCAN_RXF_VALUE) == MSCAN_RXF_VALUE)
   {
      recv_result = mscan_rx_message((pointer)mscan_addr, (void *)recv_data, &recv_id,
                    &recv_len, recv_format, &recv_hit, CAN_DISABLE);
      _lwevent_set(&event, 0x01);
   } 

   /* clear transmit interrupts */
   for (tx_buff=MSCAN_TX_BUFF1;tx_buff<=MSCAN_TX_BUFF3;tx_buff++) {
      mask = 1<<tx_buff;
      if (mscan_reg_ptr->CANTFLG & mask) {
         if (mscan_reg_ptr->CANTIER & mask) {
            mscan_reg_ptr->CANTIER &= ~mask;
         }
      }
   }

   /* MSCAN Overrun interrupt */
   if ((mscan_reg_ptr->CANRFLG & 0x02) == 0x02)
   {
      mscan_reg_ptr->CANRFLG = 0x03;
   }
    
   /* MSCAN Bus error interrupt */
   if (mscan_reg_ptr->CANRFLG & 0x40) {
       mscan_reg_ptr->CANRFLG |= 0x40;
   }
   
   /* MSCAN Wakeup interrupt */
   if (mscan_reg_ptr->CANRFLG & 0x80)  {
      mscan_reg_ptr->CANRFLG |= 0x80;
   }
}



/*TASK*-----------------------------------------------------------
*
* Task Name : Main_Task
* Comments :
* 
*
*END*-----------------------------------------------------------*/
void Main_Task(uint_32 parameter){

   _task_id     created_task;
   uint_32      log_result, result;
   uint_32      sys_clk;

   _int_install_unexpected_isr();

   /* Set the variables */
   mscan1_mode           = MSCAN_NORMAL_MODE;
   bit_rate              = 250;
   format                = CAN_EXTENDED;
   identifier            = 0x12694002;
   tx_message_priority   = 0x2;
   mscan_time_stamp      = MSCAN_WITH_TIMESTAMP;
   data_len_code         = 0x1;
   interrupt             = CAN_ENABLE;
   mscan_error_interrupt = 1;
   mscan_filter_mode     = MSCAN_FILTER_32_BIT_MODE;
   mscan_filter1         = 0x00000008;
   mscan_filter2         = 0x00000010;
   mscan_mask1           = 0xFFFFFFF0;
   mscan_mask2           = 0xFFFFFF0F;
   recv_format           = format;
   
   printf("\n*****************MSCAN test program*****************\n");
   printf("\nCAN operates in %s mode", mscan1_mode == MSCAN_NORMAL_MODE ? "normal" : "special");
   printf("\nBit rate: %d kbps", bit_rate);
   printf("\n%s message format used", format == CAN_EXTENDED ? "Extended" : "Standard");
   printf("\nMessage id: 0x%08x", identifier);
   printf("\nMessage priority: %d", tx_message_priority);
   printf("\nMessage is%s time stamped", mscan_time_stamp == MSCAN_WITH_TIMESTAMP ? "" : " not");
   printf("\nData length: %d bytes", data_len_code);
   printf("\nTx/Rx interrupt %s", interrupt == CAN_ENABLE ? "enabled" : "disabled");
   printf("\nError interrupts %s", mscan_error_interrupt ? "enabled" : "disabled");
   printf("\n%d bit filter mode selected", mscan_filter_mode);
   printf("\nMessage filter1: 0x%08x", mscan_filter1);
   printf("\nMessage filter2: 0x%08x", mscan_filter2);
   printf("\nID mask1: 0x%08x", mscan_mask1);
   printf("\nID mask2: 0x%08x\n", mscan_mask2);
   printf("\n****************************************************\n");
 
    /* create the log component */
   log_result = _log_create_component();
   if (log_result != MQX_OK) 
   { 
      printf("\nCannot create log component");
      _task_block();
   }

   /* Set up an event group */
   result = _lwevent_create(&event, LWEVENT_AUTO_CLEAR);
   if (result != MQX_OK) 
   {
      printf("\nCannot create lwevent");
      _task_block();
   }   
    
   /* 
   ** Initialize MSCAN device 
   ** MSCAN clock source is IP Bus clock
   */
   result = mscan_init((pointer)TEST_MODULE, bit_rate, NULL);
   printf("\n MSCAN1 initialization. result: 0x%lx", result);

   /* Select MSCAN mode */
   result = mscan_select_mode((pointer)TEST_MODULE, mscan1_mode, interrupt);
   printf("\n MSCAN1 initialization mode. result: 0x%lx", result);

   /* Select Filtering mode */
   result = mscan_filter_init((pointer)TEST_MODULE, mscan_filter_mode,
                        mscan_filter1, mscan_filter2, mscan_mask1, mscan_mask2, format); 
   printf("\n MSCAN1 Filter selected. result: 0x%lx", result);

   /* Take MSCAN out of initialization mode */
   result = mscan_start((pointer)TEST_MODULE);
   printf("\n MSCAN1 out of initialization mode. result: 0x%lx", result);

   /* Install ISR */
   if(interrupt == CAN_ENABLE)
   {
      result = mscan_int_init( (pointer)TEST_MODULE, (pointer)app_mscan_isr );
      printf("\n MSCAN1 ISR install. result: 0x%lx", result);
   }
   
   /* Enable Timer */
   result = mscan_timestamp_select((pointer)TEST_MODULE, mscan_time_stamp);
   printf("\n MSCAN1 Time Stamp selected. result: 0x%lx", result);

   /* create tasks */
   created_task = _task_create(0, TX_TASK, 0);
   if (created_task == MQX_NULL_TASK_ID) 
   {
      printf("\nTx task: task creation failed.");
   }
   
   created_task = _task_create(0, RX_TASK, 0);
   if (created_task == MQX_NULL_TASK_ID) 
   {
      printf("\nRx task: task creation failed.");
   }

   /* create log number 0 */
   log_result = _klog_create(9600, 0);
   if (log_result != MQX_OK) 
   { 
      printf("\nlog 0 could not be created.");
   } /* endif */

#if MQX_KERNEL_LOGGING == 1
   /* define kernel logging */
   _klog_control(0xFFFFFFFF, FALSE);
   _klog_control(
      KLOG_ENABLED                  |
      KLOG_FUNCTIONS_ENABLED        |
      KLOG_INTERRUPTS_ENABLED       |
      KLOG_SYSTEM_CLOCK_INT_ENABLED |
      KLOG_CONTEXT_ENABLED          |
      KLOG_TASKING_FUNCTIONS        |
      KLOG_ERROR_FUNCTIONS          |
      KLOG_MESSAGE_FUNCTIONS        |
      KLOG_INTERRUPT_FUNCTIONS      |
      KLOG_MEMORY_FUNCTIONS         |
      KLOG_TIME_FUNCTIONS           |
      KLOG_EVENT_FUNCTIONS          |
      KLOG_NAME_FUNCTIONS           |
      KLOG_MUTEX_FUNCTIONS          |
      KLOG_SEMAPHORE_FUNCTIONS      |
      KLOG_WATCHDOG_FUNCTIONS, 
      TRUE
      );
#endif
   while(1);

}

/*TASK*-----------------------------------------------------------
*
* Task Name : Tx_Task
* Comments :
* 
*
*END*-----------------------------------------------------------*/
void Tx_Task(uint_32 parameter){
   
   uchar   data = 0;
   uint_32 result;

   if (mscan_error_interrupt)
      mscan_error_int_enable((pointer)TEST_MODULE, CAN_TX);
   
   _time_delay(100);

   while (1)
   {
   
      result = mscan_tx_message((pointer)TEST_MODULE, &data, tx_message_priority, 
                                identifier, data_len_code, format, interrupt);
      if(result != MSCAN_OK)
         printf("\nTransmit error. Error Code: 0x%lx", result);
      else
         printf("\nData transmit: %d\n", data);
      
      data++;
      if (data == 255) 
      {
         data = 0;
      }

      _time_delay(300);
   }
}

/*TASK*-----------------------------------------------------------
*
* Task Name : Rx_Task
* Comments :
* 
*
*END*-----------------------------------------------------------*/
void Rx_Task(uint_32 parameter)
{
   if (mscan_error_interrupt)
      mscan_error_int_enable((pointer)TEST_MODULE, CAN_RX);

   _time_delay(100);
   
   while (1)
   {
      if (interrupt == CAN_ENABLE)
      {
         if (_lwevent_wait_ticks(&event, 0x01, FALSE, 0) != MQX_OK) 
         {
            printf("\nEvent Wait failed");
            _task_block();
         }
      }
      else   
      {
         do 
         {
             _time_delay(300);
             recv_result = mscan_rx_message((pointer)TEST_MODULE, (void *)recv_data, &recv_id,
                           &recv_len, recv_format, &recv_hit, CAN_DISABLE);
         } while (recv_result == ERR_MSCAN_NO_MESSAGE);
      }

      if (recv_result != MSCAN_OK)
         printf("\nReceived error. Error Code: 0x%lx", recv_result);
      else
      {
         recv_data[recv_len] = 0;
         printf("\nReceived data: %s", recv_data); 
         printf("\nID is: 0x%x", recv_id);
         printf("\nDLC is: 0x%x", recv_len);
         printf("\nFilter Hit: 0x%x\n\n", recv_hit);     
      }
   }
}
