/**HEADER********************************************************************
* 
* Copyright (c) 2008-2011 Freescale Semiconductor;
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
* $FileName: httpdsrv.c$
* $Version : 3.8.4.0$
* $Date    : Sep-22-2011$
*
* Comments:
*
*   Example HTTP server using RTCS Library.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>

#include "httpd.h"
#include "httpdsrv.h"
#include "tfs.h"
#include "cgi.h"
#include "enet_wifi.h"
#include "config.h"

#include <select.h>

#if BSPCFG_ENABLE_ADC
#include "adc.h"
#endif

#if DEMOCFG_USE_WIFI
#include "iwcfg.h"
#include "string.h"
#endif

#define HTTPD_SEPARATE_TASK     1


#define ADC_USED_CHANNELS       0


#if ! BSPCFG_ENABLE_IO_SUBSYSTEM
#error This application requires BSPCFG_ENABLE_IO_SUBSYSTEM defined non-zero in user_config.h. Please recompile BSP with this option.
#endif


#ifndef BSP_DEFAULT_IO_CHANNEL_DEFINED
#error This application requires BSP_DEFAULT_IO_CHANNEL to be not NULL. Please set corresponding BSPCFG_ENABLE_TTYx to non-zero in user_config.h and recompile BSP with this option.
#endif


#ifndef MQX_DEVICE
#define MQX_DEVICE BSP_DEFAULT_IO_CHANNEL
#endif

#ifdef LED_1
    extern  LWGPIO_STRUCT led1;
#endif
#ifdef LED_2
    extern  LWGPIO_STRUCT led2;
#endif
#ifdef LED_3
    extern  LWGPIO_STRUCT led3;
#endif
#ifdef LED_4
    extern  LWGPIO_STRUCT led4;
#endif

void main_task(uint_32);

/*
** MQX initialization information
*/

const TASK_TEMPLATE_STRUCT  MQX_template_list[] =
{
   /* Task Index,   Function,   Stack,  Priority,   Name,    Attributes,          Param, Time Slice */
    { 1,            main_task,  2500,   8,          "Main",  MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

const HTTPD_ROOT_DIR_STRUCT root_dir[] = {
    { "", "tfs:" },
    { 0, 0 }
};


boolean output_port = 0;

#define SES_MAX     4

/*TASK*-----------------------------------------------------------------
*
* Function Name  : main_task
* Returned Value : void
* Comments       :
*
*END------------------------------------------------------------------*/
void main_task(uint_32 temp) {
    int_32            error;
    uint_8            pin;
    _enet_handle      ehandle0;
    _rtcs_if_handle   ihandle0;
    _enet_address     address0;
    HTTPD_STRUCT *server0;
    HTTPD_PARAMS_STRUCT *params0;

    _enet_handle      ehandle1;
    _rtcs_if_handle   ihandle1;
    _enet_address     address1;
    HTTPD_STRUCT *server1;
    HTTPD_PARAMS_STRUCT *params1;

    extern const HTTPD_CGI_LINK_STRUCT cgi_lnk_tbl[];
    extern const TFS_DIR_ENTRY tfs_data[];

 /* Open and set port TC as output to drive LEDs (LED10 - LED13) */
#ifdef LED_1
    output_port = lwgpio_init(&led1, LED_1, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
    if(!output_port)
    {
        printf("Initializing LWGPIO for LED1 failed.\n");
    }
    lwgpio_set_functionality(&led1, BSP_LED1_MUX_GPIO);
    /*Turn off Led */
    lwgpio_set_value(&led1, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_2
    output_port = lwgpio_init(&led2, LED_2, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
    if(!output_port)
    {
        printf("Initializing LWGPIO for LED2 failed.\n");
    }
    lwgpio_set_functionality(&led2, BSP_LED2_MUX_GPIO);
    /*Turn off Led */
    lwgpio_set_value(&led2, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_3
    output_port = lwgpio_init(&led3, LED_3, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
    if(!output_port)
    {
        printf("Initializing LWGPIO for LED3 failed.\n");
    }
    lwgpio_set_functionality(&led3, BSP_LED3_MUX_GPIO);
    /*Turn off Led */
    lwgpio_set_value(&led3, LWGPIO_VALUE_LOW);
#endif

#ifdef LED_4
    output_port = lwgpio_init(&led4, LED_4, LWGPIO_DIR_OUTPUT, LWGPIO_VALUE_NOCHANGE);
    if(!output_port)
    {
        printf("Initializing LWGPIO for LED4 failed.\n");
    }
    lwgpio_set_functionality(&led4, BSP_LED4_MUX_GPIO);
    /*Turn off Led */
    lwgpio_set_value(&led4, LWGPIO_VALUE_LOW);
#endif

    /* runtime RTCS configuration */
    _RTCSPCB_init = 4;
    _RTCSPCB_grow = 2;
    _RTCSPCB_max = 6;
    _RTCS_msgpool_init = 4;
    _RTCS_msgpool_grow = 2;
    _RTCS_msgpool_max  = 8;
    _RTCS_socket_part_init = 4;
    _RTCS_socket_part_grow = 2;
    _RTCS_socket_part_max  = 6;

    _RTCSTASK_stacksize = 8000;

    error = RTCS_create();

    ENET_get_mac_address(DEMOCFG_DEFAULT_DEVICE_0, ENET_IPADDR_0, address0);
    error = ENET_initialize(DEMOCFG_DEFAULT_DEVICE_0, address0, 0, &ehandle0);

    ENET_get_mac_address(DEMOCFG_DEFAULT_DEVICE_1, ENET_IPADDR_1, address1);
    error = ENET_initialize(DEMOCFG_DEFAULT_DEVICE_1, address1, 0, &ehandle1);

    error = RTCS_if_add(ehandle0, RTCS_IF_ENET, &ihandle0);
    error = RTCS_if_bind(ihandle0, ENET_IPADDR_0, ENET_IPMASK);

    error = RTCS_if_add(ehandle1, RTCS_IF_ENET, &ihandle1);
    error = RTCS_if_bind(ihandle1, ENET_IPADDR_1, ENET_IPMASK);

    error = _io_tfs_install("tfs:", tfs_data);

    printf("preparing http server ...\n");

    params0 = httpd_default_params(NULL);
    if (params0) {
        params0->root_dir = (HTTPD_ROOT_DIR_STRUCT*)root_dir;
        params0->index_page = "\\index_0.html";
        params0->address=ENET_IPADDR_0;
        server0 = httpd_init(params0);
    }
    HTTPD_SET_PARAM_CGI_TBL(server0, (HTTPD_CGI_LINK_STRUCT*)cgi_lnk_tbl);

    params1 = httpd_default_params(NULL);
    if (params1) {
        params1->root_dir = (HTTPD_ROOT_DIR_STRUCT*)root_dir;
        params1->index_page = "\\index_1.html";
        params1->address=ENET_IPADDR_1;
        server1 = httpd_init(params1);
    }
    HTTPD_SET_PARAM_CGI_TBL(server1, (HTTPD_CGI_LINK_STRUCT*)cgi_lnk_tbl);
    printf("run http server...\n");

#if HTTPD_SEPARATE_TASK || !HTTPDCFG_POLL_MODE
    httpd_server_run(server0);
    httpd_server_run(server1);

    /* user stuff come here */
    _task_block();
#else
    while (1) {
        httpd_server_poll(server, -1);
        httpd_server_poll(server2, -1);
        /* user stuff come here - only non blocking calls */
    }
#endif

}
