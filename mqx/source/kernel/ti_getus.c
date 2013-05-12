/**HEADER********************************************************************
* 
* Copyright (c) 2011 Freescale Semiconductor;
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
* $FileName: ti_getus.c$
* $Version : 3.8.1.0$
* $Date    :May-11-2011$
*
* Comments:
*
*   This file contains the function that reads the timer and returns
*   the number of nanoseconds elapsed since the last interrupt.
*
*END*********************************************************************/


#include "mqx_inc.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _time_get_microseconds
* Returned Value   : uint_16 microseconds
* Comments         :
*    This routine returns the number of nanoseconds that have elapsed
* since the last interrupt.
*
*END*----------------------------------------------------------------------*/

uint_16 _time_get_microseconds
    (
        void
    )
{
    uint_32 hwticks;
    uint_32 tickper;
   
    KERNEL_DATA_STRUCT_PTR kernel_data;

    _GET_KERNEL_DATA(kernel_data);

    if (kernel_data->GET_HWTICKS == NULL)
        return 0; /* BSP does not support HW ticks */

    hwticks = (*kernel_data->GET_HWTICKS)(kernel_data->GET_HWTICKS_PARAM);

    /* Tick period in time units (us) */
    tickper = 1000000 / kernel_data->TICKS_PER_SECOND; /* 32bit div */

    /* The result shall fit into 16 bits */
    return (uint_16)((hwticks * tickper) / kernel_data->HW_TICKS_PER_TICK); /* 32bit mul and div */
}
