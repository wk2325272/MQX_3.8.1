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
* $FileName: sh_audio.c$
* $Version : 3.8.1.2$
* $Date    : May-21-2012$
*
* Comments:
*
*   This file contains the MQX shell commands for audio devices.
*
*END************************************************************************/
#include <string.h>

#include <mqx.h>

#include <shell.h>
#include "sh_audio.h"

#include <usb.h>
#include <usb_types.h>
#include <host_common.h>
#include <usb_host_audio.h>

#include "wav.h"
#include "audio_speaker.h"
#include "audio_timer.h"

/******************************************************************************
* Global variables
*****************************************************************************/
FILE_PTR file_ptr;
extern boolean play_flag;   /* a file is selected */
boolean playing = FALSE;    /* a file is playing */
extern volatile AUDIO_CONTROL_DEVICE_STRUCT    audio_control;
extern volatile AUDIO_STREAM_DEVICE_STRUCT audio_stream;
extern char  device_direction;  /* audio speaker or audio microphone */
extern USB_AUDIO_STREAM_DESC_FORMAT_TYPE_PTR   frm_type_desc;
extern ENDPOINT_DESCRIPTOR_PTR                 endp;

/******************************************************************************
* Local variables
*****************************************************************************/
static AUDIO_COMMAND audio_com;

/******************************************************************************
*   @name        Shell_play
*
*   @brief       Servers the play command
*
*   @param       None
*
*   @return      None
*
******************************************************************************
* This function is used to play an audio wav file
*****************************************************************************/
int_32 Shell_play(int_32 argc, char_ptr argv[])
{
    boolean print_usage, shorthelp = FALSE;
    print_usage = Shell_check_help_request (argc, argv, &shorthelp);

    if (!print_usage)
    {
        WAVE_FILE_HEADER header;
        FILE_PTR device_ptr;
        uint_16 block_size = 512;
        char_ptr data;
        uint_32 requested = 0;

        if (argc > 1)
        {
            /* stop the current file playing */
            if(play_flag)
            {
                play_flag = FALSE;
                playing = play_flag;
                _audio_timer_mask_int(AUDIO_TIMER);
            }
            /* check the device is connected */
            if ((USB_DEVICE_INTERFACED != audio_stream.DEV_STATE)||(device_direction != IN_DEVICE))
            {
                printf("  Error: Audio Speaker is not connected\n");
                return (SHELL_EXIT_ERROR);
            }
            if (((endp->bmAttributes>>2)&EP_TYPE_MASK)!=ISOCH_NOSYNC)
            {
                printf("  Error: This sync type of the device is not supported\n");
                return (SHELL_EXIT_ERROR);
            }
            if (FillWaveHeader(argv[1], &header) != 0)
            {
                printf("  Error: Unable to open file: %s\n", argv[1]);
                return (SHELL_EXIT_ERROR);
            }
            if (strcmp(header.CHUNK_DESCRIPTOR.Format, "WAVE"))
            {
                printf("  Error: File is not WAVE file.\n");
                return (SHELL_EXIT_ERROR);
            }
            if (strcmp(header.FMT_SUBCHUNK.Subchunk1ID, "fmt "))
            {
                printf("  Error: File does not contain format subchunk.\n");
            }
            if (BYTESWAP16(header.FMT_SUBCHUNK.AudioFormat) != 1)
            {
                printf("  Error: File is compressed (not PCM).\n");
                return (SHELL_EXIT_ERROR);
            }
            if (strcmp(header.DATA_SUBCHUNK.Subchunk2ID, "data"))
            {
                printf("  Error: File does not contain data subchunk.\n");
                return (SHELL_EXIT_ERROR);
            }
            file_ptr = fopen(argv[1], "r");
            if (file_ptr == NULL)
            {
                printf("  Unable to open file: %s\n", argv[1]);
                return (SHELL_EXIT_ERROR);
            }
            printf("Audio file properties:\n");
            printf("   - Sample rate      : %d Hz\n", BYTESWAP32(header.FMT_SUBCHUNK.SampleRate));
            printf("   - Sample size      : %d bits\n", BYTESWAP16(header.FMT_SUBCHUNK.BitsPerSample));
            printf("   - Number of channels : %d channels\n", BYTESWAP16(header.FMT_SUBCHUNK.NumChannels));
            /* Compare the sample rate */
            if (((frm_type_desc->tSamFreq[2] << 16) |
                        (frm_type_desc->tSamFreq[1] << 8)  |
                        (frm_type_desc->tSamFreq[0] << 0)) != BYTESWAP32(header.FMT_SUBCHUNK.SampleRate))
            {
                printf("  The audio device doesn't support that audio sample rate \n");
                return (SHELL_EXIT_ERROR);
            }
            /* Compare the bits sample number */
            if (frm_type_desc->bBitResolution != BYTESWAP16(header.FMT_SUBCHUNK.BitsPerSample))
            {
                printf("  The audio device doesn't support that audio bit sample number \n");
                return (SHELL_EXIT_ERROR);
            }
            /* Compare the channel number */
            if (frm_type_desc->bNrChannels != BYTESWAP16(header.FMT_SUBCHUNK.NumChannels))
            {
                printf("  The audio device doesn't support that audio channel number \n");
                return (SHELL_EXIT_ERROR);
            }
            fseek(file_ptr, WAVE_HEADER_SIZE, IO_SEEK_SET);
            play_flag = TRUE;
            playing = play_flag;
            printf("  Playing...\n");
            _audio_timer_unmask_int(AUDIO_TIMER);
        }
        else
        {
            printf("  Not enough parameters.\n");
        }
    }
    if (print_usage)
    {
        if (shorthelp)
        {
            printf("%s <filename>\n", argv[0]);
        }
        else
        {
            printf("Usage: %s <filename>\n", argv[0]);
            printf("   filename      = wav file to play\n");
        }
    }
    return(SHELL_EXIT_SUCCESS);
}
/******************************************************************************
*   @name        Shell_play
*
*   @brief       Servers the pause command
*
*   @param       None
*
*   @return      None
*
******************************************************************************
* This function is used to pause the current playing file
*****************************************************************************/
int_32 Shell_pause(int_32 argc, char_ptr argv[])
{
    boolean print_usage, shorthelp = FALSE;
    print_usage = Shell_check_help_request (argc, argv, &shorthelp);

    if (!print_usage)
    {
        if (argc > 1)
        {
            printf("  Error: This command doesn't need parameters\n");
            return (SHELL_EXIT_ERROR);
        }
        else
        {
            if(play_flag)
            {
                playing = !playing;
                if(playing)
                {
                    _audio_timer_unmask_int(AUDIO_TIMER);
                    printf("  Playing...\n");
                }
                else
                {
                    printf("  Paused...\n");
                    _audio_timer_mask_int(AUDIO_TIMER);
                }
            }
            else
            printf("  No file is playing!\n");
        }
    }

    if (print_usage)
    {
        if (shorthelp)
        {
            printf("%s\n", argv[0]);
        }
        else
        {
            printf("Usage: %s", argv[0]);
        }
    }
    return(SHELL_EXIT_SUCCESS);
}

