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
* $FileName: phd_com_model.c$
* $Version : 3.8.3.2$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief This file packetises the data to be sent according to the IEEE_11073 
*        protocol.    
*****************************************************************************/
 
/******************************************************************************
 * Includes
 *****************************************************************************/
#include "phd_com_model.h"
#include "weighscale.h"
#include "usb_phdc.h"
#include "usb_descriptor.h"

/*****************************************************************************
 * Constant and Macro's 
 *****************************************************************************/
extern USB_ENDPOINTS usb_desc_ep;
extern DESC_CALLBACK_FUNCTIONS_STRUCT  desc_callback;

/*****************************************************************************
 * Global Functions Prototypes
 *****************************************************************************/
void PHD_Callback(uint_8 event_type,void* val,pointer arg);
/****************************************************************************
 * Global Variables
 ****************************************************************************/
/* Add all the variables needed for phd_com_model.c to this structure */
PHDC_COM_GLOBAL_VARIABLE_STRUCT g_phdc_com;
/*****************************************************************************
 * Local Types - None
 *****************************************************************************/

/*****************************************************************************
 * Local Functions Prototypes
 *****************************************************************************/
static void PHD_Assoc_Response_Handler(uint_32 handle, APDU* val); 
static void PHD_Unhandled_Request(uint_32 handle, APDU* val); 
static void PHD_Disconnect_Handler(uint_32 handle, APDU* val); 
static void PHD_Connect_Handler(uint_32 handle, APDU* val); 
static void PHD_ABRT_Request_Handler(uint_32 handle, APDU* val);
static void PHD_Assoc_RelRes_Handler(uint_32 handle, APDU* val);
static void PHD_Assoc_RelReq_Handler(uint_32 handle, APDU* val);
static void PHD_OPN_STATE_PRST_APDU_Handler(uint_32 handle, 
    APDU* val);
/*****************************************************************************
 * Local Variables 
 *****************************************************************************/ 
/* association request to send */ 
uint_8 PHD_WSL_ASSOC_REQ[ASSOC_REQ_SIZE] = {   
0xE2, 0x00,                         /* APDU CHOICE Type (AarqApdu) */
0x00, 0x32,                         /* CHOICE.length = 50 */
0x80, 0x00, 0x00, 0x00,             /* assoc-version */
0x00, 0x01, 0x00, 0x2A,             /* data-proto-list.count=1 | length=42*/
0x50, 0x79,                         /* data-proto-id = 20601 */
0x00, 0x26,                         /* data-proto-info length = 38 */
0x80, 0x00, 0x00, 0x00,             /* protocolVersion */
0x80, 0x00,                         /* encoding rules = MDER or PER */ 
0x80, 0x00, 0x00, 0x00,             /* nomenclatureVersion */
0x00, 0x00, 0x00, 0x00,             /* functionalUnits | 
                                      no test association capabilities */
0x00, 0x80, 0x00, 0x00,             /* systemType = sys-type-agent */
0x00, 0x08,                         /* system-id length = 8 and value 
    ,                                 (manufacturer- and device- specific) */
0x4C, 0x4E, 0x49, 0x41, 0x47, 0x45, 0x4E, 0x54,  
0x05, 0xDC,                         /* dev-config-id | extended configuration*/ 
0x00, 0x01,                         /* data-req-mode-flags */
0x01, 0x00,                         /* data-req-init-agent-count, 
                                      data-req-init-manager-count */
0x00, 0x00, 0x00, 0x00              /* Atribute list */
};

