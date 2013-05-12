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
* $FileName: sc_ipq.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for inserting at task descriptor
*   into a queue, by task priority.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_insert_priorityq_internal
* Returned Value   : none
* Comments         :
*   This function inserts a task descriptor into a task descriptor queue
* by order of task priority.
*
*END*----------------------------------------------------------------------*/

void _sched_insert_priorityq_internal
   (
      /* [IN] the address of the queue header */
      register QUEUE_STRUCT_PTR queue_ptr,

      /* [IN] the address of the task descriptor to insert */
      register TD_STRUCT_PTR    td_ptr
   )
{ /* Body */
   register TD_STRUCT_PTR td2_ptr;
   register TD_STRUCT_PTR td_prev_ptr;
   register _mqx_uint      priority;
   register _mqx_uint      count;

   td_prev_ptr = (TD_STRUCT_PTR)((pointer)queue_ptr);
   td2_ptr     = (TD_STRUCT_PTR)((pointer)queue_ptr->NEXT);
   count       = _QUEUE_GET_SIZE(queue_ptr) + 1;
   priority    = td_ptr->MY_QUEUE->PRIORITY;
   while (--count) {
      if (td2_ptr->MY_QUEUE->PRIORITY > priority) {
         break;
      } /* Endif */
      td_prev_ptr = td2_ptr;
      td2_ptr     = td2_ptr->TD_NEXT;
   } /* Endwhile */
   _QUEUE_INSERT(queue_ptr,
      (QUEUE_ELEMENT_STRUCT_PTR)((pointer)td_prev_ptr),
      (QUEUE_ELEMENT_STRUCT_PTR)((pointer)td_ptr));

} /* Endbody */

/* EOF */
