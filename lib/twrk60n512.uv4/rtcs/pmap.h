#ifndef __pmap_h__
#define __pmap_h__
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
* $FileName: pmap.h$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the declarations for clients of
*   the portmapper.
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
** Protocol for the local binder service, or pmap.
** Copyright (C) 1984, Sun Microsystems, Inc.
**
** The following procedures are supported by the protocol:
**
** PMAPPROC_NULL() returns ()
**    takes nothing, returns nothing
**
** PMAPPROC_SET(struct pmap) returns (bool_t)
**    TRUE is success, FALSE is failure.  Registers the tuple
**    [prog, vers, prot, port].
**
** PMAPPROC_UNSET(struct pmap) returns (bool_t)
**    TRUE is success, FALSE is failure.  Un-registers pair
**    [prog, vers].  prot and port are ignored.
**
** PMAPPROC_GETPORT(struct pmap) returns (long unsigned).
**    0 is failure.  Otherwise returns the port number where the pair
**    [prog, vers] is registered.  It may lie!
**
** PMAPPROC_DUMP() returns (struct pmaplist *)
**
**
** The service supports remote procedure calls on UDP and TCP port 111.
*/

#define PMAPPORT           111
#define PMAPPROG           100000
#define PMAPVERS           2

#define PMAPPROC_NULL      0
#define PMAPPROC_SET       1
#define PMAPPROC_UNSET     2
#define PMAPPROC_GETPORT   3
#define PMAPPROC_DUMP      4

typedef struct pmap {
   uint_32  pm_prog;
   uint_32  pm_vers;
   uint_32  pm_prot;
   uint_32  pm_port;
} PMAP, _PTR_ PMAP_PTR;

typedef struct pmaplist {
   struct pmap             pml_map;
   struct pmaplist _PTR_   pml_next;
} PMAPLIST, _PTR_ PMAPLIST_PTR;

extern bool_t xdr_pmap     (XDR_PTR, PMAP_PTR);
extern bool_t xdr_pmapentry(XDR_PTR, PMAPLIST_PTR);
extern bool_t xdr_pmaplist (XDR_PTR, PMAPLIST_PTR _PTR_);

#endif
/* EOF */
