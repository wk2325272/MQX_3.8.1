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
* $FileName: ta_rtos.c$
* $Version : 3.8.4.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function which reserves extra memory for
*   each task that gets created.
*
*END************************************************************************/


#include "mqx_inc.h"

#if MQX_EXTRA_TASK_STACK_ENABLE
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_reserve_space
* Returned Value   : Byte offset from reserved area base address
* Comments         : 
*   This function reserves 'size' bytes that will be added to the size
* of every task's stack.  It can be thought of as a way of allocating
* "task local storage" or perhaps as a way of dynamically adding to the
* TD_STRUCT.  Both "OS Changer" and the MetaWare C/C++ runtime want
* additional per-task variables.  However, this is a general feature that
* can be used for many more purposes.
*
*     NOTE: The space added to each task must be known before ANY
*           tasks are created.
*
* See also ta_gtos.c and td_alloc.c.
*
*END*----------------------------------------------------------------------*/

/* Start CR 1124: C runtime thread local storage */
_mqx_uint _task_reserve_space
   (
      /* [IN} Amount of space to reserve */
      _mqx_uint size
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   _mqx_uint off, algn;

   _GET_KERNEL_DATA(kernel_data);

#if MQX_CHECK_ERRORS
   /* Can only be called prior to creating any tasks! */
   if (_QUEUE_GET_SIZE(&kernel_data->TD_LIST) != 0) {
      _mqx_fatal_error(MQX_EINVAL);
   } /* Endif */
#endif

   if      (size > 7) algn = 7;
   else if (size > 3) algn = 3;
   else if (size > 1) algn = 1;
   else               algn = 0;

   if (algn > kernel_data->TOS_RESERVED_ALIGN_MASK) {
      kernel_data->TOS_RESERVED_ALIGN_MASK = algn;
   } /* Endif */

   off = (kernel_data->TOS_RESERVED_SIZE + algn) & ~algn;
   kernel_data->TOS_RESERVED_SIZE = off + size;

   return off;

} /* Endbody */
/* End   CR 1124 */
#endif /* MQX_EXTRA_TASK_STACK_ENABLE */

/* EOF */
