/**HEADER********************************************************************
* 
* Copyright (c) 2011 Freescale Semiconductor;
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
* $FileName: sh_audio.h$
* $Version : 
* $Date    : 
*
* Comments:
*
*   This file contains the definitions of constants and structures
*   required for playing and recording audio.
*
*END************************************************************************/
#ifndef __sh_audio_h_
#define __sh_audio_h_

#include <mqx.h>
#include <wav.h>
#include <fio.h>
#include <lwevent.h>
#include <message.h>
/* the write message queue */
#define WRITE_QUEUE 8
/* size of each record block */
#define REC_BLOCK_SIZE 512
/* number of initial messages in the pool */
#define MSG_NUM_INIT    10
/* block write ready event */
#define EV_BLOCK_W_READY        0x01
/* block write done event */
#define EV_BLOCK_W_DONE         0x01
#if (defined(__CWCC__)||defined(__GNUC__))
   #define ALIGN  __attribute__ ((packed))
#elif defined(__IAR_SYSTEMS_ICC__)||defined(__CC_ARM)
   #pragma pack(push)
   #pragma pack(1)
   #define ALIGN
#endif
/* Parameter structure required for SD write task */
typedef struct sd_write_param_struct
{
   /* Pointer to output file */
   MQX_FILE_PTR file_ptr;
   
   /* Flag indicating finished reading from device */
   boolean dev_read_done;
   
   /* Event pointer signaling finished writing to SD card */
   LWEVENT_STRUCT_PTR sdwrite_event_ptr;
   
}ALIGN SD_WRITE_PARAM_STRUCT;

/* This struct contains a data field and a message struct. */
typedef struct {
   /* Message header */ 
   MESSAGE_HEADER_STRUCT HEADER;
   
   /* Data */
   char DATA[REC_BLOCK_SIZE];
   
} REC_MESSAGE, _PTR_ REC_MESSAGE_PTR;

#if defined(__IAR_SYSTEMS_ICC__)
#pragma pack(pop)   /* Restore memory alignment configuration */
#endif
extern int_32 Shell_mute(int_32 argc, char_ptr argv[]);
extern int_32 Shell_record(int_32 argc, char_ptr argv[]);
extern void Sdcard_write_task(uint_32 temp);
#endif

/* EOF */
