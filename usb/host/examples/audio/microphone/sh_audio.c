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
* $Version : 
* $Date    : 
*
* Comments:
*
*   This file contains the MQX shell commands for audio devices.
*
*END************************************************************************/
#include "sh_audio.h"
#include "wav.h"
#include "stdlib.h"
#include <shell.h>
#include <string.h>
#include <lwevent.h>
#include "audio_microphone.h"
/***************************************
**
** Global variables
****************************************/
extern uint_8                                  wav_write_buff_tmp[MAX_ISO_PACKET_SIZE];
/* audio speaker or audio microphone */
extern char  device_direction;                 
extern ENDPOINT_DESCRIPTOR_PTR                 endp;
extern volatile AUDIO_CONTROL_DEVICE_STRUCT    audio_control;
extern volatile AUDIO_STREAM_DEVICE_STRUCT     audio_stream;
extern boolean                                 record_flag;
uint_32                                        remainder = 0;
boolean                                        final_packet = FALSE;
LWEVENT_STRUCT_PTR                             block_rec;

/***************************************
**
** Local variables
****************************************/
static AUDIO_COMMAND audio_com;
/*FUNCTION****************************************************************
* 
* Function Name    : Shell_play
* Returned Value   : SHELL_EXIT_SUCCESS if everything is ok,
*                    SHELL_EXIT_ERROR otherwise
* Comments         :
*    Shell function for recording wave files 
*
*END*********************************************************************/
int_32 Shell_record(int_32 argc, char_ptr argv[])
{
   boolean print_usage, shorthelp = FALSE;
   int_32 return_code = SHELL_EXIT_SUCCESS;
   uint_32 requests = 0;
   uint_32 count_cpy = 0;
   print_usage = Shell_check_help_request (argc, argv, &shorthelp);
   if (!print_usage)  
   {
      if (argc > 2)
      {
         WAVE_FILE_HEADER header;
         FILE_PTR file_ptr;
         _mqx_int errcode = 0, result = 0;
         uint_32 i = 0;    
         
         if ((USB_DEVICE_INTERFACED != audio_stream.DEV_STATE)||\
             (device_direction != OUT_DEVICE))
         {
             printf("  Error: Audio Microphone is not connected\n");
             return (SHELL_EXIT_ERROR);
         }
         if (((endp->bmAttributes>>2)&EP_TYPE_MASK)!=ISOCH_NOSYNC)
         {
             printf("  Error: This sync type of the device is not supported\n");
             return (SHELL_EXIT_ERROR);
         }
         /* Fill header struct according to input parameters */
         /* write header to file */
         errcode = SetWaveHeader(&header, argv[1],argv[2]);
         switch (errcode)
         {
            case -1:
               printf("  Error: Cannot create file %s.\n", argv[1]);
               return (SHELL_EXIT_ERROR);
               break;
               
            case -2:
               printf("  Error: Wrong format specified.\n");
               return (SHELL_EXIT_ERROR); 
               break;
               
            default:
               break;  
         }
         printf("Audio file properties:\n");
         printf("   - Sample rate      : %d Hz\n", \
                  header.FMT_SUBCHUNK.SampleRate);
         printf("   - Sample size      : %d bits\n", \
                  header.FMT_SUBCHUNK.BitsPerSample);
         printf("   - Number of channels : %d channels\n", \
                  header.FMT_SUBCHUNK.NumChannels);
         /* Open file with header and make it ready for data writing */
         file_ptr = fopen(argv[1], "ab");
         if (file_ptr == NULL)
         {
            printf("  Unable to open file: %s\n", argv[1]);
            fclose(file_ptr);
            return (SHELL_EXIT_ERROR);
         }
         fseek(file_ptr, WAVE_HEADER_SIZE, IO_SEEK_SET);     
           /* Compute requested number of data blocks */
         requests = header.DATA_SUBCHUNK.Subchunk2Size / REC_BLOCK_SIZE;
         remainder = header.DATA_SUBCHUNK.Subchunk2Size % REC_BLOCK_SIZE;
         
         block_rec = _mem_alloc_zero(sizeof(LWEVENT_STRUCT));
        
         result = _lwevent_create(block_rec, LWEVENT_AUTO_CLEAR);
         if (result != MQX_OK) {
            printf("  Error: Cannot create event required for waiting block "
                    "record ready.\n");
            fclose(file_ptr);
            _task_set_error(result);
            return (SHELL_EXIT_ERROR);
         }
         printf("  Recording...");
         record_flag = TRUE;
         _audio_timer_unmask_int(AUDIO_TIMER); 
         _time_delay (50);
         for(i = 0; i < requests; i++)
         {
        	 result = _lwevent_wait_ticks(block_rec, EV_BLOCK_W_READY, TRUE, 0);
            if (result != MQX_OK)
            {
               printf("  Error: Wait for event block record ready failed.\n");
                fclose(file_ptr);
                _task_set_error(result);
                return (SHELL_EXIT_ERROR);
            }
            result = fwrite(wav_write_buff_tmp, 1, REC_BLOCK_SIZE, file_ptr);
			if (result != REC_BLOCK_SIZE)
			{
				printf("  Error: Write failed. Wrote %d Bytes instead of %d.\n",\
						result, REC_BLOCK_SIZE);
			}
         }
         final_packet = TRUE;
         /* Alloc rest of data */
         if(remainder != 0)
         {          
			 result = _lwevent_wait_ticks(block_rec, EV_BLOCK_W_READY, TRUE, 0);
			 if (result != MQX_OK)
			 {
				printf("  Error: Wait for event block record ready failed.\n");
				 fclose(file_ptr);
				 _task_set_error(result);
				 return (SHELL_EXIT_ERROR);
			 }   
			 result = fwrite(wav_write_buff_tmp, 1, remainder, file_ptr);
			 if (result != remainder)
			 {
				printf("  Error: Write failed. Wrote %d Bytes instead of %d.\n",\
						result, remainder);
			 }
         }         
		 fclose(file_ptr);
		 result = _lwevent_destroy(block_rec);
		 if (MQX_OK != result)
		 {
			printf("Error: Unable to free sdwrite_event.\n");
		 }
		 if(NULL != block_rec)
			 _mem_free(block_rec);
         printf("done\n");
         record_flag = FALSE;
         _audio_timer_mask_int(AUDIO_TIMER); 
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
         printf("%s <filename> <length>\n", argv[0]);
      } 
      else  
      {
         printf("Usage: %s <filename> <length>\n", argv[0]);
         printf("   filename      = filename for recording\n");
         printf("   length        = length in seconds\n");
      }
   }
   return(return_code);
}
/******************************************************************************
*   @name        Shell_mute
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
            if ((USB_DEVICE_INTERFACED != audio_stream.DEV_STATE)||\
                (device_direction != OUT_DEVICE))
            {
                printf("  Error: Audio Microphone is not connected\n");
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
            if(record_flag)
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
