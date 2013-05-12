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
* $FileName: mem_list.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function for returning the next memory block
*   on a queue of memory blocks.
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM
#include "mem_prv.h"

/*FUNCTION*-----------------------------------------------------
 * 
 * Function Name    : _mem_get_next_block_internal
 * Returned Value   : pointer
 * Comments         :
 *   This routine returns what the next memory block is for
 * a given memory block (where the memory block is on a
 * tasks resource list)
 *
 *END*--------------------------------------------------------*/

pointer _mem_get_next_block_internal
   (
     /* [IN] the task descriptor being checked */
     TD_STRUCT_PTR          td_ptr, 

     /* [IN] the address (USERS_AREA) of the memory block */
     pointer                memory_ptr
   )
{ /* Body */
   STOREBLOCK_STRUCT_PTR    block_ptr;

   if (memory_ptr == NULL) {
      return(td_ptr->MEMORY_RESOURCE_LIST);
   } else {
      block_ptr = GET_MEMBLOCK_PTR(memory_ptr);
      return(block_ptr->NEXTBLOCK);
   } /* Endif */
   
} /* Endbody */

#endif
/* EOF */
