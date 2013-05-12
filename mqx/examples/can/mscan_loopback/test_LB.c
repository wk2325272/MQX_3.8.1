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
* $FileName: test_LB.c$
* $Version : 3.6.3.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   
*
*END************************************************************************/
#include <mqx.h>
#include <bsp.h>
#include <mscan.h>


#define MAIN_TASK   1

#define TEST_MODULE   MSCAN_ADDRESS_1

void Main_Task(uint_32);


const TASK_TEMPLATE_STRUCT MQX_template_list[] = {
  /* Task Index,  Function,  Stack,  Priority,    Name,       Attributes,             Param,  Time Slice */
   { MAIN_TASK,   Main_Task, 2000L,  7L,         "Can demo",  MQX_AUTO_START_TASK,    0,      0           },
   { 0 }
};


char    recv_data[9];
uint_32 recv_id;
uint_32 recv_len;
uint_32 recv_format;
uint_32 recv_hit;
uint_32 recv_result;


/*FUNCTION*-----------------------------------------------------------
*
* Function Name : mscan_test_isr
* Comments :
* This function demonstrates user defined mscan interrupt service routine.
*
*END*-----------------------------------------------------------*/

void mscan_test_isr
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
* This file tests functions in the MSCAN library.
*
*END*-----------------------------------------------------------*/
void Main_Task(uint_32 parameter)
{

   uint_32      result;
   char         data_ptr[80];
   uint_32      mscan_mode = MSCAN_LOOPBACK_MODE;
   uint_32      format = CAN_STANDARD;
   uint_32      bit_rate = 250;
   uint_32      interrupt = CAN_DISABLE;
   uint_32      mscan_error_interrupt = 0;
   uint_32      mscan_time_stamp = MSCAN_WITH_TIMESTAMP;
   uint_32      identifier = 0x392;
   uint_32      data_len_code = 8;
   uint_32      filter_hit;
   uint_32      priority = 2;
   uint_32      mscan_filter_mode = MSCAN_FILTER_16_BIT_MODE;
   uint_32      filter1 = 0x4AE0;
   uint_32      filter2 = 0x7EC0;
   uint_32      mask1 = 0x0260;
   uint_32      mask2 = 0x1C80;
   recv_format = format;
 
   _int_install_unexpected_isr();
   
   printf("\n*****************MSCAN test program*****************\n");
   printf("\nCAN operates in %s mode", mscan_mode == MSCAN_LOOPBACK_MODE ? "loopback" : "non loopback");
   printf("\nBit rate: %d kbps", bit_rate);
   printf("\n%s message format used", format == CAN_EXTENDED ? "Extended" : "Standard");
   printf("\nMessage id: 0x%08x", identifier);
   printf("\nMessage priority: %d", priority);
   printf("\nMessage is%s time stamped", mscan_time_stamp == MSCAN_WITH_TIMESTAMP ? "" : " not");
   printf("\nData length: %d bytes", data_len_code);
   printf("\nTx/Rx interrupt %s", interrupt == CAN_ENABLE ? "enabled" : "disabled");
   printf("\nError interrupts %s", mscan_error_interrupt ? "enabled" : "disabled");
   printf("\n%d bit filter mode selected", mscan_filter_mode);
   printf("\nMessage filter1: 0x%08x", filter1);
   printf("\nMessage filter2: 0x%08x", filter2);
   printf("\nID mask1: 0x%08x", mask1);
   printf("\nID mask2: 0x%08x\n", mask2);
   printf("\n****************************************************\n");

   /* Initialize MSCAN device at given bit rate */
   result = mscan_init((pointer)TEST_MODULE, bit_rate, NULL);
   printf("\n MSCAN1 initialization. result: 0x%lx", result);

   /* Initialize MSCAN mode */
   result = mscan_select_mode((pointer)TEST_MODULE, mscan_mode, interrupt);
   printf("\n MSCAN1 initialization mode. result: 0x%lx", result);

   /* Select Filtering mode */
   result = mscan_filter_init((pointer)TEST_MODULE, mscan_filter_mode,
                        filter1, filter2, mask1, mask2, format);
   printf("\n MSCAN1 Filter selected. result: 0x%lx", result);

   /* Take MSCAN out of initialization mode */
   result = mscan_start((pointer)TEST_MODULE);
   printf("\n MSCAN1 out of initialization mode. result: 0x%lx", result);
   
   /* Install ISR */
   if(interrupt == CAN_ENABLE)
   {
       result = mscan_int_init( (pointer)TEST_MODULE, (pointer)mscan_test_isr );
       printf("\n MSCAN1 ISR install. result: 0x%lx", result);
   }

   /* Enable Timer */
   result = mscan_timestamp_select((pointer)TEST_MODULE, mscan_time_stamp);
   printf("\n MSCAN1 Time Stamp selected. result: 0x%lx", result);

   /* Enable Error interrupts */
   if (mscan_error_interrupt)
   {
       result = mscan_error_int_enable((pointer)TEST_MODULE, CAN_RX);
       printf("\n MSCAN1 Error interrupt enable for Rx. result: 0x%lx", result);

       result = mscan_error_int_enable((pointer)TEST_MODULE, CAN_TX);
       printf("\n MSCAN1 Error interrupt enable for Tx. result: 0x%lx", result);
   }

   /* Start the loop */
   while(1)
   {
      printf("\n\nEnter an string to transmit followed by ENTER:");
      memset (data_ptr, 0, sizeof (data_ptr));
      _io_gets(data_ptr);
      result = mscan_tx_message((pointer)TEST_MODULE, (void *)data_ptr, priority, 
                                identifier, data_len_code, format, interrupt);
      if(result != MSCAN_OK)
         printf("\nTransmit error. Error Code: 0x%lx", result);
      else
         printf("\nData transmit: %s", data_ptr);
      
      _time_delay (100);

      if(interrupt != CAN_ENABLE)
      {
          recv_result = mscan_rx_message((pointer)TEST_MODULE, (void *)recv_data, &recv_id,
                        &recv_len, recv_format, &recv_hit, CAN_DISABLE);
      }

      if(recv_result != MSCAN_OK)
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
