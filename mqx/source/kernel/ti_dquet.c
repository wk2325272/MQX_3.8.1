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
* $FileName: ti_dquet.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for removing a task from the
*   timeout queue.
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
 * 
 * Function Name    : _time_dequeue_td
 * Returned Value   : void
 * Comments         :
 *   This function removes a task from the timeout queue.  It does not
 * reschedule the task
 *
 *END*----------------------------------------------------------------------*/

void _time_dequeue_td
   (
      /* [IN] the address of the task descriptor to be removed */
      pointer td
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          td_ptr = (TD_STRUCT_PTR)td;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_time_dequeue_td, td);

   _int_disable();
   _TIME_DEQUEUE(td_ptr, kernel_data);
   _int_enable();

   _KLOGX1(KLOG_time_dequeue_td);

} /* Endbody */

/* EOF */
