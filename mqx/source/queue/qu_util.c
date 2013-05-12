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
* $FileName: qu_util.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains functions for manipulating the mqx queues.
*   Note that the QUEUE macros can be found in mqx_prv.h
*
*END************************************************************************/

/* Start CR 312 */
#define MQX_FORCE_USE_INLINE_MACROS 1
/* End CR 312 */

#include "mqx_inc.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_get_size
* Returned Value   : _mqx_uint - The size of the queue
* Comments         :
*    Returns the size of the queue
*
*END*--------------------------------------------------------*/

_mqx_uint _queue_get_size
   (
      /* [IN] the queue to obtain information about */
      QUEUE_STRUCT_PTR q_ptr
   )
{ /* Body */

   return _QUEUE_GET_SIZE(q_ptr);

} /* Endbody */


/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_is_empty
* Returned Value   : boolean - TRUE if queue is empty
* Comments         :
*    Returns whether the queue is empty or not
*
*END*--------------------------------------------------------*/

boolean _queue_is_empty
   (
      /* [IN] the queue to obtain information about */
      QUEUE_STRUCT_PTR q_ptr 
   )
{ /* Body */

   return _QUEUE_IS_EMPTY(q_ptr);

} /* Endbody */

/* EOF */
