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
* $FileName: clnttcp.c$
* $Version : 3.5.7.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the implementation of the TCP/IP
*   based client side RPC Library functions.
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

#define MCALLMSGSIZE 24

static int_32  readtcp();
static int_32  writetcp();

static enum clnt_stat   clnttcp_call();
static void             clnttcp_abort();
static void             clnttcp_geterr();
static bool_t           clnttcp_freeres();
static bool_t           clnttcp_control();
static void             clnttcp_destroy();

static struct clnt_ops tcp_ops = {
   clnttcp_call,
   clnttcp_abort,
   clnttcp_geterr,
   clnttcp_freeres,
   clnttcp_destroy,
   clnttcp_control
};

/*
** Private data kept per TCP client handle
*/
typedef struct client_tcp {
   int_32          ct_sock;
   bool_t          ct_closeit;
   sockaddr_in     ct_addr;
   struct timeval  ct_wait;
   bool_t          ct_waitset;
   RPC_ERROR       ct_error;
   XDR             ct_xdrs;
   char            ct_mcall[MCALLMSGSIZE];
   uint_32         ct_mpos;
} CLIENT_TCP, _PTR_ CLIENT_TCP_PTR;

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_create
* Returned Value   : client handle
* Comments  :  Creates a client handle for TCP/IP based RPC
*
* If *sockp<0, *sockp is set to a newly created TCP socket, and it
* is connected to raddr.
* If raddr->sin_port==0, a binder on the remote machine (portmapper) is
* consulted for the correct port number.
* NB: If the client provides a socket, clnt_destroy won't close it.
* NB: The rpch->cl_auth is initialized to null authentication.
*
*
* The TCP/RPC package performs buffering, so the caller must choose send and
* receive buffer sizes (sendsz and recvsz); 0=>default.
*
*END*-----------------------------------------------------------------*/

