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
* $FileName: sc_numq.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for use with priority
*   manipulation of tasks.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_get_max_priority_on_q_internal
* Returned Value   : _mqx_uint priority
* Comments         :
*   This function finds the maximum of the priorities of the tasks
* on the waiting queue.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_get_max_priority_on_q_internal
   (
      /* [IN] the address of the queue header */
      register QUEUE_STRUCT_PTR queue_ptr
   )
{ /* Body */
   register TD_STRUCT_PTR td_ptr;
   register _mqx_uint      priority;
   register _mqx_uint      count;

   td_ptr   = (TD_STRUCT_PTR)((pointer)queue_ptr->NEXT);
   count    = _QUEUE_GET_SIZE(queue_ptr) + 1;
   priority = MAX_MQX_UINT;
   while (--count) {
      if (td_ptr->MY_QUEUE->PRIORITY < priority) {
         priority = td_ptr->MY_QUEUE->PRIORITY;
      } /* Endif */
      td_ptr = td_ptr->TD_NEXT;
   } /* Endwhile */
   return priority;

} /* Endbody */

/* EOF */
