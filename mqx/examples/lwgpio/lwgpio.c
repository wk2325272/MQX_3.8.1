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
* $FileName: lwgpio.c$
* $Version : 3.8.9.0$
* $Date    : Oct-3-2011$
*
* Comments:
*
*   This file contains the source for the lwgpio example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>

/* Function prototypes */
void main_task(uint_32);
void int_service_routine(void *);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
    /* Task Index, Function, Stack, Priority,   Name,          Attributes, Param, Time Slice */
    { 1,          main_task,  1500,        8, "main", MQX_AUTO_START_TASK,     0,          0 },
    { 0 }
};
/* Global variables */
LWSEM_STRUCT lwsem;


/*ISR*-----------------------------------------------------
*
* Task Name    : int_service_routine
* Comments     :
* Button press ISR
*END*-----------------------------------------------------*/
void int_service_routine(void *pin)
{
    lwgpio_int_clear_flag((LWGPIO_STRUCT_PTR) pin);
    _lwsem_post(&lwsem);
}

/*TASK*-----------------------------------------------------
*
* Task Name    : main_task
* Comments     :
* This task executes 3 steps
* 1) set BSP_LED1 on and off if BSP_LED1 available
* 2) configure BSP_BUTTON1 for interrupt if possible and wait
*    for interrupt
* 3) Drives BSP_LED1 according read from BSP_BUTTON1 or
*    drives BSP_LED1 automatically if BSP_BUTTON1 not available
*END*-----------------------------------------------------*/

void main_task
   (
      uint_32 initial_data
   )
{
    /* Structures holding information about specific pin */
    LWGPIO_STRUCT led1, btn1;
    _mqx_uint result;

   /* Create the lightweight semaphore */
   result = _lwsem_create(&lwsem, 0);
   if (result != MQX_OK) {
      printf("\nCreating sem failed: 0x%X", result);
      _task_block();
   }
#ifdef BSP_LED1
/*******************************************************************************
Opening the pin (BSP_LED1) for output, drive the output level
*******************************************************************************/
    /* initialize lwgpio handle (led1) for BSP_LED1 pin (defined in mqx/source/bsp/<bsp_name>/<bsp_name>.h file) */
    if (!lwgpio_init(&led1, BSP_LED1, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE))
    {
        printf("Initializing LED1 GPIO as output failed.\n");
        _task_block();
    }
    /* swich pin functionality (MUX) to GPIO mode */
    lwgpio_set_functionality(&led1, BSP_LED1_MUX_GPIO);

    /* write logical 1 to the pin */
    lwgpio_set_value(&led1, LWGPIO_VALUE_HIGH); /* set pin to 1 */

#endif

#if defined BSP_BUTTON1
/*******************************************************************************
Opening the pin (BSP_BTN1) for input, initialize interrupt on it and set interrupt handler.
*******************************************************************************/
    /* opening pins for input */
    if (!lwgpio_init(&btn1, BSP_BUTTON1, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE))
    {
        printf("Initializing button GPIO as input failed.\n");
        _task_block();
    }

#ifdef BSP_BUTTON1_MUX_IRQ
    /* Some platforms require to setup MUX to IRQ functionality,
    for the rest just set MUXto GPIO functionality */
    lwgpio_set_functionality(&btn1, BSP_BUTTON1_MUX_IRQ);
    lwgpio_set_attribute(&btn1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);

    /* enable gpio functionality for given pin, react on falling edge */
    if (!lwgpio_int_init(&btn1, LWGPIO_INT_MODE_FALLING))
    {
        printf("Initializing button GPIO for interrupt failed.\n");
        _task_block();
    }

    /* install gpio interrupt service routine */
    _int_install_isr(lwgpio_int_get_vector(&btn1), int_service_routine, (void *) &btn1);
    /* set the interrupt level, and unmask the interrupt in interrupt controller*/
    _bsp_int_init(lwgpio_int_get_vector(&btn1), 3, 0, TRUE);
    /* enable interrupt on GPIO peripheral module*/
    lwgpio_int_enable(&btn1, TRUE);

    printf("Button (SW1) pin has been configured as input GPIO interrupt pin.\n");
    printf("Press the button to continue.\n");

    /* wait for button press, lwsem is set in button isr*/
    _lwsem_wait(&lwsem);

    /* disable interrupt on GPIO peripheral module*/
    lwgpio_int_enable(&btn1, FALSE);
#endif /* BSP_BUTTON1_MUX_IRQ */

/*******************************************************************************
Read value from input pin (BSP_BTN1)  Note that in previous phase, the pin was
configured as an interrupt and it have to be reconfigured to standard GPIO.
*******************************************************************************/
    /* set pin functionality (MUX) to GPIO*/
    lwgpio_set_functionality(&btn1, BSP_BUTTON1_MUX_GPIO);
    lwgpio_set_attribute(&btn1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);

    printf("Press button to switch LED on or off\n");
    while (TRUE)
    {
        /* read pin/signal status */
        if (LWGPIO_VALUE_HIGH == lwgpio_get_value(&btn1))
        {
            printf("Button IDLE.\n");
#ifdef BSP_LED1
            lwgpio_set_value(&led1, LWGPIO_VALUE_LOW); /* clear pin to 0 */
#endif /* BSP_LED1*/
        }
        else
        {
            printf("Button PRESSED.\n");
#ifdef BSP_LED1
            lwgpio_set_value(&led1, LWGPIO_VALUE_HIGH); /* set pin to 1 */
#endif /* BSP_LED1 */
        }
   }

#elif defined BSP_LED1

/*******************************************************************************
In case board does not have any buttons. Only blink with LED.
*******************************************************************************/

    printf("Going to blink with LED\n");

    /* toggle value */
    while (TRUE)
    {
        lwgpio_toggle_value(&led1);
        _time_delay(250);
    }

#else /* BSP_LED1, BSP_BUTTON1 */

    printf("Cannot run demo, no BSP_LED1 nor BSP_BUTTON1 defined for board.");

#endif /* BSP_LED1, BSP_BUTTON1 */

}

/* EOF */
