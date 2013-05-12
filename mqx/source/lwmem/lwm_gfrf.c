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
* $FileName: lwm_gfrf.c$
* $Version : 3.5.1.0$
* $Date    : Jan-13-2010$
*
* Comments:
*
*   This file contains the function that gets free size from pool.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_get_free_from
* Returned Value   : _mem_size - size of unallocated (free) memory
* Comments         : gets the size of memory from a specified pool
*
*END*---------------------------------------------------------*/

_mem_size  _lwmem_get_free_from
   (
      /* [IN] the pool to get free size from */
      pointer    pool_id
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   _mem_size               result;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_lwmem_get_free_from, pool_id);

   result = _lwmem_get_free_internal((_lwmem_pool_id)pool_id);

   _KLOGX2(KLOG_lwmem_get_free_from, result);
   return(result);

} /* Endbody */

#endif /* MQX_USE_LWMEM */

/* EOF */
