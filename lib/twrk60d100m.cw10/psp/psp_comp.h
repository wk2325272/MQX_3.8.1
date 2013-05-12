/**HEADER********************************************************************
* 
* Copyright (c) 2010 Freescale Semiconductor;
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
* $FileName: psp_comp.h$
* $Version : 3.8.5.0$
* $Date    : Sep-19-2011$
*
* Comments:
*
*   This file determines which compiler is running, then includes
*   the compiler specific header file
*
*END************************************************************************/
#ifndef __psp_comp_h__
#define __psp_comp_h__

#if defined(__DCC__)
#include "diab_cf.h"

#elif defined(__CODEWARRIOR__)
#include "cw_comp.h"

#elif defined(__IAR_SYSTEMS_ICC__)   //defined(__ICCCF__)
#include "iar_comp.h"

#elif defined(__CC_ARM)
#include "keil_comp.h"


#else
#error "COMPILER TYPE NOT DEFINED"
#endif

#define PSP_TASK_PARAM(stack_start_ptr) (stack_start_ptr->PARAMETER)

#endif
/* EOF */
