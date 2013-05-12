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
* $FileName: int_flash.c$
* $Version : 3.6.3.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the low level internal flash driver for the 
*   mcf5282 on-chip flash.
*
*END************************************************************************/

#include "mqx.h"
#include "mcf5282.h"
#include "int_flash.h"

extern uchar __IPSBAR[];

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : GetSectorOffset
* Returned Value   : Address of sector on success and -1 on failer
* Comments         : 
* 
*END*----------------------------------------------------------------------*/
uint_32 GetSectorOffset(uint_32 sector){
      
    if ( (sector >= 0) && (sector < MCF5282_CFM_SECTOR_NUM) )
        return MCF5282_CFM_Sectors_Map[sector];
      
    return (uint_32)-1;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_initilize
* Returned Value   : 
* Comments         : This function initilizes the internal flash on MCF5282
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_initilize
    (
     /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr
    )
{
   VMCF5282_STRUCT_PTR mcf5282_ptr = (VMCF5282_STRUCT_PTR)((void*)(&__IPSBAR[0x0]));
   /* Enable on-chip Flash */
   mcf5282_ptr->SCM.GPACR1 |= 0x0C;

   /* write and read CFM clock divider */
   handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;
   if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
      handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;
      
   /* 
   ** Enable interrupts for protection violation, Access error, Command buffer empty
   ** and Command complete and Enable security key writing. 
   ** Make CFMPROT, CFMSACC and CFMDACC writable.
   */
   handle_ptr->CFMMCR  = 0x03D0;
   /* All sectors are not protected */ 
   handle_ptr->CFMPROT = 0x00000000;
   /* Sectors are mapped in data and program address space */
   handle_ptr->CFMDACC = 0x00000000;
   /* Sectors are mapped to unrestricted space */
   handle_ptr->CFMSACC = 0x00000000;
   
   return CFM_OK;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_reset
* Returned Value   : 
* Comments         : This function programs Clock Divider and check CBEIF bit.
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_reset(MCF5282_CFM_STRUCT_PTR  handle_ptr)
{ /* Body */

   /* write and read CFM clock divider */
   handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;
   if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
      handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;
   
   /* Read CFMUSTAT for CBEIF */
   if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)
      return CFM_RESET_FAILD;

   return (CFM_OK);
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_program_32bit
* Returned Value   : 
* Comments         : This function programs the internal flash
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_program_32bit
    (  
      /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr,

      /* [IN] where to copy data from */
      uint_32_ptr from_ptr,
      
      /* [IN] sector number to start program */
      uint_32     from_sector, 
    
      /* [IN] data size in byte */
      uint_32     size
    )
{ /* Body */

   uint_32  sect_num,
            i, j,
            sector_offset,
            end_offset,
            count;

   uchar_ptr end_ptr;
   uint_32   wait_time = INTFLASH_TIMEOUT;

   if((size > MCF5282_CFM_MAX_SIZE))
      return CFM_INVALID_DATA_SIZE;
   /* calculate number of sectors from data size */
   count = 0;
   sect_num = 0;
   for(j=from_sector; j<=MCF5282_CFM_SECTOR_NUM; j++){
      count++;
      if(size <= (count * MCF5282_CFM_SECTOR_SIZE)){
         sect_num = count + from_sector;
         break;
      }
   }
   if(sect_num == 0)
      return CFM_INVALID_SECTOR_NUMBER;
   
   end_ptr = (uchar_ptr)from_ptr + size;

   for(i=from_sector; i<sect_num; i++){
      /* Read CFMUSTAT for CBEIF */
      if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0){
         while((wait_time > 0) && (handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0){
            handle_ptr->CFMUSTAT &= MCF5282_CFM_CFMUSTAT_CBEIF;
            wait_time--;
         }
      }
      if(wait_time <= 0)
         return CFM_TIMED_OUT;
      handle_ptr->CFMPROT = 0x00000000;
      sector_offset = GetSectorOffset(i);
      end_offset = sector_offset + MCF5282_CFM_SECTOR_SIZE;
      if(sector_offset != -1){
         while((sector_offset < end_offset) && (from_ptr < (uint_32_ptr)end_ptr)){
            /* Write program data to array address */
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)*from_ptr;
            /* Write program command */
            handle_ptr->CFMCMD = MCF5282_CFM_PROGRAM;
            /* Clear CBELF bit */
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
        
            /* Protection violation check */
            if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_PVIOL) != 0){
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_PVIOL;
               MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)*from_ptr;
               handle_ptr->CFMCMD = MCF5282_CFM_PROGRAM;
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
           }
           /* Access error check */
           if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_ACCERR) != 0){
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_ACCERR;
               MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)*from_ptr;
               handle_ptr->CFMCMD = MCF5282_CFM_PROGRAM;
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
           }
           /* Buffer empty check */
           if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) != 0){
              while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CCIF) == 0)){
                 handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CCIF;
                 wait_time--;
              }
              if(wait_time <= 0)
                 return CFM_TIMED_OUT;
              else{
                 handle_ptr->CFMPROT = 0x00000000;
                 if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
                    handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;

                 while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
                    handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
                    wait_time--;
                 }
                 if(wait_time <= 0)
                    return CFM_TIMED_OUT;          
              }
           }     
           /* Update sector offset buy 4 bytes */
           sector_offset += sizeof(uint_32);
           /* Update data */
           from_ptr++;
         }
      } else 
         return CFM_INVALID_SECTOR_NUMBER;
   }
   return CFM_OK;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_read
