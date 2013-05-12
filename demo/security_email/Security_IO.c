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
* $FileName: Security_IO.c$
* $Version : 3.8.21.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#include "security_public.h"
#include "security_private.h"
#include <bsp.h>
#include <lwgpio.h>
#include <string.h>

#if defined(BSP_BUTTON1)
   #define DOOR_STATE      BSP_BUTTON1
#endif
#if defined(BSP_BUTTON2)
   #define WINDOW_STATE    BSP_BUTTON2
#endif
#if (!defined BSP_BUTTON1 && !defined BSP_BUTTON2)
   #warning "Buttons are not available in this BSP"
#endif 

#ifdef BSP_LED1
   #define LED_1       BSP_LED1
#endif
#ifdef BSP_LED2
   #define LED_2       BSP_LED2
#endif
#ifdef BSP_LED3
   #define LED_3       BSP_LED3
#endif
#ifdef BSP_LED4
   #define LED_4       BSP_LED4
#endif

const char_ptr SEC_OutputName[] = { "LED1","LED2","Entry Open","Movement Detected" };
boolean SEC_OutputState[SEC_MAX_OUTPUTS] ={0};
static boolean input_port=0, output_port=0;
#ifdef LED_1
LWGPIO_STRUCT led1;
#endif
#ifdef LED_2
LWGPIO_STRUCT led2;
#endif
#ifdef LED_3
LWGPIO_STRUCT led3;
#endif
#ifdef LED_4
LWGPIO_STRUCT led4;
#endif
#ifdef BSP_BUTTON1
LWGPIO_STRUCT button1;
#endif
#ifdef BSP_BUTTON2
LWGPIO_STRUCT button2;
#endif
static void kbi_callback(pointer kbi_param_ptr);

static void kbi_callback(pointer kbi_param_ptr) 
{
   
   /* Determine if it was SW2 or SW3 that caused interrupt */
    if (SEC_GetInput(SEC_DOOR_INPUT) == OPENED) {    	
        SEC_Params.Status = SEC_DOOR_OPEN_STATUS;
        SEC_SetOutput(SEC_OPEN_OUTPUT, 1); 
    }      
    if (SEC_GetInput(SEC_WINDOW_INPUT) == OPENED) {    
        SEC_Params.Status = SEC_WINDOW_OPEN_STATUS;
        SEC_SetOutput(SEC_OPEN_OUTPUT, 1); 
    } 
    lwgpio_int_clear_flag((LWGPIO_STRUCT_PTR) kbi_param_ptr);
}

