/**HEADER********************************************************************
*
* Copyright (c) 2008-2009 Freescale Semiconductor;
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
* $FileName: hmi.c$
* $Version : 3.8.0.1$
* $Date    : Apr-2-2012$
*
* Comments:
*
*   This file contains the source for the HMI example program.
*
*END************************************************************************/
#include <mqx.h>
#include <bsp.h>
#include <shell.h>

#define MAIN_TASK   5
#define SHELL_TASK  6

#if !BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

extern void Main_task(uint_32);
extern void Shell_task(uint_32);

/* function prototypes */
void callback_print_push(pointer);
void callback_toggle_led(pointer);
void callback_rotary_movement(pointer);
void callback_print_release(pointer);
int_32 Shell_set_module(int_32, char_ptr []);
int_32 Shell_get_module(int_32, char_ptr []);


TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   1500,  9,   "main",  MQX_AUTO_START_TASK},
   {SHELL_TASK,  Shell_task,  1500, 12,   "shell", MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0,                 }
};

const SHELL_COMMAND_STRUCT Shell_commands[] = {
        {"module", Shell_set_module},
        {"get", Shell_get_module},
        {"exit", Shell_exit},
        {"help", Shell_help},
        {NULL, NULL}
};

HMI_CLIENT_STRUCT_PTR bsp_btnled_driver_handle;    /* BTNLED handle pointer */
    
/*TASK*-----------------------------------------------------
*
* Task Name    : Main_task
* Comments     :
*    Main task initializes HMI. It sets LEDs ON and adds 
*    callbacks for 12 buttons and a rotary. Client poll function
*    is polled here.
*
*END*-----------------------------------------------------*/

void Main_task(uint_32 initial_data)
{   
    printf("HMI demo application. \nPress buttons and electrodes available on the board.\n");
    printf("Set TWRPI module with module command.\n");
    
    bsp_btnled_driver_handle = _bsp_btnled_init(); /* BTNLED initialization */ 
    if (bsp_btnled_driver_handle == NULL)  
    {
        /* error in initialization */
        printf("BTNLED initialization failed. \n");
        _task_block();
    }
    /* set LED ON */
    btnled_set_value(bsp_btnled_driver_handle, HMI_LED_1, HMI_VALUE_ON);
    btnled_set_value(bsp_btnled_driver_handle, HMI_LED_2, HMI_VALUE_ON);

    /* add callbacks for PUSH/RELEASE/ROTARY MOVEMENT */
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_1, HMI_VALUE_PUSH, callback_toggle_led, "button 1");
    /* same button, same action(push), different callback */
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_1, HMI_VALUE_PUSH, callback_print_push, "button 1");   
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_2, HMI_VALUE_PUSH, callback_toggle_led, "button 2");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_3, HMI_VALUE_PUSH, callback_toggle_led, "button 3");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_3, HMI_VALUE_RELEASE, callback_print_release, "button 3");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_4, HMI_VALUE_PUSH, callback_print_push, "button 4");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_5, HMI_VALUE_PUSH, callback_print_push, "button 5");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_6, HMI_VALUE_PUSH, callback_print_push, "button 6");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_7, HMI_VALUE_PUSH, callback_print_push, "button 7");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_8, HMI_VALUE_PUSH, callback_print_push, "button 8");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_9, HMI_VALUE_RELEASE, callback_print_release, "button 9");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_10, HMI_VALUE_PUSH, callback_print_push, "button 10");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_11, HMI_VALUE_PUSH, callback_print_push, "button 11");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_BUTTON_12, HMI_VALUE_PUSH, callback_print_push, "button 12");
    btnled_add_clb(bsp_btnled_driver_handle, HMI_ROTARY_1, HMI_VALUE_MOVEMENT, callback_rotary_movement, "rotary");
    
    while (1)
    {
        btnled_poll(bsp_btnled_driver_handle); /* polls TSS and LWGPIO buttons */
        _time_delay(5);                 /* time for shell */
    }
}

/*TASK*-----------------------------------------------------
*
* Task Name    : Shell_task
* Comments     :
*   This task starts shell and polls it regulary.
*
*END*-----------------------------------------------------*/

