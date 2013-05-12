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
* $FileName: dd_rtec.c$
* $Version : 3.6.4.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   Implements some functions from the Diab Data RTA library, which 
*   need to be customized for proper integration into MQX
*
*END************************************************************************/

#include <stddef.h>
#include <rta/rtc.h>
#include <rta/rtaenv.h>
#include "mqx_inc.h"
#include "fio.h"
#include "rterrprv.h"

extern void* __RTC_INFO_PTR;
extern void* __RTC_SP_LIMIT;

/* Pointers to heap manager functions */
/* These are set to the MQX memory allocation/deallocation functions */
void * (*__malloc_func_ptr)(size_t)           = (void* (*)(size_t))_mem_alloc;
void * (*__realloc_func_ptr)(void *, size_t)  = (void* (*)(void *, size_t))NULL;
void   (*__free_func_ptr)(void *)             = (void  (*)(void *))_mem_free;

/* Globals and defines used by custom atexit() implementation */
#define ATEXIT_FCNS_ARR_SIZE   32
static boolean FirstTime = TRUE;
static void (_CODE_PTR_ ExitFcns[ATEXIT_FCNS_ARR_SIZE])(void);
static void (_CODE_PTR_ OldHandler)(void);
       void atexit_handler(void);
 
/* Error and reporting functions */
/*  These were duplicated so that they would use the MQX IO sub-system,
 *   and not the Diab Data io routines */

/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_abort
* Returned Value   : 
* Comments         : __rtc_abort is called by run-time checker when it detects
*                    severe error and execution must be aborted
*END*-------------------------------------------------------------------------*/

