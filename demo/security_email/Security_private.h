#ifndef __security_private_h__
#define __security_private_h__

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
* $FileName: Security_private.h$
* $Version : 3.8.7.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   
*
*END************************************************************************/
#include <stdlib.h>
#include <mqx.h>

#define MAX_QUEUE            10

#define SMTP_BUFFER_SIZE 512

#if defined(BSP_TWRMCF51CN)
#if defined(__CWBUILD__)
#if  (__CWBUILD__ < 3003)
   #define SECEMAIL_TWRMCF51CN_STOP_ENABLED
#else
   #warning Stop instruction causes OSBDM connection loss in CW10.  
#endif
#endif
#endif

typedef enum {
   SEC_DEFAULT_STATUS=0,
   SEC_DOOR_OPEN_STATUS,
   SEC_WINDOW_OPEN_STATUS
} SEC_Status_t;

typedef enum {
   SEC_LED1_OUTPUT=0,
   SEC_LED2_OUTPUT,
   SEC_OPEN_OUTPUT,
   SEC_MOVEMENT_OUTPUT,
   SEC_MAX_OUTPUTS
} SEC_Output_t;

typedef enum {
   SEC_DOOR_INPUT=0,
   SEC_WINDOW_INPUT,
   SEC_MAX_INPUTS
} SEC_Input_t;


typedef struct {
   SEC_Status_t         Status;
   uchar                xdefault;
   uchar                ydefault;
   uchar                zdefault;
} SEC_PARAMS, * SEC_PARAMS_PTR;

extern SEC_PARAMS SEC_Params;

extern void SEC_InitializeKBI(void);
extern void SEC_InitializeParameters(void); 
extern boolean SEC_InitializeIO(void);

extern void WaitForInputStop(void);

extern void SEC_ResetOutputs(void);
extern void SEC_SetOutput(SEC_Output_t,boolean);
extern boolean SEC_GetInput(SEC_Input_t);

extern char * base64_encode(char *source, char *destination);
extern void base64_encodeblock( unsigned char in[3], unsigned char out[4], int len );

void kbi_isr(pointer dummy);

#endif