void Shell_task(uint_32 initial_data)
{
    while (1)
    {
        Shell(Shell_commands,NULL);    /* start shell */
    }
}

/*FUNCTION*-------------------------------------------------------------------
* Function Name    : callback_print_push
* Comments     :
*   Callback for pushed button. Function prints the number of pushed button.
*END*----------------------------------------------------------------------*/

void callback_print_push(pointer text)
{
    printf("Pushed button: %s\n", text);
}

/*FUNCTION*-------------------------------------------------------------------
* Function Name    : callback_print_release
* Comments     :
*   Callback for released button. Function prints the number of released button.
*END*----------------------------------------------------------------------*/

void callback_print_release(pointer text)
{
      printf("Released button: %s\n", text);
}

/*FUNCTION*-------------------------------------------------------------------
* Function Name    : callback_toggle_led
* Comments     :
*   Callback to toggle LED. Function toggle first LED and prints the action
*   to the terminal.
*END*----------------------------------------------------------------------*/

void callback_toggle_led(pointer text)
{
    printf("Toggle first led. \n");
    btnled_toogle(bsp_btnled_driver_handle, HMI_LED_1);        /* toggle first LED */
}

/*FUNCTION*-------------------------------------------------------------------
* Function Name    : callback_rotary_movement
* Comments     :
*   Callback for rotary movement. It prints the movement was detected.
*END*----------------------------------------------------------------------*/

void callback_rotary_movement(pointer text)
{
    printf("%s movement detected.\n", text);
}

/*FUNCTION*-----------------------------------------------------
* Function Name    : Shell_set_module
* Comments     :
*   This function sets modules with a command in shell.
*END*-----------------------------------------------------*/

int_32 Shell_set_module(int_32 argc, char_ptr argv[] )
{
    boolean      print_usage, shorthelp = FALSE;
    int_32       return_code = SHELL_EXIT_SUCCESS;
    uint_32      module_number;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp);

    if (!print_usage)
    {
        if (argc > 2)
        {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else
        {
            if (argc == 2)
            {
                sscanf(argv[1],"%d", &module_number);
                if (module_number != _bsp_btnled_get_twrpi())
                {
                    if (_bsp_btnled_set_twrpi(module_number) == MQX_OK)
                    {
                        printf("Module %d set.\n", module_number);
                    }
                    else
                    {
                        printf("Failed to set module %d.\n", module_number);
                    }
                }
                else
                {
                    printf("Module %d already activated.\n", module_number);
                }
         }
        }
    }
    if (print_usage)
    {
        if (shorthelp)
        {
            printf("%s\n", argv[0]); /* shows in help command */
        }
        else
        {
            printf("Usage: %s number\n", argv[0]); /* shows available options in help */
            printf("Available TWRPI modules:\n");
#if defined(BSP_TWRPI_VOID)            
            printf("%d BSP_TWRPI_VOID\n", BSP_TWRPI_VOID);
#endif
#if defined(BSP_TWRPI_ROTARY)
            printf("%d BSP_TWRPI_ROTARY\n", BSP_TWRPI_ROTARY);
#endif
#if defined(BSP_TWRPI_KEYPAD) 
            printf("%d BSP_TWRPI_KEYPAD\n", BSP_TWRPI_KEYPAD);
#endif
        }
    }
    return return_code;
}

/*FUNCTION*-----------------------------------------------------
* Function Name    : Shell_get_module
* Comments     :
*   This function prints which module is active.
*END*-----------------------------------------------------*/

int_32 Shell_get_module(int_32 argc, char_ptr argv[] )
{
    boolean     print_usage, shorthelp = FALSE;
    int_32      return_code = SHELL_EXIT_SUCCESS;

    print_usage = Shell_check_help_request(argc, argv, &shorthelp );

    if (!print_usage)
    {
        if (argc > 2)
        {
            printf("Error, invalid number of parameters\n");
            return_code = SHELL_EXIT_ERROR;
            print_usage = TRUE;
        }
        else
        {
            if (argc == 1)
            {
                printf("Module %d active.\n", _bsp_btnled_get_twrpi());
            }
        }
    }
    if (print_usage)
    {
            printf("Usage: %s \n", argv[0]); /* show in help command */
    }
    return return_code;
}

/* EOF */
