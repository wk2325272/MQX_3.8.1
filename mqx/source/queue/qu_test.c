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
* $FileName: qu_test.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for testing a queue.
*
*END************************************************************************/

/* Start CR 312 */
#define MQX_FORCE_USE_INLINE_MACROS 1
/* End CR 312 */

#include "mqx_inc.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_test
* Returned Value   : _mqx_uint - MQX_OK or an error code
* Comments         :
*    This function checks the consitstency and validity of a queue.
*    The queue pointers are checked to ensure that they
*    correctly form a circular doubly linked list, with the
*    correct number of elements as specified in the queue header.
*
*END*--------------------------------------------------------*/

_mqx_uint _queue_test
   (
      /* [IN] the queue to test */
      QUEUE_STRUCT_PTR q_ptr,

      /* [OUT] the element where the error was detected */
      pointer _PTR_    element_in_error_ptr
   )
{ /* Body */
   QUEUE_ELEMENT_STRUCT_PTR element_ptr;
   QUEUE_ELEMENT_STRUCT_PTR prev_ptr;
   _mqx_uint                size;

   _int_disable();
   size = _QUEUE_GET_SIZE(q_ptr) + 1;
   element_ptr = q_ptr->NEXT;
   prev_ptr    = (QUEUE_ELEMENT_STRUCT_PTR)((pointer)q_ptr);
   while (--size) {
      if (element_ptr == (pointer)q_ptr) {
         _int_enable();
         /* Size too big for # elements on queue */
         *element_in_error_ptr = element_ptr;
         return(MQX_CORRUPT_QUEUE);
      } /* Endif */
      if (element_ptr->PREV != prev_ptr) {
         _int_enable();
         *element_in_error_ptr = element_ptr;
         return(MQX_CORRUPT_QUEUE);
      } /* Endif */
      prev_ptr    = element_ptr;
      element_ptr = element_ptr->NEXT;
   } /* Endwhile */

   /* Does the last element in the ring point back to the queue head */
   if ((pointer)element_ptr != (pointer)q_ptr) {
      _int_enable();
      *element_in_error_ptr = element_ptr;
      return(MQX_CORRUPT_QUEUE);
   } /* Endif */

   /* Is the last element in ring pointed to by queues PREV field */
   if (q_ptr->PREV != prev_ptr) {
      _int_enable();
      *element_in_error_ptr = element_ptr;
      return(MQX_CORRUPT_QUEUE);
   } /* Endif */

   _int_enable();
   return(MQX_OK);

} /* Endbody */

/* EOF */