/* configuration event report */ 
uint_8 PHD_WSL_CNFG_EVT_RPT[CNFG_EVT_RPT_SIZE] = {   
0xE7, 0x00,                           /* APDU CHOICE Type (PrstApdu) */
0x00, 0xA2,                           /* CHOICE.length = 162 */
0x00, 0xA0,                           /* OCTET STRING.length = 160 */
0x00, 0x05,                           /*  invoke-id = 0x1235 (start of DataApdu
                                          . MDER encoded.) */ 
0x01, 0x01,                           /*  CHOICE(Remote Operation Invoke | 
                                          Confirmed Event Report) */
0x00, 0x9A,                           /*  CHOICE.length = 154 */
0x00, 0x00,                           /*  obj-handle = 0 (MDS object) */
0xFF, 0xFF, 0xFF, 0xFF,               /*  event-time = 0xFFFFFFFF    */
0x0D, 0x1C,                           /*  event-type = MDC_NOTI_CONFIG */
0x00, 0x90,                           /*  event-info.length = 144 (start of 
                                          ConfigReport) */
0x05, 0xDC,                           /*  config-report-id */ 
0x00, 0x03,                           /*  config-obj-list.count = 3 Measurement
                                          objects will be announced */
0x00, 0x8A,                           /*  config-obj-list.length = 138 */
0x00, 0x06,                           /*  obj-class = MDC_MOC_VMO_METRIC_NU */
0x00, 0x01,                           /*  obj-handle = 1 (.. 1st Measurement is
                                          body weight) */
0x00, 0x04,                           /*  attributes.count = 4  */
0x00, 0x24,                           /*  attributes.length = 36  */
0x09, 0x2F,                           /*  attribute-id = MDC_ATTR_ID_TYPE */
0x00, 0x04,                           /*  attribute-value.length = 4      */
0x00, 0x02, 0xE1, 0x40,               /*  0xE1 0x40 MDC_PART_SCADA |  
                                          MDC_MASS_BODY_ACTUAL */ 
0x0A, 0x46,                           /*  attribute-id = 
                                          MDC_ATTR_METRIC_SPEC_SMALL      */
0x00, 0x02,                           /*  attribute-value.length = 2  */
0xF0, 0x40,                           /*  intermittent, stored data, upd & msmt
                                          aperiodic, agent init, measured */
0x09, 0x96,                           /*  attribute-id = MDC_ATTR_UNIT_CODE */
0x00, 0x02,                           /*  attribute-value.length = 2 */
0x06, 0xC3,                           /*  MDC_DIM_KILO_G             */
0x0A, 0x55,                           /*  attribute-id = 
                                          MDC_ATTR_ATTRIBUTE_VAL_MAP  */
0x00, 0x0C,                           /*  attribute-value.length = 12     */
0x00, 0x02,                           /*  AttrValMap.count = 2         */
0x00, 0x08,                           /*  AttrValMap.length = 8          */
0x0A, 0x56, 0x00, 0x04,               /*  MDC_ATTR_NU_VAL_OBS_SIMP | 
                                          value length = 4   */
0x09, 0x90, 0x00, 0x08,               /*  MDC_ATTR_TIME_STAMP_ABS | 
                                          value length = 8     */
0x00, 0x06,                           /*  obj-class = MDC_MOC_VMO_METRIC_NU */
0x00, 0x02,                           /*  obj-handle = 2 (..2nd Measurement 
                                          is body height) */
0x00, 0x04,                           /*  attributes.count = 4   */
0x00, 0x24,                           /*  attributes.length = 36   */
0x09, 0x2F,                           /*  attribute-id = MDC_ATTR_ID_TYPE */
0x00, 0x04,                           /*  attribute-value.length = 4  */
0x00, 0x02, 0xE1, 0x44,               /*  MDC_PART_SCADA | 
                                          MDC_LEN_BODY_ACTUAL */
0x0A, 0x46,                           /*  attribute-id = 
                                          MDC_ATTR_METRIC_SPEC_SMALL   */
0x00, 0x02,                           /*  attribute-value.length = 2   */
0xF0, 0x48,                           /*  intermittent, stored data, upd & msmt
                                          aperiodic, agent init, manual */
0x09, 0x96,                           /*  attribute-id = MDC_ATTR_UNIT_CODE*/
0x00, 0x02,                           /*  attribute-value.length = 2*/
0x05, 0x11,                           /*  MDC_DIM_CENTI_M            */
0x0A, 0x55,                           /*  attribute-id = 
                                          MDC_ATTR_ATTRIBUTE_VAL_MAP */
0x00, 0x0C,                           /*  attribute-value.length = 12 */
0x00, 0x02,                           /*  AttrValMap.count = 2*/
0x00, 0x08,                           /*  AttrValMap.length = 8*/
0x0A, 0x56, 0x00, 0x04,               /*  MDC_ATTR_NU_VAL_OBS_SIMP, 4 */
0x09, 0x90, 0x00, 0x08,               /*  MDC_ATTR_TIME_STAMP_ABS, 8 */
0x00, 0x06,                           /*  obj-class = MDC_MOC_VMO_METRIC_NU*/
0x00, 0x03,                           /*  obj-handle = 3 (..3rd Measurement 
                                          is body mass index) */
0x00, 0x05,                           /*  attributes.count = 5*/
0x00, 0x2A,                           /*  attributes.length = 42*/
0x09, 0x2F,                           /*  attribute-id = MDC_ATTR_ID_TYPE */
0x00, 0x04,                           /*  attribute-value.length = 4 */
0x00, 0x02, 0xE1, 0x50,               /*  MDC_PART_SCADA | 
                                          MDC_RATIO_MASS_BODY_LEN_SQ    */
0x0A, 0x46,                           /*  attribute-id = 
                                          MDC_ATTR_METRIC_SPEC_SMALL       */
0x00, 0x02,                           /*  attribute-value.length = 2    */
0xF0, 0x42,                           /*  intermittent, stored data, upd & 
                                      msmt aperiodic, agent init, calculated */
0x09, 0x96,                           /*  attribute-id = MDC_ATTR_UNIT_CODE*/
0x00, 0x02,                           /*  attribute-value.length = 2       */
0x07, 0xA0,                           /*  MDC_DIM_KG_PER_M_SQ               */
0x0A, 0x47,                           /*  attribute-id = 
                                          MDC_ATTR_SOURCE_HANDLE_REF        */
0x00, 0x02,                           /*  attribute-value.length = 2 */
0x00, 0x01,                           /*  reference handle = 1         */
0x0A, 0x55,                           /*  attribute-id = 
                                          MDC_ATTR_ATTRIBUTE_VAL_MAP*/
0x00, 0x0C,                           /*  attribute-value.length = 12  */
0x00, 0x02,                           /*  AttrValMap.count = 2          */
0x00, 0x08,                           /*  AttrValMap.length = 8         */
0x0A, 0x56, 0x00, 0x04,               /*  MDC_ATTR_NU_VAL_OBS_SIMP, 4   */
0x09, 0x90, 0x00, 0x08                /*  MDC_ATTR_TIME_STAMP_ABS, 8   */
};       

