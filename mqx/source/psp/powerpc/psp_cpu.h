/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: psp_cpu.h$
* $Version : 3.8.8.0$
* $Date    : Oct-5-2011$
*
* Comments:
*
*   This file provides a generic header file for use by the mqx kernel
*   for including processor specific information
*
*END************************************************************************/

#ifndef __psp_cpu_h__
#define __psp_cpu_h__

#include "psp_cpudef.h"

/* 
** The main requirement is to define target processor
*/
#ifndef MQX_CPU
#error  You must define target processor in "user_config.h" (MQX_CPU)
#endif


#if MQX_CPU == PSP_CPU_MPC5121
   #include <mpc5121.h>
#elif (MQX_CPU == PSP_CPU_MPC5125)
   #include <mpc5125.h>
#elif MQX_CPU == PSP_CPU_MPC5200
   #include <mpc5200.h>
#elif MQX_CPU == PSP_CPU_MPC5200B
   #include <mpc5200.h>
#elif MQX_CPU == PSP_CPU_MPC5674
   #include <mpc5674.h>
#elif MQX_CPU == PSP_CPU_MPC5675K
   #include <mpc5675k.h>
#elif MQX_CPU == PSP_CPU_MPC8306
   #include <mpc83xx.h>
#elif MQX_CPU == PSP_CPU_MPC8308
   #include <mpc83xx.h>
#elif MQX_CPU == PSP_CPU_MPC8309
   #include <mpc83xx.h>
#elif MQX_CPU == PSP_CPU_MPC8313
   #include <mpc8313.h>
#elif PSP_MQX_CPU_IS_MPC837x 
   #include <mpc83xx.h>
#elif MQX_CPU == PSP_CPU_MPC8347
   #include "mpc8349.h" 
#elif MQX_CPU == PSP_CPU_MPC8349
   #include <mpc8349.h>
#elif MQX_CPU == PSP_CPU_MPC8377
   #include <mpc8377.h>
#elif MQX_CPU == PSP_CPU_MPC5643L
   #include <mpc5643l.h>
#elif MQX_CPU == PSP_CPU_MPXN20
   #include <mpxn20.h>
#elif MQX_CPU == PSP_CPU_MPXR40
   #include <mpxs30.h>
#elif MQX_CPU == PSP_CPU_MPXD10
   #include <mpxd10.h>
#elif MQX_CPU == PSP_CPU_MPXD20
   #include <mpxd20.h>
#elif MQX_CPU == PSP_CPU_MPXS20
   #include <mpxs20.h>
#elif MQX_CPU == PSP_CPU_MPXS30
   #include <mpxs30.h>
#else
    #error INCORRECT MQX_CPU SETTING
#endif

#if PSP_MQX_CPU_IS_PPC
   #include <powerpc.h>
   
#else
   #error Unsupported CPU Architecture
#endif


#ifndef PSP_NUM_CORES
    #define PSP_NUM_CORES 1
#endif

// Needed for smarter _PSP_SET_CACR macro & backward compatibility
#ifndef CACR_AUTO_CLEAR_BITS
   #define CACR_AUTO_CLEAR_BITS    0
#endif

#ifndef PSP_CACHE_SPLIT
   #define PSP_CACHE_SPLIT         0
#endif

#endif
