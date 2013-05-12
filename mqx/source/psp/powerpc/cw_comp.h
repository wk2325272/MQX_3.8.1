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
* $Version : 3.8.8.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file defines the Codewarrior compiler specific macros for MQX
*   $Header:cw_comp.h, 14, 3/13/2006 18:44:26, Goutham D. R.$
*   $NoKeywords$
*
*END************************************************************************/
#ifndef __cw_comp_h__
#define __cw_comp_h__ 1

#ifdef __cplusplus
extern "C" {
#endif

#define _No_Console     1       /* Don't include Codewarrior console runtime */
//#define NEED_STRTOD  /* Removed due to HUGE_VAL not being valid */

#define _ASM_NOP()   asm(nop)

/*---------------------------------------------------------------------------*/
/*
**              STANDARD MACROS FOR INSTRUCTIONS
*/

#define _PSP_SPR_GET(lhs, reg) {register _t; asm{ mfspr _t, reg } lhs = _t; }
#define _PSP_SPR_SET(reg, val) {register _t = val; asm{ mtspr reg, _t } }
#define _PSP_MSR_GET(lhs)      {register _t; asm { mfmsr _t } lhs = _t; }
#define _PSP_MSR_SET(val)      {register _v = val; asm { mtmsr _v } }


#define _PSP_MFSPR(reg)        _psp_get_spr(reg)
#define _PSP_MTSPR(reg,val)    _psp_set_spr(reg,val)

#if defined(PSP_E500_CORE)
#define _PSP_EIEIO()           __eieio()
#define _PSP_SYNC()            __sync()
#define _PSP_ISYNC()           __isync()
#elif defined(PSP_E200_CORE)
#define _PSP_EIEIO()           __eieio()
#define _PSP_SYNC()            asm("  msync")
#define _PSP_ISYNC()           __isync()
#else
#define _PSP_EIEIO()           __eieio()
#define _PSP_SYNC()            __sync()
#define _PSP_ISYNC()           __isync()
#endif

#define _PSP_TLBSYNC()         asm("  tlbsync")
#define _PSP_CLEAR_ALL_TLBS()  asm("  tlbia")
#define _PSP_FLUSH_LINE(x)     __dcbf(x,0)
#define _PSP_SET_SR(x)         _psp_set_sr(x)
#define _PSP_GET_SR()          _psp_get_sr()
#define _PSP_GET_SEG_REG(x)    _psp_get_seg_reg(x)
#define _PSP_MTSRIN(reg,val)   _psp_mtsrin(reg,val)

#define _PSP_ICACHE_INVALIDATE_LINE(x) \
        {register pointer _v = x; asm { icbi  0,_v } }

#define _PSP_GET_DSISR()       _psp_get_dsisr()
#define _PSP_GET_SRR1()        _psp_get_srr1()
#define _PSP_GET_DAR()         _psp_get_dar()

// Compiler must be told to save the LR by calling another function
// otherwise it's free to leave the return address in %LR and never
// store it in the stack.
#define _PSP_GET_CALLING_PC()           _ppc_saved_lr()

/* Compatibility macros */
#define _PSP_GET_SEGREG_IN(reg)       _PSP_GET_SEG_REG(reg)
#define _PSP_SET_SEGREG_IN(val,reg)   _PSP_MTSRIN(val,reg)
#define _PSP_IO_EIEIO()               _PSP_EIEIO()
#define _PSP_IO_SYNC()                _PSP_SYNC()
#define _PSP_IO_ISYNC()               _PSP_ISYNC()
#define _PSP_IO_TLBSYNC()             _PSP_TLBSYNC()

/*---------------------------------------------------------------------------*/
/*
**                       KERNEL DATA
*/

#if MQX_ENABLE_USER_MODE
#error not implemented yet

#else

#define KERNEL_ACCESS
#define USER_RW_ACCESS
#define USER_RO_ACCESS
#define USER_NO_ACCESS

#endif //MQX_ENABLE_USER_MODE

#if defined(PSP_PPC405)
#define _GET_KERNEL_DATA(x) x = (KERNEL_DATA_STRUCT_PTR)_PSP_MFSPR(PPC405_SPRG7)
#define _SET_KERNEL_DATA(x) \
   _mqx_kernel_data = (struct kernel_data_struct _PTR_)(x); \
   _PSP_MTSPR(PPC405_SPRG7,(int)x)
#elif defined(PSP_PPC440)
#define _GET_KERNEL_DATA(x) x = (KERNEL_DATA_STRUCT_PTR)_PSP_MFSPR(PPC440_SPRG7R)
#define _SET_KERNEL_DATA(x) \
   _mqx_kernel_data = (struct kernel_data_struct _PTR_)(x); \
   _PSP_MTSPR(PPC440_SPRG7W,(int)x)
#elif defined(PSP_E500_CORE)
#define _GET_KERNEL_DATA(x) {register _t; asm{ mfspr _t, E500CORE_SPRG7R } x = (KERNEL_DATA_STRUCT_PTR)_t; }
#define _SET_KERNEL_DATA(x) \
   _mqx_kernel_data = (struct kernel_data_struct _PTR_)(x); \
   _PSP_SPR_SET(E500CORE_SPRG7W,(int)x)
#elif defined(PSP_E200_CORE)
#define _GET_KERNEL_DATA(x) {register _t; asm{ mfspr _t, E200CORE_SPRG7R } x = (KERNEL_DATA_STRUCT_PTR)_t; }
#define _SET_KERNEL_DATA(x) \
   _mqx_kernel_data = (struct kernel_data_struct _PTR_)(x); \
   _PSP_SPR_SET(E200CORE_SPRG7W,(int)x)
#else
#define _GET_KERNEL_DATA(x) x = (KERNEL_DATA_STRUCT_PTR)_mqx_kernel_data

#define _SET_KERNEL_DATA(x) \
   _mqx_kernel_data = (struct kernel_data_struct _PTR_)(x)
#endif


/*---------------------------------------------------------------------------*/
/*
**                       INTERRUPT MANAGEMENT
*/

#if defined(PSP_MPC860) || defined(PSP_MPC850) || defined(PSP_MPC855) || \
    defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC505) || \
    defined(PSP_MPC555) || defined(PSP_MPC566) || defined(PSP_MPC866) || \
    defined(PSP_MPC875)
