/**HEADER********************************************************************
* 
* Copyright (c) 2008-2009 Freescale Semiconductor;
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
* $FileName: powerpc.c$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains the function that provide generic utilities
*   for use with any PowerPC processor.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _psp_set_spr
* Returned Value   : none
* Comments         :
*  This function sets a PowerPC Special Purpose Resister value.
*
************************************************************************/

void _psp_set_spr
   (
      /* [IN] the spr register to modify */
      uint_32 spr_register,

      /* [IN] the new value for the spr register */
      uint_32 spr_value
   )
{ /* Body */
   uint_32 ramcode[5];

   ramcode[0] = 0x7C0004AC;   /* sync */
   ramcode[1] = 0x7C0003A6 | (3 << 21) | ((spr_register & 0x1F) << 16) |
      ((spr_register & 0x3E0) << 6);
   ramcode[2] = 0x7C0004AC;  /* sync */
   ramcode[3] = 0x4C00012C;  /* isync */
   ramcode[4] = 0x4E800020;  /* blr */

   _dcache_flush_line(ramcode);
   _icache_invalidate_line(ramcode);
   _dcache_flush_line(&ramcode[4]);
   _icache_invalidate_line(&ramcode[4]);

   ((void(_CODE_PTR_)(uint_32)) ramcode)(spr_value);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _psp_get_spr
* Returned Value   : none
* Comments         :
*  This function gets the value of a PowerPC Special Purpose Resister;
*
************************************************************************/

uint_32 _psp_get_spr
   (
      /* [IN] the spr register to read */
      uint_32 spr_register
   )
{ /* Body */
   uint_32 ramcode[5];

   ramcode[0] = 0x7C0004AC;   /* sync */
   ramcode[1] = 0x7C0002A6 | (3 << 21) | ((spr_register & 0x1F) << 16) | 
      ((spr_register & 0x3E0) << 6);
   ramcode[2] = 0x7C0004AC;   /* sync */
   ramcode[3] = 0x4C00012C;   /* isync */
   ramcode[4] = 0x4E800020;   /* blr */

   _dcache_flush_line(ramcode);
   _icache_invalidate_line(ramcode);
   _dcache_flush_line(&ramcode[4]);
   _icache_invalidate_line(&ramcode[4]);

   return ( ((uint_32(_PTR_)(void)) ramcode)() );

} /* Endbody */

/* EOF */
