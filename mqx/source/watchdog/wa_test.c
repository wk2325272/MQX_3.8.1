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
* $FileName: wa_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing the watchdog component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_SW_WATCHDOGS
#include "watchdog.h"
#include "wdog_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _watchdog_test
* Returned Value  : _mqx_uint - MQX_OK or an MQX error code.
* Comments        : 
*   This functions tests the consistency and validity of the watchdog
* component.
*
*END*------------------------------------------------------------------*/

_mqx_uint _watchdog_test
   (
      /*  [OUT]  the watchdog component base if an error occurs */
      pointer _PTR_ watchdog_error_ptr,

      /*  [OUT]  the watchdog table pointer if an error occurs */
      pointer _PTR_ watchdog_table_error_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR          kernel_data;
   WATCHDOG_COMPONENT_STRUCT_PTR   watchdog_component_ptr;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE3(KLOG_watchdog_test, watchdog_error_ptr, watchdog_table_error_ptr);

   *watchdog_error_ptr = NULL;
   *watchdog_table_error_ptr = NULL;

   watchdog_component_ptr = (WATCHDOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_WATCHDOG];
   if (watchdog_component_ptr == NULL) {
      _KLOGX2(KLOG_watchdog_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   *watchdog_error_ptr = watchdog_component_ptr;
   if (watchdog_component_ptr->VALID != WATCHDOG_VALID) {
      _KLOGX2(KLOG_watchdog_test, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */

   _KLOGX2(KLOG_watchdog_test, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_SW_WATCHDOGS */

/* EOF */
