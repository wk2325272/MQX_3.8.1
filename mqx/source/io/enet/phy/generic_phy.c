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
* $FileName: generic_phy.c$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains generic phy interface functions
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"
#include "enet.h"
#include "enetprv.h"
#include "generic_phy.h"
#include "mdio.h"

static boolean generic_phy_discover_addr(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean generic_phy_init(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static uint_32 generic_phy_get_speed(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean generic_phy_get_link_status(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean generic_phy_get_duplex(ENET_CONTEXT_STRUCT_PTR enet_ptr);



const ENET_PHY_IF_STRUCT generic_phy_IF = {
  generic_phy_discover_addr,
  generic_phy_init,
  generic_phy_get_speed,
  generic_phy_get_link_status,
  generic_phy_get_duplex
};



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : generic_phy_discover_addr
*  Returned Value : none
*  Comments       :
*    Scan possible PHY addresses looking for a valid device
*
*END*-----------------------------------------------------------------*/

static boolean generic_phy_discover_addr
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{
   uint_32              i;
   uint_32              id;
   uint_32              found = 32;

   for (i = 0; i < 32; i++) {
      id = 0;
      enet_ptr->PHY_ADDRESS =i;
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_PHYID1, &id, MII_TIMEOUT)) {
          if ((id != 0) && (id != 0xffff) ) {
             //printf("found generic phy at addr %d %s\n", i, found==32?"":"DUPLICATE");
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
*  Function Name  : generic_phy_init
*  Returned Value : boolean
*  Comments       :
*    Wait for PHY to automatically negotiate its configuration
*
*END*-----------------------------------------------------------------*/

static boolean generic_phy_init
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{
   uint_32              phy_status=0;
   uint_32              bcr;
   uint_32              i;
  // uint_32              gbcr = RT8211_REG_GBCR_ADVERTISE_MASTER |
  //                             RT8211_REG_GBCR_PORT_PREF_MASTER |
  //                             RT8211_REG_GBCR_ADVERTISE_FULLDUP;


   if (enet_ptr->PARAM_PTR->OPTIONS & ENET_OPTION_PHY_LOOPBACK) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMCR, &phy_status, MII_TIMEOUT)) {
         phy_status |= MDIO_CTRL_LOOPBACK;
         (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, MDIO_BMCR, phy_status, MII_TIMEOUT);
         return TRUE;
      }
   } else {
      //(*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, MDIO_GBCR, gbcr, MII_TIMEOUT);

      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMSR, &phy_status, MII_TIMEOUT)) {
         if (phy_status & MDIO_STAT1_AUTONEG_ABILITY) {
            // Has auto-negotiate ability
            for (i = 0; i < 3 * BSP_ALARM_FREQUENCY; i++) {
               if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMSR, &phy_status, MII_TIMEOUT)) {
                  if ((phy_status & MDIO_STAT1_AUTONEG_COMPLETE) != 0) {
                     return TRUE;
                  }
               }
               if( (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMCR, &bcr, MII_TIMEOUT)) {
                  bcr |= MDIO_CTRL_AUTONEG | MDIO_CTRL_RESTART_AUTO;
                  (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, MDIO_BMCR, bcr, MII_TIMEOUT);
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
*  Function Name  : generic_phy_get_speed
*  Returned Value : uint_32 - connection speed
*  Comments       :
*    Determine if connection speed is 10, 100 or 1000 Mbit
*
*END*-----------------------------------------------------------------*/

static uint_32 generic_phy_get_speed
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{
   uint_32              speed_status;

   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMCR, &speed_status, MII_TIMEOUT)) {
      if (speed_status & MDIO_CTRL_SPEED_0) {
         return 100;
      } else {
         return 10;
      }
   }
   return 0;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : generic_phy_get_link_status
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       :
*    Get actual link status.
*
*END*-----------------------------------------------------------------*/

static boolean generic_phy_get_link_status
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
   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMSR, &data, MII_TIMEOUT)) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, MDIO_BMSR, &data, MII_TIMEOUT))
      {
         res = ((data & MDIO_STAT1_LINK_UP) != 0);
      }
   }
   return res;
}



/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  :generic_phy_get_duplex
*  Returned Value : uint_32 - 1 for full duplex, 0 for 1/2
*  Comments       :
*    Determines the Ethernet duplex mode
*
*END*-----------------------------------------------------------------*/

static boolean generic_phy_get_duplex( ENET_CONTEXT_STRUCT_PTR enet_ptr )
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

