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
* $FileName: mouse.c$
* $Version : 3.8.B.3$
* $Date    : Jun-18-2012$
*
* Comments:
*
* @brief  The file emulates a mouse cursor movements
*         
*****************************************************************************/
 
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "mouse.h"
#include <usb_hid.h>

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
/* Add all the variables needed for mouse.c to this structure */
extern USB_ENDPOINTS usb_desc_ep;
extern DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback;
MOUSE_GLOBAL_VARIABLE_STRUCT g_mouse;

/*****************************************************************************
 * Local Types - None
 *****************************************************************************/
 
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
void USB_App_Callback(uint_8 event_type, void* val,pointer arg); 
uint_8 USB_App_Param_Callback(uint_8 request, uint_16 value, uint_8_ptr* data, 
    uint_32* size,pointer arg); 
/*****************************************************************************
 * Local Variables 
 *****************************************************************************/

/*****************************************************************************
 * Local Functions
 *****************************************************************************/

/*****************************************************************************
* 
*      @name     move_mouse
*
*      @brief    This function gets makes the cursor on screen move left,right
*                up and down
*
*      @param    None      
*
*      @return   None
*     
* 
******************************************************************************/
void move_mouse(enum direction dir)
{
    static int x = 0, y = 0;  
    
    switch (dir) 
    {
        case RIGHT:
            g_mouse.rpt_buf[1] = 10; 
            g_mouse.rpt_buf[2] = 0; 
            x++;
            if (x > 100)
                dir++;
            
            break;
        case DOWN:
            g_mouse.rpt_buf[1] = 0; 
            g_mouse.rpt_buf[2] = 10; 
            y++;
            if (y > 100)
                dir++;
        
            break;
        case LEFT:
            g_mouse.rpt_buf[1] = (uint_8)(-10);
            g_mouse.rpt_buf[2] = 0; 
            x--;
            if (x < 0)
                dir++;
            
            break;
        case UP:
            g_mouse.rpt_buf[1] = 0; 
            g_mouse.rpt_buf[2] = (uint_8)(-10);
            y--;
            if (y < 0)
                dir = RIGHT;
            
            break;
    }    

    USB_Class_HID_Send_Data(g_mouse.app_handle, HID_ENDPOINT, g_mouse.rpt_buf, MOUSE_BUFF_SIZE);
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
    
    switch(event_type)
    {
        case USB_APP_BUS_RESET:
            g_mouse.mouse_init=FALSE;
            break;
        case USB_APP_ENUM_COMPLETE:
            g_mouse.mouse_init = TRUE;
            break;
        case USB_APP_SEND_COMPLETE:
            break;
        case USB_APP_ERROR: 
            /* user may add code here for error handling 
               NOTE : val has the value of error from h/w*/
            break;
        default: 
            break;          
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
 *                  else return error
 *
 *****************************************************************************/
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
    //uint_8 direction =  (uint_8)((request & USB_HID_REQUEST_DIR_MASK) >>3);
    uint_8 index = (uint_8)((request - 2) & USB_HID_REQUEST_TYPE_MASK); 
    
    UNUSED_ARGUMENT(arg)
    /* index == 0 for get/set idle, index == 1 for get/set protocol */        
    *size = 0;
    /* handle the class request */
    switch(request) 
    {
        case USB_HID_GET_REPORT_REQUEST :       
            *data = &g_mouse.rpt_buf[0]; /* point to the report to send */                    
            *size = MOUSE_BUFF_SIZE; /* report size */          
            break;                                        
              
        case USB_HID_SET_REPORT_REQUEST :
            for(index = 0; index < MOUSE_BUFF_SIZE ; index++) 
            {   /* copy the report sent by the host */          
                g_mouse.rpt_buf[index] = *(*data + index);
            }        
            break;
                
        case USB_HID_GET_IDLE_REQUEST :
            /* point to the current idle rate */
            *data = &g_mouse.app_request_params[index];
            *size = REQ_DATA_SIZE;
            break;
                
        case USB_HID_SET_IDLE_REQUEST :
            /* set the idle rate sent by the host */
            g_mouse.app_request_params[index] =(uint_8)((value & MSB_MASK) >> 
                HIGH_BYTE_SHIFT);
            break;
           
        case USB_HID_GET_PROTOCOL_REQUEST :
            /* point to the current protocol code 
               0 = Boot Protocol
               1 = Report Protocol*/
            *data = &g_mouse.app_request_params[index];
            *size = REQ_DATA_SIZE;
            break;
                
        case USB_HID_SET_PROTOCOL_REQUEST :
            /* set the protocol sent by the host 
               0 = Boot Protocol
               1 = Report Protocol*/
               g_mouse.app_request_params[index] = (uint_8)(value);  
               break;
    }           
    return error; 
}  
 
/******************************************************************************
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
void MouseDev_Init(void)
{       
    HID_CONFIG_STRUCT   config_struct;
    
    _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER);

    /* initialize the Global Variable Structure */
    USB_mem_zero(&g_mouse, sizeof(MOUSE_GLOBAL_VARIABLE_STRUCT));
    USB_mem_zero(&config_struct, sizeof(HID_CONFIG_STRUCT));
    /* Initialize the USB interface */
     
    config_struct.ep_desc_data = &usb_desc_ep;
    config_struct.hid_class_callback.callback =USB_App_Callback;
    config_struct.hid_class_callback.arg = &g_mouse.app_handle;
    config_struct.param_callback.callback =USB_App_Param_Callback;
    config_struct.param_callback.arg = &g_mouse.app_handle;
    config_struct.desc_callback_ptr = &desc_callback;
    config_struct.desc_endpoint_cnt = HID_DESC_ENDPOINT_COUNT;
    config_struct.ep = g_mouse.ep;

    if (MQX_OK != _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER)) {
        printf("\nError: Driver could not be installed");
        return;
    }
  
    g_mouse.app_handle = USB_Class_HID_Init(&config_struct);

    printf("\n\nDevice initialization finished. Attach USB connector to the host PC.(board will act as mouse)");
    fflush(stdout);
} 

/* EOF */
