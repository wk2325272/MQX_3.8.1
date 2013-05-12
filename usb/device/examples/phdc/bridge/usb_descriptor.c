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
* $FileName: usb_descriptor.c$
* $Version : 3.8.4.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief The file contains USB descriptor functions
*
*****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_descriptor.h"

/*****************************************************************************
 * Constant and Macro's
 *****************************************************************************/
/* structure containing details of all the endpoints used by this device */ 
USB_EP_STRUCT ep[PHDC_DESC_ENDPOINT_COUNT]=      
{
    {
        PHDC_BULK_OUT_EP, 
        USB_BULK_PIPE, 
        USB_RECV,
        PHDC_BULK_OUT_EP_SIZE,
    },
    {
        PHDC_BULK_IN_EP,
        USB_BULK_PIPE, 
        USB_SEND,
        PHDC_BULK_IN_EP_SIZE,
    },
    {
        PHDC_INT_IN_EP,
        USB_INTERRUPT_PIPE,
        USB_SEND,
        PHDC_INT_IN_EP_SIZE,
    }
};

USB_ENDPOINTS usb_desc_ep = 
{ 
    PHDC_DESC_ENDPOINT_COUNT,
    ep
};

 
uint_8 g_device_descriptor[DEVICE_DESCRIPTOR_SIZE] =
{
    /* Device Dexcriptor Size */
    DEVICE_DESCRIPTOR_SIZE,               
    /* Device" Type of descriptor */   
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
    0x04,0x25,
    /* Product ID */
    0x01,0x04, /* for  Bridge device */ 
    /* BCD Device version */
    0x02,0x00,
    /* Manufacturer string index */
    0x01,     
    /* Product string index */  
    0x02,                        
    /*  Serial number string index */
    0x00,                  
    /*  Number of configurations */
    DEVICE_DESC_NUM_CONFIG_SUPPOTED                           
};

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
    BUS_POWERED|SELF_POWERED|(REMOTE_WAKEUP_SUPPORT<<REMOTE_WAKEUP_SHIFT),
    /*  Current draw from bus */
    CONFIG_DESC_CURRENT_DRAWN, 
   
    /* Interface Descriptor */  
    IFACE_ONLY_DESC_SIZE,  
    USB_IFACE_DESCRIPTOR,
    0x00,
    0x00,
    /* no. of endpoints - one BULK OUT, one BULK IN,one INTERRUPT IN EP */ 
    PHDC_DESC_ENDPOINT_COUNT,   
    /* class code for PHDC */
    PHDC_CLASS,
    /* subclass code -- does not assign a subclass */
    0x00,                  
    /* protocol code -- does not assign a protocol */ 
    0x01,                    
    0x00, 
    
    /* PHDC class function descriptor */
    USB_PHDC_CLASSFUNCTION_DESC_SIZE,
    /* descriptor type */
    USB_PHDC_CLASSFUNCTION_DESCRIPTOR,     
    /* data and messaging formats not defined by 
       vendor DATA FORMAT -PHDC_11073_20601*/
    0x02,                                  
    0x00|(META_DATA_MSG_PRE_IMPLEMENTED), 
    
    /*PHDC function extension descriptor */
    USB_PHDC_FUNCTION_EXT_DESC_SIZE,
    PHDC_11073PHD_FUNCTION_DESCRIPTOR,
    0x00,
    0x02,    
    0x34, 0x12,
                     
    /*Endpoint descriptor -- BULK IN */
    ENDP_ONLY_DESC_SIZE,     
    USB_ENDPOINT_DESCRIPTOR,
    (USB_SEND << 7) | PHDC_BULK_IN_EP,                                  
    USB_BULK_PIPE,
    USB_uint_16_low(PHDC_BULK_IN_EP_SIZE),
    USB_uint_16_high(PHDC_BULK_IN_EP_SIZE), 
    /*  polling interval */ 
    0x00,                 
    
    /* PHDC QoS descriptor for BULK IN */
    USB_QOS_DESC_SIZE,
    USB_PHDC_QOS_DESCRIPTOR,
    /* qos encoding version */
    0x01,
    /*  latency/reliability bin */
    PHDC_BULK_IN_QOS,
    
    /* PHDC Meta Data descriptor for BULK IN */        
    USB_METADATA_BULK_IN_DESC_SIZE, 
    PHDC_METADATA_DESCRIPTOR,       
    /* opaque meta data */
    0xAB,
    0xCD,
    0xEF,
    0x01,
    0x02,
            
    /*Endpoint descriptor -- BULK OUT */
    ENDP_ONLY_DESC_SIZE,      
    USB_ENDPOINT_DESCRIPTOR,
    (USB_RECV << 7) | PHDC_BULK_OUT_EP,                                 
    USB_BULK_PIPE,
    USB_uint_16_low(PHDC_BULK_OUT_EP_SIZE),
    USB_uint_16_high(PHDC_BULK_OUT_EP_SIZE), 
    /*  polling interval   */
    0x00,                   
        
    /* PHDC Meta Data descriptor for BULK OUT */ 
    USB_METADATA_BULK_OUT_DESC_SIZE,
    PHDC_METADATA_DESCRIPTOR,
    /* opaque meta data */
    0xCC,
    0xDD,

    /*Endpoint descriptor -- INTERRUPT IN */
    ENDP_ONLY_DESC_SIZE,      
    USB_ENDPOINT_DESCRIPTOR,
    (USB_SEND << 7) | PHDC_INT_IN_EP,         
    USB_INTERRUPT_PIPE,
    USB_uint_16_low(PHDC_INT_IN_EP_SIZE),
    USB_uint_16_high(PHDC_INT_IN_EP_SIZE), 
    /*  polling interval  */
    0x0A,                   
    
     /* PHDC QoS descriptor for INTERRUPT IN */
    USB_QOS_DESC_SIZE,
    USB_PHDC_QOS_DESCRIPTOR,
    /* qos encoding version */
    0x01,                  
    /*  latency/reliability bin    */
    PHDC_INT_IN_QOS,       
};

