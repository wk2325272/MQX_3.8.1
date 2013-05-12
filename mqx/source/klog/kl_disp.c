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
* $FileName: kl_disp.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the display function for the Kernel Data Logging
*   facility.  This function reads and displays one kernel log entry.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_KERNEL_LOGGING
#include "lwlog.h"
#include "klog.h"
#include "fio.h"

extern char _PTR_ _klog_get_function_name_internal(uint_32);


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _klog_display
* Returned Value   : boolean returns FALSE if log is empty TRUE otherwise
* Comments         :
*   This function prints out one kernel log entry
*
*END*----------------------------------------------------------------------*/

boolean _klog_display
   (
      void
   )
{ /* Body */
   LWLOG_ENTRY_STRUCT      log_entry;
   LWLOG_ENTRY_STRUCT_PTR  log_ptr;
   _mqx_uint               result;
   _mqx_int                i;

   log_ptr = &log_entry;
   result = _lwlog_read(LOG_KERNEL_LOG_NUMBER, LOG_READ_OLDEST_AND_DELETE, log_ptr);
   if (result != MQX_OK) {
      return FALSE;
   } /* Endif */

#if MQX_LWLOG_TIME_STAMP_IN_TICKS == 0
   /* Normalize the time in the record */
   log_ptr->MILLISECONDS += log_ptr->MICROSECONDS / 1000;
   log_ptr->MICROSECONDS  = log_ptr->MICROSECONDS % 1000;
   log_ptr->SECONDS      += log_ptr->MILLISECONDS / 1000;
   log_ptr->MILLISECONDS  = log_ptr->MILLISECONDS % 1000;

   printf("%ld. %ld:%03ld%03ld -> ",
      (uint_32)log_ptr->SEQUENCE_NUMBER, 
      log_ptr->SECONDS,
      log_ptr->MILLISECONDS, 
      log_ptr->MICROSECONDS);
#else

   printf("%ld. ", (uint_32)log_ptr->SEQUENCE_NUMBER);

   PSP_PRINT_TICKS(&log_ptr->TIMESTAMP);

   printf(" -> ");
#endif

   switch (log_ptr->DATA[0]) {

      case KLOG_FUNCTION_ENTRY:
      case KLOG_FUNCTION_EXIT:
         printf("%s %22.22s ", 
            (log_ptr->DATA[0] == KLOG_FUNCTION_ENTRY) ? "FUN " : "XFUN",
            _klog_get_function_name_internal((uint_32)log_ptr->DATA[1]));
         /* Start CR 238 */
         /* for (i = 2; i < LWLOG_MAXIMUM_DATA_ENETRIES; ++i) { */
         for (i = 2; i < LWLOG_MAXIMUM_DATA_ENTRIES; ++i) {
         /* End CR 238 */
            printf("0x%lX ", (uint_32)log_ptr->DATA[i]);
         } /* Endfor */
         printf("\n");
         break;

      case KLOG_INTERRUPT:
         printf("INT   0x%lX\n", (uint_32)log_ptr->DATA[1]);
         break;

      case KLOG_INTERRUPT_END:
         printf("INT   0x%lX END\n",(uint_32)log_ptr->DATA[1]);
         break;

      case KLOG_CONTEXT_SWITCH:
         printf("NEW TASK TD 0x%lX ID 0x%lX STATE 0x%lX STACK 0x%lX\n",
            (uint_32)log_ptr->DATA[1], (uint_32)log_ptr->DATA[2], 
            (uint_32)log_ptr->DATA[3], (uint_32)log_ptr->DATA[4]);
            break;

      default:
         printf("USER ENTRY: 0x%lX:", (uint_32)log_ptr->DATA[0]);
         /* Start CR 238 */
         /* for (i = 1; i < LWLOG_MAXIMUM_DATA_ENETRIES; ++i) { */
         for (i = 1; i < LWLOG_MAXIMUM_DATA_ENTRIES; ++i) {
         /* End CR 238 */
            printf("0x%lX ", (uint_32)log_ptr->DATA[i]);
         } /* Endfor */
         printf("\n");
         break;
   } /* Endswitch */

   return TRUE;

} /* Endbody */
#endif /* MQX_KERNEL_LOGGING */

/* EOF */
