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
* $FileName: svcreply.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the implementation of the server
*   side RPC Library reply generation functions.
*
*END************************************************************************/

/*
** Sun RPC is a product of Sun Microsystems, Inc. and is provided for
** unrestricted use provided that this legend is included on all tape
** media and as a part of the software program in whole or part.  Users
** may copy or modify Sun RPC without charge, but are not authorized
** to license or distribute it to anyone else except as part of a product or
** program developed by the user.
**
** SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
** WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
** PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
**
** Sun RPC is provided with no support and without any obligation on the
** part of Sun Microsystems, Inc. to assist in its use, correction,
** modification or enhancement.
**
** SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
** INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
** OR ANY PART THEREOF.
**
** In no event will Sun Microsystems, Inc. be liable for any lost revenue
** or profits or other special, indirect and consequential damages, even if
** Sun has been advised of the possibility of such damages.
**
** Sun Microsystems, Inc.
** 2550 Garcia Avenue
** Mountain View, California  94043
*/


#include <rtcsrpc.h>


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svc_sendreply
* Returned Value   : TRUE or FALSE
* Comments  :  Sends a successful reply to the client.
*
*END*-----------------------------------------------------------------*/

bool_t svc_sendreply
   (
      register SVCXPRT_PTR xprt,
            /* [IN] - transport handle */
      xdrproc_t            xdr_results,
            /* [IN] - XDR filter for return values */
      caddr_t              xdr_location
            /* [IN] - return values */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_ACCEPTED;
   rply.acpted_rply.ar_verf = xprt->xp_verf;
   rply.acpted_rply.ar_stat = SUCCESS;
   rply.acpted_rply.ar_results.where = xdr_location;
   rply.acpted_rply.ar_results.proc = xdr_results;
   return (svc_reply(xprt, &rply));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_noproc
* Returned Value   : void
* Comments  :  Sends a "procedure unavailable" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_noproc
   (
      register SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_ACCEPTED;
   rply.acpted_rply.ar_verf = xprt->xp_verf;
   rply.acpted_rply.ar_stat = PROC_UNAVAIL;
   svc_reply(xprt, &rply);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_noprog
* Returned Value   : void
* Comments  :  Sends a "program unavailable" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_noprog
   (
      register SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_ACCEPTED;
   rply.acpted_rply.ar_verf = xprt->xp_verf;
   rply.acpted_rply.ar_stat = PROG_UNAVAIL;
   svc_reply(xprt, &rply);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_progvers
* Returned Value   : void
* Comments  :  Sends a "program version mismatch" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_progvers
   (
      register SVCXPRT_PTR xprt,
            /* [IN] - transport handle */
      uint_32              low_vers,
            /* [IN] - lowest version recognized */
      uint_32              high_vers
            /* [IN] - highest version recognized */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_ACCEPTED;
   rply.acpted_rply.ar_verf = xprt->xp_verf;
   rply.acpted_rply.ar_stat = PROG_MISMATCH;
   rply.acpted_rply.ar_vers.low = low_vers;
   rply.acpted_rply.ar_vers.high = high_vers;
   svc_reply(xprt, &rply);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_auth
* Returned Value   : void
* Comments  :  Sends an "authentication error" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_auth
   (
      register SVCXPRT_PTR xprt,
            /* [IN] - transport handle */
      enum auth_stat       why
            /* [IN] - authentication error */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_DENIED;
   rply.rjcted_rply.rj_stat = AUTH_ERROR;
   rply.rjcted_rply.rj_why = why;
   svc_reply(xprt, &rply);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_weakauth
* Returned Value   : void
* Comments  :  Sends a "credentials rejected" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_weakauth
   (
      register SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */
   svcerr_auth(xprt, AUTH_TOOWEAK);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_decode
* Returned Value   : void
* Comments  :  Sends a "could not decode arguments" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_decode
   (
      register SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_ACCEPTED;
   rply.acpted_rply.ar_verf = xprt->xp_verf;
   rply.acpted_rply.ar_stat = GARBAGE_ARGS;
   svc_reply(xprt, &rply);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcerr_systemerr
* Returned Value   : void
* Comments  :  Sends a "system error" reply to the client.
*
*END*-----------------------------------------------------------------*/

void svcerr_systemerr
   (
      register SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */
   RPC_MESSAGE rply;

   rply.rm_direction = REPLY;
   rply.rm_reply.rp_stat = MSG_ACCEPTED;
   rply.acpted_rply.ar_verf = xprt->xp_verf;
   rply.acpted_rply.ar_stat = SYSTEM_ERR;
   svc_reply(xprt, &rply);
} /* Endbody */


/* EOF */
