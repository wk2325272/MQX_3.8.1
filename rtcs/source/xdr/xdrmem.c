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
* $FileName: xdrmem.c$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the implementation of the buffer
*   oriented XDR Library functions.
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

static bool_t        xdrmem_getlong();
static bool_t        xdrmem_putlong();
static bool_t        xdrmem_getbytes();
static bool_t        xdrmem_putbytes();
static uint_32       xdrmem_getpos();
static bool_t        xdrmem_setpos();
static int_32_ptr    xdrmem_inline();
static void          xdrmem_destroy();

static struct xdr_ops xdrmem_ops = {
   xdrmem_getlong,
   xdrmem_putlong,
   xdrmem_getbytes,
   xdrmem_putbytes,
   xdrmem_getpos,
   xdrmem_setpos,
   xdrmem_inline,
   xdrmem_destroy
};


/*
** For memory-based XDR, the semantics of the various fields of
** the XDR handle are:
**
**    x_public       is not used
**    x_private      points to the current position in the XDR stream
**    x_base         points to the first byte of the memory buffer
**    x_handy        contains the space remaining in the memory buffer
*/


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_create
* Returned Value   : nothing
* Comments  :  Opens a buffer-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

void xdrmem_create
   (
      XDR_PTR  xdrs,
            /* [OUT] - XDR stream handle */
      caddr_t  addr,
            /* [IN] - address of memory buffer */
      uint_32  size,
            /* [IN] - size of memory buffer (in bytes) */
      xdr_op   op
            /* [IN] - operation to perform on XDR stream */
   )
{ /* Body */
   xdrs->x_op = op;
   xdrs->x_ops = &xdrmem_ops;
   xdrs->x_private = xdrs->x_base = addr;
   xdrs->x_handy = size;
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_destroy
* Returned Value   : nothing
* Comments  :  Closes a buffer-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static void xdrmem_destroy
   (
      XDR_PTR  xdrs
            /* [IN] - XDR stream handle */
   )
{ /* Body */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_getlong
* Returned Value   : TRUE or FALSE
* Comments  :  Decodes a 32-bit integer from a buffer-oriented
*              XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrmem_getlong
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      int_32_ptr  ip
            /* [OUT] - int_32 to decode from stream */
   )
{ /* Body */
   if (xdrs->x_handy < XDRUNIT_BYTES) {
      return (FALSE);
   } /* Endif */
   xdrs->x_handy -= XDRUNIT_BYTES;
   *ip = (int_32)ntohl((uchar_ptr)xdrs->x_private);
   xdrs->x_private += XDRUNIT_BYTES;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_putlong
* Returned Value   : TRUE or FALSE
* Comments  :  Encodes a 32-bit integer onto a buffer-oriented
*              XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrmem_putlong
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      int_32_ptr  ip
            /* [IN] - int_32 to encode to stream */
   )
{ /* Body */
   if (xdrs->x_handy < XDRUNIT_BYTES) {
      return (FALSE);
   } /* Endif */
   xdrs->x_handy -= XDRUNIT_BYTES;
   htonl((uchar_ptr)xdrs->x_private, *ip);
   xdrs->x_private += XDRUNIT_BYTES;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_getbytes
* Returned Value   : TRUE or FALSE
* Comments  :  Decodes bytes from a buffer-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrmem_getbytes
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      caddr_t  addr,
            /* [OUT] - bytes to decode from stream */
      uint_32  len
            /* [IN] - number of bytes to decode */
   )
{ /* Body */
   if (xdrs->x_handy < len) {
      return (FALSE);
   } /* Endif */
   xdrs->x_handy -= len;
   _mem_copy(xdrs->x_private, addr, len);
   xdrs->x_private += len;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_putbytes
* Returned Value   : TRUE or FALSE
* Comments  :  Encodes bytes onto a buffer-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrmem_putbytes
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      caddr_t  addr,
            /* [IN] - bytes to encode to stream */
      uint_32  len
            /* [IN] - number of bytes to encode */
   )
{ /* Body */
   if (xdrs->x_handy < len) {
      return (FALSE);
   } /* Endif */
   xdrs->x_handy -= len;
   _mem_copy(addr, xdrs->x_private, len);
   xdrs->x_private += len;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_getpos
* Returned Value   : number of bytes encoded to or decoded from stream
* Comments  :  Retrieves the current stream position
*
*END*-----------------------------------------------------------------*/

static uint_32 xdrmem_getpos
   (
      XDR_PTR  xdrs
            /* [IN] - XDR stream handle */
   )
{ /* Body */
   /*
   ** Pointers were being cast before subtraction.  Removed typecast.
   */
   return (xdrs->x_private - xdrs->x_base);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_setpos
* Returned Value   : TRUE or FALSE
* Comments  :  Sets the current stream position
*
*END*-----------------------------------------------------------------*/

static bool_t xdrmem_setpos
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      uint_32  pos
            /* [IN] - new position for XDR stream */
   )
{ /* Body */
   caddr_t newaddr   = xdrs->x_base + pos;
   caddr_t lastaddr  = xdrs->x_private + xdrs->x_handy;

   /*
   ** Pointers were being cast before subtraction.  Removed typecast.
   */
   if (newaddr > lastaddr) {
      return (FALSE);
   } /* Endif */
   xdrs->x_private = newaddr;
   /*
   ** Pointers were being cast before subtraction.  Removed typecast.
   */
   xdrs->x_handy = lastaddr - newaddr;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrmem_inline
* Returned Value   : pointer to the reserved buffer
* Comments  :  Reserves bytes on a buffer-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static int_32_ptr xdrmem_inline
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      uint_32  len
            /* [IN] - size of the buffer (in bytes) */
   )
{ /* Body */
   int_32_ptr buf;

   if (xdrs->x_handy < len) {
      return (NULL);
   } /* Endif */
   xdrs->x_handy -= len;
   buf = (int_32_ptr)xdrs->x_private;
   xdrs->x_private += len;
   return (buf);
} /* Endbody */


/* EOF */