#if HIGH_SPEED_DEVICE
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
        BUS_POWERED|SELF_POWERED|(REMOTE_WAKEUP_SUPPORT<<REMOTE_WAKEUP_SHIFT),     
        /*  Current draw from bus */
        CONFIG_DESC_CURRENT_DRAWN, 
       
        /* Interface Descriptor */  
        IFACE_ONLY_DESC_SIZE,  
        USB_IFACE_DESCRIPTOR,
        0x00,
        0x00,
        /* no. of endpoints - one BULK OUT, one BULK IN,one INTERRUPT IN EP */ 
        PHDC_DESC_ENDPOINT_COUNT,   
        /* class code for PHDC */
        0x0F,
        /* subclass code -- does not assign a subclass */
        0x00,                  
        /* protocol code -- does not assign a protocol */ 
        0x01,                    
        0x00, 
        
        /* PHDC class function descriptor */
        USB_PHDC_CLASSFUNCTION_DESC_SIZE,
        /* descriptor type */
        USB_PHDC_CLASSFUNCTION_DESCRIPTOR,     
        /* data and messaging formats not defined by 
           vendor DATA FORMAT -PHDC_11073_20601*/
        0x02,                                  
        0x00|(META_DATA_MSG_PRE_IMPLEMENTED), 
        
        /*PHDC function extension descriptor */
        USB_PHDC_FUNCTION_EXT_DESC_SIZE,
        PHDC_11073PHD_FUNCTION_DESCRIPTOR,
        0x00,
        0x02,    
        0x34, 0x12,
                         
        /*Endpoint descriptor -- BULK IN */
        ENDP_ONLY_DESC_SIZE,     
        USB_ENDPOINT_DESCRIPTOR,
        (USB_SEND << 7) | PHDC_BULK_IN_EP,                                  
        USB_BULK_PIPE,
        USB_uint_16_low(OTHER_SPEED_PHDC_BULK_IN_EP_SIZE),
        USB_uint_16_high(OTHER_SPEED_PHDC_BULK_IN_EP_SIZE), 
        /*  polling interval */ 
        0x00,                 
        
        /* PHDC QoS descriptor for BULK IN */
        USB_QOS_DESC_SIZE,
        USB_PHDC_QOS_DESCRIPTOR,
        /* qos encoding version */
        0x01,
        /*  latency/reliability bin */
        PHDC_BULK_IN_QOS,
        
        /* PHDC Meta Data descriptor for BULK IN */        
        USB_METADATA_BULK_IN_DESC_SIZE, 
        PHDC_METADATA_DESCRIPTOR,       
        /* opaque meta data */
        0xAB,
        0xCD,
        0xEF,
        0x01,
        0x02,
                
        /*Endpoint descriptor -- BULK OUT */
        ENDP_ONLY_DESC_SIZE,      
        USB_ENDPOINT_DESCRIPTOR,
        (USB_RECV << 7) | PHDC_BULK_OUT_EP,                                 
        USB_BULK_PIPE,
        USB_uint_16_low(OTHER_SPEED_PHDC_BULK_OUT_EP_SIZE),
        USB_uint_16_high(OTHER_SPEED_PHDC_BULK_OUT_EP_SIZE), 
        /*  polling interval   */
        0x00,                   
            
        /* PHDC Meta Data descriptor for BULK OUT */ 
        USB_METADATA_BULK_OUT_DESC_SIZE,
        PHDC_METADATA_DESCRIPTOR,
        /* opaque meta data */
        0xCC,
        0xDD,

        /*Endpoint descriptor -- INTERRUPT IN */
        ENDP_ONLY_DESC_SIZE,      
        USB_ENDPOINT_DESCRIPTOR,
        (USB_SEND << 7) | PHDC_INT_IN_EP,         
        USB_INTERRUPT_PIPE,
        USB_uint_16_low(PHDC_INT_IN_EP_SIZE),
        USB_uint_16_high(PHDC_INT_IN_EP_SIZE), 
        /*  polling interval  */
        0xF0,                   
        
         /* PHDC QoS descriptor for INTERRUPT IN */
        USB_QOS_DESC_SIZE,
        USB_PHDC_QOS_DESCRIPTOR,
        /* qos encoding version */
        0x01,                  
        /*  latency/reliability bin    */
        PHDC_INT_IN_QOS,       
    };
