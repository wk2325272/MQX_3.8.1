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
* $FileName: int_flash.h$
* $Version : 3.8.6.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the type definitions for internal flash of
*   the mcf5282 processor.
*
*END************************************************************************/
extern uchar __IPSBAR[];

/* ColdFire on-chip Flash Module (CFM) */
#define MCF5282_CFM_CFMMCR_LOCK             (0x0400)
#define MCF5282_CFM_CFMMCR_PVIE             (0x0200)
#define MCF5282_CFM_CFMMCR_AEIE             (0x0100)
#define MCF5282_CFM_CFMMCR_CBEIE            (0x0080)
#define MCF5282_CFM_CFMMCR_CCIE             (0x0040)
#define MCF5282_CFM_CFMMCR_KEYACC           (0x0020)

#define MCF5282_CFM_CFMCLKD_DIVLD           (0x80)
#define MCF5282_CFM_CFMCLKD_PRDIV8          (0x40)
#define MCF5282_CFM_CFMCLKD_DIV(x)          (((x)&0x3F))

#define MCF5282_CFM_CFMSEC_KEYEN            (0x80000000)
#define MCF5282_CFM_CFMSEC_SECSTAT          (0x40000000)
#define MCF5282_CFM_CFMSEC_SEC(x)           (((x)&0xFFFF))

#define MCF5282_CFM_CFMUSTAT_CBEIF          (0x80)
#define MCF5282_CFM_CFMUSTAT_CCIF           (0x40)
#define MCF5282_CFM_CFMUSTAT_PVIOL          (0x20)
#define MCF5282_CFM_CFMUSTAT_ACCERR         (0x10)
#define MCF5282_CFM_CFMUSTAT_BLANK          (0x04)

#ifdef MCF5213_PROCESSOR /* MCF5213 */
#define MCF5282_CFM_SECTOR_NUM              (16)
#else
#define MCF5282_CFM_SECTOR_NUM              (32)
#endif

#define MCF5282_CFM_SECTOR_SIZE             (0x4000)

#ifdef MCF5213_PROCESSOR /* MCF5213 */
#define MCF5282_CFM_MAX_SIZE                (0x3FFFF)
#else
#define MCF5282_CFM_MAX_SIZE                (0x7FFFF)
#endif

#define MCF5282_CFM_SECTOR_BLANK_CHECK      (0xFFFFFFFF)

#define MCF5282_CFM_CFMCMD_CMD(x)           (((x)&0x7F))
#define MCF5282_CFM_BACKDOOR(x)             (*(volatile uint_32*)((void *)(&__IPSBAR[0x04000000+x])))

#define MCF5282_CFM_MASS_ERASE_VERIFY       (0x05)
#define MCF5282_CFM_PROGRAM                 (0x20)
#define MCF5282_CFM_PAGE_ERASE              (0x40)
#define MCF5282_CFM_MASS_ERASE              (0x41)
#define MCF5282_CFM_PAGE_ERASE_VERIFY       (0x06)

   /* 
   ** Program CFM clock divider: it has to be between 150KHz to 200 KHz
   ** system frequency @ 66MHz => PRDIV8 = 66/2=33 MHz > 12.8 => PRDIV8 = 1
   ** DIV[5:0] = 66MHz/(2*200KHz*(1+(PRDIV8 + 7))) = 20
   ** Therefore CFM CLK frequency will be 196.43 KHz
   */
#define MCF5282_CFM_CFMCLKD_DIVLD_VALUE     (0x54)

/* Wait time to set CBEIF and CCEIF */
#define INTFLASH_TIMEOUT                    (0x8000000)

/* CFM Error Code*/
#define CFM_ERROR_BASE  (0x100)
#define CFM_OK          (0)
#define CFM_RESET_FAILD             (CFM_ERROR_BASE | 0x01)
#define CFM_INVALID_DATA_SIZE       (CFM_ERROR_BASE | 0x02)
#define CFM_INVALID_SECTOR_NUMBER   (CFM_ERROR_BASE | 0x03)
#define CFM_TIMED_OUT               (CFM_ERROR_BASE | 0x04)
/* 
** CFM sectors memory map 
** Flash is divided into 4 physical blocks. There are 8 sectors in each block.
** Block size: 128 KB 
*/
static uint_32 MCF5282_CFM_Sectors_Map[] = {
                                            // Block 0 
                                            0x000000, 0x004000, 0x008000, 0x00C000, 0x010000,
                                            0x014000, 0x018000, 0x01C000, 
                                            // Block 1
                                            0x020000, 0x024000, 0x028000, 0x02C000, 0x030000, 
                                            0x034000, 0x038000, 0x03C000
#ifndef MCF5213_PROCESSOR /* MCF5213 */
                                            ,
                                            // Block 2
                                            0x040000, 0x044000, 0x048000, 0x04C000, 0x050000, 
                                            0x054000, 0x058000, 0x05C000, 
                                            // Block 3
                                            0x060000, 0x064000, 0x068000, 0x06C000, 0x070000, 
                                            0x074000, 0x078000, 0x07C000
#endif
                                           };
/*
** MCF5282_CFM_STRUCT
** This structure defines registers provided by on-chip Flash on MCF5282
*/
typedef struct MCF5282_CFM_struct
{
   uint_16 CFMMCR;      // Configuration
   uchar   CFMCLKD;     // Clock Divider
   uchar   filler1[5];
   uint_32 CFMSEC;      // Security
   uchar   filler2[4];
   uint_32 CFMPROT;     // Protection
   uint_32 CFMSACC;     // Supervisor Access
   uint_32 CFMDACC;     // Data Access 
   uchar   filler3[4];
   uchar   CFMUSTAT;    // User Status
   uchar   filler4[3];
   uchar   CFMCMD;      // Command
   uchar   filler5[3];
} MCF5282_CFM_STRUCT, _PTR_ MCF5282_CFM_STRUCT_PTR;
typedef volatile struct MCF5282_CFM_struct _PTR_ VMCF5282_CFM_STRUCT_PTR;

typedef struct MCF5282_CFM_MEM_struct
{
   char *  from_addr; // Where to data copy from
   char *  to_addr;   // Where to start to copy
   uint_32 reserved;
} MCF5282_CFM_MEM_STRUCT, _PTR_ MCF5282_CFM_MEM_STRUCT_PTR;
typedef volatile struct MCF5282_CFM_MEM_struct _PTR_ VMCF5282_CFM_MEM_STRUCT_PTR;

extern uint_32 _mcf5282_cfm_initilize(MCF5282_CFM_STRUCT_PTR);
extern uint_32 _mcf5282_cfm_program_32bit(MCF5282_CFM_STRUCT_PTR,uint_32_ptr,uint_32,uint_32);
extern uint_32 _mcf5282_cfm_sect_erase(MCF5282_CFM_STRUCT_PTR,uint_32,uint_32);
extern uint_32 _mcf5282_cfm_mass_erase(MCF5282_CFM_STRUCT_PTR);
extern uint_32 _mcf5282_cfm_read(MCF5282_CFM_STRUCT_PTR,uint_32_ptr,uint_32,uint_32);
extern uint_32 _mcf5282_cfm_verify_sect_erase(MCF5282_CFM_STRUCT_PTR,uint_32,uint_32);
extern uint_32 _mcf5282_cfm_verify_mass_erase(MCF5282_CFM_STRUCT_PTR);
extern uint_32 _mcf5282_cfm_sect_check(uint_32,uint_32);
extern uint_32 _mcf5282_cfm_reset(MCF5282_CFM_STRUCT_PTR);

/* EOF */
