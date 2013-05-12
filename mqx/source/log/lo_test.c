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
* $FileName: lo_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing the log component.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LOGS
#include "log.h"
#include "log_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _log_test
* Returned Value   : _mqx_uint MQX_OK, or mqx error code
* Comments         :
*   This function tests the log component for consistency.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _log_test
   (

      /* [OUT] the log in error */
      _mqx_uint _PTR_ log_error_ptr

   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR   kernel_data;
   LOG_COMPONENT_STRUCT_PTR log_component_ptr;
   LOG_HEADER_STRUCT_PTR    log_ptr;
   _mqx_uint                i;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_log_test, log_error_ptr);

   *log_error_ptr = 0;
   log_component_ptr = (LOG_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_LOG];
   if (log_component_ptr == NULL) {
      _KLOGX2(KLOG_log_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   if (log_component_ptr->VALID != LOG_VALID) {
      _KLOGX2(KLOG_log_test, MQX_INVALID_COMPONENT_BASE);
      return(MQX_INVALID_COMPONENT_BASE);
   } /* Endif */   


   _int_disable();
   for (i = 0; i < LOG_MAXIMUM_NUMBER; i++) {
      log_ptr = log_component_ptr->LOGS[i];
      if (log_ptr != NULL) {
         /* Verify the log pointers */
         if ((log_ptr->LOG_END != &log_ptr->DATA[log_ptr->MAX]) ||
             (log_ptr->LOG_START != &log_ptr->DATA[0]))
         {
            break;
         } /* Endif */
         
         if ((log_ptr->LOG_WRITE > log_ptr->LOG_END) ||
             (log_ptr->LOG_NEXT  > log_ptr->LOG_END) ||
             (log_ptr->LOG_READ  > log_ptr->LOG_END) ||
             (log_ptr->LAST_LOG  > log_ptr->LOG_END))
         {
            break;
         } /* Endif */
         if ((log_ptr->LOG_WRITE < log_ptr->LOG_START) ||
             (log_ptr->LOG_READ  < log_ptr->LOG_START) ||
             (log_ptr->LAST_LOG  < log_ptr->LOG_START))
         {
            break;
         } /* Endif */
         if ((log_ptr->LOG_NEXT != NULL) && 
            (log_ptr->LOG_NEXT < log_ptr->LOG_START))
         {
            break;
         } /* Endif */
      } /* Endif */
   } /* Endfor */
   _int_enable();
   

   if (i == LOG_MAXIMUM_NUMBER) {
      _KLOGX2(KLOG_log_test, MQX_OK);
      return(MQX_OK);
   } /* Endif */

   *log_error_ptr = i;
   _KLOGX3(KLOG_log_test, LOG_INVALID, i);
   return(LOG_INVALID);

} /* Endbody */
#endif /* MQX_USE_LOGS */

/* EOF */
