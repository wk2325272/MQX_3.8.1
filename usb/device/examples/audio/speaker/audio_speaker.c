/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
* All Rights Reserved
*
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
* $FileName: audio_speaker.c$
* $Version : 3.8.3.5$
* $Date    : May-22-2012$
*
* Comments:
*
* @brief  The file emulates a speaker.
*****************************************************************************/ 

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <mqx.h>
#include <lwevent.h>
#include <bsp.h>
#include <usb.h>
#include "audio_speaker.h"
#include "audio_pwm.h"
#include "io_gpio.h"
#include "audio_timer.h"
#include "sgtl5000.h"

extern void Main_Task(uint_32 param);
#define MAIN_TASK         10

#if (defined I2S_DEVICE_INTERRUPT)
extern void Play_Task(uint_32 param);
#define PLAY_TASK         11
#endif



TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK, Main_Task, 2000L, 7L, "Main Task", MQX_AUTO_START_TASK, 0, 0},
#if (defined I2S_DEVICE_INTERRUPT)
   { PLAY_TASK, Play_Task, 500L, 7L, "Play Task", MQX_AUTO_START_TASK, 0, 0},
#endif
   { 0L, 0L, 0L, 0L, 0L, 0L, 0, 0}
};

/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/
 
/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
void TestApp_Init(void);

/****************************************************************************
 * Global Variables
 ****************************************************************************/              
extern USB_ENDPOINTS                   usb_desc_ep;
extern USB_AUDIO_UNITS                 usb_audio_unit;
extern DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback;
uint_8 audio_data_buff0[AUDIO_DATA_SIZE];
uint_8 audio_data_buff1[AUDIO_DATA_SIZE];
AUDIO_HANDLE   g_app_handle;
LWEVENT_STRUCT                         app_event;
 
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void USB_App_Callback(uint_8 event_type, void* val,pointer arg);
static void USB_Notif_Callback(uint_8 event_type,void* val,pointer arg);

/*****************************************************************************
 * Local Variables 
 *****************************************************************************/
static boolean start_app = FALSE;
static uint_8 *g_curr_recv_buf;

/*****************************************************************************
 * Local Functions
 *****************************************************************************/

 /*****************************************************************************
 *  
 *   @name        TestApp_Init
 * 
 *   @brief       This function is the entry for speaker (or other usuage)
 * 
 *   @param       None
 * 
 *   @return      None
 **
 *****************************************************************************/

void TestApp_Init(void)
{
   AUDIO_CONFIG_STRUCT       audio_config;
   USB_CLASS_AUDIO_ENDPOINT  * endPoint_ptr;

   g_curr_recv_buf = USB_mem_alloc_zero(DATA_BUFF_SIZE);
   /* Pointer to audio endpoint entry */  
   endPoint_ptr = USB_mem_alloc_zero(sizeof(USB_CLASS_AUDIO_ENDPOINT)*AUDIO_DESC_ENDPOINT_COUNT);
   /* USB descriptor endpoint */
   audio_config.usb_ep_data = &usb_desc_ep;
   /* USB audio unit */
   audio_config.usb_ut_data = &usb_audio_unit;
   /* Endpoint count */
   audio_config.desc_endpoint_cnt = AUDIO_DESC_ENDPOINT_COUNT;
   /* Application callback */
   audio_config.audio_class_callback.callback = USB_App_Callback;
   /* Application callback argurment */
   audio_config.audio_class_callback.arg = &g_app_handle;
   /* Vendor callback */
   audio_config.vendor_req_callback.callback = NULL;
   /* Vendor callback argurment */
   audio_config.vendor_req_callback.arg = NULL;
   /* Param callback function */
   audio_config.param_callback.callback = USB_Notif_Callback;
   /* Param callback argurment */
   audio_config.param_callback.arg = &g_app_handle;
   /* Memory param callback */
   audio_config.mem_param_callback.callback = NULL;
   /* Memory param callback argurment */
   audio_config.mem_param_callback.arg = &g_app_handle;
   /* Descriptor callback pointer */
   audio_config.desc_callback_ptr =  &desc_callback;
   /* Audio enpoint pointer */
   audio_config.ep = endPoint_ptr;

#if !(defined I2S_DEVICE_INTERRUPT)
   /* Initialize PWM module */
   _pwm_init();

   /* Initialize timer module */
   audio_timer_init();

   _audio_timer_init_freq(AUDIO_TIMER, AUDIO_SPEAKER_FREQUENCY,AUDIO_TIMER_CLOCK, TRUE);

#endif /* I2S_DEVICE_INTERRUPT */

    if (MQX_OK != _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER)) {
        printf("Driver could not be installed\n");
        return;
    }

   /* Initialize the USB interface */
   g_app_handle = USB_Class_Audio_Init(&audio_config);

   if (MQX_OK != _lwevent_create(&app_event, LWEVENT_AUTO_CLEAR)) {
      printf("\n_lwevent_create app_event failed.\n");
      _task_block();
   }
   
   if (MQX_OK != _lwevent_wait_ticks(&app_event, USB_APP_ENUM_COMPLETE_EVENT_MASK, FALSE, 0)) {
      printf("\n_lwevent_wait_ticks app_event failed.\n");
      _task_block();
   }
   
   if (MQX_OK != _lwevent_clear(&app_event, USB_APP_ENUM_COMPLETE_EVENT_MASK)) {
      printf("\n_lwevent_clear app_event failed.\n");
      _task_block();
   }
   printf("Audio speaker is working ... \r\n");
   USB_Class_Audio_Recv_Data(g_app_handle,AUDIO_ISOCHRONOUS_ENDPOINT,
                  g_curr_recv_buf,AUDIO_ENDPOINT_PACKET_SIZE);

   while (TRUE) 
   {
#if (defined I2S_DEVICE_INTERRUPT)
      /* switching to play task*/
      _time_delay(1);
#endif
   }
   
} 

