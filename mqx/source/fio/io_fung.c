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
* $FileName: io_fung.c$
* $Version : 3.8.5.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the function for ungetting a character.
*
*END************************************************************************/

#include "mqx.h"
#include "fio.h"
#include "fio_prv.h"
#include "io.h"
#include "io_prv.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_fungetc
* Returned Value   : _mqx_int the char pushed back
* Comments         :
*   this function pushes back a character where it will be returned
* on the next read.  Only 1 pushback character allowed.
* The function returns the character pushed back, or IO_EOF on error.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_fungetc
   (
      /* [IN] the character to return */
      _mqx_int   character,
      
      /* [IN] the stream to return it to */
      MQX_FILE_PTR file_ptr
   )
{ /* Body */

#if MQX_CHECK_ERRORS
   if (file_ptr == NULL) {
      return(IO_EOF);
   } /* Endif */
#endif

   if (file_ptr->HAVE_UNGOT_CHARACTER) {
      return(IO_EOF);
   } /* Endif */

   if (character != IO_EOF) {
       /* Clear EOF flag */
       file_ptr->UNGOT_CHARACTER      = character;
       file_ptr->HAVE_UNGOT_CHARACTER = TRUE;
       file_ptr->FLAGS &= ~((_mqx_uint) IO_FLAG_AT_EOF);
   }
   return(character);

} /* Endbody */

/* EOF */
