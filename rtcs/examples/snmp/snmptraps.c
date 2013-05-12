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
* $FileName: snmptraps.c$
* $Version : 3.7.18.0$
* $Date    : Jan-17-2011$
*
* Comments:
*
*   Example showing how to use SNMP traps
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include <snmpcfg.h>
#include <snmp.h>

#include <config.h>
#include "snmp_demo.h"

#ifdef ENET_DEVICE
#include <enet.h>
#endif
#ifdef PPP_DEVICE
#include <ppp.h>
#endif
#ifndef MQX_DEVICE
#define MQX_DEVICE BSP_DEFAULT_IO_CHANNEL
#endif

#if ! RTCSCFG_ENABLE_SNMP
  #error This application requires RTCSCFG_ENABLE_SNMP defined non-zero in user_config.h. Please recompile BSP with this option.
#endif

#if ! MQX_HAS_TIME_SLICE
  #error This application requires MQX_HAS_TIME_SLICE defined non-zero in user_config.h. Please recompile BSP with this option.
#endif
void Main_task (uint_32);

#ifdef MON_TYPE
volatile uint_32 _mqx_monitor_type = MON_TYPE;
#endif

#ifdef ENET_DEVICE
_enet_address enet_local  = ENET_ENETADDR;
_ip_address   enet_ipaddr = ENET_IPADDR;
_ip_address   enet_ipmask = ENET_IPMASK;
#endif

#ifdef PPP_DEVICE
LWSEM_STRUCT ppp_sem;
static void PPP_linkup (pointer lwsem) {_lwsem_post(lwsem);}
#endif

_ip_address  my_trap_list[] = {ENET_TRAP_ADDR1, ENET_TRAP_ADDR2,
                               ENET_TRAP_ADDR3, ENET_TRAP_ADDR4,0 };

/*
** MQX initialization information
*/

extern void MIBdemo_init(void);

const TASK_TEMPLATE_STRUCT  MQX_template_list[] = 
{
   /* Task Index,   Function,   Stack,  Priority,   Name,     Attributes,          Param, Time Slice */
    { 1,            Main_task,  2000,   9,          "Main",   MQX_AUTO_START_TASK, 0,     0 },
    { 0 }
};

MQX_INITIALIZATION_STRUCT  MQX_init_struct =
{
   /* PROCESSOR_NUMBER                */  BSP_DEFAULT_PROCESSOR_NUMBER,
   /* START_OF_KERNEL_MEMORY          */  BSP_DEFAULT_START_OF_KERNEL_MEMORY,
   /* END_OF_KERNEL_MEMORY            */  BSP_DEFAULT_END_OF_KERNEL_MEMORY,
   /* INTERRUPT_STACK_SIZE            */  BSP_DEFAULT_INTERRUPT_STACK_SIZE,
   /* TASK_TEMPLATE_LIST              */  (pointer)MQX_template_list,
   /* MQX_HARDWARE_INTERRUPT_LEVEL_MAX*/  BSP_DEFAULT_MQX_HARDWARE_INTERRUPT_LEVEL_MAX,
   /* MAX_BUFFER_POOLS                */  BSP_DEFAULT_MAX_MSGPOOLS,
   /* MAX_QUEUE_NUMBER                */  BSP_DEFAULT_MAX_MSGQS,
   /* IO_CHANNEL                      */  MQX_DEVICE,
   /* IO_OPEN_MODE                    */  BSP_DEFAULT_IO_OPEN_MODE
};


/*TASK*-----------------------------------------------------------------
*
* Function Name  : Main_task
* Returned Value : void
* Comments       : Application should be the lowest priority. Where lower 
*                  the number, higher the priority.
*                  priority sequence should be: RTCS_TASK highest, 
*                  SNMP_TASK mid and application low.
*
*END------------------------------------------------------------------*/

