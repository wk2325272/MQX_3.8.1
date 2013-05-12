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
* $FileName: test.h$
* $Version : 3.8.17.5$
* $Date    : May-2-2012$
*
* Comments:
*
*   
*
*END************************************************************************/

#if defined (BSP_TWRMCF54418) || defined(BSP_TWR_K40X256) || defined(BSP_TWR_K60N512) || defined(BSP_KWIKSTIK_K40X256) ||\
    defined(BSP_TWR_K40D100M) || defined(BSP_TWR_K70F120M) || defined(BSP_TWRPXS20) || defined(BSP_TWRPXS30) ||\
    defined(BSP_TWR_K60F120M) || defined(BSP_TWR_K60D100M)
#define CAN_DEVICE  1
#else
#define CAN_DEVICE  0
#endif

// Set NODE to 1 or 2 depending on which endpoint you are running. See notes in test.c
#define NODE 1

#define MY_EVENT_GROUP 123

/* Task define */
#define MAIN_TASK   1
#define TX_TASK     2
#define RX_TASK     3

/* Tasks */
void MY_FLEXCAN_ISR(pointer);
extern void Main_Task(uint_32);
extern void Tx_Task(uint_32);
extern void Rx_Task(uint_32);

/* Functions */
extern void print_result(uint_32);
extern void get_string(char_ptr,uint_32_ptr);

/* Global variables */
uint_32 data_len_code;
uint_32 bit_rate;
uint_32 TX_identifier;
uint_32 RX_identifier;
uint_32 TX_remote_identifier;
uint_32 RX_remote_identifier;
uint_32 format;
uint_32 interrupt;
uint_32 TX_mailbox_num;
uint_32 RX_mailbox_num;
uint_32 TX_remote_mailbox_num;
uint_32 RX_remote_mailbox_num;
uint_32 bit_timing0;
uint_32 bit_timing1;
uint_32 frequency;
uint_32 flexcan_mode;
uint_32 flexcan_error_interrupt;

/* EOF */
