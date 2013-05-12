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
* $FileName: stack_bu.c$
* $Version : 3.8.11.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   This file contains the functions for manipulating the user
*   context on the stack.
*   $Header:stack_bu.c, 5, 3/29/2004 12:03:44 PM, $
*   $NoKeywords$
*
*END************************************************************************/

#include "mqx_inc.h"
#include "psp.h"

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_build_stack_frame
* Returned Value   : none
* Comments         :
*
*   This function sets up the stack frame of a new task descriptor.
*
*END*----------------------------------------------------------------------*/

boolean _psp_build_stack_frame
    (
        /* [IN] the address of the task descriptor */
        TD_STRUCT_PTR    td_ptr,

        /* [IN] the address of the stack memory block */
        pointer          stack_ptr,

        /* [IN] the size of the stack */
        _mem_size        stack_size,

        /* [IN] the task template address */
        TASK_TEMPLATE_STRUCT_PTR template_ptr,

        /* [IN] the status register to use in creating the task */
        _mqx_uint        status_register,

        /* [IN] the task creation parameter */
        uint_32          create_parameter
    )
{
    uchar_ptr stack_base_ptr;
    PSP_STACK_START_STRUCT_PTR stack_start_ptr;
    boolean res = TRUE;

    stack_base_ptr  = (uchar_ptr)_GET_STACK_BASE(stack_ptr,stack_size);
    stack_start_ptr = (pointer)(stack_base_ptr - sizeof(PSP_STACK_START_STRUCT));

    td_ptr->STACK_BASE  = (pointer)stack_base_ptr;
    td_ptr->STACK_LIMIT = _GET_STACK_LIMIT(stack_ptr,stack_size);
    td_ptr->STACK_PTR   = stack_start_ptr;

    /*
    ** Build the task's initial stack frame. This contains the initialized
    ** registers, and an exception frame which will cause the task to
    ** "return" to the start of the task when it is dispatched.
    */
    _mem_zero(stack_start_ptr, (uint_32)sizeof(PSP_STACK_START_STRUCT));

    _PSP_MSR_GET(stack_start_ptr->INITIAL_CONTEXT.MSR);
    if (status_register) {
        stack_start_ptr->INITIAL_CONTEXT.MSR &= ~PSP_MSR_EE;
#ifdef PSP_MSR_CE // Critical interrupt enable
        stack_start_ptr->INITIAL_CONTEXT.MSR &= ~PSP_MSR_CE;
#endif
    } else {
        stack_start_ptr->INITIAL_CONTEXT.MSR |= PSP_MSR_EE;
#ifdef PSP_MSR_CE // Critical interrupt enable
        stack_start_ptr->INITIAL_CONTEXT.MSR |= PSP_MSR_CE;
#endif
    }

    #if defined(PSP_MPC603) || defined(PSP_MPC740) || defined(PSP_MPC750) || defined(PSP_MPC7400) || \
         defined(PSP_G2_CORE) || defined(PSP_G2_LE_CORE) || defined(PSP_E300_CORE)
    stack_start_ptr->INITIAL_CONTEXT.MSR |= PSP_MSR_FP;
    #elif defined(PSP_MPC7400)
    stack_start_ptr->INITIAL_CONTEXT.MSR |= PSP_MSR_ALTIVEC;
#endif

    _mem_zero( &stack_start_ptr->INITIAL_CONTEXT.GPR[0], sizeof(stack_start_ptr->INITIAL_CONTEXT.GPR));

    stack_start_ptr->INITIAL_CONTEXT.GPR[PSP_GPR_3] = (_psp_register) create_parameter;
    stack_start_ptr->INITIAL_CONTEXT.GPR[PSP_GPR_6] = (_psp_register) 6; /* For debugging help */

    /* Make sure pointers to small const and data area copied over */
    stack_start_ptr->INITIAL_CONTEXT.GPR[PSP_GPR_13] = (_psp_register) _PSP_GET_R13();

    stack_start_ptr->INITIAL_CONTEXT.RETURN_ADDRESS = template_ptr->TASK_ADDRESS;

#if (PSPCFG_SAVE_AND_RESTORE_SPE_REGISTERS==1)
#if (PSPCFG_TEST_SAVE_AND_RESTORE_SPE_REGISTERS==1)
    // Used to fill SPE registers with known data, unique per task, to aid in verifying
    // SPE context save and restore is working
    {
         uint_64 i;
         uint_64 temp1;

         temp1 = td_ptr->TASK_ID;
         temp1 <<=32;
         stack_start_ptr->INITIAL_CONTEXT.GPR[0] |= temp1;
         for (i=1;i<PSP_NUM_SAVED_GP_REGISTERS;i++) {
              stack_start_ptr->INITIAL_CONTEXT.GPR[i] |= temp1 | (i+2)<<56;
         }
         stack_start_ptr->INITIAL_CONTEXT.ACC = temp1;
    }
#endif
    stack_start_ptr->INITIAL_CONTEXT.SPEFSCR = 0;

#endif

    stack_start_ptr->PREVIOUS_STACK_POINTER =
        &stack_start_ptr->ZERO_STACK_POINTER;

    stack_start_ptr->EXIT_ADDRESS = _task_exit_function_internal;
    stack_start_ptr->INITIAL_CONTEXT.LR = (uint_32)_task_exit_function_internal;
    stack_start_ptr->PARAMETER    = create_parameter;

#if defined(PSP_MPC7400)
    if (td_ptr->FLAGS & MQX_DSP_TASK) {
        res = _psp_build_dsp_context(td_ptr);
    }
#endif

    return res;
}

