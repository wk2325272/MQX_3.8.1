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
* $FileName: disk.c$
* $Version : 3.8.7.1$
* $Date    : May-3-2012$
*
* Comments:
*
* @brief  RAM Disk has been emulated via this Mass Storage Demo
*****************************************************************************/
 
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "disk.h"

extern void Main_Task(uint_32 param);
#define MAIN_TASK       10

TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { MAIN_TASK, Main_Task, 2*3000L, 7L, "Main", MQX_AUTO_START_TASK, 0, 0},
   { 0L, 0L, 0L, 0L, 0L, 0L, 0, 0}
};
USB_MSD_CONFIG_STRUCT  msd_config;
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
 /* Add all the variables needed for disk.c to this structure */
extern USB_ENDPOINTS usb_desc_ep;
extern DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback;
USB_CLASS_CALLBACK_STRUCT msc_class_cb;
USB_REQ_CALLBACK_STRUCT  vend_req_cb;
USB_CLASS_CALLBACK_STRUCT param_cb;
USB_CLASS_MSC_ENDPOINT *endpoint_array;

DISK_STRUCT g_disk;

#if HIGH_SPEED_DEVICE
#if PSP_MQX_CPU_IS_MPC512x || PSP_MQX_CPU_IS_MPC830x
   static uint_8 usb_recv_buff[MSD_RECV_BUFFER_SIZE] = {0};
   static uint_8 usb_send_buff[MSD_SEND_BUFFER_SIZE] = {0};
#else
    #pragma define_section hs_buffer ".usb_hs_buffer" RW
    __declspec(hs_buffer) static uint_8 usb_recv_buff[MSD_RECV_BUFFER_SIZE] = {0};
    __declspec(hs_buffer) static uint_8 usb_send_buff[MSD_SEND_BUFFER_SIZE] = {0};
#endif
#endif


/*****************************************************************************
 * Local Types - None
 *****************************************************************************/
 
/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
void USB_App_Callback(uint_8 event_type, void* val,pointer arg);
void Bulk_Transaction_Callback(uint_8 event_type,void* val,pointer arg);
void Disk_App(void);
/*****************************************************************************
 * Local Variables 
 *****************************************************************************/

/*****************************************************************************
 * Local Functions
 *****************************************************************************/


/******************************************************************************
 * 
 *    @name       Disk_App
 *    
 *    @brief      
 *                  
 *    @param      None
 * 
 *    @return     None
 *    
 *****************************************************************************/
void Disk_App(void)
{
    /* User Code */ 
    return;
}

/******************************************************************************
 * 
 *    @name        USB_App_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       pointer : 
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
        g_disk.start_app=FALSE;    
    }
    else if(event_type == USB_APP_ENUM_COMPLETE) 
    {
        g_disk.start_app=TRUE;        
    }
    else if(event_type == USB_APP_ERROR)
    {
        /* add user code for error handling */
    }
    
    return;
}

/******************************************************************************
 * 
 *    @name        Bulk_Transaction_Callback
 *    
 *    @brief       This function handles the callback  
 *                  
 *    @param       pointer : 
 *    @param       event_type : value of the event
 *    @param       val : gives the configuration value 
 * 
 *    @return      None
 *
 *****************************************************************************/
