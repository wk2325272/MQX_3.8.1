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
* $FileName: iodun.c$
* $Version : 3.6.7.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains a driver used by PPP to communicate to
*   Windows Dial-Up Networking or to Windows RAS.
*
*END************************************************************************/

#include <rtcsrtos.h>
#include <fio.h>
#include <iodun.h>


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_install
* Returned Value   : uint_32 a task error code or MQX_OK
* Comments         :
*    Install the Dial-Up Networking driver.
*
*END*-----------------------------------------------------------------*/

uint_32 _io_dun_install
   (
      /* [IN] A string that identifies the device for fopen */
      char_ptr    identifier
   )
{ /* Body */
   IODUN_DEV_STRUCT_PTR dev_ptr = _mem_alloc_system_zero(sizeof(IODUN_DEV_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (dev_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif

   dev_ptr->RECV      = "CLIENT";
   dev_ptr->SEND      = "CLIENTSERVER";


   return (_io_dev_install(identifier,
      _io_dun_open,
      _io_dun_close,
      _io_dun_read,
      _io_dun_write,
      _io_dun_ioctl,
      dev_ptr));

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_ras_install
* Returned Value   : uint_32 a task error code or MQX_OK
* Comments         :
*    Install the RAS driver.
*
*END*-----------------------------------------------------------------*/

uint_32 _io_ras_install
   (
      /* [IN] A string that identifies the device for fopen */
      char_ptr    identifier
   )
{ /* Body */
   IODUN_DEV_STRUCT_PTR dev_ptr = _mem_alloc_system_zero(sizeof(IODUN_DEV_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (dev_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif

   dev_ptr->RECV      = "CLIENTSERVER";
   dev_ptr->SEND      = "CLIENT";

   return (_io_dev_install(identifier,
      _io_dun_open,
      _io_dun_close,
      _io_dun_read,
      _io_dun_write,
#if MQX_VERSION < 250
      (int_32(_CODE_PTR_)(MQX_FILE_PTR, uint_32, uint_32_ptr))
#endif
      _io_dun_ioctl,
      dev_ptr));

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_open
* Returned Value   : MQX_OK
* Comments         : Opens and initializes the DUN or RAS driver.
*
*END*-----------------------------------------------------------------*/

_mqx_int _io_dun_open
   (
      /* [IN] the file handle for the device being opened */
      MQX_FILE_PTR fd_ptr,

      /* [IN] the name of the device */
      char_ptr    open_name_ptr,

      /* [IN] the flags to be used during operation:
      ** echo, translation, xon/xoff, encoded into a pointer.
      */
      char_ptr    f_ptr
   )
{  /* Body */
   IODUN_DEV_STRUCT_PTR dev_ptr = fd_ptr->DEV_PTR->DRIVER_INIT_PTR;
   IODUN_STRUCT_PTR ras_ptr = _mem_alloc_system_zero(sizeof(IODUN_STRUCT));

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
   if (ras_ptr == NULL) {
      return(MQX_OUT_OF_MEMORY);
   } /* Endif */
#endif

   fd_ptr->DEV_DATA_PTR = ras_ptr;

   IODUN_mutex_init(&ras_ptr->MUTEX);

   ras_ptr->F_PTR = (MQX_FILE_PTR)f_ptr;
   _io_fputs(dev_ptr->SEND, ras_ptr->F_PTR);
   return(MQX_OK);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_close
* Returned Value   : ERROR CODE
* Comments         : Closes ras driver
*
*END*-----------------------------------------------------------------*/

_mqx_int _io_dun_close
   (
      /* [IN] the file handle for the device being closed */
      MQX_FILE_PTR fd_ptr
   )
{ /* Body */
   IODUN_STRUCT_PTR ras_ptr = fd_ptr->DEV_DATA_PTR;

   IODUN_mutex_destroy(&ras_ptr->MUTEX);
   _mem_free(ras_ptr);

   return(MQX_OK);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_read
* Returned Value   : number of characters read
* Comments         : Reads data from ras driver
*
*END*-----------------------------------------------------------------*/

_mqx_int _io_dun_read
   (
      /* [IN] the file handle for the device */
      MQX_FILE_PTR fd_ptr,

      /* [IN] where the characters are to be stored */
      char_ptr    data_ptr,

      /* [IN] the number of characters to input */
      _mqx_int    num
   )
{ /* Body */
   IODUN_STRUCT_PTR ras_ptr = fd_ptr->DEV_DATA_PTR;
   _mqx_int         numchar = num;

   IODUN_mutex_lock(&ras_ptr->MUTEX);

   while (numchar--) {
      *data_ptr++ = _io_dun_read_char(fd_ptr);
   } /* Endwhile */

   IODUN_mutex_unlock(&ras_ptr->MUTEX);
   return(num);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_write
* Returned Value   : number of characters written
* Comments         : Writes data to the ras device
*
*END*-----------------------------------------------------------------*/

_mqx_int _io_dun_write
   (
      /* [IN] the file handle for the device */
      MQX_FILE_PTR fd_ptr,

      /* [IN] where the characters are */
      char_ptr    data_ptr,

      /* [IN] the number of characters to output */
      _mqx_int    num
   )
{ /* Body */
   IODUN_STRUCT_PTR ras_ptr = fd_ptr->DEV_DATA_PTR;

   return _io_write(ras_ptr->F_PTR, data_ptr, num);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_ioctl
* Returned Value   : int_32
* Comments         :
*    Returns result of ioctl operation.
*
*END*-----------------------------------------------------------------*/

_mqx_int _io_dun_ioctl
   (
      /* [IN] the file handle for the device */
      MQX_FILE_PTR fd_ptr,

      /* [IN] the ioctl command */
      _mqx_uint   cmd,

      /* [IN] the ioctl parameters */
      pointer     param_ptr
   )
{ /* Body */
   IODUN_STRUCT_PTR ras_ptr = fd_ptr->DEV_DATA_PTR;

   if (cmd == IO_IOCTL_SET_LINK_UP) {
      ras_ptr->LINK_UP = *((boolean _PTR_) param_ptr);
      return MQX_OK;
   }

   return _io_ioctl(ras_ptr->F_PTR, cmd, param_ptr);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _io_dun_read_char
* Returned Value   :
* Comments         : Reads data from ras driver
*
*END*-----------------------------------------------------------------*/

char _io_dun_read_char
   (
      /* [IN] the file handle for the device */
      MQX_FILE_PTR fd_ptr
   )
{  /* Body */
   IODUN_STRUCT_PTR  ras_ptr = fd_ptr->DEV_DATA_PTR;
   IODUN_DEV_STRUCT_PTR dev_ptr = fd_ptr->DEV_PTR->DRIVER_INIT_PTR;
   char c;

   c = _io_fgetc(ras_ptr->F_PTR);

   if (c == dev_ptr->RECV[dev_ptr->RECV_INDEX]) {
       // Matches current Receive string character
       dev_ptr->RECV_INDEX++;
       if (dev_ptr->RECV[dev_ptr->RECV_INDEX]=='\0') {
          // Matches the entire string, respond
           _io_fputs(dev_ptr->SEND, ras_ptr->F_PTR);
       }
   } else {
      dev_ptr->RECV_INDEX=0;
   }

   return c;

} /* Endbody */

/* EOF */
