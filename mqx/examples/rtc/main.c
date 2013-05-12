/**HEADER**********************************************************************
 *
 * Copyright (c) 2008 Freescale Semiconductor;
 * All Rights Reserved
 *
 ******************************************************************************
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
 ******************************************************************************
 *
 * $FileName: main.c$
 * $Version : 3.8.39.0$
 * $Date    : Oct-11-2011$
 *
 * Comments:
 *
 *   This file contains the source for the RTC example program.
 *
 *END*************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <lwevent.h>


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#if ! BSPCFG_ENABLE_RTCDEV
#error This application requires BSPCFG_ENABLE_RTCDEV defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

extern void main_task(uint_32);

/* Local function prototypes */
static void my_interrupt (pointer rtc_registers_ptr);
static void print_mqx_time(DATE_STRUCT_PTR time_rtc, TIME_STRUCT_PTR time_mqx);
static void print_rtc_time(DATE_STRUCT_PTR time_rtc, TIME_STRUCT_PTR time_mqx);
static void install_stopwatch(void);
static void install_interrupt(void);
static void install_alarm(void);
static void print_current_time(void);

/* For tamper event functionality on MCF51EMxx device */
#if PSP_MQX_CPU_IS_MCF51EM
static void test_tamper (void);
static void print_tamper_event(RTC_TAMPER_TYPE tamper_status, RTC_TIME_STRUCT_PTR tamper_time_ptr);
#endif /* PSP_MQX_CPU_IS_MCF51EM */
#if PSP_HAS_IRTC == 1
static void irtc_test(void);
#endif
#if BSPCFG_ENABLE_LCD
extern void lcd_task(uint_32);
#endif
const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
    /* Task Index,   Function,   Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    { 1,           main_task,  2000,   8,          "Main", MQX_AUTO_START_TASK, 0,     0 },
#if BSPCFG_ENABLE_LCD
    { 2,           lcd_task,   2000,   9,          "LCD",  MQX_AUTO_START_TASK, 0,     0 },
#endif
    { 0 }
};


volatile uint_32               state = 0;


#if PSP_HAS_IRTC == 1
static const uint_8   backup_data[] = "Backup Data";
static uint_8         read_data[sizeof(backup_data)];
#endif

LWEVENT_STRUCT lwevent;

#define LWE_STOPWATCH   0x01
#define LWE_ALARM       0x02

/*FUNCTION**********************************************************************
 *
 * Function Name    : print_mqx_time
 * Returned Value   :
 * Comments         : Print mqx time to terminal
 *
 *END**************************************************************************/

static void print_mqx_time
(
        DATE_STRUCT_PTR     time_rtc,
        TIME_STRUCT_PTR     time_mqx
)
{
    printf ("MQX : %d s, %d ms (%02d.%02d.%4d %02d:%02d:%02d )\n", time_mqx->SECONDS, time_mqx->MILLISECONDS,time_rtc->DAY, time_rtc->MONTH, time_rtc->YEAR, time_rtc->HOUR, time_rtc->MINUTE, time_rtc->SECOND);
}

/*FUNCTION**********************************************************************
 *
 * Function Name    : print_rtc_time
 * Returned Value   :
 * Comments         : Print rtc time to terminal
 *
 *END**************************************************************************/

static void print_rtc_time
(
        DATE_STRUCT_PTR     time_rtc,
        TIME_STRUCT_PTR     time_mqx
)
{
    printf ("RTC : %02d.%02d.%4d %02d:%02d:%02d (%02d s, %03d ms)\n", time_rtc->DAY, time_rtc->MONTH, time_rtc->YEAR, time_rtc->HOUR, time_rtc->MINUTE, time_rtc->SECOND, time_mqx->SECONDS, time_mqx->MILLISECONDS);

}

/*FUNCTION**********************************************************************
 *
 * Function Name    : install_interrupt
 * Returned Value   :
 * Comments         : install user rtc interrupt handler function
 *
 *END**************************************************************************/

