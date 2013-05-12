#ifndef __psp_prv_h__
#define __psp_prv_h__
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
* $FileName: psp_prv.h$
* $Version : 3.0.2.0$
* $Date    : Aug-19-2008$
*
* Comments:
*
*   This file contains psp private declarations for use when compiling
*   the kernel.
*
*END************************************************************************/

#define _PSP_GET_CALLING_PC() ((pointer)0)

/* This macro modifies the context of a blocked task so that the
** task will execute the provided function when it next runs
*/
#define _PSP_SET_PC_OF_BLOCKED_TASK(stack_ptr, func) \
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->FRAME.RETURN_ADDRESS = \
   (void (_CODE_PTR_)(void))(func)


/* This macro modifies the interrupt priority of a blocked task so that the
** task will execute at the correct interrupt priority when it restarts
*/
#define _PSP_SET_SR_OF_BLOCKED_TASK(stack_ptr, sr_value) \
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->FRAME.STATUS_REGISTER &= ~0700;\
   ((PSP_BLOCKED_STACK_STRUCT_PTR)(stack_ptr))->FRAME.STATUS_REGISTER |= \
      sr_value;


/* This macro modifies the context of a task that has been interrupted
** so that the task will execute the provided function when the isr returns
*/
#define _PSP_SET_PC_OF_INTERRUPTED_TASK(stack_ptr, func) \
   ((PSP_BASIC_INT_FRAME_STRUCT_PTR)(stack_ptr))->FRAME.RETURN_ADDRESS = \
   (void (_CODE_PTR_)(void))(func)

/* Calculate the address of the td extension found on the stack */
#define _PSP_GET_TD_EXTENSION(td_ptr) \
   ((uchar _PTR_)((td_ptr)->STACK_BASE) - sizeof(PSP_STACK_START_STRUCT) + \
    FIELD_OFFSET(PSP_STACK_START_STRUCT, TD_EXTENSION))

/*--------------------------------------------------------------------------*/
/*
**                  PROTOTYPES OF PSP FUNCTIONS
*/

#ifdef __cplusplus
extern "C" {
#endif

extern void    _psp_build_float_context(TD_STRUCT_PTR);
extern pointer _psp_get_float_context(pointer);

#ifdef __cplusplus
}
#endif

#endif
/* EOF */
