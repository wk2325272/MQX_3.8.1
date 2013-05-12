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
* $FileName: email_demo.c$
* $Version : 3.8.21.0$
* $Date    : Oct-4-2011$
*
* Comments:
*
*   
*
*END************************************************************************/


#include <mqx.h>
#include <bsp.h>
#include "security_private.h"
#include "security_public.h"
#include "security.h"


#define MAIN_TASK   1

extern void Main_Task(uint_32 initial_data);

const TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority, Name,     Attributes,             Param,  Time Slice */
    { MAIN_TASK,    Main_Task,  2500,    7,      "Main",    MQX_AUTO_START_TASK,    0,      0           },
    {0}
};



void Main_Task(uint_32 initial_data)
{
  #ifdef BSP_TWRMCF51CN
      VMCF51CN_STRUCT_PTR reg_ptr = (VMCF51CN_STRUCT_PTR)BSP_IPSBAR;
  #endif
   
  printf("\n\n\n\nSecurity Email Demo\n");
   
  /* Create RTCS and connect to network */
  SEC_InitializeNetworking(3, 2, 2, DEMOCFG_ENABLE_DHCP);
    
  /* Contact SNTP server to get current time */
  SEC_GetTime();

  /* Set default configuration for security system */
  SEC_InitializeParameters();
  
  /* Initialize GPIO */
  if (!SEC_InitializeIO()) {
    printf("IO initialization failed ! END !!!\n");
    _task_block();
  }
  
  while (1) {

    do {
#ifdef SECEMAIL_TWRMCF51CN_STOP_ENABLED
        /* Go to sleep until SW2 or SW3 is pressed */
        reg_ptr->SIM.SOPT1 &= ~MCF51XX_SOPT1_WAITE_MASK;
        _ASM_STOP(0x2000);
        reg_ptr->SIM.SOPT1 |= MCF51XX_SOPT1_WAITE_MASK;
#endif
    } while (SEC_GetInput(SEC_DOOR_INPUT) == CLOSED && SEC_GetInput(SEC_WINDOW_INPUT) == CLOSED);

    _time_delay(1000);  // requested delay for "stabilization" after sleep - corrupted data on console (demo work correctly)

    /* Contact SNTP server to get the current time */
    SEC_GetTime(); 
    
    /* Send email alert */
    SEC_EmailAlert();    
    
    /*  Wait for push button to be released so don't keep sending emails */
    WaitForInputStop();  
  }
}
