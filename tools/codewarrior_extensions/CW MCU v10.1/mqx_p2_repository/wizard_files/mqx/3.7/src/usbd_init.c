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
*END************************************************************************/
#include "main.h"
/* TODO: change the include file for an USB class you need to implement */
#include <usb_hid.h>

#define BCD_USB_VERSION                  (0x0200)

/* TODO: define your own endpoints here */
#define HID_DESC_ENDPOINT_COUNT (1)

#define HID_ENDPOINT (1)
#define HID_ENDPOINT_PACKET_SIZE (8)
USB_EP_STRUCT ep[HID_DESC_ENDPOINT_COUNT] = 
{
    HID_ENDPOINT, 
    USB_INTERRUPT_PIPE, 
    USB_SEND,
    HID_ENDPOINT_PACKET_SIZE
};

/* structure containing details of all the endpoints used by this device */ 
USB_ENDPOINTS usb_desc_ep =
{
    HID_DESC_ENDPOINT_COUNT,
    ep
};

#define USB_DEVICE_DESCRIPTOR     (1)
#define DEVICE_DESCRIPTOR_SIZE            (18)

#define DEVICE_DESC_DEVICE_CLASS             (0x00)
#define DEVICE_DESC_DEVICE_SUBCLASS          (0x00)
#define DEVICE_DESC_DEVICE_PROTOCOL          (0x00)
#define DEVICE_DESC_NUM_CONFIG_SUPPOTED      (0x01)
#define DEVICE_OTHER_DESC_NUM_CONFIG_SUPPOTED  (0) 
uint_8 g_device_descriptor[DEVICE_DESCRIPTOR_SIZE] =
{
    /* Device Dexcriptor Size */
    DEVICE_DESCRIPTOR_SIZE,
    /* "Device" Type of descriptor */   
    USB_DEVICE_DESCRIPTOR,
    /*  BCD USB version  */  
    USB_uint_16_low(BCD_USB_VERSION), USB_uint_16_high(BCD_USB_VERSION),
    /* Device Class is indicated in the interface descriptors */   
    DEVICE_DESC_DEVICE_CLASS,
    /*  Device Subclass is indicated in the interface descriptors  */      
    DEVICE_DESC_DEVICE_SUBCLASS,
    /*  Device Protocol  */     
    DEVICE_DESC_DEVICE_PROTOCOL,
    /* Max Packet size */
    CONTROL_MAX_PACKET_SIZE,
    /* Vendor ID */
    0x04, 0x25,
    /* Product ID */
    0x00, 0x01,  
    /* BCD Device version */
    0x02, 0x00,
    /* Manufacturer string index */
    0x01,     
    /* Product string index */  
    0x02,                        
    /*  Serial number string index */
    0x00,                  
    /*  Number of configurations */
    DEVICE_DESC_NUM_CONFIG_SUPPOTED                           
};

#define USB_CONFIG_DESCRIPTOR     (2)
#define USB_IFACE_DESCRIPTOR      (4)
#define USB_ENDPOINT_DESCRIPTOR   (5)
#define USB_HID_DESCRIPTOR        (0x21)

#define CONFIG_ONLY_DESC_SIZE             (9)
#define IFACE_ONLY_DESC_SIZE              (9)   
#define HID_ONLY_DESC_SIZE                (9)   
#define ENDP_ONLY_DESC_SIZE               (7)
#define CONFIG_DESC_SIZE                  (CONFIG_ONLY_DESC_SIZE + IFACE_ONLY_DESC_SIZE + HID_ONLY_DESC_SIZE + 1 * ENDP_ONLY_DESC_SIZE)

#define CONFIG_DESC_NUM_INTERFACES_SUPPOTED  (0x01)
#define CONFIG_DESC_CURRENT_DRAWN            (50)

