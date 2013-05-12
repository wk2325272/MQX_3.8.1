/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: irtc_mcf5xxx.h$
* $Version : 3.7.11.0$
* $Date    : Mar-16-2011$
*
* Comments:
*
*   Processor family specific file needed for RTC module.
*
*END************************************************************************/

#ifndef __irtc_mcf5xxx_h__
#define __irtc_mcf5xxx_h__

/******************************************************************************
 * interrupt masks definitions (RTC_ISR and RTC_IER registers)                *
 ******************************************************************************/  
#ifndef RTC_INVALID_TIME
#define  RTC_INVALID_TIME  (RTC_ERROR_BASE | 0x01)
#endif 

/* IRTC time reperesentation */
typedef struct rtc_time_struct
{
    uint_8  seconds;
    uint_8  minutes;
    uint_8  hours;
    uint_8  days;
    uint_8  wday;
    uint_8  month;
    uint_16 year;
} RTC_TIME_STRUCT, _PTR_ RTC_TIME_STRUCT_PTR;

/* IRTC generic functions */
/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_lock
* Returned Value   : none
* Comments         :
*    This function locks RTC registers.
*
*END*********************************************************************/

extern void     _rtc_lock (void);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_unlock
* Returned Value   : MQX_OK if sucessfull
* Comments         :
*    This function unlocks RTC registers.
*
*END*********************************************************************/

extern _mqx_uint     _rtc_unlock (void);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_tested_unlock
* Returned Value   : MQX_OK if sucessfull
* Comments         :
*    This function calls unlocks RTC registers functions. If unlock failed,
*    repeat rtc unlock num_of_test times
*
*END*********************************************************************/
extern _mqx_uint     _rtc_tested_unlock ( _mqx_uint );

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_time
* Returned Value   : none
* Comments         :
*    This function sets the RTC time according to given time struct.
*
*END*********************************************************************/

extern void     _rtc_set_time (RTC_TIME_STRUCT_PTR time);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_time
* Returned Value   : none
* Comments         :
*    This function gets the RTC time and stores it in given time struct.
*
*END*********************************************************************/

extern void     _rtc_get_time (RTC_TIME_STRUCT_PTR time);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_alarm
* Returned Value   : none
* Comments         :
*    This function sets RTC alarm time according to given time struct.
*    Alarm happens a half second after match.
*
*END*********************************************************************/

extern void     _rtc_set_alarm (RTC_TIME_STRUCT_PTR time);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_alarm
* Returned Value   : none
* Comments         :
*    This function gets the RTC alarm time and stores it in given time struct.
*
*END*********************************************************************/

extern void     _rtc_get_alarm (RTC_TIME_STRUCT_PTR time);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_stopwatch
* Returned Value   : none
* Comments         :
*    This function sets the RTC stopwatch decrementer value (minutes).
*    Stopwatch decrements each new RTC minute and stops (disables) at -1.
*
*END*********************************************************************/

extern void     _rtc_set_stopwatch (uint_32 minutes);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_stopwatch
* Returned Value   : actual RTC minute stopwatch counter value
* Comments         :
*    This function returns the actual value of RTC stopwatch decrementer.
*
*END*********************************************************************/

extern uint_32  _rtc_get_stopwatch (void);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_inc_upcounter
* Returned Value   : none
* Comments         :
*    This function increments up-counter register by 1.
*
*END*********************************************************************/

extern void     _rtc_inc_upcounter(void);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_upcounter
* Returned Value   : value of up-counter register
* Comments         :
*    This function returns value of up-counter register.
*
*END*********************************************************************/

extern uint_32  _rtc_get_upcounter(void);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_write_to_standby_ram
* Returned Value   : MQX_OK           - operation successful
*                    MQX_INVALID_SIZE - write operation failed
* Comments         :
*   Function writes "size" bytes pointed by "src_ptr" 
*   into IRTC module stand-by RAM at address "dst_address"
*
*END*********************************************************************/