/******************************************************************************
*   @name        Shell_play
*
*   @brief       Servers the mute command
*
*   @param       None
*
*   @return      None
*
******************************************************************************
* This function is used to mute the audio device
*****************************************************************************/
int_32 Shell_mute(int_32 argc, char_ptr argv[])
{
    boolean         print_usage, shorthelp = FALSE;
    static boolean  mute = FALSE;
    print_usage = Shell_check_help_request (argc, argv, &shorthelp);

    if (!print_usage)
    {
        if (argc > 1)
        {
            printf("  Error: This command doesn't need parameters\n");
            return (SHELL_EXIT_ERROR);
        }
        else
        {
            if ((USB_DEVICE_INTERFACED != audio_stream.DEV_STATE)||(device_direction != IN_DEVICE))
            {
                printf("  Error: Audio Speaker is not connected\n");
                return (SHELL_EXIT_ERROR);
            }
            mute = !mute;
            if (FALSE == mute)
            {
                printf("UnMute ...\n");
            }
            else
            {
                printf("Mute ...\n");
            }
            if(play_flag)
            {
                _audio_timer_mask_int(AUDIO_TIMER);
            }
            /* Send set mute request */
            audio_com.CLASS_PTR = (CLASS_CALL_STRUCT_PTR)&audio_control.CLASS_INTF;
            audio_com.CALLBACK_FN = usb_host_audio_mute_ctrl_callback;
            audio_com.CALLBACK_PARAM = 0;
            usb_class_audio_set_cur_mute(&audio_com,(pointer)&mute);
        }
    }

    if (print_usage)
    {
        if (shorthelp)
        {
            printf("%s\n", argv[0]);
        }
        else
        {
            printf("Usage: %s", argv[0]);
        }
    }
    return(SHELL_EXIT_SUCCESS);
}
/* EOF */