#endif

uint_8 USB_STR_0[USB_STR_0_SIZE+USB_STR_DESC_SIZE] = 
    { sizeof(USB_STR_0),    
      USB_STRING_DESCRIPTOR, 
      0x09,
      0x04
    };
                                    
uint_8 USB_STR_1[USB_STR_1_SIZE+USB_STR_DESC_SIZE] =
    {  sizeof(USB_STR_1),          
       USB_STRING_DESCRIPTOR,
       'F',0,
       'R',0,
       'E',0,
       'E',0,
       'S',0,
       'C',0,
       'A',0,
       'L',0,
       'E',0,
       ' ',0,
       'S',0,
       'E',0,
       'M',0,
       'I',0,
       'C',0,
       'O',0,
       'N',0,
       'D',0,
       'U',0,
       'C',0,
       'T',0,
       'O',0,
       'R',0,
       ' ',0,
       'I',0,
       'N',0,
       'C',0,
       '.',0                                 
    };

uint_8 USB_STR_2[USB_STR_2_SIZE+USB_STR_DESC_SIZE] =
    {  sizeof(USB_STR_2),          
       USB_STRING_DESCRIPTOR,                                
       'M',0,
       'Q',0,
       'X',0,
       ' ',0,
       'P',0,
       'H',0,
       'D',0,
       'C',0,
       ' ',0,
       'B',0,
       'R',0,
       'I',0,
       'D',0,
       'G',0,
       'E',0
    };

uint_8 USB_STR_n[USB_STR_n_SIZE+USB_STR_DESC_SIZE] =
    {  sizeof(USB_STR_n),         
       USB_STRING_DESCRIPTOR,                                
       'B',0,
       'A',0,
       'D',0,
       ' ',0,
       'S',0,
       'T',0,
       'R',0,
       'I',0,
       'N',0,
       'G',0,
       ' ',0,
       'I',0,
       'N',0,
       'D',0,
       'E',0,
       'X',0                                  
    };

uint_32 g_std_desc_size[USB_MAX_STD_DESCRIPTORS+1] = 
    { 0,
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
    };   
                                             
uint_8_ptr g_std_descriptors[USB_MAX_STD_DESCRIPTORS+1] = 
    { 
      NULL,
      (uint_8_ptr)g_device_descriptor,
      (uint_8_ptr)g_config_descriptor,
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
    }; 
   
uint_8 g_string_desc_size[USB_MAX_STRING_DESCRIPTORS+1] = 
    { sizeof(USB_STR_0),
      sizeof(USB_STR_1),
      sizeof(USB_STR_2),
      sizeof(USB_STR_n)
    };   
                                             
uint_8_ptr g_string_descriptors[USB_MAX_STRING_DESCRIPTORS+1] = 
    { USB_STR_0,
      USB_STR_1,
      USB_STR_2,
      USB_STR_n
    };    
USB_LANGUAGE usb_language[USB_MAX_SUPPORTED_INTERFACES] = { (uint_16)0x0409,g_string_descriptors, g_string_desc_size};                                                                                                  
USB_ALL_LANGUAGES g_languages = 
    { USB_STR_0, sizeof(USB_STR_0),
      usb_language
    };

uint_8 g_valid_config_values[USB_MAX_CONFIG_SUPPORTED+1]={0,1}; 
/****************************************************************************
 * Global Variables
 ****************************************************************************/
static uint_8 g_alternate_interface[USB_MAX_SUPPORTED_INTERFACES];
                                                        
/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes 
 *****************************************************************************/