void Main_task
(
    uint_32 temp
)
{ /* Body */
   #include <ipcfg.h>
   
   IPCFG_IP_ADDRESS_DATA    ip_data;
   
   _rtcs_if_handle   ihandle;
   uint_32           error;
   _ip_address       new_target; 
   _enet_address     enet_address;                                   

    /* runtime RTCS configuration */
    _RTCSPCB_init = 4;
    _RTCSPCB_grow = 2;
    _RTCSPCB_max = 6;
    _RTCS_socket_part_init = 4;
    _RTCS_socket_part_grow = 2;
    _RTCS_socket_part_max  = 6;

    error = RTCS_create();
    if (error) {
        printf("\nFailed to initialize RTCS, error = %X", error);
        _task_block();
    } /* Endif */

    ip_data.ip = ENET_IPADDR;
    ip_data.mask = ENET_IPMASK;
    ip_data.gateway = ENET_IPGATEWAY;
    
    ENET_get_mac_address (BSP_DEFAULT_ENET_DEVICE, ENET_IPADDR, enet_address);
    error = ipcfg_init_device (BSP_DEFAULT_ENET_DEVICE, enet_address);
    error = ipcfg_bind_staticip (BSP_DEFAULT_ENET_DEVICE, &ip_data);
    
    ihandle = ipcfg_get_ihandle( BSP_DEFAULT_ENET_DEVICE );

   /* 
   ** Install some MIBs for the SNMP agent
   ** MIB1213_init must be called before SNMP_init functions 
   ** It should not be removed.
   ** If application wishes to use MQX RTOS, MIBMQX_init must be called
   ** before SNMP_init functions.
   */  
   MIB1213_init();
   /* init MQX MIB */
   MIBMQX_init();
   /* demo MIB table init */
   MIBdemo_init();
    
   /* 
   ** Initialize the agent. 
   ** There are two functions for initializing SNMP agent.
   ** If application wants to send ColdStart/WarmStart traps at 
   ** the initialization time, it must call: 
   ** SNMP_init_with_traps(agent task name, agent task priority, 
   **                      agent task stack size, list of trap receivers);
   **
   ** Calling this function will initialize SNMP agent and send 
   ** ColdStart/WarmStart (depending on system up time) traps to the 
   ** station(s).
   **
   ** If application does not want to send ColdStart/WarmStart traps at 
   ** the initialization time, it must call:
   ** SNMP_init(agent task name, agent task priority, 
   **           agent task stack size);
   **
   ** Note1: If application wishes to use SNMP_init() to initialize the 
   **        SNMP agent, there is no IP address for trap to be sent.
   **        Application must add trap receivers to the list by calling
   **        RTCS_trap_target_add(_ip_address). To remove receiver from
   **        trap receivers list call RTCS_trap_target_remove(_ip_address).
   **
   ** This function initializes SNMP agent without sending any traps. 
   **
   ** This example will use SNMP_init_with_traps. 
   */
   
   //error = SNMP_init("SNMP", 7, 3000);
   
   error = SNMP_init_with_traps("SNMP", 7, 2500, my_trap_list);
   if (error) {
      printf("\nFailed to initialize SNMP agent, error = %X", error);
      _task_block();
   } /* Endif */

    printf("Demo started, wait... \n\r");   
   /* 
   ** RTCS/SNMP agent now supports private and public communities. 
   ** User can add community to the community list, that is defined 
   ** in snmpcfg.h by adding community string to the SNMPCFG_COMMUNITY_LIST
   ** and changing the SNMPCFG_NUM_COMMUNITY respectively.
   **
   ** Note1: Communities cannot be added dynamically.
   ** Note2: RTCS/SNMP agent will respond to get, set and getnext messages 
   **        to communities defined in SNMPCFG_COMMUNITY_LIST.
   **
   ** Application can select communities for send and receive traps by calling
   ** SNMP_trap_select_community(community name); Where community name should 
   ** match one of the strings in the SNMPCFG_COMMUNITY_LIST. By default
   ** current community is set to first string in SNMPCFG_COMMUNITY_LIST[].
   ** This function returns TRUE if the community was changed successfully,
   ** FALSE on failure.
   ** 
   ** Note3: SNMP_trap_select_community(char_ptr community_name); should be
   **        called after SNMP_init (or with trap) has been called. 
   ** 
   ** Note4: mylinkdown trap is enterprise specific, therefore application 
   ** need to implement this trap. SNMP_trap_myLinkDown is just an example 
   ** for use as reference.
   */
   
   /* Send some traps to private and public communities */
   
   /* trap will be sent to four IPs in my_trap_list */
   if(SNMP_trap_select_community("private")) 
   {
      SNMP_trap_linkDown(ihandle);
      SNMP_trap_linkUp(ihandle);
      SNMP_trap_myLinkDown(ihandle);
      SNMP_trap_coldStart(); 
      SNMP_trap_warmStart(); 
   }

   new_target = ENET_TRAP_ADDR5;
   
   /* Add a new IP address to the trap list */
   error = RTCS_trap_target_add(new_target);
   if (error) {
      printf("\nFailed to add target trap, error = %X", error);
   } /* Endif */
   
   /* trap will be sent to five IPs */
   if(SNMP_trap_select_community("public")) 
   {
      SNMP_trap_linkDown(ihandle);
      SNMP_trap_linkUp(ihandle);
      SNMP_trap_myLinkDown(ihandle);
      SNMP_trap_coldStart(); 
      SNMP_trap_warmStart(); 
   }

   /* Remove the new IP address from trap list */
   error = RTCS_trap_target_remove(new_target);
   if (error) {
      printf("\nFailed to remove target trap, error = %X", error);
   } /* Endif */
   
   
#ifdef SNMPCFG_SEND_V2_TRAPS   
   if (SNMP_trap_select_community("public"))
   {
      SNMPv2_trap_linkDown(ihandle);
      SNMPv2_trap_linkUp(ihandle);
      SNMPv2_trap_coldStart();
      SNMPv2_trap_warmStart();
   }
#endif
    
    /* Call the counter demo function */
    Snmp_task(0);
   _task_block();

} /* Endbody */

/* EOF */