/* release request to send */
uint_8 PHD_WSL_REL_REQ[REL_REQ_SIZE] = {   
0xE4, 0x00,                           /* APDU CHOICE Type (RlrqApdu) */
0x00, 0x02,                           /* CHOICE.length = 2 */
0x00, 0x00                            /* reason = normal */
};

/* release response to the host */
uint_8 PHD_WSL_REL_RES[REL_RES_SIZE] = {   
0xE5, 0x00,                           /* APDU CHOICE Type (RlrsApdu) */
0x00, 0x02,                           /* CHOICE.length = 2 */
0x00, 0x00                            /* reason = normal */
};

/* response to get attributes command */
uint_8 PHD_WSL_DIM_GET_RSP[DIM_GET_RSP_SIZE] = {   
0xE7, 0x00,                      /* APDU CHOICE Type (PrstApdu) */
0x00, 0x6D,                      /* CHOICE.length = 109 */
0x00, 0x6B,                      /* OCTET STRING.length = 107 */
0x00, 0x05,                      /* invoke-id =0x0002 (mirrored from request)*/
0x02, 0x03,                      /* CHOICE (Remote Operation Response | Get)*/
0x00, 0x65,                      /* CHOICE.length = 101                     */
0x00, 0x00,                      /* handle = 0 (MDS object)                */
0x00, 0x06,                      /* attribute-list.count = 6               */
0x00, 0x5F,                      /* attribute-list.length = 95              */
0x0A, 0x5A,                      /* attribute id=MDC_ATTR_SYS_TYPE_SPEC_LIST */         
0x00, 0x08,                      /* attribute-value.length = 8              */
0x00, 0x01,                      /* TypeVerList count = 1                   */
0x00, 0x04,                      /* TypeVerList length = 4                  */
0x10, 0x0F,                      /* type = MDC_DEV_SPEC_PROFILE_SCALE  */
0x00, 0x01,                      /* version=ver 1 of the specialization  */
0x09, 0x28,                      /* attribute-id = MDC_ATTR_ID_MODEL    */
0x00, 0x19,                      /* attribute-value.length = 25        */
0x00, 0x0A, 0x46, 0x72,          /* string length = 10 | TheCompany  */
0x65, 0x65, 0x73, 0x63,                                                     
0x61, 0x6C, 0x65, 0x20,
0x00, 0x0B, 0x4D, 0x65,          /* string length = 11 | TheScaleABC\0    */
0x64, 0x69, 0x63, 0x61,
0x6C, 0x20, 

0x4D, 0x51, 0x58, /* System Model - medical mqx */  



0x09, 0x84,                      /* attribute-id = MDC_ATTR_SYS_ID        */
0x00, 0x0A,                      /* attribute-value.length = 10            */
0x00, 0x08, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
                                 /* octet string length = 8 | EUI-64  */
0x0a, 0x44,                      /* attribute-id = MDC_ATTR_DEV_CONFIG_ID   */
0x00, 0x02,                      /* attribute-value.length = 2             */
0x40, 0x00,                      /* dev-config-id = 16384 
                                    (extended-config-start)*/
0x09, 0x2D,                      /* attribute-id = MDC_ATTR_ID_PROD_SPECN */
0x00, 0x12,                      /* attribute-value.length = 18           */
0x00, 0x01,                      /* ProductionSpec.count = 1              */
0x00, 0x0E,                      /* ProductionSpec.length = 14             */
0x00, 0x01,                      /* ProdSpecEntry.spec-type=1(serial-number)*/
0x00, 0x00,                      /* ProdSpecEntry.component-id = 0     */
0x00, 0x08, 0x44, 0x45,          /* string length = 8 | 
                                    prodSpecEntry.prod-spec = DE124567 */
0x31, 0x32, 0x34, 0x35,
0x36, 0x37,                                                            
0x09, 0x87,                      /* attribute-id =MDC_ATTR_TIME_ABS */
0x00, 0x08,                      /* attribute-value.length = 8  */
0x20, 0x07, 0x02, 0x01,          /* Absolute-Time-Stamp=2007-02-01T12:05:0000*/
0x12, 0x05, 0x00, 0x00                                                               
};  

