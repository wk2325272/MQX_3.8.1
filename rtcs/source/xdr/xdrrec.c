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
* $FileName: xdrrec.c$
* $Version : 3.5.7.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the implementation of the record
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

/*
** These routines interface XDR streams to a TCP/IP connection.
**
** There is a record marking layer between the XDR stream and the
** TCP transport level.  A record is a sequence of record fragments.
** A record fragment is a thirty-two bit header followed by N bytes
** of data, where N is the low 31 bits of the header.  The high
** order bit encodes whether or not the fragment is the last of
** the record (1 => fragment is last, 0 => more fragments to follow).
**
** The fragment/record machinery is not general; it is constructed to
** meet the needs of XDR and RPC based on TCP.
*/

#define XDRREC_LAST_FRAG ((uint_32)1 << 31)


typedef struct recstream {
   pointer     tcp_handle;
   caddr_t     the_buffer;
   uint_32     sendsize;
   uint_32     recvsize;

   /* outgoing bytes */
   int_32      (*writeit)(pointer, caddr_t, int_32);  /* the TCP send() function */
   caddr_t     out_base;      /* start of output buffer */
   caddr_t     out_finger;    /* current position in output buffer */
   caddr_t     out_boundry;   /* end of output buffer */
   uint_32     out_pos;       /* current position in outgoing record */
   uint_32_ptr frag_header;   /* start of current fragment */
   bool_t      frag_sent;     /* true if buffer sent in middle of record */

   /* incoming bytes */
   int_32      (*readit)(caddr_t, caddr_t, int_32);   /* the TCP recv() function */
   uint_32     in_size;       /* size of input buffer (fixed) */
   caddr_t     in_base;       /* start of input buffer */
   caddr_t     in_finger;     /* current position in input buffer */
   caddr_t     in_boundry;    /* end of input buffer */
   uint_32     in_pos;        /* current position in incoming record */
   uint_32     fbtbc;         /* fragment bytes to be consumed */
   bool_t      last_frag;
} RECSTREAM, _PTR_ RECSTREAM_PTR;


static uint_32 xdrrec_bufsize(uint_32);
static bool_t  xdrrec_flushbuf(RECSTREAM_PTR, boolean);
static bool_t  xdrrec_fillbuf(RECSTREAM_PTR);
static bool_t  xdrrec_read(RECSTREAM_PTR, caddr_t, uint_32);
static bool_t  xdrrec_skip(RECSTREAM_PTR, uint_32);
static bool_t  xdrrec_frag(RECSTREAM_PTR);

static bool_t        xdrrec_getlong(void _PTR_, int_32_ptr);
static bool_t        xdrrec_putlong(void _PTR_, int_32_ptr);
static bool_t        xdrrec_getbytes(void _PTR_, caddr_t, uint_32);
static bool_t        xdrrec_putbytes(void _PTR_, caddr_t, uint_32);
static uint_32       xdrrec_getpos(void _PTR_);
static bool_t        xdrrec_setpos(void _PTR_, uint_32);
static int_32_ptr    xdrrec_inline(void _PTR_, uint_32);
static void          xdrrec_destroy(void _PTR_);

static struct xdr_ops xdrrec_ops = {
   xdrrec_getlong,
   xdrrec_putlong,
   xdrrec_getbytes,
   xdrrec_putbytes,
   xdrrec_getpos,
   xdrrec_setpos,
   xdrrec_inline,
   xdrrec_destroy
};




/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_create
* Returned Value   : nothing
* Comments  :  Opens a record-oriented XDR stream
*
* sendsize and recvsize are send and receiver buffer sizes
* (0 => use default).
*
* tcp_handle is an opaque handle that is passed as the first parameter to
* the procedures readit() and writeit().
*
*END*-----------------------------------------------------------------*/

