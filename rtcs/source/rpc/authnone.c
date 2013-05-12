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
* $FileName: authnone.c$
* $Version : 3.6.7.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the implementation of the client
*   null authenticator for the RPC Library.
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

#define MAX_MARSHAL_SIZE 20

/*
** Authenticator operations routines
*/
static void   authnone_verf();
static void   authnone_destroy();
static bool_t authnone_marshal();
static bool_t authnone_validate();
static bool_t authnone_refresh();

static struct auth_ops ops = {
   authnone_verf,
   authnone_marshal,
   authnone_validate,
   authnone_refresh,
   authnone_destroy
};

static struct authnone_private {
   AUTH     no_client;                             /* authentication handle */
   char     marshalled_client[MAX_MARSHAL_SIZE];   /* serialized cred and verf */
   uint_32  mcnt;                                  /* size of marshalled cred and verf */
} _PTR_ authnone_private = NULL;

/*
** For null authentication, the semantics of the various fields of
** the authentication handle are:
**
**    ah_cred        rpc_null_auth
**    ah_verf        rpc_null_auth
**    ah_private     is not used
*/

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : authnone_create
* Returned Value   : authentication handle
* Comments  :  Creates a "null" authentication handle
*
*END*-----------------------------------------------------------------*/

AUTH_PTR authnone_create
   (
      void
   )
{ /* Body */
   register struct authnone_private _PTR_ ap = authnone_private;
   XDR xdr_stream;
   register XDR_PTR xdrs;

   if (ap == NULL) {
      ap = (struct authnone_private _PTR_)RTCS_mem_alloc_zero(sizeof(*ap));
      if (ap == NULL) {
         return (NULL);
      } /* Endif */
      authnone_private = ap;
   } /* Endif */

   if (!ap->mcnt) {

      /* Initialize the AUTH structure */
      ap->no_client.ah_cred = ap->no_client.ah_verf = rpc_null_auth;
      ap->no_client.ah_ops = &ops;

      /* Preserialize the credentials and the verifier */
      xdrs = &xdr_stream;
      xdrmem_create(xdrs, ap->marshalled_client, (uint_32)MAX_MARSHAL_SIZE,
          XDR_ENCODE);
      xdr_opaque_auth(xdrs, &ap->no_client.ah_cred);
      xdr_opaque_auth(xdrs, &ap->no_client.ah_verf);
      ap->mcnt = xdr_getpos(xdrs);
      xdr_destroy(xdrs);

   } /* Endif */

   return (&ap->no_client);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : authnone_verf
* Returned Value   : void
* Comments  :  Generates a client verifier
*
*END*-----------------------------------------------------------------*/

static void authnone_verf
   (
      AUTH_PTR client
         /* [IN] - authentication handle */
   )
{ /* Body */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : authnone_marshal
* Returned Value   : TRUE or FALSE
* Comments  :  Marshals the client credentials on an outgoing XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t authnone_marshal
   (
      AUTH_PTR client,
         /* [IN] - authentication handle */
      XDR_PTR  xdrs
         /* [IN] - XDR stream */
   )
{ /* Body */
   struct authnone_private _PTR_ ap = authnone_private;

   if (ap == NULL) {
      return (FALSE);
   } /* Endif */
   return ((*xdrs->x_ops->x_putbytes)(xdrs, ap->marshalled_client, ap->mcnt));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : authnone_validate
* Returned Value   : TRUE or FALSE
* Comments  :  Authenticates the server verifier on an incoming XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t authnone_validate
   (
      AUTH_PTR client,
         /* [IN] - authentication handle */
      opaque_auth verf
         /* [IN] - verifier */
   )
{ /* Body */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : authnone_refresh
* Returned Value   : TRUE or FALSE
* Comments  :  Refreshes the client credentials
*
*  Since there are no credentials in null authentication, if the server
*  rejects us, there's no point trying again, so we always return FALSE.
*
*END*-----------------------------------------------------------------*/

static bool_t authnone_refresh
   (
      AUTH_PTR client
         /* [IN] - authentication handle */
   )
{ /* Body */
   return (FALSE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : authnone_destroy
* Returned Value   : void
* Comments  :  Destroys a "null" authentication handle
*
*END*-----------------------------------------------------------------*/

static void authnone_destroy
   (
      AUTH_PTR client
         /* [IN] - authentication handle */
   )
{ /* Body */
} /* Endbody */


/* EOF */
