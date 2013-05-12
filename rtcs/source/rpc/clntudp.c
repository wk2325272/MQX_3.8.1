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
* $FileName: clntudp.c$
* $Version : 3.5.10.0$
* $Date    : Jan-20-2010$
*
* Comments:
*
*   This file contains the implementation of the UDP/IP
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

#if RTCSCFG_ENABLE_UDP

static enum clnt_stat   clntudp_call();
static void             clntudp_abort();
static void             clntudp_geterr();
static bool_t           clntudp_freeres();
static bool_t           clntudp_control();
static void             clntudp_destroy();

static struct clnt_ops udp_ops = {
   clntudp_call,
   clntudp_abort,
   clntudp_geterr,
   clntudp_freeres,
   clntudp_destroy,
   clntudp_control
};

/*
** Private data kept per UDP client handle
*/
typedef struct client_udp {
   int_32                  cu_sock;
   boolean                 cu_closeit;
   sockaddr_in             cu_raddr;
   int_32                  cu_rlen;
   struct timeval          cu_retry;
   struct timeval          cu_wait;
   bool_t                  cu_waitset;
   RPC_ERROR               cu_error;
   XDR                     cu_outxdrs;
   uint_32                 cu_xdrpos;
   uint_32                 cu_sendsz;
   char_ptr                cu_outbuf;
   uint_32                 cu_recvsz;
   char                    cu_inbuf[1];
} CLIENT_UDP, _PTR_ CLIENT_UDP_PTR;

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_bufcreate
* Returned Value   : client handle
* Comments  :  Creates a client handle for UDP/IP based RPC
*
* If *sockp<0, *sockp is set to a newly created UDP socket.
* If raddr->sin_port==0, a binder on the remote machine (portmapper) is
* consulted for the correct port number.
* NB: If the client provides a socket, clnt_destroy won't close it.
* NB: The rpch->cl_auth is initialized to null authentication.
*     Caller may wish to set this something more useful.
*
* wait is the amount of time used between retransmitting a call if
* no response has been heard;  retransmission occurs until the actual
* RPC call times out.
*
* sendsz and recvsz are the maximum allowable packet sizes that can be
* sent and received.
*
*END*-----------------------------------------------------------------*/

