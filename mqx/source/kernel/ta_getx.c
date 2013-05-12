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
* $FileName: ta_getx.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for obtaining the exception handler
*   for a task.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_HAS_EXCEPTION_HANDLER

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _task_get_exception_handler
* Returned Value   : void _CODE_PTR_
* Comments         :
*
*  This function gets the task exception handler.
*
*END*-------------------------------------------------------------------------*/

void (_CODE_PTR_ _task_get_exception_handler
   (
      /* [IN] the task id of the task whose exception handler is wanted */
      _task_id        task_id

   ))(_mqx_uint, pointer)
{ /* Body */
   TD_STRUCT_PTR           td_ptr;
   void        (_CODE_PTR_ excpt_function)(_mqx_uint, pointer);
   
   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if ( td_ptr == NULL ) {
      _task_set_error(MQX_INVALID_TASK_ID);
      return( NULL );
   }/* Endif */

   excpt_function   = td_ptr->EXCEPTION_HANDLER_PTR;

   return(excpt_function);

} /* Endbody */
#endif

/* EOF */
