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
* $FileName: io_fseek.c$
* $Version : 3.8.6.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the function for setting the current location
*   in a file.
*
*END************************************************************************/

#include "mqx.h"
#include "fio.h"
#include "fio_prv.h"
#include "io.h"
#include "io_prv.h"


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _io_fseek
* Returned Value   : _mqx_int 0 or IO_ERROR on error.
* Comments         :
*   This function sets the current file position.
*
*END*----------------------------------------------------------------------*/

_mqx_int _io_fseek
   (
      /* [IN] the stream to use */
      MQX_FILE_PTR file_ptr,

      /* [IN] the offset for the seek */
      _file_offset  offset,

      /* [IN] mode to determine where to start the seek from */
      _mqx_uint  mode
   )
{ /* Body */
   IO_DEVICE_STRUCT_PTR dev_ptr;

#if MQX_CHECK_ERRORS
    if (file_ptr == NULL) {
        return (IO_ERROR);
    } /* Endif */
#endif

    switch (mode) {
    case IO_SEEK_SET:
        if (offset < 0) {
            return IO_ERROR;
        }
        file_ptr->LOCATION = offset;
        break;
    case IO_SEEK_CUR:
        /* Adjust possibly fake location before relative seek */
        if (file_ptr->HAVE_UNGOT_CHARACTER && (file_ptr->LOCATION > 0)) {
            offset--;
        }
        if (offset < 0 && (file_ptr->LOCATION < (_file_size) (-offset))) {
            return IO_ERROR;
        }
        file_ptr->LOCATION += offset;
        break;
    case IO_SEEK_END:
        if (offset < 0 && (file_ptr->SIZE < (_file_size) (-offset))) {
            return IO_ERROR;
        }
        file_ptr->LOCATION = file_ptr->SIZE + offset;
        break;
#if MQX_CHECK_ERRORS
    default:
        return (IO_ERROR);
#endif
    } /* Endswitch */

    /* Empty ungetc buffer */
    if (file_ptr->HAVE_UNGOT_CHARACTER) {
        file_ptr->HAVE_UNGOT_CHARACTER = FALSE;
    }

    /* Clear EOF flag */
    file_ptr->FLAGS &= ~((_mqx_uint) IO_FLAG_AT_EOF);

    dev_ptr = file_ptr->DEV_PTR;
    if (dev_ptr->IO_IOCTL != NULL) {
        (*dev_ptr->IO_IOCTL)(file_ptr, IO_IOCTL_SEEK, NULL);
    } /* Endif */

    return (MQX_OK);

} /* Endbody */

/* EOF */
