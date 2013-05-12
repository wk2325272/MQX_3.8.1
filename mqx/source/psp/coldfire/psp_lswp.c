/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: psp_lswp.c$
* $Version : 3.0.2.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   Load/store reversed endian functions (usually for PCI).
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_load_long_swapped
* Returned Value   : Reversed endian 32-bit value
* Comments         :
*    This function loads a 32-bit value and converts its endian byte order
*
*END*----------------------------------------------------------------------*/
uint_32 _psp_load_long_swapped
   (
      /* [IN] address to read */
      pointer p
   )
{ /* Body */

   uint_32 v = *(uint_32_ptr)p;

   return (v >> 24)
      |  ((v >> 8) & 0x00FF00)
      |  ((v << 8) & 0xFF0000)
      |   (v << 24);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_store_long_swapped
* Returned Value   : none
* Comments         :
*    This function converts a 32-bit value to the reverse endian byte
* order and then stores it in the specified memory location.
*
*END*----------------------------------------------------------------------*/
void _psp_store_long_swapped
   (
      /* [IN] address to write */
      pointer p,

      /* [IN] data to swap and store */
      uint_32 v
   )
{ /* Body */

   uint_32 t;

   t =    (v >> 24)
      |  ((v >> 8) & 0x00FF00)
      |  ((v << 8) & 0xFF0000)
      |   (v << 24);

   *(uint_32_ptr)p = t;

} /* Endbody */

/* EOF */
