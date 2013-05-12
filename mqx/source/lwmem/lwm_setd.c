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
* $FileName: lwm_setd.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   Thie file contains the function for setting the default pool id.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
 * 
 * Function Name    : _lwmem_set_default_pool
 * Returned Value   : _lwmem_pool_id the old value.
 * Comments         :
 *   This routine sets the value of the default MQX light weight
 *   memory pool.
 *
 *END*--------------------------------------------------------*/

_lwmem_pool_id _lwmem_set_default_pool
   (  
      /* [IN] the source (owner) task id */
      _lwmem_pool_id pool_id
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   _lwmem_pool_id         old_pool_id;


   _GET_KERNEL_DATA(kernel_data);

   old_pool_id = kernel_data->KERNEL_LWMEM_POOL;
   kernel_data->KERNEL_LWMEM_POOL = pool_id;
   return(old_pool_id);

} /* Endbody */
#endif /* MQX_USE_LWMEM */

/* EOF */
