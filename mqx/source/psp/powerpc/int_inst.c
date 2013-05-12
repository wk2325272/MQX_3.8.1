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
* $FileName: int_inst.c$
* $Version : 3.5.3.0$
* $Date    : Feb-24-2010$
*
* Comments:
*
*   This file contains the function for initializing the handling of 
*   interrupts.
*
*END************************************************************************/

#include "mqx_inc.h"

// The compiler can inline this as easy as you can when not -g (debug)
static void _psp_int_install_vector
   (
      uint_32_ptr loc,
      _mqx_int    n
   )
{ /* Body */
   static const uint_32 instr[8] = {
      0x7C1043A6, // mtsprg0 %r0
      0x7C0902A6, // mfctr   %r0
      0x7C1143A6, // mtsprg1 %r0
      0x3C000000, // addis   %r0,%r0,_int_kernel_isr@h
      0x60000000, // ori     %r0,%r0,_int_kernel_isr@l
      0x7C0903A6, // mtctr   %r0
      0x38000000, // li      %r0,int#
      0x4E800420  // bctr
   };

   loc[0] = instr[0];
   loc[1] = instr[1];
   loc[2] = instr[2];
   loc[3] = instr[3] | (((uint_32)_int_kernel_isr >> 16) & 0xFFFF);
   loc[4] = instr[4] | ( (uint_32)_int_kernel_isr        & 0xFFFF);
   loc[5] = instr[5];
   loc[6] = instr[6] | n;
   loc[7] = instr[7];

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_int_install
* Returned Value   : void
* Comments         :
*    This function initializes the hardware interrupt table.
*
*END*----------------------------------------------------------------------*/

#ifdef PSP_BOOKE

_mqx_uint _psp_int_install
   (
      void
   )
{ /* Body */
   _mqx_int i;

   for (i = PSP_EXCPT_CRITICAL; i < PSP_MAXIMUM_INTERRUPT_VECTORS; i++) {
      switch (i) {
#ifdef PSP_SKIP_INT_INSTALL_FOR_VECTORS
      PSP_SKIP_INT_INSTALL_FOR_VECTORS
         break;
#endif
      default:
         _psp_int_install_vector((uint_32_ptr)_int_get_kernel_isr(i),i);
         break;
      } /* Endswitch */
   } /* Endfor */

   _PSP_SYNC();
   _PSP_ISYNC();

   return MQX_OK;

} /* Endbody */


#else   // !PSP_BOOKE (Original exception model)


_mqx_uint _psp_int_install
   (
      void
   )
{ /* Body */
   uint_32_ptr  int_location;
   _mqx_int     i;
   uchar_ptr    vector_table_ptr;
#if defined(PSP_PPC7400)
   boolean      altivec_unavail_installed = FALSE;
#endif

   /* Initialize the hardware interrupt vector table */
   vector_table_ptr = (pointer)_int_get_vector_table();

   for (i = 1; i < PSP_MAXIMUM_INTERRUPT_VECTORS; i++) {
      int_location = (uint_32_ptr)((i * 0x100) + vector_table_ptr);
#if defined(PSP_PPC7400)
      if ((i == 0x10) && !altivec_unavail_installed) {
         altivec_unavail_installed = TRUE;
         int_location = (uint_32_ptr)((uint_32)int_location - 0xE0);
         i--;
      } /* Endif */
#endif
#if defined(PSP_G2_CORE) || defined(PSP_MPC8240) || \
    defined(PSP_PPC740)  || defined(PSP_PPC750)  || \
    defined(PSP_MPC5200) || defined(PSP_E300_CORE)

    if ((i == EXCPT_TLB_INST_MISS) || (i == EXCPT_TLB_DATA_MISS_ON_LOAD) ||
       (i == EXCPT_TLB_DATA_MISS_ON_STORE))
    {
       continue; /* Do not install, installed by linker */
    }/* Endif */
#endif
/* Start CR 1033 */
#if defined(PSP_PPC405)
    if ((i == PSP_EXCPT_DTLB_MISS) || (i == PSP_EXCPT_ITLB_MISS)) {
       continue; /* Do not install, installed by linker */
    }/* Endif */
#endif
/* End CR 1033 */

#if defined(PSP_MPC860) || defined(PSP_MPC866)
    if ((i == EXCPT_INSTRUCTION_TLB_MISS) || (i == EXCPT_DATA_TLB_MISS)) {
       continue; /* Do not install, installed by MMU library */
    }/* Endif */
#endif

      _psp_int_install_vector(int_location,i);
   } /* Endfor */


#if defined(PSP_PPC401) || defined(PSP_PPC403) || defined(PSP_PPC405) || \
    defined(PSP_IOP480)
/*
** Code used to jump to _int_kernel_isr - hardcoded below
1000     b     0x1800   ; PIT
1010     b     0x1900   ; FIT
1020     b     0x1A00   ; WATCHDOG
*/
   int_location =  (uint_32_ptr)(PSP_EXCPT_PIT_VECTOR + vector_table_ptr);
   int_location[0x00] = (uint_32)0x48000800;

   int_location =  (uint_32_ptr)(PSP_EXCPT_FIT_VECTOR + vector_table_ptr);
   int_location[0x00] = (uint_32)0x480008F0;

   int_location =  (uint_32_ptr)(PSP_EXCPT_WATCHDOG_VECTOR + vector_table_ptr);
   int_location[0x00] = (uint_32)0x480009E0;

#endif

   _PSP_SYNC();
   _PSP_ISYNC();

   return MQX_OK;

} /* Endbody */


#endif /* PSP_BOOKE */

/* EOF */
