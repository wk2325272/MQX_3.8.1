/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: audio.c$
* $Version : 
* $Date    : 
*
* Comments:
*
*   This file is an example of device drivers for the Audio host class. This example
*   demonstrates the audio transfer capability of audio host class with audio devices.
*
*END************************************************************************/
#include <mqx.h>
#include <lwevent.h>
#include <fio.h>

#include <bsp.h>
#include <usb_host_hub_sm.h>

#include <usb.h>
#include <usb_host_audio.h>

#include "kbi.h"
#include "audio_timer.h"
#include "audio_pwm.h"

#include "audio.h"
  

/***************************************
**
** Macros
****************************************/
#define USB_EVENT_CTRL 0x01
#define USB_EVENT_DATA 0x02

/***************************************
**
** Global functions
****************************************/

/***************************************
**
** Local functions
****************************************/
static USB_STATUS check_device_type (USB_AUDIO_CTRL_DESC_IT_PTR,USB_AUDIO_CTRL_DESC_OT_PTR,char_ptr _PTR_,char_ptr);
static void USB_Prepare_Data(void);
void Main_Task(uint_32);

/***************************************
**
** Global variables
****************************************/
char                                    device_direction = UNDEFINE_DEVICE;
extern const uchar                      wav_data[];
extern const uint_32                    wav_size;
uint_8                                  wav_buff[MAX_ISO_PACKET_SIZE];
uint_8                                  wav_recv_buff[MAX_ISO_PACKET_SIZE];
uint_8                                  wav_recv_buff_tmp[MAX_ISO_PACKET_SIZE];
uint_32                                 packet_size=8;
uint_8                                  resolution_size;
boolean                                 play = FALSE;
uint_8                                  sample_out;
uint_8                                  sample_step;
uint_8                                  mute_flag = FALSE;
volatile AUDIO_CONTROL_DEVICE_STRUCT    audio_control = { 0 };
volatile AUDIO_CONTROL_DEVICE_STRUCT    audio_stream  = { 0 };
volatile boolean                        trCallBack    = FALSE;
static uint_8                           error_state=0;

/***************************************
**
** Local variables
****************************************/
/* Table of driver capabilities this application wants to use */
static  USB_HOST_DRIVER_INFO DriverInfoTable[] =
{
   {
      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_AUDIO,              /* Class code                       */
      USB_SUBCLASS_AUD_CONTROL,     /* Sub-Class code                   */
      0xFF,                         /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_audio_control_event  /* Application call back function   */
   },
   {
      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_AUDIO,              /* Class code                       */
      USB_SUBCLASS_AUD_STREAMING,   /* Sub-Class code                   */
      0xFF,                         /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_audio_stream_event   /* Application call back function   */
   },
   /* USB 1.1 hub */
   {

      {0x00,0x00},                  /* Vendor ID per USB-IF             */
      {0x00,0x00},                  /* Product ID per manufacturer      */
      USB_CLASS_HUB,                /* Class code                       */
      USB_SUBCLASS_HUB_NONE,        /* Sub-Class code                   */
      USB_PROTOCOL_HUB_LS,          /* Protocol                         */
      0,                            /* Reserved                         */
      usb_host_hub_device_event     /* Application call back function   */
   },
   {
      {0x00,0x00},                  /* All-zero entry terminates        */
      {0x00,0x00},                  /* driver info list.                */
      0,
      0,
      0,
      0,
      NULL
   }
};

extern LWEVENT_STRUCT            USB_Event;
LWEVENT_STRUCT_PTR               device_registered;

/* Input Terminal types */
static char_ptr it_type_string[NUMBER_OF_IT_TYPE] = 
{
    "Input Undefined",
    "Microphone",
    "Desktop Microphone",
    "Personal Microphone",
    "Omni directional Microphone",
    "Microphone array",
    "Processing Microphone array",
};

