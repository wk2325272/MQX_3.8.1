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
* $FileName: irtc_mcf5xxx.c$
* $Version : 3.8.18.2$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Processor family specific file needed for RTC module.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>

#define RTC_CTRL_WE0_MASK           (1 << 0)
#define RTC_CTRL_WE1_MASK           (1 << 1)
#define RTC_CTRL_WE_MASK            (RTC_CTRL_WE1_MASK|RTC_CTRL_WE0_MASK)


#define RTC_BASE_YEAR               2112
#define RTC_STATUS_INVAL_MASK       (1 << 0)
#define RTC_STATUS_C_DON_MASK       (1 << 1)
#define RTC_STATUS_BERR_MASK        (1 << 2)
#define RTC_STATUS_OCAL_MASK        (1 << 3)
#define RTC_STATUS_WPE_MASK         (1 << 4)
#define RTC_STATUS_CLV_MASK         (1 << 5)

#define RTC_CTRL_SWR_MASK           256

#define NUM_OF_TEST1                256
#define INTERNAL_WPE_CNTR_TEST      20

#define IRTC_MINIMAL_YEAR           1984

/* local function prototypes */
static void _rtc_memcpy(void * dst, const void * src, uint_32 num);


/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_lock
* Returned Value   : none
* Comments         :
*    This function locks RTC registers.
*
*END*********************************************************************/

