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
* $FileName: sc_gprio.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for obtaining priority
*   maximum and minimums for the running kernel.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_get_max_priority
* Returned Value   : _mqx_uint 0
* Comments         :
*   This function always returns 0, the highest priority a task may have
* under MQX.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_get_max_priority
   /* ARGS USED */
   (
      /* [IN] - not used, all task priorities same for RR or FIFO */
      _mqx_uint policy
   )
{ /* Body */

   return( 0 );
   
} /* Endbody */
   

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _sched_get_min_priority
* Returned Value   : _mqx_uint lowest_task_priority
* Comments         :
*   This function returns the priority associated with the lowest priority
* task in the system (the priority of the _Idle Task - 1)
*
*END*----------------------------------------------------------------------*/

_mqx_uint _sched_get_min_priority
   (
      /* [IN] - not used, all task priorities same for RR or FIFO */
      _mqx_uint policy
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);
   
   return( kernel_data->LOWEST_TASK_PRIORITY );
  
} /* Endbody */

/* EOF */
