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
* $FileName: weighscale.c$
* $Version : 3.8.4.3$
* $Date    : May-3-2012$
*
* Comments:
*
* @brief when the device is connected using continua manager it comes in 
*        operating state and after waiting for some time we send weighscale 
*        readings
*****************************************************************************/
 
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "weighscale.h"

extern void Main_Task(uint_32 param);
#define MAIN_TASK       10

TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK, Main_Task, 4000L, 7L, "Main", MQX_AUTO_START_TASK, 0, 0},
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
/* Add all the variables needed for phd_com_model.c to this structure */
WEIGHSCALE_GLOBAL_VARIABLE_STRUCT g_weighscale;
/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void USB_App_Callback(uint_32 handle, uint_8 event_type) ;
static void Send_Weights(void); 
/*****************************************************************************
 * Local Variables - None 
 *****************************************************************************/

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/******************************************************************************
 * 
 *    @name        USB_App_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       handle : handle to Identify the controller
 *    @param       event_type : value of the event    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the lower layer whenever an event occurs.
 * This sets a variable according to the event_type
 *****************************************************************************/
static void USB_App_Callback(uint_32 handle, uint_8 event_type) 
{
    UNUSED_ARGUMENT(handle)
    /* when event_type is APP_PHD_INITIALISED(transport connected, enumeration
       complete), execution will go in the else part and event will become 
       APP_PHD_INITIALISED */
    if(g_weighscale.event == APP_PHD_INITIALISED)
    {/* if transport is connected, enumeration is complete */            
        if(event_type == USB_PHD_CONNECTED_TO_HOST) 
        {   /* attributes sent (response to get_attributes) and the device is 
               ready to send measurements */ 
                        
            g_weighscale.event = USB_PHD_CONNECTED_TO_HOST;
        }
    } 
    else 
    {
       g_weighscale.event = event_type;    
    }            
    return;
}

/******************************************************************************
 *  
 *   @name        Send_Weights
 * 
 *   @brief       This function is used to send the association request after 
 *                the device is connected
 * 
 *   @param       None
 * 
 *   @return      None
 *
 *****************************************************************************
 * This function sends the measurement data or the dissociation request 
 * depending on the key pressed
 *****************************************************************************/ 
 static void Send_Weights(void) 
 {
    if(g_weighscale.num_of_msr_sent < NUM_OF_MSR_TO_SEND) 
    {        
        PHD_Send_Measurements_to_Manager(g_weighscale.app_handle,&g_weighscale.msr);
    } 
    else 
    {      
        PHD_Disconnect_from_Manager(g_weighscale.app_handle); 
    }         
 }

/******************************************************************************
 *  
 *   @name        TestApp_Init
 * 
 *   @brief       This function is the entry for the app (or other usuage)
 * 
 *   @param       None
 * 
 *   @return      None
 *
 *****************************************************************************
 * This function starts the PHDC (weighing scale) application                
 *****************************************************************************/ 
#if defined(__IAR_SYSTEMS_ICC__)
 #pragma optimize = low
#endif
void TestApp_Init(void)
{       
    uint_8 error;   
    uint_32 iteration = 0;
    uint_32 delay_count;    
    
    /* Initialize Global Variable Structure */
    USB_mem_zero(&g_weighscale, sizeof(WEIGHSCALE_GLOBAL_VARIABLE_STRUCT));
    g_weighscale.event = APP_PHD_UNINITIALISED;
    /* initialize measurements */  
    g_weighscale.msr.msr_time.century = 0x20;      
    g_weighscale.msr.msr_time.year    = 9;   
    g_weighscale.msr.msr_time.month   = 4;    
    g_weighscale.msr.msr_time.day     = 9;  
    g_weighscale.msr.msr_time.hour    = 3;   
    g_weighscale.msr.msr_time.minute  = 5;     
    g_weighscale.msr.msr_time.second  = 0;     
    g_weighscale.msr.msr_time.sec_fractions = 0;

#if (PSP_ENDIAN == MQX_LITTLE_ENDIAN)
    g_weighscale.msr.bmi[0]=  200;
    g_weighscale.msr.bmi[1]=  205; 
    g_weighscale.msr.weight[0]= 95;
    g_weighscale.msr.weight[1]= 54;
#else
    g_weighscale.msr.bmi[1]=  200;
    g_weighscale.msr.bmi[0]=  205; 
    g_weighscale.msr.weight[1]= 95;
    g_weighscale.msr.weight[0]= 54;
#endif
        
    if (MQX_OK != _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER)) {
        printf("Driver could not be installed\n");
        return;
    }

    /* Initialize the USB interface */
    error = PHD_Transport_Init((uint_32 *)&g_weighscale.app_handle,USB_App_Callback);                  
          
    while (TRUE) 
    {                                       
        switch (g_weighscale.event)        
        {
            case APP_PHD_INITIALISED:              
                /* enters here for the first time when enum is complete and 
                   event is APP_PHD_INITIALISED till the response to 
                   get_attribute is not sent (see func USB_App_Callback in the
                   same file) */              
                /* wait till the host is ready to recieve the association 
                   request */
                if(iteration < ITERATION_COUNT) 
                {                
                    iteration++;
                } 
                else 
                {                  
                    iteration=0;
                    PHD_Disconnect_from_Manager(g_weighscale.app_handle);          
                    /* connect to the manager */
                    PHD_Connect_to_Manager(g_weighscale.app_handle);                  
                } 
                break;
          
            case USB_PHD_CONNECTED_TO_HOST: 
                {   /* enters here for the first time when the device 
                       is ready to send measurements */            
                    {                               
                        delay_count=ITERATION_COUNT;/* this delay depends on SoC used*/
                        while(delay_count--);
                        /* update the measurements to send */  
                        g_weighscale.msr.msr_time.second++;
                        g_weighscale.msr.msr_time.second %= 60;
                        g_weighscale.msr.weight[0]++;  
                        g_weighscale.msr.weight[1]++;  
                        g_weighscale.msr.weight[0] %= 1000;  
                        g_weighscale.msr.weight[1] %= 1000;  
                        g_weighscale.msr.bmi[0]++;  
                        g_weighscale.msr.bmi[1]++;  
                        g_weighscale.msr.bmi[0] %= 500;  
                        g_weighscale.msr.bmi[1] %= 500;
                    }
                }
                Send_Weights();
                break;
          
            case USB_PHD_DISCONNECTED_FROM_HOST:               
                g_weighscale.event = USB_PHD_MEASUREMENT_SENT;                            
                break;
             
            case USB_PHD_MEASUREMENT_SENT: 
                /* enters here each time we recieve a response to the 
                   measurements sent */              
                g_weighscale.event = USB_PHD_CONNECTED_TO_HOST;       
                g_weighscale.num_of_msr_sent++;       
                break;
          
            default:
                g_weighscale.event = USB_PHD_CONNECTED_TO_HOST;    
                break;          
        }  
    }   
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

void Main_Task(uint_32 param)
{   
    UNUSED_ARGUMENT (param)
    TestApp_Init();     
}

/* EOF */
