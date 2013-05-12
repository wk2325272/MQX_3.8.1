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
* $FileName: qu_head.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for returning the head of the queue
*   without actually dequeueing it.
*   Note that the QUEUE macros can be found in mqx_prv.h
*
*END************************************************************************/

/* Start CR 313 */
/* #define MQX_USE_INLINE_MACROS 1 */
/* End CR 313 */

#include "mqx_inc.h"


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_head
* Returned Value   : QUEUE_ELEMENT_STRUCT_PTR - pointer to 
*                    element at head of queue
* Comments         :
*    Return the first element on the queue, but do not dequeue it
*
*END*--------------------------------------------------------*/

QUEUE_ELEMENT_STRUCT_PTR _queue_head
   (
      /* [IN] the queue to use */
      QUEUE_STRUCT_PTR         q_ptr
   )
{ /* Body */
   QUEUE_ELEMENT_STRUCT_PTR e_ptr;

   _int_disable();
   if (q_ptr->SIZE == 0) {
      _int_enable();
      return(NULL);
   } /* Endif */
   
   e_ptr = q_ptr->NEXT;
   _int_enable();
   return(e_ptr);
   
} /* Endbody */

/* EOF */
