#ifndef __xdr_h__
#define __xdr_h__
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
* $FileName: xdr.h$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the declarations of the XDR Library
*   functions.
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


#include <rpctypes.h>


/*
** XDR provides a conventional way for converting between C data types
** and an external bit-string representation.  Library supplied routines
** provide for the conversion on built-in C data types.  These routines
** and utility routines defined here are used to help implement a type
** encode/decode routine for each user-defined type.
**
** Each data type provides a single procedure which takes two arguments:
**
**    bool_t xdrproc
**       (
**          XDR      *xdrs,
**          <type>   *argresp
**       )
**
** xdrs is an instance of a XDR handle, to which or from which the data
** type is to be converted.  argresp is a pointer to the structure to be
** converted.  The XDR handle contains an operation field which indicates
** which of the operations (ENCODE, DECODE or FREE) is to be performed.
**
** XDR_DECODE may allocate space if *argresp is null.  This data can be
** freed with the XDR_FREE operation.
**
** We write only one procedure per data type to make it easy to keep the
** encode and decode procedures for a data type consistent.  In many cases,
** the same code performs all operations on a user defined type, because
** all the hard work is done in the component type routines.
*/

/*
** The default buffer size for record-oriented XDR.  Used when
** xdrrec_create() is called with a zero sendsize or recvsize.
*/
#define XDRRECBUFSIZE   4000

/*
** XDR operations.  XDR_ENCODE causes the type to be encoded into the
** stream.  XDR_DECODE causes the type to be extracted from the stream.
** XDR_FREE can be used to release the space allocated by an XDR_DECODE
** request.
*/
typedef enum xdr_op {
   XDR_ENCODE=0,
   XDR_DECODE=1,
   XDR_FREE=2
} xdr_op, _PTR_ xdr_op_ptr;

/*
** This is the number of bytes per unit of external data.
*/
#define XDRUNIT_BYTES      (4)
#define XDRUNIT_MASK       (3)
#define XDRUNIT_SLACK(x)   ((-(x)) & XDRUNIT_MASK)
#define XDRUNIT_RNDUP(x)   ((x) + XDRUNIT_SLACK(x))

/*
** The XDR handle.
** Contains the operation which is being applied to the stream, an
** operations vector for the particular implementation (e.g. see xdrmem.c),
** and two private fields for the use of the particular implementation.
*/
typedef struct xdr {
   xdr_op   x_op;          /* operation; fast additional param */
   struct xdr_ops {
      /*
      ** Added arguments to each function pointer as per the actual function
      */
      /* get long from underlying stream */
      bool_t      (_CODE_PTR_ x_getlong)(void _PTR_, int_32 _PTR_);
      /* put long to underlying stream */
      bool_t      (_CODE_PTR_ x_putlong)(void _PTR_, int_32 _PTR_);
      /* get bytes from underlying stream */
      bool_t      (_CODE_PTR_ x_getbytes)(void _PTR_, caddr_t, uint_32);
      /* put bytes to underlying stream */
      bool_t      (_CODE_PTR_ x_putbytes)(void _PTR_, caddr_t, uint_32);
      /* returns bytes off from beginning */
      uint_32     (_CODE_PTR_ x_getpostn)(void _PTR_);
      /* lets you reposition the stream */
      bool_t      (_CODE_PTR_ x_setpostn)(void _PTR_, uint_32);
      /* buf quick ptr to buffered data */
      int_32_ptr  (_CODE_PTR_ x_inline)(void _PTR_, uint_32);
      /* free privates of this stream */
      void        (_CODE_PTR_ x_destroy)(void _PTR_);
   } _PTR_ x_ops;
   caddr_t  x_public;      /* users' data */
   caddr_t  x_private;     /* pointer to private data */
   caddr_t  x_base;        /* private used for position info */
   int_32   x_handy;       /* extra private word */
} XDR, _PTR_ XDR_PTR;

/*
** A xdrproc_t exists for each data type which is to be encoded or decoded.
**
** The second argument to the xdrproc_t is a pointer to an opaque pointer.
** The opaque pointer generally points to a structure of the data type
** to be decoded.  If this pointer is NULL, then the type routines should
** allocate dynamic storage of the appropriate size and return it.
**    bool_t   (*xdrproc_t)(XDR *, caddr_t *);
*/
typedef bool_t (_CODE_PTR_ xdrproc_t)(XDR_PTR, pointer);

/*
** Operations defined on a XDR handle
**
** XDR      *xdrs;
** int_32   *longp;
** caddr_t  addr;
** uint_32  len;
** uint_32  pos;
*/
#define xdr_getlong(xdrs, longp)       \
   (*(xdrs)->x_ops->x_getlong)(xdrs, longp)
