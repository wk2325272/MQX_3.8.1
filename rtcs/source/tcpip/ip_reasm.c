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
* $FileName: ip_reasm.c$
* $Version : 3.8.11.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file contains the code to handle IP reassembly
*
*END************************************************************************/

#include <stddef.h>
#include <rtcs.h>
#include "rtcs_prv.h"
#include "tcpip.h"
#include "ip_prv.h"
#include "icmp_prv.h"
#if RTCSCFG_ENABLE_IP_REASSEMBLY

/*
** Datagrams awaiting reassembly are stored in sets of 256-byte
** blocks.  We allocate a partition of memory blocks; each block
** can be used in one of three ways:
**
**    1) It can store up to 256 data bytes from the datagram.
**       This is an IPREASM_BLK, called a 'direct block'.
**       This block also has 32-bit bitfield indicating which
**       part(s) of the direct block contain(s) valid data.
**       Each bit represents 8 bytes, so (32 bits) * (8 bytes/bit)
**       = 256 bytes.
**
**    2) It can be used as an array of pointers to IPREASM_BLKs
**       This is an IPREASM_INDIDX, called an 'indirect block'.
**       The indirect block could hold up to 65 direct block
**       pointers (257 in 32-bit wide memory), but we only
**       store 64 (or 256) to simplify the math.  i.e.
**       IPREASM_IND_SIZE could be
**          sizeof(IPREASM_BLK)/sizeof(IPREASM_BLK_PTR),
**       but we only set it to
**          IPREASM_BLK_SIZE/sizeof(IPREASM_BLK_PTR),
**
**    3) It can hold state information about an IP datagram, an
**       IP_DGRAM structure.  In particular, it has the datagram
**       'key' (the IP source, dest, protocol and ID), and as
**       many pointers to indirect blocks as it needs to address
**       a maximum sized datagram (256 direct blocks), i.e. 4
**       (or 1).  However, the IP_DGRAM structure is quite a
**       bit smaller than an IPREASM_BLK, so difference is
**       filled with pointers to direct blocks -- as many as
**       will fit without exceeding the size of an IPREASM_BLK.
**       When allocating blocks for incoming IP datagram fragments,
**       the direct block pointers in the IP_DGRAM are used first,
**       followed by the indirect block pointers.
*/


/* The size of the IP reassembly partition */
#define IPREASM_PART_MAXSIZE     (40*1024)
#define IPREASM_PART_GROW        32          /* 8 KB */
#define IPREASM_PART_MAX         (IPREASM_PART_MAXSIZE / sizeof(IPREASM_BLK))


/* Size of a block in octets */
#define IPREASM_BLK_SIZE         256
#define IPREASM_BLK_NUM(offset)  ((offset) / IPREASM_BLK_SIZE)
#define IPREASM_BLK_OFS(offset)  ((offset) % IPREASM_BLK_SIZE)
#define IPREASM_NB_BLK           (0x10000 / IPREASM_BLK_SIZE)

/* Number of direct block pointers that will fit in an IP_DGRAM */
#define IPREASM_NB_DIRECT        ((sizeof(IPREASM_BLK)-offsetof(IP_DGRAM,DIRECT)) \
                                  / sizeof(IPREASM_BLK_PTR))
#define IPREASM_IS_DIRECT(block) ((block) < IPREASM_NB_DIRECT)

/* Number of direct block pointers in each indirect block */
#define IPREASM_IND_SIZE         (IPREASM_BLK_SIZE / sizeof(IPREASM_BLK_PTR))
#define IPREASM_IND_NUM(block)   (((block)-IPREASM_NB_DIRECT) / IPREASM_IND_SIZE)
#define IPREASM_IND_OFS(block)   (((block)-IPREASM_NB_DIRECT) % IPREASM_IND_SIZE)
#define IPREASM_NB_IND           (IPREASM_NB_BLK / IPREASM_IND_SIZE)


typedef struct ipreasm_blk {
   uint_32  BUSYINT; /* the bitfield, 1 bit set = 8 bytes copied */
                     /* the LSB is DATA[0-7] */
                     /* the MSB is DATA[249-256] */
   uchar    DATA[IPREASM_BLK_SIZE];
} IPREASM_BLK, _PTR_ IPREASM_BLK_PTR;

