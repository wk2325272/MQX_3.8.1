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
* $Version : 3.5.7.0$
* $Date    : Nov-26-2009$
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
      pointer  from_ptr,

      /* [IN] address to copy data to */
      pointer  to_ptr,

      /* [IN] number of bytes to copy */
      register _mem_size number_of_bytes
   )
{ /* Body */
#if MQX_USE_SMALL_MEM_COPY
   register uint_8_ptr from8_ptr = (uint_8_ptr) from_ptr;
   register uint_8_ptr to8_ptr = (uint_8_ptr) to_ptr;

   if (number_of_bytes) {
      while (number_of_bytes--) {
         *to8_ptr++ = *from8_ptr++;
      } /* Endwhile */      
   } /* Endif */
#else
   uint_8_ptr from8_ptr = (uint_8_ptr) from_ptr;
   uint_8_ptr to8_ptr = (uint_8_ptr) to_ptr;
   uint_16_ptr from16_ptr = (uint_16_ptr) from_ptr;
   uint_16_ptr to16_ptr = (uint_16_ptr) to_ptr;
   register uint_32_ptr from32_ptr = (uint_32_ptr) from_ptr;
   register uint_32_ptr to32_ptr = (uint_32_ptr) to_ptr;
   register uint_32 loops;

   if (number_of_bytes > 3) {
      /* Try to align source on word */
      if ((uint_32)from_ptr & 1) {
         from8_ptr = (uint_8_ptr) from_ptr;
         to8_ptr = (uint_8_ptr) to_ptr;

         *to8_ptr++ = *from8_ptr++;

         from_ptr = from8_ptr;
         to_ptr = to8_ptr;
         --number_of_bytes;
      } /* Endif */

      /* Try to align source on longword */
      if ((uint_32)from_ptr & 2) {
         from16_ptr = (uint_16_ptr) from_ptr;
         to16_ptr = (uint_16_ptr) to_ptr;

         *to16_ptr++ = *from16_ptr++;

         from_ptr = from16_ptr;
         to_ptr = to16_ptr;
         number_of_bytes -= 2;
      } /* Endif */

      from32_ptr = (uint_32_ptr) from_ptr;
      to32_ptr = (uint_32_ptr) to_ptr;
#if MQX_USE_BLOCK_MEM_COPY
      /* 
      ** To increase performance a bit, we will copy 64 bytes (16 * longwords) sequentially
      ** This consumes more RAM, more flash, but gets less instruction cycles.
      */
      for (loops = (number_of_bytes >> 6); loops > 0; loops--) {
         /* copy 16 longwords */
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
         *to32_ptr++ = *from32_ptr++;
      } /* Endwhile */

      /* Now, write the rest of bytes */
      switch ((number_of_bytes >> 2) & 0xF) {
         case 15: *to32_ptr++ = *from32_ptr++;
         case 14: *to32_ptr++ = *from32_ptr++;
         case 13: *to32_ptr++ = *from32_ptr++;
         case 12: *to32_ptr++ = *from32_ptr++;
         case 11: *to32_ptr++ = *from32_ptr++;
         case 10: *to32_ptr++ = *from32_ptr++;
         case 9:  *to32_ptr++ = *from32_ptr++;
         case 8:  *to32_ptr++ = *from32_ptr++;
         case 7:  *to32_ptr++ = *from32_ptr++;
         case 6:  *to32_ptr++ = *from32_ptr++;
         case 5:  *to32_ptr++ = *from32_ptr++;
         case 4:  *to32_ptr++ = *from32_ptr++;
         case 3:  *to32_ptr++ = *from32_ptr++;
         case 2:  *to32_ptr++ = *from32_ptr++;
         case 1:  *to32_ptr++ = *from32_ptr++;
      } /* Endswitch */

#else /* MQX_USE_BLOCK_MEM_COPY */
      for (loops = number_of_bytes >> 2; loops != 0; loops--) {
         *to32_ptr++ = *from32_ptr++;
      }
#endif /* MQX_USE_BLOCK_MEM_COPY */
      from_ptr = from32_ptr;
      to_ptr = to32_ptr;
   } /* Endif */

   /* Copy all remaining bytes */
   if (number_of_bytes & 2) {
      from16_ptr = (uint_16_ptr) from_ptr;
      to16_ptr = (uint_16_ptr) to_ptr;

      *to16_ptr++ = *from16_ptr++;

      from_ptr = from16_ptr;
      to_ptr = to16_ptr;
   } /* Endif */

   if (number_of_bytes & 1) {
      * (uint_8_ptr) to_ptr = * (uint_8_ptr) from_ptr;
   } /* Endif */
#endif
} /* Endbody */

#endif

/* EOF */
