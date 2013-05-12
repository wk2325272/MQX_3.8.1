#ifndef __pcb_h__
#define __pcb_h__
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
* $FileName: pcb.h$
* $Version : 3.7.6.0$
* $Date    : Jan-7-2011$
*
* Comments:
*
*   This file contains the definitions of the generic
*   PCB (Packet Control Block) structure.  
*   Since this structure is common to
*   various protocols, this file is distributed with
*   several products.
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*                        CONSTANT DEFINITIONS                              */


#define __PCB__

#define PCB_free(pcb_ptr)  ((pcb_ptr)->FREE(pcb_ptr))

#define htonl(p, x)     { *(uint_32*)(p) = HOST_TO_BE_LONG(x); }
#define htons(p, x)     { *(uint_16*)(p) = HOST_TO_BE_SHORT(x); }
#define htonc(p, x)     { *(uint_8*)(p) = (x); }
#define ntohl(p)        LONG_BE_TO_HOST(*(uint_32*)(p))
#define ntohs(p)        SHORT_BE_TO_HOST(*(uint_16*)(p))
#define ntohc(p)        (*(uint_8*)(p))

/*--------------------------------------------------------------------------*/
/*
**                          DATA STRUCTURES
*/

/*
** The PCB (Packet Control Block)
** 
** One PCB contains exactly one packet, possibly split over several areas of
** memory.
** 
** The PCB structure consists of two pointers (FREE and PRIVATE) followed by a
** variable-length array of PCB_FRAGMENT structures.  The array is terminated
** by an entry with LENGTH=0 and FRAGMENT=NULL.
**
** The PCB's owner (i.e. the module that allocated it) must initialize the
** FREE field to point to a function to free the PCB.  PRIVATE can be used by
** the PCB's owner to store any information it wishes.
*/

struct pcb;
typedef void (_CODE_PTR_  _pcb_free_fn)(struct pcb _PTR_);

typedef struct pcb_fragment 
{
   uint_32           LENGTH;
   uchar_ptr         FRAGMENT;
} PCB_FRAGMENT, _PTR_ PCB_FRAGMENT_PTR;

typedef struct pcb 
{
//   void (_CODE_PTR_  FREE)(struct pcb _PTR_);
   _pcb_free_fn      FREE;

   pointer           PRIVATE;
   PCB_FRAGMENT      FRAG[1];
} PCB, _PTR_ PCB_PTR;

// The minimum size of a PCB must contain two fragments, so the following represents
// the smallest PCB
typedef struct pcb2
{
   _pcb_free_fn      FREE;
   pointer           PRIVATE;
   PCB_FRAGMENT      FRAG[2];
} PCB2, _PTR_ PCB2_PTR;

#define PCB_MINIMUM_SIZE (sizeof(PCB2))

/*--------------------------------------------------------------------------*/
/*                           EXTERNAL DECLARATIONS                          */

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
