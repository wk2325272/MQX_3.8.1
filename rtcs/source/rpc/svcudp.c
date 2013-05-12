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
* $FileName: svcudp.c$
* $Version : 3.5.8.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the implementation of the UDP/IP
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

#if RTCSCFG_ENABLE_UDP

static bool_t           svcudp_recv();
static bool_t           svcudp_reply();
static enum xprt_stat   svcudp_stat();
static bool_t           svcudp_getargs();
static bool_t           svcudp_freeargs();
static void             svcudp_destroy();

static struct xp_ops svcudp_op = {
   svcudp_recv,
   svcudp_stat,
   svcudp_getargs,
   svcudp_reply,
   svcudp_freeargs,
   svcudp_destroy
};

/*
** Private data kept in xprt->xp_p1
*/
typedef struct svc_udp {
   uint_32  su_iosz;                   /* byte size of send.recv buffer */
   bool_t   su_closeit;                /* whether to close socket */
   uint_32  su_xid;                    /* transaction id */
   XDR      su_xdrs;                   /* XDR handle */
   char     su_verf[MAX_AUTH_BYTES];   /* verifier body */
} SVC_UDP, _PTR_ SVC_UDP_PTR;

#define su_data(xprt)   ((SVC_UDP_PTR)(xprt)->xp_p1)
#define su_buffer(xprt) ((xprt)->xp_p2)

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_bufcreate
* Returned Value   : transport handle
* Comments  :  Creates a transport handle for UDP/IP based RPC
*
* If sock<0 then a UDP socket is created and bound to an arbitrary port.
*
* sendsz and recvsz are the maximum allowable packet sizes that can be
* sent and received.
*
* Once *xprt is initialized, it is registered as a transporter.
*
*END*-----------------------------------------------------------------*/

