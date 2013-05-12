/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: lwt_cncp.c$
* $Version : 3.0.4.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file contains the function for canceling a timer period.
*
*END************************************************************************/

#include "mqx_inc.h"
#if MQX_USE_LWTIMER
#include "lwtimer.h"
#include "lwtimprv.h"

/*FUNCTION*------------------------------------------------------------
*
* Function Name   : _lwtimer_cancel_period
* Returned Value  : _mqx_uint MQX error code
* Comments        : 
*  This function will cancel an entire timer period.
*
*END*------------------------------------------------------------------*/

_mqx_uint _lwtimer_cancel_period
   (
      /* [IN] the timer period to cancel */
      LWTIMER_PERIOD_STRUCT_PTR period_ptr
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR    kernel_data;

   _GET_KERNEL_DATA(kernel_data);
   _KLOGE2(KLOG_lwtimer_cancel_period, period_ptr);

#if MQX_CHECK_VALIDITY
   if (period_ptr->VALID != LWTIMER_VALID) {
      _KLOGX2(KLOG_lwtimer_cancel_period, MQX_LWTIMER_INVALID);
      return MQX_LWTIMER_INVALID;
   } /* Endif */
#endif

   _int_disable();
   period_ptr->VALID = 0;
   _QUEUE_REMOVE(&kernel_data->LWTIMERS, period_ptr);
   _int_enable();

   _KLOGX2(KLOG_lwtimer_cancel_period, MQX_OK);
   return(MQX_OK);

} /* Endbody */
#endif /* MQX_USE_LWTIMER */

/* EOF */

