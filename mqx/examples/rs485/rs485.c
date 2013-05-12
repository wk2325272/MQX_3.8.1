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
* $FileName: rs485.c$
* $Version : 3.8.4.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   This file contains the source for the rs485 example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h> 
#include <fio.h>
#include <string.h>

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

/* select if example transmit, or receive 
** 1 transmit
** 0 receive
*/
#define RS485_TRANSMIT  1

/* enable if device has RTS pin hw support */
#define HAS_485_HW_FLOW_CONTROL 1

/* set RS485 output device */
#define RS485_CHANNEL "ittyd:"

/* Task IDs */
#define RS485_TASK 5

extern void rs485_write_task(uint_32);
extern void rs485_read_task(uint_32);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{ 
   /* Task Index,   Function,         Stack,  Priority, Name,     Attributes,          Param, Time Slice */
#if (RS485_TRANSMIT == 1 )
   { RS485_TASK,   rs485_write_task,  1500,   8,        "rs_485", MQX_AUTO_START_TASK, 0,     0 },
#else
   { RS485_TASK,   rs485_read_task,   1500,   8,        "rs_485", MQX_AUTO_START_TASK, 0,     0 },
#endif
    { 0 }
};

/*TASK*-----------------------------------------------------
* 
* Task Name    : rs485_write_task
* Comments     :
*    This task send data_buffer to rs485
*
*END*-----------------------------------------------------*/
void rs485_write_task
   (
      uint_32 initial_data
   )
{
   MQX_FILE_PTR rs485_dev = NULL;
   char  data_buffer[] = "RS485 send example";
   boolean disable_rx = TRUE;
   uint_32 result;

   /* If mcu has hardware support for RTS pin drive (e.g. k60n512), open line with IO_SERIAL_HW_485_FLOW_CONTROL flag */
   #if (HAS_485_HW_FLOW_CONTROL)  
   /* HW 485 flow control on chip*/
   rs485_dev = fopen( RS485_CHANNEL, (char const *)IO_SERIAL_HW_485_FLOW_CONTROL ); 
   #else 
   /* HW 485 flow not available on chip */
   rs485_dev  = fopen( RS485_CHANNEL, NULL );                      
   #endif
   
   if( rs485_dev == NULL )
   {
      /* device could not be opened */
      _task_block();
   }
   
   /* half duplex, two wire. use only if disable receiver in transmit is desired */
   result = ioctl( rs485_dev, IO_IOCTL_SERIAL_DISABLE_RX, &disable_rx ); 
   if( result == IO_ERROR_INVALID_IOCTL_CMD )
   {
      /* ioctl not supported, use newer MQX version */
      _task_block();
   }
   
   #if !(HAS_485_HW_FLOW_CONTROL)
   /* User written function for flow control by GPIO pin - handle RTS other signal to drive RS485 HW driver */
   #endif
   
   /* write data */
   write( rs485_dev, data_buffer, strlen(data_buffer) );
   
   /* empty queue - not needed for polled mode */
   fflush( rs485_dev );
   
   /* wait for transfer complete flag */
   result = ioctl( rs485_dev, IO_IOCTL_SERIAL_WAIT_FOR_TC, NULL );
   if( result == IO_ERROR_INVALID_IOCTL_CMD )
   {
      /* ioctl not supported, use newer MQX version */
      _task_block();
   }
   
   /* half duplex, two wire */
   /* if receiver was disabled before, enable receiver again */  
   disable_rx = FALSE;
   ioctl( rs485_dev, IO_IOCTL_SERIAL_DISABLE_RX, &disable_rx ) ;
   
   #if !( HAS_485_HW_FLOW_CONTROL )
   /* User written function for flow control by GPIO pin - handle RTS */
   #endif

   _task_block();

}

/*TASK*-----------------------------------------------------
* 
* Task Name    : rs485_read_task
* Comments     :
*    This task send data_buffer to rs485
*
*END*-----------------------------------------------------*/
void rs485_read_task
   (
      uint_32 initial_data
   )
{
   MQX_FILE_PTR rs485_dev = NULL;
   char  data_buffer[256];

   rs485_dev  = fopen( RS485_CHANNEL, NULL );                      
   if( rs485_dev == NULL )
   {
      /* device could not be opened */
      _task_block();
   }   
   
   /* wait for data */
   fread(data_buffer, 1, 18, rs485_dev); 
   
   while(1)
   {     
   };
}
/* EOF */
