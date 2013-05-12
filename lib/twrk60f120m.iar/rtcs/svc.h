#ifndef __svc_h__
#define __svc_h__
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
* $FileName: svc.h$
* $Version : 3.8.6.0$
* $Date    : Aug-30-2011$
*
* Comments:
*
*   This file contains the declaration of the server
*   side RPC Library functions.
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


/*
** This interface must manage two items concerning remote procedure calling:
**
** 1) An arbitrary number of transport connections upon which RPC requests
** are received.  The two most notable transports are TCP and UDP; they are
** created and registered by routines in svctcp.c and svcudp.c, respectively;
** they in turn call xprt_register() and xprt_unregister().
**
** 2) An arbitrary number of locally registered services.  Services are
** described by the following four data: program number, version number,
** "service dispatch" function, a transport handle, and a boolean that
** indicates whether or not the exported program should be registered with a
** local binder service; if true, the program's number and version and the
** port number from the transport handle are registered with the binder.
** These data are registered with the RPC svc system via svc_register().
**
** A service's dispatch function is called whenever an RPC request comes in
** on a transport.  The request's program and version numbers must match
** those of the registered service.  The dispatch function is passed two
** parameters, struct svc_req * and SVCXPRT *, defined below.
*/

extern uint_32 _SVCTCPTASK_priority;
extern uint_32 _SVCTCPTASK_stacksize;

enum xprt_stat {
   XPRT_DIED,
   XPRT_MOREREQS,
   XPRT_IDLE
};

/*
** Server side transport handle
*/
typedef struct svcxprt {
   struct svcxprt _PTR_ xp_next; /* ptr to next in linked list */
   uint_32              xp_sock; /* associated socket */
   uint_16              xp_port; /* associated port number */
   struct xp_ops {
      /*
      ** Added arguments to each function pointer as per the actual function
      */
      /* receive incomming requests */
      bool_t         (_CODE_PTR_ xp_recv)(void _PTR_, struct rpc_message _PTR_);
      /* get transport status */
      enum xprt_stat (_CODE_PTR_ xp_stat)(void _PTR_);
      /* get arguments */
      bool_t         (_CODE_PTR_ xp_getargs)(void _PTR_, xdrproc_t, caddr_t);
      /* send reply */
      bool_t         (_CODE_PTR_ xp_reply)(void _PTR_, struct rpc_message _PTR_);
      /* free mem allocated for args */
      bool_t         (_CODE_PTR_ xp_freeargs)(void _PTR_, xdrproc_t, caddr_t);
      /* destroy this struct */
      void           (_CODE_PTR_ xp_destroy)(void _PTR_);
   } _PTR_ xp_ops;
   uint_16        xp_addrlen;    /* length of remote address */
   sockaddr_in    xp_raddr;      /* remote address */
   opaque_auth    xp_verf;       /* raw response verifier */
   caddr_t        xp_p1;         /* private */
   caddr_t        xp_p2;         /* private */
} SVCXPRT, _PTR_ SVCXPRT_PTR;

/*
**  Approved way of getting address of caller
*/
#define svc_getcaller(x) (&(x)->xp_raddr)

/*
** Operations defined on an SVCXPRT handle
**
** SVCXPRT        *xprt;
** struct rpc_msg *msg;
** xdrproc_t      xargs;
** caddr_t        argsp;
*/
#define svc_recv(xprt, msg)               \
   (*(xprt)->xp_ops->xp_recv)((xprt), (msg))

#define svc_stat(xprt)                    \
   (*(xprt)->xp_ops->xp_stat)(xprt)

#define svc_getargs(xprt, xargs, argsp)   \
   (*(xprt)->xp_ops->xp_getargs)((xprt), (xargs), (caddr_t)(argsp))

#define svc_reply(xprt, msg)              \
   (*(xprt)->xp_ops->xp_reply) ((xprt), (msg))

#define svc_freeargs(xprt, xargs, argsp)  \
   (*(xprt)->xp_ops->xp_freeargs)((xprt), (xargs), (argsp))

#define svc_destroy(xprt)                 \
   (*(xprt)->xp_ops->xp_destroy)(xprt)


/*
** Service request
*/
typedef struct svcreq {
   uint_32        rq_prog;       /* service program number */
   uint_32        rq_vers;       /* service protocol version */
   uint_32        rq_proc;       /* the desired procedure */
   opaque_auth    rq_cred;       /* raw creds from the wire */
   caddr_t        rq_clntcred;   /* read only cooked cred */
   SVCXPRT_PTR    rq_xprt;       /* associated transport */
} SVCREQ, _PTR_ SVCREQ_PTR;


