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
* $FileName: ta_util2.c$
* $Version : 3.5.5.0$
* $Date    : Jan-8-2010$
*
* Comments:
*
*   This file contains more utility functions dealing with tasks.
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_TD_HAS_TASK_TEMPLATE_PTR || MQX_TD_HAS_TEMPLATE_INDEX
/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _task_get_index_from_id
* Returned Value   : task index (or 0 if not found)
* Comments         :
*
*END*----------------------------------------------------------------------*/

_mqx_uint _task_get_index_from_id
   (
      /* [IN] the task Id to look up */
      _task_id  taskid
   )
{ /* Body */
   TD_STRUCT_PTR           td_ptr;

   td_ptr = _task_get_td(taskid);

   if (td_ptr == NULL) {
      return 0;
   } /* Endif */

#if MQX_TD_HAS_TASK_TEMPLATE_PTR
   return td_ptr->TASK_TEMPLATE_PTR->TASK_TEMPLATE_INDEX;
#elif MQX_TD_HAS_TEMPLATE_INDEX
   return td_ptr->TEMPLATE_INDEX;
#else
   return 0;   
#endif   

} /* Endbody */
#endif