_mqx_uint _rtc_write_to_standby_ram
(
    /* [IN]  Destination address in standby ram */
    uint_32  dst_address, 
    /* [IN]  Source data pointer */
    uint_8 * src_ptr, 
    /* [IN]  Number of bytes to be written */
    uint_32  size
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_read_from_standby_ram
* Returned Value   : MQX_OK             - successful
*                    MQX_INVALID_SIZE   - read operation failed
* Comments         :
*   Function reads "size" bytes from "src_address" in stand-by RAM
*   into "dst_ptr" 
*
*END*********************************************************************/

_mqx_uint _rtc_read_from_standby_ram 
(
    /* [IN]  Source address in standby ram */
    uint_32  src_address, 
    /* [OUT] Destination data pointer */
    uint_8 * dst_ptr, 
    /* [IN]  Number of bytes to be read */
    uint_32  size
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_to_mqx_time
* Returned Value   : none
* Comments         :
*    This function transforms RTC time format to MQX time format.
*    RTC time range is wider (65536 days vs. 49710 days), overflow is not checked,
*    miliseconds are set to 0.
*
*END*********************************************************************/
extern void _rtc_time_to_mqx_time 
(
    /* [IN] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time, 
    /* [OUT] MQX time representation */
    TIME_STRUCT_PTR     mqx_time
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_from_mqx_time
* Returned Value   : none
* Comments         :
*    This function transforms MQX time format to RTC time format.
*    MQX time range is shorter (49710 days vs. 65536 days), miliseconds are ignored.
*
*END*********************************************************************/

extern void _rtc_time_from_mqx_time
(
    /* [IN] MQX time representation */
    TIME_STRUCT_PTR     mqx_time, 
    /* [OUT] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_to_mqx_date
* Returned Value   : none
* Comments         :
*    This function transforms RTC time format to MQX date format.
*    Miliseconds are set to 0.
*
*END*********************************************************************/

extern void _rtc_time_to_mqx_date 
(
    /* [IN] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time, 
    /* [OUT] MQX date representation */
    DATE_STRUCT_PTR     mqx_date
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_from_mqx_date
* Returned Value   : none
* Comments         :
*    This function transforms MQX date format to RTC time format.
*    Miliseconds are ignored.
*
*END*********************************************************************/


extern void _rtc_time_from_mqx_date
(
    /* [IN] MQX date representation */
    DATE_STRUCT_PTR     mqx_date, 
    /* [OUT] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_sync_with_mqx
* Returned Value   : none
* Comments         :
*    This function synchronizes RTC time with MQX date.
*
*END*********************************************************************/

extern _mqx_int _rtc_sync_with_mqx 
(
    /* [IN] whether to update MQX time (source is RTC) or RTC time (source is MQX) */
    boolean update_mqx
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_time_mqxd
* Returned Value   : actual RTC minute stopwatch counter value
* Comments         :
*    This function sets the RTC time according to DATE_STRUCT.
*
*END*********************************************************************/
_mqx_int _rtc_set_time_mqxd 
(
   DATE_STRUCT_PTR time
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_time_mqxd
* Returned Value   : actual RTC minute stopwatch counter value
* Comments         :
*    This function returns rtc time to DATE_STRUCT.
*
*END*********************************************************************/
void _rtc_get_time_mqxd 
(
   DATE_STRUCT_PTR time
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_alarm_mqxd
* Returned Value   : none
* Comments         :
*    This function sets RTC alarm time.
*    Alarm happens immediately after match.
*
*END*********************************************************************/
_mqx_int _rtc_set_alarm_mqxd 
(
   DATE_STRUCT_PTR time
);

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_alarm
* Returned Value   : none
* Comments         :
*    This function gets the RTC alarm time and stores it in DATE_STRUCT.
*
*END*********************************************************************/
void _rtc_get_alarm_mqxd 
(
   DATE_STRUCT_PTR time
);

#endif   /* __irtc_mcf5xxx_h__ */
