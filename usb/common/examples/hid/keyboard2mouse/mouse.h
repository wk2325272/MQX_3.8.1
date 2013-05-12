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
* $FileName: mouse.h$
* $Version : 3.8.0.1$
* $Date    : May-3-2012$
*
* Comments:
*
* @brief The file contains Macro's and functions needed by the mouse 
*        application
*
*****************************************************************************/
#ifndef _MOUSE_H
#define _MOUSE_H

#include "usb_descriptor.h"

/******************************************************************************
 * Constants - None
 *****************************************************************************/
enum direction {
    RIGHT,
    DOWN,
    LEFT,
    UP
};

/******************************************************************************
 * Macro's
 *****************************************************************************/
#define  MOUSE_BUFF_SIZE   (4)   /* report buffer size */
#define  REQ_DATA_SIZE     (1)

#define COMPLIANCE_TESTING    (0)/*1:TRUE, 0:FALSE*/
/******************************************************************************
 * Types
 *****************************************************************************/
typedef struct _mouse_variable_struct
{
    HID_HANDLE app_handle;
    boolean mouse_init;/* flag to check lower layer status*/
    uint_8 rpt_buf[MOUSE_BUFF_SIZE];/*report/data buff for mouse application*/
    USB_CLASS_HID_ENDPOINT ep[HID_DESC_ENDPOINT_COUNT];
    uint_8 app_request_params[2]; /* for get/set idle and protocol requests*/
}MOUSE_GLOBAL_VARIABLE_STRUCT, _PTR_ PTR_MOUSE_GLOBAL_VARIABLE_STRUCT; 

/*****************************************************************************
 * Global variables
 *****************************************************************************/

/*****************************************************************************
 * Global Functions
 *****************************************************************************/
void MouseDev_Init(void);
void move_mouse(enum direction);

#endif 

/* EOF */
