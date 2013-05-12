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
* $FileName: lwsema.c$
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

/*   Task Code -  LWSemA     */


/*TASK---------------------------------------------------------------
*   
* Task Name   :  LWSemA
* Comments    : 
* 
*END*--------------------------------------------------------------*/

void LWSemA
   (
      uint_32   parameter
   )
{
   _mqx_uint sem_result;

   /* 
   ** LOOP - 
   */
   while ( TRUE ) {
      /* wait for lw semaphore until it is available */      
      sem_result = _lwsem_wait_ticks(&lwsem, NO_TIMEOUT);
      if (sem_result != MQX_OK) { 
         /* waiting on semaphore sem.Sem1 failed */
      }  
      /* semaphore obtained, perform work */
      _time_delay_ticks(1);
      /* semaphore protected work done, release semaphore */
      sem_result = _lwsem_post(&lwsem);
   } /* endwhile */ 
} /*end of task*/

/* End of File */
