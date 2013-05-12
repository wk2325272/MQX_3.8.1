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
* $FileName: eventa.c$
* $Version : 3.8.4.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <message.h>
#include <errno.h>
#include <mutex.h>
#include <sem.h>
#include <event.h>
#include <log.h>
#include "demo.h"

/*   Task Code -  EventA     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  EventA
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void EventA
   (
      uint_32   parameter
   )
{
   _mqx_uint event_result;
   pointer   Event1_handle;

   /* create event - event.Event1 */
   event_result = _event_create("event.Event1");
   if (event_result != MQX_OK) { 
      /* event event.Event1 not be created */
   } /* endif */
   /* open connection to event event.Event1 */
   event_result = _event_open("event.Event1",&Event1_handle);
   if (event_result != MQX_OK) { 
      /* could not open event.Event1  */
   } /* endif */
   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /* set event event.Event1 */
      event_result = _event_set(Event1_handle, 1);
      if (event_result != MQX_OK) { 
         /* setting the event event.Event1 failed */
      } /* endif */
      _time_delay_ticks(1);
   } /* endwhile */ 
} /*end of task*/

/* End of File */
