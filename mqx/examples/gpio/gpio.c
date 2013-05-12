/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: gpio.c$
* $Version : 3.8.34.2$
* $Date    : Apr-25-2012$
*
* Comments:
*
*   This file contains the source for the gpio example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <io_gpio.h>

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#if ! BSPCFG_ENABLE_GPIODEV
#error This application requires BSPCFG_ENABLE_GPIODEV defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


/* Task IDs */
#define MAIN_TASK 5

/* Function prototypes */
extern void main_task(uint_32);
extern void int_callback(void);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
  /* Task Index,   Function,  Stack,  Priority, Name,      Attributes,          Param, Time Slice */ 
   { MAIN_TASK,    main_task, 1500,   8,        "service", MQX_AUTO_START_TASK, 0,     0 },
   { 0 }
};


volatile uint_32 int_counter;

void int_callback(void) 
{
    int_counter++;
}


/*TASK*-----------------------------------------------------
** 
** Task Name    : main_task
** Comments     :
** This task performs various open - close operation on
** several sets of pins. The purpose of this demo is to
** explain and demonstrate the use of "pin files" as the output
** of GPIO fopen command. The demonstration is focused mainly
** on debugging and realising what we are performing on which
** set of pins.
** At the end of this task, neverending loop runs and reads
** status of pin connected to SWITCH_1 and the same value writes
** on output pin connected to LED D10.
*END*-----------------------------------------------------*/

