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
* $FileName: td_util.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the functions for checking the stack
*   and returing the processor number of a task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_check_stack
* Returned Value   : boolean
* Comments         : 
*    This function returns TRUE if stack is CURRENTLY overflowed.
*
*END*----------------------------------------------------------------------*/

boolean _task_check_stack
   (
      void
   )
{ /* Body */
#if MQX_TD_HAS_STACK_LIMIT
   register  KERNEL_DATA_STRUCT_PTR kernel_data;
   volatile  char                   loc = (char)0;

   _GET_KERNEL_DATA(kernel_data);

#if PSP_STACK_GROWS_TO_LOWER_MEM
   if ( &loc < ((char _PTR_)kernel_data->ACTIVE_PTR->STACK_LIMIT) ) {
#else
   if ( &loc > ((char _PTR_)kernel_data->ACTIVE_PTR->STACK_LIMIT) ) {
#endif
      return (TRUE);
   } else {
      return (FALSE);
   } /* Endif */
#else
    return (FALSE);
#endif
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_processor
* Returned Value   : _processor_number
* Comments         :
*    This function returns the processor number upon which the given
*    task id exists. 
*
*END*----------------------------------------------------------------------*/

_processor_number _task_get_processor
   (
      /* [IN] the task id whose processor number is required */
      _task_id task_id
   )
{ /* Body */

   return PROC_NUMBER_FROM_TASKID(task_id);

} /* Endbody */

/* EOF */
