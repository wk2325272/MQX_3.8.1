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
* $FileName: default_user_config.h$
* $Version : 3.8.5.0$
* $Date    : Sep-22-2011$
*
* Comments:
*
*   User configuration for MQX components
*
*END************************************************************************/

#ifndef __user_config_h__
#define __user_config_h__

/* mandatory CPU identification */
#define MQX_CPU                 PSP_CPU_MCF52259

/*
** BSP settings - for defaults see mqx\source\bsp\<board_name>\<board_name>.h
*/

#define BSPCFG_ENABLE_RTCDEV     0
#define BSPCFG_ENABLE_TTYA       1
#define BSPCFG_ENABLE_TTYB       0
#define BSPCFG_ENABLE_TTYC       0
#define BSPCFG_ENABLE_ITTYA      0
#define BSPCFG_ENABLE_ITTYB      0
#define BSPCFG_ENABLE_ITTYC      0
#define BSPCFG_ENABLE_I2C0       0
#define BSPCFG_ENABLE_I2C1       0
#define BSPCFG_ENABLE_II2C0      0
#define BSPCFG_ENABLE_II2C1      0
#define BSPCFG_ENABLE_SPI0       0
#define BSPCFG_ENABLE_ISPI0      0
#define BSPCFG_ENABLE_GPIODEV    0
#define BSPCFG_ENABLE_FLASHX     0
#define BSPCFG_ENABLE_PCFLASH    0
#define BSPCFG_ENABLE_ADC        1


#define BSPCFG_RX_RING_LEN       4
#define BSPCFG_TX_RING_LEN       2

/*
** board-specific MQX settings - see for defaults mqx\source\include\mqx_cnfg.h
*/

#define MQX_HAS_TIME_SLICE       1

/*
** board-specific RTCS settings - see for defaults rtcs\source\include\rtcscfg.h
*/

#define RTCSCFG_ENABLE_ICMP      1
#define RTCSCFG_ENABLE_UDP       1
#define RTCSCFG_ENABLE_TCP       1
#define RTCSCFG_ENABLE_STATS     1
#define RTCSCFG_ENABLE_GATEWAYS  1
#define FTPDCFG_USES_MFS         1

#define TELNETDCFG_NOWAIT        FALSE 

/*
** include common settings
*/

/* use the rest of defaults from small-RAM-device profile */
#include "small_ram_config.h"

/* and enable verification checks in kernel */
#include "verif_enabled_config.h"

#endif
/* EOF */
