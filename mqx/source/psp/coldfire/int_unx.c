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
* $FileName: int_unx.c$
* $Version : 3.5.5.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for the unexpected
*   exception handling function for MQX, which will display on the
*   console what exception has ocurred.  
*   NOTE: the default I/O for the current task is used, since a printf
*   is being done from an ISR.  
*   This default I/O must NOT be an interrupt drive I/O channel.
*
*END************************************************************************/

#include "mqx_inc.h"
#include "fio.h"

#if MQX_USE_INTERRUPTS


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _int_unexpected_isr
* Returned Value   : void
* Comments         :
*    A default handler for all unhandled interrupts.
*    It determines the type of interrupt or exception and prints out info.
*
*END*----------------------------------------------------------------------*/

void _int_unexpected_isr
   (
      /* [IN] the parameter passed to the default ISR, the vector */
      pointer parameter
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR            kernel_data;
   PSP_BASIC_INT_FRAME_STRUCT_PTR    basic_frame_ptr;
   PSP_INTERRUPT_FRAME_STRUCT_PTR    frame_ptr;
   TD_STRUCT_PTR                     td_ptr;
   volatile uint_32                           vector, offset;

   _GET_KERNEL_DATA(kernel_data);
   td_ptr      = kernel_data->ACTIVE_PTR;

   if (kernel_data->IN_ISR == 1) {
      /* Get the pointer to the data stored on the task stack */
      basic_frame_ptr = (pointer)td_ptr->STACK_PTR;
   } else {
      frame_ptr = (PSP_INTERRUPT_FRAME_STRUCT_PTR)&parameter;
      basic_frame_ptr = &frame_ptr->BASIC_FRAME;
   }/* Endif */

   vector = PSP_FAV_GET_VECTOR_NUMBER(basic_frame_ptr->FRAME.FORMAT_AND_VECTOR);
   offset = PSP_FAV_GET_VECTOR_OFFSET(basic_frame_ptr->FRAME.FORMAT_AND_VECTOR);

   printf( "\n*** UNHANDLED INTERRUPT ***\n"); 
   printf( "Vector #: %d  0x%x\n\r",vector,vector);
   printf( "Offset  : %d  0x%x\n\r",offset,offset);
   printf( "Task Id: 0x%0x Td_ptr 0x%x Stack Frame: 0x%x\n\r",
      td_ptr->TASK_ID, td_ptr, basic_frame_ptr);

   printf( "Interrupt_nesting level: %d   PC: 0x%08x   SR: 0x%04x\n\r",
      kernel_data->IN_ISR,
      basic_frame_ptr->FRAME.RETURN_ADDRESS,
      basic_frame_ptr->FRAME.STATUS_REGISTER );

   td_ptr->STATE = UNHANDLED_INT_BLOCKED;
   _task_set_error_td_internal(td_ptr, MQX_UNHANDLED_INTERRUPT);
 
   kernel_data->IN_ISR = 0;
   _task_block();

} /* Endbody */

#endif
/* EOF */
