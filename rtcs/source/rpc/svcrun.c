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
* $FileName: svcrun.c$
* $Version : 3.5.7.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the implementation of the server
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


#include <rtcsrpc.h>

#define RQCRED_SIZE XDRUNIT_BYTES

/*
** The services list
** Each entry represents a set of procedures (an RPC program).
** The dispatch routine takes request structs and runs the
** apropriate procedure.
*/

#define NULL_SVC ((struct svc_callout _PTR_)0)

static struct svc_callout {
   struct svc_callout   _PTR_ sc_next;
   uint_32              sc_prog;
   uint_32              sc_vers;
   void                 (_CODE_PTR_ sc_dispatch)(pointer, SVCXPRT_PTR);
} _PTR_ svc_head = NULL;

static struct svc_callout _PTR_ svc_find(uint_32, uint_32, struct svc_callout _PTR_ _PTR_);


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svc_register
* Returned Value   : TRUE or FALSE
* Comments  :  Registers a remote service with the RPC server.
*
*END*-----------------------------------------------------------------*/

bool_t svc_register
   (
      SVCXPRT_PTR xprt,
            /* [IN] - transport handle */
      uint_32     prog,
            /* [IN] - service program number */
      uint_32     vers,
            /* [IN] - service program version */
      void        (_CODE_PTR_ dispatch)(),
            /* [IN] - service program */
      int_32      protocol
            /* [IN] - transport protocol */
   )
{ /* Body */
   register struct svc_callout _PTR_ s;
   struct svc_callout _PTR_ p;

   s = svc_find(prog, vers, &p);
   if (s == NULL_SVC) {

         /*
         ** We're registering a new service
         */
      s = (struct svc_callout _PTR_)RTCS_mem_alloc_zero(sizeof(struct svc_callout));
      if (s == NULL_SVC) {
         return (FALSE);
      } /* Endif */

      s->sc_prog = prog;
      s->sc_vers = vers;
      s->sc_dispatch = dispatch;
      s->sc_next = svc_head;
      svc_head = s;

   } else if (s->sc_dispatch != dispatch) {

         /*
         ** We can register a single service with two different transports,
         ** provided they really are the same service (i.e. the dispatch
         ** functions match).
         */
      return (FALSE);

   } /* Endif */

      /* Register the information with the local portmapper */
   if (protocol) {
      return (pmap_set(prog, vers, protocol, xprt->xp_port));
   } /* Endif */

   return (TRUE);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svc_find
* Returned Value   : remote service
* Comments  :  Finds a registered service.
*
*END*-----------------------------------------------------------------*/

static struct svc_callout _PTR_ svc_find
   (
      uint_32  prog,
            /* [IN] - service program number */
      uint_32  vers,
            /* [IN] - service program version */
      struct svc_callout _PTR_ _PTR_ prev
            /* [OUT] - service program */
   )
{ /* Body */
   register struct svc_callout _PTR_ s, _PTR_ p;

   for (p = NULL_SVC, s = svc_head; s != NULL_SVC; p = s, s = s->sc_next) {
      if ((s->sc_prog == prog) && (s->sc_vers == vers)) {
         break;
      } /* Endif */
   } /* Endfor */
   *prev = p;

   return (s);

} /* Endbody */



/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svc_unregister
* Returned Value   : void
* Comments  :  Unregisters a remote service from the RPC server.
*
*END*-----------------------------------------------------------------*/

void svc_unregister
   (
      uint_32  prog,
            /* [IN] - service program number */
      uint_32  vers
            /* [IN] - service program version */
   )
{ /* Body */
   register struct svc_callout _PTR_ s;
   struct svc_callout _PTR_ p;

   s = svc_find(prog, vers, &p);
   if (s == NULL_SVC) {
      return;
   } else if (p == NULL_SVC) {
      svc_head = s->sc_next;
   } else {
      p->sc_next = s->sc_next;
   } /* Endif */
   s->sc_next = NULL_SVC;
   _mem_free(s);

      /* Unregister the information with the local portmapper */
   pmap_unset(prog, vers);

} /* Endbody */


/*
** The transports list
** Each entry represents associates a socket to its transport
*/

#define NULL_XPRT ((SVCXPRT_PTR)0)

static SVCXPRT_PTR xprt_head = NULL;


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xprt_register
* Returned Value   : void
* Comments  :  Activates a transport handle
*
*END*-----------------------------------------------------------------*/

void xprt_register
   (
      SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */

   xprt->xp_next = xprt_head;
   xprt_head = xprt;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xprt_unregister
* Returned Value   : void
* Comments  :  Deactivates a transport handle
*
*END*-----------------------------------------------------------------*/

void xprt_unregister
   (
      SVCXPRT_PTR xprt
            /* [IN] - transport handle */
   )
{ /* Body */
   register SVCXPRT_PTR x, p;

   for (p = NULL_XPRT, x = xprt_head; x != NULL_XPRT; p = x, x = x->xp_next) {
      if (x == xprt) {
         break;
      } /* Endif */
   } /* Endfor */

   if (x == NULL_XPRT) {
      return;
   } else if (p == NULL_XPRT) {
      xprt_head = x->xp_next;
   } else {
      p->xp_next = x->xp_next;
   } /* Endif */
   x->xp_next = NULL_XPRT;

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svc_getreq
* Returned Value   : void
* Comments  :  Services an RPC request.
*
* Statement of authentication parameters management:
* This function owns and manages all authentication parameters, specifically
* the "raw" parameters (msg.rm_call.cb_cred and msg.rm_call.cb_verf) and
* the "cooked" credentials (rqst->rq_clntcred).
* However, this function does not know the structure of the cooked
* credentials, so it makes the following assumptions:
*   a) the structure is contiguous (no pointers), and
*   b) the cred structure size does not exceed RQCRED_SIZE bytes.
* In all events, all three parameters are freed upon exit from this routine.
* The storage is trivially managed on the call stack in user land, but
* is allocated in kernel land.
*
*END*-----------------------------------------------------------------*/

void svc_getreq
   (
      SVCXPRT_PTR xprt
         /* [IN] - transport to service */
   )
{ /* Body */
   enum xprt_stat stat;
   RPC_MESSAGE msg;
   struct svcreq r;
   char cred_area[2*MAX_AUTH_BYTES + RQCRED_SIZE];

   msg.rm_call.cb_cred.oa_base = cred_area;
   msg.rm_call.cb_verf.oa_base = &(cred_area[MAX_AUTH_BYTES]);
   r.rq_clntcred = &(cred_area[2*MAX_AUTH_BYTES]);

   do {
      while (svc_recv(xprt, &msg)) {

         register struct svc_callout _PTR_ s;
         enum auth_stat why;
         boolean prog_found, vers_found;
         uint_32 low_vers, high_vers;

         r.rq_xprt = xprt;
         r.rq_prog = msg.rm_call.cb_prog;
         r.rq_vers = msg.rm_call.cb_vers;
         r.rq_proc = msg.rm_call.cb_proc;
         r.rq_cred = msg.rm_call.cb_cred;

         /* Authenticate the message */
         if ((why= _authenticate(&r, &msg)) != AUTH_OK) {
            svcerr_auth(xprt, why);
            break;
         } /* Endif */

         /* Try to find a matching registered service */
         prog_found = vers_found = FALSE;
         low_vers = (uint_32)-1;
         high_vers = 0;
         for (s = svc_head; s != NULL_SVC; s = s->sc_next) {
            if (s->sc_prog == r.rq_prog) {
               prog_found = TRUE;
               if (s->sc_vers == r.rq_vers) {
                  vers_found = TRUE;
                  (*s->sc_dispatch)(&r, xprt);
                  break;
               } else if (s->sc_vers < low_vers) {
                  low_vers = s->sc_vers;
               } else if (s->sc_vers > high_vers) {
                  high_vers = s->sc_vers;
               } /* Endif */
            } /* Endif */
         } /* Endfor */

         /* Return an error if no match was found */
         if (!prog_found) {
            svcerr_noprog(xprt);
         } else if (!vers_found) {
            svcerr_progvers(xprt, low_vers, high_vers);
         } /* Endif */

         break;
      } /* Endwhile */

      if ((stat = svc_stat(xprt)) == XPRT_DIED) {
         svc_destroy(xprt);
      } /* Endif */
   } while (stat == XPRT_MOREREQS);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : svc_run
* Returned Value   : void
* Comments  :  This is the RPC server-side idle loop.  It blocks waiting
*              for a request, and it services it.
*
*END*-----------------------------------------------------------------*/

void svc_run
   (
      void
   )
{ /* Body */
   int_32 sock;
   SVCXPRT_PTR xprt;

   for (;;) {
      sock = RTCS_selectall(0);
      if (sock == (int_32)RTCS_SOCKET_ERROR) {
         break;
      } /* Endif */

      for (xprt = xprt_head; xprt != NULL_XPRT; xprt = xprt->xp_next) {
         if (xprt->xp_sock == sock) {
            break;
         } /* Endif */
      } /* Endfor */
      if (xprt == NULL_XPRT) {
         break;
      } /* Endif */

      svc_getreq(xprt);
   } /* Endfor */

} /* Endbody */


/* EOF */
