/**HEADER********************************************************************
* 
* Copyright (c) 2008-2011 Freescale Semiconductor;
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
* $FileName: iar.c$
* $Version : 3.8.3.0$
* $Date    : Sep-2-2011$
*
* Comments:
*
*   This file contains runtime support for the CodeWarrior Compiler.
*
*END************************************************************************/
#include "mqx.h"
#include "bsp.h"

extern void init_hardware(void);

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : __low_level_init
* Returned Value   : void
* Comments         :
*   Call function to initialize CPU registers
*
*END*----------------------------------------------------------------------*/
    
int __low_level_init(void)
{
    /* Initialize hardware. */
    init_hardware();
    return (1);
}
    

/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : malloc
* Returned Value   : pointer
* Comments         :
*   Override C/C++ runtime heap allocation function in IAR's DLIB
*
*END*----------------------------------------------------------------------*/

pointer malloc(unsigned int bytes)
{
  return _mem_alloc_system(bytes);
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : calloc
* Returned Value   : pointer
* Comments         :
*   Override C/C++ runtime heap allocation function in IAR's DLIB
*
*END*----------------------------------------------------------------------*/

pointer calloc(unsigned int n, unsigned int z)
{
   return _mem_alloc_system_zero(n*z);
} 


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : free
* Returned Value   : void
* Comments         :
*   Override C/C++ runtime heap deallocation function in IAR's DLIB
*
*END*----------------------------------------------------------------------*/

void free(pointer p)
{
   _mem_free(p);
}


/*FUNCTION*-------------------------------------------------------------------
*
* Function Name    : exit
* Returned Value   : should not return
* Comments         :
*   Replacement for Codewarrior's exit function
*
*END*----------------------------------------------------------------------*/
#if MQX_EXIT_ENABLED

void __cexit_call_dtors(void);

int exit(int status)
{
   /* Destroy all constructed global objects */
   __cexit_call_dtors();

   /* Destroy all constructed global objects */
   while (TRUE) {
   }
}

#else  /* MQX_EXIT_ENABLED */

/* int exit(int) is supplied by IAR's DLIB */

#endif /* MQX_EXIT_ENABLED */

/* EOF */