#define REMOTE_WAKEUP_SHIFT              (5)
#define REMOTE_WAKEUP_SUPPORT            (0)/*1:TRUE;0:FALSE*/
uint_8 g_config_descriptor[CONFIG_DESC_SIZE] =
{   
    /* Configuration Descriptor Size - always 9 bytes*/   
    CONFIG_ONLY_DESC_SIZE,  
    /* "Configuration" type of descriptor */   
    USB_CONFIG_DESCRIPTOR,  
    /*  Total length of the Configuration descriptor */   
    USB_uint_16_low(CONFIG_DESC_SIZE),USB_uint_16_high(CONFIG_DESC_SIZE),
    /*  NumInterfaces */   
    CONFIG_DESC_NUM_INTERFACES_SUPPOTED,
    /*  Configuration Value */      
    1,
    /* Configuration Description String Index */   
    0,
    /*  Attributes.support RemoteWakeup and self power */
    BUS_POWERED | SELF_POWERED | (REMOTE_WAKEUP_SUPPORT << REMOTE_WAKEUP_SHIFT),
    /*  Current draw from bus */
    CONFIG_DESC_CURRENT_DRAWN, 
   
    /* Interface Descriptor */   
    IFACE_ONLY_DESC_SIZE,
    USB_IFACE_DESCRIPTOR,
    0x00,
    0x00,
    HID_DESC_ENDPOINT_COUNT,
    0x03,
    0x01,
    0x02,
    0x00,
   
    /* HID descriptor */
    HID_ONLY_DESC_SIZE, 
    USB_HID_DESCRIPTOR,
    0x00,0x01,
    0x00,
    0x01,
    0x22,
    0x34,0x00,

    /*Endpoint descriptor */
    ENDP_ONLY_DESC_SIZE, 
    USB_ENDPOINT_DESCRIPTOR,
    HID_ENDPOINT|(USB_SEND << 7),
    USB_INTERRUPT_PIPE, 
    HID_ENDPOINT_PACKET_SIZE, 0x00, 
    0x0A
};

#if HIGH_SPEED_DEVICE
#define USB_DEVQUAL_DESCRIPTOR              (6)
#define USB_OTHER_SPEED_DESCRIPTOR          (7)
#define DEVICE_QUALIFIER_DESCRIPTOR_SIZE    (10)
uint_8  g_device_qualifier_descriptor[DEVICE_QUALIFIER_DESCRIPTOR_SIZE] =
{
    /* Device Qualifier Descriptor Size */
    DEVICE_QUALIFIER_DESCRIPTOR_SIZE, 
    /* Type of Descriptor */
    USB_DEVQUAL_DESCRIPTOR,           
    /*  BCD USB version  */  
    USB_uint_16_low(BCD_USB_VERSION), USB_uint_16_high(BCD_USB_VERSION),
    /* bDeviceClass */
    DEVICE_DESC_DEVICE_CLASS,
    /* bDeviceSubClass */
    DEVICE_DESC_DEVICE_SUBCLASS,      
    /* bDeviceProtocol */
    DEVICE_DESC_DEVICE_PROTOCOL,      
    /* bMaxPacketSize0 */
    CONTROL_MAX_PACKET_SIZE,          
    /* bNumConfigurations */
    DEVICE_OTHER_DESC_NUM_CONFIG_SUPPOTED,  
    /* Reserved : must be zero */ 
    0x00                              
};

#define OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE  (CONFIG_DESC_SIZE)  
uint_8  g_other_speed_config_descriptor[OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE] =
{
    /* Length of this descriptor */
    CONFIG_ONLY_DESC_SIZE,     
    /* This is a Other speed config descr */
    USB_OTHER_SPEED_DESCRIPTOR,
    /*  Total length of the Configuration descriptor */   
    USB_uint_16_low(CONFIG_DESC_SIZE), USB_uint_16_high(CONFIG_DESC_SIZE),
    CONFIG_DESC_NUM_INTERFACES_SUPPOTED,
    /*value used to selct this configuration : Configuration Value */      
    1, 
    /*  Configuration Description String Index*/   
    0, 
    /*Attributes.support RemoteWakeup and self power*/
    BUS_POWERED | SELF_POWERED | (REMOTE_WAKEUP_SUPPORT << REMOTE_WAKEUP_SHIFT),     
    /*  Current draw from bus */
    CONFIG_DESC_CURRENT_DRAWN, 
   
    /* Interface Descriptor */   
    IFACE_ONLY_DESC_SIZE,
    USB_IFACE_DESCRIPTOR,
    0x00,
    0x00,
    HID_DESC_ENDPOINT_COUNT,
    0x03,
    0x01,
    0x02,
    0x00,
   
    /* HID descriptor */
    HID_ONLY_DESC_SIZE, 
    USB_HID_DESCRIPTOR,
    0x00,0x01,
    0x00,
    0x01,
    0x22,
    0x34,0x00,
     
    /*Endpoint descriptor */
    ENDP_ONLY_DESC_SIZE, 
    USB_ENDPOINT_DESCRIPTOR,
    HID_ENDPOINT | (USB_SEND << 7),
    USB_INTERRUPT_PIPE, 
    HID_ENDPOINT_PACKET_SIZE, 0x00, 
    0x0A
};
#endif

