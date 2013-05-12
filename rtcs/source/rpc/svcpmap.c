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
* $FileName: svcpmap.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the implementation of the server
*   side interface to the portmapper functions.
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

static struct timeval timeout = {5, 0};
static struct timeval tottimeout = {60, 0};


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : pmap_set
* Returned Value   : void
* Comments  :  Set a mapping between program,version and port.
*     Calls the pmap service remotely to do the mapping.
*
*END*-----------------------------------------------------------------*/

bool_t pmap_set
   (
      uint_32 program,
      uint_32 version,
      uint_32 protocol,
      uint_16 port
   )
{ /* Body */
   sockaddr_in             myaddr;
   int_32                  socket = -1;
   CLIENT_PTR              client;
   PMAP                    parms;
   bool_t                  result;

   myaddr.sin_family = AF_INET;
   myaddr.sin_port = PMAPPORT;
   myaddr.sin_addr.s_addr = INADDR_LOOPBACK;

   client = clntudp_bufcreate(&myaddr, PMAPPROG, PMAPVERS, timeout,
            &socket, RPCSMALLMSGSIZE, RPCSMALLMSGSIZE);
   if (client == NULL) {
      return (FALSE);
   } /* Endif */
   parms.pm_prog = program;
   parms.pm_vers = version;
   parms.pm_prot = protocol;
   parms.pm_port = port;
   if (clnt_call(client, PMAPPROC_SET, (xdrproc_t)xdr_pmap, (caddr_t)&parms,
       (xdrproc_t)xdr_bool, (caddr_t)&result, tottimeout) != RPC_SUCCESS) {
      return (FALSE);
   } /* Endif */
   clnt_destroy(client);
   shutdown(socket, 0);

   return (result);
} /* EndBody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : pmap_unset
* Returned Value   : void
* Comments  :  Remove the mapping between program,version and port.
*     Calls the pmap service remotely to do the unmapping.
*
*END*-----------------------------------------------------------------*/

bool_t pmap_unset
   (
      uint_32 program,
      uint_32 version
   )
{ /* Body */
   sockaddr_in             myaddr;
   int_32                  socket = -1;
   CLIENT_PTR              client;
   PMAP                    parms;
   bool_t                  result;

   myaddr.sin_family = AF_INET;
   myaddr.sin_port = PMAPPORT;
   myaddr.sin_addr.s_addr = INADDR_LOOPBACK;

   client = clntudp_bufcreate(&myaddr, PMAPPROG, PMAPVERS, timeout,
            &socket, RPCSMALLMSGSIZE, RPCSMALLMSGSIZE);
   if (client == NULL) {
      return (FALSE);
   } /* Endif */
   parms.pm_prog = program;
   parms.pm_vers = version;
   parms.pm_port = 0;
   parms.pm_prot = 0;
   clnt_call(client, PMAPPROC_UNSET, (xdrproc_t)xdr_pmap, (caddr_t)&parms,
      (xdrproc_t)xdr_bool, (caddr_t)&result, tottimeout);
   clnt_destroy(client);
   shutdown(socket, 0);

   return (result);
} /* EndBody */


/* EOF */