/* Output Terminal types */
static char_ptr ot_type_string[NUMBER_OF_OT_TYPE] = 
{
    "Output Undefined",
    "Speaker",
    "Headphones",
    "Head Mounted Display Audio",
    "Desktop Speaker",
    "Room Speaker",
    "Communication Speaker",
    "Low Frequency Effects Speaker",
};

static char_ptr                         device_string;
static uint_32                          audio_position = 0;
static USB_AUDIO_CONTROL_STATUS audio_status_interrupt;
static AUDIO_COMMAND         audio_com;
static uint_8 g_cur_mute[1] ={0x00};

TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   { 10,      Main_Task,          5000L,    10L, "Main",    MQX_AUTO_START_TASK},
   { 0L,             0L,             0L,     0L, 0L,          0L }
};


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Prepare_Data
* Returned Value : None
* Comments       :
*    This function prepares data to send. 
*
*END*--------------------------------------------------------------------*/
static void USB_Prepare_Data(void)
{
    uint_8 k;
    /* copy data to buffer */
    for(k=1;k<packet_size+1;k++,audio_position+= sample_step)
    {
        wav_buff[resolution_size*k - 1]   = wav_data[audio_position];
    }
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : check_device_type
* Returned Value : None
* Comments       :
*    This function check whether the attached device is out-device or in-device. 
*
*END*--------------------------------------------------------------------*/
USB_STATUS check_device_type
    (
    /* [IN] Input terminal descriptor */
    USB_AUDIO_CTRL_DESC_IT_PTR      it_desc,

    /* [IN] Output terminal descriptor */
    USB_AUDIO_CTRL_DESC_OT_PTR      ot_desc,

    /* [OUT] Terminal type name */
    char_ptr _PTR_                  device_type,

    /* [OUT] device direction */
    char_ptr                        direction
    )
{
    uchar it_type_high, it_type_low, ot_type_high, ot_type_low;

    it_type_high = it_desc->wTerminalType[0];
    it_type_low  = it_desc->wTerminalType[1];
    ot_type_high = ot_desc->wTerminalType[0];
    ot_type_low  = ot_desc->wTerminalType[1];

    /* Input terminal associates with audio streaming */
    if (USB_TERMINAL_TYPE == it_type_low)
    {
        *direction = IN_DEVICE;
    }
    /* Input terminal type */
    else if (INPUT_TERMINAL_TYPE == it_type_low)
    {
        /* get type device name */
        *device_type = it_type_string[it_type_high];
    }
    else
    {
        return USBERR_ERROR;
    }

    /* Output terminal associates with audio streaming */
    if (USB_TERMINAL_TYPE == ot_type_low)
    {
        if (IN_DEVICE == (*direction))
        {
            *direction = UNDEFINE_DEVICE;
            return USBERR_ERROR;
        }
        else
        {
            *direction = OUT_DEVICE;
        }
    }
    /* Output terminal type */
    else if (OUTPUT_TERMINAL_TYPE == ot_type_low)
    {
        /* get type device name */
        *device_type = ot_type_string[it_type_high];
    }
    else
    {
        return USBERR_ERROR;
    }
    return USB_OK;
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : USB_Audio_Get_Packet_Size
* Returned Value : None
* Comments       :
*     This function gets the frequency supported by the attached device.
*
*END*--------------------------------------------------------------------*/
static uint_32 USB_Audio_Get_Packet_Size
    (
        /* [IN] Point to format type descriptor */
        USB_AUDIO_STREAM_DESC_FORMAT_TYPE_PTR format_type_desc
    )
{
    uint_32 packet_size_tmp;
    /* copy data to buffer */
    packet_size_tmp = (format_type_desc->tSamFreq[2] << 16) |
                      (format_type_desc->tSamFreq[1] << 8)  |
                      (format_type_desc->tSamFreq[0] << 0);
                      
    return (packet_size_tmp/1000);
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : Main_Task
* Returned Value : none
* Comments       :
*     Execution starts here
*
*END*--------------------------------------------------------------------*/
void Main_Task ( uint_32 param )
{ /* Body */
   USB_STATUS           status = USB_OK;
   _usb_host_handle     host_handle;
   
   if (NULL == (device_registered = _mem_alloc(sizeof(*device_registered))))
   {
       printf("\nMemory allocation failed");
      _task_block();
   }
   if (USB_OK != _lwevent_create(device_registered, LWEVENT_AUTO_CLEAR))
   {
      printf("\nlwevent create failed");
      _task_block();
   }
   /* _usb_otg_init needs to be done with interrupts disabled */
   _int_disable();

   _int_install_unexpected_isr();
   if (MQX_OK != _usb_host_driver_install(USBCFG_DEFAULT_HOST_CONTROLLER)) {
      printf("\n Driver installation failed");
      _task_block();
   }

   /*
   ** It means that we are going to act like host, so we initialize the
   ** host stack. This call will allow USB system to allocate memory for
   ** data structures, it uses later (e.g pipes etc.).
   */
   status = _usb_host_init (USBCFG_DEFAULT_HOST_CONTROLLER, &host_handle);

   if (status != USB_OK) 
   {
      printf("\nUSB Host Initialization failed. STATUS: %x", status);
      _int_enable();
      _task_block();
   }

   /*
   ** since we are going to act as the host driver, register the driver
   ** information for wanted class/subclass/protocols
   */
   status = _usb_host_driver_info_register (
                                    host_handle,
                                    DriverInfoTable
                                    );
   if (status != USB_OK) {
      printf("\nDriver Registration failed. STATUS: %x", status);
      _int_enable();
      _task_block();
   }
   _int_enable();
   
   printf("USB Host Audio Demo\nWaitting for USB Audio Device to be attached...\n");
   switch_init();
   _pwm_init();
   
   audio_timer_init();
#ifdef BSP_BUTTON2
   play = FALSE;
#else
   play = TRUE;
#endif
   while (1) 
   {
      if(USB_DEVICE_INTERFACED == audio_control.DEV_STATE)
      {
         polling_switch();
         if((0==error_state)&&(mute_flag==1))
         {
            mute_flag=0;
            g_cur_mute[0]=1-g_cur_mute[0];
            if (FALSE == g_cur_mute[0])
            {
               printf("Set Mute ON\n");
            }
            else
            {
               printf("Set Mute OFF\n");
            }
            if(1 == play)
            {
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
               DisableCmtInterrupt();
#elif (defined BSP_TWRMCF51JF)
               DisableFTM0Interrupt();
#else
               _audio_timer_mask_int(BSP_LAST_TIMER); 
#endif
            }
            /* Send set mute request */
            audio_com.CLASS_PTR = (CLASS_CALL_STRUCT_PTR)&audio_control.CLASS_INTF;
            audio_com.CALLBACK_FN = usb_host_audio_mute_ctrl_callback;
            audio_com.CALLBACK_PARAM = 0;
            usb_class_audio_set_cur_mute(&audio_com,(pointer)g_cur_mute);
            _lwevent_wait_ticks(&USB_Event, USB_EVENT_CTRL | USB_EVENT_DATA, FALSE, 0);
        }
      }
      fflush(stdout);
   }
} /* Endbody */


/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_audio_mute_ctrl_callback
* Returned Value : None
* Comments       :
*     Called when a mute request is sent successfully.
*
*END*--------------------------------------------------------------------*/
void usb_host_audio_mute_ctrl_callback
   (
      /* [IN] pointer to pipe */
      _usb_pipe_handle  pipe_handle,

      /* [IN] user-defined parameter */
      pointer           user_parm,

      /* [IN] buffer address */
      uchar_ptr         buffer,

      /* [IN] length of data transferred */
      uint_32           buflen,

      /* [IN] status, hopefully USB_OK or USB_DONE */
      uint_32           status
   )
{ 
   /* Body */
   printf("Set Mute successfully\n");
   fflush(stdout);
   if(TRUE == play)
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
    EnableCmtInterrupt();
#elif (defined BSP_TWRMCF51JF)
    EnableFTM0Interrupt();
#else
    _audio_timer_unmask_int(BSP_LAST_TIMER);    
#endif
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_audio_tr_callback
* Returned Value : None
* Comments       :
*     Called when a ISO packet is sent/received successfully.
*
*END*--------------------------------------------------------------------*/
void usb_host_audio_tr_callback(
    /* [IN] pointer to pipe */
    _usb_pipe_handle pipe_handle,
    
    /* [IN] user-defined parameter */
    pointer user_parm,
    
    /* [IN] buffer address */
    uchar_ptr buffer,
    
    /* [IN] length of data transferred */
    uint_32 buflen,
    
    /* [IN] status, hopefully USB_OK or USB_DONE */
    uint_32 status)
{
   trCallBack=TRUE;

   if (IN_DEVICE == device_direction)
   {
      if(audio_position > wav_size)
      {
         audio_position = 0;
      }
      USB_Prepare_Data();     
   }
   else
   {
      uint_8 k;
      for(k=0; k < packet_size; k++)
      {
         wav_recv_buff_tmp[k] = wav_recv_buff[resolution_size -1 + resolution_size*k];
      }
   }
}

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_audio_interrupt_callback
* Returned Value : None
* Comments       :
*     Called when a interrupt pipe transfer is completed.
*
*END*--------------------------------------------------------------------*/
void usb_host_audio_interrupt_callback
   (
      /* [IN] pointer to pipe */
      _usb_pipe_handle  pipe_handle,

      /* [IN] user-defined parameter */
      pointer           user_parm,

      /* [IN] buffer address */
      uchar_ptr         buffer,

      /* [IN] length of data transferred */
      uint_32           buflen,

      /* [IN] status, hopefully USB_OK or USB_DONE */
      uint_32           status
   )
{ /* Body */
   if(!status)
   {
      audio_status_interrupt.Status=buffer[0];
      audio_status_interrupt.Originator=buffer[1];
      printf("\nStatus Interrupt:");
      printf("   Status: %d",audio_status_interrupt.Status);   
      printf("   Originator: %d",audio_status_interrupt.Originator);
   }
   fflush(stdout);
} /* Endbody */

/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_audio_control_event
* Returned Value : None
* Comments       :
*     Called when control interface has been attached, detached, etc.
*END*--------------------------------------------------------------------*/
void usb_host_audio_control_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint_32                          event_code
   )
{
   INTERFACE_DESCRIPTOR_PTR   intf_ptr =
      (INTERFACE_DESCRIPTOR_PTR)intf_handle;
    
   fflush(stdout);
   switch (event_code) {
      case USB_CONFIG_EVENT:
      /* Drop through into attach, same processing */
      case USB_ATTACH_EVENT: 
      {
         USB_AUDIO_CTRL_DESC_HEADER_PTR     header_desc = NULL;
         USB_AUDIO_CTRL_DESC_IT_PTR       it_desc  = NULL;
         USB_AUDIO_CTRL_DESC_OT_PTR       ot_desc  = NULL;
         USB_AUDIO_CTRL_DESC_FU_PTR       fu_desc  = NULL;

         if((audio_stream.DEV_STATE == USB_DEVICE_IDLE) || (audio_stream.DEV_STATE == USB_DEVICE_DETACHED)) 
         {
            audio_control.DEV_HANDLE  = dev_handle;
            audio_control.INTF_HANDLE = intf_handle;
            audio_control.DEV_STATE   = USB_DEVICE_ATTACHED;
         }
         else
         {
            printf("Audio device already attached\n");
            fflush(stdout);
         }

         /* finds all the descriptors in the configuration */
         if (USB_OK != usb_class_audio_control_get_descriptors(dev_handle,
             intf_handle,
             &header_desc,
             &it_desc,
             &ot_desc,
             &fu_desc))
         {
            break;
         };

           /* initialize new interface members and select this interface */
         if (USB_OK != _usb_hostdev_select_interface(dev_handle,
            intf_handle, (pointer)&audio_control.CLASS_INTF))
         {
            break;
         }

         /* set all info got from descriptors to the class interface struct */
         usb_class_audio_control_set_descriptors((pointer)&audio_control.CLASS_INTF,
            header_desc, it_desc, ot_desc, fu_desc);

         if(USB_OK != check_device_type(it_desc, ot_desc, &device_string, &device_direction))
         {
            error_state=1;
            break;
         }

         printf("----- Audio control interface: attach event -----\n");
         fflush(stdout);
         printf("State = attached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         break;
      }
      case USB_INTF_EVENT: 
      {
         USB_STATUS status;

         status = usb_class_audio_init_ipipe((CLASS_CALL_STRUCT_PTR)&audio_control.CLASS_INTF,
         usb_host_audio_interrupt_callback,NULL);

         if ((status != USB_OK) && (status != USBERR_OPEN_PIPE_FAILED))
             break;

         printf("----- Audio control interface: interface event -----\n");
         audio_control.DEV_STATE = USB_DEVICE_INTERFACED;
         break;
      }

      case USB_DETACH_EVENT:
      {
         AUDIO_CONTROL_INTERFACE_STRUCT_PTR if_ptr;
   
         if_ptr = (AUDIO_CONTROL_INTERFACE_STRUCT_PTR) audio_control.CLASS_INTF.class_intf_handle;
         
         _lwevent_destroy(&if_ptr->control_event);

         printf("----- Audio control interface: detach event -----\n");
         fflush(stdout);
         printf("State = detached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);
         audio_control.DEV_HANDLE = NULL;
         audio_control.INTF_HANDLE = NULL;
         audio_control.DEV_STATE = USB_DEVICE_DETACHED;
         error_state=0;
         device_direction = UNDEFINE_DEVICE;
         break;
      }
      default:
         printf("Audio Device: unknown control event\n");
         fflush(stdout);
         break;
   } /* EndSwitch */
   fflush(stdout);
} /* Endbody */
   
/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : usb_host_audio_stream_event
* Returned Value : None
* Comments       :
*     Called when stream interface has been attached, detached, etc.
*END*--------------------------------------------------------------------*/
void usb_host_audio_stream_event
   (
      /* [IN] pointer to device instance */
      _usb_device_instance_handle      dev_handle,

      /* [IN] pointer to interface descriptor */
      _usb_interface_descriptor_handle intf_handle,

      /* [IN] code number for event causing callback */
      uint_32                          event_code
   )
{ /* Body */
   INTERFACE_DESCRIPTOR_PTR   intf_ptr =
      (INTERFACE_DESCRIPTOR_PTR)intf_handle;
   
   /* Check audio stream interface alternating 0 */
   if (intf_ptr->bNumEndpoints == 0)
        return;
   
   switch (event_code) {
      case USB_CONFIG_EVENT:
      /* Drop through into attach, same processing */
      case USB_ATTACH_EVENT: 
      {
         USB_AUDIO_STREAM_DESC_SPEPIFIC_AS_IF_PTR     as_itf_desc = NULL;
         USB_AUDIO_STREAM_DESC_FORMAT_TYPE_PTR        frm_type_desc = NULL;
         USB_AUDIO_STREAM_DESC_SPECIFIC_ISO_ENDP_PTR  iso_endp_spec_desc = NULL;

        if((audio_stream.DEV_STATE == USB_DEVICE_IDLE) || (audio_stream.DEV_STATE == USB_DEVICE_DETACHED)) 
         {
            audio_stream.DEV_HANDLE  = dev_handle;
            audio_stream.INTF_HANDLE = intf_handle;
            audio_stream.DEV_STATE   = USB_DEVICE_ATTACHED;
         }
         else
         {
            printf("Audio device already attached\n");
            fflush(stdout);
         }

         /* finds all the descriptors in the configuration */
         if (USB_OK != usb_class_audio_stream_get_descriptors(dev_handle,
             intf_handle,
             &as_itf_desc,
             &frm_type_desc,
             &iso_endp_spec_desc))
         {
            break;
         };

         /* initialize new interface members and select this interface */
         if (USB_OK != _usb_hostdev_select_interface(dev_handle,
            intf_handle, (pointer)&audio_stream.CLASS_INTF))
         {
              break;
         }

         packet_size = USB_Audio_Get_Packet_Size(frm_type_desc);
         resolution_size = frm_type_desc->bBitResolution / 8;


         if (OUT_DEVICE == device_direction)
         {
            sample_out = packet_size / 8;
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
            cmt_init_freq(AUDIO_MICROPHONE_FREQUENCY);
#elif (defined BSP_TWRMCF51JF)
            ftm0_init_freq(AUDIO_MICROPHONE_FREQUENCY);
#else
            _audio_timer_init_freq(BSP_LAST_TIMER,
                 AUDIO_MICROPHONE_FREQUENCY,AUDIO_TIMER_CLOCK, FALSE);
#endif
         }
         else
         {
            sample_step = 48/packet_size;
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
            cmt_init_freq(AUDIO_SPEAKER_FREQUENCY);
#elif (defined BSP_TWRMCF51JF)
            ftm0_init_freq(AUDIO_SPEAKER_FREQUENCY);
#else
            _audio_timer_init_freq(BSP_LAST_TIMER,
                 AUDIO_SPEAKER_FREQUENCY,AUDIO_TIMER_CLOCK, FALSE);
#endif 
         }
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
         DisableCmtInterrupt();
#elif (defined BSP_TWRMCF51JF)
         DisableFTM0Interrupt();
#else
         _audio_timer_mask_int(BSP_LAST_TIMER); 
#endif

         /* set all info got from descriptors to the class interface struct */
         usb_class_audio_stream_set_descriptors((pointer)&audio_stream.CLASS_INTF,
            as_itf_desc, frm_type_desc, iso_endp_spec_desc);

         printf("----- Audio stream interface: attach event -----\n");
         fflush(stdout);
         printf("State = attached");
         printf("  Class = %d", intf_ptr->bInterfaceClass);
         printf("  SubClass = %d", intf_ptr->bInterfaceSubClass);
         printf("  Protocol = %d\n", intf_ptr->bInterfaceProtocol);
         fflush(stdout);

         break;
      }   
      case USB_INTF_EVENT: 
      {
         if(0==error_state)
         {
            audio_stream.DEV_STATE = USB_DEVICE_INTERFACED;
            printf("----- Audio stream interface: inteface event-----\n");
            printf("Audio device information:\n");
            printf("   - Device type    : %s\n", device_string);
            printf("   - Frequency      : %d KHz\n", packet_size);
            printf("   - Bit resolution : %d bit\n", resolution_size*8); 
            fflush(stdout);
         }
         else
         {
            printf("The device is unsupported!\n"); 
            fflush(stdout);
         }
         break;
      }
      case USB_DETACH_EVENT: 
      {
         audio_stream.DEV_HANDLE = NULL;
         audio_stream.INTF_HANDLE = NULL;
         audio_stream.DEV_STATE = USB_DEVICE_DETACHED;
         printf("----- Audio stream interface: detach event-----\n");
         fflush(stdout);
#ifdef BSP_BUTTON2
         play = FALSE;
#else
         play = TRUE;
#endif
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
         DisableCmtInterrupt();
#elif (defined BSP_TWRMCF51JF)
         DisableFTM0Interrupt();
#else
         _audio_timer_mask_int(BSP_LAST_TIMER);     
#endif
         break;
      }
      default:
         printf("Audio device: unknown data event\n");
         fflush(stdout);
         break;
   } /* EndSwitch */
} /* Endbody */
/* EOF */
