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
* $FileName: dcd_demo.c$
* $Version : 3.8.4.1$
* $Date    : Feb-16-2012$
*
* Comments:
*
*   This file contains the source for the dcd example program.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <fio.h>

#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif

#if ! BSPCFG_ENABLE_USBDCD
#error This application requires BSPCFG_ENABLE_USBDCD defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#if defined BSP_TWR_K60N512 || BSP_TWR_K70F120M || BSP_TWR_K60F120M
   #define  VBUS_DETECT   (48000)
   #define MY_ADC "adc0:" /* must be #1 as the inputs are wired to ADC 1 */
   #define MY_TRIGGER ADC_PDB_TRIGGER
   #if !BSPCFG_ENABLE_ADC0
      #error This application requires BSPCFG_ENABLE_ADC0 defined non-zero in user_config.h. Please recompile BSP with this option.
   #endif /* BSPCFG_ENABLE_ADCx */
   /* Logical channel #1 init struct */
   const ADC_INIT_CHANNEL_STRUCT adc_channel_param =
   {
       /* physical ADC channel */
       ADC0_SOURCE_AD16,
       ADC_CHANNEL_MEASURE_LOOP | ADC_CHANNEL_START_TRIGGERED, /* runs continuously after IOCTL trigger */
       10,             /* number of samples in one run sequence */
       0,              /* time offset from trigger point in us */
       300000,         /* period in us (= 0.3 sec) */
       0x10000,        /* scale range of result (not used now) */
       10,             /* circular buffer size (sample count) */
       MY_TRIGGER,     /* logical trigger ID that starts this ADC channel */

   };

   /* ADC device init struct */
   const ADC_INIT_STRUCT adc_init = {
       ADC_RESOLUTION_DEFAULT,     /* resolution */
   };
#endif /* BSP_TWR_K60N512 */

#if defined BSP_LED1
  #define CHARGING_LED                 BSP_LED1
  #define CHARGING_LED_MUX_GPIO        BSP_LED1_MUX_GPIO
#endif

#ifdef BSP_TWR_K53N512
  #define USB_DCD_PIN_VBUS          (LWGPIO_PORT_B | LWGPIO_PIN4)
  #define USB_DCD_PIN_VBUS_MUX_GPIO (LWGPIO_MUX_B4_GPIO)
#endif

#ifdef BSP_KWIKSTIK_K40X256
  #define USB_DCD_PIN_VBUS          (LWGPIO_PORT_E | LWGPIO_PIN12)
  #define USB_DCD_PIN_VBUS_MUX_GPIO (LWGPIO_MUX_E12_GPIO)
#endif

#ifdef BSP_TWRMCF51JF
  #define USB_DCD_PIN_VBUS          (LWGPIO_PORT_D | LWGPIO_PIN5)
  #define USB_DCD_PIN_VBUS_MUX_GPIO (LWGPIO_MUX_D5_GPIO)
#endif

#ifdef BSP_TWR_K40X256
  #define USB_DCD_PIN_VBUS          (LWGPIO_PORT_B | LWGPIO_PIN4)
  #define USB_DCD_PIN_VBUS_MUX_GPIO (LWGPIO_MUX_B4_GPIO)
#endif

/* Task IDs */
#define DCD_TASK  4
#if defined BSP_LED1
  #define LED_BLINK 5
#endif

extern void dcd_task(uint_32);
extern void led_blink();
extern void IO_IOCTL_TEST();
MQX_FILE_PTR f, f_ch1;

const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,          Param, Time Slice */
    { DCD_TASK,   dcd_task,     1500,     4,      "usbdcd",  MQX_AUTO_START_TASK, 0,     0 },
#if defined BSP_LED1
    { LED_BLINK,  led_blink,    1500,     5,      "ledblink",MQX_AUTO_START_TASK, 0,     0 },
#endif
    { 0 }
};

MQX_FILE_PTR                 fd;
uint_32                      Vbus_flag=0x00;

#if defined BSP_TWR_K60N512 || BSP_TWR_K70F120M || BSP_TWR_K60F120M
void Adc_Vbus()
{

   printf("Opening ADC device...");
   f = fopen(MY_ADC, (const char*)&adc_init);
   if(f != NULL)
   {
      printf("done\n");
   }
   else
   {
      printf("failed\n");
      _task_block();
   }

   printf("Opening ADC channel...");
   f_ch1 = fopen(MY_ADC "first", (const char*)&adc_channel_param);
   if(f_ch1 != NULL)
   {
      printf("done, prepared to start by trigger\n");
   }
   else
   {
      printf("failed\n");
      _task_block();
   }
   ioctl(f, ADC_IOCTL_FIRE_TRIGGER, (pointer) MY_TRIGGER);
}
#endif

