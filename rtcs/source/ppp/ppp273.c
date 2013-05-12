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
* $FileName: ppp273.c$
* $Version : 3.6.4.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the 2.73 function calls with their
*   2.74 equivalents.
*
*END************************************************************************/

#include <ppp.h>
#include "ppp_prv.h"

_ppp_handle _PPP273_handle;

/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  PPP_Initialize
* Returned Value  :  error code
* Comments        :
*     Initializes PPP for a channel.
*
*END*-----------------------------------------------------------------*/

int_32 PPP_Initialize
   (
      _ppp_channel   channel,
            /* [IN] - PPP channel number */
      MQX_FILE_PTR   device,
            /* [IN] - I/O stream to use */
      PPP_CALL_PTR   call
            /* [IN] - call table */
   )
{ /* Body */
   _iopcb_handle     ioppp;
   void (_CODE_PTR_  callfn)();
   pointer           callparam;
   int_32            error;

   ioppp = _iopcb_ppphdlc_init(device);
   error = PPP_initialize(ioppp, &_PPP273_handle);
   if (!error) {
      callfn    = call->PPP_Link_up;
      callparam = (pointer)call->PARAM;
      PPP_setcall(_PPP273_handle, PPP_CALL_LINK_UP, &callfn, &callparam);
      callfn    = call->PPP_Link_down;
      callparam = (pointer)call->PARAM;
      PPP_setcall(_PPP273_handle, PPP_CALL_LINK_DOWN, &callfn, &callparam);
      callfn    = (void(_CODE_PTR_)())call->PPP_Echo_reply;
      callparam = (pointer)call->PARAM;
      PPP_setcall(_PPP273_handle, PPP_CALL_ECHO_REPLY, &callfn, &callparam);
      callfn    = call->PPP_Auth_fail;
      callparam = (pointer)call->PARAM;
      PPP_setcall(_PPP273_handle, PPP_CALL_AUTH_FAIL, &callfn, &callparam);
      _iopcb_open(ioppp, PPP_lowerup, PPP_lowerdown, _PPP273_handle);
   } /* Endif */
   return error;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  PPP_Setcall
* Returned Value  :  error code
* Comments        :
*     Sets an application callback function.
*
*END*-----------------------------------------------------------------*/

int_32 PPP_Setcall
   (
      _ppp_channel        channel,
            /* [IN] - the PPP state structure */
      uint_32             callnum,
            /* [IN] - PPP callback */
      void (_CODE_PTR_    callback)(),
            /* [IN] - callback function */
      uint_32             callparam
            /* [IN] - callback parameter */
   )
{ /* Body */
   return PPP_setcall(_PPP273_handle, callnum, &callback, (pointer _PTR_)&callparam);
} /* Endbody */


/* EOF */
