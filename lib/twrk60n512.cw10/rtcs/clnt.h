#ifndef __clnt_h__
#define __clnt_h__
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
* $FileName: clnt.h$
* $Version : 3.6.6.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the declarations of the client
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


#define UDPMSGSIZE      1460           /* RTCS limit on UDP msg size */
#define RPCSMALLMSGSIZE 400            /* a more reasonable packet size */
#define CLNTERRBUFSIZE  256            /* maximum size of an error string */

/*
** RPC calls return an enum clnt_stat.  This should be looked at more,
** since each implementation is required to live with this (implementation
** independent) list of errors.
*/
enum clnt_stat {
   RPC_SUCCESS=0,             /* call succeeded */

   /* local errors */
   RPC_CANTENCODEARGS=1,      /* can't encode arguments */
   RPC_CANTDECODERES=2,       /* can't decode results */
   RPC_CANTSEND=3,            /* failure in sending call */
   RPC_CANTRECV=4,            /* failure in receiving result */
   RPC_TIMEDOUT=5,            /* call timed out */

   /* remote errors */
   RPC_VERSMISMATCH=6,        /* rpc versions not compatible */
   RPC_AUTHERROR=7,           /* authentication error */
   RPC_PROGUNAVAIL=8,         /* program not available */
   RPC_PROGVERSMISMATCH=9,    /* program version mismatched */
   RPC_PROCUNAVAIL=10,        /* procedure unavailable */
   RPC_CANTDECODEARGS=11,     /* decode arguments error */
   RPC_SYSTEMERROR=12,        /* generic kernel problem */
   RPC_SOCKETERROR=18,        /* generic transport problem */

   /* callrpc & clnt_create errors */
   RPC_UNKNOWNHOST=13,        /* unknown host name */
   RPC_UNKNOWNPROTO=17,       /* unkown protocol */

   /* _create errors */
   RPC_PMAPFAILURE=14,        /* the pmapper failed in its call */
   RPC_PROGNOTREGISTERED=15,  /* remote program is not registered */

   /* unspecified error */
   RPC_FAILED=16
};


/* Error info */
typedef struct rpc_error {
   enum clnt_stat re_status;
   union {
      int_32 RE_errno;        /* related system error */
      enum auth_stat RE_why;  /* why the auth error occurred */
      struct {
         uint_32 low;         /* lowest verion supported */
         uint_32 high;        /* highest verion supported */
      } RE_vers;
      struct {                /* maybe meaningful if RPC_FAILED */
         int_32 s1;
         int_32 s2;
      } RE_lb;                /* life boot & debugging only */
   } ru;
} RPC_ERROR, _PTR_ RPC_ERROR_PTR;
#define   re_errno   ru.RE_errno
#define   re_why     ru.RE_why
#define   re_vers    ru.RE_vers
#define   re_lb      ru.RE_lb


/*
** Client RPC handle.
** Created by individual implementations, see e.g. rpc_udp.c.
** Client is responsible for initializing auth, see e.g. auth_none.c.
*/
typedef struct client {
   AUTH_PTR    cl_auth;       /* authentication handle */
   struct clnt_ops {
      /*
      ** Added arguments to each function pointer as per the actual function
      */
      /* call remote procedure */
      enum clnt_stat (_CODE_PTR_ cl_call)(void _PTR_, uint_32, xdrproc_t, \
         caddr_t, xdrproc_t, caddr_t, struct timeval);
      /* abort a call */
      void           (_CODE_PTR_ cl_abort)(void _PTR_);
      /* get specific error code */
      void           (_CODE_PTR_ cl_geterr)(void _PTR_, RPC_ERROR _PTR_);
      /* frees results */
      bool_t         (_CODE_PTR_ cl_freeres)(void _PTR_, xdrproc_t, caddr_t);
      /* destroy this structure */
      void           (_CODE_PTR_ cl_destroy)(void _PTR_);
      /* the ioctl() of rpc */
      bool_t         (_CODE_PTR_ cl_control)(void _PTR_, uint_32, char_ptr);
   } _PTR_ cl_ops;
   pointer     cl_private;    /* private stuff */
   char_ptr    cl_errbuf;     /* buffer for error strings */
} CLIENT, _PTR_ CLIENT_PTR;


/*
** client side RPC interface operations
*/

/*
** enum clnt_stat CLNT_CALL
**    (
**       CLIENT_PTR     rh,
**       uint_32        proc,
**       xdrproc_t      xargs,
**       caddr_t        argsp,
**       xdrproc_t      xres,
**       caddr_t        resp,
**       struct timeval timeout
**    )
*/
#define  clnt_call(rh, proc, xargs, argsp, xres, resp, secs)   \
   ((*(rh)->cl_ops->cl_call)(rh, proc, xargs, argsp, xres, resp, secs))