/*****************************************************************************
 * Local Variables - None
 *****************************************************************************/
 
 
 /*****************************************************************************
 * Local Functions - None
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Descriptor
 *
 * @brief The function returns the correponding descriptor
 *
 * @param handle:        handle     
 * @param type:          type of descriptor requested     
 * @param str_num:       string index for string descriptor     
 * @param index:         string descriptor language Id     
 * @param descriptor:    output descriptor pointer
 * @param size:          size of descriptor returned
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint_8 USB_Desc_Get_Descriptor(
     uint_32 handle, 
     uint_8 type,
     uint_8 str_num, 
     uint_16 index,
     uint_8_ptr *descriptor,
     uint_32 *size) 
{
     UNUSED_ARGUMENT (handle)

    /* string descriptors are handled saperately */
    if (type == USB_STRING_DESCRIPTOR)
    { 
        if(index == 0) 
        {  
            /* return the string and size of all languages */      
            *descriptor = (uint_8_ptr)g_languages.languages_supported_string;
            *size = g_languages.languages_supported_size;            
        } 
        else 
        {
            uint_8 lang_id=0;
            uint_8 lang_index=USB_MAX_LANGUAGES_SUPPORTED;
            
            for(;lang_id< USB_MAX_LANGUAGES_SUPPORTED;lang_id++) 
            {
                /* check whether we have a string for this language */
                if(index == g_languages.usb_language[lang_id].language_id) 
                {   /* check for max descriptors */
                    if(str_num < USB_MAX_STRING_DESCRIPTORS) 
                    {   /* setup index for the string to be returned */
                        lang_index=str_num;                 
                    }
                    
                    break;                    
                }                
            }
            /* set return val for descriptor and size */
            *descriptor = (uint_8_ptr)
                g_languages.usb_language[lang_id].lang_desc[lang_index];
            *size = 
                g_languages.usb_language[lang_id].lang_desc_size[lang_index];
        }        
    }
    else if (type < USB_MAX_STD_DESCRIPTORS+1)
    {
        /* set return val for descriptor and size*/
        *descriptor = (uint_8_ptr)g_std_descriptors [type];
       
        /* if there is no descriptor then return error */
        if(*descriptor == NULL) 
        {
            return USBERR_INVALID_REQ_TYPE;
        }
        
        *size = g_std_desc_size[type];                
    }
    else /* invalid descriptor */
    {
        return USBERR_INVALID_REQ_TYPE;
    }
    
    return USB_OK;  
}

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Interface
 *
 * @brief The function returns the alternate interface
 *
 * @param handle:         handle     
 * @param interface:      interface number     
 * @param alt_interface:  output alternate interface     
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint_8 USB_Desc_Get_Interface(uint_32 handle, 
                              uint_8 interface, 
                              uint_8_ptr alt_interface)
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

/**************************************************************************//*!
 *
 * @name  USB_Desc_Set_Interface
 *
 * @brief The function sets the alternate interface
 *
 * @param handle:         handle     
 * @param interface:      interface number     
 * @param alt_interface:  input alternate interface     
 *
 * @return USB_OK                              When Successfull
 *         USBERR_INVALID_REQ_TYPE             when Error
 *****************************************************************************/
uint_8 USB_Desc_Set_Interface(uint_32 handle, 
                              uint_8 interface, 
                              uint_8 alt_interface)
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

/**************************************************************************//*!
 *
 * @name  USB_Desc_Valid_Configation
 *
 * @brief The function checks whether the configuration parameter 
 *        input is valid or not
 *
 * @param handle          handle    
 * @param config_val      configuration value     
 *
 * @return TRUE           When Valid
 *         FALSE          When Error
 *****************************************************************************/
boolean USB_Desc_Valid_Configation(uint_32 handle,
                                   uint_16 config_val)
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

/**************************************************************************//*!
 *
 * @name  USB_Desc_Remote_Wakeup
 *
 * @brief The function checks whether the remote wakeup is supported or not
 *
 * @param handle:        handle     
 *
 * @return REMOTE_WAKEUP_SUPPORT (TRUE) - if remote wakeup supported
 *****************************************************************************/
boolean USB_Desc_Remote_Wakeup(uint_32 handle) 
{
    UNUSED_ARGUMENT (handle)
    return REMOTE_WAKEUP_SUPPORT;   
}           

/**************************************************************************//*!
 *
 * @name  USB_Desc_Get_Endpoints
 *
 * @brief The function returns with the list of all non control endpoints used
 *
 * @param handle:          handle     
 *
 * @return pointer to USB_ENDPOINTS
 *****************************************************************************
 * This function returns the information about all the non control endpoints
 * implemented
 *****************************************************************************/
USB_ENDPOINTS *USB_Desc_Get_Endpoints(uint_32 handle) 
{
    UNUSED_ARGUMENT (handle)
    return &usb_desc_ep;
}         

DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback = 
{
   0xFF,
   USB_Desc_Get_Descriptor,
   USB_Desc_Get_Endpoints,
   USB_Desc_Get_Interface,
   USB_Desc_Set_Interface,
   USB_Desc_Valid_Configation,
   USB_Desc_Remote_Wakeup,
   NULL,
   NULL 
};
/* EOF */
