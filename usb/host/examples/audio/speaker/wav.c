/**HEADER********************************************************************
*
* Copyright (c) 2010 Freescale Semiconductor;
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
_mqx_int FillWaveHeader(char_ptr filename, WAVE_FILE_HEADER_PTR header)
{
    FILE_PTR file;
    uchar def[] = {sizeof(header->CHUNK_DESCRIPTOR.ChunkSize), 0};
    /* CHUNK DESCRIPTOR */
    WAVE_CHUNK_DESCRIPTOR_PTR ch_des = &(header->CHUNK_DESCRIPTOR);
    /* FMT SUBCHUNK */
    WAVE_FMT_SUBCHUNK_PTR fmt_sch = &(header->FMT_SUBCHUNK);
    /* DATA SUBCHUNK */
    WAVE_DATA_SUBCHUNK_PTR data_sch = &(header->DATA_SUBCHUNK);
    file = fopen(filename, "r");
    if (file == NULL)
    {
        return(-1);
    }
    /* read Chunk ID */
    fread(&(ch_des->ChunkID), 1, 4, file);
    ch_des->ChunkID[4] = '\0';
    /* read  Chunk Size */
    fread(&(ch_des->ChunkSize), 1, 4, file);
    _mem_swap_endian(def, &(ch_des->ChunkSize));
    /* read  Chunk Format */
    fread(&(ch_des->Format), 1, 4, file);
    ch_des->Format[4] = '\0';
    /* read  Sub chunk1 ID */
    fread(&(fmt_sch->Subchunk1ID), 1, 4, file);
    fmt_sch->Subchunk1ID[4] = '\0';
    /* read  Sub chunk1 Size */
    fread(&(fmt_sch->Subchunk1Size), 1, 4, file);
    def[0] = sizeof(fmt_sch->Subchunk1Size);
    _mem_swap_endian(def, &(fmt_sch->Subchunk1Size));
    /* read  Audio Format */
    fread(&(fmt_sch->AudioFormat), 1, 2, file);
    def[0] = sizeof(fmt_sch->AudioFormat);
    _mem_swap_endian(def, &(fmt_sch->AudioFormat));
    /* read  Num Channels */
    fread(&(fmt_sch->NumChannels), 1, 2, file);
    def[0] = sizeof(fmt_sch->NumChannels);
    _mem_swap_endian(def, &(fmt_sch->NumChannels));
    /* read  Sample Rate */
    fread(&(fmt_sch->SampleRate), 1, 4, file);
    def[0] = sizeof(fmt_sch->SampleRate);
    _mem_swap_endian(def, &(fmt_sch->SampleRate));
    /* read  Byte Rate */
    fread(&(fmt_sch->ByteRate), 1, 4, file);
    def[0] = sizeof(fmt_sch->ByteRate);
    _mem_swap_endian(def, &(fmt_sch->ByteRate));
    /* read  Block Align */
    fread(&(fmt_sch->BlockAlign), 1, 2, file);
    def[0] = sizeof(fmt_sch->BlockAlign);
    _mem_swap_endian(def, &(fmt_sch->BlockAlign));
    /* read  Bits Per Sample*/
    fread(&(fmt_sch->BitsPerSample), 1, 2, file);
    def[0] = sizeof(fmt_sch->BitsPerSample);
    _mem_swap_endian(def, &(fmt_sch->BitsPerSample));
    /* read  Sub chunk2 ID*/
    fread(&(data_sch->Subchunk2ID), 1, 4, file);

    data_sch->Subchunk2ID[4] = '\0';
    /* read  Sub chunk2 Size*/
    fread(&(data_sch->Subchunk2Size), 1, 4, file);
    def[0] = sizeof(data_sch->Subchunk2Size);
    _mem_swap_endian(def, &(data_sch->Subchunk2Size));
    fclose(file);
    return(0);
}
/* EOF */