void __rtc_abort
   (
      void 
   )
{ /* Body */

   fputs("RTC: severe error was detected - program terminated\n", stderr);            
   _mqx_exit(0);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_error_default
* Returned Value   : 
* Comments         : default function for RTC error printing.
*                   __rtc_error is initialized by pointer to this function
*END*-------------------------------------------------------------------------*/

void __rtc_error_default
   (
      void * ptr,
      char * msg
   )
{ /* Body */

   fprintf(stderr, "RTC: 0x%08x, %s\n", ptr, msg);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_print
* Returned Value   : 
* Comments         : __rtc_print is used by RTC to print reports other than error message,
*                   e.g. memory leaks report
*END*-------------------------------------------------------------------------*/

void __rtc_print
   (
      char * msg
   )
{ /* Body */

   fputs(msg, stderr);

} /* Endbody */


/* Heap support functions */
/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_heap_walk
* Returned Value   : 
* Comments         : __rtc_heap_walk must iterate system memory blocks which are used by heap.
*                   Usually a program has only one such block created by sbrk
*                   To start iteration __rtc_heap_walk will be called with 'addr' == NULL,
*                   last call to __rtc_heap_walk must return NULL in 'addr'.
*END*-------------------------------------------------------------------------*/

void __rtc_heap_walk
   (
      struct __rtc_heap_walk_struct * p
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   extern char __DATA_RAM[], __DATA_END[];
   extern char __BSS_START[], __BSS_END[];

   if (p->addr == (void*)NULL) {
      /* First iteration - data and sdata sections */
      p->addr = __DATA_RAM;
      p->size = __DATA_END - __DATA_RAM; 
      return;
   } /* Endif */

   if (p->addr == __DATA_RAM) {
      /* Second iteration - bss section */
      p->addr = __BSS_START;
      p->size = __BSS_END - __BSS_START;
      return;
   } /* Endif */

   if (p->addr == __BSS_START) {
      /* Active task's stack range */
      _GET_KERNEL_DATA(kernel_data);
      p->addr = kernel_data->ACTIVE_PTR->STACK_LIMIT;
      p->size = (uint_32)kernel_data->ACTIVE_PTR->STACK_BASE - (uint_32)kernel_data->ACTIVE_PTR->STACK_LIMIT;
      return;
   } /* Endif */

   /* Subsequent calls */
   p->addr = (void*)NULL;
   p->size = 0;

   return;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_heap_type
* Returned Value   : 0 - heap does not support multiple threads
*                    1 - each thread has its own heap
*                    2 - one heap for all threads
* Comments         :
*
*END*-------------------------------------------------------------------------*/

int __rtc_heap_type
   (
      void 
   )
{ /* Body */

   /* MQX has one memory manager for all tasks */
   return 2;

} /* Endbody */


/* Stack checking functions */
/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_sp_limit
* Returned Value   : 
* Comments         : __rtc_sp_limit returns stack limit of current thread
*                   or NULL if stack limit is unknown
*END*-------------------------------------------------------------------------*/

void * __rtc_sp_limit
   (
      void 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR kernel_data;
   TD_STRUCT_PTR          active_td_ptr;
   extern char            __SP_END[];

   _GET_KERNEL_DATA(kernel_data);

   active_td_ptr = kernel_data->ACTIVE_PTR;

   /* If this function is being called from the context of the system td,
      we need to give give the limit of the boot stack, not the interrupt stack */
   if (active_td_ptr == SYSTEM_TD_PTR(kernel_data)) {
      return __SP_END;      
   } /* Endif */

   return (kernel_data->ACTIVE_PTR->STACK_LIMIT);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : __rtc_sp_grow
* Returned Value   : 
* Comments         : __rtc_sp_grow change stack limit to 'need' if possible
*
*END*-------------------------------------------------------------------------*/

void __rtc_sp_grow
   (
      void * need
   )
{ /* Body */

   return;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : atexit
* Returned Value   : 
* Comments         : This was inserted here to avoid linker conflicts.
*                    One of the RTEC functions caused exit() to be pulled in
*                    which conflicted with the customized exit in mqx library.
*                    The functionality of this routine is modified so it will
*                    register functions, so they get called from the MQX Exit Handler
*END*-------------------------------------------------------------------------*/

int atexit
   (
      void (*fcn)(void) 
   )
{ /* Body */
   uint_32    i, j;
   
   if (FirstTime) {
      for (i = 0; i < ATEXIT_FCNS_ARR_SIZE; i++) {
         ExitFcns[i] = (void (_CODE_PTR_)(void))NULL;
      } /* Endfor */
      OldHandler = _mqx_get_exit_handler();
      _mqx_set_exit_handler(atexit_handler);
      FirstTime = FALSE;
   } /* Endif */

   j = 0;
   while ((j < ATEXIT_FCNS_ARR_SIZE) && (ExitFcns[j] != (void (_CODE_PTR_)(void))NULL)) {
      j++;
   } /* Endwhile */

   if (j == ATEXIT_FCNS_ARR_SIZE) {
      /* All entries full, can't store any more */
      return -1;
   } /* Endif */   

   ExitFcns[j] = fcn;
   return 0;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : atexit_handler
* Returned Value   : 
* Comments         : Runs when MQX_Exit is called.  Chains to previous exit handler
*
*END*-------------------------------------------------------------------------*/

void atexit_handler
   (
      void 
   )
{ /* Body */
   uint_32 j = 0;

   /* Go through the list and call each of the functions */
   while ((j < ATEXIT_FCNS_ARR_SIZE) && (ExitFcns[j] != (void (_CODE_PTR_)(void))NULL)) {
      (*ExitFcns[j])();
      j++;
   } /* Endwhile */
   
   /* Chain to previous exit handler, usually set by BSP initialization */
   if (OldHandler != (void (_CODE_PTR_)(void))NULL) {
      (*OldHandler)();
   } /* Endif */

} /* Endbody */
      
/* Context initialize, save and restore functions */
/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_initialize_context
* Returned Value   : 
* Comments         : Initializes the fields in RTERRCHK_CONTEXT_STRUCT
*
*END*-------------------------------------------------------------------------*/

void _rterrchk_initialize_context
   (
      RTERRCHK_CONTEXT_STRUCT_PTR context_ptr
   )
{ /* Body */

   context_ptr->RTC_INFO_PTR_SAVE = NULL;
   context_ptr->RTC_SP_LIMIT_SAVE = (pointer)0xFFFFFFFF;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_save_context
* Returned Value   : 
* Comments         :
*
*END*-------------------------------------------------------------------------*/

void _rterrchk_save_context
   (
      void 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR           kernel_data;
   RTERRCHK_CONTEXT_STRUCT_PTR      context_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   context_ptr = kernel_data->ACTIVE_PTR->RUNTIME_ERROR_CHECK_PTR;
   if (context_ptr != NULL) {
      context_ptr->RTC_INFO_PTR_SAVE = __RTC_INFO_PTR;
      context_ptr->RTC_SP_LIMIT_SAVE = __RTC_SP_LIMIT;
   } /* Endif */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_restore_context
* Returned Value   : 
* Comments         :
*
*END*-------------------------------------------------------------------------*/

void _rterrchk_restore_context
   (
      void 
   )
{ /* Body */
   KERNEL_DATA_STRUCT_PTR           kernel_data;
   RTERRCHK_CONTEXT_STRUCT_PTR      context_ptr;
   
   _GET_KERNEL_DATA(kernel_data);

   context_ptr = kernel_data->ACTIVE_PTR->RUNTIME_ERROR_CHECK_PTR;
   __RTC_INFO_PTR = context_ptr->RTC_INFO_PTR_SAVE;
   __RTC_SP_LIMIT = context_ptr->RTC_SP_LIMIT_SAVE;

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_finish
* Returned Value   : 
* Comments         : Performs any rtec-specific cleanup that needs to 
*                    be done when a task is destroyed.
*
*END*-------------------------------------------------------------------------*/

void _rterrchk_finish
   (
      RTERRCHK_CONTEXT_STRUCT_PTR context_ptr
   )
{ /* Body */

   /* Nothing to do for Diab's RTEC */

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_mem_alloc
* Returned Value   : 
* Comments         : replaces mem alloc
*
*END*-------------------------------------------------------------------------*/

pointer _rterrchk_mem_alloc
   (
      _mem_size size
   )
{ /* Body */

   return (*__malloc_func_ptr)(size);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_mem_alloc_zero
* Returned Value   : 
* Comments         : replaces mem alloc_zero
*
*END*-------------------------------------------------------------------------*/

pointer _rterrchk_mem_alloc_zero
   (
      _mem_size size
   )
{ /* Body */
   pointer result;

   result =  (*__malloc_func_ptr)(size);
   if (result) {
      _mem_zero(result, size);
   }/* Endif */
   return(result);

} /* Endbody */


/*FUNCTION*---------------------------------------------------------------------
*
* Function Name    : _rterrchk_mem_free
* Returned Value   : 
* Comments         : replaces mem free
*
*END*-------------------------------------------------------------------------*/

_mqx_uint _rterrchk_mem_free
   (
      pointer p
   )
{ /* Body */

   (*__free_func_ptr)(p);
   return(MQX_OK);

} /* Endbody */


/* EOF */