/* TODO: define additional class-specific descriptors here */
#define REPORT_DESC_SIZE                  (52)
#define USB_REPORT_DESCRIPTOR     (0x22)
uint_8 g_report_descriptor[REPORT_DESC_SIZE] =
{
    0x05, 0x01,   /* Usage Page (Generic Desktop)*/
    0x09, 0x02,   /* Usage (Mouse) */
    0xA1, 0x01,   /* Collection (Application) */
    0x09, 0x01,   /* Usage (Pointer) */
   
    0xA1, 0x00,   /* Collection (Physical) */
    0x05, 0x09,   /* Usage Page (Buttons) */
    0x19, 0x01,   /* Usage Minimun (01) */
    0x29, 0x03,   /* Usage Maximum (03) */
   
    0x15, 0x00,   /* logical Minimun (0) */
    0x25, 0x01,   /* logical Maximum (1) */
    0x95, 0x03,   /* Report Count (3) */
    0x75, 0x01,   /* Report Size (1) */
   
    0x81, 0x02,   /* Input(Data, Variable, Absolute) 3 button bits */
    0x95, 0x01,   /* Report count (1) */
    0x75, 0x05,   /* Report Size (5) */
    0x81, 0x01,   /* Input (Constant), 5 bit padding */
   
    0x05, 0x01,   /* Usage Page (Generic Desktop) */
    0x09, 0x30,   /* Usage (X) */
    0x09, 0x31,   /* Usage (Y) */
    0x09, 0x38,   /* Usage (Z) */
   
    0x15, 0x81,   /* Logical Minimum (-127) */
    0x25, 0x7F,   /* Logical Maximum (127) */
    0x75, 0x08,   /* Report Size (8) */
    0x95, 0x03,   /* Report Count (2) */
   
    0x81, 0x06,   /* Input(Data, Variable, Relative), 2 position bytes (X & Y)*/
    0xC0,         /* end collection */
    0xC0          /* end collection */
};

/* TODO: optionally define string descriptors for your application */
#define USB_STRING_DESCRIPTOR     (3)
#define USB_STR_DESC_SIZE (2)
const uint_8 USB_STR_NUM = 3;

#define USB_MAX_LANGUAGES_SUPPORTED       (1)
#define USB_STR_0_SIZE  (2 * USB_MAX_LANGUAGES_SUPPORTED)
uint_8 USB_STR_0[USB_STR_0_SIZE + USB_STR_DESC_SIZE] = 
{ 
    sizeof(USB_STR_0),    
    USB_STRING_DESCRIPTOR, 
    0x09,
    0x04 /* Supported language: 0x0409 = English, US */ 
};
                                    
#define USB_STR_1_SIZE  (56)
uint_8 USB_STR_1[USB_STR_1_SIZE + USB_STR_DESC_SIZE] =
{
    sizeof(USB_STR_1),
    USB_STRING_DESCRIPTOR,
    'F', 0,
    'R', 0,
    'E', 0,
    'E', 0,
    'S', 0,
    'C', 0,
    'A', 0,
    'L', 0,
    'E', 0,
    ' ', 0,
    'S', 0,
    'E', 0,
    'M', 0,
    'I', 0,
    'C', 0,
    'O', 0,
    'N', 0,
    'D', 0,
    'U', 0,
    'C', 0,
    'T', 0,
    'O', 0,
    'R', 0,
    ' ', 0,
    'I', 0,
    'N', 0,
    'C', 0,
    '.', 0
};


#define USB_STR_2_SIZE  (34)
uint_8 USB_STR_2[USB_STR_2_SIZE + USB_STR_DESC_SIZE] =
{
    sizeof(USB_STR_2),
    USB_STRING_DESCRIPTOR,
    ' ', 0,
    ' ', 0,
    'M', 0,
    'Q', 0,
    'X', 0,
    ' ', 0,
    'M', 0,
    'O' ,0,
    'U', 0,
    'S', 0,
    'E', 0,
    ' ', 0,
    'D', 0,
    'E', 0,
    'M', 0,
    'O', 0,
    ' ', 0
};

#define USB_STR_n_SIZE  (32)
uint_8 USB_STR_n[USB_STR_n_SIZE + USB_STR_DESC_SIZE] =
{
    sizeof(USB_STR_n),         
    USB_STRING_DESCRIPTOR,                                
    'B', 0,
    'A', 0,
    'D', 0,
    ' ', 0,
    'S', 0,
    'T', 0,
    'R', 0,
    'I', 0,
    'N', 0,
    'G', 0,
    ' ', 0,
    'I', 0,
    'N', 0,
    'D', 0,
    'E', 0,
    'X', 0                                 
};

