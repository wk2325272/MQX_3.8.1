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
* $FileName: svctcp.c$
* $Version : 3.8.8.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file contains the implementation of the TCP/IP
*   based server side RPC Library functions.
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

static int_32  readtcp(SVCXPRT_PTR xprt,
      caddr_t buf,
      int_32 len);
static int_32  writetcp(SVCXPRT_PTR xprt,
      caddr_t buf,
      int_32 len);

/*
** Ops vector for TCP/IP based rpc service handle
*/
static enum xprt_stat   svctcp_stat();
static bool_t           svctcp_recv();
static bool_t           svctcp_reply();
static bool_t           svctcp_getargs();
static bool_t           svctcp_freeargs();
static void             svctcp_destroy();
static bool_t           svcrv_invalid( SVCXPRT_PTR xprt );

static struct xp_ops svctcp_op = {
   svctcp_recv,
   svctcp_stat,
   svctcp_getargs,
   svctcp_reply,
   svctcp_freeargs,
   svctcp_destroy
};

/*
** Ops vector for TCP/IP rendezvous handler
*/
static SVCXPRT_PTR      svcrv_create(SVCXPRT_PTR);
static bool_t           svcrv_recv();
static enum xprt_stat   svcrv_stat();
static void             svcrv_destroy();

static struct xp_ops svcrv_op = {
   svcrv_recv,
   svcrv_stat,
   (bool_t (_CODE_PTR_)())svcrv_invalid,
   (bool_t (_CODE_PTR_)())svcrv_invalid,
   (bool_t (_CODE_PTR_)())svcrv_invalid,
   svcrv_destroy,
};


/*
** Private pre-connection data kept in xprt->xp_p1
*/
typedef struct svc_conn {
   uint_32 sc_sendsz;
   uint_32 sc_recvsz;
   bool_t  sc_closeit;
} SVC_CONN, _PTR_ SVC_CONN_PTR;

/*
** Private post-connection data kept in xprt->xp_p1
*/
typedef struct svc_tcp {
   enum xprt_stat st_stat;
   uint_32        st_sendsz;
   uint_32        st_recvsz;
   uint_32        st_xid;
   XDR            st_xdrs;
   char           st_verf[MAX_AUTH_BYTES];
} SVC_TCP, _PTR_ SVC_TCP_PTR;

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : SVCTCP_init
* Returned Value   : error code
* Comments  :  Start the svc_tcp_task.
*
*END*-----------------------------------------------------------------*/

