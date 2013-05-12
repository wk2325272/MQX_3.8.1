/**HEADER********************************************************************
* 
* Copyright (c) 2008-2011 Freescale Semiconductor;
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
* $FileName: httpdsrv.h$
* $Version : 3.8.4.0$
* $Date    : Sep-22-2011$
*
* Comments:
*
*   Configurable information for the RTCS dual webserver demo.
*
*END************************************************************************/
#ifndef _httpdsrv_h_
#define _httpdsrv_h_

#include <bsp.h>

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

#endif /* _httpdsrv_h_ */
