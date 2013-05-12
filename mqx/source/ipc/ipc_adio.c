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
* $FileName: ipc_adio.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the functions for the ipc task.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_IPC
#include "message.h"
#include "ipc.h"
#include "ipc_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _ipc_add_io_ipc_handler
* Returned Value   : _mqx_uint an mqx task error code
* Comments         :
*   this function adds an IPC io component handler.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _ipc_add_io_ipc_handler
   (
      /* [IN] The handler for the component */
      _mqx_uint (_CODE_PTR_ handler)(pointer),

      /* [IN] The component number */
      _mqx_uint             component
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   IPC_COMPONENT_STRUCT_PTR   ipc_component_ptr;

   _GET_KERNEL_DATA(kernel_data);

   ipc_component_ptr = kernel_data->IPC_COMPONENT_PTR;

#if MQX_CHECK_ERRORS
   if (!ipc_component_ptr) {
      return MQX_IPC_SERVICE_NOT_AVAILABLE;
   } /* Endif */
#endif

   ipc_component_ptr->IPC_IO_COMPONENT_HANDLER[component] =
      (_mqx_uint (_CODE_PTR_)(IPC_MESSAGE_STRUCT_PTR))handler;
      
   return MQX_OK;

} /* Endif */
#endif /* MQX_USE_IPC */

/* EOF */