#if (MQX_CPU==PSP_CPU_MPC7400)
/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_build_dsp_context
* Returned Value   : none
* Comments         :
*
*   This function sets up the DSP context of a new task descriptor.
*
*END*----------------------------------------------------------------------*/

boolean _psp_build_dsp_context
    (
        /* [IN] the address of the task descriptor */
        pointer in_td_ptr
    )
{
    TD_STRUCT_PTR                    td_ptr = (TD_STRUCT_PTR) in_td_ptr;
    uchar_ptr                        mem,mem2;
    PSP_BLOCKED_DSP_STACK_STRUCT_PTR dsp_ptr;

    /* Allocate space for saving/restoring the DSP registers.
    ** We need to align to 16-bytes for best AltiVec performance.
    */
    mem = (uchar_ptr) _mem_alloc_zero(
        (_mem_size)(sizeof(PSP_BLOCKED_DSP_STACK_STRUCT)+8));
    if ((((uint_32)mem) & 15) != 0) {
        mem2 = mem + 8;
    } else {
        mem2 = mem;
    }

    // mem should now by 16-byte aligned
    dsp_ptr = (PSP_BLOCKED_DSP_STACK_STRUCT_PTR) mem2;

#if MQX_CHECK_MEMORY_ALLOCATION_ERRORS
    if (!dsp_ptr) {
        /* Couldn't allocate memory for the DSP register context */
        _task_set_error_td_internal(td_ptr, MQX_OUT_OF_MEMORY);
        return FALSE;
    }
#endif

    /*
    ** Transfer the block to the task being created. This will ensure the DSP context
    ** will be freed if the task is destroyed
    */
    _mem_transfer_internal((pointer)dsp_ptr, td_ptr);

    dsp_ptr->TID = td_ptr->TASK_ID;
    td_ptr->FLOAT_CONTEXT_PTR = (pointer)fp_ptr;

    return TRUE;
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : _psp_get_dsp_context
* Returned Value   : pointer to task's DSP context
* Comments         :
*
*   This function returns the DSP context of a the specified task.
*
*END*----------------------------------------------------------------------*/

pointer _psp_get_dsp_context
    (
        /* [IN] the address of the task descriptor */
        pointer    in_td_ptr
    )
{

#if MQX_CHECK_ERRORS
    /* This field should never be overwitten */
    if (!in_td_ptr) {
        return NULL;
    }
#endif

    return ((TD_STRUCT_PTR)in_td_ptr)->DSP_CONTEXT_PTR;

}
#endif /* (MQX_CPU==PSP_CPU_MPC7400) */

/* EOF */
