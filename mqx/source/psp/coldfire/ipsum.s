;*HEADER********************************************************************
;* 
;* Copyright (c) 2008 Freescale Semiconductor;
;* All Rights Reserved                       
;*
;* Copyright (c) 1989-2008 ARC International;
;* All Rights Reserved
;*
;*************************************************************************** 
;*
;* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
;* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
;* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
;* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
;* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
;* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
;* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
;* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
;* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
;* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
;* THE POSSIBILITY OF SUCH DAMAGE.
;*
;**************************************************************************
;*
;* $FileName: ipsum.s$
;* $Version : 3.8.6.0$
;* $Date    : Aug-30-2011$
;*
;* Comments:
;*
;*   This file contains the implementation for a one's
;*   complement checksum.
;*   The function can handle ANY alignment for the data area.
;*   It is optimized for the 68020 and above in order to
;*   perform in the best possible manner.
;*
;*END***********************************************************************

#include <asm_mac.h>
#include "user_config.h"
        ; Start CR 2042
        ;.file "ipsum.s"
        ; End CR 2042
#ifdef __ACF__
        RSEG IPSUM:CODE (4)
#else
        .text
        .section IPSUM,16,x
#endif

;FUNCTION*--------------------------------------------------------------
;
; Function Name    : __mem_sum_ip(a, len, src)
; Returned Value   : one's complement checksum
; Comments         :
;       This function calculates a 16 bit checksum over the specified data.
;
;       Note:  This function returns 0 iff all summands are 0.
;
;END*-------------------------------------------------------------------

        ASM_PUBLIC(_mem_sum_ip)

ASM_PREFIX(_mem_sum_ip):

#if PSP_ABI == PSP_ABI_STD
        clr.l   d1
        clr.l   d0
        movea.l (12,sp),a0       ; a0 is the source address
        move.w  (10,sp),d1       ; d1 is the number of bytes to checksum
        move.w  (6,sp),d0        ; d0 is the initial sum
#endif

        move.l  a1,-(sp)
        move.l  d2,-(sp)
        move.l  d3,-(sp)

;
;   Here is the CW stack frame for this function:
;
;       +20 src (A0)
;       +16 len (D1)
;       +12 a   (D0)
;   IAR call frame starts here:
;       +8  return PC
;       +4  old D2
;   A7   => old D3
;

        cmp.l   #3,d1           ; Do we have 3 or less bytes?
        ble.w   try_words

;
;   Note:  If the source address isn't word aligned, we could align it,
;   perform a byte-swapped sum, and byte-swap the result (this works).
;   However, since RTCS never uses non word aligned addresses, this
;   isn't implemented.
;

        move.l  a0,d2           ; Try to get onto even word
        btst.l  #1,d2
        beq.b   try_longs       ; YES, address is long aligned already
        clr.l   d3              ; NO, align that puppy
        move.w  (a0)+,d3
        add.l   d3,d0
        subq.l  #2,d1

try_longs:
        move.l  d1,d2
        lsr.l   #6,d2           ; 64 bytes summed per loop
        movea.l d2,a1

        move.l  d1,d3           ; 1 long processed by 4 bytes
        and.l   #0x3C,d3
        neg.l   d3
        
#ifdef __ACF__
        move.b  #0,ccr          ; Clear X
#else
        move.w  #0,ccr          ; Clear X
#endif

#ifdef __ACF__
        // ACF does not do this jump correctly this is a hack for now
        DC32   0x4EFB3842
#else
        jmp     66(pc,d3.l)
#endif
do_longs:
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0

        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0
        move.l  (a0)+,d3
        addx.l  d3,d0

do_longs_end:
        suba.l  #1,a1
        tst.l   a1
        bge.b     do_longs

do_word:
        btst    #1,d1           ; Sum last word
        beq.b     do_byte
        clr.l   d3
        move.w  (a0)+,d3
        addx.l  d3,d0

do_byte:
        btst    #0,d1           ; Sum last byte
        beq.b   done
        clr.l   d3
        addx.l  d3,d0            ; add the carry if there is one.
        move.b  (a0)+,d3
        asl.l   #8,d3
        add.l   d3,d0

done:   move.l  d0,d1           ; Fold 32 bit sum into 16
        swap    d1
        and.l   #0xFFFF,d0
        and.l   #0xFFFF,d1
        addx.l  d1,d0
        move.l  d0,d1
        swap    d1
        add.l   d1,d0
        and.l   #0xFFFF,d0

        move.l  (sp)+,d3
        move.l  (sp)+,d2
        movea.l  (sp)+,a1
        rts
        ASM_CONST16(0x0000)

try_words:
#ifdef __ACF__
        move.b  #0,ccr          ; Clear X
#else
        move.w  #0,ccr          ; Clear X
#endif
        bra.b   do_word

 
; EOF
