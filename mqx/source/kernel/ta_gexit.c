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
* $FileName: ta_gexit.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for obtaining the exit handler
*   for a task.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_EXIT_HANDLER
/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _task_get_exit_handler
* Returned Value   : void _CODE_PTR_
* Comments         :
*
*  This function gets the task exit handler.
*
*END*-------------------------------------------------------------------------*/

void (_CODE_PTR_ _task_get_exit_handler
   (
      /* [IN] the task id for the task whose exit handler is desired */
      _task_id        task_id
   ))(void)
{ /* Body */
   TD_STRUCT_PTR          td_ptr;
   void     (_CODE_PTR_   exit_function)(void);
   
   td_ptr = (TD_STRUCT_PTR)_task_get_td(task_id);
   if ( td_ptr == NULL ) {
      _task_set_error(MQX_INVALID_TASK_ID);
      return( NULL );
   }/* Endif */

   exit_function = td_ptr->EXIT_HANDLER_PTR;

   return(exit_function);

} /* Endbody */
#endif

/* EOF */
