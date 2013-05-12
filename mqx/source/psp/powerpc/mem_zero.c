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
* $FileName: mem_zero.c$
* $Version : 3.8.5.1$
* $Date    : May-21-2012$
*
* Comments:
*
*   This file contains the functions for zeroing memory
*
*END************************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mem_zero
* Returned Value   : none
* Comments         :
*   This function zeros the specified number of bytes at the specified
* location.
*   The zeroing is optimized to avoid alignment problems, and attempts
* to zero 32bit numbers optimally
*
*END*----------------------------------------------------------------------*/

#if MQX_USE_MEM_ZERO

/* Make compiler optimize better */
uint_32 LOAD_ZERO(void) {return 0;}

void _mem_zero
   (
      /* [IN] the address to start zeroing memory from */
      pointer   loc,

      /* [IN] the number of bytes to zero */
      _mem_size number_of_bytes
   )
{ /* Body */
#if MQX_USE_SMALL_MEM_ZERO
   if (number_of_bytes) {
      while (number_of_bytes--) {
         *((uchar_ptr)loc)++ = 0;
      } /* Endwhile */      
   } /* Endif */
#else
   register uint_32 loops;
   register uchar _PTR_   b_ptr = loc;
   register uint_32 _PTR_ b32_ptr;
   register uint_32       zero  = LOAD_ZERO();

   if (number_of_bytes > 3) {

      /* Align source on word */
      if ( (uint_32)b_ptr & 1 ) {
         *b_ptr++ = zero;
         --number_of_bytes;
      } /* Endif */

      /* Align source on longword */
      if ( (uint_32)b_ptr & 2 ) {
         *b_ptr++ = zero;
         *b_ptr++ = zero;
         number_of_bytes -= 2;
      } /* Endif */

      /* Decrement pointer so can optimize for PowerPc assembler generation */
      b32_ptr = (uint_32 _PTR_)(b_ptr - 4);

      /* Get number of long words
      ** we will unravel the zero loop to zero 64 bytes per loop
      */
      loops  = (number_of_bytes >> 6) + 1;
      switch ((number_of_bytes >> 2) & 0xF) {
         while (--loops) {  /* --loops since avoid while first time in */
                     *++b32_ptr = zero;
            case 15: *++b32_ptr = zero;
            case 14: *++b32_ptr = zero;
            case 13: *++b32_ptr = zero;
            case 12: *++b32_ptr = zero;
            case 11: *++b32_ptr = zero;
            case 10: *++b32_ptr = zero;
            case  9: *++b32_ptr = zero;
            case  8: *++b32_ptr = zero;
            case  7: *++b32_ptr = zero;
            case  6: *++b32_ptr = zero;
            case  5: *++b32_ptr = zero;
            case  4: *++b32_ptr = zero;
            case  3: *++b32_ptr = zero;
            case  2: *++b32_ptr = zero;
            case  1: *++b32_ptr = zero;
            case  0: ;
         } /* Endwhile */
      } /* Endswitch */
      b_ptr = (uchar _PTR_)b32_ptr + 4;

   }/* Endif */

   /* Handle leftover bytes */
   if ( number_of_bytes & 2 ) {
      *b_ptr++ = zero;
      *b_ptr++ = zero;
   }/* Endif */
   if ( number_of_bytes & 1 ) {
      *b_ptr = zero;
   }/* Endif */
#endif
} /* Endbody */

#endif
/* EOF */
