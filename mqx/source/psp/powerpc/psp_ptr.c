/**HEADER********************************************************************
*
* Copyright (c) 2011 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2011 Embedded Access Inc.;
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
* $FileName: psp_ptr.c$
* $Version : 3.8.1.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains functions for setting * getting the PSP support pointer.
*
*END*********************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_get_support_ptr
* Returned Value   : psp support ptr
* Comments         :
*  get the psp support ptr
*
*END*------------------------------------------------------------------------*/
PSP_SUPPORT_STRUCT_PTR _psp_get_support_ptr(void)
{
    KERNEL_DATA_STRUCT_PTR       kernel_data;
    _GET_KERNEL_DATA(kernel_data);

    return kernel_data->PSP_SUPPORT_PTR;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_set_support_ptr
* Returned Value   : none
* Comments         :
* get the psp support ptr
*
*END*------------------------------------------------------------------------*/
void _psp_set_support_ptr(PSP_SUPPORT_STRUCT_PTR       support_ptr)
{
    KERNEL_DATA_STRUCT_PTR       kernel_data;

    _GET_KERNEL_DATA(kernel_data);
    kernel_data->PSP_SUPPORT_PTR = support_ptr;
}