/* TODO: define all the descriptors your application will use */
#define USB_MAX_STD_DESCRIPTORS               (8)
uint_32 g_std_desc_size[USB_MAX_STD_DESCRIPTORS + 1] =
{
    0,
    DEVICE_DESCRIPTOR_SIZE,
    CONFIG_DESC_SIZE,
    0, /* string */
    0, /* Interfdace */
    0, /* Endpoint */
    #if HIGH_SPEED_DEVICE
        DEVICE_QUALIFIER_DESCRIPTOR_SIZE,
        OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE,
    #else                                         
        0, /* Device Qualifier */
        0, /* other spped config */
    #endif
    REPORT_DESC_SIZE
};   
                                             
uint_8 *g_std_descriptors[USB_MAX_STD_DESCRIPTORS + 1] = 
{
    NULL,
    g_device_descriptor,
    g_config_descriptor,
    NULL, /* string */
    NULL, /* Interfdace */
    NULL, /* Endpoint */
    #if HIGH_SPEED_DEVICE
        g_device_qualifier_descriptor,
        g_other_speed_config_descriptor,
    #else
        NULL, /* Device Qualifier */
        NULL, /* other spped config*/
    #endif
    g_report_descriptor
}; 
   
#define USB_MAX_STRING_DESCRIPTORS        (3)
uint_8 g_string_desc_size[USB_MAX_STRING_DESCRIPTORS + 1] = 
{
    sizeof(USB_STR_0),
    sizeof(USB_STR_1),
    sizeof(USB_STR_2),
    sizeof(USB_STR_n)
};   
                                             
uint_8 *g_string_descriptors[USB_MAX_STRING_DESCRIPTORS + 1] =
{
    USB_STR_0,
    USB_STR_1,
    USB_STR_2,
    USB_STR_n
};    

#define USB_MAX_SUPPORTED_INTERFACES     (1)
USB_LANGUAGE usb_language[USB_MAX_SUPPORTED_INTERFACES] = 
{
    (uint_16) 0x0409,
    g_string_descriptors,
    g_string_desc_size
};
                                                                                                   
USB_ALL_LANGUAGES g_languages = 
{
    USB_STR_0, sizeof(USB_STR_0),
    usb_language      
};
                              
#define USB_MAX_CONFIG_SUPPORTED          (1)
uint_8 g_valid_config_values[USB_MAX_CONFIG_SUPPORTED + 1] = { 0, 1 };

uint_8 g_alternate_interface[USB_MAX_SUPPORTED_INTERFACES];

#define MOUSE_BUFF_SIZE                   (4)
static struct _mouse_variable_struct
{
    HID_HANDLE app_handle;
    boolean mouse_init; /* flag to check lower layer status*/
    uint_8 rpt_buf[MOUSE_BUFF_SIZE]; /* report/data buff for mouse application*/
    USB_CLASS_HID_ENDPOINT ep[HID_DESC_ENDPOINT_COUNT];
    uint_8 app_request_params[2]; /* for get/set idle and protocol requests*/
} g_mouse;

static uint_8 USB_Desc_Get_Descriptor(HID_HANDLE handle, uint_8 type, uint_8 str_num, uint_16 index, uint_8_ptr *descriptor, uint_32 *size);
static uint_8 USB_Desc_Get_Interface(HID_HANDLE handle, uint_8 interface, uint_8_ptr alt_interface);
static uint_8 USB_Desc_Set_Interface(HID_HANDLE handle, uint_8 interface, uint_8 alt_interface);
static boolean USB_Desc_Valid_Configation(HID_HANDLE handle, uint_16 config_val); 
static boolean USB_Desc_Remote_Wakeup(HID_HANDLE handle);
static USB_ENDPOINTS *USB_Desc_Get_Endpoints(HID_HANDLE handle); 

static void usbd_event_handler(uint_8 event_type, void* val,pointer arg); 
static uint_8 usbd_hid_handler(uint_8 request, uint_16 value, uint_8_ptr *data, uint_32* size,pointer arg); 

