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
* $Version : 3.8.3.0$
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
#define MQX_CPU                 PSP_CPU_MCF51CN128   

/* determine which clock source is used, depends on J11 & J12 jumper setting 
   Only one xtal must be selected */
#define BSPCFG_USE_25MHZ_XTAL          1   
#define BSPCFG_USE_32KHZ_XTAL          0

/*
** BSP settings - for defaults see mqx\source\bsp\<board_name>\<board_name>.h
*/

#define BSPCFG_ENABLE_RTCDEV     0
#define BSPCFG_ENABLE_TTYA       0
#define BSPCFG_ENABLE_TTYB       1
#define BSPCFG_ENABLE_TTYC       0
#define BSPCFG_ENABLE_ITTYA      0
#define BSPCFG_ENABLE_ITTYB      0
#define BSPCFG_ENABLE_ITTYC      0
#define BSPCFG_ENABLE_I2C0       0   /* NOTE: Shares wires with KBI, FB */
#define BSPCFG_ENABLE_I2C1       0   /* NOTE: Shares wires with SPI0, ADC */
#define BSPCFG_ENABLE_II2C0      0
#define BSPCFG_ENABLE_II2C1      0
#define BSPCFG_ENABLE_GPIODEV    0
#define BSPCFG_ENABLE_ADC        1   /* NOTE: Shares wires with I2C1, SPI0 */
#define BSPCFG_ENABLE_SPI0       0   /* NOTE: Shares wires with I2C1, ADC */    
#define BSPCFG_ENABLE_ISPI0      0   /* NOTE: Shares wires with I2C1, ADC */   
#define BSPCFG_ENABLE_SPI1       0   /* NOTE: Shares wires with KBI, ADC */
#define BSPCFG_ENABLE_ISPI1      0   /* NOTE: Shares wires with KBI, ADC */
#define BSPCFG_ENABLE_FLASHX     0
#define BSPCFG_ENABLE_PCFLASH    0   /* NOTE: CompcatFlash shares CS0 signal with MRAM on TWR-MEM board */
#define BSPCFG_ENABLE_TCHSRES    0

#define BSPCFG_RX_RING_LEN       2
#define BSPCFG_TX_RING_LEN       2

#define BSPCFG_ENABLE_ENET_MULTICAST   0
#define BSPCFG_ENABLE_ENET_STATS       0  

/*
** board-specific MQX settings
*/

#define MQX_COMPONENT_DESTRUCTION      0
#define MQX_CHECK_ERRORS               0

/*
** Workaround for CW10 OSBDM asm "stop" issue 
** In case IDLE_TASK is switched on, stop is never called, OSBDM does not stop debugging
*/
#if defined(__CWBUILD__)
#if  (__CWBUILD__ >= 3003)
#define MQX_USE_IDLE_TASK              1
#endif
#endif

/*
** board-specific MFS settings
*/

#define SHELLCFG_USES_MFS              0

/*
** board-specific RTCS settings
*/

#define RTCS_MINIMUM_FOOTPRINT         1
#define RTCSCFG_CHECK_ERRORS           0
#define RTCSCFG_CHECK_ADDRSIZE         0
#define RTCSCFG_ENABLE_ICMP            1
#define RTCSCFG_ENABLE_UDP             1
#define RTCSCFG_ENABLE_LWDNS           1
#define RTCSCFG_ENABLE_TCP             1
#define RTCSCFG_ENABLE_STATS           0
#define RTCSCFG_ENABLE_GATEWAYS        1
#define RTCSCFG_LOG_SOCKET_API         0
#define RTCSCFG_LOG_PCB                0

#define FTPDCFG_USES_MFS               0

#define TELNETDCFG_NOWAIT              FALSE 
#define ARPCACHE_SIZE                  2 
#define ARPALLOC_SIZE                  2 

#define HTTPDCFG_DEF_SES_CNT           1
#define HTTPDCFG_MAX_BYTES_TO_SEND     (128)
#define HTTPCFG_TX_WINDOW_SIZE         (256)
#define HTTPCFG_RX_WINDOW_SIZE         (256)
 
/*
** include common settings
*/

/* use the rest of defaults from small-RAM-device profile */
#include "small_ram_config.h"

/* and enable verification checks in kernel */
#include "verif_enabled_config.h"

#endif
/* EOF */