#if defined BSP_LED1
/*TASK**************************************************************************
*
* Function Name   : led_blink
*
* Input Params    : None
*
* Returned Value  : None
*
* Comments        : Perform blink BSP_LED1 led when the board is connected to a USB charger
*
*END***************************************************************************/
void led_blink()
{
   LWGPIO_STRUCT Charging_led;
   uint_32       value = 1;
   lwgpio_init(&Charging_led, CHARGING_LED, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_LOW);
   lwgpio_set_functionality(&Charging_led, CHARGING_LED_MUX_GPIO);
   lwgpio_set_value(&Charging_led, LWGPIO_VALUE_HIGH);
   while (1)
   {
      if (Vbus_flag)
      {
         lwgpio_set_value(&Charging_led, value);
         _time_delay (500);
         value ^= 1;
      }
      else
      {
         lwgpio_set_value(&Charging_led, LWGPIO_VALUE_HIGH);
      }
   }
}
#endif /* BSP_LED1 */

/*TASK**************************************************************************
*
* Function Name   : dcd_task
*
* Input Params    : initial_data
*
* Returned Value  : None
*
* Comments        : Detect connect to USB, test IOCTL for USB DCD driver
*
*END***************************************************************************/
void dcd_task
   (
      uint_32 initial_data
   )
{
   uint_32                      param;
   char ch;
#if defined BSP_TWR_K60N512 || BSP_TWR_K70F120M || BSP_TWR_K60F120M
   ADC_RESULT_STRUCT data;
#else
   LWGPIO_STRUCT vbus_file;
#endif

   printf("\nUSB DCD testing... %d \n ");
   fd = fopen ("usbdcd:", NULL);
   if (fd == NULL)
   {
       printf ("ERROR opening the USB DCD driver!\n");
        _task_block();
   }
#if defined BSP_TWR_K60N512 || BSP_TWR_K70F120M || BSP_TWR_K60F120M
   Adc_Vbus();
#else
    lwgpio_init(&vbus_file, USB_DCD_PIN_VBUS, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE);
    lwgpio_set_functionality(&vbus_file, USB_DCD_PIN_VBUS_MUX_GPIO);

   if (!(lwgpio_get_value(&vbus_file)))
   {
         printf("Vbus is disable...\n");
         Vbus_flag = 0x00;
   }
#endif
   printf("\n---------------------------------------------------");
   printf("\n Press 1 to select testing IOCTL for USB DCD driver");
   printf("\n Press 2 to start detecting USB charger");
   printf("\n---------------------------------------------------");
   printf("\n Your choice: ");
   do{
      ch = fgetc(stdin);
   }while ((ch != '1') && (ch != '2'));
   if (ch == '1')
   {
      IO_IOCTL_TEST();
   }

   printf("\nStart detecting USB charger");
   for (;;)
   {

#if defined BSP_TWR_K60N512 || BSP_TWR_K70F120M || BSP_TWR_K60F120M
   read(f_ch1, &data, sizeof(data));

      if (( data.result  > VBUS_DETECT)&& (!Vbus_flag) )
      {
         /* Starting DCD - IO_IOCTL_USB_DCD_START */
         printf("\nVbus is enable ...\n");
         if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_START, &param))
         {
                 printf ("Starting detecting...\n");
         } else {
                 printf ("ERROR\n");
         }
         Vbus_flag = 0x01;
      }

      if ((data.result  < VBUS_DETECT)&& (Vbus_flag))
      {
         printf("Vbus is disable...\n");
         Vbus_flag = 0x00;
      }
#else
      if ( (lwgpio_get_value(&vbus_file)) && (!Vbus_flag) )
            {
               /* Starting DCD - IO_IOCTL_USB_DCD_START */
               printf("\nVbus is enable...\n");
               if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_START, &param))
               {
                       printf ("Starting detecting...\n");
               } else {
                       printf ("ERROR\n");
               }
               Vbus_flag = 0x01;
            }

            if ((!(lwgpio_get_value(&vbus_file))) && (Vbus_flag))
            {
               printf("Vbus is disable ...\n");
               Vbus_flag = 0x00;
            }