/* the structure of an indirect block */
typedef struct ipreasm_indidx {
   IPREASM_BLK_PTR   DIRECT[IPREASM_IND_SIZE];
} IPREASM_INDIDX, _PTR_ IPREASM_INDIDX_PTR;

typedef struct ip_dgram {
   /* the "key" of the dgram.  rfc815.7 */
   _ip_address IPSRC, IPDST;
   uint_8      PROTO;
   uint_8      RESERVED;
   uint_16     ID;

   /* move-to-front list of IP_DGRAMs */
   struct ip_dgram _PTR_       NEXT;
   struct ip_dgram _PTR_ _PTR_ PREV;

   /* the IP header of the complete datagram */
   IP_HEADER   IPH;
   /* leave rooms for the ip options. MUST follow the IPH field */
   uchar       IP_OPTIONS[60 - sizeof(IP_HEADER)];
   uchar       ICMP_DATA[8];

   /* Information from the PCB with the first fragment */
   uint_32              TYPE;
   pointer              IFSRC;
   RTCS_LINKOPT_STRUCT  LINKOPT;

   uint_32    TOTLEN; /* the data of the complete ip datagram payload */
   uint_32    CURLEN; /* the current data size */
   uint_32    MAXLEN; /* the last fragment seen */

   TCPIP_EVENT TIMER; /* the timer to handle datagram expiry */

   IPREASM_INDIDX_PTR   INDIR[IPREASM_NB_IND];
   IPREASM_BLK_PTR      DIRECT[1];
} IP_DGRAM, _PTR_ IP_DGRAM_PTR;


static IP_DGRAM_PTR  IPREASM_head;  /* list of all IP_DGRAMs */
static _rtcs_part    IPREASM_part;  /* the partition id for the buffer */


static IPREASM_BLK_PTR IPREASM_blk_get      (IP_DGRAM_PTR, uint_32);
static uint_32         IPREASM_blk_write    (IP_DGRAM_PTR, IPREASM_BLK_PTR, uint_32, uchar_ptr, uint_32);
static void            IPREASM_blk_read_all (IP_DGRAM_PTR, uchar_ptr, uint_32);
static void            IPREASM_blk_del_all  (IP_DGRAM_PTR);

static IP_DGRAM_PTR IPREASM_get_dgram   (_ip_address, _ip_address, uint_8, uint_16);
static void         IPREASM_del_dgram   (IP_DGRAM_PTR);
static void         IPREASM_add_frag    (IP_DGRAM_PTR, RTCSPCB_PTR);
static RTCSPCB_PTR  IPREASM_reasm_dgram (IP_DGRAM_PTR);

static boolean IPREASM_expire (TCPIP_EVENT_PTR);


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IP_reasm_init
*  Returned Value : RTCS_OK or error code
*  Comments       : called by IP_init()
*
*END*-----------------------------------------------------------------*/

uint_32 IP_reasm_init
   (
      void
   )
{ /* Body */

   RTCS_DLIST_INIT(IPREASM_head);

   /*
   ** Allocate the memory partition.  Use an initial count of zero, since we
   ** usually never need to reassemble datagrams.
   */
   IPREASM_part =
      RTCS_part_create(sizeof(IPREASM_BLK),
         0, IPREASM_PART_GROW, IPREASM_PART_MAX, NULL, NULL);
   if (IPREASM_part == 0) {
      return RTCSERR_OUT_OF_MEMORY;
   } /* Endif */

   return RTCS_OK;
} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_blk_get
*  Returned Value : pointer to block containing offset
*  Comments       :
*     Retrieve the block containing a particular datagram offset,
*     allocating one if necessary.
*
*END*-----------------------------------------------------------------*/

