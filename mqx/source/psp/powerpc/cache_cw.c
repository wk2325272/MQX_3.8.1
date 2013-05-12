/**HEADER********************************************************************
* 
* Copyright (c) 2008-2009 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2010 Embedded Access Inc.;
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
* $FileName: cache_cw.c$
* $Version : 3.8.9.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file contains functions for use with the PPC caches.
*
*END************************************************************************/

#include "mqx_inc.h"

#ifndef PSP_DCACHE_SIZE
#define PSP_DCACHE_SIZE (16*1024)
#endif

/* Start CR 1032 */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_enable
* Returned Value   : none
* Comments         : 
*   This function enables the data cache
*
*END*----------------------------------------------------------------------*/

void _dcache_enable
   (
      /* [IN] mask that defines cachable areas on 403,401 */
      /* [IN] dummy input variable on other PPCs*/
      uint_32 dcache_en_mask
   )
{ /* Body */
  
#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480) || \
    defined(PSP_PPC405)

   _PSP_SYNC();
   _PSP_ISYNC();
   _PSP_SPR_SET(1018, dcache_en_mask);  // mtspr dccr,dcache_en_mask
   _PSP_SYNC();
   _PSP_ISYNC();

#elif defined(PSP_PPC603) || defined(PSP_MPC8240) || defined(PSP_G2_CORE) || \
      defined(PSP_E300_CORE)

   register uint_32 val,i,e;
   _PSP_SPR_GET(val,1008);
   i = val | 0x5400;
   e = i ^ 0x1400;
   i = i ^ 0x1000;
   _PSP_SYNC();
   _PSP_SPR_SET(1008,i);
   _PSP_SPR_SET(1008,e);
   _PSP_SYNC();
   
#elif defined(PSP_E500_CORE)
   register uint_32 i,e;
   /* Enable address boardcast enable for L1 & L2 caches */
   _PSP_SPR_GET(i,E500CORE_HID1);
   i |= E500CORE_HID1_ABE;   
   _PSP_SPR_SET(E500CORE_HID1,i);
   _PSP_SPR_GET(i,1010); /* L1CSR0 */
   i |= 0x03;
   e = i ^ 0x02;
   _PSP_SYNC();
   _PSP_ISYNC();
   _PSP_SPR_SET(1010,i);
   _PSP_SPR_SET(1010,e);
   _PSP_ISYNC();

#elif defined(PSP_E200_CORE)
   register uint_32 i;
   /* Cahce must be invalidated first and then enabled */
   _dcache_invalidate();
   _PSP_SPR_GET(i,E200CORE_L1CSR0);        /* L1CSR0 */
   /* enable copy-back mode, 
   push buffer disabled, error injection disabled on 5674,
   enable cache */
   i |= E200CORE_L1CSR0_CE | E200CORE_L1CSR0_CWM | E200CORE_L1CSR0_DPB;
   asm ("msync");
   _PSP_ISYNC();
   _PSP_SPR_SET(E200CORE_L1CSR0,i);      /* enable cache */
   _PSP_ISYNC();
   
   asm ("isync");

  
#elif defined(PSP_PPC740) || defined(PSP_PPC750) || defined(PSP_PPC7400)

   register uint_32 val,i,e;
   _PSP_SPR_GET(val,1008);
   i = val | 0x5480;
   e = i ^ 0x1400;
   i = i ^ 0x1000;
   _PSP_SYNC();
   _PSP_SPR_SET(1008,i);
   _PSP_SPR_SET(1008,e);
   _PSP_SYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

#elif defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC850) || \
      defined(PSP_MPC855) || defined(PSP_MPC860) || defined(PSP_MPC866) || \
      defined(PSP_MPC875)

   register uint_32 val;
   _PSP_SPR_GET(val,568);
   val |= 0x02000000;
   _PSP_SYNC();
   _PSP_SPR_SET(568,val);
   _PSP_SYNC();

#elif defined(PSP_PPC440)
   // Cache control registers have to be set up during boot process.
   // marks@6/25/03 The ibevb440 BSP has not yet done this since it relies
   //               on the IBM OS/Open rom monitor to initialize the board.

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_disable
* Returned Value   : none
* Comments         : 
*   This function disables the data cache (if possible)
*
*END*----------------------------------------------------------------------*/

void _dcache_disable
   (
      void
   )
{ /* Body */
   
#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480) || \
    defined(PSP_PPC405)

   _dcache_flush();
   _PSP_SPR_SET(1018,0);    // mtspr dccr,0
   _PSP_SYNC();

#elif defined(PSP_PPC603)  || defined(PSP_PPC740)  || defined(PSP_PPC750) || \
      defined(PSP_MPC8240) || defined(PSP_G2_CORE) || defined(PSP_PPC7400) || \
      defined(PSP_E300_CORE)

   register uint_32 val;
   _dcache_flush();
   _PSP_SPR_GET(val,1008);
   val &= ~0x4000;
   _PSP_SYNC();
   _PSP_SPR_SET(1008,val);
   _PSP_SYNC();

#elif defined(PSP_E500_CORE) || defined(PSP_E200_CORE)

   register uint_32 val;
   _dcache_flush();
   _PSP_SPR_GET(val,1010); /* L1CSR0 */
   val &= ~0x01;
#ifdef PSP_BOOKE    
   asm ("msync");
#else   
   _PSP_SYNC();
#endif   
   _PSP_ISYNC();
   _PSP_SPR_SET(1010,val);
   _PSP_ISYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

/* define PSP_MPC823 in macro */   
#elif defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC850) || \
      defined(PSP_MPC855) || defined(PSP_MPC860) || defined(PSP_MPC866) || \
      defined(PSP_MPC875)

   register uint_32 val;
   _dcache_flush();
   _PSP_SPR_GET(val,568);
   val |= 0x04000000;
   _PSP_SYNC();
   _PSP_SPR_SET(568,val);
   
#elif defined(PSP_PPC440)

   // Cache cannot be easily disabled.  You must modify the appropriate
   // TLB entries that allow cache to disallow it.  I'm not sure that's
   // a good idea.

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_flush
* Returned Value   : none
* Comments         : 
*   This function flushes the data cache
*
*END*----------------------------------------------------------------------*/

void _dcache_flush
   (
      void
   )
{ /* Body */
   
#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480)

   /* hardcoded branches are used, insert lines with care */
   asm(" li     r5, 0");          /* line 0 */
   asm(" mtcrf   0xFF, r5");      /* Set whole CR to zero */
   asm(" mfcdbcr   r3");          /* CDBCR -> R3*/
   asm(" clrrwi r4, r3, 1");      /* set Way A */
   asm("_dcache_flush_next_way:");
   asm(" ori    r4, r4, 0x10");   /* set CIS to tag */
   asm(" mtcdbcr   r4");          /* R4 -> CDBCR */
   asm("_dcache_flush_next_line:");
   asm(" dcread r4, 0, r5");      /* read cache TAG info*/
   asm(" andi.  r6, r4,0x10");    /* check the valid bit */ 
   asm(" beq    CR0, _dcache_flush_invalid");   /* skip line if invalid */
   asm(" clrrwi r4, r4, 9");      /* form EA that is flushed*/
   asm(" dcbf   r4, r5");         /* flush line */
   asm("_dcache_flush_invalid:");
   asm(" addi   r5, r5, 16");     /* set r5 for next line*/
   asm(" andi.  r6, r5, 0xFE00"); /* last flushed ?*/
   asm(" beq    CR0, _dcache_flush_next_line"); /* next line */
   asm(" bgt    CR1, _dcache_flush_end_flush"); /* end ? */
   asm(" mcrf   CR1, CR0");       /* set CR1[gt] */
   asm(" li     r5, 0");          /* line 0 */
   asm(" ori    r4, r3, 1");      /* set Way B*/
   asm(" b      _dcache_flush_next_way");       /* next line */
   asm("_dcache_flush_end_flush:");
   asm(" mtcdbcr   r3");          /* restore CDBCR */
   _PSP_SYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

#elif defined(PSP_PPC603) || defined(PSP_MPC8240) || defined(PSP_G2_CORE) || \
      defined(PSP_E300_CORE) || defined(PSP_E500_CORE)
   volatile uint_32 _PTR_ tmp_ptr;
   volatile uint_32       tmp;
   uint_32                i;
   
   /*
   ** We just stored our link register %LR on the top of the stack.
   ** When debugging with the cache enabled we want to force it to
   ** memory now.  Problems were found on the MPC8266 where the LR
   ** value restored was not the value written.
   */
   _PSP_SYNC();
   _PSP_ISYNC();
   asm(" dcbf   r0, r1");       /* Flush top of stack + 0x00 */
   asm(" li     r0, 32");
   asm(" dcbf   r1, r0");       /* Flush top of stack + 0x20 */
   asm(" li     r0, 64");
   asm(" dcbf   r1, r0");       /* Flush top of stack + 0x40 */
   asm(" li     r0, 96");
   asm(" dcbf   r1, r0");       /* Flush top of stack + 0x60 */
   _PSP_SYNC();

   /* In order to "Flush" the entire data cache, fake reads
   ** need to be executed for a select number of memory locations,
   ** then those locations need to be "flushed"
   ** there are 128 sets of 4 blocks each.
   ** the sets are addressed by bits A20 to A26
   */
   tmp_ptr = 0;

   i = PSP_DCACHE_SIZE / PSP_CACHE_LINE_SIZE + 1;
   while (--i) {
      tmp = *tmp_ptr;
      tmp_ptr = (pointer)((uchar_ptr)tmp_ptr + PSP_CACHE_LINE_SIZE);
   } /* Endwhile */
   tmp_ptr = 0;

   i = PSP_DCACHE_SIZE / PSP_CACHE_LINE_SIZE;
   while (--i) {
      _PSP_FLUSH_LINE((pointer)tmp_ptr);
      tmp_ptr = (pointer)((uchar_ptr)tmp_ptr + PSP_CACHE_LINE_SIZE);
   } /* Endwhile */
   _dcache_flush_line((pointer)tmp_ptr);

#elif defined(PSP_E200_CORE)
   register uint_32 i;
   uint_32  j;
   volatile uint_32 _PTR_ tmp_ptr;

   /* In order to "Flush" the entire data cache, we need to flush
   ** either by the way or by the set (line). 
   */
   tmp_ptr = 0;   
   asm ("msync");
   _PSP_ISYNC();
   _PSP_SPR_GET(i,E200CORE_L1FINV0); /* L1FINV0 */
   for (j = 0; j < PSP_DCACHE_NUM_LINES; j++) 
   {
      i = j << 5;       /* Specify the cache set to be selected */
      i |= 0x00000001;  /* Flush and valid without invalidation */
      _PSP_FLUSH_LINE((pointer)tmp_ptr);
      tmp_ptr = (pointer)((uchar_ptr)tmp_ptr + PSP_CACHE_LINE_SIZE);      
      _PSP_SPR_SET(E200CORE_L1FINV0,i);
   } /* Endfor */
   _dcache_flush_line((pointer)tmp_ptr);

#elif defined(PSP_PPC740) || defined(PSP_PPC750)
   volatile uint_32 _PTR_ tmp_ptr;
   volatile uint_32       tmp;
   uint_32                i;
   uint_32                l2_cache;
   uint_32                init;
   
   /* In order to "Flush" the entire data cache, fake reads
   ** need to be executed for a select number of memory locations,
   ** then those locations need to be "flushed"
   ** there are 128 sets of 8 blocks each.
   ** the sets are addressed by bits A20 to A26
   ** However if the L2 cached is enabled, then it can be of
   ** size 128K, 512K or 1024K.
   */
   tmp_ptr = 0;
   _PSP_SPR_GET(l2_cache, PPC750_L2CR);
   if (l2_cache & PPC750_L2_CACHE_L2E) {
      if ((l2_cache & PPC750_L2_CACHE_L2SIZ_MASK) == 
         PPC750_L2_CACHE_L2SIZ_256K) 
      {
         init = 8193;
      } else if ((l2_cache & PPC750_L2_CACHE_L2SIZ_MASK) == 
         PPC750_L2_CACHE_L2SIZ_512K) 
      {
         init = 16385;
      } else {
         init = 32769;
      }/* Endif */
   } else {
      init = 1025;
   }/* Endif */
   i = init;
   while (--i) {
      tmp = *tmp_ptr;
      tmp_ptr = (pointer)((uchar_ptr)tmp_ptr + 0x20);
   } /* Endwhile */
   tmp_ptr = 0;

   i = init - 1;
   while (--i) {
      _PSP_FLUSH_LINE((pointer)tmp_ptr);
      tmp_ptr = (pointer)((uchar_ptr)tmp_ptr + 0x20);
   } /* Endwhile */
   _dcache_flush_line((pointer)tmp_ptr);

#elif defined(PSP_PPC7400)

   /* Flush L1 and L2 cache */
   _ppc7400_flushL1();
   _ppc7400_flushL2();
   
#elif defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC850) || \
      defined(PSP_MPC855) || defined(PSP_MPC860) || defined(PSP_MPC866) || \
      defined(PSP_MPC875)

   uint_32 i,way0 = 0x0000, way1 = 0x1000, cmd;

   _PSP_SPR_GET(cmd,568);
   cmd |= 0x0E000000;
   for (i = 0; i < 256; i++) {
      _PSP_SPR_SET(569,way0);
      _PSP_SPR_SET(568,cmd);
      _PSP_SPR_SET(569,way1);
      _PSP_SPR_SET(568,cmd);
      way0 += 16;
      way1 += 16;
   } /* Endfor */
   _PSP_SYNC();

#elif defined(PSP_PPC405)
   register uchar_ptr p;
   uint_32   i;

   // I'm assuming KERNEL_DATA is a large enough block of memory
   // and that it can be aligned to the beginning of a 4 * dcache
   // sized memory block.
   _GET_KERNEL_DATA(p);
   p = (uchar_ptr)(((uint_32)p) & ~((PSP_DCACHE_SIZE*4)-1));

   _int_disable();
   for (i = 0; i < PSP_DCACHE_NUM_LINES*4; i++) {
      register uint_32 zero = 0;
      (void) *(volatile _mqx_int _PTR_)p; // Load from the address
      __dcbf((pointer)p, zero); // Data cache block flush
      asm {
     iccci p, zero
      }
      p += PSP_CACHE_LINE_SIZE;
   } /* Endfor */
   _PSP_SYNC();
   _PSP_ISYNC();
   _int_enable();

#elif defined(PSP_PPC440)
   uchar_ptr p;
   _mqx_int i, old_msr = _MFMSR();

   // I'm assuming KERNEL_DATA is a large enough block of memory
   // and that it can be aligned to the beginning of a dcache
   // sized memory block.
   _GET_KERNEL_DATA(p);
   p = (uchar_ptr)(((uint_32)p) & ~(PSP_DCACHE_SIZE-1));

   // Must be atomic
   _MTMSR(old_msr & ~(PPC440_MSR_EE|PPC440_MSR_CE|PPC440_MSR_ME));

   // Finish all data writes
   _PSP_SYNC();

   // There are 1024 lines in the dcache.
   // Twice as much data must be read in.
   for (i = 0; i < PPC440_DCACHE_NUM_LINES2X; i++) {
      (void) *(volatile _mqx_int _PTR_)p; // Load from the address
      _DCBF((pointer)p,0); // Data cache block flush
      _ICBI((pointer)p,0); // Instruction cache block invalidate
      p += PPC440_DCACHE_LINE_SIZE;
   } /* Endfor */

   _PSP_SYNC();
   _MTMSR(old_msr);

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_flush_line
* Comments         :
*   This function is called to push (flush and invalidate) a line
*   out of the data cache.
*
*END*------------------------------------------------------------------------*/

void _dcache_flush_line
   (
      /* [IN] the cache line to flush */
      register pointer line
   )
{ /* Body */   

#if defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */
#else
   _PSP_FLUSH_LINE(line);
#ifdef PSP_BOOKE   
   asm ("msync");
#else   
   _PSP_SYNC();
#endif
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_flush_mlines
* Returned Value   : none
* Comments         :
* 
*  This function is called to flush the data cache by performing cache
*  copybacks.  It must determine how many cache lines need to be copied
*  back and then perform the copybacks.
*
*END*-----------------------------------------------------------------------*/

void _dcache_flush_mlines
   (
      /* [IN] the address somewhere in the cache line */
      pointer     cache_entry,

      /* [IN] the number of bytes from the address requiring flushing */
      uint_32     entry_size
   )
{ /* Body */

#if defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */
#else
   uint_32     final_address;
   uint_32     cache_address;

   /*
   **  set cache_entry to the beginning of the cache line;  final_address
   **  indicates the last address which needs to be pushed.  Increment
   **  cache_entry and push the resulting line until final_address is passed.
   */
   if(cache_entry == NULL)
      return;
   
   cache_address = (uint_32)cache_entry;
   final_address = (cache_address + entry_size -1);
   cache_address = cache_address & PSP_CACHE_LINE_ALIGNMENT;

   while ( (cache_address <= final_address) ) {
      _PSP_FLUSH_LINE((pointer)cache_address);
      cache_address += PSP_CACHE_LINE_SIZE;
   } /* Endwhile */
#ifdef PSP_BOOKE   
   asm ("msync");
#else   
   _PSP_SYNC();
#endif   
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_invalidate
* Returned Value   : none
* Comments         : 
*   This function invalidates the entire data cache
*
*END*----------------------------------------------------------------------*/

void _dcache_invalidate
   (
      void
   )
{ /* Body */

#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480)

   asm(" li     r5, 0");           /* line 0 */
   asm("1:");
   asm(" dccci  0, r5");           /* invalidate class */
   asm(" addi   r5, r5, 16");      /* increment address */
   asm(" andis. r6, r5, 0xFFFE");  /* last flushed ?*/
   asm(" bne    CR0, 1b"); /* next line */
   _PSP_SYNC();

#elif defined(PSP_PPC405)
   _mqx_int line;
   register offset = 0;
   register uchar_ptr address = 0;
   for (line =0; line < PSP_DCACHE_NUM_LINES; line++) {
      /*D-cache has m_lines congruence classes */
      _DCCCI(address, offset);
      address += PSP_CACHE_LINE_SIZE; /*point to the next congruence class */
   } /* Endfor */
   _PSP_SYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

#elif defined(PSP_PPC603)  || defined(PSP_PPC740)  || defined(PSP_PPC750) ||  \
      defined(PSP_MPC8240) || defined(PSP_G2_CORE) || defined(PSP_PPC7400) || \
      defined(PSP_E300_CORE)

   asm("   mfspr r3,1008");
   asm("   ori   r4,r3,0x400");
   asm("   xori  r3,r4,0x400");
   _PSP_SYNC();
   asm("   mtspr 1008,r4");
   asm("   mtspr 1008,r3");
   _PSP_SYNC();

#elif defined(PSP_E500_CORE)

   asm("   mfspr r3,1010");
   asm("   ori   r4,r3,0x02");
   asm("   xori  r3,r4,0x02");
   _PSP_SYNC();
   _PSP_ISYNC();
   asm("   mtspr 1010,r4");
   asm("   mtspr 1010,r3");
   _PSP_ISYNC();

#elif defined(PSP_E200_CORE) 
   /* Invalidation operation */
   asm("   loop_out:");
   asm("   li     r3, 0x2@l");
   asm("   msync");
   asm("   isync");
   asm("   mtspr  1010, r3");
   asm("   loop:");
   /* Wait until invalidation operation is completed */
   asm("   mfspr  r3, 1010");
   asm("   rlwinm r3, r3, 0, 30, 30");
   asm("   cmplwi r3, 0x2");
   asm("   beq    loop");
   /* Check invalidation operation completion status */
   asm("   mfspr  r3, 1010");
   asm("   rlwinm r3, r3, 0, 29, 29");
   asm("   cmplwi r3, 0x4");
   asm("   beq    loop_out");


#elif defined(PSP_MPC821) || defined(PSP_MPC823) || defined(PSP_MPC850) || \
      defined(PSP_MPC855) || defined(PSP_MPC860) || defined(PSP_MPC866) || \
      defined(PSP_MPC875)

   asm("   mfspr r3,568");
   asm("   oris  r3,r3,0x0C00");
   _PSP_SYNC();
   asm("   mtspr 568,r3");
   _PSP_SYNC();

#elif defined(PSP_PPC440)
   asm("   dccci %r0,%r0");
   _PSP_SYNC();

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_invalidate_line
* Comments         :
*    This function is called to invalidate a line
*    out of the data cache.
*
*END*------------------------------------------------------------------------*/

void _dcache_invalidate_line
   (
      /* [IN] the address in the cache line to invalidate */
      pointer address
   )
{ /* Body */

#if defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */
#elif defined(PSP_E500_CORE) || defined(PSP_E200_CORE)
   register pointer p = address;
   asm {
      dcbi  0, p
      msync
      msync
   } 

#else
   register pointer p = address;
   asm {
      dcbi  0, p
      sync
      sync
   } 
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _dcache_invalidate_mlines
* Returned Value   : none
* Comments         :
* 
*  This function is called to Invalidate the data cache.
*
*END*-----------------------------------------------------------------------*/

void _dcache_invalidate_mlines
   (
      /* [IN] the address somewhere in the cache line */
      pointer     cache_entry,

      /* [IN] the number of bytes from the address requiring flushing */
      uint_32     entry_size
   )
{ /* Body */

#if defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */
#else
   uint_32     final_address;
   uint_32     cache_address;

   /*
   **  set cache_entry to the beginning of the cache line;  final_address
   **  indicates the last address which needs to be pushed.  Increment
   **  cache_entry and push the resulting line until final_address is passed.
   */
   if(cache_entry == NULL)
      return;
   
   cache_address = (uint_32)cache_entry;
   final_address = (cache_address + entry_size -1);
   cache_address = cache_address & PSP_CACHE_LINE_ALIGNMENT;

   while ( (cache_address <= final_address) ) {
      _dcache_invalidate_line((pointer)cache_address);
      cache_address += PSP_CACHE_LINE_SIZE;
   } /* Endwhile */
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _icache_enable
* Returned Value   : none
* Comments         : 
*   This function enables the instruction cache
*
*END*----------------------------------------------------------------------*/

void _icache_enable
   (
      /* [IN] icache enable parameter */
      uint_32 parameter
   )
{ /* Body */

#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480) || \
    defined(PSP_PPC405)

   _PSP_SYNC();
   _PSP_ISYNC();
   _PSP_SPR_SET(1019, parameter);   // mtspr iccr,param
   _PSP_SYNC();
   _PSP_ISYNC();

#elif defined(PSP_PPC603) || defined(PSP_MPC8240) || defined(PSP_G2_CORE) || \
      defined(PSP_E300_CORE)

   asm("   mfspr r3,1008");
   asm("   ori   r4,r3,0xA800");
   asm("   xori  r3,r4,0x2800");
   asm("   xori  r4,r4,0x2000");
   _PSP_ISYNC();
   asm("   mtspr 1008,r4");
   asm("   mtspr 1008,r3");
   _PSP_SYNC();

#elif defined(PSP_E500_CORE)
   register uint_32 i,e;
   _PSP_SPR_GET(i,1011); /* L1CSR1 */
   i |= 0x03;
   e = i ^ 0x02;
   _PSP_SYNC();
   _PSP_ISYNC();
   _PSP_SPR_SET(1011,i);
   _PSP_SPR_SET(1011,e);
   _PSP_ISYNC();

#elif defined(PSP_E200_CORE)
#if defined (PSP_E200_CORE_Z7)
   register uint_32 i;
   /* Cache must be invalidated first and then enabled */
   _icache_invalidate();
   _PSP_SPR_GET(i,E200CORE_L1CSR1);        /* L1CSR1 */
   /* enable instruction cache */
   i |= E200CORE_L1CSR1_ICE;
   asm ("msync");
   _PSP_ISYNC();
   _PSP_SPR_SET(E200CORE_L1CSR1,i);      /* instruction enable cache */
   _PSP_ISYNC();
   asm ("isync");
#endif
   

#elif defined(PSP_PPC740) || defined(PSP_PPC750) || defined(PSP_PPC7400)
   asm("   mfspr r3,1008");
   asm("   ori   r4,r3,0xA808");
   asm("   xori  r3,r4,0x2800");
   asm("   xori  r4,r4,0x2000");
   _PSP_ISYNC();
   asm("   mtspr 1008,r4");
   asm("   mtspr 1008,r3");
   _PSP_SYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

#elif defined(PSP_MPC505) || defined(PSP_MPC821) || defined(PSP_MPC823) || \
      defined(PSP_MPC850) || defined(PSP_MPC855) || defined(PSP_MPC860) || \
      defined(PSP_MPC866) || defined(PSP_MPC875)

   asm("   mfspr r3,560");
   asm("   oris  r3,r3,0x0200");
   asm("   mtspr 560,r3");
   _PSP_ISYNC();
   
#elif defined(PSP_PPC440)
   // Cache control registers have to be set up during boot process.

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _icache_disable
* Returned Value   : none
* Comments         : 
*   This function disables the instruction cache
*
*END*----------------------------------------------------------------------*/

void _icache_disable
   (
      void
   )
{ /* Body */

#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480) || \
    defined(PSP_PPC405)

   _icache_invalidate();
   _PSP_ISYNC();
   _PSP_SPR_SET(1019, 0);   // ICCR <= 0

#elif defined(PSP_PPC603)  || defined(PSP_PPC740)  || defined(PSP_PPC750) || \
      defined(PSP_MPC8240) || defined(PSP_G2_CORE) || defined(PSP_PPC7400) || \
      defined(PSP_E300_CORE)

   _icache_invalidate();
   asm("   mfspr r3,1008");
   asm("   rlwinm r3,r3,0,17,15");
   _PSP_ISYNC();
   asm("   mtspr 1008,r3");
   _PSP_ISYNC();

#elif defined(PSP_E500_CORE) || (defined(PSP_E200_CORE) && defined (PSP_E200_CORE_Z7))

   register uint_32 val;
   _dcache_flush();
   _PSP_SPR_GET(val,1011); /* L1CSR1 */
   val &= ~0x01;
#ifdef PSP_BOOKE    
   asm ("msync");
#else   
   _PSP_SYNC();
#endif   
   _PSP_ISYNC();
   _PSP_SPR_SET(1011,val);
   _PSP_ISYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

#elif defined(PSP_MPC505) || defined(PSP_MPC821) || defined(PSP_MPC823) || \
      defined(PSP_MPC850) || defined(PSP_MPC855) || defined(PSP_MPC860) || \
      defined(PSP_MPC866) || defined(PSP_MPC875)

   _icache_invalidate();
   asm("   mfspr r3,560");
   asm("   oris r3,r3,0x0400");
   asm("   mtspr 560,r3");
   _PSP_ISYNC();
   
#elif defined(PSP_PPC440)
   // Cache cannot be easily disabled.  You must modify the appropriate
   // TLB entries....

#elif defined(PSP_E200_CORE)

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _icache_invalidate
* Returned Value   : none
* Comments         : 
*   This function invalidates the instruction cache
*
*END*----------------------------------------------------------------------*/

void _icache_invalidate
   (
      void
   )
{ /* Body */
   
#if defined(PSP_PPC403) || defined(PSP_PPC401) || defined(PSP_IOP480)

   asm(" li     r5, 0");           /* line 0 */
   asm("1:");             /* loop */
   asm(" iccci  0, r5");           /* invalidate class */
   asm(" addi   r5, r5, 16");      /* increment address */
   asm(" andis. r6, r5, 0xFFFC");  /* last flushed ?*/
   asm(" bne    CR0, 1b"); /* next line */

#elif defined(PSP_PPC405)
   _mqx_int line;
   register offset = 0;
   register uchar_ptr address = 0;
   for (line =0; line < PSP_ICACHE_NUM_LINES*8; line++) {
      /*D-cache has m_lines congruence classes */
      _ICCCI(address, offset);
      address += PSP_CACHE_LINE_SIZE; /*point to the next congruence class */
   } /* Endfor */
   _PSP_SYNC();


#elif defined(PSP_PPC603)  || defined(PSP_PPC740)  || defined(PSP_PPC750) || \
      defined(PSP_MPC8240) || defined(PSP_G2_CORE) || defined(PSP_PPC7400) || \
      defined(PSP_E300_CORE)

   asm("   mfspr r3,1008");
   asm("   ori   r4,r3,0x800");
   asm("   xori  r3,r4,0x800");
   _PSP_ISYNC();
   asm("   mtspr 1008,r4");
   asm("   mtspr 1008,r3");
   _PSP_SYNC();

#elif defined(PSP_E500_CORE) || (defined(PSP_E200_CORE) && defined (PSP_E200_CORE_Z7))

   asm("   mfspr r3,1011");
   asm("   ori   r4,r3,0x02");
   asm("   xori  r3,r4,0x02");
#ifdef PSP_BOOKE    
   asm ("msync");
#else   
   _PSP_SYNC();
#endif   
   _PSP_ISYNC();
   asm("   mtspr 1011,r4");
   asm("   mtspr 1011,r3");
   _PSP_ISYNC();

#elif defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */

#elif defined(PSP_MPC505) || defined(PSP_MPC821) || defined(PSP_MPC823) || \
      defined(PSP_MPC850) || defined(PSP_MPC855) || defined(PSP_MPC860) || \
      defined(PSP_MPC866) || defined(PSP_MPC875)

   asm("   mfspr r3,560");
   asm("   oris  r3,r3,0x0C00");
   asm("   mtspr 560,r3");
   _PSP_ISYNC();

#elif defined(PSP_PPC440)
   asm("   iccci %r0,%r0");
   _PSP_SYNC();

#elif defined(PSP_E200_CORE)

/* catch ERROR : undefined cpu type " */
#else
   #error "CPU NOT DEFINED"
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _icache_invalidate_line
* Comments         :
*   This function is called to invalidate a line
*   out of the code cache.
*
*END*------------------------------------------------------------------------*/

void _icache_invalidate_line
   (
      /* [IN] the address whose cache line is to be invalidated */
      pointer location
   )
{ /* Body */

#if defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */
#else
   register pointer loc = location;
   asm {
      icbi  0,loc
   }
#ifdef PSP_BOOKE
   asm ("msync");
#else
   _PSP_SYNC();
#endif   
   _PSP_ISYNC();
#endif

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _icache_invalidate_mlines
* Comments         :
* 
*  This function is responsible for performing an instruction cache
*  invalidate.  It must determine how many cache lines need to be invalidated
*  and then perform the invalidation.
*
*END*----------------------------------------------------------------------*/

void _icache_invalidate_mlines
   (
      /* [IN] the starting address to invalidate from */
      pointer     cache_address,
      
      /* [IN] the number of bytes to invalidate */
      uint_32     entry_size
   )
{ /* Body */
#if defined(PSP_MPC555) || defined(PSP_MPC566)
   /* Has no cache */
#else
   uint_32     final_address;
   uint_32     cache_entry;

   if(cache_address == NULL)
      return;
   
   cache_entry = (uint_32)cache_address;

   /*
   **  set cache_entry to the beginning of the cache line;  final_address
   **  indicates the last address which needs to be invalidated.  Increment
   **  cache_entry and invalidate the resulting line until final_address is passed.
   */
   final_address = cache_entry + entry_size -1;
   cache_entry = cache_entry & PSP_CACHE_LINE_ALIGNMENT;

   while ( ((uint_32)cache_entry <= final_address) ) {
      _icache_invalidate_line((pointer)cache_entry);
      cache_entry += PSP_CACHE_LINE_SIZE;
   } /* Endwhile */
#endif

} /* Endbody */

/* End CR 1032 */

/* EOF */
