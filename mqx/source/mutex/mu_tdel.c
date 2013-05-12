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
* $FileName: mu_tdel.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function called when a task is destroyed
*   so that the mutex component can release any owned resources.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MUTEXES
#include "mutex.h"
#include "mutx_prv.h"

/*FUNCTION****************************************************************
* 
* Function Name    : _mutex_cleanup
* Returned Value   : _mqx_uint MQX_EOK or POSIX error code
* Comments         :
*    Used during task destruction to free up any mutex owned by this task.
*
* 
*END**********************************************************************/

void _mutex_cleanup
   (
      /* [IN] the task being destroyed */
      TD_STRUCT_PTR td_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR     kernel_data;
   MUTEX_COMPONENT_STRUCT_PTR mutex_component_ptr;
   MUTEX_STRUCT_PTR           mutex_ptr;

   _GET_KERNEL_DATA(kernel_data);

   mutex_component_ptr = (MUTEX_COMPONENT_STRUCT_PTR)
      kernel_data->KERNEL_COMPONENTS[KERNEL_MUTEXES];
   if (mutex_component_ptr == NULL) {
      return; /* No work to do! */
   } /* Endif */

#if MQX_CHECK_VALIDITY
   if (mutex_component_ptr->VALID != MUTEX_VALID) {
      return;
   } /* Endif */
#endif

   _int_disable();
   mutex_ptr = (MUTEX_STRUCT_PTR)((pointer)mutex_component_ptr->MUTEXES.NEXT);
   while (mutex_ptr != (MUTEX_STRUCT_PTR)
      ((pointer)&mutex_component_ptr->MUTEXES))
   {
      if ((mutex_ptr->LOCK) && (mutex_ptr->OWNER_TD == td_ptr)) {
         mutex_ptr->OWNER_TD = kernel_data->ACTIVE_PTR;
         _mutex_unlock(mutex_ptr);
         mutex_ptr = (MUTEX_STRUCT_PTR)
            ((pointer)mutex_component_ptr->MUTEXES.NEXT);
      } else {
         mutex_ptr = (MUTEX_STRUCT_PTR)((pointer)mutex_ptr->LINK.NEXT);
      } /* Endif */
   } /* Endwhile */
   _int_enable();

} /* Endbody */
#endif /* MQX_USE_MUTEXES */

/* EOF */