void main_task 
   (
      uint_32 initial_data
   )
{
   MQX_FILE_PTR port_file1, port_file2, port_file3, port_file4;
   
#if defined BSP_LED2 && defined BSP_LED3 && defined BSP_LED4
    #define PINS1
    GPIO_PIN_STRUCT pins1[] = {
        /* when opening, explicitly set output to 0 */
        BSP_LED2 | GPIO_PIN_STATUS_0,
        /* when opening, explicitly set output to 1 */
        BSP_LED3 | GPIO_PIN_STATUS_1,
        /* when opening, implicitly set output to 0 */
        BSP_LED4,
        GPIO_LIST_END
    };
#endif

#if defined BSP_LED2
    #define PINS2       
    GPIO_PIN_STRUCT pins2[] = {
        BSP_LED2,
        GPIO_LIST_END
    };
#endif

#if defined BSP_LED1
    #define PINS3
    GPIO_PIN_STRUCT pins3[] = {
        BSP_LED1,
        GPIO_LIST_END
    };
#endif

#if defined BSP_BUTTON1 && defined BSP_BUTTON2
    #define PINS4
    GPIO_PIN_STRUCT pins4[] = {
        BSP_BUTTON1,
        BSP_BUTTON2,
        GPIO_LIST_END
    };
#endif

/************************************************
 Opening file for write with associated pins,
 write LOG 0 and LOG 1 to file.
*************************************************/
#if defined PINS1 && defined PINS2  
   /* open file containing pin1 set of pins/signals */
   if (NULL == (port_file1 = fopen("gpio:write", (char_ptr) &pins1 ))) 
   {
       printf("Opening file1 GPIO for pins1 failed.\n");
      _task_block();
   }
   /* write logical 1 to all signals in the file (fast) */
   if (IO_OK != ioctl(port_file1, GPIO_IOCTL_WRITE_LOG1, NULL ))
   {
       printf("Writing to whole file1 failed.\n");
      _task_block();
   }
   /* write logical 0 to the pins/signals listed in pins2 array (slow) */
   if (IO_OK != ioctl(port_file1, GPIO_IOCTL_WRITE_LOG0, &pins2 ))
   {
       printf("Writing to pins2 of file1 failed.\n");
      _task_block();
   }
   /* close file1 */
   fclose(port_file1);
#endif
/************************************************
 Opening file for write without associated pins,
 pins are added later via IOCTL commans,
 write LOG 0 and LOG 1 to file.
************************************************/   
#ifdef PINS3  
   /* open file not containing any pin/signal */
   if (NULL == (port_file2 = fopen("gpio:write", (char_ptr) NULL)))
   {
       printf("Opening file2 GPIO without associated pins failed.\n");
      _task_block();
   }

   /* add pins/signals to the existing file */
   if (IO_OK != ioctl(port_file2, GPIO_IOCTL_ADD_PINS, &pins3 ))
   {
       printf("Adding pins3 to file2 failed.\n");
      _task_block();
   }

   /* write logical 0 to all signals in the file (fast) */
   if (IO_OK != ioctl(port_file2, GPIO_IOCTL_WRITE_LOG0, NULL ))
   {
       printf("Writing to pins3 of file2 failed.\n");
      _task_block();
   }
   /* close port 2 */
   fclose(port_file2);
#endif
/************************************************
 Opening file for read with associated pins(buttons),
 Opening file for write with associated pins(LEDs),
 After pressing button LED is activated.
************************************************/  
#if defined PINS4 && defined PINS2 
   

#if BSP_TWRMCF51MM || BSP_TWR_K40X256 || BSP_TWR_K40D100M || BSP_TWR_K60N512 ||\
    BSP_TWR_K53N512 || BSP_TWRMCF51QM || BSP_TWR_K60D100M
/* test interrupt capability for TWRMCF51MM board */
    {
        GPIO_PIN_STRUCT pins_int[] = {
            BSP_BUTTON1 | GPIO_PIN_IRQ_FALLING,
            GPIO_LIST_END
        };

        /* opening pins/signals for input */
        if (NULL == (port_file4 = fopen("gpio:read", (char_ptr) &pins_int )))
        {
           printf("Opening file4 GPIO with associated pins failed.\n");
          _task_block();
        }


        /* install gpio interrupt callback */
        ioctl(port_file4, GPIO_IOCTL_SET_IRQ_FUNCTION, (pointer)int_callback);


        while (int_counter < 5);
        
        fclose(port_file4);

    }
#endif   
   
   
   
   /* opening pins/signals for input */
   if (NULL == (port_file3 = fopen("gpio:read", (char_ptr) &pins4 )))
   {
       printf("Opening file3 GPIO with associated pins failed.\n");
      _task_block();
   }

   /* prepare new file with only one pin output */
   /* and open with new specification */
   if (NULL == (port_file1 = fopen("gpio:write", (char_ptr) &pins2 )))
   {
       printf("Opening file1 GPIO for pins2 failed.\n");
      _task_block();
   }

   /* It is not easy to generalize switch: every board has its own switch names */
   printf("Use SWITCH on board to switch LED on or off\n");
   while (TRUE)
   {
       /* read pin/signal status to pin list */
       if (IO_OK != ioctl(port_file3, GPIO_IOCTL_READ, (char_ptr) &pins4))
       {
           printf("Reading pin status from file3 to pins4 list failed.\n");
          _task_block();
       }
       /* test first's pin of file binary value */       
       if (pins4[0] & GPIO_PIN_STATUS)
       {
           printf("LED OFF.\n"); 
           /* clear pin to 0 (fast) */
           if (IO_OK != ioctl(port_file1, GPIO_IOCTL_WRITE_LOG0, NULL))
           {
               printf("Clearing pins2 to log 0 failed.\n");
              _task_block();
           }
       }
       else 
       {
           printf("LED ON.\n");
           /* set pin to 1 (fast) */
           if (IO_OK != ioctl(port_file1, GPIO_IOCTL_WRITE_LOG1, NULL))
           {
               printf("Setting pins2 to log 1 failed.\n");
              _task_block();
           }
       }
       
   }
/************************************************
 In case board does not have BUTTONS, 
 Only blink with LED.
************************************************/    
   
#elif defined PINS3

    
    
    /* if we don't have PINS4 set, blink */
    /* open file not containing any pin/signal */
    if (NULL == (port_file3 = fopen("gpio:write", (char_ptr) &pins3)))
    {
        printf("Opening file2 GPIO without associated pins failed.\n");
        _task_block();
    }

    while(TRUE){
    
        /* write logical 0 to all signals in the file (fast) */
        if (IO_OK != ioctl(port_file3, GPIO_IOCTL_WRITE_LOG0, NULL ))
        {
            printf("Writing to pins3 of file2 failed.\n");
            _task_block();
        }
        _time_delay(1000);
        /* write logical 1 to all signals in the file (fast) */
        if (IO_OK != ioctl(port_file3, GPIO_IOCTL_WRITE_LOG1, NULL ))
        {
            printf("Writing to pins3 of file2 failed.\n");
            _task_block();
        }   
        _time_delay(1000);
   }
#endif  
}  


/* EOF */
