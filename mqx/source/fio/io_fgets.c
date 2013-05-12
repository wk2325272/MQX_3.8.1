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
* $FileName: io_fgets.c$
* $Version : 3.6.5.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the function for getting a string.
*
*END************************************************************************/

#include "mqx.h"
#include "fio.h"
#include "fio_prv.h"
#include "io.h"
#include "io_prv.h"

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_fgets
* Returned Value   : char *
* Comments         :
*    This function reads at most the next size-1 characters into the array
*   tty_line_ptr, stopping if a newline is encountered; The newline is 
*   included in the array, which is terminated by '\0'.
*   This function returns tty_line_ptr, or NULL (if end of file or error).
*
*END*----------------------------------------------------------------------*/

char _PTR_ _io_fgets
   (
      /* [IN/OUT] where to store the input string */
      char _PTR_  tty_line_ptr,

      /* [IN] the maximum length to store */
      _mqx_int    size,
      
      /* [IN] the stream to read from */
      MQX_FILE_PTR file_ptr
   )
{ /* Body */
   _mqx_int result;

#if MQX_CHECK_ERRORS
   if (file_ptr == NULL) {
      return(NULL);
   } /* Endif */
#endif

   // Null terminate the buffer so we can tell if _io_fgetline() actually
   // read any data...

   tty_line_ptr[0] = '\0';

   // Attempt to read a line of text from file_ptr

   result = _io_fgetline(file_ptr, tty_line_ptr, size);

   // If _io_fgetline() returned IO_EOF *and* no data was
   // read into the tty_line_ptr buffer, return NULL...

   if ((result == IO_EOF) && (tty_line_ptr[0] == '\0')) {
      return(NULL);
   } /* Endif */

   // Otherwise, at least 1 byte was read so return the buffer pointer

   return tty_line_ptr;

} /* Endbody */

/* EOF */

