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
* $FileName: phd_com_model.h$
* $Version : 3.8.3.2$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief The file contains Macro's and functions needed by the phdc 
*        com model layer
*
*****************************************************************************/


#ifndef _PHD_COM_MODEL_H
#define _PHD_COM_MODEL_H

#include "devapi.h"
#include "ieee11073_phd_types.h"
#include "ieee11073_nom_codes.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define SEND_DATA_QOS                                   (0x88)
/* Agent states */
#define  PHD_AG_STATE_DISCONNECTED                      0x00
#define  PHD_AG_STATE_CON_UNASSOCIATED                  0x11
#define  PHD_AG_STATE_CON_ASSOCIATING                   0x12
#define  PHD_AG_STATE_CON_ASSOC_CFG_SENDING_CONFIG      0x73
#define  PHD_AG_STATE_CON_ASSOC_CFG_WAITING_APPROVAL    0x74
#define  PHD_AG_STATE_CON_ASSOC_OPERATING               0x35
#define  PHD_AG_STATE_CON_DISASSOCIATING                0x16

/* Agent event */
#define  PHD_AG_EVT_TRANSPORT_DISCONNECTED       0x00
#define  PHD_AG_EVT_TRANSPORT_CONNECTED          0x01

/* apdu recieved events */
#define  PHD_AG_EVT_ASSOC_REQ_RECIVED            0x02
#define  PHD_AG_EVT_ASSOC_RES_RECIVED            0x03
#define  PHD_AG_EVT_ASSOC_REL_REQ_RECIVED        0x04
#define  PHD_AG_EVT_ASSOC_REL_RES_RECIVED        0x05
#define  PHD_AG_EVT_ASSOC_ABRT_RECIVED           0x06
#define  PHD_AG_EVT_PRESENTATION_RECIVED         0x07

/* transport send/recieve event */
#define  PHD_AG_EVT_TRANSPORT_APDU_RECIEVED      0x80
#define  PHD_AG_EVT_TRANSPORT_SENT_COMPLETED     0x81

#define  AG_MAX_STATES                      7
#define  AG_MAX_EVENTS                      0x08


#define  AG_PHD_STATE_MASK                  0x0f


/* Events sent to application layer */
#define APP_PHD_UNINITIALISED               0
#define APP_PHD_INITIALISED                 1
#define USB_PHD_CONNECTED_TO_HOST           2
#define USB_PHD_DISCONNECTED_FROM_HOST      3
#define USB_PHD_MEASUREMENT_SENT            4
#define USB_PHD_ERROR                       5

#define ASSOC_REQ_SIZE                      54
#define CNFG_EVT_RPT_SIZE                   166
#define REL_REQ_SIZE                        6
#define REL_RES_SIZE                        6
#define DIM_GET_RSP_SIZE                    113
#define DIM_DATA_TX_SIZE                    94

#define UPPER_BYTE_SHIFT                    8
#define LOW_NIBBLE_MASK                     0x0f 
#define LOW_BYTE_MASK                       0xff

/* APDU Header Size */
#define APDU_HEADER_SIZE                    4

/*****************************************************************************
 * Types
 *****************************************************************************/

/* callback function pointer structure for Application to handle events */
typedef void(_CODE_PTR_ PHD_STATE_MC_FUNC)(uint_32, APDU*);

/* callback function pointer structure for Application to handle events */
typedef void(_CODE_PTR_ PHD_CALLBACK)(uint_32 ,uint_8);

/* structure for the measurements that are changing */
typedef struct _phd_measurement 
{    
    AbsoluteTime msr_time;
    uint_16 weight[2];
    uint_16 bmi[2];
}PHD_MEASUREMENT, *PHD_MEASUREMENT_PTR;

typedef struct _phdc_com_variable_struct
{
    /* buffer used for get attributes response and measurements */
    uint_8 phd_buff[DIM_GET_RSP_SIZE];
    /* variables to keep track of the phd_buffer usage */                                       
    boolean phd_buff_being_used;
    boolean sent_resp_get_attr; 
    uint_8 phd_com_state; 
    PHD_CALLBACK phdc_com_callback;
    uint_8_ptr com_rx_buff_ptr;
}PHDC_COM_GLOBAL_VARIABLE_STRUCT, _PTR_ PTR_PHDC_COM_GLOBAL_VARIABLE_STRUCT;

/*****************************************************************************
 * Global variables
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern uint_8 PHD_Transport_Init(uint_32 *handle, PHD_CALLBACK phd_callback);   
extern void PHD_Connect_to_Manager(uint_32 handle);
extern void PHD_Disconnect_from_Manager(uint_32 handle);
extern void PHD_Send_Measurements_to_Manager(uint_32 handle, PHD_MEASUREMENT_PTR msr);

#endif