void Bulk_Transaction_Callback(uint_8 event_type, 
                               void* val, pointer arg) 
{
    PTR_LBA_APP_STRUCT lba_data_ptr;
    uint_8_ptr prevent_removal_ptr, load_eject_start_ptr;   
    PTR_DEVICE_LBA_INFO_STRUCT device_lba_info_ptr;

    UNUSED_ARGUMENT (arg)
            
    switch(event_type)
    {
        case USB_APP_DATA_RECEIVED :
            /* Add User defined code -- if required*/
            break;
        case USB_APP_SEND_COMPLETE :
            /* Add User defined code -- if required*/
            break;
        case USB_MSC_START_STOP_EJECT_MEDIA :
            load_eject_start_ptr = (uint_8_ptr)val;
            /*  Code to be added by user for starting, stopping or 
                ejecting the disk drive. e.g. starting/stopping the motor in 
                case of CD/DVD*/
            break;
        case USB_MSC_DEVICE_READ_REQUEST :          
            /* copy data from storage device before sending it on USB Bus 
               (Called before calling send_data on BULK IN endpoints)*/
            lba_data_ptr = (PTR_LBA_APP_STRUCT)val;
            /* read data from mass storage device to driver buffer */
            #if RAM_DISK_APP
                USB_mem_copy(g_disk.storage_disk + lba_data_ptr->offset,
                    lba_data_ptr->buff_ptr, 
                    lba_data_ptr->size);
            #endif         
            break;
        case USB_MSC_DEVICE_WRITE_REQUEST :
            /* copy data from USb buffer to Storage device 
               (Called before after recv_data on BULK OUT endpoints)*/
            lba_data_ptr = (PTR_LBA_APP_STRUCT)val;
            /* read data from driver buffer to mass storage device */
            #if RAM_DISK_APP
                USB_mem_copy(lba_data_ptr->buff_ptr,
                    g_disk.storage_disk + lba_data_ptr->offset,
                    lba_data_ptr->size);
            #endif                          
            break;
        case USB_MSC_DEVICE_FORMAT_COMPLETE :
            break;
        case USB_MSC_DEVICE_REMOVAL_REQUEST :
            prevent_removal_ptr = (uint_8_ptr) val;
            if(SUPPORT_DISK_LOCKING_MECHANISM)
            {                
                g_disk.disk_lock = *prevent_removal_ptr;
            }
            else if((!SUPPORT_DISK_LOCKING_MECHANISM)&&(!(*prevent_removal_ptr)))
            {
                /*there is no support for disk locking and removal of medium is enabled*/
                /* code to be added here for this condition, if required */ 
            }
            break;
        case USB_MSC_DEVICE_GET_INFO :
            device_lba_info_ptr = (PTR_DEVICE_LBA_INFO_STRUCT)val;
            #if RAM_DISK_APP
                        device_lba_info_ptr->total_lba_device_supports = TOTAL_LOGICAL_ADDRESS_BLOCKS;  
                        device_lba_info_ptr->length_of_each_lab_of_device = LENGTH_OF_EACH_LAB; 
            #endif
            device_lba_info_ptr->num_lun_supported = LOGICAL_UNIT_SUPPORTED;
            break;          
        default : 
            break;
    }
        
    return;
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
 
void TestApp_Init(void)
{       
    /* initialize the Global Variable Structure */
    endpoint_array = USB_mem_alloc_zero(sizeof(USB_CLASS_MSC_ENDPOINT)*
        MSC_DESC_ENDPOINT_COUNT);
    if(endpoint_array == NULL)
    {
        #if _DEBUG
            printf("1: memalloc failed in TestApp_Init\n");
        #endif  
        return ;
    }
    USB_mem_zero(&g_disk, sizeof(DISK_STRUCT));
    USB_mem_zero(&msd_config,sizeof(USB_MSD_CONFIG_STRUCT)); 

    msc_class_cb.callback = USB_App_Callback;
    msc_class_cb.arg = &g_disk.app_handle;
    
    vend_req_cb.callback = (USB_REQ_FUNC)NULL;
    vend_req_cb.arg = NULL;
    
    param_cb.callback = Bulk_Transaction_Callback;
    param_cb.arg = &g_disk.app_handle;

    /*Pass the configuration of the MASS STORAGE DEVICE to lower layers*/
#if RAM_DISK_APP
    msd_config.device_info.total_lba_device_supports = TOTAL_LOGICAL_ADDRESS_BLOCKS;    
    msd_config.device_info.length_of_each_lab_of_device = LENGTH_OF_EACH_LAB; 
#else
    #error "UNSUPPORTED MASS STOARGE DEVICE"        
#endif
    msd_config.device_info.num_lun_supported = LOGICAL_UNIT_SUPPORTED;  
    msd_config.implementing_disk_drive = IMPLEMENTING_DISK_DRIVE;
    /* Pass USB Interface settings to lower layer */    
    msd_config.usb_max_suported_interfaces = USB_MAX_SUPPORTED_INTERFACES;
    msd_config.bulk_in_endpoint = BULK_IN_ENDPOINT;
    msd_config.bulk_in_endpoint_packet_size =BULK_IN_ENDP_PACKET_SIZE;
    msd_config.bulk_out_endpoint = BULK_OUT_ENDPOINT;   
    msd_config.ep_desc_data = &usb_desc_ep;
    msd_config.desc_endpoint_cnt = MSC_DESC_ENDPOINT_COUNT;
    msd_config.ep = endpoint_array;
    /* Configure and Pass App buffers for send and receive to lower layers*/        
    msd_config.msd_buff.msc_lba_send_buff_size = MSD_SEND_BUFFER_SIZE;

#if HIGH_SPEED_DEVICE 
    msd_config.msd_buff.msc_lba_send_ptr = usb_send_buff;
#else
    msd_config.msd_buff.msc_lba_send_ptr = USB_mem_alloc_zero(MSD_SEND_BUFFER_SIZE);
    if(NULL == msd_config.msd_buff.msc_lba_send_ptr)
    {
        #if _DEBUG
            printf("2: memalloc failed in TestApp_Init\n");
        #endif  
        return ;
    }
#endif  
    msd_config.msd_buff.msc_lba_recv_buff_size = MSD_RECV_BUFFER_SIZE;
#if HIGH_SPEED_DEVICE
    msd_config.msd_buff.msc_lba_recv_ptr = usb_recv_buff;
#else   
    msd_config.msd_buff.msc_lba_recv_ptr = USB_mem_alloc_zero(MSD_RECV_BUFFER_SIZE);    
    if(NULL == msd_config.msd_buff.msc_lba_recv_ptr)
    {
        #if _DEBUG
            printf("3: memalloc failed in TestApp_Init\n");
        #endif
        return ;
    }
#endif  
    /* Register the callbacks to lower layers */
    msd_config.msc_class_callback = &msc_class_cb;
    msd_config.vendor_req_callback = &vend_req_cb;
    msd_config.param_callback = &param_cb;
    msd_config.desc_callback_ptr = &desc_callback;

    if (MQX_OK != _usb_device_driver_install(USBCFG_DEFAULT_DEVICE_CONTROLLER)) {
        #if _DEBUG
            printf("4: Driver could not be installed\n");
        #endif
        return;
    }

    /* Finally, Initialize the device and USB Stack layers*/    
    g_disk.app_handle = USB_Class_MSC_Init(&msd_config);

    while (TRUE) 
    {
        /* call the periodic task function */      
        USB_MSC_Periodic_Task();           

       /*check whether enumeration is complete or not */
        if(g_disk.start_app==TRUE)
        {        
            Disk_App(); 
        }            
    }/* Endwhile */   
} 

/*FUNCTION*----------------------------------------------------------------
* 
* Function Name  : Main_Task
* Returned Value : None
* Comments       :
*     First function called.  Calls Test_App
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