static void install_interrupt(void)
{
    printf ("\nInstalling RTC interrupt for stopwatch and alarm ... ");
    if (MQX_OK != _rtc_int_install ((pointer)my_interrupt))
    {
        printf ("Error!\n");
        _task_block();
    }

    _rtc_clear_requests (RTC_RTCISR_SW | RTC_RTCISR_ALM);
    if (0 == _rtc_int_enable (TRUE, RTC_RTCIENR_SW | RTC_RTCIENR_ALM))
    {
        printf ("Error!\n");
        _task_block();
    }
    printf ("OK\n");
}

/*TASK**************************************************************************
 *
 * Function Name    : main_task
 * Returned Value   :
 * Comments         : Playing aroung with the RTC module.
 *
 *END**************************************************************************/

void main_task
(
        uint_32 initial_data
)
{
    DATE_STRUCT     time_rtc;
    TIME_STRUCT     time_mqx;

    if (_lwevent_create(&lwevent,0) != MQX_OK)
    {
        printf("\nMake event failed");
        _task_block();
    }

    printf ("\fStart time (MQX synchronized to RTC time during bsp init):\n\n");


    /* initialize time */
    time_rtc.YEAR     = 2010;
    time_rtc.MONTH    = 10;
    time_rtc.DAY      = 15;
    time_rtc.HOUR     = 10;
    time_rtc.MINUTE   = 8;
    time_rtc.SECOND   = 0;
    time_rtc.MILLISEC = 0;

    _time_from_date (&time_rtc, &time_mqx);

    _time_set( &time_mqx);
    if( _rtc_sync_with_mqx(FALSE) != MQX_OK )
    {
        printf("\nError synchronize time!\n");
        _task_block();
    }
    _time_get (&time_mqx);

    _time_to_date (&time_mqx, &time_rtc);
    print_mqx_time(&time_rtc, &time_mqx);
    print_current_time();

    /* except MPC5125 */
#ifndef BSP_TWRMPC5125
    install_interrupt();

    /* enable stopwatch */
    install_stopwatch();

    /* enable alarm */
    install_alarm();

    _lwevent_wait_ticks(&lwevent,LWE_ALARM,FALSE,0);
    _lwevent_clear(&lwevent,LWE_ALARM);

    printf ("\nALARM!\n");
    print_current_time();
    /* end of alarm */

    printf ("Continue wasting time (2 minutes max) ...\n");
    _lwevent_wait_ticks(&lwevent,LWE_STOPWATCH,FALSE,0);
    _lwevent_clear(&lwevent,LWE_STOPWATCH);

    printf ("\nSTOPWATCH!\n");
    print_current_time();

    printf ("\nClearing RTC:\n");
    _rtc_init (RTC_INIT_FLAG_CLEAR | RTC_INIT_FLAG_ENABLE);

    print_current_time();

    install_alarm();
    _lwevent_wait_ticks(&lwevent,LWE_ALARM,FALSE,0);
    _lwevent_clear(&lwevent,LWE_ALARM);

    printf ("ALARM!\n");
    print_current_time();

#else /* BSP_TWRMPC5125 */
    printf ("Waste 10 seconds here\n");
    _time_delay(10000);
    _rtc_get_time_mqxd (&time_rtc);
    print_rtc_time(&time_rtc, &time_mqx);
#endif

    printf ("Synchronize RTC to MQX time again:\n");
    _rtc_sync_with_mqx (FALSE);
    _rtc_get_time_mqxd (&time_rtc);
    _time_from_date (&time_rtc, &time_mqx);
    print_rtc_time(&time_rtc, &time_mqx);

#if PSP_HAS_IRTC == 1
    irtc_test();
#endif /* PSP_HAS_IRTC == 1 */

    /* Test tamper event functionality on MCF51EMxx device */
#if PSP_MQX_CPU_IS_MCF51EM
    test_tamper();
#else
    printf ("Finish, press/hold reset to repeat.\n");
    _task_block() ;
#endif
}

/*FUNCTION**********************************************************************
 *
 * Function Name    : my_interrupt
 * Returned Value   :
 * Comments         :
 *    RTC interrupt callback
 *
 *END**************************************************************************/

