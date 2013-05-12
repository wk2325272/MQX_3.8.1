/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: cw_comp.h$
* $Version : 3.8.22.1$
* $Date    : Mar-12-2012$
*
* Comments:
*
*   This file defines the Codewarrior compiler specific macros for MQX
*
*END************************************************************************/
#ifndef __cw_comp_h__
#define __cw_comp_h__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define _No_Console     1       /* Don't include Codewarrior console runtime */
//#define NEED_STRTOD  /* Removed due to HUGE_VAL not being valid */

#if defined (__CODEWARRIOR__)
#define _PSP_SET_A6_SP_AND_GOTO(x,y,z) _psp_set_a6_sp_and_goto((uint_32)x, (uint_32)y, (uint_32)z)
#endif


#define _ASM_NOP()   asm(nop)
#define _ASM_STOP(x) asm(stop #(x))
#define _ASM_SLEEP() _ASM_STOP(0x2000)

/*---------------------------------------------------------------------------*/
/*
**              STANDARD MACROS FOR INSTRUCTIONS
*/

#define _PSP_SET_A6_SP_AND_GOTO(x,y,z)  _psp_set_a6_sp_and_goto((uint_32)x, (uint_32)y, (uint_32)z)
#define _PSP_SET_SR(x)                  __psp_set_sr((uint_16)(x))
#define _PSP_SET_DISABLE_SR             _PSP_SET_SR
#define _PSP_SET_ENABLE_SR              _PSP_SET_SR
#define _PSP_GET_SR(x)                  x = __psp_get_sr()
#define _PSP_SET_CPUCR                  _psp_set_cpucr()

#if MQX_ENABLE_USER_MODE
#error not implemented yet
/*#define KERNEL_ACCESS  _Pragma("location=\".kernel_data\"")
#define USER_RW_ACCESS _Pragma("location=\".rwuser\"")
#define USER_RO_ACCESS _Pragma("location=\".rouser\"")
#define USER_NO_ACCESS _Pragma("location=\".nouser\"")*/

#else

#define KERNEL_ACCESS
#define USER_RW_ACCESS
#define USER_RO_ACCESS
#define USER_NO_ACCESS

#endif //MQX_ENABLE_USER_MODE

/* workaround for CodeWarrior 6.0 Build 8 bug */
#if (__MWERKS__ == 0x4010)
   inline asm void _PSP_SYNC(void)
   {
      nop
   }
#else
   #define _PSP_SYNC()                     _ASM_NOP()
#endif

/* This macro obtains the address of the kernel data structure */
#define _GET_KERNEL_DATA(x) x = _mqx_kernel_data
#define _SET_KERNEL_DATA(x) \
   _mqx_kernel_data = (struct kernel_data_struct _PTR_)(x)

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
    __declspec(standard_abi)
#endif
inline asm uint_32 __PSP_GET_A5(void)
{
   move.l  a5,d0
}
#define _PSP_GET_A5(x)      x = __PSP_GET_A5();

#if   __REGABI__
    __declspec(register_abi)
#elif __STDABI__
    __declspec(standard_abi)
#endif
inline asm pointer __PSP_GET_A6(void)
{
   move.l  a6,d0
}
#define _PSP_GET_A6(x)      x = __PSP_GET_A6();


#define _PSP_GET_VBR(x)     x = (_mqx_max_type)_psp_saved_vbr;
#define _PSP_SET_VBR(x)     _psp_saved_vbr = _psp_set_vbr((uint_32)(x));

#define _PSP_GET_CACR()     _psp_saved_cacr
#define _PSP_SET_CACR(x)    \
   { \
      uint_32 _xx = (uint_32)x; \
      _psp_saved_cacr = _xx & ~(CACR_AUTO_CLEAR_BITS); \
      _psp_set_cacr(_xx); \
   }



#define _PSP_SET_ACR(n, x) \
   { \
      uint_32 _xx = (uint_32)x; \
      if (n == 0) { \
         _psp_saved_acr0 = _xx; \
      } else if (n == 1) { \
         _psp_saved_acr1 = _xx; \
      } else if (n == 2) { \
         _psp_saved_acr2 = _xx; \
      } else { \
         _psp_saved_acr3 = _xx; \
      } \
      _psp_set_acr(n, _xx); \
   }

#define _PSP_GET_ACR(n) \
    ((n) == 0) ? _psp_saved_acr0 : \
   (((n) == 1) ? _psp_saved_acr1 : \
   (((n) == 2) ? _psp_saved_acr2 : _psp_saved_acr3))

#if (PSP_MQX_CPU_IS_MCF5222X || PSP_MQX_CPU_IS_MCF5223X || PSP_MQX_CPU_IS_MCF5225X || PSP_MQX_CPU_IS_MCF5282)
#define _PSP_SET_IPSBAR(x)  _PSP_SET_MBAR(x)
#define _PSP_GET_IPSBAR()   _PSP_GET_MBAR()
#endif

#if (MQX_CPU == PSP_CPU_MCF5282)
#define _PSP_SET_IPSBAR2(x) _PSP_SET_MBAR2(x)
#define _PSP_GET_IPSBAR2()  _PSP_GET_MBAR2()
#endif

#if (PSP_MQX_CPU_IS_MCF5227X || PSP_MQX_CPU_IS_MCF5301X || PSP_MQX_CPU_IS_MCF5445X || PSP_MQX_CPU_IS_MCF5208 || PSP_MQX_CPU_IS_MCF520X)
#define _PSP_SET_IPSBAR(x) ;
#define _PSP_GET_IPSBAR()  (pointer)0xFC000000
#endif

#if (PSP_MQX_CPU_IS_MCF5441X || PSP_MQX_CPU_IS_MCF532X)
#define _PSP_SET_IPSBAR(x) ;
#define _PSP_GET_IPSBAR()  (pointer)0xEC000000
#endif

#if (PSP_MQX_CPU_IS_MCF5222X || PSP_MQX_CPU_IS_MCF5223X || PSP_MQX_CPU_IS_MCF5225X || MQX_CPU == PSP_CPU_MCF5282)
#define _PSP_SET_MBAR(x)     \
   {                          \
      uint_32 y;              \
      y = (x) & 0xFFFFF000;   \
      *(uint_32_ptr)_psp_saved_mbar = y | 1; \
      _psp_saved_mbar = y;    \
   }
#else
#define _PSP_SET_MBAR(x)     \
   {                          \
      uint_32 y;              \
      y = (x) & 0xFFFFF000;   \
      _psp_saved_mbar = y;    \
      y |= 0x1;               \
      _psp_set_mbar(y);       \
   }
#endif

#define _PSP_GET_MBAR()   (pointer)_psp_saved_mbar


#define _PSP_SET_MBAR2(x)      \
   {                          \
      uint_32 y;              \
      y = (x) & 0xFFFFF000;    \
      _psp_saved_mbar2 = y;    \
      y |= 0x1;               \
      _psp_set_mbar2(y);       \
   }

#define _PSP_GET_MBAR2()  (pointer)_psp_saved_mbar2


extern void    _psp_set_a6_sp_and_goto(uint_32, uint_32, uint_32);
extern uint_32 _psp_set_vbr(uint_32);
extern void    _psp_set_cacr(uint_32);
extern void    _psp_set_cpucr(uint_32);
extern void    __psp_set_sr(uint_32);
extern uint_32 __psp_get_sr(void);

#ifdef __cplusplus
}
#endif

#endif
/*#EOF*/
