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
* $FileName: virtual_com.c$
* $Version : 3.8.5.1$
* $Date    : May-3-2012$
*
* Comments:
*
* @brief  The file emulates a USB PORT as RS232 PORT.
*****************************************************************************/ 

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "virtual_com.h"

extern void Main_Task(uint_32 param);
#define MAIN_TASK       10
TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK, Main_Task, 2*3000L, 7L, "Main", MQX_AUTO_START_TASK, 0, 0},
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
extern USB_ENDPOINTS usb_desc_ep;
extern DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback;

CDC_HANDLE   g_app_handle;

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/
 
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
void USB_App_Callback(uint_8 event_type, void* val,pointer arg);
void USB_Notif_Callback(uint_8 event_type,void* val,pointer arg); 
void Virtual_Com_App(void);
/*****************************************************************************
 * Local Variables 
 *****************************************************************************/
static boolean start_app = FALSE;
static boolean start_transactions = FALSE;
static uint_8 *g_curr_recv_buf;
static uint_8 *g_curr_send_buf;
static uint_8 g_recv_size;
static uint_8 g_send_size;
/*****************************************************************************
 * Local Functions
 *****************************************************************************/

 /*****************************************************************************
 *  
 *   @name        TestApp_Init
 * 
 *   @brief       This function is the entry for mouse (or other usuage)
 * 
 *   @param       None
 * 
 *   @return      None
 **                
 *****************************************************************************/
void TestApp_Init(void)
{       
    //uint_8   error;
    CDC_CONFIG_STRUCT cdc_config;
    USB_CLASS_CDC_ENDPOINT * endPoint_ptr;
    
    g_curr_recv_buf = _mem_alloc_uncached(DATA_BUFF_SIZE);
    g_curr_send_buf = _mem_alloc_uncached(DATA_BUFF_SIZE);
    
    endPoint_ptr = USB_mem_alloc_zero(sizeof(USB_CLASS_CDC_ENDPOINT)*CDC_DESC_ENDPOINT_COUNT);
    cdc_config.comm_feature_data_size = COMM_FEATURE_DATA_SIZE;
    cdc_config.usb_ep_data = &usb_desc_ep;
    cdc_config.desc_endpoint_cnt = CDC_DESC_ENDPOINT_COUNT;
    cdc_config.cdc_class_cb.callback = USB_App_Callback;
    cdc_config.cdc_class_cb.arg = &g_app_handle;
    cdc_config.vendor_req_callback.callback = NULL;
    cdc_config.vendor_req_callback.arg = NULL;
    cdc_config.param_callback.callback = USB_Notif_Callback;
    cdc_config.param_callback.arg = &g_app_handle;
    cdc_config.desc_callback_ptr =  &desc_callback;
    cdc_config.ep = endPoint_ptr;
    cdc_config.cic_send_endpoint = CIC_NOTIF_ENDPOINT;
    /* Always happend in control endpoint hence hard coded in Class layer*/
    //cdc_config.cic_recv_endpoint = 
    cdc_config.dic_send_endpoint = DIC_BULK_IN_ENDPOINT;
    cdc_config.dic_recv_endpoint = DIC_BULK_OUT_ENDPOINT;
    
    if (MQX_OK != _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER)) {
        printf("Driver could not be installed\n");
        return;
    }

    /* Initialize the USB interface */
    g_app_handle = USB_Class_CDC_Init(&cdc_config);
    g_recv_size = 0;
    g_send_size= 0;    
  
    while (TRUE) 
    {
        /* call the periodic task function */      
        USB_CDC_Periodic_Task();           

       /*check whether enumeration is complete or not */
        if((start_app==TRUE) && (start_transactions==TRUE))
        {        
            Virtual_Com_App(); 
        }            
    }/* Endwhile */   
} 

/******************************************************************************
 * 
 *    @name       Virtual_Com_App
 *    
 *    @brief      
 *                  
 *    @param      None
 * 
 *    @return     None
 *    
 *****************************************************************************/
void Virtual_Com_App(void)
{
    /* User Code */ 
    if(g_recv_size) 
    {
        _mqx_int i;
        
        /* Copy Buffer to Send Buff */
        for (i = 0; i < g_recv_size; i++)
        {
            printf("Copied: %c\n", g_curr_recv_buf[i]);
        	g_curr_send_buf[g_send_size++] = g_curr_recv_buf[i];
        }
        g_recv_size = 0;
    }
    
    if(g_send_size) 
    {
        uint_8 error;
        uint_8 size = g_send_size;
        g_send_size = 0;

        error = USB_Class_CDC_Send_Data(g_app_handle,DIC_BULK_IN_ENDPOINT,
        	g_curr_send_buf, size);
        if(error != USB_OK) 
        {
            /* Failure to send Data Handling code here */
        } 
    }
    return;
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
    if(event_type == USB_APP_BUS_RESET) 
    {
        start_app=FALSE;    
    }
    else if(event_type == USB_APP_ENUM_COMPLETE) 
    {
        start_app=TRUE;        
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
 
void USB_Notif_Callback(uint_8 event_type,void* val,pointer arg) 
{
    uint_32 handle;
    uint_8 index;
    
    handle = *((uint_32 *)arg);
    if(start_app == TRUE) 
    {
        if(event_type == USB_APP_CDC_DTE_ACTIVATED) 
        {
           start_transactions = TRUE;        
        } 
        else if(event_type == USB_APP_CDC_DTE_DEACTIVATED) 
        {
           start_transactions = FALSE;        
        }
        else if((event_type == USB_APP_DATA_RECEIVED)&&
                                              (start_transactions == TRUE))
        {
           
            uint_32 BytesToBeCopied;            
            APP_DATA_STRUCT* dp_rcv;
            dp_rcv = (APP_DATA_STRUCT*)val; 
            
            BytesToBeCopied = dp_rcv->data_size;
            for(index = 0; index < BytesToBeCopied; index++) 
            {
                g_curr_recv_buf[index] = dp_rcv->data_ptr[index];
                printf("Received: %c\n", g_curr_recv_buf[index]);
            }
            g_recv_size = index;

            /* Schedule buffer for next receive event */
			USB_Class_CDC_Recv_Data(handle, DIC_BULK_OUT_ENDPOINT,
                g_curr_recv_buf, DIC_BULK_OUT_ENDP_PACKET_SIZE); 

        }        
        else if((event_type == USB_APP_SEND_COMPLETE)&&
            (start_transactions == TRUE))
        { 
            /* User: add your own code for send complete event */ 
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