/* measurements to send */
uint_8  PHD_WSL_DIM_DATA_TX[DIM_DATA_TX_SIZE] = {   
0xE7, 0x00, /*APDU CHOICE Type (PrstApdu)*/
0x00, 0x5A, /*CHOICE.length = 90*/
0x00, 0x58, /*OCTET STRING.length = 88*/
0x12, 0x36, /*invoke-id = 0x1236*/
0x01, 0x01, /*CHOICE(Remote Operation Invoke | Confirmed Event Report)*/
0x00, 0x52, /*CHOICE.length = 82*/
0x00, 0x00, /*obj-handle = 0 (MDS object)*/
0x00, 0x00, 0x00, 0x00, /*event-time = 0*/
0x0D, 0x1D, /*event-type = MDC_NOTI_SCAN_REPORT_FIXED*/
0x00, 0x48, /*event-info.length = 72*/
0xF0, 0x00, /*ScanReportInfoFixed.data-req-id = 0xF000*/
0x00, 0x00, /*ScanReportInfoFixed.scan-report-no = 0*/
0x00, 0x04, /*ScanReportInfoFixed.obs-scan-fixed.count = 4*/
0x00, 0x40, /*ScanReportInfoFixed.obs-scan-fixed.length = 64*/
0x00, 0x01, /*ScanReportInfoFixed.obs-scan-fixed.value[0].obj-handle = 1*/
0x00, 0x0C, /*ScanReportInfoFixed.obs-scan-fixed.value[0]. obs-val-data.length 
              = 12*/
0xFF, 0x00, 0x02, 0xFA, /*Simple-Nu-Observed-Value = 76.2 (kg)*/
0x20, 0x07, 0x12, 0x06, /*Absolute-Time-Stamp = 2007-12-06T12:10:0000*/
0x12, 0x10, 0x00, 0x00,
0x00, 0x03, /* ScanReportInfoFixed.obs-scan-fixed.value[1].obj-handle = 3*/
0x00, 0x0C, /* ScanReportInfoFixed.obs-scan-fixed.value[1]. obs-val-data.length
               = 12*/
0xFF, 0x00, 0x00, 0xF3, /* Simple-Nu-Observed-Value = 24.3 (kg/m2)*/
0x20, 0x07, 0x12, 0x06, /* Absolute-Time-Stamp = 2007-12-06T12:10:0000*/
0x12, 0x10, 0x00, 0x00,
0x00, 0x01, /*ScanReportInfoFixed.obs-scan-fixed.value[0].obj-handle = 1*/
0x00, 0x0C, /*ScanReportInfoFixed.obs-scan-fixed.value[0]. obs-val-data.length 
            = 12*/
0xFF, 0x00, 0x02, 0xF8, /*Simple-Nu-Observed-Value = 76.0 (kg)*/
0x20, 0x07, 0x12, 0x06, /*Absolute-Time-Stamp = 2007-12-06T20:05:0000*/
0x20, 0x05, 0x00, 0x00,
0x00, 0x03, /*ScanReportInfoFixed.obs-scan-fixed.value[1].obj-handle = 3*/
0x00, 0x0C, /*ScanReportInfoFixed.obs-scan-fixed.value[1]. obs-val-data.length 
                = 12*/
0xFF, 0x00, 0x00, 0xF2, /*Simple-Nu-Observed-Value = 24.2 (kg/m2)*/
0x20, 0x07, 0x12, 0x06, /*Absolute-Time-Stamp = 2007-12-06T20:05:0000*/
0x20, 0x05, 0x00, 0x00
};
PHD_STATE_MC_FUNC phd_state_mc_func[AG_MAX_STATES][AG_MAX_EVENTS] = 
{
   /* PHD_AG_STATE_DISCONNECTED */  
   PHD_Disconnect_Handler,PHD_Connect_Handler,NULL,NULL,NULL,NULL,NULL,NULL,
   /* PHD_AG_STATE_CON_UNASSOCIATED */
   PHD_Disconnect_Handler,PHD_Connect_Handler,PHD_Unhandled_Request,NULL,NULL,
   NULL,PHD_ABRT_Request_Handler,NULL,
   /* PHD_AG_STATE_CON_ASSOCIATING */
   PHD_Disconnect_Handler,PHD_Connect_Handler,PHD_Unhandled_Request,
   PHD_Assoc_Response_Handler,NULL,NULL,PHD_ABRT_Request_Handler,PHD_OPN_STATE_PRST_APDU_Handler,
   /* PHD_AG_STATE_CON_ASSOC_CFG_SENDING_CONFIG */
   PHD_Disconnect_Handler,PHD_Connect_Handler,PHD_Unhandled_Request,NULL,
   PHD_Assoc_RelReq_Handler,NULL,PHD_ABRT_Request_Handler,PHD_OPN_STATE_PRST_APDU_Handler,
   /* PHD_AG_STATE_CON_ASSOC_CFG_WAITING_APPROVAL */
   PHD_Disconnect_Handler,PHD_Connect_Handler,PHD_Unhandled_Request,NULL,
   PHD_Assoc_RelReq_Handler,NULL,PHD_ABRT_Request_Handler,
   PHD_OPN_STATE_PRST_APDU_Handler,
   /* PHD_AG_STATE_CON_ASSOC_OPERATING */
   PHD_Disconnect_Handler,PHD_Connect_Handler,PHD_Unhandled_Request,NULL,
   PHD_Assoc_RelReq_Handler,NULL,PHD_ABRT_Request_Handler,
   PHD_OPN_STATE_PRST_APDU_Handler,
   /* PHD_AG_STATE_CON_DISASSOCIATING */
   PHD_Disconnect_Handler,PHD_Connect_Handler,PHD_Unhandled_Request,NULL,
   PHD_Assoc_RelReq_Handler,PHD_Assoc_RelRes_Handler,PHD_ABRT_Request_Handler,NULL
};

