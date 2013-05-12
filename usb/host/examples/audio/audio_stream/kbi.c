/******************************************************************************
 *
 * Freescale Semiconductor Inc.
 * (c) Copyright 2004-2010 Freescale Semiconductor, Inc.
 * ALL RIGHTS RESERVED.
 *
 ******************************************************************************
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
 **************************************************************************//*!
 *
 * @file kbi.c
 *
 * @author
 *
 * @version
 *
 * @date
 *
 * @brief
 *
 *****************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/
#include <string.h>
#include <mqx.h>
#include <usb_types.h>
#include "audio.h"
#include "audio_timer.h"
#include "kbi.h"

/******************************************************************************
 * Local functions
 *****************************************************************************/
static void process_play_button(void);

/******************************************************************************
 * Global variables
 *****************************************************************************/
extern boolean play;
extern uint_8 mute_flag;
extern LWEVENT_STRUCT USB_Event;
extern volatile AUDIO_CONTROL_DEVICE_STRUCT  audio_stream;

#ifdef BSP_BUTTON1
LWGPIO_STRUCT btn1;
#define SW1_EVENT_MASK         (1 << 0)
#endif

#ifdef BSP_BUTTON2
LWGPIO_STRUCT btn2;
#define SW2_EVENT_MASK         (1 << 1)
#endif

/******************************************************************************
*   @name        process_play_button
*
*   @brief       process play button
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
static void process_play_button(void)
{
   if(USB_DEVICE_INTERFACED == audio_stream.DEV_STATE)
   {
      play = 1 - play;
      /* play */
      if (TRUE == play)
      {
         printf("Playing ...\n");
         _lwevent_set(&USB_Event, USB_EVENT_RECEIVED_DATA);
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
         EnableCmtInterrupt();
#elif (defined BSP_TWRMCF51JF)
         EnableFTM0Interrupt();
#else
         _audio_timer_unmask_int(BSP_LAST_TIMER);
#endif
      }
      /* stop */
      else
      {
         printf("\nPaused.\n");
#if (defined BSP_TWRMCF51JE) || (defined BSP_TWRMCF51MM)||(defined BSP_MCF51JMEVB)
         DisableCmtInterrupt();
#elif (defined BSP_TWRMCF51JF)
         DisableFTM0Interrupt();
#else
         _audio_timer_mask_int(BSP_LAST_TIMER);
#endif
      }
   }
}

/******************************************************************************
*   @name        GPIO_Init
*
*   @brief       This function init GPIO
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void switch_init(void)
{
#ifdef BSP_BUTTON1
   /* opening pins/signals for input */
   if (!lwgpio_init(&btn1, BSP_BUTTON1, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE))
   {
      puts("\nSW initialization failed.\n");
      _task_block();
   }

   lwgpio_set_functionality(&btn1, BSP_BUTTON1_MUX_GPIO);
   lwgpio_set_attribute(&btn1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
#endif
#ifdef BSP_BUTTON2
   /* opening pins/signals for input */
   if (!lwgpio_init(&btn2, BSP_BUTTON2, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE))
   {
      puts("\nSW initialization failed.\n");
      _task_block();
   }

   lwgpio_set_functionality(&btn2, BSP_BUTTON2_MUX_GPIO);
   lwgpio_set_attribute(&btn2, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
#endif
}

/******************************************************************************
*   @name        polling_switch
*
*   @brief       This function poll the button
*
*   @return      None
*
*   @comment
*
*******************************************************************************/
void polling_switch (void)
{
   static uint_32 switch_state = 1;
#ifdef BSP_BUTTON1
   /* Test if SW1 was pressed */
   if (LWGPIO_VALUE_LOW == lwgpio_get_value(&btn1))    {
      if ((switch_state & SW1_EVENT_MASK) == 0)  {
         switch_state |= SW1_EVENT_MASK;
         mute_flag = !mute_flag;
      }
   }
   else
   {
      switch_state &= ~SW1_EVENT_MASK;
   } 
#endif
#ifdef BSP_BUTTON2
   /* Test if SW2 was pressed */
   if (LWGPIO_VALUE_LOW == lwgpio_get_value(&btn2))    {
      if ((switch_state & SW2_EVENT_MASK) == 0)  {
         switch_state |= SW2_EVENT_MASK;
         process_play_button();
      }
   }
   else
   {
      switch_state &= ~SW2_EVENT_MASK;
   }
#endif
}
/* EOF */