static DESC_CALLBACK_FUNCTIONS_STRUCT desc_callback = 
{
    0,
    USB_Desc_Get_Descriptor,
    USB_Desc_Get_Endpoints,
    USB_Desc_Get_Interface,
    USB_Desc_Set_Interface,
    USB_Desc_Valid_Configation,
    USB_Desc_Remote_Wakeup   
};

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Desc_Get_Descriptor
* Returned Value : error or USB_OK
* Comments       :
*     Handles Get Descriptor standard USB control
*END*--------------------------------------------------------------------*/

uint_8 USB_Desc_Get_Descriptor(
    HID_HANDLE handle, 
    uint_8 type,
    uint_8 str_num, 
    uint_16 index,
    uint_8_ptr *descriptor,
    uint_32 *size
) 
{
    UNUSED_ARGUMENT (handle)
    
    switch(type)  
    {
        case USB_REPORT_DESCRIPTOR: 
            {
                type = USB_MAX_STD_DESCRIPTORS;
                *descriptor = (uint_8_ptr)g_std_descriptors [type];
                *size = g_std_desc_size[type]; 
            }
            break;
        case USB_HID_DESCRIPTOR: 
            {
                type = USB_CONFIG_DESCRIPTOR ; 
                *descriptor = (uint_8_ptr)(g_std_descriptors [type]+
                    CONFIG_ONLY_DESC_SIZE+IFACE_ONLY_DESC_SIZE);
                *size = HID_ONLY_DESC_SIZE;
            }
        break;
        case USB_STRING_DESCRIPTOR: 
            {
                if(index == 0) 
                {  
                    /* return the string and size of all languages */      
                    *descriptor = (uint_8_ptr)g_languages.languages_supported_string;
                    *size = g_languages.languages_supported_size;            
                } 
                else 
                {
                    uint_8 lang_id = 0;
                    uint_8 lang_index=USB_MAX_LANGUAGES_SUPPORTED;
                
                    for(; lang_id < USB_MAX_LANGUAGES_SUPPORTED; lang_id++) 
                    {
                        /* check whether we have a string for this language */
                        if(index == g_languages.usb_language[lang_id].language_id) 
                        {   /* check for max descriptors */
                            if(str_num < USB_MAX_STRING_DESCRIPTORS) 
                            {   /* setup index for the string to be returned */
                                lang_index = str_num;                 
                            }                        
                            break;                    
                        }                    
                    }
                    /* set return val for descriptor and size */
                    *descriptor = (uint_8_ptr)
                        g_languages.usb_language[lang_id].lang_desc[lang_index];
                    *size = 
                        g_languages.usb_language[lang_id].
                            lang_desc_size[lang_index];                
                }
            }
            break;
        default :           
            if (type < USB_MAX_STD_DESCRIPTORS)
            {       
                /* set return val for descriptor and size*/
                *descriptor = (uint_8_ptr)g_std_descriptors [type];
           
                /* if there is no descriptor then return error */
                *size = g_std_desc_size[type];

                if(*descriptor == NULL) 
                {
                    return USBERR_INVALID_REQ_TYPE;
                }            

            }
            else /* invalid descriptor */
            {
                return USBERR_INVALID_REQ_TYPE;
            }
            break; 
    }/* End Switch */    
    return USB_OK;  
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Desc_Get_Interface
* Returned Value : error or USB_OK
* Comments       :
*     Handles Get Interface standard USB control
*END*--------------------------------------------------------------------*/

uint_8 USB_Desc_Get_Interface
(
    HID_HANDLE handle, 
    uint_8 interface, 
    uint_8_ptr alt_interface
)
{
    UNUSED_ARGUMENT (handle)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* get alternate interface*/
        *alt_interface = g_alternate_interface[interface];  
        return USB_OK;  
    }    
    return USBERR_INVALID_REQ_TYPE;
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Desc_Set_Interface
* Returned Value : error or USB_OK
* Comments       :
*     Handles Set Interface standard USB control
*END*--------------------------------------------------------------------*/

uint_8 USB_Desc_Set_Interface
(
    HID_HANDLE handle, 
    uint_8 interface, 
    uint_8 alt_interface
)
{
    UNUSED_ARGUMENT (handle)
    /* if interface valid */
    if(interface < USB_MAX_SUPPORTED_INTERFACES)
    {
        /* set alternate interface*/
        g_alternate_interface[interface]=alt_interface;
        return USB_OK;  
    }    
    return USBERR_INVALID_REQ_TYPE;
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Desc_Valid_Configation
* Returned Value : TRUE or FALSE
* Comments       :
*     Returns if config_val configuration number is valid
*END*--------------------------------------------------------------------*/

boolean USB_Desc_Valid_Configation
(
    HID_HANDLE handle, 
    uint_16 config_val
) 
{
    uint_8 loop_index=0;
    UNUSED_ARGUMENT (handle)
    
    /* check with only supported val right now */
    while(loop_index < (USB_MAX_CONFIG_SUPPORTED+1)) 
    {
        if(config_val == g_valid_config_values[loop_index]) 
        {          
            return TRUE;
        }
        loop_index++;  
    }    
    return FALSE;
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Desc_Remote_Wakeup
* Returned Value : TRUE or FALSE
* Comments       :
*     Returns if the device cannot be woken up remotely
*END*--------------------------------------------------------------------*/

boolean USB_Desc_Remote_Wakeup(HID_HANDLE handle) 
{
    UNUSED_ARGUMENT (handle)
    return REMOTE_WAKEUP_SUPPORT;   
}           

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Desc_Get_Endpoints
* Returned Value : pointer to USB_ENDPOINTS
* Comments       :
*     Returns pointer to the list of USB endpoints
*END*--------------------------------------------------------------------*/

USB_ENDPOINTS *USB_Desc_Get_Endpoints(HID_HANDLE handle) 
{
    UNUSED_ARGUMENT (handle)
    return &usb_desc_ep;
}         

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usbd_event_handler
* Returned Value : None
* Comments       :
*     Handles events on USB
*END*--------------------------------------------------------------------*/

void usbd_event_handler(uint_8 event_type, void* val,pointer arg) 
{    
    UNUSED_ARGUMENT (arg)
    UNUSED_ARGUMENT (val)
    
    switch(event_type)
    {
        case USB_APP_BUS_RESET:     
            /* TODO: add your bus reset event handling */
            g_mouse.mouse_init=FALSE;
            break;
        case USB_APP_ENUM_COMPLETE:    
            /* TODO: add your code when enumeration completes */
            g_mouse.mouse_init = TRUE;         
//          mouse_move();// start to move mouse pointer
            break;
        case USB_APP_SEND_COMPLETE:    
            /* TODO: add your code when enumeration completes */
//          mouse_move(); //move next mouse pointer           
            break;
        case USB_APP_ERROR: 
            /* TODO: add your code for error handling, use val as error value from HW */
            break;
        default: 
            break;          
    }   
    return;
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usbd_hid_handler
* Returned Value : None
* Comments       :
*     Handles events of USB HID device
*END*--------------------------------------------------------------------*/

uint_8 usbd_hid_handler (uint_8 request, uint_16 value, uint_8_ptr* data, uint_32* size, pointer arg) 
{
    /* TODO: add (replace) the function body by your code */
    uint_8 error = USB_OK;
    uint_8 direction =  (uint_8)((request & USB_HID_REQUEST_DIR_MASK) >>3);
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
            *size = 1;
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
            *size = 1;
            break;
                
        case USB_HID_SET_PROTOCOL_REQUEST :
            /* set the protocol sent by the host 
               0 = Boot Protocol
               1 = Report Protocol
            */
            g_mouse.app_request_params[index] = (uint_8)(value);  
            break;
    }           
    return error; 
}  

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usbd_init
* Returned Value : None
* Comments       :
*     Initializes everything for USB Device to be running
*END*--------------------------------------------------------------------*/

void usbd_init(void)
{       
    HID_CONFIG_STRUCT   config_struct;
    
    _int_disable();
    /* initialize the Global Variable Structure */
    USB_mem_zero(&g_mouse, sizeof(g_mouse));
    USB_mem_zero(&config_struct, sizeof(HID_CONFIG_STRUCT));

    /* Initialize the USB interface */
    config_struct.ep_desc_data = &usb_desc_ep;
    config_struct.hid_class_callback.callback = usbd_event_handler;
    config_struct.hid_class_callback.arg = &g_mouse.app_handle;
    config_struct.param_callback.callback = usbd_hid_handler;
    config_struct.param_callback.arg = &g_mouse.app_handle;
    config_struct.desc_callback_ptr = &desc_callback;
    config_struct.desc_endpoint_cnt = HID_DESC_ENDPOINT_COUNT;
    config_struct.ep = g_mouse.ep;

    g_mouse.app_handle = USB_Class_HID_Init(&config_struct);
    _int_enable(); 

    /* TODO: call this function periodically in your main application,
    ** to be able to react to USB communication with low delays.
    */
/*  while (1) 
    {
        USB_HID_Periodic_Task();                
    } 
*/
}