/*
** Service registration
**
** svc_register(xprt, prog, vers, dispatch, protocol)
** SVCXPRT *xprt;
** u_long prog;
** u_long vers;
** void (*dispatch)();
** int protocol;  * like TCP or UDP
*/
extern bool_t svc_register (SVCXPRT_PTR, uint_32, uint_32, void (_CODE_PTR_)(),
                            int_32);

/*
** Service unregistration
**
** svc_unregister(prog, vers)
** u_long prog;
** u_long vers;
*/
extern void svc_unregister (uint_32, uint_32);

/*
** Transport registration.
**
** xprt_register(xprt)
** SVCXPRT *xprt;
*/
extern void xprt_register (SVCXPRT_PTR);

/*
** Transport unregistration
**
** xprt_unregister(xprt)
** SVCXPRT *xprt;
*/
extern void xprt_unregister (SVCXPRT_PTR);


/*
** When the service routine is called, it must first check to see if it
** knows about the procedure;  if not, it should call svcerr_noproc
** and return.  If so, it should deserialize its arguments via
** SVC_GETARGS (defined above).  If the deserialization does not work,
** svcerr_decode should be called followed by a return.  Successful
** decoding of the arguments should be followed the execution of the
** procedure's code and a call to svc_sendreply.
**
** Also, if the service refuses to execute the procedure due to too-
** weak authentication parameters, svcerr_weakauth should be called.
** Note: do not confuse access-control failure with weak authentication!
**
** NB: In pure implementations of rpc, the caller always waits for a reply
** msg.  This message is sent when svc_sendreply is called.
** Therefore pure service implementations should always call
** svc_sendreply even if the function logically returns void;  use
** xdr.h - xdr_void for the xdr routine.  HOWEVER, tcp based rpc allows
** for the abuse of pure rpc via batched calling or pipelining.  In the
** case of a batched call, svc_sendreply should NOT be called since
** this would send a return message, which is what batching tries to avoid.
** It is the service/protocol writer's responsibility to know which calls are
** batched and which are not.  Warning: responding to batch calls may
** deadlock the caller and server processes!
*/

extern bool_t  svc_sendreply     (SVCXPRT_PTR, xdrproc_t, caddr_t);
extern void    svcerr_decode     (SVCXPRT_PTR);
extern void    svcerr_weakauth   (SVCXPRT_PTR);
extern void    svcerr_noproc     (SVCXPRT_PTR);
extern void    svcerr_progvers   (SVCXPRT_PTR, uint_32, uint_32);
extern void    svcerr_auth       (SVCXPRT_PTR, enum auth_stat);
extern void    svcerr_noprog     (SVCXPRT_PTR);
extern void    svcerr_systemerr  (SVCXPRT_PTR);

/*
** Lowest level dispatching -OR- who owns this process anyway.
** Somebody has to wait for incoming requests and then call the correct
** service routine.  The routine svc_run does infinite waiting; i.e.,
** svc_run never returns.
** Since another (co-existant) package may wish to selectively wait for
** incoming calls or other events outside of the rpc architecture, the
** routine svc_getreq is provided.  It must be passed readfds, the
** "in-place" results of a select system call (see select, section 2).
*/

/*
** a small program implemented by the svc_rpc implementation itself;
** also see clnt.h for protocol numbers.
*/
extern void rpctest_service(void);

/*
** The portmapper RPC server
*/

extern bool_t pmap_init (void);

/*
** The functions that actually service RPC requests
*/

extern void svc_getreq  (SVCXPRT_PTR);
extern void svc_run     (void);

/*
** Socket to use on svcxxx_create call to get default socket
*/
#define  RPC_ANYSOCK -1

/*
** These are the existing service side transport implementations
*/

/*
** UDP based RPC.
*/
extern SVCXPRT_PTR svcudp_create    (int_32);
extern SVCXPRT_PTR svcudp_bufcreate (int_32, uint_32, uint_32);


/*
** TCP based RPC.
*/
extern SVCXPRT_PTR svctcp_create (int_32, uint_32, uint_32);

/*
** The TCP slave task
*/
#define SVCTCP_TASK  (0x60)
extern void svctcp_task (pointer, pointer);
extern uint_32 SVCTCP_init
   (
      char_ptr name,
      uint_32  priority,
      uint_32  stacksize,
      pointer  creation_parameter
   );
#endif
/* EOF */
