/**HEADER********************************************************************
*
* Copyright (c) 2011 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2011 Embedded Access Inc.;
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
* $FileName: e200core.c$
* $Version : 3.8.1.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file contains utility functions for use with the e200 cores.
*
*END*********************************************************************/

#include "mqx_inc.h"


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _PSP_GET_VECTOR_TABLE
* Returned Value   : int
* Comments         :
*    This function returns the IVPR
*
*END*----------------------------------------------------------------------*/
asm uint_32 _PSP_GET_VECTOR_TABLE()
{
    mfspr r3,E200CORE_IVPR
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_core_num
* Returned Value   : int
* Comments         :
*    This function returns the core number
*
*END*----------------------------------------------------------------------*/
asm uint_32 _psp_core_num()
{
    mfspr r3,E200CORE_PIR
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_set_pid
* Returned Value   : int
* Comments         :
*    This function sets the core's PID
*
*END*----------------------------------------------------------------------*/
__asm void _psp_set_pid( uint_32 pid)
{ 
//    fralloc

	/* Write PID; SPR 48 */
    mtspr   48, r3 // pid
    
//    frfree
    blr
}

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _e200_enable_timer
* Returned Value   : int
* Comments         :
*    This function enables Timebase unit
*
*END*----------------------------------------------------------------------*/
int_32 _e200_enable_timer(boolean external)
{
    register uint_32 tmp;

    _PSP_SPR_GET(tmp,E200CORE_HID0);

    /* Set the time base clock */
    if (external) {
        tmp |= E200CORE_HID0_SEL_TBCLK;
    } else {
        tmp &= ~E200CORE_HID0_SEL_TBCLK;
    }

    /* Time base enable */
    tmp |= E200CORE_HID0_TBEN;
    _PSP_SPR_SET(E200CORE_HID0, tmp);

    return MQX_OK;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_generate_interrupt
* Returned Value   : none
* Comments         :
*
*
*END*------------------------------------------------------------------------*/
void _psp_generate_interrupt(uint_32 int_vector)
{
    KERNEL_DATA_STRUCT_PTR           kernel_data;
    PSP_INTERRUPT_FRAME_STRUCT_PTR   exception_frame_ptr;

    _GET_KERNEL_DATA(kernel_data);

    /* Enable external interrupt */
    _PSP_SET_SR (_PSP_GET_SR() | PSP_MSR_EE);

    exception_frame_ptr = (pointer)((uchar_ptr)kernel_data->INTERRUPT_CONTEXT_PTR -
            FIELD_OFFSET(PSP_INTERRUPT_FRAME_STRUCT,INTERRUPT_CONTEXT));
    exception_frame_ptr->INTERRUPT_CONTEXT.ENABLE_SR = 0;

    /* invoke the interrupt service routine associated with the interrupt code */
    _PSP_GENERATE_INTERRUPT(int_vector);

    /* prevent the core from servicing interrupts */
    _PSP_SET_SR (_PSP_GET_SR() & ~PSP_MSR_EE);
}