/******************************************************************************
 * 
 *    @name        USB_App_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       handle : handle to Identify the controller
 *    @param       event_type : value of the event
 *    @param       val : gives the configuration value 
 * 
 *    @return      None
 *
 *****************************************************************************/
static void USB_App_Callback(uint_8 event_type, void* val,pointer arg) 
{
   UNUSED_ARGUMENT (arg)
   UNUSED_ARGUMENT (val)

   if(event_type == USB_APP_BUS_RESET) 
   {
      start_app=FALSE;
   }
   else if(event_type == USB_APP_ENUM_COMPLETE_EVENT_MASK) 
   {
      start_app = TRUE;
      if (MQX_OK !=_lwevent_set(&app_event, USB_APP_ENUM_COMPLETE_EVENT_MASK)){
         printf("_lwevent_set app_event failed.\n");
      }
   }
   else if(event_type == USB_APP_ERROR)
   {
   /* add user code for error handling */
   }
   return;
}

/******************************************************************************
 * 
 *    @name        USB_Notif_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       handle:  handle to Identify the controller
 *    @param       event_type : value of the event
 *    @param       val : gives the configuration value 
 * 
 *    @return      None
 *
 *****************************************************************************/
static void USB_Notif_Callback(uint_8 event_type,void* val,pointer arg) 
{
   uint_32 handle;
   uint_8 i;
   APP_DATA_STRUCT* data_receive;
   static _mqx_int audio_data_cnt = 0;
   
   handle = *((uint_32 *)arg);
   if(start_app == TRUE) 
   { 
      if(event_type == USB_APP_DATA_RECEIVED)
      {
         /* Previous Send is complete. Queue next recieve */ 
         USB_Class_Audio_Recv_Data(handle,AUDIO_ISOCHRONOUS_ENDPOINT,
           g_curr_recv_buf,AUDIO_ENDPOINT_PACKET_SIZE);
         data_receive = (APP_DATA_STRUCT*)val;
#if (defined I2S_DEVICE_INTERRUPT)
         if(!(app_event.VALUE & USB_APP_BUFFER0_FULL_EVENT_MASK)) {
            for (i = 0; i < data_receive->data_size; i++)
            {
               audio_data_buff0[audio_data_cnt] = data_receive->data_ptr[i];
               audio_data_cnt ++;
               if(AUDIO_DATA_SIZE == audio_data_cnt)
               {
                   audio_data_cnt = 0;
                   if (MQX_OK !=_lwevent_set(&app_event, USB_APP_DATA_RECEIVED_EVENT_MASK
                                              | USB_APP_BUFFER0_FULL_EVENT_MASK)){
                      printf("_lwevent_set app_event failed.\n");
                   }
               }
            }
         } else if(!(app_event.VALUE & USB_APP_BUFFER1_FULL_EVENT_MASK)) {
            for (i = 0; i < data_receive->data_size; i++)
            {
               audio_data_buff1[audio_data_cnt] = data_receive->data_ptr[i];
               audio_data_cnt ++;
               if(AUDIO_DATA_SIZE == audio_data_cnt)
               {
                   audio_data_cnt = 0;
                   if (MQX_OK !=_lwevent_set(&app_event, USB_APP_DATA_RECEIVED_EVENT_MASK 
                                              | USB_APP_BUFFER1_FULL_EVENT_MASK)){
                      printf("_lwevent_set app_event failed.\n");
                   }
               }
            }
         }
#else       
        for (i = 0; i < data_receive->data_size; i++)
        {
           audio_data_buff0[audio_data_cnt] = data_receive->data_ptr[i];
           audio_data_cnt ++;
           if(AUDIO_DATA_SIZE == audio_data_cnt)
           {
               audio_data_cnt = 0;
               if (MQX_OK !=_lwevent_set(&app_event, USB_APP_DATA_RECEIVED_EVENT_MASK)){
                  printf("\_lwevent_set app_event failed.\n");
               }
           }
        }
#endif /* I2S_DEVICE_INTERRUPT */
      }
   }
   return;
}

