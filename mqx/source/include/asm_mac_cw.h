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
* $FileName: asm_mac_cw.h$
* $Version : 3.8.1.0$
* $Date    : Aug-30-2011$
*
* Comments:
*   This file contains macros used by the CW assembler 
*
*END************************************************************************/

#ifndef __asm_mac_cw_h__
#define __asm_mac_cw_h__ 1

#if defined(__CWARM__)
#define ASM_PREFIX(x)	x
#else
#define ASM_PREFIX(x) 	_##x 
#endif

#define ASM_EQUATE(label,value) label  .equ  value
#define ASM_EXTERN(label)       .extern ASM_PREFIX(label)
#define ASM_ALIGN(value)        .align value
#define ASM_PUBLIC(label)       .global ASM_PREFIX(label) 
#define ASM_CONST16(value)      .word value 
#define ASM_CONST32(value)      .long value
#define ASM_LABEL(label)        label:
#define ASM_LABEL_CONST32(label,value) label .long value
#define ASM_CODE_SECTION(name)  .section name,16,x
#define ASM_DATA_SECTION(name)  TBD /* .section name,16,r */
#define ASM_END                 .end


#define ASM_PUBLIC_BEGIN(name) 
#define ASM_PUBLIC_FUNC(name)     
#define ASM_PUBLIC_END(name)

#endif /* __asm_mac_cw_h__ */
