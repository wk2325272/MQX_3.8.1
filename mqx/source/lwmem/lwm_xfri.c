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
* $FileName: lwm_xfri.c$
* $Version : 3.0.5.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function for transferring memory between
*   tasks.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
 * 
 * Function Name    : _lwmem_transfer_internal
 * Returned Value   : _mqx_uint MQX_OK or an MQX error code
 * Comments         :
 *   This routine transfers the ownership of a block of memory from
 *   an owner task to another task.  
 *
 *END*--------------------------------------------------------*/

void _lwmem_transfer_internal
   (
     /* [IN] the address of the USER_AREA in the memory block to transfer */
     pointer       memory_ptr,

     /* [IN] the target task descriptor */
     TD_STRUCT_PTR target_td
   )
{ /* Body */
   LWMEM_BLOCK_STRUCT_PTR  block_ptr;

   /* Verify the block */
   block_ptr = GET_LWMEMBLOCK_PTR(memory_ptr);
   block_ptr->U.S.TASK_NUMBER = TASK_NUMBER_FROM_TASKID(target_td->TASK_ID);
 
} /* Endbody */
#endif /* MQX_USE_LWMEM */

/* EOF */
