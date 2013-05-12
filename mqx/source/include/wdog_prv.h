#ifndef __wdog_prv_h__
#define __wdog_prv_h__ 1
/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: wdog_prv.h$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This include file is used to define constants and data types for the
*   watchdog component.
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*                        CONSTANT DEFINITIONS                              */

/* The number of tasks in the alarm table */
#define WATCHDOG_TABLE_SIZE   (16)

/* The watchdog validation number */
#define WATCHDOG_VALID        (_mqx_uint)(0x77646f67)     /* "wdog" */

/*--------------------------------------------------------------------------*/
/*                       DATATYPE DEFINITIONS                               */

/*
**  watchdog alarm table structure
*/
typedef struct watchdog_alarm_table_struct
{

   /* The next table if required */
   struct watchdog_alarm_table_struct _PTR_ NEXT_TABLE_PTR;

   /* The tasks being monitored */
   TD_STRUCT_PTR                            TD_PTRS[WATCHDOG_TABLE_SIZE];

} WATCHDOG_ALARM_TABLE_STRUCT, _PTR_ WATCHDOG_ALARM_TABLE_STRUCT_PTR;

/*
** watchdog component structure
*/
typedef struct watchdog_component_struct
{

   /* The table of alarms */
   WATCHDOG_ALARM_TABLE_STRUCT ALARM_ENTRIES;

   /* Watchdog validation stamp */
   _mqx_uint                    VALID;
   
   /* The function to call when the watchdog expires */
   void            (_CODE_PTR_ ERROR_FUNCTION)(TD_STRUCT_PTR td_ptr);
   
   /* The old timer interrupt handler */
   void            (_CODE_PTR_ TIMER_INTERRUPT_HANDLER)(pointer parameter);

   /* The interrupt vector */
   _mqx_uint                    INTERRUPT_VECTOR;

} WATCHDOG_COMPONENT_STRUCT, _PTR_ WATCHDOG_COMPONENT_STRUCT_PTR;


/*--------------------------------------------------------------------------*/
/*                       PROTOTYPE DEFINITIONS                              */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TAD_COMPILE__
extern void     _watchdog_cleanup(TD_STRUCT_PTR);
extern void     _watchdog_isr(pointer);
extern boolean  _watchdog_start_internal(MQX_TICK_STRUCT_PTR);
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */

