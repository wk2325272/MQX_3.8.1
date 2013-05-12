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
* $FileName: wav.c$
* $Version : 
* $Date    : 
*
* Comments:
*
*   This file contains code for operations with WAVE files 
*
*END************************************************************************/
#include <mqx.h>
#include <fio.h>
#include "wav.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "audio_microphone.h"
/***************************************
**
** Global variables
****************************************/
extern USB_AUDIO_STREAM_DESC_FORMAT_TYPE_PTR   frm_type_desc;
/*FUNCTION****************************************************************
* 
* Function Name    : SetWaveHeader
* Returned Value   : Zero if everything is ok, non zero otherwise
* Comments         :
*    This function creates header according to format and writes it into
*    output file.
*
*END*********************************************************************/

_mqx_int SetWaveHeader
   (
    /* [OUT] header pointer to save header */
    WAVE_FILE_HEADER_PTR header, 
    
    /* [IN] file for writing */
    char_ptr filename,
    /* length in seconds*/
    char_ptr length
   )
{
   uint_32 tmp32 = 0;
   uint_16 tmp16 = 0;
   uint_32 vlength;
   FILE_PTR file_ptr;
   uchar def[] = {0, 0};
   /* chunk descriptor */
   WAVE_CHUNK_DESCRIPTOR_PTR ch_des = &(header->CHUNK_DESCRIPTOR);
   /* sub chunk */
   WAVE_FMT_SUBCHUNK_PTR fmt_sch = &(header->FMT_SUBCHUNK);
   /* data chunk */
   WAVE_DATA_SUBCHUNK_PTR data_sch = &(header->DATA_SUBCHUNK);
   /* open file to write */
   file_ptr = fopen(filename, "wb");
   if (NULL == file_ptr)
   {
      return(-1);
   }               
   /* Fill header */
   strcpy(ch_des->ChunkID, "RIFF");
   strcpy(ch_des->Format, "WAVE");
   strcpy(fmt_sch->Subchunk1ID, "fmt ");
   /* Sub chunk1 Size */
   fmt_sch->Subchunk1Size = 16;
   fmt_sch->AudioFormat = 1; /* PCM */
   /* Sample rate */
   fmt_sch->SampleRate = ((frm_type_desc->tSamFreq[2] << BYTE2_SHIFT) |
                          (frm_type_desc->tSamFreq[1] << BYTE1_SHIFT) |
                          (frm_type_desc->tSamFreq[0] << BYTE0_SHIFT)) ;
   /* Bits per sample */
   fmt_sch->BitsPerSample = frm_type_desc->bBitResolution;
   /* Number of Channels */
   fmt_sch->NumChannels = frm_type_desc->bNrChannels;
   fmt_sch->ByteRate = fmt_sch->SampleRate * fmt_sch->NumChannels *\
                       fmt_sch->BitsPerSample/8;
   /* Block Align*/
   fmt_sch->BlockAlign = fmt_sch->NumChannels * fmt_sch->BitsPerSample/8;
   strcpy(data_sch->Subchunk2ID, "data");
   /* Sub chunk2 Size*/
   vlength = atoi(length);
   data_sch->Subchunk2Size = vlength * fmt_sch->SampleRate * \
                             fmt_sch->NumChannels * fmt_sch->BitsPerSample/8;
   ch_des->ChunkSize = 4 + (8 + fmt_sch->Subchunk1Size) + (8 + data_sch->Subchunk2Size);
   
   /* Write header to file */
   /* Chunk descriptor */
   fwrite(&(ch_des->ChunkID), 1, 4, file_ptr);
   tmp32 = ch_des->ChunkSize;
   def[0] = sizeof(tmp32);
   swap_endian(def, &(tmp32));
   fwrite(&tmp32, 1, 4, file_ptr);
   fwrite(&(ch_des->Format), 1, 4, file_ptr);
   
   /* write format sub chunk */
   fwrite(&(fmt_sch->Subchunk1ID), 1, 4, file_ptr);
   /* write Sub chunk1 Size*/
   tmp32 = fmt_sch->Subchunk1Size;
   def[0] = sizeof(tmp32);
   swap_endian(def, &(tmp32));
   fwrite(&tmp32, 1, 4, file_ptr);
   /* write Audio Format*/
   tmp16 = fmt_sch->AudioFormat;
   def[0] = sizeof(tmp16);
   swap_endian(def, &(tmp16));
   fwrite(&tmp16, 1, 2, file_ptr);
   /* write Num Channels*/
   tmp16 = fmt_sch->NumChannels;
   def[0] = sizeof(tmp16);
   swap_endian(def, &(tmp16));
   fwrite(&tmp16, 1, 2, file_ptr);
   /* write Sample Rate*/
   tmp32 = fmt_sch->SampleRate;
   def[0] = sizeof(tmp32);
   swap_endian(def, &(tmp32));
   fwrite(&tmp32, 1, 4, file_ptr);
   /* write Byte Rate*/
   tmp32 = fmt_sch->ByteRate;
   def[0] = sizeof(tmp32);
   swap_endian(def, &(tmp32));
   fwrite(&tmp32, 1, 4, file_ptr);
   /* write Block Align*/
   tmp16 = fmt_sch->BlockAlign;
   def[0] = sizeof(tmp16);
   swap_endian(def, &(tmp16));
   fwrite(&tmp16, 1, 2, file_ptr);
   /* write Bits Per Sample*/
   tmp16 = fmt_sch->BitsPerSample;
   def[0] = sizeof(tmp16);
   swap_endian(def, &(tmp16));
   fwrite(&tmp16, 1, 2, file_ptr);
   
   /* data sub chunk */
   fwrite(&(data_sch->Subchunk2ID), 1, 4, file_ptr);
   /* Sub chunk2 Size*/
   tmp32 = data_sch->Subchunk2Size;
   def[0] = sizeof(tmp32);
   swap_endian(def, &(tmp32));
   fwrite(&tmp32, 1, 4, file_ptr);
   fclose(file_ptr);
   return(0);
}
/* EOF */
