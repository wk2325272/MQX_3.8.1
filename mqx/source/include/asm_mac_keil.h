/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2008 Embedded Access Inc.;
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
* $FileName: asm_mac_keil.h$
* $Version : 3.8.4.1$
* $Date    : Feb-13-2012$
*
* Comments:
*   This file contains macros used by the Keil assembler
*
*END************************************************************************/

#ifndef __asm_mac_keil_h__
#define __asm_mac_keil_h__ 1

#define ASM_PREFIX(x) x
          
#define ASM_EXTERN(label)        EXTERN label

#define ASM_ALIGN(value)         ALIGN value
#define ASM_LABEL(label)         label
#define ASM_PUBLIC(label)        EXPORT label
#define ASM_SET(label,value)     label SETA value
#define ASM_CONST16(value)       DCWU value
#define ASM_CONST32(value)       DCDU value
#define ASM_LABEL_CONST32(l,v)   ASM_LABEL(label) ASM_CONST32(value)
#define ASM_SECTION(label)       AREA  label
#define ASM_CODE_SECTION(label)  AREA  |label|, CODE, READONLY
#define ASM_END                  END
#define ASM_EQUATE(label, value) label   EQU  value

 PRESERVE8 {TRUE}

#endif /* __asm_mac_keil_h__ */