/*****************************************************************************
 * Local Functions
 *****************************************************************************/
/******************************************************************************
 * 
 *    @name        PHD_OPN_STATE_PRST_APDU_Handler
 *    
 *    @brief       This function handles the request/data sent by the host when
 *                 the device is in operating state   
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function parses the data recieved and checks whether it is a response 
 * to the attributes sent or the response to the measurement data
 *****************************************************************************/
DATA_apdu *p_data_pdu;
static void PHD_OPN_STATE_PRST_APDU_Handler(uint_32 handle,APDU*val)
{ 
    if((HOST_TO_BE_SHORT(val->choice) ==  PRST_CHOSEN)&&
        (
         (g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_OPERATING) ||
         (g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOCIATING)     ||
         (g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_CFG_SENDING_CONFIG) ||
         (g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_CFG_WAITING_APPROVAL)
        )
      )
    {    
        /* get the APDU recieved starting from invoke id */
    	DATA_apdu *p_data_pdu = (DATA_apdu *)&(val->u.prst.value);
        if(HOST_TO_BE_SHORT(p_data_pdu->choice.choice) == ROIV_CMIP_GET_CHOSEN) 
        {/* its the Get command */          
            if(p_data_pdu->choice.u.roiv_cmipGet.attribute_id_list.count == 0) 
            {   /* count 0 implies the whole MDS class */              
                uint_8 i=0;
                uint_16 invoke_id = HOST_TO_BE_SHORT(p_data_pdu->invoke_id);
                
                _int_disable(); 
                /* if g_phdc_com.phd_buff is already in use, return */               
                if(g_phdc_com.phd_buff_being_used == TRUE)
                {
                    _int_enable();
                    return;     
                }
                g_phdc_com.phd_buff_being_used = TRUE;         
                _int_enable();
                
                /* copy the get attribute response into the g_phdc_com.phd_buff */
                for(i = 0; i < DIM_GET_RSP_SIZE; i++) 
                { 
                    g_phdc_com.phd_buff[i] = PHD_WSL_DIM_GET_RSP[i];
                }
                /* get the invoke id from the get attribute request sent 
                   by manager */ 
                g_phdc_com.phd_buff[6] = (uint_8)((invoke_id >> UPPER_BYTE_SHIFT) 
                    & LOW_BYTE_MASK);
                g_phdc_com.phd_buff[7]= (uint_8)((invoke_id) & LOW_BYTE_MASK);                
                /* Send Atributes to Manager */   
                (void)USB_Class_PHDC_Send_Data(handle, FALSE, 0,
                    SEND_DATA_QOS, (uint_8_ptr)g_phdc_com.phd_buff, DIM_GET_RSP_SIZE);
                g_phdc_com.sent_resp_get_attr=TRUE;                                                                          
            }
        } 
        else if(HOST_TO_BE_SHORT(p_data_pdu->choice.choice) == RORS_CMIP_CONFIRMED_EVENT_REPORT_CHOSEN)
        {
          	if(g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_CFG_WAITING_APPROVAL) 
          	{
				/* configuration accepted */
				ConfigReportRsp *p_rsp = (ConfigReportRsp *)p_data_pdu->choice.
					u.rors_cmipConfirmedEventReport.event_reply_info.value;
				p_rsp->config_report_id = HOST_TO_BE_SHORT(p_rsp->config_report_id);
				if((p_rsp->config_report_id == EXTENDED_CONFIG_START) && 
					(p_rsp->config_result == ACCEPTED_CONFIG )) 
				{/* if configuration accepted, enter operating state */
					g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOC_OPERATING;                   
				} 
				else 
				{/* configuration not accepted by the manager */  
					g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOC_CFG_SENDING_CONFIG;                                     
				}
          	} else /* confirmed report on completion */
	        {
	            /* if the recieved APDU is the response to the measurements sent */
	            g_phdc_com.phdc_com_callback(handle,USB_PHD_MEASUREMENT_SENT);
	        }  
        }
    }  
}

/******************************************************************************
 * 
 *    @name        PHD_Assoc_Response_Handler
 *    
 *    @brief       This function handles the assciation request response sent 
 *                 by the host                  
 *                  
 *    @param       handle
 *    @param       val    : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function parses the association request response to check whether the 
 * configuration was already known to the manager or not. In case the 
 * configuartion was not known, configuration event report is sent
 *****************************************************************************/