#endif /* BSP_TWR_K60N512 */
      if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_STATE, &param))
      {
         if (param == USB_DCD_SEQ_COMPLETE )
         {
            if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_CHARGER_TYPE, &param))
            {
               if ((param & 0x0F) == STANDARD_HOST )
               {
                  printf("Connected to a Standard Host\n");
               }
               else if ((param & 0x0F) == CHARGING_HOST)
               {
                  printf("Connected to a Charging Host\n");
               }
               else if ((param & 0x0F) == DEDICATED_CHARGER)
               {
                  printf("Connected to a dedicated Charger\n");
               }
               else // if (param & 0xF0)
                  printf("------------- DCD Sequence occurs error (param = 0x%X)....  \r\n",param);
            }
            else
            {
               printf("IO_IOCTL_USB_DCD_GET_CHARGER_TYPE - ERROR...\n");
            }
            if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_RESET, &param))
            {
               printf("USB-DCD module : Reset\n");
            }
            else
            {
               printf("Cannot reset USB-DCD module\n");
            }
         }
         /* ioctl command test*/
          _time_delay(20);
      }
      else
      {
         printf("Can not get state...\n");
         /* ioctl command test*/
          _time_delay(20);
      }

   }
   printf("Closing channels...");
   fclose(f_ch1);

   _task_block();

}

/*FUNCTION**********************************************************************
*
* Function Name   : IO_IOCTL_TEST
*
* Input Params    : None
*
* Returned Value  : None
*
* Comments        : Test all IOCTL commands
*
*END***************************************************************************/
void IO_IOCTL_TEST
(
   void
)
{
     uint_32                      param;
     /* Testing ioctl*/
     printf("\nIOCTL command for USB DCD testing...\n");
     param = USBDCD_TSEQ_INIT_RESET_VALUE;
     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_SET_TSEQ_INIT, &param))
     {
        printf ("IO_IOCTL_USB_DCD_SET_TSEQ_INIT with value = %x - OK \n", param );
     } else {
        printf ("ERROR\n");
     }

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_TSEQ_INIT, &param))
     {
        printf ("TSEQ_INIT value = %x\n", param );
     } else {
        printf ("ERROR\n");
     }

     param = USB_DCD_TDCD_DBNC_RESET_VALUE;

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_SET_TDCD_DBNC, &param))
     {
         printf ("IO_IOCTL_USB_DCD_SET_TDCD_DBNC with TDCD_DBNC value = %x\n", param );
     } else {
         printf ("ERROR\n");
     }

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_TDCD_DBNC, &param))
     {
        printf ("IO_IOCTL_USB_DCD_GET_TDCD_DBNC - with TDCD_DBNC value = %x\n", param );
     } else {
        printf ("ERROR\n");
     }

     param = USB_DCD_TVDPSRC_ON_RESET_VALUE;

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_SET_TVDPSRC_ON, &param))
     {
         printf ("IO_IOCTL_USB_DCD_SET_TVDPSRC_ON with TVDPSRC_ON value = %x\n", param );
     } else {
         printf ("ERROR\n");
     }

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_TVDPSRC_ON, &param))
     {
        printf ("IO_IOCTL_USB_DCD_GET_TVDPSRC_ON - with TVDPSRC_ON value = %x\n", param );
     } else {
        printf ("ERROR\n");
     }

     param = USB_DCD_TVDPSRC_CON_RESET_VALUE;
     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_SET_TVDPSRC_CON, &param))
      {
          printf ("IO_IOCTL_USB_DCD_SET_TVDPSRC_CON with TVDPSRC_CON value = %x\n", param );
      } else {
          printf ("ERROR\n");
      }

      if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_TVDPSRC_CON, &param))
      {
         printf ("IO_IOCTL_USB_DCD_GET_TVDPSRC_CON - with TVDPSRC_CON value = %x\n", param );
      } else {
         printf ("ERROR\n");
      }
     param = USB_DCD_CHECK_DM_RESET_VALUE;
     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_SET_CHECK_DM, &param))
     {
         printf ("IO_IOCTL_USB_DCD_SET_CHECK_DM with CHECK_DM value = %x\n", param );
     } else {
         printf ("ERROR\n");
     }

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_CHECK_DM, &param))
     {
        printf ("IO_IOCTL_USB_DCD_GET_CHECK_DM - with CHECK_DM value = %x\n", param );
     } else {
        printf ("ERROR\n");
     }

     param = 48000;
     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_SET_CLOCK_SPEED, &param))
     {
        printf ("IO_IOCTL_USB_DCD_SET_CLOCK_SPEED with clock = %d [Khz] - OK\n", param );
     } else {
        printf ("ERROR\n");
     }

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_CLOCK_SPEED, &param))
     {
        printf ("Clock speed = %d [Khz]\n", param );
     } else {
        printf ("ERROR\n");
     }

     if (USB_DCD_OK == ioctl (fd, IO_IOCTL_USB_DCD_GET_STATUS, &param))
     {
        printf ("IO_IOCTL_USB_DCD_GET_STATUS - with STATUS REG value = %x\n", param );
     } else {
        printf ("ERROR\n");
     }
}

/* EOF */
