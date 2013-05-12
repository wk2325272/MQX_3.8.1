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
* $FileName: svcauth.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the implementation of the server
*   side RPC Library authentication functions.
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

/*
** Server side authenticators are called from _authenticate() by
** using the client auth struct flavor field to index into svcauthsw[].
** The server auth flavors must implement a routine that looks like:
**
** enum auth_stat
** flavorx_auth(rqst, msg)
**    register struct svcreq *rqst;
**    register struct rpc_msg *msg;
**
*/

enum auth_stat _svcauth_null(struct svcreq _PTR_, RPC_MESSAGE_PTR);     /* no authentication */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _svcauth_null
* Returned Value   : status
* Comments  :  Authenticates an RPC request using "null" authentication
*
*END*-----------------------------------------------------------------*/

enum auth_stat _svcauth_null
   (
      register struct svcreq _PTR_ rqst,
            /* [OUT] - Service request structure */
      RPC_MESSAGE_PTR msg
            /* [IN] - RPC request */
   )
{ /* Body */
   return (AUTH_OK);
} /* Endbody */


static struct {
   enum auth_stat (_CODE_PTR_ authenticator)(struct svcreq _PTR_, RPC_MESSAGE_PTR);
} svcauthsw[] = {
   _svcauth_null        /* AUTH_NULL */
};

#define  AUTH_MAX 0     /* highest AUTH number -- only "null" is implemented */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : _authenticate
* Returned Value   : status
* Comments  :  Authenticates an RPC request
*
* The RPC request, msg, has been obtained from the wire.  The msg contains
* the raw form of credentials and verifiers.  _authenticate() returns AUTH_OK
* if the msg is successfully authenticated.  If AUTH_OK, then the routine also
* does the following things:
*
* sets rqst->rq_xprt->verf to the appropriate response verifier;
* sets rqst->rq_client_cred to the "cooked" form of the credentials.
*
* NB: rqst->rq_cxprt->verf must be pre-alloctaed;
* its length is set appropriately.
*
* The caller still owns and is responsible for msg->u.cmb.cred and
* msg->u.cmb.verf.  The authentication system retains ownership of
* rqst->rq_client_cred, the cooked credentials.
*
* There is an assumption that any flavour less than AUTH_NULL is
* invalid.
*
*END*-----------------------------------------------------------------*/

enum auth_stat _authenticate
   (
      register struct svcreq _PTR_ rqst,
            /* [OUT] - Service request structure */
      RPC_MESSAGE_PTR         msg
            /* [IN] - RPC request */
   )
{ /* Body */
   register uint_32 cred_flavor;

   rqst->rq_cred = msg->rm_call.cb_cred;
   rqst->rq_xprt->xp_verf.oa_flavor = rpc_null_auth.oa_flavor;
   rqst->rq_xprt->xp_verf.oa_length = 0;
   cred_flavor = rqst->rq_cred.oa_flavor;
   /* Start SPR P122-0266-13 remove all warnings from RTCS code. */
   /* Unsigned compare to 0 always true. */
   /* if ((cred_flavor <= AUTH_MAX) && (cred_flavor >= AUTH_NULL)) { */
   if (cred_flavor <= AUTH_MAX) {
   /* End SPR P122-0266-13 */
      return ((*(svcauthsw[cred_flavor].authenticator))(rqst, msg));
   } /* Endif */

   return (AUTH_REJECTEDCRED);
} /* Endbody */


/* EOF */