/*Task*----------------------------------------------------------------
* 
* Task Name  : Main_Task
* Returned Value : None
* Comments       :
*     First function called.  Calls the Test_App
*     callback functions.
* 
*END*--------------------------------------------------------------------*/
void Main_Task
(
   uint_32 param
)
{
   UNUSED_ARGUMENT (param)
   TestApp_Init();
   _time_delay(200);
   _task_block();
}

#if (defined I2S_DEVICE_INTERRUPT)
/*Task*----------------------------------------------------------------
* 
* Task Name      : Play_Task
* Returned Value : None
* Comments       :
*     This task writes audio data buffer to I2S device.
* 
*END*--------------------------------------------------------------------*/
void Play_Task
(
   uint_32 param
)
{
   MQX_FILE_PTR              i2s_ptr = NULL;
   AUDIO_DATA_FORMAT         audio_format;
   _mqx_int                  errcode;
   _mqx_int                  freq;
   _mqx_int                  app_event_value;
   i2s_ptr = fopen(I2S_DEVICE_INTERRUPT, NULL);
   if (i2s_ptr == NULL)
   {
      printf("Unable to open I2S device\n");
      _task_block();
   }
   
   errcode = InitCodec(i2s_ptr);
   if (errcode != 0)
   {
      printf("Initializing audio codec...[FAIL]\n");
      printf("  Error 0x%X\n", errcode);
      fclose(i2s_ptr);
      _task_block();
   }
   /* Audio format in little endian */
   audio_format.ENDIAN = AUDIO_LITTLE_ENDIAN;
   /* Alignment of input audio data format */
   audio_format.ALIGNMENT = AUDIO_ALIGNMENT_LEFT;
   /* Audio format bits */
   audio_format.BITS = AUDIO_FORMAT_BITS;
   /* Audio format size */
   audio_format.SIZE = AUDIO_FORMAT_SIZE;
   /* Audio format channel number */
   audio_format.CHANNELS = AUDIO_FORMAT_CHANNELS;
   
   /* Setting audio format */
   ioctl(i2s_ptr, IO_IOCTL_AUDIO_SET_IO_DATA_FORMAT, &audio_format);
   
   /* Setting over sampling clock frequency in Hz */
   freq = AUDIO_FORMAT_SAMPLE_RATE * AUDIO_I2S_FS_FREQ_DIV;
   ioctl(i2s_ptr, IO_IOCTL_I2S_SET_MCLK_FREQ, &freq);
   
   /* Setting bit clock frequency in Hz */
   freq = AUDIO_FORMAT_SAMPLE_RATE;
   ioctl(i2s_ptr, IO_IOCTL_I2S_SET_FS_FREQ, &freq);
   ioctl(i2s_ptr, IO_IOCTL_I2S_TX_DUMMY_OFF, NULL);
   
   /* Setting audio codec for SGTL5000 device */
   SetupCodec(i2s_ptr);
   
   while (TRUE) 
   {
      if (MQX_OK != _lwevent_wait_ticks(&app_event, USB_APP_DATA_RECEIVED_EVENT_MASK, FALSE, 0)) {
         printf("\n_lwevent_wait_ticks app_event failed.\n");
         fclose(i2s_ptr);
         _task_block();
      }
      app_event_value = app_event.VALUE;
      if (MQX_OK != _lwevent_clear(&app_event, USB_APP_DATA_RECEIVED_EVENT_MASK)) {
         printf("\n_lwevent_clear app_event failed.\n");
         fclose(i2s_ptr);
         _task_block();
      }
      if (USB_APP_BUFFER0_FULL_EVENT_MASK == app_event_value) {
         write(i2s_ptr, audio_data_buff0, AUDIO_DATA_SIZE);
         if (MQX_OK != _lwevent_clear(&app_event, USB_APP_BUFFER0_FULL_EVENT_MASK)) {
            printf("\n_lwevent_clear app_event failed.\n");
            fclose(i2s_ptr);
            _task_block();
         }
      } else if (USB_APP_BUFFER1_FULL_EVENT_MASK == app_event_value) {
         write(i2s_ptr, audio_data_buff1, AUDIO_DATA_SIZE);
         if (MQX_OK != _lwevent_clear(&app_event, USB_APP_BUFFER1_FULL_EVENT_MASK)) {
            printf("\n_lwevent_clear app_event failed.\n");
            fclose(i2s_ptr);
            _task_block();
         }
      }
   }/* End while */
}
#endif  
/* EOF */