uint_32 SVCTCP_init
   (
      char_ptr name,
      uint_32  priority,
      uint_32  stacksize,
      pointer  creation_parameter
   )
{ /* Body */
   return RTCS_task_create(name, priority,stacksize,svctcp_task,creation_parameter);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_create
* Returned Value   : transport handle
* Comments  :  Creates a transport handle for TCP/IP based RPC
*
* If sock<0 then a TCP socket is created and bound to an arbitrary port.
*
* The TCP/RPC package performs buffering, so the caller must choose send and
* receive buffer sizes (sendsz and recvsz); 0=>default.
*
* Once *xprt is initialized, it is registered as a transporter.
*
*END*-----------------------------------------------------------------*/

SVCXPRT_PTR svctcp_create
   (
      int_32   sock,
         /* [IN] - local socket */
      uint_32  sendsz,
         /* [IN] - size of send buffer */
      uint_32  recvsz
         /* [IN] - size of receive buffer */
   )
{ /* Body */
   register SVCXPRT_PTR  xprt = NULL;
   register SVC_CONN_PTR sc = NULL;
   sockaddr_in laddr;
   uint_16 llen = sizeof(laddr);

      /*
      ** Allocate a transport handle
      */
   xprt = RTCS_mem_alloc_zero(sizeof(SVCXPRT));
   if (xprt == NULL) {
      fprintf(stderr, "svctcp_create: out of memory\n");
      goto create_fail;
   } /* Endif */

      /*
      ** Allocate a private handle
      */
   sc = RTCS_mem_alloc_zero(sizeof(SVC_CONN));
   if (sc == NULL) {
      fprintf(stderr, "svctcp_create: out of memory\n");
      goto create_fail;
   } /* Endif */
   xprt->xp_p1 = (caddr_t)sc;
   xprt->xp_p2 = NULL;

      /*
      ** Initialize the private handle
      */
   sc->sc_sendsz = sendsz;
   sc->sc_recvsz = recvsz;
   xprt->xp_ops = &svcrv_op;
   xprt->xp_verf = rpc_null_auth;

      /*
      ** Allocate and bind a socket if the caller didn't provide one
      ** If the caller did provide one, we need to find its local port
      */
   if (sock == RPC_ANYSOCK) {

      sock = socket(PF_INET, SOCK_STREAM, 0);
      if (sock == (int_32)RTCS_SOCKET_ERROR) {
         goto create_fail;
      } /* Endif */

      laddr.sin_family = AF_INET;
      laddr.sin_port = 0;
      laddr.sin_addr.s_addr = 0;

      bind(sock, &laddr, sizeof(laddr));
      listen(sock, 0);

      sc->sc_closeit = TRUE;
   } else {
      sc->sc_closeit = FALSE;
   } /* Endif */

   if (getsockname(sock, &laddr, &llen) != RTCS_OK) {
      goto create_fail;
   } /* Endif */

   xprt->xp_sock = sock;
   xprt->xp_port = laddr.sin_port;

      /*
      ** Register the transport
      */
   xprt_register(xprt);
   return (xprt);

      /*
      ** If svc_create failed, free all the memory we allocated.
      */
create_fail:
   if (sc) {
      _mem_free(sc);
   } /* Endif */
   if (xprt) {
      _mem_free(xprt);
   } /* Endif */
   return (NULL);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcrv_invalid
* Returned Value   : none
* Comments  :  Used for invalid service
*
*END*-----------------------------------------------------------------*/

static bool_t svcrv_invalid
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   return FALSE;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcrv_create
* Returned Value   : transport handle
* Comments  :  Creates a transport handle for TCP/IP based RPC
*
* If sock<0 then a TCP socket is created and bound to an arbitrary port.
*
* The TCP/RPC package performs buffering, so the caller must choose send and
* receive buffer sizes (sendsz and recvsz); 0=>default.
*
* Once *xprt is initialized, it is registered as a transporter.
*
*END*-----------------------------------------------------------------*/

static SVCXPRT_PTR svcrv_create
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   SVC_CONN_PTR sc = (SVC_CONN_PTR)(xprt->xp_p1);
   SVC_TCP_PTR  st = NULL;

      /*
      ** Allocate a transport handle
      */
   xprt = RTCS_mem_alloc_system_zero(sizeof(SVCXPRT));
   if (xprt == NULL) {
      fprintf(stderr, "svctcp_fork: out of memory\n");
      return (NULL);
   } /* Endif */

      /*
      ** Allocate a private handle
      */
   st = RTCS_mem_alloc_system_zero(sizeof(SVC_TCP));
   if (st == NULL) {
      fprintf(stderr, "svctcp_fork: out of memory\n");
      _mem_free(xprt);
      return (NULL);
   } /* Endif */
   xprt->xp_p1 = (caddr_t)st;
   xprt->xp_p2 = NULL;

      /*
      ** Initialize the private handle
      */
   st->st_stat = XPRT_MOREREQS;
   st->st_sendsz = sc->sc_sendsz;
   st->st_recvsz = sc->sc_recvsz;
   xprt->xp_ops = &svctcp_op;
   xprt->xp_verf.oa_base = st->st_verf;

   return (xprt);
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcrv_recv
* Returned Value   : status of transport
* Comments  :  Returns the current state of a transport
*
*END*-----------------------------------------------------------------*/

static bool_t svcrv_recv
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   int_32 sock;
   sockaddr_in raddr;
   uint_16 rlen = sizeof(raddr);
   int_32 error;

      /*
      ** Accept the new connection
      */
   sock = accept(xprt->xp_sock, &raddr, &rlen);
   if (sock == (int_32)RTCS_SOCKET_ERROR) {
      return (FALSE);
   } /* Endif */

      /*
      ** Create a new transporter (re-uses xprt)
      */
   xprt = svcrv_create(xprt);
   if (xprt == NULL) {
      shutdown(sock, FLAG_CLOSE_TX);
      return (FALSE);
   } /* Endif */
   xprt->xp_sock = sock;
   xprt->xp_raddr = raddr;
   xprt->xp_addrlen = rlen;

      /*
      ** Create a task to handle the new connection
      */
   error = RTCS_task_create("svctcp_task", _SVCTCPTASK_priority,
      _SVCTCPTASK_stacksize + 1000,
      svctcp_task, xprt);
   if (error) {
      shutdown(sock, FLAG_CLOSE_TX);
      _mem_free(xprt->xp_p1);
      _mem_free(xprt);
      return (FALSE);
   } /* Endif */

   RTCS_detachsock(sock);

      /*
      ** There is never an RPC request to be processed.
      */
   return (FALSE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcrv_stat
* Returned Value   : status of transport
* Comments  :  Returns the current state of a transport
*
*END*-----------------------------------------------------------------*/

static enum xprt_stat svcrv_stat
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   return (XPRT_IDLE);
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcrv_destroy
* Returned Value   : void
* Comments  :  Destroys a TCP/IP based transport handle
*
*END*-----------------------------------------------------------------*/

static void svcrv_destroy
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   register SVC_CONN_PTR sc = (SVC_CONN_PTR)(xprt->xp_p1);

   xprt_unregister(xprt);
   if (sc->sc_closeit) {
      shutdown(xprt->xp_sock, FLAG_CLOSE_TX);
   } /* Endif */
   _mem_free(sc);
   _mem_free(xprt);
} /* Endbody */


/*TASK*-----------------------------------------------------------------
*
* Function Name    : svctcp_task
* Comments  :  Slave task created by svcrv_recv to service a connected
*     TCP/IP transport.
*
*END*-----------------------------------------------------------------*/

void svctcp_task
   (
      pointer  parameter,
         /* [IN] - transport handle */
      pointer  creator
   )
{ /* Body */
   SVCXPRT_PTR xprt = parameter;
   SVC_TCP_PTR st = (SVC_TCP_PTR)(xprt->xp_p1);

   RTCS_attachsock(xprt->xp_sock);
   xdrrec_create( &st->st_xdrs, st->st_sendsz, st->st_recvsz,
       (caddr_t)xprt, readtcp, writetcp );

   RTCS_task_resume_creator(creator, RTCS_OK);

   svc_getreq(xprt);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_stat
* Returned Value   : status of transport
* Comments  :  Returns the current state of a transport
*
*END*-----------------------------------------------------------------*/

static enum xprt_stat svctcp_stat
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   return (((SVC_TCP_PTR)xprt->xp_p1)->st_stat);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_recv
* Returned Value   : TRUE or FALSE
* Comments  :  Retrieves an RPC request from a transport
*
*END*-----------------------------------------------------------------*/

static bool_t svctcp_recv
   (
      SVCXPRT_PTR       xprt,
         /* [IN] - transport handle */
      RPC_MESSAGE_PTR   msg
         /* [OUT] - RPC request */
   )
{ /* Body */
   register SVC_TCP_PTR st = (SVC_TCP_PTR)(xprt->xp_p1);
   XDR_PTR xdrs = &(st->st_xdrs);

   xdrs->x_op = XDR_DECODE;
   xdrrec_skiprecord(xdrs);
   if (!xdr_callmsg(xdrs, msg)) {
      return (FALSE);
   } /* Endif */
   st->st_xid = msg->rm_xid;

   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_reply
* Returned Value   : TRUE or FALSE
* Comments  :  Replies to an RPC request
*
*END*-----------------------------------------------------------------*/

static bool_t svctcp_reply
   (
      SVCXPRT_PTR       xprt,
         /* [IN] - transport handle */
      RPC_MESSAGE_PTR   msg
         /* [IN] - RPC reply */
   )
{ /* Body */
   register SVC_TCP_PTR st = (SVC_TCP_PTR)(xprt->xp_p1);
   XDR_PTR xdrs = &(st->st_xdrs);
   bool_t stat;

   xdrs->x_op = XDR_ENCODE;
   msg->rm_xid = st->st_xid;
   stat = xdr_replymsg(xdrs, msg);
   xdrrec_endofrecord(xdrs, TRUE);
   return (stat);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_getargs
* Returned Value   : TRUE or FALSE
* Comments  :  Decodes the arguments of an RPC request
*
*END*-----------------------------------------------------------------*/

static bool_t svctcp_getargs
   (
      SVCXPRT_PTR    xprt,
         /* [IN] - transport handle */
      xdrproc_t      xdr_args,
         /* [IN] - xdr filter for arguments */
      caddr_t        args_ptr
         /* [IN,OUT] - pointer to arguments */
   )
{ /* Body */
   return ((*xdr_args)(&((SVC_TCP_PTR)xprt->xp_p1)->st_xdrs, args_ptr));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_freeargs
* Returned Value   : TRUE or FALSE
* Comments  :  Frees the arguments from an RPC request
*
*END*-----------------------------------------------------------------*/

static bool_t svctcp_freeargs
   (
      SVCXPRT_PTR xprt,
         /* [IN] - transport handle */
      xdrproc_t xdr_args,
         /* [IN] - xdr filter for arguments */
      caddr_t args_ptr
         /* [IN] - pointer to arguments */
   )
{ /* Body */
   XDR_PTR xdrs = &(((SVC_TCP_PTR)xprt->xp_p1)->st_xdrs);

   xdrs->x_op = XDR_FREE;
   return ((*xdr_args)(xdrs, args_ptr));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svctcp_destroy
* Returned Value   : void
* Comments  :  Destroys a TCP/IP based transport handle
*
*END*-----------------------------------------------------------------*/

static void svctcp_destroy
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   register SVC_TCP_PTR st = (SVC_TCP_PTR)(xprt->xp_p1);

   shutdown(xprt->xp_sock, FLAG_CLOSE_TX);
   xdr_destroy(&st->st_xdrs);
   _mem_free(st);
   _mem_free(xprt);
} /* Endbody */


/*
** Interface between XDR serializer and TCP connection.
*/


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : readtcp
* Returned Value   : number of bytes read
* Comments  :  Receives bytes from a connected TCP socket.
*     Any error is fatal and the connection is closed.
*
*END*-----------------------------------------------------------------*/

static int_32 readtcp
   (
      SVCXPRT_PTR xprt,
      caddr_t buf,
      int_32 len
   )
{ /* Body */

   if ((len = recv(xprt->xp_sock, buf, len, 0)) <= 0) {
      ((SVC_TCP_PTR)xprt->xp_p1)->st_stat = XPRT_DIED;
      len = -1;
   } /* Endif */

   return (len);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : writetcp
* Returned Value   : number of bytes written
* Comments  :  Sends bytes out a connected TCP socket.
*     Any error is fatal and the connection is closed.
*
*END*-----------------------------------------------------------------*/

static int_32 writetcp
   (
      SVCXPRT_PTR xprt,
      caddr_t buf,
      int_32 len
   )
{ /* Body */
   register int_32 i, cnt;

   for (cnt = len; cnt > 0; cnt -= i, buf += i) {
      if ((i = send(xprt->xp_sock, buf, cnt, 0)) < 0) {
         ((SVC_TCP_PTR)xprt->xp_p1)->st_stat = XPRT_DIED;
         return (-1);
      } /* Endif */
   } /* Endfor */

   return (len);
} /* Endbody */

/* EOF */
