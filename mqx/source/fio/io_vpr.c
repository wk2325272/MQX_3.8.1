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
* $FileName: io_vpr.c$
* $Version : 3.8.4.2$
* $Date    : Mar-5-2012$
*
* Comments:
*
*   This file contains the functions for vprintf, vfprintf and vsprintf.
*   These functions are equivalent to the corresponding printf functions,
*   except that the variable argument list is replaced by one argument,
*   which has been initialized by the va_start macro.
*
*END************************************************************************/

#include "mqx.h"
#include "fio.h"
#include "fio_prv.h"
#include "io.h"
#include "io_prv.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_vprintf
* Returned Value   : _mqx_int number of characters printed
* Comments         :
*   This function is equivalent to the corresponding printf function,
*   except that the variable argument list is replaced by one argument,
*   which has been initialized by the va_start macro.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_vprintf
   (
      /* [IN] the format string */
      const char _PTR_ fmt_ptr, 

      /* [IN] the arguments */
      va_list          arg
   )
{ /* Body */
   _mqx_int result;
   
   result = _io_doprint(stdout, _io_fputc, -1, (char _PTR_)fmt_ptr, arg);

   return result;

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_vfprintf
* Returned Value   : _mqx_int number of characters outputted.
* Comments         : 
*   This function is equivalent to the corresponding printf function,
*   except that the variable argument list is replaced by one argument,
*   which has been initialized by the va_start macro.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_vfprintf
   (
      /* [IN] the stream to print upon */
      MQX_FILE_PTR     file_ptr,

      /* [IN] the format string to use for printing */
      const char _PTR_ fmt_ptr,

      /* [IN] the argument list to print */
      va_list          arg
   )
{ /* Body */
   _mqx_int result;
   
   result = 0;
   if ( file_ptr ) {
      result = _io_doprint(file_ptr, _io_fputc, -1, (char _PTR_)fmt_ptr, arg);
   } /* Endif */
   return result;

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_vsprintf
* Returned Value   : _mqx_int number of characters printed
* Comments         :
*   This function is equivalent to the corresponding printf function,
*   except that the variable argument list is replaced by one argument,
*   which has been initialized by the va_start macro.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_vsprintf
   ( 
      /* [IN] the string to print into */
      char        _PTR_ str_ptr,
      
      /* [IN] the format string */
      const char  _PTR_ fmt_ptr,
      
      /* [IN] the arguments */
      va_list           arg
   )
{ /* Body */
   _mqx_int result;
   
   result = _io_doprint((MQX_FILE_PTR)((pointer)&str_ptr), _io_sputc, -1, (char _PTR_)fmt_ptr, arg);
   *str_ptr = '\0';
   return result;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_vsnprintf
* Returned Value   : _mqx_int number of characters printed
* Comments         :
*   This function is equivalent to the corresponding printf function,
*   except that the variable argument list is replaced by one argument,
*   which has been initialized by the va_start macro.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_vsnprintf
   (
      /* [IN] the string to print into */
      char          _PTR_ str_ptr,
      
      /* [IN] the Maximal size of string */
      _mqx_int      max_count,
      
      /* [IN] the format specifier */
      const char    _PTR_ fmt_ptr,
      
      /* [IN] the arguments */
      va_list           arg
   )
{
    _mqx_int result;

    result = _io_doprint((MQX_FILE_PTR)((pointer)&str_ptr), _io_sputc, max_count, (char _PTR_)fmt_ptr, arg);
    
    if (0 != max_count)
    {
        *str_ptr = '\0';
    }
    
    return result;

}
/* EOF */
