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
* $FileName: lwm_init.c$
* $Version : 3.8.12.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the function that initializes a default lwmemory
*   pool for use by MQX.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWMEM
#include "lwmem.h"
#include "lwmemprv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _lwmem_init_internal
* Returned Value   : an MQX error code
* Comments         : initializes MQX to use the lwmem manager
*
*END*---------------------------------------------------------*/

_mqx_uint _lwmem_init_internal
   (
      void
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR  kernel_data;
   LWMEM_POOL_STRUCT_PTR   lwmem_pool_ptr;
   uchar_ptr               start;

   _GET_KERNEL_DATA(kernel_data);


   /*
   ** Move the MQX memory pool pointer past the end of kernel data.
   */
   start = (pointer)((uchar_ptr)kernel_data + 
      sizeof(KERNEL_DATA_STRUCT));
   lwmem_pool_ptr = (LWMEM_POOL_STRUCT_PTR)start;
   kernel_data->KERNEL_LWMEM_POOL = lwmem_pool_ptr;

   start = (pointer)((uchar_ptr)start + sizeof(LWMEM_POOL_STRUCT));

   _lwmem_create_pool(lwmem_pool_ptr, start, (uchar_ptr)kernel_data->INIT.END_OF_KERNEL_MEMORY - (uchar_ptr)start);
   
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LWMEM */

/* EOF */
