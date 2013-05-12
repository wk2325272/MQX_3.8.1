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
* $FileName: audio_generator.c$
* $Version : 
* $Date    : 
*
* Comments:
*
* @brief  The file emulates a generator.
*****************************************************************************/ 

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "audio_generator.h"

/*****************************************************************************
 * Constant and Macro's - None
 *****************************************************************************/
 #define MAIN_TASK       10
 
/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
void TestApp_Init(void);
void USB_Prepare_Data(void);
extern void Main_Task(uint_32 param);

/****************************************************************************
 * Global Variables
 ****************************************************************************/              
extern USB_ENDPOINTS usb_desc_ep;
extern USB_AUDIO_UNITS usb_audio_unit;
extern DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback;

extern const unsigned char wav_data[];
extern const uint_16 wav_size;

uint_8 wav_buff[8];
uint_32 audio_position = 0;

AUDIO_HANDLE   g_app_handle;
TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK, Main_Task, 2*3000L, 7L, "Main", MQX_AUTO_START_TASK, 0, 0},
   { 0L, 0L, 0L, 0L, 0L, 0L, 0, 0}
};

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/
 
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
void USB_App_Callback(uint_8 event_type, void* val,pointer arg);
void USB_Notif_Callback(uint_8 event_type,void* val,pointer arg);
uint_8 USB_App_Param_Callback(uint_8 request, uint_16 value, uint_8_ptr* data, 
    uint_32* size,pointer arg); 

/*****************************************************************************
 * Local Variables 
 *****************************************************************************/
static boolean start_app = FALSE;
static boolean start_send = TRUE;
/*****************************************************************************
 * Local Functions
 *****************************************************************************/

 /******************************************************************************
 *
 *   @name        USB_Prepare_Data
 *
 *   @brief       This function prepares data to send
 *
 *   @param       None
 *
 *   @return      None
 *****************************************************************************
 * This function prepare data before sending
 *****************************************************************************/
void USB_Prepare_Data(void)
{
   uint_8 k;
   /* copy data to buffer */
   for(k=0;k<8;k++,audio_position++)
   {
      wav_buff[k] = wav_data[audio_position];
   }
}

 /*****************************************************************************
 *  
 *   @name        TestApp_Init
 * 
 *   @brief       This function is the entry for Audio generator
 * 
 *   @param       None
 * 
 *   @return      None
 **                
 *****************************************************************************/
void TestApp_Init(void)
{
   uint_16 i,j;
   AUDIO_CONFIG_STRUCT audio_config;
   USB_CLASS_AUDIO_ENDPOINT * endPoint_ptr;

   endPoint_ptr = USB_mem_alloc_zero(sizeof(USB_CLASS_AUDIO_ENDPOINT)*AUDIO_DESC_ENDPOINT_COUNT);
   audio_config.usb_ep_data = &usb_desc_ep;
   audio_config.usb_ut_data = &usb_audio_unit;
   audio_config.desc_endpoint_cnt = AUDIO_DESC_ENDPOINT_COUNT;
   audio_config.audio_class_callback.callback = USB_App_Callback;
   audio_config.audio_class_callback.arg = &g_app_handle;
   audio_config.vendor_req_callback.callback = NULL;
   audio_config.vendor_req_callback.arg = NULL;
   audio_config.param_callback.callback = USB_Notif_Callback;
   audio_config.mem_param_callback.callback = USB_App_Param_Callback;
   audio_config.mem_param_callback.arg = &g_app_handle;
   audio_config.param_callback.arg = &g_app_handle;
   audio_config.desc_callback_ptr =  &desc_callback;
   audio_config.ep = endPoint_ptr;

    if (MQX_OK != _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER)) {
        printf("Driver could not be installed\n");
        return;
    }

   /* Initialize the USB interface */
   g_app_handle = USB_Class_Audio_Init(&audio_config);

   while (TRUE) 
   {
      /*check whether enumeration is complete or not */
      if((start_app==TRUE)  && (start_send == TRUE))
      { 
         for(i=0;i<60000;i++)
            for(j=0;j<100;j++);
         start_send = FALSE;
         USB_Prepare_Data();
         USB_Class_Audio_Send_Data(g_app_handle, AUDIO_ENDPOINT, wav_buff,8) ;
      }
   }/* Endwhile */
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
void USB_App_Callback(uint_8 event_type, void* val,pointer arg) 
{
   UNUSED_ARGUMENT (arg)
   UNUSED_ARGUMENT (val)

   if ((event_type == USB_APP_BUS_RESET) || (event_type == USB_APP_CONFIG_CHANGED))
   {
      start_app=FALSE;
   }
   else if(event_type == USB_APP_ENUM_COMPLETE) 
   {
      start_app=TRUE; 
      printf("Audio generator is working ... \r\n");       
   }
   else if(event_type == USB_APP_ERROR)
   {
      /* add user code for error handling */
   }
   return;
}

/******************************************************************************
 * 
 *    @name        USB_App_Param_Callback
 *    
 *    @brief       This function handles the callback for Get/Set report req  
 *                  
 *    @param       request  :  request type
 *    @param       value    :  give report type and id
 *    @param       data     :  pointer to the data 
 *    @param       size     :  size of the transfer
 *
 *    @return      status
 *                  USB_OK  :  if successful
 *                              else return error
 *
 *****************************************************************************/
uint_8 endpoint_memory[5] = {0x01, 0x02, 0x03, 0x04, 0x05};
uint_8 USB_App_Param_Callback
(
   uint_8 request, 
   uint_16 value, 
   uint_8_ptr* data, 
   uint_32* size,
   pointer arg
) 
{
   uint_8 error = USB_OK;
   uint_8 index;

   UNUSED_ARGUMENT(arg)
   /* handle the class request */
   switch(request) 
   {
      case GET_MEM :
         *data = &endpoint_memory[value]; /* point to the report to send */                    
         break;
      case SET_MEM :
         for(index = 0; index < *size ; index++) 
         {   /* copy the report sent by the host */          
            endpoint_memory[value + index] = *(*data + index);
         }
         *size = 0;
         break;
      default:
         break;
   }
   return error;
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
void USB_Notif_Callback(uint_8 event_type,void* val,pointer arg) 
{
   uint_32 handle;

   handle = *((uint_32 *)arg);

   if(start_app == TRUE) 
   {
      if(event_type == USB_APP_SEND_COMPLETE)
      {
         if(audio_position > wav_size)
         {
            audio_position = 0;
         }
         USB_Prepare_Data();

         USB_Class_Audio_Send_Data(g_app_handle, AUDIO_ENDPOINT, wav_buff,(sizeof(wav_buff)/sizeof(wav_buff[0]))) ;
      }
   }
   return;
}
/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : Main_Task
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

}
/* EOF */
