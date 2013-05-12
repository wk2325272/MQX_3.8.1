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
* $FileName: sc_irdyq.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains psp functions for initializing the scheduler.n
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _psp_set_kernel_disable_level
* Returned Value   : 
* Comments         :
*    This function sets up the kernel disable priority.
*
*END*----------------------------------------------------------------------*/

void _psp_set_kernel_disable_level
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR        kernel_data;
   MQX_INITIALIZATION_STRUCT_PTR init_ptr;
   uint_32                       temp;

   _GET_KERNEL_DATA(kernel_data);

   init_ptr    = (MQX_INITIALIZATION_STRUCT_PTR)&kernel_data->INIT;


   /* Compute the enable and disable interrupt values for the kernel. */
   temp = init_ptr->MQX_HARDWARE_INTERRUPT_LEVEL_MAX;
   if ( temp > 7 ) {
      temp = 7;
      init_ptr->MQX_HARDWARE_INTERRUPT_LEVEL_MAX = 7;
   } else if ( temp == 0 ) {
      temp = 1;
      init_ptr->MQX_HARDWARE_INTERRUPT_LEVEL_MAX = 1;
   } /* Endif */

   kernel_data->DISABLE_SR = (uint_16)0x2000 | (uint_16)(temp << 8);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _psp_init_readyqs
* Returned Value   : uint_32 result_code
* Comments         :
*    This function sets up the kernel priority ready queues
*
*END*----------------------------------------------------------------------*/

uint_32 _psp_init_readyqs
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   READY_Q_STRUCT_PTR     q_ptr;
   uint_32                priority_levels;
   uint_32                n;
   uint_16                sr;

   _GET_KERNEL_DATA(kernel_data);
   kernel_data->READY_Q_LIST = (READY_Q_STRUCT_PTR) NULL;
   priority_levels = kernel_data->LOWEST_TASK_PRIORITY + 2;

   q_ptr = (READY_Q_STRUCT_PTR)
      _mem_alloc_zero(sizeof(READY_Q_STRUCT) * priority_levels);
#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if ( q_ptr == NULL ) {
      return (MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif
   _mem_set_type(q_ptr, MEM_TYPE_READYQ);

   n = priority_levels;
   while (n--) {
      q_ptr->HEAD_READY_Q  = (TD_STRUCT_PTR)q_ptr;
      q_ptr->TAIL_READY_Q  = (TD_STRUCT_PTR)q_ptr;
      q_ptr->PRIORITY      = (uint_16)n;
      q_ptr->NEXT_Q        = kernel_data->READY_Q_LIST;
      kernel_data->READY_Q_LIST = q_ptr++;
   } /* Endwhile */

   /* 
   ** Set the current ready q (where the ready queue searches start) to
   ** the head of the list of ready queues.
   */
   kernel_data->CURRENT_READY_Q = kernel_data->READY_Q_LIST;


   /* Initialize the ENABLE_SR fields in the ready queues */
   sr = (uint_16)0x2000 | 
      (uint_16)((kernel_data->INIT.MQX_HARDWARE_INTERRUPT_LEVEL_MAX - 1) << 8);
   n  = priority_levels;
   q_ptr =  kernel_data->READY_Q_LIST;
   while (n--) {
      q_ptr->ENABLE_SR = sr;
      if (  ((uint_16) 0xF00 & sr) > 0  ) {
         sr = (uint_16)sr - (uint_16)0x100; 
      } /* Endif */
      q_ptr = q_ptr->NEXT_Q;
   } /* Endwhile */

   return MQX_OK;

} /* Endbody */

/* EOF */
