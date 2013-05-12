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
* $FileName: qu_next.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for returning the element on the
*   queue next to the input element, without dequeuing it.
*   Note that the QUEUE macros can be found in mqx_prv.h
*
*END************************************************************************/

/* Start CR 313 */
/* #define MQX_USE_INLINE_MACROS 1 */
/* End CR 313 */

#include "mqx_inc.h"


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_next
* Returned Value   : QUEUE_ELEMENT_STRUCT_PTR pointer to next
*                    element in queue
* Comments         :
*    return the element on the queue after the provided element,
* but do not dequeue it.
*
*END*--------------------------------------------------------*/

QUEUE_ELEMENT_STRUCT_PTR _queue_next
   (
      /* [IN] the queue to use */
      QUEUE_STRUCT_PTR         q_ptr,

      /* [IN] the element after this element is wanted */
      QUEUE_ELEMENT_STRUCT_PTR e_ptr
   )
{ /* Body */
   QUEUE_ELEMENT_STRUCT_PTR next_ptr;

   if (e_ptr == NULL) {
      return NULL;
   } /* Endif */
   _int_disable();
   next_ptr = e_ptr->NEXT;
   _int_enable();
   if (next_ptr == (QUEUE_ELEMENT_STRUCT_PTR)((pointer)q_ptr)) {
      /* At end of queue */
      next_ptr = NULL;
   } /* Endif */
   return(next_ptr);
   
} /* Endbody */

/* EOF */
