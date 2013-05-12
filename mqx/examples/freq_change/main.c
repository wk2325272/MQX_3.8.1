/**HEADER**********************************************************************
*
* Copyright (c) 2011 Freescale Semiconductor;
* All Rights Reserved
*
*******************************************************************************
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
*******************************************************************************
*
* $FileName: main.c$
* $Version : 3.8.B.2$
* $Date    : Jun-14-2012$
*
* Comments:
*
*END**************************************************************************/

#include <mqx.h>
#include <bsp.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if !MQX_ENABLE_LOW_POWER
#error This application requires MQX_ENABLE_LOW_POWER to be defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

/* Task IDs */
enum task_ids {
    MAIN_TASK = 1,
    LED_TASK,
    FOR_LOOP_TASK
};


/* Task prototypes */
void main_task(uint_32);
void led_task(uint_32);
void for_loop_led_task(uint_32);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
    {
        /* Task Index       */  MAIN_TASK,
        /* Function         */  main_task,
        /* Stack Size       */  2500,
        /* Priority Level   */  10,
        /* Name             */  "Main Task",
        /* Attributes       */  MQX_AUTO_START_TASK,
        /* Creation Params  */  0,
        /* Time Slice       */  0,
    },
    {
        /* Task Index       */  LED_TASK,
        /* Function         */  led_task,
        /* Stack Size       */  2500,
        /* Priority Level   */  11,
        /* Name             */  "LED Task",
        /* Attributes       */  MQX_AUTO_START_TASK,
        /* Creation Params  */  0,
        /* Time Slice       */  0,
    },
    {
        /* Task Index       */  FOR_LOOP_TASK,
        /* Function         */  for_loop_led_task,
        /* Stack Size       */  2500,
        /* Priority Level   */  12,
        /* Name             */  "FOR loop LED Task",
        /* Attributes       */  MQX_AUTO_START_TASK,
        /* Creation Params  */  0,
        /* Time Slice       */  0,
    },
    { 0 }
};


/* local function prototypes */
void wait_for_event (void);
void button_init(void);

/* LWEVENT masks for app_event */
#define SW_EVENT_MASK          (1 << 0)

LWEVENT_STRUCT                  app_event;


/*FUNCTION*-----------------------------------------------------
*
* Task Name    : display_clock_values
* Comments     : Prints settings for given clock configuration.
*
*END*-----------------------------------------------------------*/