void xdrrec_create
   (
      register XDR_PTR xdrs,
            /* [OUT] - XDR stream handle */
      register uint_32 sendsize,
            /* [IN] - size of send buffer */
      register uint_32 recvsize,
            /* [IN] - size of receive buffer */
      pointer  tcp_handle,
            /* [IN] - handle for readit() and writeit() */
      int_32 (*readit)(),
            /* [IN] - function to read data from transport layer */
      int_32 (*writeit)()
            /* [IN] - function to write data to transport layer */
   )
{ /* Body */
   register RECSTREAM_PTR rstrm =
      (RECSTREAM_PTR)RTCS_mem_alloc_zero(sizeof(RECSTREAM));

   if (rstrm == NULL) {
      /* This is bad.  We have no way to inform the caller that create failed */
      fprintf(stderr, "xdrrec_create: out of memory\n");
      return;
   } /* Endif */

   /*
   ** adjust sizes and allocate quadbyte aligned buffer
   */
   rstrm->sendsize = sendsize = xdrrec_bufsize(sendsize);
   rstrm->recvsize = recvsize = xdrrec_bufsize(recvsize);
   rstrm->the_buffer = (caddr_t)RTCS_mem_alloc_zero(sendsize + recvsize);
   if (rstrm->the_buffer == NULL) {
      fprintf(stderr, "xdrrec_create: out of memory\n");
      return;
   } /* Endif */

   xdrs->x_ops = &xdrrec_ops;
   xdrs->x_private = (caddr_t)rstrm;

   rstrm->tcp_handle = tcp_handle;
   rstrm->readit = readit;
   rstrm->writeit = writeit;

   rstrm->out_base = rstrm->the_buffer;
   rstrm->out_finger = rstrm->out_base + XDRUNIT_BYTES;
   rstrm->out_boundry = rstrm->in_base = rstrm->out_base + sendsize;
   rstrm->in_size = recvsize;
   rstrm->in_boundry = rstrm->in_finger = rstrm->in_base + recvsize;
   rstrm->out_pos = rstrm->in_pos = rstrm->fbtbc = 0;
   rstrm->frag_header = (uint_32_ptr)rstrm->out_base;
   rstrm->frag_sent = FALSE;
   rstrm->last_frag = TRUE;
} /* Endbody */


/*
** The standard XDR operations for record-oriented XDR
*/


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_destroy
* Returned Value   : nothing
* Comments  :  Closes a record-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static void xdrrec_destroy
   (
      void _PTR_   handle            /* [IN] - XDR stream handle */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);

   _mem_free(rstrm->the_buffer);
   _mem_free((caddr_t)rstrm);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_getlong
