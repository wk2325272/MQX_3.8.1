/**HEADER********************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: nandflash_demo.c$
* $Version : 3.8.9.1$
* $Date    : Feb-16-2012$
*
* Comments:
*
*   This file contains the source for the nandflash example application.
*
*END************************************************************************/

#include <stdlib.h>
#include <mqx.h>
#include <bsp.h>
#include <fio.h>
#include <string.h>

#if BSP_TWRMCF54418 | BSP_M5329EVB | BSP_TWRMPC5125 | BSP_TWR_K70F120M | BSP_TWR_K60F120M
    #if ! BSPCFG_ENABLE_NANDFLASH
    #error This application requires BSPCFG_ENABLE_NANDFLASH defined non-zero in user_config.h. Please recompile BSP with this option.
    #endif
    /* external flash name */
    #define         FLASH_NAME "nandflash:"
#endif

/* Task IDs */
#define NILL_TASK 7

/* Set to 1 to call FLASH_IOCTL_ERASE_CHIP if chip erase function is present */
#define FLASH_TEST_ERASE_CHIP    (0)

/* function prototypes */
MQX_FILE_PTR nandflash_open(char_ptr name);
void nandflash_close(MQX_FILE_PTR nandflash);
uchar_ptr allocate_buffer(_mem_size buffer_size, char_ptr string);
void size_compare( MQX_FILE_PTR flash_hdl, _mqx_int i, _mqx_int read_write_size );
void compare_test( uchar_ptr write_buffer, uchar_ptr read_buffer, _mem_size read_write_size);
extern void nill_task(uint_32);