#define __PSP_SET_DISABLE_SR() asm("  mtspr 81,r0");
#elif defined(PSP_MPC401) || defined(PSP_MPC403) || defined(PSP_PPC405) || \
      defined(PSP_480)
#define __PSP_SET_ENABLE_SR() asm(" wrteei 1");
#else
#define __PSP_SET_DISABLE_SR() _psp_set_disable_sr()
#endif
#define _PSP_SET_DISABLE_SR(x) __PSP_SET_DISABLE_SR()


#if defined(PSP_MPC860) || defined(PSP_MPC850) || defined(PSP_MPC855) || \
    defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC505) || \
    defined(PSP_MPC555) || defined(PSP_MPC566) || defined(PSP_MPC866) || \
    defined(PSP_MPC875)
#define __PSP_SET_ENABLE_SR() asm("  mtspr 80,r0");
#elif defined(PSP_MPC401) || defined(PSP_MPC403) || defined(PSP_PPC405) || \
      defined(PSP_480)
#define __PSP_SET_ENABLE_SR() asm(" wrteei 0");
#else
#define __PSP_SET_ENABLE_SR() _psp_set_enable_sr()
#endif
#define _PSP_SET_ENABLE_SR(x) if (!(x)) __PSP_SET_ENABLE_SR();



#define _PSP_DISABLE_EXTERNAL_INTS() __PSP_SET_DISABLE_SR()

#define _PSP_ENABLE_EXTERNAL_INTS() {\
   PSP_INTERRUPT_FRAME_STRUCT_PTR exception_frame_ptr;\
   KERNEL_DATA_STRUCT_PTR         kernel_data;\
   _GET_KERNEL_DATA(kernel_data);\
   exception_frame_ptr = (pointer)\
      ((uchar_ptr)kernel_data->INTERRUPT_CONTEXT_PTR -\
      FIELD_OFFSET(PSP_INTERRUPT_FRAME_STRUCT,INTERRUPT_CONTEXT));\
   exception_frame_ptr->INTERRUPT_CONTEXT.ENABLE_SR = 0;\
   __PSP_SET_ENABLE_SR();\
}


/*---------------------------------------------------------------------------*/
/*
**                            REGISTER MANIPULATION
*/

#define _PSP_TLBIE(x)         _psp_tlbie(x)
#define _PSP_GET_R2()         _psp_get_r2()
#define _PSP_GET_R13()        _psp_get_r13()
#define _PSP_GET_SP()         _psp_get_sp()
#define _PSP_SET_SP(x)        _psp_set_sp(x)
#define _PSP_GOTO(x)          _psp_goto(x)

/*--------------------------------------------------------------------------*/
/*
**                         SPECIAL INSTRUCTIONS
*/
#define _PSP_LOAD_LONG_SWAPPED(addr)       __lwbrx((pointer)(addr),0)
#define _PSP_STORE_LONG_SWAPPED(addr,val)  __stwbrx(val,(pointer)(addr),0)
#define _PSP_LOAD_HALF_SWAPPED(addr)       __lhbrx((pointer)(addr),0)
#define _PSP_STORE_HALF_SWAPPED(addr,val)  __sthbrx(val,(pointer)(addr),0)



