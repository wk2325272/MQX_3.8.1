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
* $FileName: xdrpmap.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the implementation of the XDR
*   filters for the portmapper.
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

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_pmap
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a portmapper entry
*
*END*-----------------------------------------------------------------*/

bool_t xdr_pmap
   (
      XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      PMAP_PTR          regp
            /* [IN,OUT] - pointer to the portmapper entry */
   )
{ /* Body */
   return (
      xdr_uint_32(xdrs, &regp->pm_prog) &&
      xdr_uint_32(xdrs, &regp->pm_vers) &&
      xdr_uint_32(xdrs, &regp->pm_prot) &&
      xdr_uint_32(xdrs, &regp->pm_port));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_pmapentry
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a portmapper list entry
*
*END*-----------------------------------------------------------------*/

bool_t xdr_pmapentry
   (
      XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      PMAPLIST_PTR      regp
            /* [IN,OUT] - pointer to the portmapper list entry */
   )
{ /* Body */
   return (
      xdr_pmap(xdrs, &regp->pml_map) &&
      xdr_pointer(xdrs, (pointer)&regp->pml_next, sizeof(PMAPLIST),
         (xdrproc_t)xdr_pmapentry));
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_pmaplist
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a portmapper list
*
*END*-----------------------------------------------------------------*/

bool_t xdr_pmaplist
   (
      XDR_PTR              xdrs,
            /* [IN] - XDR stream handle */
      PMAPLIST_PTR _PTR_   regp
            /* [IN,OUT] - pointer to the portmapper list */
   )
{ /* Body */
   return (
      xdr_pointer(xdrs, (pointer)regp, sizeof(PMAPLIST),
         (xdrproc_t)xdr_pmapentry));
} /* Endbody */


/* EOF */
