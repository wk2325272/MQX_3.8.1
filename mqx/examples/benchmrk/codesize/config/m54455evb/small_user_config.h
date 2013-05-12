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
* $FileName: small_user_config.h$
* $Version : 3.6.3.0$
* $Date    : Jun-11-2010$
*
* Comments:
*
*   User configuration for MQX components
*
*END************************************************************************/

#ifndef __small_user_config_h__
#define __small_user_config_h__

/* mandatory CPU identification */
#define MQX_CPU                 PSP_CPU_MCF54455

/*
** BSP settings - see for defaults mqx\source\bsp\m54455evb\m54455evb.h
*/
#define BSPCFG_ENABLE_ENET_MULTICAST 0
#define BSPCFG_ENABLE_ENET_STATS     0
#define BSPCFG_ENET_RESTORE          0
#define BSPCFG_ENABLE_CPP            0
#define BSPCFG_ENABLE_IO_SUBSYSTEM   0
#define BSPCFG_ENABLE_TTYA           0
#define BSPCFG_ENABLE_TTYB           0
#define BSPCFG_ENABLE_TTYC           0
#define BSPCFG_ENABLE_ITTYA          0
#define BSPCFG_ENABLE_ITTYB          0
#define BSPCFG_ENABLE_ITTYC          0
#define BSPCFG_ENABLE_I2C0           0
#define BSPCFG_ENABLE_I2C1           0
#define BSPCFG_ENABLE_II2C0          0
#define BSPCFG_ENABLE_II2C1          0
#define BSPCFG_ENABLE_GPIODEV        0
#define BSPCFG_ENABLE_RTCDEV         0
#define BSPCFG_ENABLE_PCFLASH        0
#define BSPCFG_ENABLE_SPI0           0
#define BSPCFG_ENABLE_ISPI0          0
#define BSPCFG_ENABLE_ADC            0
#define BSPCFG_ENABLE_FLASHX         0

#define MQX_USE_LWMEM_ALLOCATOR      0

/* PSP settings */
#include <smallest_config.h>

/* and enable verification checks in kernel */
#include "verif_enabled_config.h"

#endif
/* EOF */
