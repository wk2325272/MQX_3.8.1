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
* $FileName: iar_comp.h$
* $Version : 3.8.B.4$
* $Date    : May-3-2012$
*
* Comments:
*
*   This file defines the IAR compiler specific macros for MQX
*
*END************************************************************************/
#ifndef __iar_comp_h__
#define __iar_comp_h__ 1

#include <intrinsics.h>

#ifdef __cplusplus
extern "C" {
#endif

#define _ASM_NOP()    asm("nop")
#define _ASM_STOP(x)  asm("stop #(" #x ")")
#define _ASM_WFI()    asm("wfi")
#define _ASM_SLEEP    _ASM_WFI

#define _PSP_SET_ENABLE_SR(x)   __set_BASEPRI(x)
 
#define _PSP_SET_DISABLE_SR(x)  _PSP_SET_ENABLE_SR(x)

#define _PSP_SYNC()

#if MQX_ENABLE_USER_MODE
#define KERNEL_ACCESS  _Pragma("location=\".kernel_data\"")
#define USER_RW_ACCESS _Pragma("location=\".rwuser\"")
#define USER_RO_ACCESS _Pragma("location=\".rouser\"")
#define USER_NO_ACCESS _Pragma("location=\".nouser\"")

#else /* MQX_ENABLE_USER_MODE */

#define KERNEL_ACCESS
#define USER_RW_ACCESS
#define USER_RO_ACCESS
#define USER_NO_ACCESS

#endif /* MQX_ENABLE_USER_MODE */

#ifdef __cplusplus
}
#endif

#endif
