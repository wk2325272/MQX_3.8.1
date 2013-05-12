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
* $FileName: ti_setr.c$
* $Version : 3.5.4.0$
* $Date    : Dec-8-2009$
*
* Comments:
*
*   This file contains the function for setting the timer interrupt
*   tick resolution (in ms)
*
*END************************************************************************/

#include "mqx_inc.h"

#if MQX_HAS_TICK

/*FUNCTION*-------------------------------------------------------------------
* 
* Function Name    : _time_set_resolution
* Returned Value   : _mqx_uint Task error code
* Comments         :
*    This function sets the period of the clock interrupt.
*    THIS MUST AGREE WITH THE ACTUAL INTERRUPT PERIOD, or all _time_delays
*    WILL CHANGE.
*
*END*----------------------------------------------------------------------*/

_mqx_uint _time_set_resolution
   (
      /* [IN] the alarm resolution in milliseconds to be used by the kernel */
      _mqx_uint resolution
   )
{ /* Body */
   register KERNEL_DATA_STRUCT_PTR  kernel_data;

   _GET_KERNEL_DATA(kernel_data);

   /* 
   ** Convert resolution into ticks per second so new tick format will
   ** work 
   */
   kernel_data->TICKS_PER_SECOND = 1000 / resolution;

   /* Also set hw ticks per tick */
   kernel_data->HW_TICKS_PER_TICK = resolution * 1000;

   kernel_data->GET_HWTICKS = (uint_32 (_CODE_PTR_)(pointer))
      _time_get_microseconds;

   return MQX_OK;

} /* Endbody */
#endif

/* EOF */
