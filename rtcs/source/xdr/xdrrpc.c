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
* $FileName: xdrrpc.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the implementation of the XDR
*   filters for the RPC message headers.
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

const opaque_auth rpc_null_auth = {AUTH_NONE, NULL, 0};

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_callmsg
* Returned Value   : TRUE or FALSE
* Comments  :  XDR an opaque authentication struct
*
*END*-----------------------------------------------------------------*/

bool_t xdr_opaque_auth
   (
      register XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      register opaque_auth_ptr   ap
            /* [IN.OUT] - pointer to the opaque authentication struct */
   )
{ /* Body */
   return (
      xdr_enum(xdrs, &(ap->oa_flavor)) &&
      xdr_bytes(xdrs, &ap->oa_base, &ap->oa_length, MAX_AUTH_BYTES));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_accepted_reply
* Returned Value   : TRUE or FALSE
* Comments  :  XDR an accepted_reply struct from an RPC reply union
*
*END*-----------------------------------------------------------------*/

static bool_t xdr_accepted_reply
   (
      register XDR_PTR                 xdrs,
            /* [IN] - XDR stream handle */
      register struct accepted_reply   _PTR_ ar
            /* [IN,OUT] - pointer to the accepted reply struct */
   )
{ /* Body */

   if (! xdr_opaque_auth(xdrs, &(ar->ar_verf))) {
      return (FALSE);
   } /* Endif */

   /*
   ** We could use xdr_union, but then we'd have to write xdrproc_t's
   ** for handling the SUCCESS and PROG_MISMATCH codes.  Instead, we
   ** filter the union manually.
   */
   if (! xdr_enum(xdrs, (enum_t _PTR_)&(ar->ar_stat))) {
      return (FALSE);
   } /* Endif */

   switch (ar->ar_stat) {

   case SUCCESS:
      return ((*(ar->ar_results.proc))(xdrs, ar->ar_results.where));

   case PROG_MISMATCH:
      if (! xdr_uint_32(xdrs, &(ar->ar_vers.low))) {
         return (FALSE);
      } /* Endif */
      return (xdr_uint_32(xdrs, &(ar->ar_vers.high)));

   default:
      return TRUE;
   } /* Endswitch */

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_rejected_reply
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a rejected_reply struct from an RPC reply union
*
*END*-----------------------------------------------------------------*/

static bool_t xdr_rejected_reply
   (
      register XDR_PTR                 xdrs,
            /* [IN] - XDR stream handle */
      register struct rejected_reply   _PTR_ rr
            /* [IN,OUT] - pointer to the rejected reply struct */
   )
{ /* Body */

   /*
   ** We could use xdr_union, but then we'd have to write an xdrproc_t
   ** for handling the RPC_MISMATCH code.  Instead, we filter the
   ** union manually.
   */
   if (! xdr_enum(xdrs, (enum_t _PTR_)&(rr->rj_stat))) {
      return (FALSE);
   } /* Endif */

   switch (rr->rj_stat) {

   case RPC_MISMATCH:
      if (! xdr_uint_32(xdrs, &(rr->rj_vers.low))) {
         return (FALSE);
      } /* Endif */
      return (xdr_uint_32(xdrs, &(rr->rj_vers.high)));

   case AUTH_ERROR:
      return (xdr_enum(xdrs, (enum_t _PTR_)&(rr->rj_why)));

   default:
      return TRUE;
   } /* Endswitch */

} /* Endbody */

/*
** The RPC reply message union discrim array for the xdr_union filter
*/

static xdr_discrim reply_dscrm[3] = {
   { (enum_t)MSG_ACCEPTED,    (xdrproc_t)xdr_accepted_reply },
   { (enum_t)MSG_DENIED,      (xdrproc_t)xdr_rejected_reply },
   { __dontcare__,            NULL_xdrproc_t } };

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_replymsg
* Returned Value   : TRUE or FALSE
* Comments  :  XDR an RPC reply union
*
*END*-----------------------------------------------------------------*/

bool_t xdr_replymsg
   (
      register XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      register RPC_MESSAGE_PTR   rmsg
            /* [IN,OUT] - pointer to the reply union */
   )
{ /* Body */
   return (
      xdr_uint_32(xdrs, &(rmsg->rm_xid)) &&
      xdr_enum(xdrs, (enum_t _PTR_)&(rmsg->rm_direction)) &&
      (rmsg->rm_direction == REPLY) &&
      xdr_union(xdrs, (enum_t _PTR_)&(rmsg->rm_reply.rp_stat),
         (caddr_t)&(rmsg->rm_reply.ru), reply_dscrm, NULL_xdrproc_t));
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_callmsg
* Returned Value   : TRUE or FALSE
* Comments  :  XDR an RPC call message
*
*END*-----------------------------------------------------------------*/

bool_t xdr_callmsg
   (
      register XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      register RPC_MESSAGE_PTR   cmsg
            /* [IN,OUT] - pointer to the call message to filter */
   )
{ /* Body */
   return (
       xdr_uint_32(xdrs, &(cmsg->rm_xid)) &&
       xdr_enum(xdrs, (enum_t _PTR_)&(cmsg->rm_direction)) &&
       (cmsg->rm_direction == CALL) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_rpcvers)) &&
       (cmsg->rm_call.cb_rpcvers == RPC_MSG_VERSION) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_prog)) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_vers)) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_proc)) &&
       xdr_opaque_auth(xdrs, &(cmsg->rm_call.cb_cred)) &&
       xdr_opaque_auth(xdrs, &(cmsg->rm_call.cb_verf)));
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_callhdr
* Returned Value   : TRUE or FALSE
* Comments  :  Serializes the "static part" of an RPC call message
*
* The fields include: rm_xid, rm_direction, rpcvers, prog, and vers.
* The rm_xid is not really static, but the user can easily munge on the fly.
*
*END*-----------------------------------------------------------------*/

bool_t xdr_callhdr
   (
      register XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      register RPC_MESSAGE_PTR   cmsg
   )
{ /* Body */
   cmsg->rm_direction = CALL;
   cmsg->rm_call.cb_rpcvers = RPC_MSG_VERSION;
   return (
       (xdrs->x_op == XDR_ENCODE) &&
       xdr_uint_32(xdrs, &(cmsg->rm_xid)) &&
       xdr_enum(xdrs, (enum_t _PTR_)&(cmsg->rm_direction)) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_rpcvers)) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_prog)) &&
       xdr_uint_32(xdrs, &(cmsg->rm_call.cb_vers)));
} /* Endbody */


/* EOF */