const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority,   Name,   Attributes,          Param, Time Slice */
    { NILL_TASK,    nill_task,  4000,   8,          "nill", MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

/*FUNCTION*-----------------------------------------------------
*
* Task Name    : nandflash_open
* Comments     :
*    Open the flash device
*
*END*-----------------------------------------------------*/
MQX_FILE_PTR nandflash_open(char_ptr name)
{
    MQX_FILE_PTR nandflash_hdl = NULL;

    /* Open the flash device */

   nandflash_hdl = fopen(name, NULL);
    if (nandflash_hdl == NULL) {
        printf("\nUnable to open NAND flash device %s", name);
        _task_block();
    } /* Endif */
    printf("\nNAND Flash device %s opened", name);
    return nandflash_hdl;
}

/*FUNCTION*-----------------------------------------------------
*
* Task Name    : nandflash_close
* Comments     :
*    Close the flash device
*
*END*-----------------------------------------------------*/
void nandflash_close(MQX_FILE_PTR nandflash)
{
    fclose(nandflash);
    printf("\n\nNAND Flash closed.");
}

/*FUNCTION*-----------------------------------------------------
*
* Task Name    : allocate_buffer
* Comments     :
*    Allocate the buffer
*
*END*-----------------------------------------------------*/
uchar_ptr allocate_buffer(_mem_size buffer_size, char_ptr string)
{
    uchar_ptr buffer_ptr;

    buffer_ptr = _mem_alloc_zero(buffer_size);
    if (buffer_ptr == NULL) {
        printf("\nFailed to get %s buffer", string);
        _task_block();
    } /* Endif */
    return buffer_ptr;
}


/*FUNCTION*-----------------------------------------------------
*
* Task Name    : compare_test
* Comments     :
*    function compare write_buffer and read_buffer
*
*END*-----------------------------------------------------*/
void compare_test( uchar_ptr write_buffer, uchar_ptr read_buffer, _mem_size read_write_size)
{
    if (memcmp(write_buffer, read_buffer, read_write_size)) {
         printf("Compare failed\n");
         _task_block();
    }
    else {
        printf("Compare OK\n");
    }
}

/*FUNCTION*-----------------------------------------------------
*
* Task Name    : size_compare
* Comments     :
*    function compare i and read_write_size
*
*END*-----------------------------------------------------*/
void size_compare( MQX_FILE_PTR flash_hdl, _mqx_int i, _mqx_int read_write_size )
{
    if (i != read_write_size ) {
        printf("\nFailed to write flash, size returned:%d expected %d", i,
        read_write_size);
        printf("\nTEC:0x%X FERROR:0x%X", _task_get_error(), ferror(flash_hdl));
        _task_block();
    }
    else {
        printf("Done");
    }/* Endif */
}

/*TASK*-----------------------------------------------------
*
* Task Name    : nill_task
* Comments     :
*    This task does nothing
*
*END*-----------------------------------------------------*/
void nill_task
   (
      uint_32 initial_data
   )
{
   MQX_FILE_PTR   nandflash_hdl;
   _mqx_int       i;
   _mqx_uint      ID, num_blocks, virt_page_size, block_size, tmp = 0;
   uchar_ptr      write_buffer, read_buffer;
   _mem_size      num_virt_pages_per_block, seek_location;
   uint_8_ptr     bbt;

   _int_install_unexpected_isr();

   puts("\n\n");
   puts("MQX NAND Flash Example Application\n");
   puts("==================================\n");

   /* Open the flash device */
   nandflash_hdl = nandflash_open(FLASH_NAME);

   /* Get NAND Flash organization data */
   puts("\nObtaining NAND Flash organization data ...");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_ID, &ID))
       printf("\nID:                      0x%x", ID);
   else
       printf("\nID not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_PHY_PAGE_SIZE, &tmp))
       printf("\nPhysical page size:      %d bytes", tmp);
   else
       printf("\nPhysical page size not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_SPARE_AREA_SIZE, &tmp))
       printf("\nSpare area size:         %d bytes", tmp);
   else
       printf("\nSpare area size not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_BLOCK_SIZE, &block_size))
       printf("\nBlock size:              %d bytes", block_size);
   else
       printf("\nBlock size not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_NUM_BLOCKS, &num_blocks))
       printf("\nNumber of blocks:        %d", num_blocks);
   else
       printf("\nNumber of blocks not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_WIDTH, &tmp))
       printf("\nWidth:                   %d", tmp);
   else
       printf("\nWidth not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_NUM_VIRT_PAGES, &tmp))
       printf("\nNumber of virtual pages: %d", tmp);
   else
       printf("\nNumber of virtual pages not obtained");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_VIRT_PAGE_SIZE, &virt_page_size))
       printf("\nVirtual page size:       %d bytes\n", virt_page_size);
   else
       printf("\nVirtual page size not obtained\n");

   /* Get Bad Block Table */
   bbt = _mem_alloc_zero(num_blocks);
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_GET_BAD_BLOCK_TABLE, bbt))
   {
       for (i=0; i<num_blocks; i++)
           if(bbt[i]==0)
           {
                printf("\nBlock #%d is bad", i);
                if(NANDFLASHERR_BLOCK_BAD != ioctl(nandflash_hdl, NANDFLASH_IOCTL_CHECK_BLOCK, (void*)i))
                    printf("\nCHECK BLOCK IOCTL command testing failed");
           }
   }
   else
   {
       printf("\nReading Bad block table not successful ");
   }

#if BSP_TWR_K60F120M
   /*
   ** Needed to decrease the size of buffer because of limited SRAM memory
   ** TWR-K60F120M - use nand device which block size is 128kB.
   ** There is not enough RAM memory for allocating one block.
   */
   block_size = block_size/8;
#endif

   /* Allocate read and write buffers */
   write_buffer = allocate_buffer(2*block_size, "write");
   read_buffer = allocate_buffer(2*block_size, "read");

   /* Calculate the number of virtual pages per one block */
   num_virt_pages_per_block = block_size / virt_page_size;

   /* Fill data to write buffer */
   for ( i = 0; i < 2*block_size; i++ )
        write_buffer[i] = rand();

   /* Any single bit can only be programmed one time before an erase is required
      and pages must be programmed consecutively from the least significant bit page
      of the block to the most significant bit pages of the block.
      Erase block #0 before writting to the first page of the block. */
   if(MQX_OK != ioctl(nandflash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK, (void*)0))
       printf("\n\nErasing block #0 failed.");
   else
       printf("\n\nErasing block #0 passed.");

   /* Write to block #0 */
   printf("\nWriting data to %d virtual pages of block #0 ... ", num_virt_pages_per_block);
   seek_location = 0;
   fseek(nandflash_hdl, seek_location, IO_SEEK_SET);
   i = write(nandflash_hdl, &write_buffer[0], num_virt_pages_per_block);
   size_compare(nandflash_hdl, i, num_virt_pages_per_block);

   /* Read data back from block #0 and compare with the write buffer */
   printf("\n\nReading data back from %d virtual pages of block #0 ... ", num_virt_pages_per_block);
   fseek(nandflash_hdl, seek_location, IO_SEEK_SET);
   if(num_virt_pages_per_block == read(nandflash_hdl, read_buffer, num_virt_pages_per_block))
       printf("Done");
   else
       printf("Failed");
   printf("\nComparing data ... ");
   compare_test(write_buffer, read_buffer, block_size);


#if (FLASH_TEST_ERASE_CHIP)
   printf("\n\nErasing entire chip ... ");
   if(MQX_OK == ioctl(nandflash_hdl, NANDFLASH_IOCTL_ERASE_CHIP, NULL))
       printf("Done");
   else
       printf("Failed");
#endif

   /* The following section shows how bad block management is performed.
      Block #1 is marked as bad and checked afterwards. Then, the block
      marked as bad is forced to be erased causing the block #1 is marked
      as good again which is checked afterwards. */
   if(NANDFLASHERR_NO_ERROR != ioctl(nandflash_hdl, NANDFLASH_IOCTL_MARK_BLOCK_AS_BAD, (void*)1))
       printf("\nMarking block #1 as bad failed.");
   if(NANDFLASHERR_BLOCK_BAD != ioctl(nandflash_hdl, NANDFLASH_IOCTL_CHECK_BLOCK, (void*)1))
       printf("\nChecking block #1 failed.");
   if(NANDFLASHERR_NO_ERROR != ioctl(nandflash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK_FORCE, (void*)1))
       printf("\nErasing block #1 failed.");
   else
       printf("\nErasing block #1 passed.");

   if(NANDFLASHERR_BLOCK_NOT_BAD != ioctl(nandflash_hdl, NANDFLASH_IOCTL_CHECK_BLOCK, (void*)1))
       printf("\nChecking block #1 failed.");

   /* Do several consequent writes, starting at the beginning of the block #1 and loading the whole blocks #1 and #2. */

   /* Fill new data to write buffer */
   for ( i = 0; i < 2*block_size; i++ )
        write_buffer[i] = rand();

   /* As there is no need to erase the block #1 again (NANDFLASH_IOCTL_ERASE_BLOCK_FORCE IOCTL command was already
      executed) erase just the block #2 */
   if(MQX_OK != ioctl(nandflash_hdl, NANDFLASH_IOCTL_ERASE_BLOCK, (void*)2))
       printf("\n\nErasing block #2 failed.");
   else
       printf("\n\nErasing block #2 passed.");

   /* Write into 3/4 of virtual pages of the block #1 */
   printf("\n\nWriting data to %d virtual pages of block #1 ... ", 3*num_virt_pages_per_block/4);
   seek_location = 1*num_virt_pages_per_block;
   fseek(nandflash_hdl, seek_location, IO_SEEK_SET);
   i = write(nandflash_hdl, &write_buffer[0], 3*num_virt_pages_per_block/4);
   size_compare(nandflash_hdl, i, 3*num_virt_pages_per_block/4);

   /* Write another X virtual pages, crossing between block #1 and block #2 */
   printf("\nWriting another %d virtual pages, crossing between block #1 and block #2 ... ", 3*num_virt_pages_per_block/4);
   seek_location += 3*num_virt_pages_per_block/4;
   fseek(nandflash_hdl, seek_location, IO_SEEK_SET);
   i = write(nandflash_hdl, &write_buffer[(3*num_virt_pages_per_block/4)*virt_page_size], 3*num_virt_pages_per_block/4);
   size_compare(nandflash_hdl, i, 3*num_virt_pages_per_block/4);

   /* Write the rest of the block #2 */
   printf("\nWriting data to the rest of block #2 ... ");
   seek_location += 3*num_virt_pages_per_block/4;
   fseek(nandflash_hdl, seek_location, IO_SEEK_SET);
   i = write(nandflash_hdl, &write_buffer[2*(3*num_virt_pages_per_block/4)*virt_page_size], (2*num_virt_pages_per_block)-(2*3*num_virt_pages_per_block/4));
   size_compare(nandflash_hdl, i, (2*num_virt_pages_per_block)-(2*3*num_virt_pages_per_block/4));

   /* Read data back from the whole block #1 and #2 and compare with the write buffer */
   printf("\n\nReading data back from the whole block #1 and #2 ... ");
   seek_location -= 2*(3*num_virt_pages_per_block/4);
   fseek(nandflash_hdl, seek_location, IO_SEEK_SET);
   if(2*num_virt_pages_per_block == read(nandflash_hdl, read_buffer, 2*num_virt_pages_per_block))
       printf("Done");
   else
       printf("Failed");
   printf("\nComparing data ... ");
   compare_test(write_buffer, read_buffer, 2*block_size);


   /* Close the device */
   nandflash_close(nandflash_hdl);
}

/* EOF */