CLIENT_PTR clnttcp_create
   (
      sockaddr_in _PTR_          raddr,
         /* [IN] - address of RPC server */
      uint_32                    prog,
         /* [IN] - number of remote program to bind to */
      uint_32                    vers,
         /* [IN] - version of remote program to bind to */
      int_32_ptr                 sockp,
         /* [IN,OUT] - local socket */
      uint_32                    sendsz,
         /* [IN] - size of send buffer */
      uint_32                    recvsz
         /* [IN] - size of receive buffer */
   )
{ /* Body */
   CLIENT_PTR cl = NULL;
   register CLIENT_TCP_PTR ct = NULL;
   RPC_MESSAGE call_msg;

      /*
      ** Allocate a client handle
      */
   cl = (CLIENT_PTR)RTCS_mem_alloc_zero(sizeof(CLIENT));
   if (cl == NULL) {
      fprintf(stderr, "clnttcp_create: out of memory\n");
      rpc_createerr.cf_stat = RPC_SYSTEMERROR;
      rpc_createerr.cf_error.re_errno = RTCSERR_OUT_OF_MEMORY;
      goto create_fail;
   } /* Endif */

      /*
      ** Allocate a private handle
      */
   ct = (CLIENT_TCP_PTR)RTCS_mem_alloc_zero(sizeof(CLIENT_TCP));
   if (ct == NULL) {
      fprintf(stderr, "clnttcp_create: out of memory\n");
      rpc_createerr.cf_stat = RPC_SYSTEMERROR;
      rpc_createerr.cf_error.re_errno = RTCSERR_OUT_OF_MEMORY;
      goto create_fail;
   } /* Endif */

      /*
      ** If caller doesn't know the remote port, ask portmapper
      */
   if (raddr->sin_port == 0) {
      uint_16 port;
      port = pmap_getport(raddr, prog, vers, IPPROTO_TCP);
      if (!port) {
         goto create_fail;
      } /* Endif */
      raddr->sin_port = port;
   } /* Endif */

      /*
      ** Initialize the private handle
      */
   cl->cl_ops = &tcp_ops;
   cl->cl_private = ct;
   cl->cl_errbuf = NULL;
   ct->ct_addr = *raddr;
   ct->ct_wait.tv_sec = 0;
   ct->ct_wait.tv_usec = 0;
   ct->ct_waitset = FALSE;

      /*
      ** Preserialize the RPC message header
      */
   call_msg.rm_xid = RTCS_rand();
   call_msg.rm_direction = CALL;
   call_msg.rm_call.cb_rpcvers = RPC_MSG_VERSION;
   call_msg.rm_call.cb_prog = prog;
   call_msg.rm_call.cb_vers = vers;
   xdrmem_create(&ct->ct_xdrs, ct->ct_mcall, MCALLMSGSIZE, XDR_ENCODE);
   if (! xdr_callhdr(&ct->ct_xdrs, &call_msg)) {
      goto create_fail;
   } /* Endif */
   ct->ct_mpos = xdr_getpos(&ct->ct_xdrs);
   xdr_destroy(&ct->ct_xdrs);

      /*
      ** Open a record-based XDR stream
      */
   xdrrec_create(&ct->ct_xdrs, sendsz, recvsz, ct, readtcp, writetcp);

      /*
      ** Allocate, bind and connect a socket if the caller didn't provide one
      */
   if (*sockp == RPC_ANYSOCK) {
      sockaddr_in laddr;

      *sockp = socket(PF_INET, SOCK_STREAM, 0);
      if (*sockp == (int_32)RTCS_SOCKET_ERROR) {
         rpc_createerr.cf_stat = RPC_SOCKETERROR;
         rpc_createerr.cf_error.re_errno = *sockp;
         goto create_fail;
      } /* Endif */

      laddr.sin_family = AF_INET;
      laddr.sin_port = 0;
      laddr.sin_addr.s_addr = 0;

      bind(*sockp, &laddr, sizeof(laddr));

      if (connect(*sockp, raddr, sizeof(*raddr))) {
         rpc_createerr.cf_stat = RPC_SOCKETERROR;
         rpc_createerr.cf_error.re_errno = RTCS_geterror(*sockp);
         shutdown(*sockp, FLAG_CLOSE_TX);
         goto create_fail;
      } /* Endif */

      ct->ct_closeit = TRUE;
   } else {
      ct->ct_closeit = FALSE;
   } /* Endif */
   ct->ct_sock = *sockp;

      /*
      ** Use null authentication by default
      */
   cl->cl_auth = authnone_create();

   return (cl);

      /*
      ** If clnt_create failed, free all the memory we allocated.
      */
create_fail:
   if (ct) {
      _mem_free(ct);
   } /* Endif */
   if (cl) {
      _mem_free(cl);
   } /* Endif */
   return (NULL);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_call
* Returned Value   : error code
* Comments  :  Calls a remote procedure using TCP/IP based RPC
*
*END*-----------------------------------------------------------------*/

static enum clnt_stat clnttcp_call
   (
   CLIENT_PTR     cl,
         /* [IN] - client handle */
   uint_32        proc,
         /* [IN] - remote program to call */
   xdrproc_t      xargs,
         /* [IN] - xdr filter for arguments */
   caddr_t        argsp,
         /* [IN] - pointer to arguments */
   xdrproc_t      xresults,
         /* [IN] - xdr filter for results */
   caddr_t        resultsp,
         /* [IN,OUT] - pointer to results */
   struct timeval timeout
         /* [IN] - time to wait before giving up */
   )
{ /* Body */
   register CLIENT_TCP_PTR ct = (CLIENT_TCP_PTR)cl->cl_private;
   register XDR_PTR xdrs = &ct->ct_xdrs;
   RPC_MESSAGE reply_msg;
   uint_32 xid;
   bool_t shipnow;
   uint_16 refreshes = 2;

      /*
      ** Use the supplied timeout only if no timeout has been
      ** specified with clnt_control(CLSET_TIMEOUT).
      */
   if (!ct->ct_waitset) {
      ct->ct_wait = timeout;
   } /* Endif */

   shipnow = !(xresults == NULL_xdrproc_t
            && timeout.tv_sec == 0
            && timeout.tv_usec == 0);

clnt_request:
   xdrs->x_op = XDR_ENCODE;
   ct->ct_error.re_status = RPC_SUCCESS;

      /*
      ** Update the transaction ID -- it's the first field in
      ** preserialized message header.
      */
   xid = ntohl((uchar_ptr)ct->ct_mcall);
   xid--;
   htonl((uchar_ptr)ct->ct_mcall, xid);

      /*
      ** Marshall the message:  message header (which is already
      ** serialized in ct->ct_mcall), procedure number, credentials,
      ** verifier and arguments.
      */
   if ((! xdr_putbytes(xdrs, ct->ct_mcall, ct->ct_mpos)) ||
       (! xdr_uint_32(xdrs, &proc)) ||
       (! auth_marshall(cl->cl_auth, xdrs)) ||
       (! (*xargs)(xdrs, argsp))) {
      if (ct->ct_error.re_status == RPC_SUCCESS)
         ct->ct_error.re_status = RPC_CANTENCODEARGS;
      xdrrec_endofrecord(xdrs, TRUE);
      return (ct->ct_error.re_status);
   } /* Endif */
   if (! xdrrec_endofrecord(xdrs, shipnow)) {
      return (ct->ct_error.re_status = RPC_CANTSEND);
   } /* Endif */
   if (! shipnow) {
      return (RPC_SUCCESS);
   } /* Endif */

      /*
      ** Hack for timeout == 0
      */
   if (timeout.tv_sec == 0 && timeout.tv_usec == 0) {
      return (ct->ct_error.re_status = RPC_TIMEDOUT);
   } /* Endif */

      /*
      ** Try receiving until we get a valid transaction ID
      */
   xdrs->x_op = XDR_DECODE;
clnt_recv:

      /*
      ** Try to receive a reply
      */
   reply_msg.acpted_rply.ar_verf = rpc_null_auth;
   reply_msg.acpted_rply.ar_results.where = NULL;
   reply_msg.acpted_rply.ar_results.proc = (xdrproc_t)xdr_void;
   if (! xdrrec_skiprecord(xdrs)) {
      return (ct->ct_error.re_status);
   } /* Endif */

      /*
      ** Verify the transaction ID
      */
   if (! xdr_replymsg(xdrs, &reply_msg)) {
      if (ct->ct_error.re_status == RPC_SUCCESS) {
         goto clnt_recv;
      } /* Endif */
      return (ct->ct_error.re_status);
   } /* Endif */
   if (reply_msg.rm_xid != xid) {
      goto clnt_recv;
   } /* Endif */

      /*
      ** Assume we have the proper reply.  Decode and validate.
      */
   rpc_seterr_reply(&reply_msg, &ct->ct_error);
   if (ct->ct_error.re_status == RPC_SUCCESS) {
         /*
         ** If the server accepted our request, we must
         ** authenticate the reply before we can accept it.
         */
      if (! auth_validate(cl->cl_auth, &reply_msg.acpted_rply.ar_verf)) {
         ct->ct_error.re_status = RPC_AUTHERROR;
         ct->ct_error.re_why = AUTH_INVALIDRESP;
         /*
         ** Now, decode the results.
         */
      } else if (! (*xresults)(xdrs, resultsp)) {
         if (ct->ct_error.re_status == RPC_SUCCESS) {
            ct->ct_error.re_status = RPC_CANTDECODERES;
         } /* Endif */
      } /* Endif */

         /*
         ** Free the verifier in the accepted_reply structure.
         */
      xdrs->x_op = XDR_FREE;
      xdr_opaque_auth(xdrs, &(reply_msg.acpted_rply.ar_verf));

   } else {
         /*
         ** If the server rejected our request, it may be because
         ** our credentials are stale.  Refresh and try again.
         */
      if (refreshes > 0 && auth_refresh(cl->cl_auth)) {
         refreshes--;
         goto clnt_request;
      } /* Endif */
   } /* Endif */

   return (ct->ct_error.re_status);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_geterr
* Returned Value   : void
* Comments  :  Retrieves the last error code from a client call
*
*END*-----------------------------------------------------------------*/

static void clnttcp_geterr
   (
      CLIENT_PTR     cl,
            /* [IN] - client handle */
      RPC_ERROR_PTR  errp
            /* [OUT] - last error that occurred on client handle */
   )
{ /* Body */
   CLIENT_TCP_PTR ct = (CLIENT_TCP_PTR)cl->cl_private;

   *errp = ct->ct_error;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_freeres
* Returned Value   : TRUE or FALSE
* Comments  :  Frees the result from a remote procedure call
*
*END*-----------------------------------------------------------------*/

static bool_t clnttcp_freeres
   (
      CLIENT_PTR  cl,
            /* [IN] - client handle */
      xdrproc_t   xdr_res,
            /* [IN] - filter used to decode result from remote procedure */
      caddr_t     res_ptr
            /* [IN] - pointer to dynamically allocated result */
   )
{ /* Body */
   CLIENT_TCP_PTR ct = (CLIENT_TCP_PTR)cl->cl_private;
   XDR_PTR xdrs = &ct->ct_xdrs;

   xdrs->x_op = XDR_FREE;
   return ((*xdr_res)(xdrs, res_ptr));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_abort
* Returned Value   : nothing
* Comments  :  Halts a remote procedure call in progress (not implemented)
*
*END*-----------------------------------------------------------------*/

static void clnttcp_abort
   (
      CLIENT_PTR  cl
            /* [IN] - client handle */
   )
{ /* Body */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_control
* Returned Value   : TRUE or FALSE
* Comments  :  ioctl() for remote procedure calls
*
*END*-----------------------------------------------------------------*/

static bool_t clnttcp_control
   (
      CLIENT_PTR  cl,
            /* [IN] - client handle */
      int_32      request,
            /* [IN] - control operation to perform */
      char_ptr    info
            /* [IN,OUT] - pointer to control information */
   )
{ /* Body */
   CLIENT_TCP_PTR ct = (CLIENT_TCP_PTR)cl->cl_private;

   switch (request) {
   case CLSET_TIMEOUT:
      ct->ct_wait = *(struct timeval _PTR_)info;
      ct->ct_waitset = TRUE;
      break;
   case CLGET_TIMEOUT:
      *(struct timeval _PTR_)info = ct->ct_wait;
      break;
   case CLGET_SERVER_ADDR:
      *(sockaddr_in _PTR_)info = ct->ct_addr;
      break;
   default:
      return (FALSE);
   } /* Endswitch */

   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clnttcp_destroy
* Returned Value   : nothing
* Comments  :  Destroys a TCP/IP based RPC client handle
*
*END*-----------------------------------------------------------------*/

static void clnttcp_destroy
   (
      CLIENT_PTR  cl
            /* [IN] - client handle */
   )
{ /* Body */
   CLIENT_TCP_PTR ct = (CLIENT_TCP_PTR)cl->cl_private;

   if (ct->ct_closeit) {
      shutdown(ct->ct_sock, 0);
   } /* Endif */
   xdr_destroy(&ct->ct_xdrs);
   _mem_free(ct);
   if (cl->cl_errbuf) {
      _mem_free(cl->cl_errbuf);
   } /* Endif */
   _mem_free(cl);

} /* Endbody */


/*
** Interface between XDR serializer and TCP connection.
*/


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : readtcp
* Returned Value   : number of bytes read
* Comments  :  Receives bytes from a connected TCP socket.
*
*END*-----------------------------------------------------------------*/

static int_32 readtcp
   (
      CLIENT_TCP_PTR ct,
      caddr_t buf,
      int_32 len
   )
{ /* Body */

   if ((len = recv(ct->ct_sock, buf, len, 0)) == -1) {
      ct->ct_error.re_errno = RTCS_geterror(ct->ct_sock);
      ct->ct_error.re_status = RPC_CANTRECV;
   } /* Endif */

   return (len);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : writetcp
* Returned Value   : number of bytes written
* Comments  :  Sends bytes out a connected TCP socket.
*
*END*-----------------------------------------------------------------*/

static int_32 writetcp
   (
      CLIENT_TCP_PTR ct,
      caddr_t buf,
      int_32 len
   )
{ /* Body */
   register int_32 i, cnt;

   for (cnt = len; cnt > 0; cnt -= i, buf += i) {
      if ((i = send(ct->ct_sock, buf, cnt, 0)) == -1) {
         ct->ct_error.re_errno = RTCS_geterror(ct->ct_sock);
         ct->ct_error.re_status = RPC_CANTSEND;
         return (-1);
      } /* Endif */
   } /* Endfor */

   return (len);
} /* Endbody */


/* EOF */