SVCXPRT_PTR svcudp_bufcreate
   (
      int_32      sock,
         /* [IN] - local socket */
      uint_32     sendsz,
         /* [IN] - maximum size of send buffer */
      uint_32     recvsz
         /* [IN] - maximum size of receive buffer */
   )
{ /* Body */
   register SVCXPRT_PTR xprt = NULL;
   register SVC_UDP_PTR su = NULL;
            caddr_t     sd = NULL;
   sockaddr_in laddr;
   uint_16 llen = sizeof(laddr);

      /*
      ** Allocate a transport handle
      */
   xprt = (SVCXPRT_PTR)RTCS_mem_alloc_zero(sizeof(SVCXPRT));
   if (xprt == NULL) {
      fprintf(stderr, "svcudp_create: out of memory\n");
      goto create_fail;
   } /* Endif */

      /*
      ** Allocate a private handle
      */
   su = (SVC_UDP_PTR)RTCS_mem_alloc_zero(sizeof(SVC_UDP));
   if (su == NULL) {
      fprintf(stderr, "svcudp_create: out of memory\n");
      goto create_fail;
   } /* Endif */
   xprt->xp_p1 = (caddr_t)su;

      /*
      ** Allocate a buffer
      */
   if (sendsz > recvsz) {
      su->su_iosz = XDRUNIT_RNDUP(sendsz);
   } else {
      su->su_iosz = XDRUNIT_RNDUP(recvsz);
   } /* Endif */
   sd = RTCS_mem_alloc_zero(su->su_iosz);
   if (sd == NULL) {
      fprintf(stderr, "svcudp_create: out of memory\n");
      goto create_fail;
   } /* Endif */
   xprt->xp_p2 = sd;

      /*
      ** Initialize the private handle
      */
   xdrmem_create(&su->su_xdrs, su_buffer(xprt), su->su_iosz, XDR_DECODE);
   xprt->xp_ops = &svcudp_op;
   xprt->xp_verf.oa_base = su->su_verf;

      /*
      ** Allocate and bind a socket if the caller didn't provide one
      ** If the caller did provide one, we need to find its local port
      */
   if (sock == RPC_ANYSOCK) {

      sock = socket(PF_INET, SOCK_DGRAM, 0);
      if (sock == (int_32)RTCS_SOCKET_ERROR) {
         goto create_fail;
      } /* Endif */

      laddr.sin_family = AF_INET;
      laddr.sin_port = 0;
      laddr.sin_addr.s_addr = 0;

      bind(sock, &laddr, sizeof(laddr));

      su->su_closeit = TRUE;
   } else {
      su->su_closeit = FALSE;
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
   if (sd) {
      _mem_free(sd);
   } /* Endif */
   if (su) {
      _mem_free(su);
   } /* Endif */
   if (xprt) {
      _mem_free(xprt);
   } /* Endif */
   return (NULL);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_create
* Returned Value   : transport handle
* Comments  :  Creates a transport handle for UDP/IP based RPC using
*   default buffer sizes.
*
*END*-----------------------------------------------------------------*/

SVCXPRT_PTR svcudp_create
   (
      int_32         sock
         /* [IN] - local socket */
   )
{ /* Body */
   return(svcudp_bufcreate(sock, UDPMSGSIZE, UDPMSGSIZE));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_stat
* Returned Value   : status of transport
* Comments  :  Returns the current state of a transport
*
*END*-----------------------------------------------------------------*/

static enum xprt_stat svcudp_stat
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   return (XPRT_IDLE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_recv
* Returned Value   : TRUE or FALSE
* Comments  :  Retrieves an RPC request from a transport
*
*END*-----------------------------------------------------------------*/

static bool_t svcudp_recv
   (
      SVCXPRT_PTR       xprt,
         /* [IN] - transport handle */
      RPC_MESSAGE_PTR   msg
         /* [OUT] - RPC request */
   )
{ /* Body */
   register SVC_UDP_PTR su = (SVC_UDP_PTR)su_data(xprt);
   XDR_PTR xdrs = &(su->su_xdrs);
   int_32 rlen;

   xprt->xp_addrlen = sizeof(sockaddr_in);
   rlen = recvfrom(xprt->xp_sock, su_buffer(xprt), su->su_iosz, 0,
                   &xprt->xp_raddr, &xprt->xp_addrlen);
   if (rlen < 0) {
      return (FALSE);
   } /* Endif */
   xdrs->x_op = XDR_DECODE;
   xdr_setpos(xdrs, 0);
   if (!xdr_callmsg(xdrs, msg)) {
      return (FALSE);
   } /* Endif */
   su->su_xid = msg->rm_xid;

   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_reply
* Returned Value   : TRUE or FALSE
* Comments  :  Replies to an RPC request
*
*END*-----------------------------------------------------------------*/

static bool_t svcudp_reply
   (
      SVCXPRT_PTR       xprt,
         /* [IN] - transport handle */
      RPC_MESSAGE_PTR   msg
         /* [IN] - RPC reply */
   )
{ /* Body */
   register SVC_UDP_PTR su = (SVC_UDP_PTR)su_data(xprt);
   XDR_PTR xdrs = &(su->su_xdrs);
   int_32 slen;

   xdrs->x_op = XDR_ENCODE;
   xdr_setpos(xdrs, 0);
   msg->rm_xid = su->su_xid;
   if (xdr_replymsg(xdrs, msg)) {
      slen = xdr_getpos(xdrs);
      if (sendto(xprt->xp_sock, su_buffer(xprt), slen, 0,
                 &(xprt->xp_raddr), xprt->xp_addrlen)     == slen) {
         return (TRUE);
      } /* Endif */
   } /* Endif */

   return (FALSE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_getargs
* Returned Value   : TRUE or FALSE
* Comments  :  Decodes the arguments of an RPC request
*
*END*-----------------------------------------------------------------*/

static bool_t svcudp_getargs
   (
      SVCXPRT_PTR xprt,
         /* [IN] - transport handle */
      xdrproc_t xdr_args,
         /* [IN] - xdr filter for arguments */
      caddr_t args_ptr
         /* [IN,OUT] - pointer to arguments */
   )
{ /* Body */
   return ((*xdr_args)(&(su_data(xprt)->su_xdrs), args_ptr));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_freeargs
* Returned Value   : TRUE or FALSE
* Comments  :  Frees the arguments from an RPC request
*
*END*-----------------------------------------------------------------*/

static bool_t svcudp_freeargs
   (
      SVCXPRT_PTR xprt,
         /* [IN] - transport handle */
      xdrproc_t xdr_args,
         /* [IN] - xdr filter for arguments */
      caddr_t args_ptr
         /* [IN] - pointer to arguments */
   )
{ /* Body */
   XDR_PTR xdrs = &(su_data(xprt)->su_xdrs);

   xdrs->x_op = XDR_FREE;
   return ((*xdr_args)(xdrs, args_ptr));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svcudp_destroy
* Returned Value   : void
* Comments  :  Destroys a UDP/IP based transport handle
*
*END*-----------------------------------------------------------------*/

static void svcudp_destroy
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport handle */
   )
{ /* Body */
   SVC_UDP_PTR su = (SVC_UDP_PTR)su_data(xprt);

   xprt_unregister(xprt);
   if (su->su_closeit) {
      shutdown(xprt->xp_sock, 0);
   } /* Endif */
   xdr_destroy(&su->su_xdrs);
   _mem_free(su_buffer(xprt));
   _mem_free(su);
   _mem_free(xprt);
} /* Endbody */

#endif
/* EOF */
