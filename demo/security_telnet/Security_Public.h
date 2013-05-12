#ifndef __security_public_h__
#define __security_public_h__

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
* $FileName: Security_Public.h$
* $Version : 3.8.9.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   
*
*END************************************************************************/
#include <stdlib.h>
#include <mqx.h>


#define OPENED                1
#define CLOSED                0

#define MOVING                1
#define STOPPED               0

void SEC_SetDoorOpened(void);
void SEC_SetDoorClosed(void);
void SEC_SetWindowOpened(void);
void SEC_SetWindowClosed(void);
uint_32 SEC_GetDoorStatus(void);
uint_32 SEC_GetWindowStatus(void);
uint_32 SEC_GetMovementStatus(void);
_mqx_int SEC_GetGarageStatus(void);
_mqx_int SEC_DetectMotion(void);
void SEC_SetMovementStopped(void);
void SEC_SetMovementStarted(void);
void SEC_WaterLevel(void);

extern void SEC_Initialize(void);
extern void SEC_InitializeNetworking(uint_32 pcbs, uint_32 msgs, uint_32 sockets, boolean dhcp) ;
extern boolean SEC_GetTime(void); 

#endif
