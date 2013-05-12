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
* $FileName: phy_rt8211.c$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the Real Tek 8211B
* Ethernet Phy
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"
#include "enet.h"
#include "enetprv.h"
#include "phy_rt8211.h"
#include "mdio.h"


const ENET_PHY_IF_STRUCT phy_rt8211_IF = {
  phy_rt8211_discover_addr,
  phy_rt8211_init,
  phy_rt8211_get_speed,
  phy_rt8211_get_link_status,
  phy_rt8211_get_duplex
};
  
  

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_rt8211_discover_addr
*  Returned Value : none
*  Comments       :
*    Scan possible PHY addresses looking for a valid device
*
*END*-----------------------------------------------------------------*/

boolean phy_rt8211_discover_addr
   (
       ENET_CONTEXT_STRUCT_PTR	   enet_ptr
   )
{ 
   uint_32              i;
   uint_32              id;
   uint_32              found = 32;
   
   for (i = 0; i < 32; i++) {
      id = 0;
      enet_ptr->PHY_ADDRESS =i;
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_IDR1, &id, MII_TIMEOUT)) {
	      if (id == RT8211_PHY_ID) {
	         printf("found rt8211 at addr %d %s\n", i, found==32?"":"DUPLICATE");
	         found = i;
	      }
      } 
   }
    
   if (found <32) {
      enet_ptr->PHY_ADDRESS = found;
      return TRUE;
   }
  
   return FALSE; 
} 


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_rt8211_init
*  Returned Value : boolean
*  Comments       :
*    Wait for PHY to automatically negotiate its configuration
*
*END*-----------------------------------------------------------------*/

boolean phy_rt8211_init
   (
       ENET_CONTEXT_STRUCT_PTR	   enet_ptr
   )
{ 
   uint_32              phy_status=0;
   uint_32              bcr;
   uint_32              i;
   uint_32              gbcr = RT8211_REG_GBCR_ADVERTISE_MASTER |
                               RT8211_REG_GBCR_PORT_PREF_MASTER |
                               RT8211_REG_GBCR_ADVERTISE_FULLDUP;
                               

   if (enet_ptr->PARAM_PTR->OPTIONS & ENET_OPTION_PHY_LOOPBACK) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_BMCR, &phy_status, MII_TIMEOUT)) {
         phy_status |= RT8211_REG_BMCR_LOOPBACK;
         (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, RT8211_REG_BMCR, phy_status, MII_TIMEOUT);
         return TRUE;
      }
   } else {
      (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, RT8211_REG_GBCR, gbcr, MII_TIMEOUT);
     
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_BMSR, &phy_status, MII_TIMEOUT)) {
         if (phy_status & RT8211_REG_BMSR_AUTO_NEG_ABILITY) { 
            // Has auto-negotiate ability
            for (i = 0; i < 3 * BSP_ALARM_FREQUENCY; i++) {
               if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_BMSR, &phy_status, MII_TIMEOUT)) {
                  if ((phy_status & RT8211_REG_BMSR_AUTO_NEG_COMPLETE) != 0) { 
                     return TRUE;
                  }
               }
               if( (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_BMCR, &bcr, MII_TIMEOUT)) {
                  bcr |= RT8211_REG_BMCR_AUTO_NEG_ENABLE | RT8211_REG_BMCR_RESTART_AUTONEG;
                  (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, RT8211_REG_BMCR, bcr, MII_TIMEOUT);
               }
               _time_delay(BSP_ALARM_RESOLUTION);
            }  
         }
        
//         return TRUE;
	   }
   }  
   return FALSE;
}  


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_rt8211_get_speed
*  Returned Value : uint_32 - connection speed
*  Comments       :
*    Determine if connection speed is 10, 100 or 1000 Mbit
*
*END*-----------------------------------------------------------------*/

uint_32 phy_rt8211_get_speed
   (
       ENET_CONTEXT_STRUCT_PTR	   enet_ptr
   )
{ 
   uint_32              speed_status;

   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_PHYSR, &speed_status, MII_TIMEOUT)) {
      speed_status &= RT8211_REG_PHYSR_SPEED;
      if (speed_status == RT8211_REG_PHYSR_1000MB) {
         return 1000;
      }
      else if (speed_status == RT8211_REG_PHYSR_100MB) {
         return 100;
      }
      else if (speed_status== RT8211_REG_PHYSR_10MB) {
         return 10;
      }
   }
   return 0;
} 
   
  
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_rt8211_get_link_status
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       : 
*    Get actual link status.
*
*END*-----------------------------------------------------------------*/

boolean phy_rt8211_get_link_status
   (
       ENET_CONTEXT_STRUCT_PTR	   enet_ptr
   )
{ 
   uint_32 data;
   boolean res = FALSE;

   /* Some PHY (e.g.DP8340) returns "unconnected" and than "connected" state
   *  just to show that was transition event from one state to another.
   *  As we need only curent state,  read 2 times and return 
   *  the current/latest state. */
   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_BMSR, &data, MII_TIMEOUT)) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, RT8211_REG_BMSR, &data, MII_TIMEOUT))
      {
         res = ((data & RT8211_REG_BMSR_LINK_STATUS) != 0) ? TRUE : FALSE;
      }
   }
   return res;
} 

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_rt8211_dump
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       : 
*    Dump the phy registers.
*
*END*-----------------------------------------------------------------*/

void phy_rt8211_dump( ENET_CONTEXT_STRUCT_PTR enet_ptr )
{ 
   uint_32 data;
   boolean res = FALSE;
   uint_32 i;
   
   for (i=RT8211_REG_BMCR;i<=RT8211_REG_LEDCR;i++ ){
   

      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, i, &data, MII_TIMEOUT)) {
         printf("rt8211 reg #0x%02x = 0x%08x\n",i,data);
      } else {
           printf("rt8211 reg #0x%02x - error reading register\n",i);
    
      }
   }
} 
  
  
  
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  :phy_rt8211_get_duplex
*  Returned Value : uint_32 - 1 for full duplex, 0 for 1/2
*  Comments       :
*    Determines the Ethernet duplex mode
*
*END*-----------------------------------------------------------------*/

boolean phy_rt8211_get_duplex( ENET_CONTEXT_STRUCT_PTR enet_ptr )
{
   uint_32                 duplex = 0, tmp, tmp2;

   /* Read PHY control register */
   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMCR, &tmp, MII_TIMEOUT)) {

      if (tmp & MDIO_CTRL_AUTONEG) {
         /* Auto Negotiation enabled */
         /* Determine duplex */
         if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_ANLPAR, &tmp2, MII_TIMEOUT)) {
            if (tmp2 & (MDIO_AUTO_LNK_100BASE_TX_FULL | MDIO_AUTO_LNK_10BASE_T_FULL)) {
               duplex = 1;
            } 
         }
      } else { 
         /* Auto Negotiation disabled */
         if (tmp & MDIO_CTRL_FULL_DUPLEX) {
            duplex = 1;
         }
      } 
   }
   return duplex;
}
