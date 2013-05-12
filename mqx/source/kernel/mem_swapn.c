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
* $FileName: mem_swapn.c$
* $Version : 3.0.6.0$
* $Date    : Apr-14-2009$
*
* Comments:
*
*   This file contains the functions for performing endian
*   swapping of memory fields. This version is length limited
*
*END************************************************************************/

#define __MEMORY_MANAGER_COMPILE__
#include "mqx_inc.h"
#if MQX_USE_MEM || MQX_USE_LWMEM
#include "mem_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _mem_swap_endian_len
* Returned Value  : None
* Comments        : convert data from Big to Little Endian
*   byte order ( or vice versa ).
*   The size of the fields in the data are defined by 
*   the null terminated array of 8 bit numbers.
*
*END*------------------------------------------------------------------*/

void _mem_swap_endian_len
   (
      /* 
      ** [IN] the address of a entry size array
      ** The array indicates the sizes of consecutive fields in the
      ** data, in bytes.
      */
      register uchar _PTR_ definition,

      /* [IN] the address of the data to modify */
      pointer              data,

      /* [IN] the fields in the definition array to process */
      _mqx_uint            len
      
   )
{ /* Body */
   register uchar _PTR_ data_ptr;
   register uchar _PTR_ next_ptr;
            uchar _PTR_ b_ptr;
            _mqx_uint   i;
   register _mqx_uint   size;
            uchar       c;

   data_ptr = (uchar _PTR_)data;
   size     = (_mqx_uint)*definition++;
   while ( size && len-- ) {
      switch ( size ) {
         case 0:  /* For compiler optimizations */
            break;
         case 1:  /* No need to swap */
            ++data_ptr;
            break;
         /* Cases 2 & 4 are common sizes */
         case 2:  
            c = data_ptr[0];
            data_ptr[0] = data_ptr[1];
            data_ptr[1] = c;
            data_ptr += 2;
            break;
         case 4:
            c = data_ptr[0];
            data_ptr[0] = data_ptr[3];
            data_ptr[3] = c;
            c = data_ptr[1];
            data_ptr[1] = data_ptr[2];
            data_ptr[2] = c;
            data_ptr += 4;
            break;
         /* All others done long hand */
         default: 
            next_ptr = data_ptr+size;
            b_ptr = data_ptr+size-1;
            i = (size/2) + 1;
            while ( --i ) {
               c = *data_ptr;
               *data_ptr++ = *b_ptr;
               *b_ptr-- = c;
            } /* Endwhile */
            data_ptr = next_ptr;
            break;
      } /* Endswitch */
      size = (_mqx_uint)*definition++;
   } /* Endwhile */

} /* Endbody */

#endif
/* EOF */
