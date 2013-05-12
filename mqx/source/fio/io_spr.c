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
* $FileName: io_spr.c$
* $Version : 3.8.4.1$
* $Date    : Mar-2-2012$
*
* Comments:
*
*   This file contains the function for sprintf and snprintf.
*
*END************************************************************************/

#include "mqx.h"
#include "fio.h"
#include "fio_prv.h"
#include "io.h"
#include "io_prv.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_sprintf
* Returned Value   : _mqx_int number of characters printed
* Comments         :
*    This function performs similarly to the sprintf function found in 'C'.
*    See doprint.c for comments.
*    The returned number of characters does not include the terminating '\0'
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_sprintf
   (
      /* [IN] the string to print into */
      char        _PTR_ str_ptr,
      
      /* [IN] the format specifier */
      const char  _PTR_ fmt_ptr,
      ...
   )
{ /* Body */
   _mqx_int result;
   va_list ap;
   
   va_start(ap, fmt_ptr);
   result = _io_doprint((MQX_FILE_PTR)((pointer)&str_ptr), _io_sputc, -1, (char _PTR_)fmt_ptr, ap);
   *str_ptr = '\0';
   va_end(ap);
   return result;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_snprintf
* Returned Value   : _mqx_int number of characters printed
* Comments         :
*    This function performs similarly to the sprintf function found in 'C'.
*    See doprint.c for comments.
*    The returned number of characters does not include the terminating '\0'
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_snprintf
   (
      /* [IN] the string to print into */
      char          _PTR_ str_ptr,
      
      /* [IN] the Maximal size of string */
      _mqx_int      max_count,
      
      /* [IN] the format specifier */
      const char    _PTR_ fmt_ptr,
      ...
   )
{
    _mqx_int result;

    va_list ap;
    va_start(ap, fmt_ptr);
    result = _io_doprint((MQX_FILE_PTR)((pointer)&str_ptr), _io_sputc, max_count, (char _PTR_)fmt_ptr, ap);
    va_end(ap);
    if (0 != max_count)
    {
        *str_ptr = '\0';
    }
   
    return result;

}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_sputc
* Returned Value   : void
* Comments         :
*    writes the character into the string located by the string pointer and
*    updates the string pointer.
*
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_sputc
   (
      /* [IN] the character to put into the string */
      _mqx_int  c,
      
      /* [IN/OUT] this is REALLY a pointer to a string pointer, updated */
      MQX_FILE_PTR input_string_ptr
   )
{ /* Body */
   char _PTR_ _PTR_ string_ptr = (char _PTR_ _PTR_)((pointer)input_string_ptr);

   *(*string_ptr)++ = (char)c;
   return c;

} /* Endbody */

/* EOF */