static void PHD_Assoc_Response_Handler(uint_32 handle, APDU* val) 
{
    if(( g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOCIATING) && 
        (HOST_TO_BE_SHORT(val->choice) ==  AARE_CHOSEN))
    {
        AARE_apdu *p_assoc_res = &(val->u.aare);
       
        if(HOST_TO_BE_SHORT(p_assoc_res->result) == ACCEPTED_UNKNOWN_CONFIG) 
        {/* if manager says the configuration is unknown, send configuration 
            event report */    
            g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOC_CFG_SENDING_CONFIG;
       
            /* send the configuration information */
            (void)USB_Class_PHDC_Send_Data(handle, FALSE,0,SEND_DATA_QOS,
                (uint_8_ptr)PHD_WSL_CNFG_EVT_RPT, (uint_32)CNFG_EVT_RPT_SIZE);           
            g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOC_CFG_WAITING_APPROVAL;
        }
        else 
        {   /* if the configuration is already known to the manager, enter 
               operating state */ 
            g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOC_OPERATING;          
        }
    }
}

/******************************************************************************
 * 
 *    @name        PHD_Unhandled_Request
 *    
 *    @brief       This function takes care of the unhandled request               
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function should take care of any request which is not supported or is 
 * illegal
 *****************************************************************************/ 
static void PHD_Unhandled_Request(uint_32 handle, APDU* val) 
{ 
    UNUSED_ARGUMENT(handle)
    UNUSED_ARGUMENT(val)
    /* user code to be added */  
}
/******************************************************************************
 * 
 *    @name        PHD_ABRT_Request_Handler
 *    
 *    @brief       This function handles abrt req            
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This handles the abort request sent by the manager
 *****************************************************************************/ 
static void PHD_ABRT_Request_Handler(uint_32 handle, APDU* val) 
{
    UNUSED_ARGUMENT(val)
    g_phdc_com.phd_com_state = PHD_AG_STATE_CON_UNASSOCIATED;
    g_phdc_com.phdc_com_callback(handle,USB_PHD_DISCONNECTED_FROM_HOST);
}

/******************************************************************************
 * 
 *    @name        PHD_Assoc_RelRes_Handler
 *    
 *    @brief       This function handles association release response            
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function handles the association release response
 *****************************************************************************/ 
static void PHD_Assoc_RelRes_Handler(uint_32 handle, APDU* val) 
{           
    UNUSED_ARGUMENT(val)
    g_phdc_com.phd_com_state = PHD_AG_STATE_CON_UNASSOCIATED;
    g_phdc_com.phdc_com_callback(handle,USB_PHD_DISCONNECTED_FROM_HOST);
}
/******************************************************************************
 * 
 *    @name        PHD_Assoc_RelReq_Handler
 *    
 *    @brief       This function handles association release request            
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function sends a response to the association release request
 *****************************************************************************/ 
static void PHD_Assoc_RelReq_Handler(uint_32 handle, APDU* val) 
{   
    UNUSED_ARGUMENT(val)
    /* send release response */    
    (void)USB_Class_PHDC_Send_Data(handle, FALSE, 0,SEND_DATA_QOS, 
        (uint_8_ptr)PHD_WSL_REL_RES, REL_RES_SIZE);
    g_phdc_com.phd_com_state = PHD_AG_STATE_CON_UNASSOCIATED;
    g_phdc_com.phdc_com_callback(handle,USB_PHD_DISCONNECTED_FROM_HOST);
}

/******************************************************************************
 * 
 *    @name        PHD_Disconnect_Handler
 *    
 *    @brief       This function handles disconnect request            
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * Sets the PHDC state to disconnected
 *****************************************************************************/ 
static void PHD_Disconnect_Handler(uint_32 handle, APDU* val)
{    
    UNUSED_ARGUMENT(handle)
    UNUSED_ARGUMENT(val)
    g_phdc_com.phd_com_state = PHD_AG_STATE_DISCONNECTED;
}

/******************************************************************************
 * 
 *    @name        PHD_Connect_Handler
 *    
 *    @brief       This function handles connect request            
 *                  
 *    @param       handle
 *    @param       val           : the APDU received    
 * 
 *    @return      None
 *
 *****************************************************************************
 * Sets the PHDC state to unassciated if it was disconnected
 *****************************************************************************/  
static void PHD_Connect_Handler(uint_32 handle, APDU* val)
{
    UNUSED_ARGUMENT(val)
    
    if(g_phdc_com.phd_com_state == PHD_AG_STATE_DISCONNECTED) 
    {
        g_phdc_com.phd_com_state = PHD_AG_STATE_CON_UNASSOCIATED;           
        g_phdc_com.phdc_com_callback(handle,APP_PHD_INITIALISED);                      
    }     
} 

