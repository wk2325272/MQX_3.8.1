/**HEADER********************************************************************
* 
* Copyright (c) 2008-2009 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2010 Embedded Access Inc.;
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
* $FileName: ctx_dmp.c$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains a function to display a diagnostic message
*   for an exception or unhandled interrupt event.
*
*   NOTE: the default I/O for the current task is used, since a printf
*   is being done from an ISR.
*   This default I/O must NOT be an interrupt drive I/O channel.
*
*END*******************************************************************/

#include "mqx_inc.h"
#include "fio.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_dump_context
* Returned Value   : void
* Comments         :
*    An exception or unhandled interrupt has occurred!
*    Determine the type of interrupt or exception and print out info.
*
*END*----------------------------------------------------------------------*/

void _psp_dump_context
   (
      pointer parameter
   )
{ /* Body */
   volatile KERNEL_DATA_STRUCT _PTR_       kernel_data;
   volatile TD_STRUCT _PTR_                td_ptr;
   volatile PSP_BLOCKED_STACK_STRUCT _PTR_ bl_ptr;
   pointer _sp;
   uint_32 tmp;

   _GET_KERNEL_DATA(kernel_data);
   td_ptr = kernel_data->ACTIVE_PTR;

   printf("Vector #: 0x%02lx  Task Id: 0x%08lx  Td_ptr 0x%08lx  TD_SP: 0x%08lx\n\r",
      (uint_32)parameter, (uint_32)td_ptr->TASK_ID,
      (uint_32)td_ptr, (uint_32)td_ptr->STACK_PTR);

   _sp = (pointer)_PSP_GET_SP();
   if (kernel_data->IN_ISR > 1) {
      volatile TD_STRUCT _PTR_ sys_td = SYSTEM_TD_PTR(kernel_data);
      if (_sp > sys_td->STACK_BASE) {
         printf(" *** ISR Stack underflow (SP > Base): 0x%08x > 0x%08x\n\r",
                (uint_32)_sp, (uint_32)sys_td->STACK_BASE);
      } /* Endif */
      if (_sp < sys_td->STACK_LIMIT) {
         printf(" *** ISR Stack overflow (SP < limit): 0x%08x < 0x%08x\n\r",
                 (uint_32)_sp, (uint_32)sys_td->STACK_LIMIT);
      } /* Endif */
   }
   else {
      if (_sp > td_ptr->STACK_BASE) {
         printf(" *** TD Stack underflow (SP > Base): 0x%08x > 0x%08x\n\r",
                (uint_32)_sp, (uint_32)td_ptr->STACK_BASE);
      } /* Endif */
      if (_sp < td_ptr->STACK_LIMIT) {
         printf(" *** TD Stack overflow (SP < limit): 0x%08x < 0x%08x\n\r",
                 (uint_32)_sp, (uint_32)td_ptr->STACK_LIMIT);
      } /* Endif */
   } /* Endif */

/* Start CR 1032 */
   _PSP_MSR_GET(tmp);
#if (MQX_CPU==PSP_CPU_AMCC405)
   {
   uint_32 tmp2;
   _PSP_SPR_GET(tmp2,PPC405_DEAR);
   printf("SP: 0x%08lx  DSISR:   0x%08lx  DEAR:  0x%08lx  \nSR: 0x%08lx  SRR1:    0x%08lx\n\r",
      _PSP_GET_SP(), _PSP_GET_DSISR(), tmp2, tmp,
      _PSP_GET_SRR1());
   }
#else
   printf("SP: 0x%08lx  DSISR:   0x%08lx  DAR:   0x%08lx  \nSR: 0x%08lx  SRR1:    0x%08lx\n\r",
      _PSP_GET_SP(), _PSP_GET_DSISR(), _PSP_GET_DAR(), tmp,
      _PSP_GET_SRR1());
#endif

   bl_ptr = (PSP_BLOCKED_STACK_STRUCT_PTR)td_ptr->STACK_PTR;
   printf("Task Saved Registers:         USER_SP: 0x%08x\n",
      (uint_32)bl_ptr + sizeof(*bl_ptr));
/* End CR 1032 */
   printf("LR:  %08lx  CTR: %08lx  XER: %08lx  CCR: %08lx\n",
      bl_ptr->LR, bl_ptr->CTR, bl_ptr->XER, bl_ptr->CCR);
   printf("MSR: %08lx  PC:  %08lx  R0:  %08lx  R3:  %08lx\n",
      bl_ptr->MSR, bl_ptr->RETURN_ADDRESS, bl_ptr->GPR[0], bl_ptr->GPR[1]);
   printf("R4:  %08lx  R5:  %08lx  R6:  %08lx  R7:  %08lx\n",
      bl_ptr->GPR[2], bl_ptr->GPR[3], bl_ptr->GPR[4], bl_ptr->GPR[5]);
   printf("R8:  %08lx  R9:  %08lx  R10: %08lx  R11: %08lx\n",
      bl_ptr->GPR[6], bl_ptr->GPR[7], bl_ptr->GPR[8], bl_ptr->GPR[9]);
   printf("R12: %08lx  R13: %08lx  R14: %08lx  R15: %08lx\n",
      bl_ptr->GPR[10], bl_ptr->GPR[11], bl_ptr->GPR[12], bl_ptr->GPR[13]);
   printf("R16: %08lx  R17: %08lx  R18: %08lx  R19: %08lx\n",
      bl_ptr->GPR[14], bl_ptr->GPR[15], bl_ptr->GPR[16], bl_ptr->GPR[17]);
   printf("R20: %08lx  R21: %08lx  R22: %08lx  R23: %08lx\n",
      bl_ptr->GPR[18], bl_ptr->GPR[19], bl_ptr->GPR[20], bl_ptr->GPR[21]);
   printf("R24: %08lx  R25: %08lx  R26: %08lx  R27: %08lx\n",
      bl_ptr->GPR[22], bl_ptr->GPR[23], bl_ptr->GPR[24], bl_ptr->GPR[25]);
   printf("R28: %08lx  R29: %08lx  R30: %08lx  R31: %08lx\n",
      bl_ptr->GPR[26], bl_ptr->GPR[27], bl_ptr->GPR[28], bl_ptr->GPR[29]);

   _INT_DISABLE();
   if (td_ptr->STATE != UNHANDLED_INT_BLOCKED) {
      td_ptr->STATE = UNHANDLED_INT_BLOCKED;
      td_ptr->INFO  = (_mqx_uint)parameter;

      _QUEUE_UNLINK(td_ptr);
   } /* Endif */
   _INT_ENABLE();

} /* Endbody */

/* EOF */