static IPREASM_BLK_PTR IPREASM_blk_get
   (
      IP_DGRAM_PTR   dgram,  /* [IN] the IP datagram */
      uint_32        offset  /* [IN] the offset into the datagram */
   )
{ /* Body */
   IPREASM_BLK_PTR   blk;
   uint_32           nb;
   uint_32           ni;

   nb = IPREASM_BLK_NUM(offset);
   if (IPREASM_IS_DIRECT(nb)) {
      blk = dgram->DIRECT[nb];
      if (!blk) {
         blk = RTCS_part_alloc(IPREASM_part);
         if (blk) {
            blk->BUSYINT = 0;
            dgram->DIRECT[nb] = blk;
         } /* Endif */
      } /* Endif */
      return blk;
   } /* Endif */

   /* If we get here, it's an indirect block */

   ni = IPREASM_IND_NUM(nb);
   if (ni >= IPREASM_NB_IND) {
      return NULL;
   } /* Endif */

   if (!dgram->INDIR[ni]) {
      dgram->INDIR[ni] = RTCS_part_alloc_zero(IPREASM_part);
      if (!dgram->INDIR[ni]) {
         return NULL;
      } /* Endif */
   } /* Endif */

   nb = IPREASM_IND_OFS(nb);
   blk = dgram->INDIR[ni]->DIRECT[nb];
   if (!blk) {
      blk = RTCS_part_alloc(IPREASM_part);
      if (blk) {
         blk->BUSYINT = 0;
         dgram->INDIR[ni]->DIRECT[nb] = blk;
      } /* Endif */
   } /* Endif */
   return blk;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_blk_write
*  Returned Value : number of bytes written
*  Comments       :
*     Writes data to blk at offset.
*
*END*-----------------------------------------------------------------*/

static uint_32 IPREASM_blk_write
   (
      IP_DGRAM_PTR      dgram,
      IPREASM_BLK_PTR   blk,
      uint_32           offset,
      uchar_ptr         ptr,
      uint_32           len
   )
{ /* Body */
   uint_32 blkoffset, bitoffset, numbits;
   uint_32 mask, oldbits;

   blkoffset = IPREASM_BLK_OFS(offset);

   if (blkoffset + len > IPREASM_BLK_SIZE) {
      len = IPREASM_BLK_SIZE - blkoffset;
   } /* Endif */
   _mem_copy(ptr, blk->DATA + blkoffset, len);

   bitoffset = blkoffset >> 3;  /* first bit to set in BUSYINT */
   numbits   = (len+7)   >> 3;  /* number of bits to set */
   /* mask = (1 << (bitoffset+numbits)) - (1 << bitoffset) */
   mask = (2 << (bitoffset+numbits-1)) - (1 << bitoffset);

   oldbits = mask & blk->BUSYINT;
   while (oldbits) {
      oldbits &= oldbits - 1;
      numbits--;
   } /* Endfor */
   blk->BUSYINT |= mask;

   dgram->CURLEN += numbits << 3;
   if (dgram->MAXLEN < offset + len) {
      dgram->MAXLEN = offset + len;
   } /* Endif */

   return len;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_blk_read_all
*  Returned Value : void
*  Comments       :
*     Reads data into ptr.
*
*END*-----------------------------------------------------------------*/

static void IPREASM_blk_read_all
   (
      IP_DGRAM_PTR   dgram,
      uchar_ptr      ptr,
      uint_32        len
   )
{ /* Body */
   uint_32              i, j;
   IPREASM_INDIDX_PTR   indir;

   /* Read the direct blocks */
   for (i = 0; i < IPREASM_NB_DIRECT; i++) {
      if (len <= IPREASM_BLK_SIZE) {
         _mem_copy(dgram->DIRECT[i]->DATA, ptr, len);
         return;
      } /* Endif */
      _mem_copy(dgram->DIRECT[i]->DATA, ptr, IPREASM_BLK_SIZE);
      ptr += IPREASM_BLK_SIZE;
      len -= IPREASM_BLK_SIZE;
   } /* Endfor */

   /* Read the indirect blocks */
   for (i = 0; i < IPREASM_NB_IND; i++) {
      indir = dgram->INDIR[i];
      for (j = 0; j < IPREASM_IND_SIZE; j++) {
         if (len <= IPREASM_BLK_SIZE) {
            _mem_copy(indir->DIRECT[j]->DATA, ptr, len);
            return;
         } /* Endif */
         _mem_copy(indir->DIRECT[j]->DATA, ptr, IPREASM_BLK_SIZE);
         ptr += IPREASM_BLK_SIZE;
         len -= IPREASM_BLK_SIZE;
      } /* Endfor */
   } /* Endfor */

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_blk_del_all
*  Returned Value : void
*  Comments       :
*     Free all blocks allocated to a datagram.
*
*END*-----------------------------------------------------------------*/

static void IPREASM_blk_del_all
   (
      IP_DGRAM_PTR    dgram   /* [IN] the IP datagram */
   )
{ /* Body */
   uint_32              i, j;
   IPREASM_INDIDX_PTR   indir;

   /* free the direct blocks */
   for (i = 0; i < IPREASM_NB_DIRECT; i++) {
      if (!dgram->DIRECT[i]) continue;
      RTCS_part_free(dgram->DIRECT[i]);
   } /* Endfor */

   /* free the indirect blocks */
   for (i = 0; i < IPREASM_NB_IND; i++) {
      indir = dgram->INDIR[i];
      if (!indir) continue;
      for (j = 0; j < IPREASM_IND_SIZE; j++) {
         if (!indir->DIRECT[j]) continue;
         RTCS_part_free(indir->DIRECT[j]);
      } /* Endfor */
      RTCS_part_free(indir);
   } /* Endfor */

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_get_dgram
*  Returned Value : pointer the matching IP datagram
*  Comments       :
*     Find the IP datagram matching the given (ipsrc, ipdst, proto, id)
*     key.
*
*     If the structure isn't in the list, allocate one.
*
*END*-----------------------------------------------------------------*/

static IP_DGRAM_PTR IPREASM_get_dgram
   (
      _ip_address ipsrc,   /* [IN] the IP source */
      _ip_address ipdst,   /* [IN] the IP destination */
      uint_8      proto,   /* [IN] the IP protocol */
      uint_16     id       /* [IN] the IP ID */
   )
{ /* Body */
   register IP_DGRAM_PTR dgram;

   RTCS_DLIST_PEEK(IPREASM_head, dgram);
   if (dgram) {

      /* There's a good probability our IP_DGRAM is at the head of the list */
      if (ipsrc == dgram->IPSRC
       && ipdst == dgram->IPDST
       && proto == dgram->PROTO
       && id    == dgram->ID) {
         return dgram;
      } /* Endif */

      /* It's not at the head.  Scan the list for it */
      RTCS_DLIST_SEARCH_REST(IPREASM_head, dgram) {
         if (ipsrc == dgram->IPSRC
          && ipdst == dgram->IPDST
          && proto == dgram->PROTO
          && id    == dgram->ID) {
            /* Found it.  Move it to the head of the list */
            RTCS_DLIST_DEL(IPREASM_head, dgram);
            RTCS_DLIST_INS(IPREASM_head, dgram);
            return dgram;
         } /* Endif */
      } /* End SEARCH */

   } /* Endif */

   /* Not found.  Allocate one */
   dgram = RTCS_part_alloc_zero(IPREASM_part);
   if (!dgram) return NULL;

   /* Initialize it */
   dgram->IPSRC = ipsrc;
   dgram->IPDST = ipdst;
   dgram->PROTO = proto;
   dgram->ID    = id;
   dgram->TIMER.TIME  = IPREASM_TTL;
   dgram->TIMER.EVENT = IPREASM_expire;
   dgram->TIMER.PRIVATE = dgram;
   TCPIP_Event_add(&dgram->TIMER);

   /* Add it to the list*/
   RTCS_DLIST_INS(IPREASM_head, dgram);
   return dgram;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_del_dgram
*  Returned Value : void
*  Comments       :
*     Free an IP datagram.
*
*END*-----------------------------------------------------------------*/

static void IPREASM_del_dgram
   (
      IP_DGRAM_PTR   dgram    /* [IN] the IP datagram */
   )
{ /* Body */

   /* Cancel the expire timer and free all memory */
   TCPIP_Event_cancel(&dgram->TIMER);
   RTCS_DLIST_DEL(IPREASM_head, dgram);
   IPREASM_blk_del_all(dgram);
   RTCS_part_free(dgram);

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_add_frag
*  Returned Value : void
*  Comments       :
*     Adds a received IP fragment to an IP datagram.
*
*END*-----------------------------------------------------------------*/

static void IPREASM_add_frag
   (
      IP_DGRAM_PTR   dgram,   /* [IN] the dgram */
      RTCSPCB_PTR    inpcb    /* [IN] the received PCB */
   )
{ /* Body */
   IP_HEADER_PTR     iph     = (IP_HEADER_PTR)RTCSPCB_DATA(inpcb);
   uint_32           iphlen  = (ntohc(iph->VERSLEN) & 0xF) << 2;
   uint_32           totlen  = ntohs(iph->LENGTH);
   int_32            offset  = ntohs(iph->FRAGMENT);
   int_32            first   = (offset & IP_FRAG_MASK) << IP_FRAG_SHIFT;
   uint_32           datalen = totlen - iphlen;
   uchar_ptr         data    = (uchar_ptr)iph + iphlen;
   IPREASM_BLK_PTR   blk;
   uint_32           len;

   if (offset & IP_FRAG_MF) {

      /* Fragments (except the last) must be multiples of 8 bytes */
      if ((datalen & 0x07) != 0) {
         return;
      } /* Endif */

   } else {
      /* If this fragment is the last one, record the total length */
      dgram->TOTLEN = first + datalen;

   } /* Endif */

   /* Once we get the first fragment, record the IP header and 8 data bytes */
   if (first == 0) {
      _mem_copy(iph, &dgram->IPH, iphlen + 8);
      dgram->TYPE    = inpcb->TYPE;
      dgram->IFSRC   = inpcb->IFSRC;
      dgram->LINKOPT = inpcb->LINK_OPTIONS.RX;
   } /* Endif */

   while (datalen) {
      blk = IPREASM_blk_get(dgram, first);
      if (!blk) {
         return;
      } /* Endif */

      len = IPREASM_blk_write(dgram, blk, first, data, datalen);
      first   += len;
      data    += len;
      datalen -= len;
   } /* Endwhile */

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IPREASM_reasm_dgram
*  Returned Value : RTCSPCB containing the completed datagram.
*  Comments       :
*     Reassemble a completed datagram.
*
*END*-----------------------------------------------------------------*/

static RTCSPCB_PTR IPREASM_reasm_dgram
   (
      IP_DGRAM_PTR   dgram    /* [IN] the dgram descriptor */
   )
{ /* Body */
   RTCSPCB_PTR outpcb;
   PCB_PTR     bpcb;
   PCB_FRAGMENT _PTR_ pcb_frag_ptr;
   uchar_ptr   data;
   uint_32     iphlen  = (ntohc(dgram->IPH.VERSLEN) & 0xF) << 2;
   uint_32     ip_totlen = iphlen + dgram->TOTLEN;

   bpcb = RTCS_mem_alloc_system(sizeof(PCB) + sizeof(PCB_FRAGMENT) + ip_totlen);
   if (!bpcb) {
      return NULL;
   } /* Endif */

   data = (uchar_ptr)bpcb + sizeof(PCB) + sizeof(PCB_FRAGMENT);
   bpcb->FREE    = (void(_CODE_PTR_)(PCB_PTR))_mem_free;
   bpcb->PRIVATE = NULL;
 
   pcb_frag_ptr = bpcb->FRAG;
   pcb_frag_ptr->LENGTH   = ip_totlen;
   pcb_frag_ptr->FRAGMENT = data;
   pcb_frag_ptr++;
   pcb_frag_ptr->LENGTH   = 0;
   pcb_frag_ptr->FRAGMENT = NULL;

   /* Copy the IP header with options */
   htons(dgram->IPH.FRAGMENT, 0);
   _mem_copy(&dgram->IPH, data, iphlen);
   data += iphlen;

   /*
   ** At this point, we really should update the LENGTH
   ** and CHECKSUM fields in the new IP header, but we
   ** don't actually need to, because this datagram is
   ** going straight to IPLOCAL_service, which doesn't
   ** check these things.
   */

   /* Copy the stored data in the new packet */
   IPREASM_blk_read_all(dgram, data, dgram->TOTLEN);

   /* Put it in an RTCSPCB */
   outpcb = RTCSPCB_alloc_recv(bpcb);
   if (outpcb == NULL) {
      PCB_free(bpcb);
      return NULL;
   } /* Endif */
   //RTCSLOG_PCB_ALLOC(bpcb);
   outpcb->IFSRC           = dgram->IFSRC;
   outpcb->TYPE            = dgram->TYPE;
   outpcb->LINK_OPTIONS.RX = dgram->LINKOPT;
   RTCSPCB_DATA_NETWORK(outpcb) = RTCSPCB_DATA(outpcb);
   RTCSPCB_SET_TRANS_PROTL(outpcb, dgram->PROTO);
   RTCSPCB_SET_TRANS_DELTA(outpcb, iphlen);

   /* Delete the local structure */
   IPREASM_del_dgram(dgram);

   return outpcb;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name   : IPREASM_expire
*  Returned Values : boolean
*  Comments        :
*     Called by the Timer.  Expire an IP datagram.
*
*END*-----------------------------------------------------------------*/

static boolean IPREASM_expire
   (
      TCPIP_EVENT_PTR   event    /* [IN/OUT] the expire event */
   )
{ /* Body */
   IP_CFG_STRUCT_PTR IP_cfg_ptr = RTCS_getcfg(IP);
   IP_DGRAM_PTR      dgram = event->PRIVATE;

   IF_IP_STATS_ENABLED(IP_cfg_ptr->STATS.ST_RX_FRAG_DISCARDED++);

#if RTCSCFG_ENABLE_ICMP
   /* Send a ICMP Time Exceeded */
   if (dgram->DIRECT[0]
    && dgram->DIRECT[0]->BUSYINT & 1
    && dgram->TYPE & RTCSPCB_TYPE_UNICAST) {
      ICMP_send_error_internal(ICMPTYPE_TIMEEXCEED, ICMPCODE_TE_REASM, 0, 
         &dgram->IPH, NULL, 0);
   } /* Endif */
#endif

   IPREASM_del_dgram(dgram);
   return FALSE;

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : IP_reasm
*  Returned Value : RTCS_OK or error code
*  Comments       :
*     Process an IP fragment.  If an error occurs, outpcb is
*     guaranteed to be NULL.
*
*  Warning:
*     It is assumed the PCB is not fragmented.  This is
*     true for all PCBs that come from the link layer.
*     Loopback PCBs generally are fragmented, but IP
*     doesn't FRAGMENT them, so they don't go through
*     this function.
*
*  Warning:
*     I just realized that the word 'fragment' is overloaded
*     in the warning above.  There are PCB fragments and IP
*     fragments.  I've rewritten the previous warning so that
*     PCB fragments are 'fragments' (lowercase) and IP fragments
*     are 'FRAGMENTS' (uppercase).
*
*END*-----------------------------------------------------------------*/

uint_32 IP_reasm
   (
      RTCSPCB_PTR       pcb,     /* [IN] the packet to deliver */
      RTCSPCB_PTR _PTR_ outpcb   /* [OUT] the reassembled packet or NULL */
   )
{ /* Body */
   IP_HEADER_PTR  iph    = (IP_HEADER_PTR)RTCSPCB_DATA(pcb);
   _ip_address    ipsrc  = ntohl(iph->SOURCE);
   _ip_address    ipdst  = ntohl(iph->DEST);
   uint_8         proto  = ntohc(iph->PROTOCOL);
   uint_16        id     = ntohs(iph->ID);
   IP_DGRAM_PTR   dgram;

   *outpcb = NULL;

   dgram = IPREASM_get_dgram(ipsrc, ipdst, proto, id);
   if (!dgram) {
      RTCSLOG_PCB_FREE(pcb, RTCSERR_OUT_OF_MEMORY);
      RTCSPCB_free(pcb);
      return RTCSERR_OUT_OF_MEMORY;
   } /* Endif */

   IPREASM_add_frag(dgram, pcb);

   /*
   ** We're done when:
   **    0 < TOTLEN = MAXLEN <= CURLEN
   **
   ** Note: we can get TOTLEN < CURLEN because CURLEN is
   ** always a multiple of eight, but TOTLEN may not be.
   */
   if (dgram->TOTLEN > 0
    && dgram->MAXLEN == dgram->TOTLEN
    && dgram->CURLEN >= dgram->MAXLEN) {
      *outpcb = IPREASM_reasm_dgram(dgram);
   } /* Endif */

   /* Free the old pcb */
   RTCSLOG_PCB_FREE(pcb, RTCS_OK);
   RTCSPCB_free(pcb);

   return RTCS_OK;
} /* Endbody */

#endif
/* EOF */