/*--------------------------------------------------------------------------*/
/*
**                         PROCESSOR SPECIFIC MACROS
*/


/*--------------------------------------------------------------------------*/

#if defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC850) || \
    defined(PSP_MPC855) || defined(PSP_MPC860) || defined(PSP_MPC866) || \
    defined(PSP_MPC875)


#define _PSP_GET_IMMR() (pointer)((_psp_read_immr()) & 0xffff0000)

#define _PSP_GET_PARTNUM() (uint_32)((_psp_read_immr()) & 0xffff)

#endif


/*--------------------------------------------------------------------------*/

#if defined(PSP_MPC555) || defined(PSP_MPC566)

#define _PSP_GET_IMMR() (pointer)((_psp_read_immr() & 0xE) << 21)

#endif

/*--------------------------------------------------------------------------*/

#if defined(PSP_PPC603)  || defined(PSP_PPC740)  || defined(PSP_PPC750) || \
    defined(PSP_MPC8240) || defined(PSP_G2_CORE) || defined(PSP_PPC7400) || \
    defined(PSP_E300_CORE)

#define _PPC7400_VEC_DSSALL()              asm(" dssall");

#define _PSP_CLEAR_TLB_ENTRIES() \
{ \
   register loc = 0x00000000; \
   register top = 0x00080000; \
   while (loc != top) { \
      _PSP_TLBIE((pointer)loc); \
      _PSP_TLBSYNC(); \
      _PSP_SYNC(); \
      _PSP_ISYNC(); \
      loc += 0x1000; \
   } \
}

#define _PSP_CLEAR_ALL_SRS()  \
{ \
   register val = 0; \
   register reg = 0; \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_MTSRIN(reg++,val); \
   _PSP_ISYNC(); \
}

/* Set segment register indirect */
#undef  _PSP_SET_SEGREG_IN
#define _PSP_SET_SEGREG_IN(reg_val, index) \
   _PSP_ISYNC();_PSP_MTSRIN(reg_val, index); _PSP_ISYNC();

#endif


/*--------------------------------------------------------------------------*/

#if defined(PSP_PPC7400)
#define _PSP_VEC_DSSALL()  asm(" dssall");
#endif

/*--------------------------------------------------------------------------*/

#if defined(PSP_PPC403)

#define _PSP_GET_EXISR() _psp_get_exisr()
#define _PSP_GET_PIT()   _psp_get_pit()
#define _PSP_SET_PIT(x)  _psp_set_pit(x)


_Asm uint_32 _PSP_GET_TCR(void)
{
   mftcr r3
}

_Asm void _PSP_SET_TCR(uint_32 new_val)
{
%reg new_val

   mttcr new_val
}

_Asm void _PSP_MASK_TSR(uint_32 new_val)
{
%reg new_val
   mttsr new_val
}

_Asm uint_32 _PSP_GET_TSR(void)
{
   mftsr r3
}

/* enables external interrupt and timers */
_Asm void _PSP_EXT_INT_ENABLE(void)
{
   wrteei 1
}


_Asm void _PSP_CRIT_INT_ENABLE(void)
{
   mfmsr r5
   oris   r6, r5, 0x0002
   mtmsr r6
}
#endif

/*--------------------------------------------------------------------------*/

#if defined(PSP_PPC405)

#define _PSP_SET_TLBHI_TLBLO(tlbhi, tlblo) \
   _TLBWELO(tlblo); _TLBWEHI(tlbhi)

#define _PSP_GET_PIT()         _PSP_MFSPR(0x3DB)
#define _PSP_SET_PIT(new_val)  _PSP_SPR_SET(0x3DB, new_val)

#define _PSP_GET_TCR()         _PSP_MFSPR(0x3DA)
#define _PSP_SET_TCR(new_val)  _PSP_SPR_SET(0x3DA, new_val)

#define _PSP_GET_TSR()         _PSP_MFSPR(0x3D8)
#define _PSP_SET_TSR(new_val)  _PSP_SPR_SET(0x3D8, new_val)

#define _PSP_SET_PID(new_val)  _PSP_SPR_SET(0x3B1, new_val)

#define _PSP_SET_ZPR(new_val)  _PSP_SPR_SET(0x3B0, new_val); _PSP_ISYNC()


#define _PSP_GET_CPC0_CR0()    _MFDCR(0xB1)
#define _PSP_SET_CPC0_CR0(val) _MTDCR(0xB1, val)

#define _PSP_GET_CPC0_CR1()    _MFDCR(0xB2)
#define _PSP_SET_CPC0_CR1(val) _MTDCR(0xB2, val)

