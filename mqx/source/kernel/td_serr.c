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
* $FileName: td_serr.c$
* $Version : 3.8.5.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the function for setting the task error code
*   for the current task.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_set_error
* Returned Value   : _mqx_uint
*                  :   The old value of the error code
* Comments         :
*    This function sets the task error code.  If the task error code does
*    not equal MQX_OK, then the value is not changed.  However, the value
*    of the task error code may be reset to MQX_OK by setting the task
*    error code to MQX_OK.  The old value of the task error code is 
*    returned.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _task_set_error
   (
      /* [IN] the new error code */
      _mqx_uint new_error_code
   )
{ /* Body */
#if MQX_TD_HAS_ERROR_CODE
   register KERNEL_DATA_STRUCT_PTR  kernel_data;
   register _mqx_uint                old_error_code;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_task_set_error, new_error_code);

   /* get the old value */
   if (kernel_data->IN_ISR) {
      old_error_code = kernel_data->INTERRUPT_CONTEXT_PTR->ERROR_CODE;
   } else {
      old_error_code = kernel_data->ACTIVE_PTR->TASK_ERROR_CODE;
   } /* Endif */

   if (( new_error_code == MQX_OK ) || ( old_error_code == MQX_OK )) {
      if (kernel_data->IN_ISR) {
         kernel_data->INTERRUPT_CONTEXT_PTR->ERROR_CODE = new_error_code;
      } else {
         kernel_data->ACTIVE_PTR->TASK_ERROR_CODE = new_error_code;
      } /* Endif */
   } /* Endif */

   _KLOGX2(KLOG_task_set_error, old_error_code);
   return( old_error_code );
#else
   return MQX_OK;
#endif

}

#if MQX_ENABLE_USER_MODE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_set_error
* Returned Value   : _mqx_uint
*                  :   The old value of the error code
* Comments         :
*    This function sets the task error code.  If the task error code does
*    not equal MQX_OK, then the value is not changed.  However, the value
*    of the task error code may be reset to MQX_OK by setting the task
*    error code to MQX_OK.  The old value of the task error code is 
*    returned.
*
*END*----------------------------------------------------------------------*/

_task_id _usr_task_set_error
    (
        /* [IN] the new error code */
        _mqx_uint new_error_code
    )
{
    MQX_API_CALL_PARAMS params = {(uint_32)new_error_code, 0, 0, 0, 0};
    return _mqx_api_call(MQX_API_TASK_SET_ERROR, &params);
}

#endif // MQX_ENABLE_USER_MODE
