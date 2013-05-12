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
* $FileName: phy_mcf5223x.c$
* $Version : 3.8.5.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the Natioanl Semiconductor
*  PHY chips DP83848, DP83849, DP83640.
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"
#include "enet.h"
#include "enetprv.h"
#include "phy_mcf5223x.h"

static boolean phy_mcf5223x_discover_addr(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean phy_mcf5223x_init(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static uint_32 phy_mcf5223x_get_speed(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean phy_mcf5223x_get_link_status(ENET_CONTEXT_STRUCT_PTR enet_ptr);

const ENET_PHY_IF_STRUCT phy_mcf5223x_IF = {
  phy_mcf5223x_discover_addr,
  phy_mcf5223x_init,
  phy_mcf5223x_get_speed,
  phy_mcf5223x_get_link_status
};
  
  

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_mcf5223x_discover_addr
*  Returned Value : none
*  Comments       :
*    Scan possible PHY addresses looking for a valid device
*
*END*-----------------------------------------------------------------*/

static boolean phy_mcf5223x_discover_addr
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32              i;
   uint_32              id;

   for (i = 0; i < 32; i++) {
      id = 0;
      enet_ptr->PHY_ADDRESS = i;
      
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_IDR1, &id, MII_TIMEOUT)) 
      {
          if ((id != 0) && (id != 0xffff)) 
          {
                return TRUE;
          }
      } 
   } 
   
   return FALSE; 
} 


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_mcf5223x_init
*  Returned Value : boolean
*  Comments       :
*    Wait for PHY to automatically negotiate its configuration
*
*END*-----------------------------------------------------------------*/

static boolean phy_mcf5223x_init
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32                waitcount;
   uint_32                phy_status=0;
   MCF5223_EPT_STRUCT_PTR ephy = (MCF5223_EPT_STRUCT_PTR)(&((VMCF5223_STRUCT_PTR)_PSP_GET_IPSBAR())->EPT);

   ephy->EPHYCTL0 &= (~ (MCF5223_EPHYCTL0_EPHYEN | MCF5223_EPHYCTL0_ANDIS));   // Disable device, enable autonegotiation
   ephy->EPHYCTL0 |= (MCF5223_EPHYCTL0_DIS100 | MCF5223_EPHYCTL0_DIS10);       // Turn off clocks
   ephy->EPHYCTL1 = MCF5223_EPHYCTL1_PHYADD(enet_ptr->PHY_ADDRESS);                      // Set phy address
   ephy->EPHYCTL0 |= MCF5223_EPHYCTL0_LEDEN;                                   // Turn on LEDs
   ephy->EPHYCTL0 |= MCF5223_EPHYCTL0_EPHYEN;                                  // Enable device
   _time_delay((BSP_ALARM_FREQUENCY*BSP_ALARM_RESOLUTION));                    // 1 sec pause

      // ERRATA FIX for PHY configuration   
   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_ANAR, &phy_status, MII_TIMEOUT)) {
      phy_status &= (~ ( MCF5223X_PHY_ANAR_NEXT_PAGE));                    // Turn off next page mode
      (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, MCF5223X_PHY_ANAR, phy_status, MII_TIMEOUT);
      _time_delay((BSP_ALARM_FREQUENCY*BSP_ALARM_RESOLUTION));                 // 1 sec pause
   }
   (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_ANAR, &phy_status, MII_TIMEOUT);
   ephy->EPHYCTL0 &= (~ (MCF5223_EPHYCTL0_DIS100 | MCF5223_EPHYCTL0_DIS10));   // Turn clocks on
   _time_delay((BSP_ALARM_FREQUENCY*BSP_ALARM_RESOLUTION));                    // 1 sec pause
   (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_ANAR, &phy_status, MII_TIMEOUT);

   if (enet_ptr->PARAM_PTR->OPTIONS & ENET_OPTION_PHY_LOOPBACK) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_CR, &phy_status, MII_TIMEOUT)) {
         phy_status |= MCF5223X_PHY_CR_LOOPBACK;
         (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, MCF5223X_PHY_CR, phy_status, MII_TIMEOUT);
         return TRUE;
      }
   } else {
     if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_SR, &phy_status, MII_TIMEOUT)) {
        if (phy_status & MCF5223X_PHY_SR_AN_ABILITY) { 
           // Has auto-negotiate ability
           int i;
           for (i = 0; i < 3 * BSP_ALARM_FREQUENCY; i++) {
              if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_SR, &phy_status, MII_TIMEOUT)) 
                 if ((phy_status & MCF5223X_PHY_SR_AN_COMPLETE) != 0) 
                    return TRUE;
              _time_delay(BSP_ALARM_RESOLUTION);
           }  
           (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, MCF5223X_PHY_CR, MCF5223X_PHY_CR_RESET, MII_TIMEOUT); // Reset PHY command
           _time_delay((BSP_ALARM_FREQUENCY*BSP_ALARM_RESOLUTION));            // 1 sec pause
        }
        return TRUE;
     }
   }  
   return FALSE;
}  


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_mcf5223x_get_speed
*  Returned Value : uint_32 - connection speed
*  Comments       :
*    Determine if connection speed is 10 or 100 Mbit
*
*END*-----------------------------------------------------------------*/

static uint_32 phy_mcf5223x_get_speed
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32              speed_status;

   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_PSR, &speed_status, MII_TIMEOUT)) {
      return ((speed_status & MCF5223X_PHY_PSR_SPEED) == 0) ? 10 : 100;
   }
   return 0;
} 
   
  
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_mcf5223x_get_link_status
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       : 
*    Get actual link status.
*
*END*-----------------------------------------------------------------*/

static boolean phy_mcf5223x_get_link_status
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32 data;
   boolean res = FALSE;

   /* Some PHY (e.g.DP8340) returns "unconnected" and than "connected" state
   *  just to show that was transition event from one state to another.
   *  As we need only curent state,  read 2 times and return 
   *  the current/latest state. */
   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_SR, &data, MII_TIMEOUT)) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MCF5223X_PHY_SR, &data, MII_TIMEOUT))
      {
         res = ((data & MCF5223X_PHY_SR_LINK_STATUS) != 0) ? TRUE : FALSE;
      }
   }
   return res;
}
