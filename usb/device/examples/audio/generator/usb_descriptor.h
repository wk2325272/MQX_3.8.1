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
 * @file usb_descriptor.h
 *
 * @author
 *
 * @version
 *
 * @date 
 *
 * @brief The file is a header file for USB Descriptors required for Mouse
 *        Application
 *****************************************************************************/

#ifndef _USB_DESCRIPTOR_H
#define _USB_DESCRIPTOR_H

/******************************************************************************
 * Includes
 *****************************************************************************/
#include "usb_class.h"
#include "usb_audio.h"

/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
 
#define BCD_USB_VERSION                  (0x0200)
 
#define REMOTE_WAKEUP_SHIFT              (5)
#define REMOTE_WAKEUP_SUPPORT            (TRUE)

/* Various descriptor sizes */
#define DEVICE_DESCRIPTOR_SIZE            (18)
#define CONFIG_DESC_SIZE                  (0x6D) 
#define DEVICE_QUALIFIER_DESCRIPTOR_SIZE  (10)
#define REPORT_DESC_SIZE                  (50)
#define CONFIG_ONLY_DESC_SIZE             (9)
#define IFACE_ONLY_DESC_SIZE              (9)
#define AUDIO_ONLY_DESC_SIZE              (9)
#define ENDP_ONLY_DESC_SIZE               (9)
#define HEADER_ONLY_DESC_SIZE             (9)
#define INPUT_TERMINAL_ONLY_DESC_SIZE     (12)
#define OUTPUT_TERMINAL_ONLY_DESC_SIZE    (9)
#define FEATURE_UNIT_ONLY_DESC_SIZE       (9)
#define AUDIO_STREAMING_IFACE_DESC_SIZE   (7)
#define AUDIO_STREAMING_ENDP_DESC_SIZE    (7)
#define AUDIO_STREAMING_TYPE_I_DESC_SIZE  (11)

#define USB_DEVICE_CLASS_AUDIO            (0x01)
#define USB_SUBCLASS_AUDIOCONTROL         (0x01)
#define USB_SUBCLASS_AUDIOSTREAM          (0x02)

#define AUDIO_INTERFACE_DESCRIPTOR_TYPE   (0x24)

#define AUDIO_CONTROL_HEADER              (0x01)
#define AUDIO_CONTROL_INPUT_TERMINAL      (0x02)
#define AUDIO_CONTROL_OUTPUT_TERMINAL     (0x03)
#define AUDIO_CONTROL_FEATURE_UNIT        (0x06)
#define AUDIO_STREAMING_GENERAL           (0x01)
#define AUDIO_STREAMING_FORMAT_TYPE       (0x02)
#define AUDIO_FORMAT_TYPE_I               (0x01)
#define AUDIO_ENDPOINT_GENERAL            (0x01)

/* Max descriptors provided by the Application */
#define USB_MAX_STD_DESCRIPTORS               (8)
#define USB_MAX_CLASS_SPECIFIC_DESCRIPTORS    (2)
/* Max configuration supported by the Application */
#define USB_MAX_CONFIG_SUPPORTED          (1)

/* Max string descriptors supported by the Application */
#define USB_MAX_STRING_DESCRIPTORS        (4)

/* Max language codes supported by the USB */
#define USB_MAX_LANGUAGES_SUPPORTED       (1)


#define AUDIO_UNIT_COUNT                  (3)
#define AUDIO_DESC_ENDPOINT_COUNT         (1)
#define AUDIO_ENDPOINT                    (1)


#define AUDIO_ENDPOINT_PACKET_SIZE (64)

/* string descriptors sizes */
#define USB_STR_DESC_SIZE (2)
#define USB_STR_0_SIZE  (2)
#define USB_STR_1_SIZE  (56)
#define USB_STR_2_SIZE  (28)
#define USB_STR_n_SIZE  (32)

/* descriptors codes */
#define USB_DEVICE_DESCRIPTOR     (1)
#define USB_CONFIG_DESCRIPTOR     (2)
#define USB_STRING_DESCRIPTOR     (3)
#define USB_IFACE_DESCRIPTOR      (4)
#define USB_ENDPOINT_DESCRIPTOR   (5)
#define USB_DEVQUAL_DESCRIPTOR    (6)
#define USB_AUDIO_DESCRIPTOR        (0x25)
#define USB_REPORT_DESCRIPTOR     (0x22)

#define USB_MAX_SUPPORTED_INTERFACES     (1)


/******************************************************************************
 * Types
 *****************************************************************************/


/******************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint_8 USB_Desc_Get_Descriptor(
     uint_32 handle, 
     uint_8 type,
     uint_8 str_num, 
     uint_16 index,
     uint_8_ptr *descriptor,
     uint_32 *size
);

extern uint_8 USB_Desc_Get_Interface(uint_32 handle, 
                              uint_8 interface, 
                              uint_8_ptr alt_interface
);


extern uint_8 USB_Desc_Set_Interface(
    uint_32 handle, 
    uint_8 interface, 
    uint_8 alt_interface
);

extern boolean USB_Desc_Valid_Configation(
    uint_32 handle,
    uint_16 config_val);

extern boolean USB_Desc_Valid_Interface(
    uint_32 handle,
    uint_8 interface);

extern boolean USB_Desc_Remote_Wakeup(uint_32 handle);

extern USB_ENDPOINTS* USB_Desc_Get_Endpoints(uint_32 handle);
extern USB_AUDIO_UNITS* USB_Desc_Get_Entities(uint_32 handle);
extern uint_8 USB_Desc_Set_Copy_Protect(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Copy_Protect(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Mute(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Mute(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Volume(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Bass(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Mid(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Treble(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Graphic_Equalizer(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Automatic_Gain(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Automatic_Gain(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Delay(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Bass_Boost(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Bass_Boost(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Loudness(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Loudness(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Min_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Max_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Res_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Min_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Max_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Res_Sampling_Frequency(uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Set_Cur_Pitch (uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
extern uint_8 USB_Desc_Get_Cur_Pitch (uint_32 handle,uint_8 interface,uint_8_ptr *coding_data);
uint_8 USB_Desc_Get_feature(uint_32 handle,int_32 cmd, uint_8 in_data,uint_8_ptr * in_buf);
uint_8 USB_Desc_Set_feature(uint_32 handle,int_32 cmd, uint_8 in_data,uint_8_ptr * in_buf);


#endif
