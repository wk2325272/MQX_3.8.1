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
* $FileName: mqx_stad.c$
* $Version : 3.0.4.0$
* $Date    : Mar-5-2009$
*
* Comments:
*
*   This file contains the function for returning TAD_RESERVED field
*
*END************************************************************************/


#include "mqx_inc.h"
#if  MQX_TAD_RESERVED_ENABLE

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mqx_set_tad_data
* Returned Value   : none
* Comments         : 
*   This function sets the value of the TAD_RESERVED field.
*
*END*----------------------------------------------------------------------*/

void _mqx_set_tad_data
   (
      /* [IN] the TD address */
      pointer  td,

      /* [IN] the tad data */
      pointer  tad_data
   )
{ /* Body */
   TD_STRUCT_PTR td_ptr = (TD_STRUCT_PTR)td;

   td_ptr->TAD_RESERVED = tad_data;

} /* Endbody */
#endif
/* EOF */
