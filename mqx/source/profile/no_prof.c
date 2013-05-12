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
* $FileName: no_prof.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   Dummy profiling functions for compilers that don't support
*   profiling
*
*END************************************************************************/

#include "mqx_inc.h"
#include "profprv.h"


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_initialize_context
* Returned Value   : 
* Comments         : Initializes the fields in PROFILE_CONTEXT_STRUCT
*
*END*-------------------------------------------------------------------------*/

void _profiler_initialize_context
   (
      /* [IN] The profile context for this task */
      PROFILE_CONTEXT_STRUCT_PTR    context_ptr
   )
{ /* Body */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_save_context
* Returned Value   : 
* Comments         : 
*
*END*-------------------------------------------------------------------------*/

void _profiler_save_context
   (
      void 
   )
{ /* Body */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_restore_context
* Returned Value   : 
* Comments         : 
*
*END*-------------------------------------------------------------------------*/

void _profiler_restore_context
   (
      void 
   )
{ /* Body */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _profiler_finish
* Returned Value   : 
* Comments         : 
*
*END*-------------------------------------------------------------------------*/

void _profiler_finish
   (
      /* [IN] The profile context for this task */
      PROFILE_CONTEXT_STRUCT_PTR    context_ptr
   )
{ /* Body */

} /* Endbody */

/* EOF */