#define xdr_putlong(xdrs, longp)       \
   (*(xdrs)->x_ops->x_putlong)(xdrs, longp)

#define xdr_getbytes(xdrs, addr, len)  \
   (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)
#define xdr_putbytes(xdrs, addr, len)  \
   (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)

#define xdr_getpos(xdrs)               \
   (*(xdrs)->x_ops->x_getpostn)(xdrs)
#define xdr_setpos(xdrs, pos)          \
   (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)

#define xdr_inline(xdrs, len)          \
   (*(xdrs)->x_ops->x_inline)(xdrs, len)

#define xdr_destroy(xdrs)              \
   if ((xdrs)->x_ops->x_destroy)       \
      (*(xdrs)->x_ops->x_destroy)(xdrs)

/*
** Support struct for discriminated unions.
** You create an array of xdr_discrim structures, terminated with an
** entry with a null procedure pointer.  The xdr_union routine gets the
** discriminant value and then searches the array of structures for a
** matching value.  If a match is found, the associated XDR routine is
** called to handle that part of the union.  If there is no match, then
** a default routine may be called (see xdr_union()).  If there is no
** match and no default routine, it is an error.
*/
#define NULL_xdrproc_t ((xdrproc_t)0)
typedef struct xdr_discrim {
   enum_t      value;
   xdrproc_t   proc;
} xdr_discrim, _PTR_ xdr_discrim_ptr;

/*
** These are the "generic" XDR routines.
*/
extern bool_t  xdr_void(      XDR_PTR, pointer);
extern bool_t  xdr_int_32(    XDR_PTR,  int_32_ptr);
extern bool_t  xdr_uint_32(   XDR_PTR, uint_32_ptr);
extern bool_t  xdr_int_16(    XDR_PTR,  int_16_ptr);
extern bool_t  xdr_uint_16(   XDR_PTR, uint_16_ptr);
extern bool_t  xdr_int(       XDR_PTR,   int    _PTR_);
extern bool_t  xdr_u_int(     XDR_PTR, u_int    _PTR_);
extern bool_t  xdr_char(      XDR_PTR,  char_ptr);
extern bool_t  xdr_uchar(     XDR_PTR, uchar_ptr);
extern bool_t  xdr_bool(      XDR_PTR, bool_t   _PTR_);
extern bool_t  xdr_enum(      XDR_PTR, enum_t   _PTR_);
extern bool_t  xdr_float(     XDR_PTR, float    _PTR_);
extern bool_t  xdr_double(    XDR_PTR, double   _PTR_);
extern bool_t  xdr_opaque(    XDR_PTR, caddr_t,        uint_32);
extern bool_t  xdr_bytes(     XDR_PTR, caddr_t  _PTR_, uint_32_ptr, uint_32);
extern bool_t  xdr_string(    XDR_PTR, char_ptr _PTR_, uint_32);
extern bool_t  xdr_wrapstring(XDR_PTR, char_ptr _PTR_);
extern bool_t  xdr_vector(    XDR_PTR, pointer,        uint_32, uint_32, xdrproc_t);
extern bool_t  xdr_array(     XDR_PTR, pointer  _PTR_, uint_32_ptr, uint_32, uint_32, xdrproc_t);
extern bool_t  xdr_union(     XDR_PTR, enum_t   _PTR_, pointer, xdr_discrim_ptr, xdrproc_t);
extern bool_t  xdr_reference( XDR_PTR, caddr_t  _PTR_, uint_32, xdrproc_t);
extern bool_t  xdr_pointer(   XDR_PTR, caddr_t  _PTR_, uint_32, xdrproc_t);

#define        xdr_long     xdr_int_32
#define        xdr_u_long   xdr_uint_32
#define        xdr_short    xdr_int_16
#define        xdr_u_short  xdr_uint_16
#define        xdr_u_char   xdr_uchar

extern void    xdr_free(xdrproc_t, pointer);

/*
** These are the public routines for the various implementations of
** XDR streams.
*/

      /* XDR using memory buffers */
extern void    xdrmem_create(XDR_PTR, caddr_t, uint_32, xdr_op);

      /* XDR pseudo records for TCP */
extern void    xdrrec_create(XDR_PTR, uint_32, uint_32, pointer, int_32 (*)(), int_32 (*)());
extern bool_t  xdrrec_endofrecord(XDR_PTR, bool_t);   /* make end of XDR record */
extern bool_t  xdrrec_skiprecord(XDR_PTR);    /* move to beginning of next record */
extern bool_t  xdrrec_eof(XDR_PTR);           /* true if no more input */

#endif
/* EOF */
