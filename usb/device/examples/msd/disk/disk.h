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
* $FileName: disk.h$
* $Version : 3.6.2.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
* @brief The file contains Macro's and functions needed by the disk 
*        application
*
*****************************************************************************/


#ifndef _DISK_H
#define _DISK_H  1

#include "usb_descriptor.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define RAM_DISK_APP        (1)

#if RAM_DISK_APP
    /* Length of Each Logical Address Block */
    #define LENGTH_OF_EACH_LAB              (512)    
    /* total number of logical blocks present */
    #define TOTAL_LOGICAL_ADDRESS_BLOCKS    (60)                                       
    /* Net Disk Size */
    #define DISK_SIZE                       (LENGTH_OF_EACH_LAB * TOTAL_LOGICAL_ADDRESS_BLOCKS)
#endif

#define LOGICAL_UNIT_SUPPORTED          (1)

#define SUPPORT_DISK_LOCKING_MECHANISM  (0) /*1: TRUE; 0:FALSE*/
/* If Implementing Disk Drive then configure the macro below as TRUE,
   otherwise keep it False(say for Hard Disk)*/
#define IMPLEMENTING_DISK_DRIVE         (0) /*1: TRUE; 0:FALSE*/

#define MSD_RECV_BUFFER_SIZE            (BULK_OUT_ENDP_PACKET_SIZE)
#define MSD_SEND_BUFFER_SIZE            (BULK_IN_ENDP_PACKET_SIZE)

/*****************************************************************************
 * Global variables
 *****************************************************************************/
 
/******************************************************************************
 * Types
 *****************************************************************************/
typedef struct _disk_variable_struct
{
    MSD_HANDLE app_handle;
    boolean start_app;
#if RAM_DISK_APP
    /* disk space reserved */
    uint_8 storage_disk[DISK_SIZE]; 
#endif  
    uint_8 disk_lock;   
}DISK_STRUCT, _PTR_ DISK_STRUCT_PTR; 

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
extern void TestApp_Init(void);
extern void Disk_App(void);

#endif


/* EOF */
