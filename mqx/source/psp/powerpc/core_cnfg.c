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
* $FileName: core_cnfg.c$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the core configuration utility functions.
*
*END*********************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_core_peripheral_enabled
* Returned Value   : pointer to peripheral
* Comments         :
*    This function returns the supplied address if the peripheral is enabled
* on this core
*
*END*----------------------------------------------------------------------*/
boolean _psp_core_peripheral_enabled(uint_32 config)
{
    return (config & (1<<_psp_core_num()));
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_core_peripheral_address
* Returned Value   : pointer to peripheral
* Comments         :
*    This function returns the supplied address if the peripheral is enabled
* on this core
*
*END*----------------------------------------------------------------------*/
pointer _psp_core_peripheral_address(uint_32 config, pointer address)
{
    return (config & (1<<_psp_core_num())) ? address : NULL;
}