* Returned Value   : TRUE or FALSE
* Comments  :  Decodes a 32-bit integer from a record-oriented
*              XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_getlong
   (
      void _PTR_  handle,
            /* [IN] - XDR stream handle */
      int_32_ptr  ip
            /* [OUT] - int_32 to decode from stream */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   register int_32_ptr srcp = (int_32_ptr)(rstrm->in_finger);
   char i[XDRUNIT_BYTES];

   /*
   ** Check if there is sufficient room in the in_buffer
   */
   if ((rstrm->fbtbc >= XDRUNIT_BYTES) &&
      (((caddr_t)srcp + XDRUNIT_BYTES) <= rstrm->in_boundry)) {
      /*
      ** This is just an optimization for speed.  The 'else' case
      ** could always be used.
      */
      *ip = (int_32)ntohl((uchar_ptr)srcp);
      rstrm->fbtbc -= XDRUNIT_BYTES;
      rstrm->in_finger += XDRUNIT_BYTES;
   } else {
      if (! xdrrec_getbytes((void _PTR_)xdrs, (caddr_t)i, XDRUNIT_BYTES)) {
         return (FALSE);
      } /* Endif */
      *ip = (int_32)ntohl((uchar_ptr)i);
   } /* Endif */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_putlong
* Returned Value   : TRUE or FALSE
* Comments  :  Encodes a 32-bit integer onto a record-oriented
*              XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_putlong
   (
      void _PTR_  handle,
             /* [IN] - XDR stream handle */
      int_32_ptr  ip
            /* [IN] - int_32 to encode to stream */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   register uint_32_ptr destp;

   /*
   ** If there isn't enough room in the out_buffer, flush it first
   */
   if ((rstrm->out_finger + XDRUNIT_BYTES) > rstrm->out_boundry) {
      rstrm->frag_sent = TRUE;
      if (! xdrrec_flushbuf(rstrm, FALSE)) {
         return (FALSE);
      } /* Endif */
   } /* Endif */
   destp = (uint_32_ptr)rstrm->out_finger;
   rstrm->out_finger += XDRUNIT_BYTES;
   htonl((uchar_ptr)destp, *ip);
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_getbytes
* Returned Value   : TRUE or FALSE
* Comments  :  Decodes bytes from a record-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_getbytes
   (
      void _PTR_   handle,
            /* [IN] - XDR stream handle */
      caddr_t      addr,
            /* [OUT] - bytes to decode from stream */
      uint_32      len
            /* [IN] - number of bytes to decode */
   )
{ /* Body */
   register XDR_PTR  xdrs = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   register int_32 current;

   while (len > 0) {
      current = rstrm->fbtbc;
      if (current == 0) {
         if (rstrm->last_frag) {
            return (FALSE);
         } /* Endif */
         if (! xdrrec_frag(rstrm)) {
            return (FALSE);
         } /* Endif */
         continue;
      } /* Endif */
      if (len < current) {
         current = len;
      } /* Endif */
      if (! xdrrec_read(rstrm, addr, current)) {
         return (FALSE);
      } /* Endif */
      addr += current;
      rstrm->fbtbc -= current;
      len -= current;
   } /* Endwhile */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_putbytes
* Returned Value   : TRUE or FALSE
* Comments  :  Encodes bytes onto a record-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_putbytes
   (
      void _PTR_    handle,
            /* [IN] - XDR stream handle */
      caddr_t  addr,
            /* [IN] - bytes to encode to stream */
      uint_32  len
            /* [IN] - number of bytes to encode */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   register int_32 current;

   while (len > 0) {
      current = (uint_32)rstrm->out_boundry - (uint_32)rstrm->out_finger;
      if (len < current) {
         current = len;
      } /* Endif */
      _mem_copy(addr, rstrm->out_finger, current);
      rstrm->out_finger += current;
      addr += current;
      len -= current;
      if (rstrm->out_finger == rstrm->out_boundry) {
         rstrm->frag_sent = TRUE;
         if (! xdrrec_flushbuf(rstrm, FALSE)) {
            return (FALSE);
         } /* Endif */
      } /* Endif */
   } /* Endwhile */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_getpos
* Returned Value   : number of bytes encoded to or decoded from stream
* Comments  :  Retrieves the current stream position
*
*END*-----------------------------------------------------------------*/

static uint_32 xdrrec_getpos
   (
      void _PTR_   handle            /* [IN] - XDR stream handle */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   int_32 pos;

   switch (xdrs->x_op) {

   case XDR_ENCODE:
      pos = rstrm->out_pos + (rstrm->out_finger - rstrm->out_base);
      break;

   case XDR_DECODE:
      pos = rstrm->in_pos - (rstrm->in_boundry - rstrm->in_finger);
      break;

   default:
      pos = -1;
      break;
   }
   return ((uint_32) pos);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_setpos
* Returned Value   : TRUE or FALSE
* Comments  :  Sets the current stream position
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_setpos
   (
      void _PTR_  handle,            /* [IN] - XDR stream handle */
      uint_32 pos
            /* [IN] - new position for XDR stream */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   uint_32 currpos = xdrrec_getpos((void _PTR_)xdrs);
   int_32 delta = currpos - pos;
   caddr_t newpos;

   if ((int_32)currpos != -1)
      switch (xdrs->x_op) {

      case XDR_ENCODE:
         newpos = rstrm->out_finger - delta;
         if ((newpos > (caddr_t)rstrm->frag_header) &&
            (newpos < rstrm->out_boundry)) {
            rstrm->out_finger = newpos;
            return (TRUE);
         } /* Endif */
         break;

      case XDR_DECODE:
         newpos = rstrm->in_finger - delta;
         if ((delta < (int_32)rstrm->fbtbc) &&
            (newpos <= rstrm->in_boundry) &&
            (newpos >= rstrm->in_base)) {
            rstrm->in_finger = newpos;
            rstrm->fbtbc -= delta;
            return (TRUE);
         } /* Endif */
         break;
      }
   return (FALSE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_inline
* Returned Value   : pointer to the reserved buffer
* Comments  :  Reserves bytes on a record-oriented XDR stream
*
*END*-----------------------------------------------------------------*/

static int_32_ptr xdrrec_inline
   (
      void _PTR_  handle,
            /* [IN] - XDR stream handle */
      uint_32 len
            /* [IN] - size of the buffer (in bytes) */
   )
{ /* Body */
   register XDR_PTR       xdrs  = handle;
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   int_32_ptr buf = NULL;

   switch (xdrs->x_op) {

   case XDR_ENCODE:
      if ((rstrm->out_finger + len) <= rstrm->out_boundry) {
         buf = (int_32_ptr)rstrm->out_finger;
         rstrm->out_finger += len;
      } /* Endif */
      break;

   case XDR_DECODE:
      if ((len <= rstrm->fbtbc) &&
         ((rstrm->in_finger + len) <= rstrm->in_boundry)) {
         buf = (int_32_ptr)rstrm->in_finger;
         rstrm->fbtbc -= len;
         rstrm->in_finger += len;
      } /* Endif */
      break;
   } /* Endswitch */
   return (buf);
} /* Endbody */


/*
** Exported routines to manage XDR records
*/


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_skiprecord
* Returned Value   : TRUE or FALSE
* Comments  :  Discards the remainder of the current record.
*
*  Before reading (deserializing) from the stream, one should always call
*  this procedure to guarantee proper record alignment.
*
*END*-----------------------------------------------------------------*/

bool_t xdrrec_skiprecord
   (
      XDR_PTR xdrs
            /* [IN] - XDR stream handle */
   )
{ /* Body */
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);

   for (;;) {
      if (! xdrrec_skip(rstrm, rstrm->fbtbc)) {
         return (FALSE);
      } /* Endif */
      rstrm->fbtbc = 0;
      if (rstrm->last_frag) {
         break;
      } else if (! xdrrec_frag(rstrm)) {
         return (FALSE);
      } /* Endif */
   } /* Endfor */
   rstrm->last_frag = FALSE;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_eof
* Returned Value   : TRUE or FALSE
* Comments  :  Discards the remainder of the current record, and
*              determines whether there is any data left in the input
*              buffer.
*
*END*-----------------------------------------------------------------*/

bool_t xdrrec_eof
   (
      XDR_PTR xdrs
            /* [IN] - XDR stream handle */
   )
{ /* Body */
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);

   for (;;) {
      if (! xdrrec_skip(rstrm, rstrm->fbtbc)) {
         return (TRUE);
      } /* Endif */
      rstrm->fbtbc = 0;
      if (rstrm->last_frag) {
         break;
      } else if (! xdrrec_frag(rstrm)) {
         return (TRUE);
      } /* Endif */
   } /* Endfor */
   return (rstrm->in_finger == rstrm->in_boundry);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_endofrecord
* Returned Value   : TRUE or FALSE
* Comments  :  Marks the end of the last record fragment in a record,
*              and possibly flushes it to the transport layer.
*
* The client must tell the package when an end-of-record has occurred.
* The second paraemter tells whether the record should be flushed to the
* TCP stream immediately.  TRUE => immmediate flush to TCP connection.
*
*END*-----------------------------------------------------------------*/

bool_t xdrrec_endofrecord
   (
      XDR_PTR xdrs,
            /* [IN] - XDR stream handle */
      bool_t sendnow
            /* [IN] - whether or not to flush the record fragment immediately */
   )
{ /* Body */
   register RECSTREAM_PTR rstrm = (RECSTREAM_PTR)(xdrs->x_private);
   register uint_32 len;

   if (sendnow || rstrm->frag_sent ||
      (rstrm->out_finger + XDRUNIT_BYTES >= rstrm->out_boundry)) {
      rstrm->frag_sent = FALSE;
      return (xdrrec_flushbuf(rstrm, TRUE));
   } else {
      len = (uint_32)(rstrm->out_finger) - (uint_32)(rstrm->frag_header) -
         XDRUNIT_BYTES;
      htonl((uchar_ptr)rstrm->frag_header, (int_32)(len | XDRREC_LAST_FRAG));
      rstrm->frag_header = (uint_32_ptr)rstrm->out_finger;
      rstrm->out_finger += XDRUNIT_BYTES;
      return (TRUE);
   } /* Endif */
} /* Endbody */


/*
** Useful internal functions
*/


/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_flushbuf
* Returned Value   : TRUE or FALSE
* Comments  :  Marks the end of a record fragment, and flushes the output
*              buffer to the transport layer.  The output buffer may
*              contain more than the last record fragment if the previous
*              record fragment ended mid-buffer and was not immediately
*              flushed out.
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_flushbuf
   (
      register RECSTREAM_PTR  rstrm,
            /* [IN/OUT] - record stream handle */
      boolean                 eor
            /* [IN] - whether or not this is the last fragment in the record */
   )
{ /* Body */
   uint_32 eormask = eor ? XDRREC_LAST_FRAG : 0;
   register uint_32 len = (uint_32)(rstrm->out_finger)
                        - (uint_32)(rstrm->frag_header)
                        - XDRUNIT_BYTES;

   htonl((uchar_ptr)rstrm->frag_header, (int_32)(len | eormask));
   len = (uint_32)(rstrm->out_finger) - (uint_32)(rstrm->out_base);
   if ((*(rstrm->writeit))(rstrm->tcp_handle, rstrm->out_base, (int_32)len)
      != (int_32)len) {
      return (FALSE);
   } /* Endif */
   rstrm->frag_header = (uint_32_ptr)rstrm->out_base;
   rstrm->out_finger = (caddr_t)rstrm->out_base + XDRUNIT_BYTES;
   rstrm->out_pos += len;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_fillbuf
* Returned Value   : TRUE or FALSE
* Comments  :  Fills the input buffer with data from the transport layer
*
* NB:  This function knows nothing of record fragments.  It is the
*      caller's responsibility to observe record fragment boundaries.
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_fillbuf
   (
      register RECSTREAM_PTR rstrm
            /* [IN/OUT] - record stream handle */
   )
{ /* Body */
   register caddr_t where;
   register int_32 len;

   where = rstrm->in_base;
   len = (*(rstrm->readit))(rstrm->tcp_handle, where, (int_32)rstrm->in_size);
   if (len == -1) {
      return (FALSE);
   } /* Endif */
   rstrm->in_finger = where;
   where += len;
   rstrm->in_boundry = where;
   rstrm->in_pos += len;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_read
* Returned Value   : TRUE or FALSE
* Comments  :  Retrieves bytes from the input buffer, refilling the
*              buffer (with xdrrec_fillbuf) if necessary.
*
* NB:  This function knows nothing of record fragments.  It is the
*      caller's responsibility to observe record fragment boundaries.
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_read
   (
      register RECSTREAM_PTR rstrm,
            /* [IN/OUT] - record stream handle */
      caddr_t addr,
            /* [OUT] - bytes to read from stream */
      uint_32 len
            /* [IN] - number of bytes to read */
   )
{ /* Body */
   register int_32 current;

   while (len > 0) {
      current = (int_32)rstrm->in_boundry - (int_32)rstrm->in_finger;
      if (current == 0) {
         if (! xdrrec_fillbuf(rstrm)) {
            return (FALSE);
         } /* Endif */
         continue;
      } /* Endif */
      if (len < current) {
         current = len;
      } /* Endif */
      _mem_copy(rstrm->in_finger, addr, current);
      rstrm->in_finger += current;
      addr += current;
      len -= current;
   } /* Endwhile */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_skip
* Returned Value   : TRUE or FALSE
* Comments  :  Discards bytes from the input buffer, refilling the
*              buffer (with xdrrec_fillbuf) if necessary.
*
* NB:  This function knows nothing of record fragments.  It is the
*      caller's responsibility to observe record fragment boundaries.
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_skip
   (
      register RECSTREAM_PTR rstrm,
            /* [IN/OUT] - record stream handle */
      uint_32 len
            /* [IN] - number of bytes to skip */
   )
{ /* Body */
   register int_32 current;

   while (len > 0) {
      current = (int_32)rstrm->in_boundry - (int_32)rstrm->in_finger;
      if (current == 0) {
         if (! xdrrec_fillbuf(rstrm)) {
            return (FALSE);
         } /* Endif */
         continue;
      } /* Endif */
      if (len < current) {
         current = len;
      } /* Endif */
      rstrm->in_finger += current;
      len -= current;
   } /* Endwhile */
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_frag
* Returned Value   : TRUE or FALSE
* Comments  :  Reads the header of a record fragment
*
*END*-----------------------------------------------------------------*/

static bool_t xdrrec_frag
   (
      register RECSTREAM_PTR rstrm
            /* [IN/OUT] - record stream handle */
   )
{ /* Body */
   char netheader[XDRUNIT_BYTES];
   int_32 header;

   if (! xdrrec_read(rstrm, (caddr_t)netheader, XDRUNIT_BYTES)) {
      return (FALSE);
   } /* Endif */
   header = (int_32)ntohl((uchar_ptr)netheader);
   rstrm->last_frag = (header & XDRREC_LAST_FRAG) ? TRUE : FALSE;
   rstrm->fbtbc = header & ~XDRREC_LAST_FRAG;
   return (TRUE);
} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
* Function Name    : xdrrec_bufsize
* Returned Value   : adjusted buffer size
* Comments  :  Quadbyte aligns the requested buffer size.  Also returns
*              the default size if the requested size was zero.
*
*END*-----------------------------------------------------------------*/

static uint_32 xdrrec_bufsize
   (
      uint_32 s
   )
{ /* Body */
   if (s == 0) {
      s = XDRRECBUFSIZE;
   } /* Endif */
   return (XDRUNIT_RNDUP(s));
} /* Endbody */


/* EOF */
