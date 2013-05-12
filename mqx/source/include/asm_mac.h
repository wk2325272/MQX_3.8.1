/**HEADER********************************************************************
* 
* Copyright (c) 2008-2011 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2008-2011 Embedded Access Inc.;
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
* $FileName: asm_mac.h$
* $Version : 3.8.11.0$
* $Date    : Sep-19-2011$
*
* Comments:
*   This file contains macros used by the assembler and resolves
*   differences between different assemblers.  Specifically
*   the IAR, Codewarrior and Keil assemblers.
*
*END************************************************************************/


#ifndef __asm_mac_h__
#define __asm_mac_h__ 1

#if defined(__CODEWARRIOR__)
#include "asm_mac_cw.h"

#elif defined(__IAR_SYSTEMS_ICC__)  || defined (__IASMARM__)  
#include "asm_mac_iar.h"

#elif defined(__ARMCC_VERSION)
#include "asm_mac_keil.h"

#else

// #warning "COMPILER/ASSEMBLER TYPE NOT DEFINED, ASSUMING CODEWARRIOR"
#include "asm_mac_cw.h"
#endif

/* most of compilers do not need to define anything special for public 
  functions */

#ifndef ASM_PUBLIC_BEGIN 
#define ASM_PUBLIC_BEGIN(name) 
#endif

#ifndef ASM_PUBLIC_FUNC 
#define ASM_PUBLIC_FUNC(name)   
#endif

#ifndef ASM_PUBLIC_END 
#define ASM_PUBLIC_END(name)
#endif

#endif /* __asm_mac_h__ */
