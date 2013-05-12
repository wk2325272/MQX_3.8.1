/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
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
* $FileName: weighscale.h$
* $Version : 3.8.4.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
* @brief The file contains Macro's and functions needed by weighscale 
*        application
*
*****************************************************************************/


#ifndef _WEIGHSCALE_H
#define _WEIGHSCALE_H

#include "usb_phdc.h"
#include "phd_com_model.h"
/******************************************************************************
 * Constants - None
 *****************************************************************************/

/******************************************************************************
 * Macro's
 *****************************************************************************/
#if PSP_MQX_CPU_IS_MCF5445X
    #define ITERATION_COUNT    (14000000)/* this delay depends on SoC used*/
#else
    #define ITERATION_COUNT    (1400000)/* this delay depends on SoC used*/
#endif

#define SEND_MEASUREMENT   (0x02)
#define DISCONNECT         (0x04) 
#define NUM_OF_MSR_TO_SEND (10)

/*****************************************************************************
 * Types
 *****************************************************************************/
typedef struct _weighscale_variable_struct
{
    uint_32 app_handle;
    uint_8 event;
    PHD_MEASUREMENT msr;
    uint_8 num_of_msr_sent;
}WEIGHSCALE_GLOBAL_VARIABLE_STRUCT, _PTR_ PTR_WEIGHSCALE_GLOBAL_VARIABLE_STRUCT;
 /*****************************************************************************
 * Global variables - None
 *****************************************************************************/
  
/*****************************************************************************
 * Global Functions
 *****************************************************************************/
 extern void TestApp_Init(void);
 
 #endif
