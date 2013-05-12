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
* $FileName: Security_Util.c$
* $Version : 3.6.4.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   
*
*END************************************************************************/

#include "security_public.h"
#include "security_private.h"
#include <string.h>
#include <bsp.h>

SEC_PARAMS SEC_Params = {0};

void SEC_InitializeParameters(void) 
{
   SEC_Params.Status=SEC_DEFAULT_STATUS;
}

void WaitForInputStop(void) 
{ 
  /* Wait for SW2 to rise */
  while(SEC_GetInput(SEC_DOOR_INPUT)==OPENED) 
    {_ASM_NOP();}
    
  /* Wait for SW3 to rise */  
  while(SEC_GetInput(SEC_WINDOW_INPUT)==OPENED) 
    {_ASM_NOP();}
  
  /* Turn off LED */  
  SEC_SetOutput(SEC_OPEN_OUTPUT,0); 
  
  /* All inputs are back to normal */  
  SEC_Params.Status=SEC_DEFAULT_STATUS; 
}
