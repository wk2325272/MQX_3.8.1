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
* $FileName: pa_sallo.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for allocating a partition block
*   owned by the system.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_PARTITIONS
#include "part.h"
#include "part_prv.h"

/*FUNCTION*-----------------------------------------------------
* 
* Function Name    : _partition_alloc_system
* Returned Value   : pointer
* Comments         :
*    Allocates a partition block owned by the system.
*
*END*--------------------------------------------------------*/

pointer _partition_alloc_system
   (
      /* [IN] the partition from which to obtain the memory block */
      _partition_id partition
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   pointer                result;

   _GET_KERNEL_DATA(kernel_data);

   _KLOGE2(KLOG_partition_alloc_system, partition);

   result = _partition_alloc_internal((PARTPOOL_STRUCT_PTR)partition, 
      SYSTEM_TD_PTR(kernel_data));

   _KLOGX2(KLOG_partition_alloc_system, result);
   return (result);

} /* Endbody */
#endif /* MQX_USE_PARTITIONS */

/* EOF */
