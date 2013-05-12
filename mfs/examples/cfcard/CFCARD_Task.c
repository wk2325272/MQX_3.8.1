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
* $FileName: CFCARD_Task.c$
* $Version : 3.8.5.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file is the main file for filesystem demo. Note that this example
*   is a multi tasking example and needs an operating system to run. This 
*   means that customers who are not using MQX should change the operating system
*   dependent code. An attempt has been made to comment out the code
*   however, a programmer must review all lines of code to ensure that
*   it correctly compiles with their libraries of operating system and
*   targetcompiler. This program has been compiled and tested for ARC AA3
*   processor with MQX real time operating system.
*
*END************************************************************************/


#include "CFCard_Task.h"
#include "CFCard_File.h"



/*FUNCTION*----------------------------------------------------------------
*
* Function Name  : CFCard_task
* Returned Value : None
* Comments       :
*     This rutine controls CompactFlash card insert and remove events
*     CompactFlash driver consists from two parts:
*             - lower layer (platform dependend) "pccarda:" driver can be open 
*               even if the CF card is not inserted
*             - upper layer (platform independend) "pcflasha:" driver should be 
*               opened only when card is present in the slot.Otherwise error is 
*               reported during fopen("pcflasha",..) function call.
*
*END*--------------------------------------------------------------------*/

void CFCard_task(uint_32 param)
{ 
   FS_STRUCT_PTR    CF_fs_handle = NULL;
   MQX_FILE_PTR     pccard_fp = NULL;
   uint_32          card_inserted;
   uint_32          device_state = CARD_DETACHED;

    /* open pccarda: driver*/
    pccard_fp = fopen("pccarda:", NULL);
    if ( pccard_fp == NULL ) 
    {
        printf("\nUnable to open pccard device \"pccarda:\"");
    }
    

    // there is no interrupt connected to pccard/pcflash driver, checking if the flash card was
    // removed has to be done in polling mode
    for ( ; ; ) 
    {
        // wait 200 ms
        _time_delay(200);
        
        if(device_state == CARD_DETACHED)
        {
            // check if card is inserted
            _io_ioctl(pccard_fp, APCCARD_IOCTL_IS_CARD_INSERTED, &card_inserted);
            if ((boolean)card_inserted == TRUE)  
            {
                // wait till instertion of card is completed and check once more
                _time_delay(300);
                // check if card is inserted
                _io_ioctl(pccard_fp, APCCARD_IOCTL_IS_CARD_INSERTED, &card_inserted);
                if ((boolean)card_inserted == TRUE) 
                {
                    // card is present in the slot - install filesystem
                    CF_fs_handle = CF_filesystem_install(pccard_fp, "PM_C2:", "d:" );
                    if(CF_fs_handle != NULL) 
                    {
                        device_state = CARD_INTERFACED;
                    }
                    else 
                    {
                        printf("Compact flash can not be initialized.\n");
                        device_state = CARD_DETACHED;
                    }
                }
            }
        }
        
         
        if(device_state == CARD_INTERFACED) 
        {
            
            // check if card is inserted - one of following IOCTL can be used here
            _io_ioctl(pccard_fp, APCCARD_IOCTL_IS_CARD_INSERTED, &card_inserted);
            //_io_ioctl(CF_fs_handle->DEV_FD_PTR, PCFLASH_IOCTL_IS_CARD_INSERTED, &card_inserted);
            if ((boolean)card_inserted == FALSE)  
            {
                // card is not present in the slot
                device_state = CARD_DETACHED;
                CF_filesystem_uninstall(CF_fs_handle);
            }
        }
    }
}

/* EOF */
