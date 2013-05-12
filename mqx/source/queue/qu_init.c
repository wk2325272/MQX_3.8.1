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
* $FileName: qu_init.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for initializing a queue.
*   Note that the QUEUE macros can be found in mqx_prv.h
*
*END************************************************************************/

/* Start CR 312 */
#define MQX_FORCE_USE_INLINE_MACROS 1
/* End CR 312 */

#include "mqx_inc.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _queue_init
* Returned Value   : none
* Comments         :
*    initialize the provided queue
*
*END*--------------------------------------------------------*/

void _queue_init
   (
      /* [IN] the queue to initialize */
      QUEUE_STRUCT_PTR q_ptr,

      /* [IN] the maximum size of the queue (0 is infinite) */
      uint_16 size
   )
{ /* Body */

   _QUEUE_INIT(q_ptr, size);

} /* Endbody */

/* EOF */