* Returned Value   : 
* Comments         : This function reads from flash at specified sector and 
*                    copy the data to a buffer.
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_read
    (  
      /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr,

      /* [IN/OUT] start address of buffer to store the data */
      uint_32_ptr   buff_ptr,

      /* [IN] where to copy data from */
      uint_32       from_sector,
      
      /* [IN] size to read in byte */
      uint_32       size
   )
{ /* Body */
   pointer from;
   uint_32 sect_offset;

   sect_offset = GetSectorOffset(from_sector);
   from = (pointer)MCF5282_CFM_BACKDOOR(sect_offset);
   _mem_copy((pointer)from,(pointer)buff_ptr,(_mem_size)size);
   
   return CFM_OK;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_sect_erase
* Returned Value   : 
* Comments         : This function erases sectors of internal flash.
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_sect_erase
   (
      /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr,

      /* [IN] from sector number to erase */
      uint_32  sect_from,

      /* [IN] erase to sector */
      uint_32  sect_to
   )
{ /* Body */
   int i;
   uint_32 sector_offset,
           end_offset,
           result;
   uint_32 wait_time = INTFLASH_TIMEOUT;

   if((sect_from < 0) || (sect_from >= MCF5282_CFM_SECTOR_NUM) || 
      (sect_to < 0) || (sect_to >= MCF5282_CFM_SECTOR_NUM))
      return CFM_INVALID_SECTOR_NUMBER;
   
   result = _mcf5282_cfm_reset(handle_ptr);
   if(result != CFM_OK)
      return result;
   
   for(i=sect_from; i < sect_to ; i++){
      sector_offset = GetSectorOffset(i);   
      if(sector_offset == -1)
         return CFM_INVALID_SECTOR_NUMBER;
      handle_ptr->CFMPROT = 0x00000000;
      if( (handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
         handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;   
      while ((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         wait_time--;
      }
      if(wait_time <= 0)
         return CFM_TIMED_OUT;
      end_offset = sector_offset + MCF5282_CFM_SECTOR_SIZE;
      while ( sector_offset <   end_offset) {
         /* Write program data to array address */
         MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
         /* Write program command */
         handle_ptr->CFMCMD = MCF5282_CFM_PAGE_ERASE;
         /* Clear CBELF bit */
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         /* Protection violation check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_PVIOL) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_PVIOL;
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_PAGE_ERASE;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         }
         /* Access error check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_ACCERR) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_ACCERR;
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_PAGE_ERASE;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         } 
         /* Buffer empty check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) != 0){
            while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CCIF) == 0)){
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CCIF;
               wait_time--;
            }
            if(wait_time <= 0)
               return CFM_TIMED_OUT;
            else{
               handle_ptr->CFMPROT = 0x00000000;
               if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
                  handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;

               while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
                  handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
                  wait_time--;
               }
               if(wait_time <= 0)
                  return CFM_TIMED_OUT;        
            }
         }   
      sector_offset += 0x800;
      }
   }
   return CFM_OK;
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_mass_erase
* Returned Value   : 
* Comments         : This function erases the entier flash.
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_mass_erase
   (
      /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr
   )
{ /* Body */
   int     i;
   uint_32 sect_offset,
           end_offset,
           result; 
   uint_32 wait_time = INTFLASH_TIMEOUT;

   result = _mcf5282_cfm_reset(handle_ptr);
   if(result != CFM_OK)
      return result;

   for(i=0; i<MCF5282_CFM_SECTOR_NUM; i+=16){
      /* Read CFMUSTAT for CBEIF */
      if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0){
         while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
            wait_time--;
         }
      }
      if(wait_time < 0)
         return CFM_TIMED_OUT;
      handle_ptr->CFMPROT = 0x00000000;
      sect_offset = GetSectorOffset(i);
      if(sect_offset == -1)
         return CFM_INVALID_SECTOR_NUMBER;
      // Mass Erase: 256KB       
      end_offset = sect_offset + (MCF5282_CFM_SECTOR_SIZE * 16);
      while(sect_offset < end_offset){
         /* Write program data to array address */
         MCF5282_CFM_BACKDOOR(sect_offset) = (uint_32)0x00000000;
         /* Write program command */
         handle_ptr->CFMCMD = MCF5282_CFM_MASS_ERASE;
         /* Clear CBELF bit */
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         /* Protection violation check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_PVIOL) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_PVIOL;
            MCF5282_CFM_BACKDOOR(sect_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_MASS_ERASE;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         }
         /* Access error check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_ACCERR) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_ACCERR;
            MCF5282_CFM_BACKDOOR(sect_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_MASS_ERASE;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         }
         /* Buffer empty check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) != 0){
            while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CCIF) == 0)){
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CCIF;
               wait_time--;
            }
            if(wait_time <= 0)
               return CFM_TIMED_OUT;
            else{
               handle_ptr->CFMPROT = 0x00000000;
               if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
                  handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;

               while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
                  handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
                  wait_time--;
               }
               if(wait_time <= 0)
                  return CFM_TIMED_OUT;        
            }
         }   
         /* Mass erase: update sector offset by 0x40000 */
         sect_offset += 0x40000;      
      }
   }
   return CFM_OK;
} 

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_verify_sect_erase
* Returned Value   : 
* Comments         : This function verifies sector erase of Flash.
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_verify_sect_erase
   (
      /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr,

      /* [IN] */
      uint_32 sect_from,

      /* [IN] */
      uint_32 sect_to 
   )
{ /* Body */
   int i;
   uint_32 sector_offset,
           end_offset,
           result;
   uint_32 wait_time = INTFLASH_TIMEOUT;

   if((sect_from < 0) || (sect_from >= MCF5282_CFM_SECTOR_NUM) || 
      (sect_to < 0) || (sect_to >= MCF5282_CFM_SECTOR_NUM))
      return CFM_INVALID_SECTOR_NUMBER;
   
   result = _mcf5282_cfm_reset(handle_ptr);
   if(result != CFM_OK)
      return result;
   
   for(i=sect_from; i < sect_to ; i++){
      sector_offset = GetSectorOffset(i);   
      if(sector_offset == -1)
         return CFM_INVALID_SECTOR_NUMBER;
      handle_ptr->CFMPROT = 0x00000000;
      if( (handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
         handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;   
      while ((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         wait_time--;
      }
      if(wait_time <= 0)
         return CFM_TIMED_OUT;
      end_offset = sector_offset + MCF5282_CFM_SECTOR_SIZE;
      while ( sector_offset <   end_offset) {
         /* Write program data to array address */
         MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
         /* Write program command */
         handle_ptr->CFMCMD = MCF5282_CFM_PAGE_ERASE_VERIFY;
         /* Clear CBELF bit */
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         /* Protection violation check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_PVIOL) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_PVIOL;
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_PAGE_ERASE_VERIFY;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         }
         /* Access error check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_ACCERR) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_ACCERR;
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_PAGE_ERASE_VERIFY;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         } 
         /* Buffer empty check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) != 0){
            while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CCIF) == 0)){
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CCIF;
               wait_time--;
            }
            if(wait_time <= 0)
               return CFM_TIMED_OUT;
            else{
               handle_ptr->CFMPROT = 0x00000000;
               if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
                  handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;

               while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
                  handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
                  wait_time--;
               }
               if(wait_time <= 0)
                  return CFM_TIMED_OUT;        
            }
         }   
      sector_offset += 0x800;
      }
   }
   return CFM_OK;
   
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_verify_mass_erase
* Returned Value   : 
* Comments         : This function verifies entier erase of Flash.
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_verify_mass_erase
   (
      /* [IN] the base address of the flash */
      MCF5282_CFM_STRUCT_PTR  handle_ptr
   )
{ /* Body */
   int i;
   uint_32 sector_offset,
           end_offset,
           result;
   uint_32 wait_time = INTFLASH_TIMEOUT;

   result = _mcf5282_cfm_reset(handle_ptr);
   if(result != CFM_OK)
      return result;
   
   for(i=0; i < MCF5282_CFM_SECTOR_NUM ; i+=16){
      sector_offset = GetSectorOffset(i);   
      if(sector_offset == -1)
         return CFM_INVALID_SECTOR_NUMBER;
      handle_ptr->CFMPROT = 0x00000000;
      if( (handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
         handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;   
      while ((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         wait_time--;
      }
      if(wait_time <= 0)
         return CFM_TIMED_OUT;
      end_offset = sector_offset + (MCF5282_CFM_SECTOR_SIZE * 16);
      while ( sector_offset <   end_offset) {
         /* Write program data to array address */
         MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
         /* Write program command */
         handle_ptr->CFMCMD = MCF5282_CFM_MASS_ERASE_VERIFY;
         /* Clear CBELF bit */
         handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         /* Protection violation check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_PVIOL) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_PVIOL;
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_MASS_ERASE_VERIFY;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         }
         /* Access error check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_ACCERR) != 0){
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_ACCERR;
            MCF5282_CFM_BACKDOOR(sector_offset) = (uint_32)0x00000000;
            handle_ptr->CFMCMD = MCF5282_CFM_MASS_ERASE_VERIFY;
            handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
         } 
         /* Buffer empty check */
         if((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) != 0){
            while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CCIF) == 0)){
               handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CCIF;
               wait_time--;
            }
            if(wait_time <= 0)
               return CFM_TIMED_OUT;
            else{
               handle_ptr->CFMPROT = 0x00000000;
               if((handle_ptr->CFMCLKD & MCF5282_CFM_CFMCLKD_DIVLD) == 0)
                  handle_ptr->CFMCLKD = MCF5282_CFM_CFMCLKD_DIVLD_VALUE;

               while((wait_time > 0) && ((handle_ptr->CFMUSTAT & MCF5282_CFM_CFMUSTAT_CBEIF) == 0)){
                  handle_ptr->CFMUSTAT = MCF5282_CFM_CFMUSTAT_CBEIF;
                  wait_time--;
               }
               if(wait_time <= 0)
                  return CFM_TIMED_OUT;        
            }
         }   
      sector_offset += 0x40000;
      }
   }
   return CFM_OK;
   
}

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _mcf5282_cfm_sect_check
* Returned Value   : 
* Comments         : This function blank check the sector
* 
*END*----------------------------------------------------------------------*/
uint_32 _mcf5282_cfm_sect_check
    (  
      /* [IN] the base address of the flash */
      uint_32  handle_ptr,

      /* [IN] sector number to check */
      uint_32  sector   
    )
{ /* Body */

   uint_32_ptr temp_ptr;
   uint_32 sect_offset,end_offset;
   volatile int i;
   
   if((sector < 0) || (sector >= MCF5282_CFM_SECTOR_NUM))
      return CFM_INVALID_SECTOR_NUMBER;

   sect_offset =  GetSectorOffset(sector);
   if(sect_offset == -1)
      return CFM_INVALID_SECTOR_NUMBER;

   for(i=0; i<100; i++){}
   end_offset = sect_offset + MCF5282_CFM_SECTOR_SIZE;
   temp_ptr = (uint_32_ptr)(handle_ptr + sect_offset);

   while(sect_offset < end_offset){
      if(*temp_ptr != MCF5282_CFM_SECTOR_BLANK_CHECK)
         return 1;
      temp_ptr++;
      sect_offset+=4;
   }

   return CFM_OK;
}

