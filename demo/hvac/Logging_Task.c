/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: Logging_Task.c$
* $Version : 3.8.9.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <string.h>
#include "hvac.h"
#include "logging_public.h"

#if DEMOCFG_ENABLE_AUTO_LOGGING

#include "logging_private.h"
#include "usb_file.h"

extern LWSEM_STRUCT  USB_Stick;
extern pointer       USB_handle;

_pool_id  log_pool=NULL;
LWSEM_STRUCT Logging_init_sem;


void LogInit(void)
{ 
   _lwsem_create(&Logging_init_sem,0);
   _task_create(0, LOGGING_TASK, 0); 
   _lwsem_wait(&Logging_init_sem);
}


void Log(char_ptr msg) 
{
   LOG_MESSAGE_PTR msg_ptr;

   if (log_pool) {
      msg_ptr = (LOG_MESSAGE_PTR)_msg_alloc(log_pool);

      if (msg_ptr != NULL) {
         msg_ptr->HEADER.TARGET_QID = _msgq_get_id(0, LOG_QUEUE);
         strncpy(msg_ptr->MESSAGE,msg,LOG_MESSAGE_SIZE);
         _msgq_send(msg_ptr);
      }
   }
}


void Logging_task(uint_32 param)
{
   _queue_id         log_qid;
   LOG_MESSAGE_PTR   msg_ptr;
#if DEMOCFG_ENABLE_USB_FILESYSTEM
  MQX_FILE_PTR      log_fp;
#endif
 
   // create a pool of logging messages   
   log_pool = _msgpool_create(sizeof(LOG_MESSAGE), LOG_POOL_SIZE, 0, 0);

   // open a message queue to receive log message on
   log_qid = _msgq_open(LOG_QUEUE, 0);
   
      
   // signal that initialization is complete
   _lwsem_post(&Logging_init_sem);

   while (TRUE) {
      // wait for a message
      msg_ptr = _msgq_receive(log_qid, 0);

      if (msg_ptr) {
         #if DEMOCFG_ENABLE_USB_FILESYSTEM
            // check to see if a filesystem is available
            if (_lwsem_poll(&USB_Stick)) {

               // Open the log file and position to the end
               log_fp = fopen(LOG_FILE,"a");

               if (log_fp) {
                  fseek(log_fp,0,IO_SEEK_END);
         
                  do {
                     // Write the message to the log file
                     write(log_fp,msg_ptr->MESSAGE, strlen(msg_ptr->MESSAGE));

                     // Return the message back to the message pool
                     _msg_free(msg_ptr);

                     // check for another message
                     msg_ptr = _msgq_poll(log_qid);
                  } while (msg_ptr != NULL);

                  // close the file
                  fclose(log_fp);
               }

               // Indicate that the USB stick is no longer in use
               _lwsem_post(&USB_Stick);
            } else {
               _msg_free(msg_ptr);
            }
         #else
            printf(msg_ptr->MESSAGE);
            _msg_free(msg_ptr);
         #endif
      }
   }
}

#else
void Log(char_ptr msg) 
{
}
#endif

/* EOF */
