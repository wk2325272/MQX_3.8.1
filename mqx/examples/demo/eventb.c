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
* $FileName: eventb.c$
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

/*   Task Code -  EventB     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  EventB
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void EventB
   (
      uint_32   parameter
   )
{
   _mqx_uint  event_result;
   pointer    Event1_handle;

   /* open connection to event event.Event1 */
   event_result = _event_open("event.Event1",&Event1_handle);
   if (event_result != MQX_OK) { 
      /* could not open event.Event1  */
   } /* endif */

   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /* wait on event event.Event1 */
      event_result = _event_wait_all_ticks(Event1_handle, 1, NO_TIMEOUT);
      if (event_result != MQX_OK) { 
         /* waiting on event event.Event1 failed */
      } 
         /* clear the event bits after processing event */
      event_result = _event_clear(Event1_handle, 1);
   } /* endwhile */ 
} /*end of task*/

/* End of File */