#if PSP_HAS_IRTC == 1
static void irtc_test(void)
{
    uint_16         i;

    /* Testing RTC Up counter */
    printf ("Testing RTC Up-Value Counter\n");
    printf ("Current   Up-Counter Value = %d\n", _rtc_get_upcounter());
    printf ("Increment Up-Counter Value by 12 ");

    /* Increment upcounter by 12 */
    for (i = 0; i < 12; i++)   {
        _rtc_inc_upcounter();
        puts(".");
    }
    printf("\nNew  up-counter value = %d\n", _rtc_get_upcounter());

    /* Testing RTC Stand By RAM */
    printf("Testing RTC Stand By RAM\n");

    if (MQX_OK !=  _rtc_write_to_standby_ram(0, (uint_8 *)backup_data, sizeof(backup_data)))
    {
        /* Writing data to stand-by RAM failed */
        puts("Stand By RAM test failed \n");
    }
    else
    {
        /* Writing data to stand-by RAM successful */
        if (MQX_OK != _rtc_read_from_standby_ram(0, (uint_8 *)read_data, sizeof(read_data)))
        {
            puts("Stand-By RAM test failed \n");
        }
        else {
            printf ((char *)read_data);
            puts("\nStand-By RAM test succesfull \n");
        }

    }
}
#endif /* PSP_HAS_IRTC == 1 */

/*FUNCTION**********************************************************************
 *
 * Function Name    : install_stopwatch
 * Returned Value   :
 * Comments         :
 *
 *END**************************************************************************/
static void install_stopwatch(void)
{
    printf ("Stopwatch set to 1 minute (next whole minute) ... ");
    _rtc_set_stopwatch (1);
    if (_rtc_get_stopwatch () != 1)
    {
        printf ("Error!\n");
        _task_block();
    }
    printf ("OK\n");
}

/*FUNCTION**********************************************************************
 *
 * Function Name    : install_alarm
 * Returned Value   :
 * Comments         :
 *
 *END**************************************************************************/
static void install_alarm(void)
{
    DATE_STRUCT     time_rtc;
    TIME_STRUCT     time_mqx;

    _rtc_get_time_mqxd (&time_rtc);
    _time_from_date (&time_rtc, &time_mqx);
    time_mqx.SECONDS += 10;
    _time_to_date (&time_mqx, &time_rtc);
    _rtc_set_alarm_mqxd (&time_rtc);
    printf ("\nSetting alarm to 10 seconds to %02d.%02d.%04d %02d:%02d:%02d ... OK\n", time_rtc.DAY, time_rtc.MONTH, time_rtc.YEAR, time_rtc.HOUR, time_rtc.MINUTE, time_rtc.SECOND);
    printf ("Wasting time ... \n");
}

/*FUNCTION**********************************************************************
 *
 * Function Name    : print_current_time
 * Returned Value   :
 * Comments         :
 *
 *END**************************************************************************/
static void print_current_time(void)
{
    DATE_STRUCT     time_rtc;
    TIME_STRUCT     time_mqx;

    _rtc_get_time_mqxd (&time_rtc);
    _time_from_date (&time_rtc, &time_mqx);
    print_rtc_time(&time_rtc, &time_mqx);
}


/*FUNCTION**********************************************************************
 *
 * Function Name    : my_interrupt
 * Returned Value   :
 * Comments         :
 *    RTC interrupt callback
 *
 *END**************************************************************************/

static void my_interrupt (pointer rtc_registers_ptr)
{
    state = _rtc_get_status ();

    if( state & RTC_RTCISR_SW)
    {
        _lwevent_set(&lwevent,LWE_STOPWATCH);
    }
    if( state &   RTC_RTCISR_ALM)
    {
        _lwevent_set(&lwevent,LWE_ALARM);
    }
    _rtc_clear_requests (state);
}

/* Test tamper event functionality on MCF51EMxx device */
#if PSP_MQX_CPU_IS_MCF51EM

/*FUNCTION**********************************************************************
 *
 * Function Name    : test_tamper
 * Returned Value   :
 * Comments         :
 *   Function runs in infinite loop. If you press tamper or remove
 *   battery it displays the cause of tamper event and its timestamp.
 *
 *END**************************************************************************/