/******************************************************************************
 * 
 *    @name        PHD_Callback
 *    
 *    @brief       This function handles the callback            
 *                  
 *    @param       handle
 *    @param       event_tyoe    : type of the event 
 *    @param       val           : val
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function is called from the class layer and handles the events (reset, 
 * enumeration complete, send/recv complete)
 *****************************************************************************/
void PHD_Callback(uint_8 event_type,void* val,pointer arg) 
{
    uint_8 trans_event = 0xff;
    
    uint_32 handle = *((uint_32 *)arg);
    switch (event_type) 
    {
        case USB_APP_BUS_RESET:
            /* on reset, transport is disconnected */
            trans_event = PHD_AG_EVT_TRANSPORT_DISCONNECTED;       
            break;
        case USB_APP_ENUM_COMPLETE:
            /* when enumeration is complete, transport is connected */
            trans_event = PHD_AG_EVT_TRANSPORT_CONNECTED; 
            break;
        case USB_APP_GET_DATA_BUFF:
            /* called by lower layer to get recv buffer */ 
            {
            	PTR_USB_CLASS_PHDC_RX_BUFF rx_buff = (PTR_USB_CLASS_PHDC_RX_BUFF)val; 
                if(g_phdc_com.com_rx_buff_ptr == NULL) 
                {/* save the buffer from lower layer */
                    g_phdc_com.com_rx_buff_ptr = rx_buff->in_buff;  
                }
                
                rx_buff->out_size = HOST_TO_BE_SHORT(*((uint_16_ptr)rx_buff->in_buff + 1)) +
                    APDU_HEADER_SIZE;               
                rx_buff->out_buff = (uint_8_ptr)(&g_phdc_com.phd_buff);        
            }
            break;
        case USB_APP_DATA_RECEIVED:
            /* receive data complete */
            trans_event = PHD_AG_EVT_TRANSPORT_APDU_RECIEVED;
            /* Queue another recv on USB BUS */
            break;
        case USB_APP_SEND_COMPLETE:
            /* send data complete */          
            g_phdc_com.phd_buff_being_used = FALSE;/* release the g_phdc_com.phd_buff */
          
            if(g_phdc_com.sent_resp_get_attr==TRUE) 
            {/* attributes sent, ready to send measurements */           
                g_phdc_com.phdc_com_callback(handle,USB_PHD_CONNECTED_TO_HOST);               
                g_phdc_com.sent_resp_get_attr=FALSE;
            }
            
            
            if (g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_CFG_SENDING_CONFIG)
			{                
				g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOC_CFG_WAITING_APPROVAL;
			}
            break;
      
        default:
            break;      
    }
    
    if(trans_event != 0xff) 
    {      
        uint_8 com_state;   
        /* get the recieved APDU */     
        USB_APP_EVENT_DATA_RECIEVED * p_recv = (USB_APP_EVENT_DATA_RECIEVED *)val;
        APDU *p_apdu = (APDU *)(p_recv->buffer_ptr);        
        
        if(trans_event == PHD_AG_EVT_TRANSPORT_APDU_RECIEVED) 
        {/* pdu recieved from transport */
            trans_event = (uint_8)((HOST_TO_BE_SHORT(p_apdu->choice) >> UPPER_BYTE_SHIFT) 
                & LOW_NIBBLE_MASK);
        } 

        com_state = (uint_8)(g_phdc_com.phd_com_state & AG_PHD_STATE_MASK);
        
        if(phd_state_mc_func[com_state][trans_event] != NULL) 
        {   /* incase valid event then call the function */  
            (void)phd_state_mc_func[com_state][trans_event]
                (handle,p_apdu);
        } 
        else 
        {          
            /* send error to app layer */
            g_phdc_com.phdc_com_callback(handle,USB_PHD_ERROR);
        }
    }
    if(event_type == USB_APP_DATA_RECEIVED)
    {
        (void)USB_Class_PHDC_Recv_Data(handle, PHDC_BULK_OUT_QOS, 
            g_phdc_com.com_rx_buff_ptr, PHDC_BULK_OUT_EP_SIZE);     
    }
            
    
    return;
}
 
/*****************************************************************************
 * Global Functions
 *****************************************************************************/ 
/******************************************************************************
 * 
 *    @name        PHD_Transport_Init
 *    
 *    @brief       This function initializes the PHDC class layer and layers 
 *                 below            
 *                  
 *    @param       *handle
 *    @param       phd_callback  : callback function to register
 * 
 *    @return      None
 *
 *****************************************************************************
 * Called by the application layer to initialize all the layers below it
 *****************************************************************************/    
