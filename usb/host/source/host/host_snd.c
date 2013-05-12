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
* $FileName: host_snd.c$
* $Version : 3.8.B.1$
* $Date    : Jun-28-2012$
*
* Comments:
*
*   This file contains the USB Host API specific functions to send data.
*
*END************************************************************************/
#include <mqx.h>

#include "usb.h"
#include "usb_prv.h"

#include "host_cnfg.h"
#include "hostapi.h"
#include "host_main.h"
#include "host_snd.h"

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_host_get_frame_number
*  Returned Value : frame number
*  Comments       :
* This function can be used to inquire the current frame number.
*
*END*-----------------------------------------------------------------*/
uint_32 _usb_host_get_frame_number
   (
      /* [IN] the USB Host state structure */
      _usb_host_handle  handle
   )
{ /* Body */
   USB_HOST_STATE_STRUCT_PTR usb_host_ptr;
   usb_host_ptr = (USB_HOST_STATE_STRUCT_PTR)handle;
   #ifdef _HOST_DEBUG_
      DEBUG_LOG_TRACE("_usb_host_get_frame_number");
   #endif

      /* Call the low-level send routine */      
   #ifdef _HOST_DEBUG_
      DEBUG_LOG_TRACE("_usb_host_get_frame_number OS mode SUCCESSFUL");
   #endif
  
    return((USB_HOST_CALLBACK_FUNCTIONS_STRUCT_PTR)usb_host_ptr->CALLBACK_STRUCT_PTR)->HOST_GET_FRAME_NUM(handle);
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_host_get_micro_frame_number
*  Returned Value : frame number
*  Comments       :
* This function can be used to inquire the current micro frame number.
*
*END*-----------------------------------------------------------------*/
uint_32 _usb_host_get_micro_frame_number
   (
      /* [IN] the USB Host state structure */
      _usb_host_handle  handle
   )
{ /* Body */
   USB_HOST_STATE_STRUCT_PTR usb_host_ptr;
   usb_host_ptr = (USB_HOST_STATE_STRUCT_PTR)handle;
   #ifdef _HOST_DEBUG_
      DEBUG_LOG_TRACE("_usb_host_get_micro_frame_number");
   #endif

   #ifdef _HOST_DEBUG_
      DEBUG_LOG_TRACE("_usb_host_get_micro_frame_number OS mode SUCCESSFUL");
   #endif

      /* Call the low-level send routine */      
    return((USB_HOST_CALLBACK_FUNCTIONS_STRUCT_PTR)usb_host_ptr->CALLBACK_STRUCT_PTR)->HOST_GET_MICRO_FRAME_NUM(handle);
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : _usb_host_send_data
*  Returned Value : error or status of the transfer
*  Comments       :
* The Send Data routine is non-blocking routine that causes a block of data 
* to be made available for transmission to the USB host.
*
*END*-----------------------------------------------------------------*/
USB_STATUS _usb_host_send_data
   (
      /* [IN] the USB Host state structure */
      _usb_host_handle           handle, 

      /* [IN] the pipe handle */
      _usb_pipe_handle           pipe_handle,

      /* [IN] transfer parameters */
      TR_INIT_PARAM_STRUCT_PTR   tr_params_ptr
   )
{ /* Body */
   PIPE_STRUCT_PTR            pipe_ptr;
   TR_STRUCT_PTR              tr_ptr;
   USB_HOST_STATE_STRUCT_PTR  usb_host_ptr;
   uint_32  return_code;

   #ifdef _HOST_DEBUG_
      DEBUG_LOG_TRACE("_usb_host_send_data");
   #endif

   USB_lock();
   return_code = _usb_host_set_up_tr(pipe_handle, tr_params_ptr, &tr_ptr);
   if (return_code != USB_STATUS_TRANSFER_QUEUED) {
      USB_unlock();
      #ifdef _HOST_DEBUG_
         DEBUG_LOG_TRACE("_usb_host_send_data failed to queue transfer");
      #endif

      return USB_log_error(__FILE__,__LINE__,return_code);
   } /* Endif */
      
   usb_host_ptr = (USB_HOST_STATE_STRUCT_PTR)handle;
   pipe_ptr = (PIPE_STRUCT_PTR)pipe_handle;

   /*Must Flush and Invalidate the buffer before sending
   /receiving the data in it */
   USB_dcache_flush_mlines((void *)tr_ptr->TX_BUFFER, tr_ptr->TX_LENGTH);

   /* We have obtained the current TR on the Pipe's TR list 
   ** from _usb_host_set_up_tr
   */
      /* Call the low-level send routine */      
   return_code = _usb_host_send_call_interface (handle, pipe_ptr, tr_ptr);

   USB_unlock();
   
   #ifdef _HOST_DEBUG_
      DEBUG_LOG_TRACE("_usb_host_send_data SUCCESSFUL");
   #endif

   if(return_code == USB_OK)
   {
      return USB_STATUS_TRANSFER_QUEUED;
   }
   else
   {
      return USB_log_error(__FILE__,__LINE__,return_code);
   }

} /* Endbody */

/* EOF */