#define _PSP_GET_CPC0_PLLMR()  _MFDCR(0xB0)

#define _PSP_GET_UIC0_SR()     _MFDCR(0xC0)
#define _PSP_SET_UIC0_SR(val)  _MTDCR(0xC0, val)

#define _PSP_GET_UIC0_ER()     _MFDCR(0xC2)
#define _PSP_SET_UIC0_ER(val)  _MTDCR(0xC2, val)

#define _PSP_GET_UIC0_CR()     _MFDCR(0xC3)
#define _PSP_SET_UIC0_CR(val)  _MTDCR(0xC3, val)

#define _PSP_GET_UIC0_PR()     _MFDCR(0xC4)
#define _PSP_SET_UIC0_PR(val)  _MTDCR(0xC4, val)

#define _PSP_GET_UIC0_TR()     _MFDCR(0xC5)
#define _PSP_SET_UIC0_TR(val)  _MTDCR(0xC5, val)

#define _PSP_GET_UIC0_MSR()    _MFDCR(0xC6)

#define _PSP_GET_UIC0_VR()     _MFDCR(0xC7)

#define _PSP_GET_UIC0_VCR()    _MFDCR(0xC8)
#define _PSP_SET_UIC0_VCR(val) _MTDCR(0xC8, val)

#define _PSP_GET_MAL0_CFG()        _MFDCR(0x180)
#define _PSP_SET_MAL0_CFG(new_val) _MTDCR(0x180, new_val)

#define _PSP_GET_MAL0_ESR()        _MFDCR(0x181)
#define _PSP_SET_MAL0_ESR(new_val) _MTDCR(0x181, new_val)

#define _PSP_GET_MAL0_IER()        _MFDCR(0x182)
#define _PSP_SET_MAL0_IER(new_val) _MTDCR(0x182, new_val)

#define _PSP_GET_MAL0_TXCASR()        _MFDCR(0x184)
#define _PSP_SET_MAL0_TXCASR(new_val) _MTDCR(0x184, new_val)

#define _PSP_GET_MAL0_TXCARR()        _MFDCR(0x185)
#define _PSP_SET_MAL0_TXCARR(new_val) _MTDCR(0x185, new_val)

#define _PSP_GET_MAL0_TXEOBISR()        _MFDCR(0x186)
#define _PSP_SET_MAL0_TXEOBISR(new_val) _MTDCR(0x186, new_val)

#define _PSP_GET_MAL0_TXDEIR()          _MFDCR(0x187)
#define _PSP_SET_MAL0_TXDEIR(new_val)   _MTDCR(0x187, new_val)

#define _PSP_GET_MAL0_RXCASR()          _MFDCR(0x190)
#define _PSP_SET_MAL0_RXCASR(new_val)   _MTDCR(0x190, new_val)

#define _PSP_GET_MAL0_RXCARR()          _MFDCR(0x191)
#define _PSP_SET_MAL0_RXCARR(new_val)   _MTDCR(0x191, new_val)

#define _PSP_GET_MAL0_RXEOBISR()        _MFDCR(0x192)
#define _PSP_SET_MAL0_RXEOBISR(new_val) _MTDCR(0x192, new_val)

#define _PSP_GET_MAL0_RXDEIR()          _MFDCR(0x193)
#define _PSP_SET_MAL0_RXDEIR(new_val)   _MTDCR(0x193, new_val)

#define _PSP_GET_MAL0_TXCTP0R()         _MFDCR(0x1A0)
#define _PSP_SET_MAL0_TXCTP0R(new_val)  _MTDCR(0x1A0, new_val)

#define _PSP_GET_MAL0_TXCTP1R()         _MFDCR(0x1A1)
#define _PSP_SET_MAL0_TXCTP1R(new_val)  _MTDCR(0x1A1, new_val)

#define _PSP_GET_MAL0_RXCTP0R()         _MFDCR(0x1C0)
#define _PSP_SET_MAL0_RXCTP0R(new_val)  _MTDCR(0x1C0, new_val)

#define _PSP_GET_MAL0_RCBS0()           _MFDCR(0x1E0)
#define _PSP_SET_MAL0_RCBS0(new_val)    _MTDCR(0x1E0, new_val)

#endif

#ifdef PSP_MPC5200
#define MPC5200_SET_MBAR(x)     __psp_set_mbar(x)
#define MPC5200_GET_MBAR()      __psp_get_mbar()
#endif


#define _PSP_FIND_FIRST_BIT(value)      __cntlzw(value)

/*--------------------------------------------------------------------------*/

extern uint_32   __psp_get_mbar(void);
extern uint_32   __psp_set_mbar(uint_32);
extern uint_32   __psp_get_sr(void);
extern uint_32   __psp_set_sr(uint_32);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