uint_8 PHD_Transport_Init(uint_32 *handle,PHD_CALLBACK phd_callback)
{    
    /* Initialize Global Variable Structure */
    PHDC_CONFIG_STRUCT phdc_config;
    phdc_config.phdc_callback.callback =  PHD_Callback;
    phdc_config.phdc_callback.arg = (void *)handle;
    phdc_config.vendor_callback.callback = NULL;
    phdc_config.vendor_callback.arg = NULL;
    phdc_config.desc_callback_ptr = &desc_callback;
    phdc_config.info = &usb_desc_ep;
    
    USB_mem_zero(&g_phdc_com, sizeof(PHDC_COM_GLOBAL_VARIABLE_STRUCT));
    g_phdc_com.phd_com_state = PHD_AG_STATE_DISCONNECTED; 
    g_phdc_com.phdc_com_callback = phd_callback;    
    *handle = USB_Class_PHDC_Init(&phdc_config);
    g_phdc_com.com_rx_buff_ptr = NULL; 
    return USB_OK;
}

/******************************************************************************
 * 
 *    @name        PHD_Connect_to_Manager
 *    
 *    @brief       This function sends the association request to the host   
 *                  
 *    @param       handle 
 * 
 *    @return      None
 *
 *****************************************************************************
 * This function is called by the application when enumeration is complete to 
 * send the association request
 *****************************************************************************/    
void PHD_Connect_to_Manager(uint_32 handle) 
{
    if(g_phdc_com.phd_com_state == PHD_AG_STATE_CON_UNASSOCIATED) 
    {                                 
        /* Send Assoc request to Manager */   
        (void)USB_Class_PHDC_Send_Data(handle, FALSE, 0,SEND_DATA_QOS, 
            (uint_8_ptr)PHD_WSL_ASSOC_REQ, ASSOC_REQ_SIZE);        
        g_phdc_com.phd_com_state = PHD_AG_STATE_CON_ASSOCIATING;        
    }     
}

/******************************************************************************
 * 
 *    @name        PHD_Disconnect_from_Manager
 *    
 *    @brief       This function sends the association release request to the 
 *                 host   
 *                  
 *    @param       handle 
 * 
 *    @return      None
 *
 ****************************************************************************
 * Called by the application to send the assciation release request
 *****************************************************************************/  
void PHD_Disconnect_from_Manager(uint_32 handle) 
{
    if(g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_OPERATING) 
    {                  
        /* Send Assoc release to Manager */   
        (void)USB_Class_PHDC_Send_Data(handle, FALSE, 0,SEND_DATA_QOS, 
            (uint_8_ptr)PHD_WSL_REL_REQ, REL_REQ_SIZE);                                                              
        g_phdc_com.phd_com_state = PHD_AG_STATE_CON_DISASSOCIATING;        
    } 
    else if(g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOCIATING)
    {
        g_phdc_com.phd_com_state = PHD_AG_STATE_CON_UNASSOCIATED;        
    }     
}

/******************************************************************************
 * 
 *    @name        PHD_Send_Measurements_to_Manager
 *    
 *    @brief       This function sends measurements to the host
 *                  
 *    @param       handle
 *    @param       p_msr         : measurements changed by the application 
 * 
 *    @return      None
 *
 *****************************************************************************
 * Called by the appliaction to send the measurement data via event report
 *****************************************************************************/  
 uint_16 scan_report_no = 0;
void PHD_Send_Measurements_to_Manager
(
    uint_32 handle,
    PHD_MEASUREMENT_PTR p_msr
) 
{
    if(g_phdc_com.phd_com_state == PHD_AG_STATE_CON_ASSOC_OPERATING) 
    {                  
        uint_8 i=0; 
        uint_16 *scan_rpt_no;
        AbsoluteTime *p_time = NULL;                         

        _int_disable();
        /* if g_phdc_com.phd_buff already in use, return */
        if(g_phdc_com.phd_buff_being_used == TRUE) 
        {
            _int_enable();
            return; 
        }
        g_phdc_com.phd_buff_being_used = TRUE;         
        _int_enable();
                  
        /* copy the measurements to send in the g_phdc_com.phd_buff */
        for(i=0; i < DIM_DATA_TX_SIZE; i++) 
        { 
            g_phdc_com.phd_buff[i] = PHD_WSL_DIM_DATA_TX[i];
        }
        /* set measurement  */
        for(i = 0; i < 4; i++) 
        {
            p_time = (AbsoluteTime *)&g_phdc_com.phd_buff[38+i*16];
            *p_time = p_msr->msr_time;
        }

        for(i=0;i<4;i++) 
        {
            uint_8 j = (uint_8)(i >> 1);
            uint_16 *data = (uint_16*)&g_phdc_com.phd_buff[36+i*16];             
            if((i & 0x01) == 0) 
            {
                *data = HOST_TO_BE_SHORT(p_msr->weight[j]);
            } 
            else /* bmi */ 
            {
                *data = HOST_TO_BE_SHORT(p_msr->bmi[j]);
            }
        }
        scan_rpt_no = (uint_16*)&g_phdc_com.phd_buff[24];
        *scan_rpt_no = HOST_TO_BE_SHORT(scan_report_no++);
        
        /* send measurements */          
        (void)USB_Class_PHDC_Send_Data(handle, FALSE, 0,SEND_DATA_QOS, 
            (uint_8_ptr)g_phdc_com.phd_buff, DIM_DATA_TX_SIZE);                                     
    }     
}

/* EOF */
