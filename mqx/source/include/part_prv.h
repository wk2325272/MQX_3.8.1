#ifndef __part_prv_h__
#define __part_prv_h__
/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: part_prv.h$
* $Version : 3.0.3.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains private definitions for the partition management
*   component
*
*END************************************************************************/

/*--------------------------------------------------------------------------*/
/*                        CONSTANT DEFINITIONS                              */

/* The validation check for the partitions */
#define PARTITION_VALID      ((_mqx_uint)(0x70617274))  /* "part" */

/* Checksum macros */
#if MQX_CHECK_VALIDITY
#define CALC_PARTITION_CHECKSUM(block) \
   (block)->CHECKSUM = (_mqx_uint)((block)->LINK.PARTITION_PTR) + \
      (_mqx_uint)((block)->TASK_ID);
#else
#define CALC_PARTITION_CHECKSUM(block)
#endif

#if MQX_CHECK_VALIDITY
#define VALID_PARTITION_CHECKSUM(block) \
   ( (block)->CHECKSUM == ((_mqx_uint)((block)->LINK.PARTITION_PTR) + \
          (_mqx_uint)((block)->TASK_ID)) )
#else
#define VALID_PARTITION_CHECKSUM(block) TRUE
#endif

/* Parition created from MQX memory pool by _partition_create and can grow */
#define PARTITION_DYNAMIC         (0x2)

/* Parition created at a fixed location by _partition_create_at */
#define PARTITION_STATIC          (0x4)

/*----------------------------------------------------------------------------*/
/*                        DATA TYPE DEFINTIONS
*/

struct internal_partition_block_struct;

/* 
** PARTPOOL BLOCK STRUCT
** This structure is used at the beginning of each memory area where 
** blocks have been created for a partition.
*/
typedef struct partpool_block_struct
{                  
   /* The next memory area where blocks are for this partition */
   struct partpool_block_struct _PTR_             NEXT_POOL_PTR;

   /* Is the pool valid */
   _mqx_uint                                      VALID;

   /* How many blocks in this memory area */
   _mqx_uint                                      NUMBER_OF_BLOCKS;
   
   /* The address of the first internal block of the pool */
   struct internal_partition_block_struct _PTR_   FIRST_IBLOCK_PTR;
   
} PARTPOOL_BLOCK_STRUCT, _PTR_ PARTPOOL_BLOCK_STRUCT_PTR;


/* 
** PARTPOOL_STRUCT
** This structure defines what a partition pool looks like.
** It overlays the beginning of the memory area used for a partition, when
** the partition is created.
** 
** All of the partition pools in the kernel are linked together via 
** the NEXT_PARITION_PTR field.
**
** When a new memory area is added (to extend the partition), the new
** memory area begins with a PARTPOOL_BLOCK structure.  All of these 
** are linked together via the NEXT_POOL_PTR field of the partpool block struct.
**
** All of the Partition Blocks are individually linked together onto a free list      
** 
*/
typedef struct partpool_struct
{
   /* Link pointers to maintain a list of partitions in the kernel */   
   pointer                                       NEXT;
   pointer                                       PREV;

   /* The actual size each block in the partition */
   _mem_size                                     BLOCK_SIZE;

   /* How many total blocks are available in the partition */
   _mem_size                                     AVAILABLE;

   /* The free list of available blocks */
   struct internal_partition_block_struct _PTR_  FREE_LIST_PTR;

   /* The total number of blocks in the partition */
   _mem_size                                     TOTAL_BLOCKS;
 
   /* The maximum number of blocks that have been in use at one time */
   _mem_size                                     MAX_BLOCKS_USED;

   /* The number of blocks to grow the partition by if necessary */
   _mem_size                                     GROW_BLOCKS;

   /* The maximum # of blocks to allow in  the partition */
   _mem_size                                     MAXIMUM_BLOCKS;

   /* The partition type, either DYNAMIC or STATIC */
   _mqx_uint                                     PARTITION_TYPE;

   /* A context pointer used by the partition test function */
   struct internal_partition_block_struct volatile _PTR_  TEST_FREE_PTR;
   
   /* The first set of blocks in the partition */
   PARTPOOL_BLOCK_STRUCT                         POOL;
   
} PARTPOOL_STRUCT, _PTR_ PARTPOOL_STRUCT_PTR;


/* 
** INTERNAL PARTITION BLOCK STRUCT
** Each block in the partition has this structure overlaid onto the beginning
** of the block.  The address provided to the application for the block
** is the memory address following this header.
*/
typedef struct internal_partition_block_struct
{
   /* 
   ** When on the free list this pointer links blocks together
   ** When allocated, this field points back to the original partition
   */
   union  {
      struct internal_partition_block_struct _PTR_ NEXT_BLOCK_PTR;
      PARTPOOL_STRUCT_PTR                          PARTITION_PTR;      
   } LINK;

   /* The block checksum */
   _mqx_uint            CHECKSUM;

   /* The owner task ID */
   _task_id             TASK_ID;

} INTERNAL_PARTITION_BLOCK_STRUCT, _PTR_ INTERNAL_PARTITION_BLOCK_STRUCT_PTR;


/* 
** PARTITION COMPONENT STRUCT
**
** This structure is used to store information 
** required for partition component
*/
typedef struct partition_component_struct
{
   /* A queue of all created partitions */
   QUEUE_STRUCT PARTITIONS;

   /* A validation field for partitions */
   _mqx_uint    VALID;

} PARTITION_COMPONENT_STRUCT, _PTR_ PARTITION_COMPONENT_STRUCT_PTR;


/*--------------------------------------------------------------------------*/
/*                        Prototypes                                        */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __TAD_COMPILE__
extern _mqx_uint _partition_create_internal(PARTPOOL_STRUCT_PTR, _mem_size, 
   _mqx_uint);
extern void      _partition_extend_internal(PARTPOOL_STRUCT_PTR, 
   PARTPOOL_BLOCK_STRUCT_PTR, _mqx_uint); 
extern pointer   _partition_alloc_internal(PARTPOOL_STRUCT_PTR, TD_STRUCT_PTR);
extern void      _partition_cleanup(TD_STRUCT_PTR);
#endif

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
