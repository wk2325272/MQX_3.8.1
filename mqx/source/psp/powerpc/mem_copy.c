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
* $FileName: mem_copy.c$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains the functions for copying memory.
*
*END************************************************************************/

#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mem_copy
* Returned Value   : none
* Comments         :
*   This function copies the specified number of bytes from the
* source address to the destination address.  No attempt is made
* to handle overlapping copies to prevent loss of data.
*   The copying is optimized to avoid alignment problems, and attempts
* to copy 32bit numbers optimally
*
*END*----------------------------------------------------------------------*/

#if MQXCFG_MEM_COPY

void _mem_copy
   (
      /* [IN] address to copy data from */
      pointer   from,

      /* [IN] address to copy data to */
      pointer   to,

      /* [IN] number of bytes to copy */
      _mem_size number_of_bytes
   )
{ /* Body */
   register uint_32       loops;
   register uchar _PTR_   from_ptr = from;
   register uchar _PTR_   to_ptr   = to;
   register uint_32 _PTR_ to32_ptr;
   register uint_32 _PTR_ from32_ptr;

   if (number_of_bytes > 3) {

      /* Try to align source on word */
      if ((uint_32)from_ptr & 1) {
         *to_ptr++ = *from_ptr++;
         --number_of_bytes;
      } /* Endif */

      /* Try to align source on longword */
      if ((uint_32)from_ptr & 2) {
         *to_ptr++ = *from_ptr++;
         *to_ptr++ = *from_ptr++;
         number_of_bytes -= 2;
      } /* Endif */

      /* Decrement pointers to allow Power PC compiler to optimize better */
      to32_ptr   = (uint_32 _PTR_)(to_ptr - 4);
      from32_ptr = (uint_32 _PTR_)(from_ptr - 4); 

      /* 
      ** so lets copy longwords...
      ** we will unravel the copy loop to copy 64 bytes per loop
      */
      loops = (number_of_bytes >> 6) + 1;

      /* Jump in to the middle of the loop */
      switch ((number_of_bytes >> 2) & 0xF) {
         while (--loops) {  /* --loops since avoid while first time in */
                     *++to32_ptr = *++from32_ptr;
            case 15: *++to32_ptr = *++from32_ptr;
            case 14: *++to32_ptr = *++from32_ptr;
            case 13: *++to32_ptr = *++from32_ptr;
            case 12: *++to32_ptr = *++from32_ptr;
            case 11: *++to32_ptr = *++from32_ptr;
            case 10: *++to32_ptr = *++from32_ptr;
            case  9: *++to32_ptr = *++from32_ptr;
            case  8: *++to32_ptr = *++from32_ptr;
            case  7: *++to32_ptr = *++from32_ptr;
            case  6: *++to32_ptr = *++from32_ptr;
            case  5: *++to32_ptr = *++from32_ptr;
            case  4: *++to32_ptr = *++from32_ptr;
            case  3: *++to32_ptr = *++from32_ptr;
            case  2: *++to32_ptr = *++from32_ptr;
            case  1: *++to32_ptr = *++from32_ptr;
            case  0: ;
         } /* Endwhile */
      } /* Endswitch */

      /* Restore pointers */
      to_ptr   = (uchar _PTR_)to32_ptr + 4;
      from_ptr = (uchar _PTR_)from32_ptr + 4;

   } /* Endif */

   /* Copy all remaining bytes */
   if (number_of_bytes & 2) {
      *to_ptr++ = *from_ptr++;
      *to_ptr++ = *from_ptr++;
   } /* Endif */
   if (number_of_bytes & 1) {
       *to_ptr = *from_ptr;
   } /* Endif */

} /* Endbody */

#endif

/* EOF */
