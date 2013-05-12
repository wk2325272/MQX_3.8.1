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
* $FileName: qu_insrt.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for inserting into to a queue
*   Note that the QUEUE macros can be found in mqx_prv.h
*
*END************************************************************************/

/* Start CR 312 */
#define MQX_FORCE_USE_INLINE_MACROS 1
/* End CR 312 */

#include "mqx_inc.h"


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_insert
* Returned Value   : boolean - TRUE if successful
* Comments         :
*    Queue the given element onto the queue
*
*END*--------------------------------------------------------*/

boolean _queue_insert
   (
      /* [IN] the queue to use */
      QUEUE_STRUCT_PTR         q_ptr,

      /* 
      ** [IN] an element in the queue, after which the new
      ** element is  to be inserted
      */
      QUEUE_ELEMENT_STRUCT_PTR qe_ptr,

      /* [IN] the element to insert into the queue */
      QUEUE_ELEMENT_STRUCT_PTR e_ptr
   )
{ /* Body */

   if (qe_ptr == NULL) {
      /* Insert at the front */
      qe_ptr = (QUEUE_ELEMENT_STRUCT_PTR)((pointer)q_ptr);
   } /* Endif */
   _int_disable();
   if ((q_ptr->MAX != 0) && (q_ptr->SIZE >= q_ptr->MAX)) {
      _int_enable();
      return(FALSE);
   } /* Endif */
   
   _QUEUE_INSERT(q_ptr, qe_ptr, e_ptr);
   _int_enable();
   return(TRUE);
   
} /* Endbody */

/* EOF */