/*
** void CLNT_ABORT
**    (
**       CLIENT_PTR  rh
**    )
*/
#define  clnt_abort(rh)   ((*(rh)->cl_ops->cl_abort)(rh))

/*
** struct rpc_err CLNT_GETERR
**    (
**       CLIENT_PTR  rh
**    )
*/

#define  clnt_geterr(rh,errp)   ((*(rh)->cl_ops->cl_geterr)(rh, errp))

/*
** bool_t CLNT_FREERES
**    (
**       CLIENT_PTR  rh,
**       xdrproc_t   xres,
**       caddr_t     resp
**    )
*/
#define  clnt_freeres(rh,xres,resp) ((*(rh)->cl_ops->cl_freeres)(rh,xres,resp))

/*
** bool_t CLNT_CONTROL
**    (
**      CLIENT_PTR   cl,
**      uint_32      request,
**      char_ptr     info
**    )
*/
#define  clnt_control(cl,rq,in) ((*(cl)->cl_ops->cl_control)(cl,rq,in))

/*
** control operations that apply to both UDP and TCP transports
*/
#define CLSET_TIMEOUT       1   /* set timeout (timeval) */
#define CLGET_TIMEOUT       2   /* get timeout (timeval) */
#define CLGET_SERVER_ADDR   3   /* get server's address (sockaddr) */

/*
** udp only control operations
*/
#define CLSET_RETRY_TIMEOUT 4   /* set retry timeout (timeval) */
#define CLGET_RETRY_TIMEOUT 5   /* get retry timeout (timeval) */

/*
** void CLNT_DESTROY
**    (
**       CLIENT_PTR  rh
**    )
*/
            
#define  clnt_destroy(rh)   ((*(rh)->cl_ops->cl_destroy)(rh))


/*
** RPCTEST is a test program which is accessible on every RPC
** transport/port.  It is used for testing, performance evaluation,
** and network administration.
*/

#define RPCTEST_PROGRAM          ((uint_32)1)
#define RPCTEST_VERSION          ((uint_32)1)
#define RPCTEST_NULL_PROC        ((uint_32)2)
#define RPCTEST_NULL_BATCH_PROC  ((uint_32)3)

/*
** By convention, procedure 0 takes null arguments and returns them
*/

#define NULLPROC ((uint_32)0)

/*
** Below are the client handle creation routines for the various
** implementations of client side RPC.  They can return NULL if a
** creation failure occurs.
*/

/*
** UDP based RPC.
** CLIENT_PTR clntudp_create
**    (
**       struct sockaddr_in *raddr,
**       uint_32     program,
**       uint_32     version,
**       struct timeval wait,
**       int_32_ptr  sockp
**    )
**
** Same as above, but you specify max packet sizes.
** CLIENT_PTR clntudp_bufcreate
**    (
**       struct sockaddr_in *raddr,
**       uint_32     program,
**       uint_32     version,
**       struct timeval wait,
**       int_32_ptr  sockp,
**       uint_32     sendsz,
**       uint_32     recvsz
**    )
*/
extern CLIENT_PTR clntudp_create(sockaddr_in _PTR_,
                                 uint_32, uint_32,
                                 struct timeval,
                                 int_32_ptr);
extern CLIENT_PTR clntudp_bufcreate(sockaddr_in _PTR_,
                                    uint_32, uint_32,
                                    struct timeval,
                                    int_32_ptr,
                                    uint_32, uint_32);


/*
** TCP based RPC
** CLIENT_PTR clnttcp_create(raddr, prog, vers, sockp, sendsz, recvsz)
**    (
**       struct sockaddr_in *raddr,
**       uint_32     prog,
**       uint_32     version,
**       int_32_ptr  sockp,
**       uint_32     sendsz,
**       uint_32     recvsz
**    )
*/
extern CLIENT_PTR clnttcp_create(sockaddr_in _PTR_,
                                 uint_32, uint_32,
                                 int_32_ptr,
                                 uint_32, uint_32);


/*
** Print why creation failed
*/
extern void     clnt_pcreateerror(char_ptr);       /* stderr */
extern char_ptr clnt_spcreateerror(char_ptr);      /* string */

/*
** Like clnt_perror(), but is more verbose in its output
*/
extern void     clnt_perrno(enum clnt_stat);       /* stderr */
extern char_ptr clnt_sperrno(enum clnt_stat);      /* string */

/*
** Print an English error message, given the client error code
*/
extern void     clnt_perror(CLIENT_PTR, char_ptr);    /* stderr */
extern char_ptr clnt_sperror(CLIENT_PTR, char_ptr);   /* string */


/*
** If a creation fails, the following allows the user to figure out why.
*/
struct rpc_createerr {
   enum clnt_stat cf_stat;
   struct rpc_error cf_error;   /* useful when cf_stat == RPC_PMAPFAILURE */
};

extern struct rpc_createerr rpc_createerr;


#endif
/* EOF */
