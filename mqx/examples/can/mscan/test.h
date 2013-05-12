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
* $Version : 3.6.6.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   
*
*END************************************************************************/
#define MAIN_TASK   1
#define TX_TASK     2
#define RX_TASK     3

extern void Main_Task(uint_32);
extern void Tx_Task(uint_32);
extern void Rx_Task(uint_32);

/* Global variables */
uint_32 data_len_code;
uint_32 bit_rate;
uint_32 identifier;
uint_32 format;
uint_32 pin;
uint_32 interrupt;
uint_32 mscan1_mode;
uint_32 mscan_filter1;
uint_32 mscan_filter2;
uint_32 mscan_mask1;
uint_32 mscan_mask2;
uint_32 mscan_filter_mode;
uint_32 mscan_time_stamp;
uint_32 mscan_error_interrupt;
uint_32 tx_message_priority;