CLIENT_PTR clntudp_bufcreate
   (
      sockaddr_in _PTR_          raddr,
         /* [IN] - address of RPC server */
      uint_32                    prog,
         /* [IN] - number of remote program to bind to */
      uint_32                    vers,
         /* [IN] - version of remote program to bind to */
      struct timeval             wait,
         /* [IN] - retry timeout */
      int_32_ptr                 sockp,
         /* [IN,OUT] - local socket */
      uint_32                    sendsz,
         /* [IN] - maximum size of send buffer */
      uint_32                    recvsz
         /* [IN] - maximum size of receive buffer */
   )
{ /* Body */
   CLIENT_PTR cl = NULL;
   register CLIENT_UDP_PTR cu = NULL;
   RPC_MESSAGE call_msg;

      /*
      ** Allocate a client handle
      */
   cl = (CLIENT_PTR)RTCS_mem_alloc_zero(sizeof(CLIENT));
   if (cl == NULL) {
      fprintf(stderr, "clntudp_create: out of memory\n");
      rpc_createerr.cf_stat = RPC_SYSTEMERROR;
      rpc_createerr.cf_error.re_errno = RTCSERR_OUT_OF_MEMORY;
      goto create_fail;
   } /* Endif */

      /*
      ** Allocate a private handle, with send and receive buffers
      */
   sendsz = XDRUNIT_RNDUP(sendsz);
   recvsz = XDRUNIT_RNDUP(recvsz);
   cu = (CLIENT_UDP_PTR)RTCS_mem_alloc_zero(sizeof(CLIENT_UDP) - 1 + sendsz + recvsz);
   if (cu == NULL) {
      fprintf(stderr, "clntudp_create: out of memory\n");
      rpc_createerr.cf_stat = RPC_SYSTEMERROR;
      rpc_createerr.cf_error.re_errno = RTCSERR_OUT_OF_MEMORY;
      goto create_fail;
   } /* Endif */
   cu->cu_outbuf = &cu->cu_inbuf[recvsz];

      /*
      ** If caller doesn't know the remote port, ask portmapper
      */
   if (raddr->sin_port == 0) {
      uint_16 port;
      port = pmap_getport(raddr, prog, vers, IPPROTO_UDP);
      if (!port) {
         goto create_fail;
      } /* Endif */
      raddr->sin_port = port;
   } /* Endif */

      /*
      ** Initialize the private handle
      */
   cl->cl_ops = &udp_ops;
   cl->cl_private = cu;
   cl->cl_errbuf = NULL;
   cu->cu_raddr = *raddr;
   cu->cu_rlen = sizeof (cu->cu_raddr);
   cu->cu_retry = wait;
   cu->cu_wait.tv_sec = 0;
   cu->cu_wait.tv_usec = 0;
   cu->cu_waitset = FALSE;
   cu->cu_sendsz = sendsz;
   cu->cu_recvsz = recvsz;

      /*
      ** Preserialize the RPC message header
      */
   call_msg.rm_xid = RTCS_rand();
   call_msg.rm_direction = CALL;
   call_msg.rm_call.cb_rpcvers = RPC_MSG_VERSION;
   call_msg.rm_call.cb_prog = prog;
   call_msg.rm_call.cb_vers = vers;
   xdrmem_create(&cu->cu_outxdrs, cu->cu_outbuf, sendsz, XDR_ENCODE);
   if (! xdr_callhdr(&cu->cu_outxdrs, &call_msg)) {
      goto create_fail;
   } /* Endif */
   cu->cu_xdrpos = xdr_getpos(&cu->cu_outxdrs);

      /*
      ** Allocate and bind a socket if the caller didn't provide one
      */
   if (*sockp == RPC_ANYSOCK) {
      sockaddr_in laddr;

      *sockp = socket(PF_INET, SOCK_DGRAM, 0);
      if (*sockp == (int_32)RTCS_SOCKET_ERROR) {
         rpc_createerr.cf_stat = RPC_SOCKETERROR;
         rpc_createerr.cf_error.re_errno = *sockp;
         goto create_fail;
      } /* Endif */

      laddr.sin_family = AF_INET;
      laddr.sin_port = 0;
      laddr.sin_addr.s_addr = 0;

      bind(*sockp, &laddr, sizeof(laddr));

      cu->cu_closeit = TRUE;
   } else {
      cu->cu_closeit = FALSE;
   } /* Endif */
   cu->cu_sock = *sockp;

      /*
      ** Use null authentication by default
      */
   cl->cl_auth = authnone_create();

   return (cl);

      /*
      ** If clnt_create failed, free all the memory we allocated.
      */
create_fail:
   if (cu) {
      _mem_free(cu);
   } /* Endif */
   if (cl) {
      _mem_free(cl);
   } /* Endif */
   return (NULL);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_create
* Returned Value   : client handle
* Comments  :  Creates a client handle for UDP/IP based RPC using default
*   buffer sizes.
*
*END*-----------------------------------------------------------------*/

CLIENT_PTR clntudp_create
   (
      sockaddr_in _PTR_          raddr,
         /* [IN] - address of RPC server */
      uint_32                    prog,
         /* [IN] - number of remote program to bind to */
      uint_32                    vers,
         /* [IN] - version of remote program to bind to */
      struct timeval             wait,
         /* [IN] - retry timeout */
      int_32_ptr                 sockp
         /* [IN,OUT] - local socket */
   )
{ /* Body */
   return(clntudp_bufcreate(raddr, prog, vers, wait, sockp,
       UDPMSGSIZE, UDPMSGSIZE));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_call
* Returned Value   : error code
* Comments  :  Calls a remote procedure using UDP/IP based RPC
*
*END*-----------------------------------------------------------------*/

static enum clnt_stat clntudp_call
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
   struct timeval utimeout
         /* [IN] - time to wait before giving up */
   )
{ /* Body */
   register       CLIENT_UDP_PTR cu = (CLIENT_UDP_PTR)cl->cl_private;
   register       XDR_PTR xdrs;
   int_32         outlen;
   XDR            inxdrs;
   int_32         inlen;
   bool_t         inok;
   sockaddr_in    from;
   uint_16        fromlen;
   RPC_MESSAGE    reply_msg;
   uint_32        xid;
   struct         timeval time_waited;
   struct         timeval timeout;
   uint_16        refreshes = 2;
   uint_32        time = (cu->cu_retry.tv_sec * 1000) + \
                     (cu->cu_retry.tv_usec / 1000);

      /*
      ** Use the supplied timeout only if no timeout has been
      ** specified with clnt_control(CLSET_TIMEOUT).
      */
   if (cu->cu_waitset) {
      timeout = cu->cu_wait;
   } else {
      timeout = utimeout;
   } /* Endif */

   time_waited.tv_sec = 0;
   time_waited.tv_usec = 0;

clnt_request:
      /*
      ** The message header is already serialized in the send buffer,
      ** so just move the XDR pointer immediately after the header.
      */
   xdrs = &cu->cu_outxdrs;
   xdrs->x_op = XDR_ENCODE;
   xdr_setpos(xdrs, cu->cu_xdrpos);

      /*
      ** However, the transaction ID must be updated -- it's the first
      ** field in the buffer
      */
   xid = ntohl((uchar_ptr)cu->cu_outbuf);
   xid++;
   htonl((uchar_ptr)cu->cu_outbuf, xid);

      /*
      ** Marshall the rest of the message:  procedure number, credentials,
      ** verifier and arguments.
      */
   if ((! xdr_uint_32(xdrs, &proc)) ||
       (! auth_marshall(cl->cl_auth, xdrs)) ||
       (! (*xargs)(xdrs, argsp))) {
      return (cu->cu_error.re_status = RPC_CANTENCODEARGS);
   } /* Endif */

   outlen = xdr_getpos(xdrs);

clnt_send:
   /*
   ** Send the request
   */
   if (sendto(cu->cu_sock, cu->cu_outbuf, outlen, 0, &cu->cu_raddr, cu->cu_rlen)
       != outlen) {
      cu->cu_error.re_errno = RTCS_geterror(cu->cu_sock);
      return (cu->cu_error.re_status = RPC_CANTSEND);
   } /* Endif */

clnt_recv:
   /*
   ** Added timeout and return error if response is not received before timeout
   */
   if (!RTCS_selectset(&cu->cu_sock, 1, time)) {
      time_waited.tv_sec += cu->cu_retry.tv_sec;
      time_waited.tv_usec += cu->cu_retry.tv_usec;
      while (time_waited.tv_usec >= 1000000) {
         time_waited.tv_sec++;
         time_waited.tv_usec -= 1000000;
      } /* Endwhile */
      if ((time_waited.tv_sec < timeout.tv_sec) ||
         ((time_waited.tv_sec == timeout.tv_sec) &&
          (time_waited.tv_usec < timeout.tv_usec))) {
         goto clnt_send;
      } /* Endif */
      return (cu->cu_error.re_status = RPC_TIMEDOUT);
   } /* Endif */
   fromlen = sizeof(sockaddr_in);
   inlen = recvfrom(cu->cu_sock, cu->cu_inbuf, cu->cu_recvsz, 0,
      &from, &fromlen);
   if (inlen < 0) {
      cu->cu_error.re_errno = RTCS_geterror(cu->cu_sock);
      return (cu->cu_error.re_status = RPC_CANTRECV);
   } /* Endif */

   if (inlen < sizeof(uint_32)) {
      goto clnt_recv;
   } /* Endif */
   if (ntohl((uchar_ptr)cu->cu_inbuf) != xid) {
      goto clnt_recv;
   } /* Endif */

      /*
      ** Assume we have the proper reply.  Decode and validate.
      */
   xdrmem_create(&inxdrs, cu->cu_inbuf, inlen, XDR_DECODE);
   reply_msg.acpted_rply.ar_verf = rpc_null_auth;
   reply_msg.acpted_rply.ar_results.where = resultsp;
   reply_msg.acpted_rply.ar_results.proc = xresults;
   inok = xdr_replymsg(&inxdrs, &reply_msg);
   xdr_destroy(&inxdrs);
   if (!inok) {
      return (cu->cu_error.re_status = RPC_CANTDECODERES);
   } /* Endif */

      /*
      ** We got a valid response from the RPC server.
      ** Update the error struct in the client handle.
      */
   rpc_seterr_reply(&reply_msg, &cu->cu_error);

   if (cu->cu_error.re_status == RPC_SUCCESS) {
         /*
         ** If the server accepted our request, we must
         ** authenticate the reply before we can accept it.
         */
      if (! auth_validate(cl->cl_auth, &reply_msg.acpted_rply.ar_verf)) {
         cu->cu_error.re_status = RPC_AUTHERROR;
         cu->cu_error.re_why = AUTH_INVALIDRESP;
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
   }  /* Endif */

   return (cu->cu_error.re_status);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_geterr
* Returned Value   : void
* Comments  :  Retrieves the last error code from a client call
*
*END*-----------------------------------------------------------------*/

static void clntudp_geterr
   (
      CLIENT_PTR     cl,
            /* [IN] - client handle */
      RPC_ERROR_PTR  errp
            /* [OUT] - last error that occurred on client handle */
   )
{ /* Body */
   CLIENT_UDP_PTR cu = (CLIENT_UDP_PTR)cl->cl_private;

   *errp = cu->cu_error;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_freeres
* Returned Value   : TRUE or FALSE
* Comments  :  Frees the result from a remote procedure call
*
*END*-----------------------------------------------------------------*/

static bool_t clntudp_freeres
   (
      CLIENT_PTR  cl,
            /* [IN] - client handle */
      xdrproc_t   xdr_res,
            /* [IN] - filter used to decode result from remote procedure */
      caddr_t     res_ptr
            /* [IN] - pointer to dynamically allocated result */
   )
{ /* Body */
   CLIENT_UDP_PTR cu = (CLIENT_UDP_PTR)cl->cl_private;
   XDR_PTR xdrs = &cu->cu_outxdrs;

   xdrs->x_op = XDR_FREE;
   return ((*xdr_res)(xdrs, res_ptr));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_abort
* Returned Value   : nothing
* Comments  :  Halts a remote procedure call in progress (not implemented)
*
*END*-----------------------------------------------------------------*/

static void clntudp_abort
   (
      CLIENT_PTR  cl
            /* [IN] - client handle */
   )
{ /* Body */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_control
* Returned Value   : TRUE or FALSE
* Comments  :  ioctl() for remote procedure calls
*
*END*-----------------------------------------------------------------*/

static bool_t clntudp_control
   (
      CLIENT_PTR  cl,
            /* [IN] - client handle */
      int_32      request,
            /* [IN] - control operation to perform */
      char_ptr    info
            /* [IN,OUT] - pointer to control information */
   )
{ /* Body */
   CLIENT_UDP_PTR cu = (CLIENT_UDP_PTR)cl->cl_private;

   switch (request) {
   case CLSET_TIMEOUT:
      cu->cu_wait = *(struct timeval _PTR_)info;
      cu->cu_waitset = TRUE;
      break;
   case CLGET_TIMEOUT:
      *(struct timeval _PTR_)info = cu->cu_wait;
      break;
   case CLSET_RETRY_TIMEOUT:
      cu->cu_retry = *(struct timeval _PTR_)info;
      break;
   case CLGET_RETRY_TIMEOUT:
      *(struct timeval _PTR_)info = cu->cu_retry;
      break;
   case CLGET_SERVER_ADDR:
      *(sockaddr_in _PTR_)info = cu->cu_raddr;
      break;
   default:
      return (FALSE);
   } /* Endswitch */

   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : clntudp_destroy
* Returned Value   : nothing
* Comments  :  Destroys a UDP/IP based RPC client handle
*
*END*-----------------------------------------------------------------*/

static void clntudp_destroy
   (
      CLIENT_PTR  cl
            /* [IN] - client handle */
   )
{ /* Body */
   CLIENT_UDP_PTR cu = (CLIENT_UDP_PTR)cl->cl_private;

   if (cu->cu_closeit) {
      shutdown(cu->cu_sock, 0);
   } /* Endif */
   xdr_destroy(&cu->cu_outxdrs);
   _mem_free(cu);
   if (cl->cl_errbuf) {
      _mem_free(cl->cl_errbuf);
   } /* Endif */
   _mem_free(cl);

} /* Endbody */

#endif
/* EOF */
