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
* $Version : 3.8.21.0$
* $Date    : Aug-1-2011$
*
* Comments:
*
*   This file provides a generic header file for use by the mqx kernel
*   for including processor specific information
*
*END************************************************************************/

#ifndef __psp_cpu_h__
#define __psp_cpu_h__

//#include "psptypes.h"
#include "psp_cpudef.h"

/* 
** The main requirement is to define target processor
*/
#ifndef MQX_CPU
#error  You must define target processor in "user_config.h" (MQX_CPU)
#endif


#if PSP_MQX_CPU_IS_MCF51CN
    #include <mcf51CN.h>
#elif PSP_MQX_CPU_IS_MCF51EM
    #include <mcf51em.h>
#elif PSP_MQX_CPU_IS_MCF51JE
    #include <mcf51je.h> 
#elif PSP_MQX_CPU_IS_MCF51MM
    #include <mcf51mm.h>
#elif PSP_MQX_CPU_IS_MCF51AC
    #include <mcf51ac.h> 
#elif PSP_MQX_CPU_IS_MCF51AG
   #include  <mcf51ag.h>   	
#elif PSP_MQX_CPU_IS_MCF51JM
   #include  <mcf51jm.h>          
#elif PSP_MQX_CPU_IS_MCF5222X
    #include <mcf5222.h>
#elif PSP_MQX_CPU_IS_MCF5223X
    #include <mcf5223.h>
#elif PSP_MQX_CPU_IS_MCF5225X
    #include <mcf5225.h>
#elif PSP_MQX_CPU_IS_MCF5227X
    #include <mcf5227.h>
#elif PSP_MQX_CPU_IS_MCF5301X
    #include <mcf5301.h>    
#elif PSP_MQX_CPU_IS_MCF5445X
    #include <mcf5445.h>
#elif PSP_MQX_CPU_IS_MCF5441X
    #include <mcf5441.h>
#elif PSP_MQX_CPU_IS_MCF520X
    #include <mcf5208.h> 
#elif PSP_MQX_CPU_IS_MCF532X
    #include <mcf532.h>     
#elif PSP_MQX_CPU_IS_MCF51FD        
   #include  <mcf51fd.h> 
#elif PSP_MQX_CPU_IS_MCF51JF        
   #include  <mcf51jf.h>   
#elif PSP_MQX_CPU_IS_MCF51QM        
   #include  <mcf51qm.h>      
#else
    #error INCORRECT MQX_CPU SETTING
#endif

#include <coldfire.h>

// Needed for smarter _PSP_SET_CACR macro & backward compatibility
#ifndef CACR_AUTO_CLEAR_BITS
#define CACR_AUTO_CLEAR_BITS    0
#endif

#ifndef PSP_CACHE_SPLIT
#define PSP_CACHE_SPLIT         0
#endif

#endif
