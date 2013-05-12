/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: cw_comp.h$
* $Version : 3.8.11.4$
* $Date    : Mar-12-2012$
*
* Comments:
*
*   This file defines the CodeWarrior compiler specific macros for MQX
*
*END************************************************************************/
#ifndef __cw_comp_h__
#define __cw_comp_h__ 1


#ifdef __cplusplus
extern "C" {
#endif

extern asm void __set_BASEPRI(uint_32 basePri);

#define _PSP_SET_ENABLE_SR(x)   __set_BASEPRI(x)
  
#define _ASM_NOP()   asm(nop)
#define _ASM_STOP(x) asm(stop #(x))
#define _ASM_WFI()   asm(wfi)
#define _ASM_SLEEP   _ASM_WFI

#define _PSP_SET_DISABLE_SR(x)  _PSP_SET_ENABLE_SR(x)

#define _PSP_SYNC()


#if MQX_ENABLE_USER_MODE
#pragma  define_section kernel_data ".kernel_data" abs32 RW
#pragma  define_section rwuser ".rwuser" abs32 RW
#pragma  define_section rouser ".rouser" abs32 RW
#pragma  define_section nouser ".nouser" abs32 RW

#define KERNEL_ACCESS  __declspec(kernel_data)
#define USER_RW_ACCESS __declspec(rwuser)
#define USER_RO_ACCESS __declspec(rouser)
#define USER_NO_ACCESS __declspec(nouser)
#else

#define KERNEL_ACCESS
#define USER_RW_ACCESS
#define USER_RO_ACCESS
#define USER_NO_ACCESS

#endif //MQX_ENABLE_USER_MODE

#ifdef __cplusplus
}
#endif

#endif   /* __cw_comp_h__ */