void test_tamper (void)
{
    RTC_TAMPER_TYPE         tamper_status;
    RTC_TIME_STRUCT         tamper_time;
    VMCF51EM_RTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    /* Test tamper event */
    puts("\nPress tamper button or remove battery\n");
    puts("or press/hold reset to repeat whole test again.\n\n");

    while (1)
    {
        /* Wait for tamper event assertion */
        while ((_rtc_get_status() & RTC_INT_TMPR_MASK) != RTC_INT_TMPR_MASK) {};

        _rtc_get_tamper_timestamp(&tamper_time);
        tamper_status = _rtc_get_tamper_status();
        print_tamper_event(tamper_status, &tamper_time);

        /* Wait until tamper event is cleared */
        while ((_rtc_get_status() & RTC_INT_TMPR_MASK) == RTC_INT_TMPR_MASK)
        {
            /* Clear tamper event */
            _rtc_clear_requests(RTC_INT_TMPR_MASK);
            /* Wait 200ms time before tamper status checking */
            _time_delay(200);
        }

        /* Print when tamper event was cleared */
        _rtc_get_tamper_timestamp(&tamper_time);
        tamper_status = _rtc_get_tamper_status();
        print_tamper_event(tamper_status, &tamper_time);
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name    : print_tamper_event
 * Returned Value   :
 * Comments         :
 *
 *END**************************************************************************/

void print_tamper_event
(
        RTC_TAMPER_TYPE tamper_status,
        RTC_TIME_STRUCT_PTR tamper_time_ptr
)
{
    /* Print tamper status */
    switch(tamper_status)
    {
    /* 00 – No tamper detected                          */
    case RTC_TMPR_CLEAR:
        puts("Tamper cleared          ");
        break;
        /* 01 – Tamper detected via external signal         */
    case RTC_TMPR_PIN:
        puts("External tamper detected");
        break;
        /* 10 – Battery disconnected when MCU power is ON   */
    case RTC_TMPR_BATTERY_VDDON:
        puts("VBAT = OFF,  VDD = ON   ");
        break;
        /* 11 – Battery disconnected when MCU power is OFF  */
    case RTC_TMPR_BATTERY_VDDOFF:
        puts("VBAT = OFF,  VDD = OFF  ");
        break;
    }

    /* Tamper detected on */
    if (tamper_time_ptr != NULL)
    {
        printf(" on: %d d, %d hr, %d m, %d s\n",
                tamper_time_ptr->days,
                tamper_time_ptr->hours,
                tamper_time_ptr->minutes,
                tamper_time_ptr->seconds
        );

    }
    else
    {
        puts("\n");
    }

}

#endif /* PSP_MQX_CPU_IS_MCF51EM */

#if BSPCFG_ENABLE_LCD
/*TASK**************************************************************************
 *
 * Function Name    : lcd_task
 * Returned Value   :
 * Comments         : Show rtc time on lcd.
 *
 *END**************************************************************************/

void lcd_task
(
        uint_32 initial_data
)
{
    DATE_STRUCT      time_rtc;
    eLCD_Symbols     spec_sym;
    char             time[5];
    char             state = 0;

    puts("\n\n");
    if (_lcd_init() == IO_ERROR)
    {
        puts("_lcd_init() function returned IO_ERROR\n");
        puts("lcd_task blocked\n\n");
        _task_block();
    }

    puts("TWRPI-SLCD display is connected and lcd_task is running\n");

    _lcd_segments( FALSE );
    while(1)
    {
        _rtc_get_time_mqxd (&time_rtc);
        /* post meridiem */
        if( time_rtc.HOUR > 12 )
        {
            time_rtc.HOUR -= 12;
            spec_sym = LCD_AM;
            _lcd_symbol( spec_sym, FALSE );
            spec_sym = LCD_PM;
            _lcd_symbol( spec_sym, TRUE);
        }
        else
        {
            spec_sym = LCD_PM;
            _lcd_symbol( spec_sym, FALSE);
            spec_sym = LCD_AM;
            _lcd_symbol( spec_sym, TRUE);
        }
        sprintf( time, "%2d%2d", time_rtc.HOUR, time_rtc.MINUTE);
        if( time_rtc.MINUTE < 10 )
        {
            time[2] = '0';
        }
        _lcd_puts( time );
        spec_sym = LCD_COL1;
        state = (state + 1) & 1;
        _lcd_symbol( spec_sym, (boolean)state );
        _time_delay(1000);
    }

}
#endif /* BSPCFG_ENABLE_LCD */

/* EOF */
