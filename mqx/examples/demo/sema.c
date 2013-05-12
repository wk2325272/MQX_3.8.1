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
* $FileName: sema.c$
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

/*   Task Code -  SemA     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  SemA
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void SemA
   (
      uint_32   parameter
   )
{
   _mqx_uint  sem_result;
   pointer    Sem1_handle;

   /* create semaphore - sem.Sem1 */
   sem_result = _sem_create("sem.Sem1", 1, 0);
   if (sem_result != MQX_OK) { 
      /* semaphore sem.Sem1 not be created */
   } /* endif */
   /* open connection to semaphore sem.Sem1 */
   sem_result = _sem_open("sem.Sem1",&Sem1_handle);
   if (sem_result != MQX_OK) { 
      /* could not open sem.Sem1  */
   } /* endif */
   
   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /* wait for semaphore sem.Sem1 */
      sem_result = _sem_wait_ticks(Sem1_handle, NO_TIMEOUT);
      if (sem_result != MQX_OK) { 
         /* waiting on semaphore sem.Sem1 failed */
      }  
      /* semaphore obtained, perform work */
      _time_delay_ticks(1);
      /* semaphore protected work done, release semaphore */
      sem_result = _sem_post(Sem1_handle);
   } /* endwhile */ 
} /*end of task*/

/* End of File */
