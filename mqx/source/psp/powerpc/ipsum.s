#*HEADER********************************************************************
#* 
#* Copyright (c) 2008 Freescale Semiconductor;
#* All Rights Reserved                       
#*
#* Copyright (c) 1989-2008 ARC International;
#* All Rights Reserved
#*
#*************************************************************************** 
#*
#* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR 
#* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
#* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  
#* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
#* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
#* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
#* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
#* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
#* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
#* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
#* THE POSSIBILITY OF SUCH DAMAGE.
#*
#**************************************************************************
#*
#* $FileName: ipsum.s$
#* $Version : 3.8.3.0$
#* $Date    : Sep-19-2011$
#*
#* Comments:
#*
#*   This file contains the implementation for a one's
#*   complement checksum.  The function can handle any
#*   alignment of the memory area being checksum'd.
#*   The function maximizes speed throughput by checksumming
#*   longwords wherever possible.
#*
#*END***********************************************************************

        .file "ipsum.s"

        .section IPSUM,text

#FUNCTION*--------------------------------------------------------------
#
# Function Name    : _mem_sum_ip(a, len, src)
# Returned Value   : one's complement checksum
# Comments         :
#       This function calculates a 16 bit checksum over the specified data.
#       Note:  This function returns 0 iff all summands are 0.
#
#END*-------------------------------------------------------------------

        .globl _mem_sum_ip

_mem_sum_ip:

        # r3 is the initial sum
        # r4 is the number of bytes to checksum
        # r5 is the source address

        cmpwi   r4,7            # Do we have 7 or less bytes?
        ble     do_small

        li      r11,0           # initialize flag
        andi.   r8,r5,1         # is the address on an odd byte?
        beq     not_odd_address # no, try to align on words

        lbz     r8,0(r5)        # get the byte
        addi    r5,r5,1

        andi.   r7,r3,0xff      # swap bytes in r3
        slwi    r7,r7,8         # move to uppper byte
        extrwi  r9,r3,8,16      # get upper byte into r9's lower byte
        or      r3,r9,r7
                                # 
        add     r3,r3,r8
        subi    r4,r4,1
        li      r11,1           # indicate odd address

not_odd_address:

        andi.   r8,r5,2         # is the address on an odd word boundary?
        beq     done_alignment  # no, address is long aligned already
        lhz     r8,0(r5)        # align the address to a long word
        addi    r5,r5,2
        add     r3,r3,r8
        subi    r4,r4,2

done_alignment:
        addi    r9,r0,1         # need to use r9 as loop constant

        srwi    r6,r4,6         # 64 bytes summed per loop (16 longs)

        subi    r5,r5,4         # Optimize addressing for PowerPC

        addic   r7,r0,0         # Make sure carry in XER is clear
        andi.   r7,r4,0x3C      # Get number of longs (0-15 to do)
                                # NOTE r7 now has #longs * 4
        add     r7,r7,r7        # calculate where to jump to
                                # r7 now has #longs * 8 
                                # it takes 8 bytes to process 1 long word
        addis   r8,r0,do_longs_end@h
        ori     r8,r8,do_longs_end@l
        sub     r8,r8,r7
        mtspr   ctr,r8
        bcctr   20,0

do_longs:
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8

        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8
        lwzu    r8,4(r5)
        adde    r3,r3,r8

do_longs_end:
        sub.    r6,r6,r9
        bge     do_longs

        addi    r5,r5,4         # Restore address for remaining

do_word:
        andi.   r6,r4,2         # Sum last word?
        beq     do_byte
        lhz     r8,0(r5)
        addi    r5,r5,2
        adde    r3,r3,r8

do_byte:
        andi.   r6,r4,1         # Sum last byte?
        beq     done
        lbz     r8,0(r5)
        slwi    r8,r8,8         # move up 1 byte
        adde    r3,r3,r8

done:   srwi    r5,r3,16        # Get top 16 bits into r5
        clrlwi  r3,r3,16        # get bottom 16 bits in r3
        adde    r3,r5,r3        # Add with carry, carry no longer be set after this
                                # Highest possible number in r3 is 0x1FFFF
        srwi.   r5,r3,16        # Get top 16 bits into r5
        bne     more_folding

        cmpwi   r11,1           # was orignal address byte aligned
        bnelr                   # no, so just return
        andi.   r5,r3,0xff      # get lower byte
        slwi    r5,r5,8         # move to uppper byte
        extrwi  r4,r3,8,16      # get upper byte into r4's lower byte
        or      r3,r4,r5
        blr

more_folding:
        clrlwi  r3,r3,16        # get bottom 16 bits in r5
        adde    r3,r5,r3        # Highest possible number in r3 is 0x10000
        srwi.   r5,r3,16        # Get top 16 bits into r3
        bne     more_folding

        cmpwi   r11,1           # was orignal address byte aligned
        bnelr                   # no, so just return
        andi.   r5,r3,0xff      # get lower byte
        slwi    r5,r5,8         # move to uppper byte
        extrwi  r4,r3,8,16      # get upper byte into r4's lower byte
        or      r3,r4,r5
        blr



do_small:
        andi.   r6,r4,4         # Sum long word?
        beq     do_small_word
        lhz     r8,0(r5)
        addi    r5,r5,2
        add     r3,r3,r8
        lhz     r8,0(r5)
        addi    r5,r5,2
        add     r3,r3,r8
do_small_word:
        andi.   r6,r4,2         # Sum last word?
        beq     do_small_byte
        lhz     r8,0(r5)
        addi    r5,r5,2
        add     r3,r3,r8
do_small_byte:
        andi.   r6,r4,1         # Sum last byte?
        beq     do_small_done
        lbz     r8,0(r5)
        slwi    r8,r8,8         # move up 1 byte
        add     r3,r3,r8
do_small_done:
        srwi    r5,r3,16        # Get top 16 bits into r5
do_small_folding:
        clrlwi  r3,r3,16        # get bottom 16 bits in r3
        add     r3,r5,r3
        srwi.   r5,r3,16        # Get top 16 bits into r5
        bne     do_small_folding
        blr

# EOF
