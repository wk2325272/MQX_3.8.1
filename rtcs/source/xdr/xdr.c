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
* $FileName: xdr.c$
* $Version : 3.6.7.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the implementation of the generic
*   XDR Library functions.
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


#include <string.h>
#include <rtcsrpc.h>

/*
** constants specific to the xdr "protocol"
*/
#define XDR_FALSE       ((int_32) 0)
#define XDR_TRUE        ((int_32) 1)
#define LASTUNSIGNED    ((uint_32) -1)

/*
** for unit alignment
*/
static char xdr_zero[XDRUNIT_BYTES] = { 0, 0, 0, 0 };


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_free
* Returned Value   : none
* Comments  :  Free a data structure using XDR
*              Not a filter, but a convenient utility nonetheless.
*
*END*-----------------------------------------------------------------*/

void xdr_free
   (
      xdrproc_t   proc,
            /* [IN] - filter that has some dynamic data to free */
      pointer     objp
            /* [IN] - pointer to the dynamic data */
   )
{ /* Body */
   XDR x;
   x.x_op = XDR_FREE;
   (*proc)(&x, objp);
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_void
* Returned Value   : TRUE or FALSE
* Comments  :  XDR nothing
*
*END*-----------------------------------------------------------------*/

bool_t xdr_void
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      pointer     objp
            /* [IN] - pointer to "void" */
   )
{ /* Body */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_int_32
* Returned Value   : TRUE or FALSE
* Comments  :  XDR 32-bit signed integers
*
*END*-----------------------------------------------------------------*/

bool_t xdr_int_32
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      int_32_ptr  ip
            /* [IN,OUT] - pointer to the integer to filter */
   )
{ /* Body */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      return (xdr_putlong(xdrs, ip));

   case XDR_DECODE:
      return (xdr_getlong(xdrs, ip));

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_uint_32
* Returned Value   : TRUE or FALSE
* Comments  :  XDR 32-bit unsigned integers
*
*END*-----------------------------------------------------------------*/

bool_t xdr_uint_32
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      uint_32_ptr ip
            /* [IN,OUT] - pointer to the integer to filter */
   )
{ /* Body */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      return (xdr_putlong(xdrs, (int_32_ptr)ip));

   case XDR_DECODE:
      return (xdr_getlong(xdrs, (int_32_ptr)ip));

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_int_16
* Returned Value   : TRUE or FALSE
* Comments  :  XDR 16-bit signed integers
*
*END*-----------------------------------------------------------------*/

bool_t xdr_int_16
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      int_16_ptr  ip
            /* [IN,OUT] - pointer to the integer to filter */
   )
{ /* Body */
   int_32 i;

   switch (xdrs->x_op) {

   case XDR_ENCODE:
      i = (int_32) *ip;
      return (xdr_putlong(xdrs, &i));

   case XDR_DECODE:
      if (!xdr_getlong(xdrs, &i)) {
         return (FALSE);
      } /* Endif */
      *ip = (int_16) i;
      return (TRUE);

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_uint_16
* Returned Value   : TRUE or FALSE
* Comments  :  XDR 16-bit unsigned integers
*
*END*-----------------------------------------------------------------*/

bool_t xdr_uint_16
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      uint_16_ptr ip
            /* [IN,OUT] - pointer to the integer to filter */
   )
{ /* Body */
   int_32 i;

   switch (xdrs->x_op) {

   case XDR_ENCODE:
      i = (int_32) *ip;
      return (xdr_putlong(xdrs, &i));

   case XDR_DECODE:
      if (!xdr_getlong(xdrs, &i)) {
         return (FALSE);
      } /* Endif */
      *ip = (uint_16) i;
      return (TRUE);

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_int
* Returned Value   : TRUE or FALSE
* Comments  :  XDR signed integers (provided for compatibility)
*              int is assumed to be either 16 or 32 bits
*
*END*-----------------------------------------------------------------*/

bool_t xdr_int
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      int      _PTR_ ip
            /* [IN,OUT] - pointer to the integer to filter */
   )
{ /* Body */
   if (sizeof (int) == sizeof (int_32)) {
      return (xdr_int_32(xdrs, (int_32_ptr)ip));
   } else {
      /* Start SPR P122-0266-21 remove all warnings from RTCS code. */
      /* return (xdr_int_16(xdrs, (int_16_ptr)ip)); */
      return (xdr_int_16(xdrs, (int_16_ptr)((void _PTR_)ip)));
      /* End SPR P122-0266-21 */
   } /* Endif */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_u_int
* Returned Value   : TRUE or FALSE
* Comments  :  XDR unsigned integers (provided for compatibility)
*              int is assumed to be either 16 or 32 bits
*
*END*-----------------------------------------------------------------*/

bool_t xdr_u_int
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      u_int    _PTR_ ip
            /* [IN,OUT] - pointer to the integer to filter */
   )
{ /* Body */
   if (sizeof (u_int) == sizeof (uint_32)) {
      return (xdr_uint_32(xdrs, (uint_32_ptr)ip));
   } else {
      /* Start SPR P122-0266-21 remove all warnings from RTCS code. */
      /* return (xdr_uint_16(xdrs, (uint_16_ptr)ip)); */
      return (xdr_uint_16(xdrs, (uint_16_ptr)((void _PTR_)ip)));
      /* End SPR P122-0266-21 */
   } /* Endif */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_char
* Returned Value   : TRUE or FALSE
* Comments  :  XDR signed chars
*
*END*-----------------------------------------------------------------*/

bool_t xdr_char
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      char_ptr    cp
            /* [IN,OUT] - pointer to the character to filter */
   )
{ /* Body */
   int_32 i;

   i = (int_32)(*cp);
   if (!xdr_int_32(xdrs, &i)) {
      return (FALSE);
   } /* Endif */
   *cp = (char)i;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_uchar
* Returned Value   : TRUE or FALSE
* Comments  :  XDR unsigned chars
*
*END*-----------------------------------------------------------------*/

bool_t xdr_uchar
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      uchar_ptr   cp
            /* [IN,OUT] - pointer to the character to filter */
   )
{ /* Body */
   uint_32 i;

   i = (int_32)(*cp);
   if (!xdr_uint_32(xdrs, &i)) {
      return (FALSE);
   } /* Endif */
   *cp = (uchar)i;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_bool
* Returned Value   : TRUE or FALSE
* Comments  :  XDR booleans
*
*END*-----------------------------------------------------------------*/

bool_t xdr_bool
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      bool_t   _PTR_ bp
            /* [IN,OUT] - pointer to the boolean to filter */
   )
{ /* Body */
   int_32 i;

   switch (xdrs->x_op) {

   case XDR_ENCODE:
      i = (!*bp) ? XDR_FALSE : XDR_TRUE;
      return (xdr_putlong(xdrs, &i));

   case XDR_DECODE:
      if (!xdr_getlong(xdrs, &i)) {
         return (FALSE);
      } /* Endif */
      *bp = (i == XDR_FALSE) ? FALSE : TRUE;
      return (TRUE);

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_enum
* Returned Value   : TRUE or FALSE
* Comments  :  XDR enumerated types
*              enums are assumed to be 8, 16 or 32 bits
*
*END*-----------------------------------------------------------------*/

bool_t xdr_enum
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      enum_t   _PTR_ ep
            /* [IN,OUT] - pointer to the enum to filter */
   )
{ /* Body */
   if (sizeof (xdr_op) == sizeof (int_32)) {
      return (xdr_int_32(xdrs, (int_32_ptr)ep));
   } else if (sizeof (xdr_op) == sizeof (int_16)) {
      /* Start SPR P122-0266-21 remove all warnings from RTCS code. */
      /* return (xdr_int_16(xdrs, (int_16_ptr)ep)); */
      return (xdr_int_16(xdrs, (int_16_ptr)((void _PTR_)ep)));
      /* End SPR P122-0266-21 */
   } else {
      return (FALSE);
   } /* Endif */
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_float
* Returned Value   : TRUE or FALSE
* Comments  :  XDR IEEE single-precision floating-point numbers
*
*END*-----------------------------------------------------------------*/

bool_t xdr_float
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      float    _PTR_ fp
            /* [IN,OUT] - pointer to the fp number to filter */
   )
{ /* Body */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      return (xdr_putlong(xdrs, (int_32_ptr)fp));

   case XDR_DECODE:
      return (xdr_getlong(xdrs, (int_32_ptr)fp));

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_double
* Returned Value   : TRUE or FALSE
* Comments  :  XDR IEEE double-precision floating-point numbers
*
*END*-----------------------------------------------------------------*/

bool_t xdr_double
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      double   _PTR_ fp
            /* [IN,OUT] - pointer to the fp number to filter */
   )
{ /* Body */
   switch (xdrs->x_op) {

#if PSP_ENDIAN == MQX_BIG_ENDIAN

      /*
      ** Big endian
      */
   case XDR_ENCODE:
      return (xdr_putlong(xdrs, ((int_32_ptr)fp))
           && xdr_putlong(xdrs, ((int_32_ptr)fp)+1));

   case XDR_DECODE:
      return (xdr_getlong(xdrs, ((int_32_ptr)fp))
           && xdr_getlong(xdrs, ((int_32_ptr)fp)+1));

#else

      /*
      ** Little endian
      */
   case XDR_ENCODE:
      /* Start SPR P122-0266-21 remove all warnings from RTCS code. */
      /* return (xdr_putlong(xdrs, ((int_32_ptr)fp)+1) */
      /*     && xdr_putlong(xdrs, ((int_32_ptr)fp))); */
      return (xdr_putlong(xdrs, ((int_32_ptr)((void _PTR_)fp))+1)
           && xdr_putlong(xdrs, ((int_32_ptr)((void _PTR_)fp))));

   case XDR_DECODE:
      /* return (xdr_getlong(xdrs, ((int_32_ptr)fp)+1) */
      /*   && xdr_getlong(xdrs, ((int_32_ptr)fp))); */
      return (xdr_getlong(xdrs, ((int_32_ptr)((void _PTR_)fp))+1)
           && xdr_getlong(xdrs, ((int_32_ptr)((void _PTR_)fp))));
      /* End SPR P122-0266-21 */

#endif

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_opaque
* Returned Value   : TRUE or FALSE
* Comments  :  XDR fixed-length opaque data
*
*END*-----------------------------------------------------------------*/

bool_t xdr_opaque
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      caddr_t  cp,
            /* [IN,OUT] - pointer to the opaque data to filter */
      uint_32  count
            /* [IN] - size of the opaque data (in bytes) */
   )
{ /* Body */
   uint_32  slack = XDRUNIT_SLACK(count);
   char     xdr_fill[XDRUNIT_BYTES];

   if (count == 0) {
      return (TRUE);
   } /* Endif */

      /*
      ** Filter the data
      */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      if (!xdr_putbytes(xdrs, cp, count)) {
         return (FALSE);
      } /* Endif */
      if (slack == 0) {
         return (TRUE);
      } /* Endif */
      return (xdr_putbytes(xdrs, xdr_zero, slack));

   case XDR_DECODE:
      if (!xdr_getbytes(xdrs, cp, count)) {
         return (FALSE);
      } /* Endif */
      if (slack == 0) {
         return (TRUE);
      } /* Endif */
      return (xdr_getbytes(xdrs, xdr_fill, slack));

   case XDR_FREE:
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_bytes
* Returned Value   : TRUE or FALSE
* Comments  :  XDR variable-length opaque data
*
*END*-----------------------------------------------------------------*/

bool_t xdr_bytes
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      caddr_t     _PTR_ cpp,
            /* [IN,OUT] - pointer to the opaque data to filter */
      uint_32     _PTR_ countp,
            /* [IN,OUT] - pointer to the size of the opaque data (in bytes) */
      uint_32     maxsize
            /* [IN] - maximum size of the opaque data (in bytes) */
   )
{ /* Body */
   caddr_t  cp = *cpp;
   uint_32  count;

      /*
      ** Filter the data length
      */
   if (!xdr_uint_32(xdrs, countp)) {
      return (FALSE);
   } /* Endif */
   count = *countp;
   if (count > maxsize) {
      return (FALSE);
   } /* Endif */
   if (count == 0) {
      return (TRUE);
   } /* Endif */

      /*
      ** Now, filter the data
      */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      if (cp == NULL) {
         return (FALSE);
      } /* Endif */
      return (xdr_opaque(xdrs, cp, count));

   case XDR_DECODE:
      if (cp == NULL) {
         cp = (caddr_t)RTCS_mem_alloc_zero(count);
         if (cp == NULL) {
            fprintf(stderr, "xdr_bytes: out of memory\n");
            return (FALSE);
         } /* Endif */
         *cpp = cp;
      } /* Endif */
      return (xdr_opaque(xdrs, cp, count));

   case XDR_FREE:
      if (cp != NULL) {
         _mem_free(cp);
         *cpp = NULL;
      } /* Endif */
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_string
* Returned Value   : TRUE or FALSE
* Comments  :  XDR null-terminated ASCII strings
*
* It is an error to filter NULL pointers with xdr_string().  Use
* xdr_pointer() instead.
*
*END*-----------------------------------------------------------------*/

bool_t xdr_string
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      char_ptr _PTR_ cpp,
            /* [IN,OUT] - pointer to the string to filter */
      uint_32  maxsize
            /* [IN] - maximum length of the string */
   )
{ /* Body */
   char_ptr cp = *cpp;
   uint_32  count;
   uint_32  size;

      /*
      ** Filter the string length
      */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
   case XDR_FREE:
      if (cp == NULL) {
         return (xdrs->x_op == XDR_FREE);
      } /* Endif */
      count = strlen(cp);

   } /* Endswitch */

   if (!xdr_uint_32(xdrs, &count)) {
      return (FALSE);
   } /* Endif */
   if (count > maxsize) {
      return (FALSE);
   } /* Endif */
   size = count + 1;

      /*
      ** Now, filter the string
      */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      return (xdr_opaque(xdrs, cp, count));

   case XDR_DECODE:
         /* size == 0 means count == LASTUNSIGNED */
      if (size == 0) {
         return (FALSE);
      } /* Endif */
      if (cp == NULL) {
         cp = (char_ptr)RTCS_mem_alloc_zero(size);
         if (cp == NULL) {
            fprintf(stderr, "xdr_string: out of memory\n");
            return (FALSE);
         } /* Endif */
         *cpp = cp;
      } /* Endif */
      cp[count] = '\0';
      return (xdr_opaque(xdrs, cp, count));

   case XDR_FREE:
      _mem_free(cp);
      *cpp = NULL;
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_wrapstring
* Returned Value   : TRUE or FALSE
* Comments  :  XDR null-terminated ASCII strings
*
* This is a simple wrapper for xdr_string.
*
*END*-----------------------------------------------------------------*/

bool_t xdr_wrapstring
   (
      XDR_PTR  xdrs,
            /* [IN] - XDR stream handle */
      char_ptr _PTR_ cpp
            /* [IN,OUT] - pointer to the string to filter */
   )
{ /* Body */
    if (xdr_string(xdrs, cpp, LASTUNSIGNED)) {
        return (TRUE);
    } else {
        return (FALSE);
   } /* Endif */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_vector
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a fixed-length array
*
*END*-----------------------------------------------------------------*/

bool_t xdr_vector
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      pointer     basep,
            /* [IN,OUT] - pointer to the array to filter */
      uint_32     count,
            /* [IN] - size of the opaque data (in bytes) */
      uint_32     elsize,
            /* [IN] - size of an element (in bytes) */
      xdrproc_t   elproc
            /* [IN] - filter to call for each element */
   )
{ /* Body */
   uint_32  i;
   caddr_t  elp = (caddr_t)basep;

   for (i = 0; i < count; i++) {
      if (!(*elproc)(xdrs, elp)) {
         return(FALSE);
      } /* Endif */
      elp += elsize;
   } /* Endfor */
   return(TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_array
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a variable-length array
*
*END*-----------------------------------------------------------------*/

bool_t xdr_array
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      pointer     _PTR_ basep,
            /* [IN,OUT] - pointer to the array to filter */
      uint_32     _PTR_ countp,
            /* [IN,OUT] - pointer to the size of the array (in elements) */
      uint_32     maxsize,
            /* [IN] - maximum size of the array (in elements) */
      uint_32     elsize,
            /* [IN] - size of an element (in bytes) */
      xdrproc_t   elproc
            /* [IN] - filter to call for each element */
   )
{ /* Body */
   caddr_t  elp = (caddr_t)*basep;
   uint_32  count;
   uint_32  size;

      /*
      ** Filter the data length
      */
   if (!xdr_uint_32(xdrs, countp)) {
      return (FALSE);
   } /* Endif */
   count = *countp;
   if (count > maxsize) {
      return (FALSE);
   } /* Endif */
   if (count == 0) {
      return (TRUE);
   } /* Endif */
   size = count * elsize;

      /*
      ** Now, filter the data
      */
   switch (xdrs->x_op) {

   case XDR_ENCODE:
      if (elp == NULL) {
         return (FALSE);
      } /* Endif */
      return (xdr_vector(xdrs, elp, count, elsize, elproc));

   case XDR_DECODE:
      if (elp == NULL) {
         elp = (caddr_t)_mem_alloc_zero(size);
         if (elp == NULL) {
            fprintf(stderr, "xdr_array: out of memory\n");
            return (FALSE);
         } /* Endif */
         *basep = elp;
      } /* Endif */
      return (xdr_vector(xdrs, elp, count, elsize, elproc));

   case XDR_FREE:
      if (elp != NULL) {
         if (!xdr_vector(xdrs, elp, count, elsize, elproc)) {
            return FALSE;
         } /* Endif */
         _mem_free(elp);
         *basep = NULL;
      } /* Endif */
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_union
* Returned Value   : TRUE or FALSE
* Comments  :  XDR a discriminated union
*
* To filter a discriminated union, you create an array of xdr_discrim
* structures, terminated with an entry with a null procedure pointer.
* This routine searches the array of xdr_discrims (listproc) for the
* discriminant (*discrp).  If it finds the value in the array, it calls
* the procedure given in the xdr_discrim to handle the discriminant.
* If it doesn't find the value, the default routine (genproc) is called.
* If there is no specific or default routine, an error is returned.
*
* Calling xdr_union with genproc == NULL_xdrproc_t is an error only if
* *discrp isn't in listproc.
*
*END*-----------------------------------------------------------------*/

bool_t xdr_union
   (
      XDR_PTR           xdrs,
            /* [IN] - XDR stream handle */
      enum_t            _PTR_ discrp,
            /* [IN,OUT] - pointer to the discriminator */
      pointer           unp,
            /* [IN,OUT] - pointer to the union to filter */
      xdr_discrim_ptr   listproc,
            /* [IN] - the array of (discriminant, xdrproc) pairs */
      xdrproc_t         genproc
            /* [IN] - the default union filter */
   )
{ /* Body */
   enum_t            discr;
   xdr_discrim_ptr   curproc = listproc;

      /*
      ** Filter the discriminator
      */
   if (!xdr_enum(xdrs, discrp)) {
      return (FALSE);
   } /* Endif */
   discr = *discrp;

      /*
      ** Search listproc for discr
      */
   for (; curproc->proc != NULL_xdrproc_t; curproc++) {
      if (curproc->value == discr) {
         return ((*(curproc->proc))(xdrs, unp));
      } /* Endif */
   } /* Endfor */

      /*
      ** No match -- execute genproc
      */
   if (genproc == NULL_xdrproc_t) {
      return (FALSE);
   } /* Endif */
   return (*genproc)(xdrs, unp);
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_reference
* Returned Value   : TRUE or FALSE
* Comments  :  XDR pointers
*
* To filter a pointer, this function requires the address of the pointer
* to filter, and the address of a function that can filter the object
* pointed to.
*
* It is an error to filter NULL pointers with xdr_reference().  Use
* xdr_pointer() instead.
*
*END*-----------------------------------------------------------------*/

bool_t xdr_reference
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      caddr_t     _PTR_ objpp,
            /* [IN,OUT] - pointer to the pointer to filter */
      uint_32     size,
            /* [IN] - size of the object pointed to (in bytes) */
      xdrproc_t   objproc
            /* [IN] - filter to call for the object pointed to */
   )
{ /* Body */
   caddr_t  objp = *objpp;

   switch (xdrs->x_op) {
   case XDR_ENCODE:
      if (objp == NULL) {
         return (FALSE);
      } /* Endif */
      return (*objproc)(xdrs, objp);

   case XDR_DECODE:
      if (objp == NULL) {
         objp = (caddr_t)RTCS_mem_alloc_zero(size);
         if (objp == NULL) {
            fprintf(stderr, "xdr_reference: out of memory\n");
            return (FALSE);
         } /* Endif */
         *objpp = objp;
      } /* Endif */
      return (*objproc)(xdrs, objp);

   case XDR_FREE:
      if (objp != NULL) {
         if (!(*objproc)(xdrs, objp)) {
            return (FALSE);
         } /* Endif */
         _mem_free(objp);
         *objpp = NULL;
      } /* Endif */
      return (TRUE);

   default:
      return (FALSE);
   } /* Endswitch */
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdr_pointer
* Returned Value   : TRUE or FALSE
* Comments  :  XDR possibly NULL pointers
*
* This function actually filters the following union:
*
*     union obj_ptr switch (boolean b) {
*     case TRUE:  obj_data data;
*     case FALSE: void     nothing;
*     }
*
* xdr_reference() is used to filter the obj_data if the discriminant
* is TRUE (i.e. pointer is non-NULL).
*
*END*-----------------------------------------------------------------*/

bool_t xdr_pointer
   (
      XDR_PTR     xdrs,
            /* [IN] - XDR stream handle */
      caddr_t     _PTR_ objpp,
            /* [IN,OUT] - pointer to the pointer to filter */
      uint_32     size,
            /* [IN] - size of the object pointed to (in bytes) */
      xdrproc_t   objproc
            /* [IN] - filter to call for the object pointed to */
   )
{ /* Body */
   bool_t discr;

   discr = (*objpp != NULL);
   if (!xdr_bool(xdrs, &discr)) {
      return (FALSE);
   } /* Endif */
   if (!discr) {
      *objpp = NULL;
      return (TRUE);
   } /* Endif */
   return (xdr_reference(xdrs, objpp, size, objproc));
} /* Endbody */


/* EOF */
