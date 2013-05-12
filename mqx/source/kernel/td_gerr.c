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
* $FileName: td_gerr.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for retuning the task error
*   code for the current task.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_TD_HAS_ERROR_CODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_error
* Returned Value   : _mqx_uint
* Comments         :
*    Returns the current task error code of the calling (active) task.
* 
*END*----------------------------------------------------------------------*/
_mqx_uint _task_get_error
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   if (kernel_data->IN_ISR) {
      return( kernel_data->INTERRUPT_CONTEXT_PTR->ERROR_CODE );
   } else {
      return( kernel_data->ACTIVE_PTR->TASK_ERROR_CODE );
   } /* Endif */

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_error_ptr
* Returned Value   : _mqx_uint _PTR_
* Comments         :
*    Returns the address of the task error code
* 
*END*----------------------------------------------------------------------*/

_mqx_uint _PTR_ _task_get_error_ptr
   (
      void
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);
   if (kernel_data->IN_ISR) {
      return( &kernel_data->INTERRUPT_CONTEXT_PTR->ERROR_CODE );
   } else {
      return( &kernel_data->ACTIVE_PTR->TASK_ERROR_CODE );
   } /* Endif */      

} /* Endbody */
#endif
/* EOF */
