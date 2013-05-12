#ifndef __rpcmsg_h__
#define __rpcmsg_h__
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
* $FileName: rpcmsg.h$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the declaration of the RPC
*   message headers.
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


#define RPC_MSG_VERSION    ((uint_32) 2)
#define RPC_SERVICE_PORT   ((uint_16) 2048)

/*
** Bottom up definition of an rpc message.
** NOTE: call and reply use the same overall stuct but
** different parts of unions within it.
*/

enum msg_type {
   CALL=0,
   REPLY=1
};

enum reply_stat {
   MSG_ACCEPTED=0,
   MSG_DENIED=1
};

enum accept_stat {
   SUCCESS=0,
   PROG_UNAVAIL=1,
   PROG_MISMATCH=2,
   PROC_UNAVAIL=3,
   GARBAGE_ARGS=4,
   SYSTEM_ERR=5
};

enum reject_stat {
   RPC_MISMATCH=0,
   AUTH_ERROR=1
};

/*
** Reply part of an RPC exchange
*/

/*
** Reply to an RPC request that was accepted by the server.
** Note:  There could be an error even though the request was accepted.
*/
struct accepted_reply {
   opaque_auth          ar_verf;
   enum accept_stat     ar_stat;
   union {
      struct {
         uint_32  low;
         uint_32  high;
      } AR_versions;
      struct {
         caddr_t  where;
         xdrproc_t proc;
      } AR_results;
      /* and many other null cases */
   } ru;
#define  ar_results  ru.AR_results
#define  ar_vers     ru.AR_versions
};

/*
** Reply to an RPC request that was rejected by the server.
*/
struct rejected_reply {
   enum reject_stat rj_stat;
   union {
      struct {
         uint_32 low;
         uint_32 high;
      } RJ_versions;
      enum auth_stat RJ_why;  /* why authentication did not work */
   } ru;
#define  rj_vers  ru.RJ_versions
#define  rj_why   ru.RJ_why
};

/*
** Body of a reply to an RPC request.
*/
struct reply_body {
   enum reply_stat rp_stat;
   union {
      struct accepted_reply RP_ar;
      struct rejected_reply RP_dr;
   } ru;
#define  rp_acpt  ru.RP_ar
#define  rp_rjct  ru.RP_dr
};

/*
** Body of an RPC request call.
*/
struct call_body {
   uint_32        cb_rpcvers; /* must be equal to two */
   uint_32        cb_prog;
   uint_32        cb_vers;
   uint_32        cb_proc;
   opaque_auth    cb_cred;
   opaque_auth    cb_verf;    /* protocol specific - provided by client */
};

/*
** The RPC message
*/
typedef struct rpc_message {
   uint_32           rm_xid;
   enum msg_type     rm_direction;
   union {
      struct call_body  RM_cmb;
      struct reply_body RM_rmb;
   } ru;
#define  rm_call     ru.RM_cmb
#define  rm_reply    ru.RM_rmb
} RPC_MESSAGE, _PTR_ RPC_MESSAGE_PTR;

#define  acpted_rply ru.RM_rmb.ru.RP_ar
#define  rjcted_rply ru.RM_rmb.ru.RP_dr


/*
** XDR routine to handle a rpc message.
** xdr_callmsg(xdrs, cmsg)
**    XDR *xdrs;
**    struct rpc_msg *cmsg;
*/
extern bool_t xdr_callmsg (XDR_PTR, RPC_MESSAGE_PTR);

/*
** XDR routine to pre-serialize the static part of a rpc message.
** xdr_callhdr(xdrs, cmsg)
**    XDR *xdrs;
**    struct rpc_msg *cmsg;
*/
extern bool_t xdr_callhdr (XDR_PTR, RPC_MESSAGE_PTR);

/*
** XDR routine to handle a rpc reply.
** xdr_replymsg(xdrs, rmsg)
**    XDR *xdrs;
**    struct rpc_msg *rmsg;
*/
extern bool_t xdr_replymsg (XDR_PTR, RPC_MESSAGE_PTR);

/*
** Fills in the error part of a reply message.
** rpc_seterr_reply(msg, error)
**    struct rpc_msg *msg;
**    struct rpc_err *error;
*/
extern void rpc_seterr_reply (RPC_MESSAGE_PTR, RPC_ERROR_PTR);

#endif
/* EOF */
