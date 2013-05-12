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
* $FileName: lweventa.c$
* $Version : 3.0.3.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains source for the Lightweight MQX demo test.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
//#include <message.h>
#include <errno.h>
#include <lwevent.h>
#include "lwdemo.h"

/*   Task Code -  LWEventA     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  LWEventA
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void LWEventA
   (
      uint_32   parameter
   )
{
   _mqx_uint event_result;

   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /* set lwevent bit */
      event_result = _lwevent_set(&lwevent, 1);
      if (event_result != MQX_OK) {
         /* setting the event event.Event1 failed */
      } /* endif */
      _time_delay_ticks(1);
   } /* endwhile */ 
} /*end of task*/

/* End of File */