boolean SEC_InitializeIO(void)
{

    /* Init Gpio for Leds as output to drive LEDs (LED10 - LED13) */
#ifdef LED_1
       output_port = lwgpio_init(&led1, LED_1, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED1 failed.\n");
       }
       lwgpio_set_functionality(&led1, BSP_LED1_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led1, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_2
       output_port = lwgpio_init(&led2, LED_2, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED2 failed.\n");
       }
       lwgpio_set_functionality(&led2, BSP_LED2_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led2, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_3
       output_port = lwgpio_init(&led3, LED_3, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED3 failed.\n");
       }
       lwgpio_set_functionality(&led3, BSP_LED3_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led3, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_4
       output_port = lwgpio_init(&led4, LED_4, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
       if(!output_port){
          printf("Initializing LWGPIO for LED3 failed.\n");
       }
       lwgpio_set_functionality(&led4, BSP_LED4_MUX_GPIO);
       /*Turn off Led */
       lwgpio_set_value(&led4, LWGPIO_VALUE_LOW);
#endif

#ifdef BSP_BUTTON1
    /* Open and set port DD as input to read value from switches */
       input_port = lwgpio_init(&button1, DOOR_STATE, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE);
       if(!input_port)
       {
           printf("Initializing LW GPIO for button1 as input failed.\n");
           _task_block();
       }    
       #if (defined BSP_BUTTON1_MUX_IRQ && defined SECEMAIL_TWRMCF51CN_STOP_ENABLED)
       lwgpio_set_functionality(&button1 ,BSP_BUTTON1_MUX_IRQ);
       lwgpio_set_attribute(&button1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
       if (!lwgpio_int_init(&button1, LWGPIO_INT_MODE_RISING))
       {
           printf("Initializing button GPIO for interrupt failed.\n");
       }
       #else 
       lwgpio_set_functionality(&button1 ,BSP_BUTTON1_MUX_GPIO);
       lwgpio_set_attribute(&button1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
       #endif
       /* install gpio interrupt service routine */
       _int_install_isr(lwgpio_int_get_vector(&button1), kbi_callback, (void *) &button1);
       _bsp_int_init(lwgpio_int_get_vector(&button1), 3, 0, TRUE);
       lwgpio_int_enable(&button1, TRUE);
#endif
       
#ifdef BSP_BUTTON2
       input_port = lwgpio_init(&button2, WINDOW_STATE, LWGPIO_DIR_INPUT, LWGPIO_VALUE_NOCHANGE);
       if(!input_port)
       {
           printf("Initializing LW GPIO for button2 as input failed.\n");
           _task_block();
       }
       #if (defined BSP_BUTTON2_MUX_IRQ && defined SECEMAIL_TWRMCF51CN_STOP_ENABLED)
       lwgpio_set_functionality(&button2 ,BSP_BUTTON2_MUX_IRQ);
       lwgpio_set_attribute(&button2, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
       if (!lwgpio_int_init(&button2, LWGPIO_INT_MODE_FALLING))
       {
           printf("Initializing button GPIO for interrupt failed.\n");
       }
       #else 
       lwgpio_set_functionality(&button2 ,BSP_BUTTON2_MUX_GPIO);
       lwgpio_set_attribute(&button2, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
       #endif
       /* install gpio interrupt service routine */
       _int_install_isr(lwgpio_int_get_vector(&button2), kbi_callback, (void *) &button2);
       _bsp_int_init(lwgpio_int_get_vector(&button2), 3, 0, TRUE);
       lwgpio_int_enable(&button2, TRUE);
#endif

    return (input_port!=0) && (output_port!=0);
}
     
      
void SEC_ResetOutputs(void) 
{
   uint_32 i;
   
   for (i=0;i<SEC_MAX_OUTPUTS;i++) {
      SEC_SetOutput(i,FALSE);
   }
}

  
void SEC_SetOutput(SEC_Output_t signal,boolean state) 
{
    
   if (SEC_OutputState[signal] != state) {
      SEC_OutputState[signal] = state;

      if (output_port) {
         switch (signal) {
#ifdef LED_1
             case SEC_LED1_OUTPUT:
                (state) ? lwgpio_set_value(&led1, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led1, LWGPIO_VALUE_LOW);            	
                break;
#endif
#ifdef LED_2
             case SEC_LED2_OUTPUT:
                (state) ? lwgpio_set_value(&led2, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led2, LWGPIO_VALUE_LOW);            	
                break;
#endif
#ifdef LED_3
             case SEC_OPEN_OUTPUT:
                (state) ? lwgpio_set_value(&led3, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led3, LWGPIO_VALUE_LOW);            	
                break;
#endif
#ifdef LED_4
             case SEC_MOVEMENT_OUTPUT:
                (state) ? lwgpio_set_value(&led4, LWGPIO_VALUE_HIGH):lwgpio_set_value(&led4, LWGPIO_VALUE_LOW);            	
                break;
#endif
         }
      }

   }
}
  
   
boolean SEC_GetInput(SEC_Input_t signal)
{
 
   boolean  value=FALSE;    

   if (input_port) {
      switch (signal) {
#ifdef BSP_BUTTON1
         case SEC_DOOR_INPUT:
#ifdef SECEMAIL_TWRMCF51CN_STOP_ENABLED
            lwgpio_set_functionality(&button1, BSP_BUTTON1_MUX_GPIO); /*Switch function as GPIO polling*/
            lwgpio_set_attribute(&button1, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
#endif
            value = !lwgpio_get_value(&button1);
            break;
#endif
#ifdef BSP_BUTTON2
         case SEC_WINDOW_INPUT:
#ifdef SECEMAIL_TWRMCF51CN_STOP_ENABLED
            lwgpio_set_functionality(&button2, BSP_BUTTON2_MUX_GPIO);/*Switch function as GPIO polling*/
            lwgpio_set_attribute(&button2, LWGPIO_ATTR_PULL_UP, LWGPIO_AVAL_ENABLE);
#endif
            value = !lwgpio_get_value(&button2);
            break;
#endif
      }
   }
   return value;
}
 
/* EOF */