void _rtc_lock (void)
{
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    rtc->RTC_CTRL &= ~RTC_CTRL_WE_MASK;     /* 0b00 */    
    rtc->RTC_CTRL |=  RTC_CTRL_WE1_MASK;    /* 0b10 */
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_unlock
* Returned Value   : MQX_OK if sucessfull
* Comments         :
*    This function unlocks RTC registers.
*
*END*********************************************************************/
_mqx_uint _rtc_unlock(void)
{
    
   VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();
   uint_16 rtc_ctrl = rtc->RTC_CTRL;
   int i; 
   
   rtc_ctrl &= ~( RTC_CTRL_WE0_MASK | RTC_CTRL_WE1_MASK ); 
   rtc->RTC_CTRL = rtc_ctrl;        /* 0b00 */
   rtc_ctrl |= RTC_CTRL_WE0_MASK;   
   rtc->RTC_CTRL = rtc_ctrl;        /* 0b01 */
   rtc_ctrl |= RTC_CTRL_WE1_MASK;   
   rtc->RTC_CTRL = rtc_ctrl;        /* 0b11 */
   rtc_ctrl &= ~RTC_CTRL_WE0_MASK;   
   rtc->RTC_CTRL = rtc_ctrl;        /* 0b10 */ 
  
   /* test, if unlock was successfull, there is some delay
   ** in setting the WPE bit, we repeat the test a few times */ 
   for(i = 0; i < INTERNAL_WPE_CNTR_TEST; i++) 
   {      
      if( !(rtc->RTC_STATUS & RTC_STATUS_WPE_MASK) )
      {
         return MQX_OK;
      } 
   }
   
   return MQX_RTC_UNLOCK_FAILED;
    
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_tested_unlock
* Returned Value   : MQX_OK if sucessfull
* Comments         :
*    This function calls unlocks RTC registers functions. If unlock failed,
*    repeat rtc unlock num_of_test times
*
*END*********************************************************************/
_mqx_uint _rtc_tested_unlock ( _mqx_uint num_of_test )
{
   _mqx_uint counter = 0;

   VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address();
   
   /* wait, until CPU Low Voltage. */
   while( rtc->RTC_STATUS & RTC_STATUS_CLV_MASK )
   {
   }

   for( counter = 0; counter < num_of_test; counter++ )
   {
      if( _rtc_unlock() == MQX_OK ) 
      {
         return MQX_OK;
      }
   }

   return MQX_RTC_UNLOCK_FAILED;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_time
* Returned Value   : none
* Comments         :
*    This function sets the RTC time according to given time struct.
*
*END*********************************************************************/

void _rtc_set_time 
(
    /* [IN] given time to be set as RTC time */
    RTC_TIME_STRUCT_PTR time
)
{
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    _int_disable(); 

    while( _rtc_tested_unlock( NUM_OF_TEST1 ) != MQX_OK ){
        ;
    }
    
    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};
    
    rtc->RTC_YEAR        = (int_8)((int_16)time->year-RTC_BASE_YEAR);
    rtc->RTC_MONTH       = (uint_8)time->month;
    rtc->RTC_DAYS        = (uint_8)time->days;
    rtc->RTC_DAY_OF_WEEK = (uint_8)time->wday;
    rtc->RTC_HOURS       = (uint_8)time->hours;
    rtc->RTC_MINUTES     = (uint_8)time->minutes;
    rtc->RTC_SECONDS     = (uint_8)time->seconds;
    
    _rtc_lock();
    _int_enable(); 
}


/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_time
* Returned Value   : none
* Comments         :
*    This function gets the RTC time and stores it in given time struct.
*
*END*********************************************************************/
void _rtc_get_time 
(
    /* [OUT] this parameter gets actual RTC time */
    RTC_TIME_STRUCT_PTR time
)
{
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};

    time->year    = rtc->RTC_YEAR + RTC_BASE_YEAR;
    time->month   = rtc->RTC_MONTH;       
    time->days    = rtc->RTC_DAYS;        
    time->wday    = rtc->RTC_DAY_OF_WEEK; 
    time->hours   = rtc->RTC_HOURS;       
    time->minutes = rtc->RTC_MINUTES;     
    time->seconds = rtc->RTC_SECONDS;     
}


/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_alarm
* Returned Value   : none
* Comments         :
*    This function sets RTC alarm time according to given time struct.
*    Alarm happens a half second after match.
*
*END*********************************************************************/
void _rtc_set_alarm 
(
    /* [IN] time to be set as RTC alarm time */
    RTC_TIME_STRUCT_PTR time
)
{
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    _int_disable();
    _rtc_tested_unlock(NUM_OF_TEST1);
    
    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};

    rtc->RTC_ALM_YEAR        = (int_8)((uint_16)time->year-RTC_BASE_YEAR);  
    rtc->RTC_ALM_MONTH       = (uint_8)time->month;
    rtc->RTC_ALM_DAYS        = (uint_8)time->days;
    rtc->RTC_ALM_HOURS       = (uint_8)time->hours;
    rtc->RTC_ALM_MINUTES     = (uint_8)time->minutes;
    rtc->RTC_ALM_SEC         = (uint_8)time->seconds;
    
    _rtc_lock();
    _int_enable();
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_alarm
* Returned Value   : none
* Comments         :
*    This function gets the RTC alarm time and stores it in given time struct.
*
*END*********************************************************************/
void _rtc_get_alarm 
(
    /* [OUT] this parameter gets the RTC alarm time */
    RTC_TIME_STRUCT_PTR time
)
{
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    /* Wait until time is coherent */
    while (rtc->RTC_STATUS & RTC_STATUS_INVAL_MASK){};  
    
    time->year    = rtc->RTC_ALM_YEAR + RTC_BASE_YEAR;
    time->month   = rtc->RTC_ALM_MONTH;       
    time->days    = rtc->RTC_ALM_DAYS;        
    time->hours   = rtc->RTC_ALM_HOURS;       
    time->minutes = rtc->RTC_ALM_MINUTES;     
    time->seconds = rtc->RTC_ALM_SEC;  
}


/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_set_stopwatch
* Returned Value   : none
* Comments         :
*    This function sets the RTC stopwatch decrementer value (minutes).
*    Stopwatch decrements each new RTC minute and stops (disables) at -1.
*
*END*********************************************************************/
void _rtc_set_stopwatch 
(
    /* [IN] number of minutes to countdown */
    uint_32 minutes
)
{
    uint_32 tmp;
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    _int_disable();
    _rtc_tested_unlock(NUM_OF_TEST1);
    
    rtc->RTC_COUNTDOWN_COUNT = (uint_8)minutes;
    
    _rtc_lock();
    _int_enable();  
}


/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_stopwatch
* Returned Value   : actual RTC minute stopwatch counter value
* Comments         :
*    This function returns the actual value of RTC stopwatch decrementer.
*
*END*********************************************************************/

uint_32 _rtc_get_stopwatch 
(
    void
)
{
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    return rtc->RTC_COUNTDOWN_COUNT;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_inc_upcounter
* Returned Value   : none
* Comments         :
*    This function increments up-counter register by 1.
*
*END*********************************************************************/

void _rtc_inc_upcounter
(
    void
) 
{ 
    
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    _int_disable();    
    _rtc_tested_unlock(NUM_OF_TEST1);

    rtc->RTC_UP_CNTR_L = 0; 

    _rtc_lock();
    _int_enable();    
}


/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_get_upcounter
* Returned Value   : value of up-counter register
* Comments         :
*    This function returns value of up-counter register.
*
*END*********************************************************************/

uint_32 _rtc_get_upcounter
(
    void
) 
{ 
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    return ((uint_32)rtc->RTC_UP_CNTR_L + ((uint_32)rtc->RTC_UP_CNTR_H << 16)); 
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_sync_with_mqx
* Returned Value   : none
* Comments         :
*    This function synchronizes RTC time with MQX date.
*
*END*********************************************************************/

_mqx_int _rtc_sync_with_mqx 
(
    /* [IN] whether to update MQX time (source is RTC) or RTC time (source is MQX) */
    boolean update_mqx
)
{
    RTC_TIME_STRUCT rtc_time;
    DATE_STRUCT     mqx_date;
    TIME_STRUCT     mqx_time;
    
    if (update_mqx == TRUE) 
    {
        _rtc_get_time (&rtc_time);
        _rtc_time_to_mqx_date (&rtc_time, &mqx_date);
        if (TRUE == _time_from_date(&mqx_date, &mqx_time)) {
            _time_set (&mqx_time);       	
        }
    }
    else
    {
        _time_get (&mqx_time);
        _time_to_date(&mqx_time, &mqx_date);
        if( mqx_date.YEAR < IRTC_MINIMAL_YEAR )
        {
            return RTC_INVALID_TIME;
        }
        _rtc_time_from_mqx_date (&mqx_date, &rtc_time);
        _rtc_set_time (&rtc_time);
    }
    return MQX_OK;
}

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

void _rtc_time_to_mqx_time 
(
    /* [IN] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time, 
    /* [OUT] MQX time representation */
    TIME_STRUCT_PTR     mqx_time
)
{

    DATE_STRUCT     mqx_date;
    
    _rtc_time_to_mqx_date (rtc_time, &mqx_date);
    _time_from_date(&mqx_date, mqx_time);
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_from_mqx_time
* Returned Value   : none
* Comments         :
*    This function transforms MQX time format to RTC time format.
*    MQX time range is shorter (49710 days vs. 65536 days), miliseconds are ignored.
*
*END*********************************************************************/

void _rtc_time_from_mqx_time
(
    /* [IN] MQX time representation */
    TIME_STRUCT_PTR     mqx_time, 
    /* [OUT] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time
)
{

    DATE_STRUCT     mqx_date;

    _time_to_date(mqx_time, &mqx_date);
    _rtc_time_from_mqx_date (&mqx_date, rtc_time);

}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_to_mqx_date
* Returned Value   : none
* Comments         :
*    This function transforms RTC time format to MQX date format.
*    Miliseconds are set to 0.
*
*END*********************************************************************/

void _rtc_time_to_mqx_date 
(
    /* [IN] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time, 
    /* [OUT] MQX date representation */
    DATE_STRUCT_PTR     mqx_date
)
{
    mqx_date->MILLISEC =  0;
    mqx_date->SECOND   =  rtc_time->seconds;
    mqx_date->MINUTE   =  rtc_time->minutes;
    mqx_date->HOUR     =  rtc_time->hours;
    mqx_date->DAY      =  rtc_time->days;
    mqx_date->MONTH    =  rtc_time->month;
    mqx_date->YEAR     =  rtc_time->year;
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_time_from_mqx_date
* Returned Value   : none
* Comments         :
*    This function transforms MQX date format to RTC time format.
*    Miliseconds are ignored.
*
*END*********************************************************************/

void _rtc_time_from_mqx_date
(
    /* [IN] MQX date representation */
    DATE_STRUCT_PTR     mqx_date, 
    /* [OUT] RTC time representation */
    RTC_TIME_STRUCT_PTR rtc_time
)
{
    rtc_time->seconds   = mqx_date->SECOND;   
    rtc_time->minutes   = mqx_date->MINUTE;    
    rtc_time->hours     = mqx_date->HOUR;      
    rtc_time->days      = mqx_date->DAY;       
    rtc_time->month     = mqx_date->MONTH;     
    rtc_time->year      = mqx_date->YEAR;      
}

/*FUNCTION****************************************************************
* 
* Function Name    : _rtc_memcpy
* Returned Value   : void

* Comments         :
*   Local function for memory copy byte by byte    
*
*END*********************************************************************/


static void _rtc_memcpy(void * dst, const void * src, uint_32 num)
{
    while(num--){*((uint_8 *)dst)++ = *((uint_8 *)src)++;}
}


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
)
{
    
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();
    
    if (dst_address + size <= sizeof(rtc->RTC_STAND_BY_RAM))
    {
        _int_disable();    
        _rtc_tested_unlock(NUM_OF_TEST1);

        _rtc_memcpy (
                (void *)&(rtc->RTC_STAND_BY_RAM[dst_address]), 
                src_ptr, 
                size
            );    

        _rtc_lock();
        _int_enable();    
        
        return MQX_OK;
    }
    
    return MQX_INVALID_SIZE;
}



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
)
{
  
    VMCF5XXX_IRTC_STRUCT_PTR rtc = _bsp_get_rtc_base_address ();

    if (src_address + size <= sizeof(rtc->RTC_STAND_BY_RAM))
    {
        _rtc_memcpy (
                dst_ptr, 
                (void *)&(rtc->RTC_STAND_BY_RAM[src_address]), 
                size
            );
        
        return MQX_OK;
    }
    
    return MQX_INVALID_SIZE;
} 

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
)
{
   RTC_TIME_STRUCT rtc_time; 
   
   /* IRTC hardware cannot set date lower than 1984 */
   if( time->YEAR < IRTC_MINIMAL_YEAR )
   {
      return RTC_INVALID_TIME;
   }
   
   rtc_time.year = time->YEAR;
   rtc_time.month = time->MONTH;
   rtc_time.days = time->DAY;
   rtc_time.hours = time->HOUR;
   rtc_time.minutes = time->MINUTE;
   rtc_time.seconds = time->SECOND; 
   
   _rtc_set_time( &rtc_time ); 
   return MQX_OK;
}

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
)
{
    RTC_TIME_STRUCT rtc_time; 
    
    _rtc_get_time( &rtc_time);
    time->YEAR = rtc_time.year;
    time->MONTH = rtc_time.month;
    time->DAY = rtc_time.days;
    time->HOUR = rtc_time.hours;
    time->MINUTE = rtc_time.minutes;
    time->SECOND = rtc_time.seconds; 
    time->MILLISEC = 0;
}

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
)
{
   RTC_TIME_STRUCT rtc_alarm;
   
   /* IRTC hardware cannot set date lower than 1984 */
   if( time->YEAR < IRTC_MINIMAL_YEAR )
   {
      return RTC_INVALID_TIME;
   }
   
   rtc_alarm.year = time->YEAR;
   rtc_alarm.month = time->MONTH;
   rtc_alarm.days = time->DAY;
   rtc_alarm.hours = time->HOUR;
   rtc_alarm.minutes = time->MINUTE;
   rtc_alarm.seconds = time->SECOND; 
   _rtc_set_alarm( &rtc_alarm );
    
   return MQX_OK;
}

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
)
{
   RTC_TIME_STRUCT rtc_alarm;
   
    _rtc_get_alarm( &rtc_alarm );
    time->YEAR = rtc_alarm.year;
    time->MONTH = rtc_alarm.month;
    time->DAY = rtc_alarm.days;
    time->HOUR = rtc_alarm.hours;
    time->MINUTE = rtc_alarm.minutes;
    time->SECOND = rtc_alarm.seconds;
    time->MILLISEC = 0;
}
/* EOF */
