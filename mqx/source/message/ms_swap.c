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
* $FileName: ms_swap.c$
* $Version : 3.8.5.0$
* $Date    : Sep-12-2011$
*
* Comments:
*
*   This file contains the functions for manipulating the 
*   message queues, and queue ids.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_MESSAGES
#include "message.h"
#include "msg_prv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msg_swap_endian_header
* Returned Value  : None
* Comments        : convert a data messages header from intel to motorola
*   byte order ( or vice versa ).
*
*END*------------------------------------------------------------------*/

static const uchar _KRNL_Hdr_def[] = {
  sizeof(_msg_size),
#if MQX_USE_32BIT_MESSAGE_QIDS
  sizeof(uint_16),
#endif
  sizeof(_queue_id), 
  sizeof(_queue_id), 
  0 
};

void _msg_swap_endian_header
   (
      /* [IN] the message whose header is to be byte swapped */
      register MESSAGE_HEADER_STRUCT_PTR message_ptr
   )
{ /* Body */

   _mem_swap_endian( (uchar _PTR_)_KRNL_Hdr_def, (pointer)&(message_ptr->SIZE));
   message_ptr->CONTROL &= ~MSG_HDR_ENDIAN_MASK;
   message_ptr->CONTROL |= MSG_HDR_ENDIAN;

} /* Endbody */


/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _msg_swap_endian_data
* Returned Value  : None
* Comments        : convert a data messages data from intel to motorola
*   byte order ( or vice versa ).
*   The size of the fields in the data are defined by 
*   the null terminated array of 8 bit numbers.  
*
*END*------------------------------------------------------------------*/

void _msg_swap_endian_data
   (
      /* [IN] the address of a entry size array
      ** The array indicates the sizes of consecutive fields in the
      ** data, in bytes.
      */
      uchar _PTR_               definition,

      /* [IN] the message whose data is to be byte swapped */
      register MESSAGE_HEADER_STRUCT_PTR message_ptr
   )
{ /* Body */

   _mem_swap_endian( definition, (pointer)((uchar _PTR_)message_ptr +
      sizeof(MESSAGE_HEADER_STRUCT)) );
   message_ptr->CONTROL &= ~MSG_DATA_ENDIAN_MASK;
   message_ptr->CONTROL |=  MSG_DATA_ENDIAN;

} /* Endbody */
#endif /* MQX_USE_MESSAGES */

/* EOF */