static void display_clock_values
    (
        BSP_CLOCK_CONFIGURATION clock_configuration
    )
{
    printf("CLOCK_SOURCE_CORE     = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_CORE));
    printf("CLOCK_SOURCE_BUS      = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_BUS));
    printf("CLOCK_SOURCE_FLEXBUS  = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_FLEXBUS));
    printf("CLOCK_SOURCE_FLASH    = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_FLASH));
    printf("CLOCK_SOURCE_USB      = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_USB));
    printf("CLOCK_SOURCE_PLLFLL   = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_PLLFLL));
    printf("CLOCK_SOURCE_MCGIR    = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_MCGIR));
    printf("CLOCK_SOURCE_OSCER    = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_OSCER));
    printf("CLOCK_SOURCE_ERCLK32K = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_ERCLK32K));
    printf("CLOCK_SOURCE_MCGFF    = %d Hz\n",_cm_get_clock(clock_configuration, CM_CLOCK_SOURCE_MCGFF));
}


/*FUNCTION*-----------------------------------------------------
*
* Task Name    : my_button_isr
* Comments     : SW interrupt callback used to catch SW press
*                and trigger SW_EVENT
*
*END*-----------------------------------------------------------*/

static void my_button_isr(void *pin)
{
    _lwevent_set(&app_event, SW_EVENT_MASK);
    lwgpio_int_clear_flag((LWGPIO_STRUCT_PTR) pin);
}



/*FUNCTION*-----------------------------------------------------
*
* Task Name    : button_init
* Comments     : Setup the button to trigger interrupt on every press.
*
*END*-----------------------------------------------------------*/

void button_init
    (
        void
    )
{
    static LWGPIO_STRUCT                   button;

    /* set the pin to input */
    if (!lwgpio_init(&button, BSP_BUTTON2, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE))
    {
        printf("\nButton initialization failed.\n");
        _task_block();
    }

    /* Set multiplexer to GPIO functionality */
    lwgpio_set_functionality(&button, BSP_SW2_MUX_GPIO);
    /* Enable pull up */
    lwgpio_set_attribute(&button, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);

    /* setup the pin interrupt mode */
    if (!lwgpio_int_init(&button, LWGPIO_INT_MODE_FALLING))
    {
        printf("Initializing button for interrupt failed.\n");
        _task_block();
    }

    /* install gpio interrupt service routine */
    _int_install_isr(lwgpio_int_get_vector(&button), my_button_isr, (void *) &button);
    /* set interrupt priority and enable interrupt source in the interrupt controller */
    _bsp_int_init(lwgpio_int_get_vector(&button), 3, 0, TRUE);
    /* enable interrupt for pin */
    lwgpio_int_enable(&button, TRUE);

    printf("Button has been configured as an external interrupt source.\n");
}

/*FUNCTION*-----------------------------------------------------
*
* Task Name    : wait_for_event
* Comments     : Wait for button press, lwevent is set in button
*                ist
*
*END*-----------------------------------------------------------*/

void wait_for_event (void)
{
    _mqx_uint   app_event_value;

    /* Wait for button press */
    _lwevent_wait_ticks (&app_event, (SW_EVENT_MASK), FALSE, 0);

      /* Copy event masks into temporary variable */
     app_event_value = _lwevent_get_signalled();

    /* If button pressed or switch expires, moving to next clock configuration */
    if (app_event_value & SW_EVENT_MASK)
    {
        printf("\nButton pressed. Moving to next clock configuration.\n");
    }
}

/*TASK*-----------------------------------------------------
*
* Task Name    : main_task
* Comments     : Low power modes switching.
*
*END*-----------------------------------------------------*/

void main_task
    (
        uint_32 initial_data
    )
{
    BSP_CLOCK_CONFIGURATION clock_configuration;

    printf("\n***********************************************************\n");
    printf("   MQX frequency change demo\n");
    printf("***********************************************************\n");

    /* Initialize buttons */
    button_init();

    /* Create global event */
    if (_lwevent_create(&app_event, LWEVENT_AUTO_CLEAR) != MQX_OK)
    {
        printf("\nCreating app_event failed.\n");
        _task_block();
    }

    printf("\n***********  DEFAULT CLOCK CONFIGURATION  **********\n");

    /* Get and print active clock configuration */
    clock_configuration =  _lpm_get_clock_configuration();
    display_clock_values(clock_configuration);

    printf("\nPress button on the board to loop over frequency modes.\n");

    while (1)
    {
        /* wait for RTC timer of button press */
        wait_for_event();
        printf("\n***************  BSP_CLOCK_CONFIGURATION_0  **************\n");

        /* Switch clock configuration */
        if (CM_ERR_OK != _lpm_set_clock_configuration(BSP_CLOCK_CONFIGURATION_0))
        {
          printf("Cannot change clock configuration");
          _task_block();
        }

        /* Get active clock configuration */
        clock_configuration =  _lpm_get_clock_configuration();
        /* Display new clock settings */
        display_clock_values(clock_configuration);

         /* wait for RTC timer of button press */
        wait_for_event();
        printf("\n***************  BSP_CLOCK_CONFIGURATION_1  **************\n");

        /* Switch clock configuration */
        if (CM_ERR_OK != _lpm_set_clock_configuration(BSP_CLOCK_CONFIGURATION_1))
        {
          printf("Cannot change clock configuration");
          _task_block();
        }

        /* Get active clock configuration */
        clock_configuration =  _lpm_get_clock_configuration();
        /* Display new clock settings */
        display_clock_values(clock_configuration);

         /* wait for RTC timer of button press */
        wait_for_event();
        printf("\n***************  BSP_CLOCK_CONFIGURATION_2  **************\n");

        /* Switch clock configuration */
        if (CM_ERR_OK != _lpm_set_clock_configuration(BSP_CLOCK_CONFIGURATION_2))
        {
          printf("Cannot change clock configuration");
          _task_block();
        }

        /* Get active clock configuration */
        clock_configuration =  _lpm_get_clock_configuration();
        /* Display new clock settings */
        display_clock_values(clock_configuration);
    }
}

/*TASK*-----------------------------------------------------
*
* Task Name    : led_task
* Comments     : LEDs setup and blinking. Blinking frequency is constant,
*                MQX tick is core speed independent
*
*END*-----------------------------------------------------*/

void led_task
    (
        uint_32 initial_data
    )
{
    LWGPIO_STRUCT led1;
    /* Initialize LED pin for output */
    if (!lwgpio_init(&led1, BSP_LED1, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_HIGH))
    {
        printf("\nLED1 initialization failed.\n");
        _task_block();
    }
    /* Set LED pin to GPIO functionality */
    lwgpio_set_functionality(&led1, BSP_LED1_MUX_GPIO);

    while(1)
    {
        _time_delay (500);
        /* toggle led 1 */
        lwgpio_toggle_value(&led1);
    }

}


/*TASK*-----------------------------------------------------
*
* Task Name    : led_task
* Comments     : LEDs setup and blinking, blinking frequency
*                  depends on core speed
*
*END*-----------------------------------------------------*/

void for_loop_led_task
    (
        uint_32 initial_data
    )
{
    volatile uint_32 i = 0;
    LWGPIO_STRUCT led2;

    /* Initialize LED pin for output */
    if (!lwgpio_init(&led2, BSP_LED2, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_HIGH))
    {
        printf("\nLED2 initialization failed.\n");
        _task_block();
    }
    /* Set LED pin to GPIO functionality */
    lwgpio_set_functionality(&led2, BSP_LED2_MUX_GPIO);

    while(1)
    {
        for (i = 0; i < 700000; i++) {};
        /* toggle led 2 */
        lwgpio_toggle_value(&led2);
    }
}
