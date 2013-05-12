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
* $FileName: int_ixcp.c$
* $Version : 3.7.6.0$
* $Date    : Feb-7-2011$
*
* Comments:
*
*   This file contains the function for installing the exception handler
*   as the default ISR.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _int_install_exception_isr
* Returned Value   : _CODE_PTR_ addressof previous default ISR
* Comments         :
*   This function installs the exception isr as the default ISR.
*   The exception ISR handler performs the following service:
*
*   If an unhandled interrupt occurs and
*      a) A task is running
*         i)  If the task has an exception handler, call this handler
*         ii) Otherwise, abort the task (_task_abort)
*      b) An ISR is running
*         If the ISR has an exception handler installed, call it.
*         then remove both the exception and ISR interrupt frames.
*
*END*-------------------------------------------------------------------------*/

void (_CODE_PTR_ _int_install_exception_isr
   (
      void 
   ))(pointer)
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;

   _GET_KERNEL_DATA(kernel_data);
   kernel_data->FLAGS |= MQX_FLAGS_EXCEPTION_HANDLER_INSTALLED;
   return(_int_install_default_isr(_int_exception_isr));

}

