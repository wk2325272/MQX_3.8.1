/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 2004-2008 Embedded Access Inc.;
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
* $FileName: phy_lan8710.c$
* $Version : 3.8.3.1$
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
#include "phy_lan8710.h"

static boolean phy_lan8710_discover_addr(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean phy_lan8710_init(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static uint_32 phy_lan8710_get_speed(ENET_CONTEXT_STRUCT_PTR enet_ptr);
static boolean phy_lan8710_get_link_status(ENET_CONTEXT_STRUCT_PTR enet_ptr);

const ENET_PHY_IF_STRUCT phy_lan8710_IF = {
  phy_lan8710_discover_addr,
  phy_lan8710_init,
  phy_lan8710_get_speed,
  phy_lan8710_get_link_status
};
  
  

/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_lan8710_discover_addr
*  Returned Value : none
*  Comments       :
*    Scan possible PHY addresses looking for a valid device
*
*END*-----------------------------------------------------------------*/

static boolean phy_lan8710_discover_addr
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32              i;
   uint_32              id;

   for (i = 0; i < 32; i++) {
      id = 0;
      
      // Try an address - note this needs to be set in order for *PHY_READ to use it.
      enet_ptr->PHY_ADDRESS = i;
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_IDR1, &id, MII_TIMEOUT)) 
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
*  Function Name  : phy_lan8710_init
*  Returned Value : boolean
*  Comments       :
*    Wait for PHY to automatically negotiate its configuration
*
*END*-----------------------------------------------------------------*/

static boolean phy_lan8710_init
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32              phy_status=0;

   if (enet_ptr->PARAM_PTR->OPTIONS & ENET_OPTION_PHY_LOOPBACK) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_BMCR, &phy_status, MII_TIMEOUT)) {
         phy_status |= LAN8710_REG_BMCR_LOOPBACK;
         (*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_WRITE)(enet_ptr, LAN8710_REG_BMCR, phy_status, MII_TIMEOUT);
         return TRUE;
      }
   } else {
     if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_BMSR, &phy_status, MII_TIMEOUT)) {
        if (phy_status & LAN8710_REG_BMSR_AUTO_NEG_ABILITY) { 
           // Has auto-negotiate ability
           int i;
           for (i = 0; i < 3 * BSP_ALARM_FREQUENCY; i++) {
              if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_BMSR, &phy_status, MII_TIMEOUT)) 
                 if ((phy_status & LAN8710_REG_BMSR_AUTO_NEG_COMPLETE) != 0) 
                    return TRUE;
              _time_delay(BSP_ALARM_RESOLUTION);
           }  
        }
        return TRUE;
      }
   }  
   return FALSE;
}  


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_lan8710_get_speed
*  Returned Value : uint_32 - connection speed
*  Comments       :
*    Determine if connection speed is 10 or 100 Mbit
*
*END*-----------------------------------------------------------------*/

static uint_32 phy_lan8710_get_speed
   (
       ENET_CONTEXT_STRUCT_PTR     enet_ptr
   )
{ 
   uint_32              speed_status;

   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_PHY_SCSR, &speed_status, MII_TIMEOUT)) {
      return ((speed_status & LAN8710_REG_PHY_SCSR_SPEED_MASK) == LAN8710_REG_PHY_SCSR_SPEED_100) ? 100 : 10;
   }
   return 0;
} 
   
  
/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : phy_lan8710_get_link_status
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       : 
*    Get actual link status.
*
*END*-----------------------------------------------------------------*/

static boolean phy_lan8710_get_link_status
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
   if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_BMSR, &data, MII_TIMEOUT)) {
      if ((*enet_ptr->PARAM_PTR->ENET_IF->MAC_IF->PHY_READ)(enet_ptr, LAN8710_REG_BMSR, &data, MII_TIMEOUT))
      {
         res = ((data & LAN8710_REG_BMSR_LINK_STATUS) != 0) ? TRUE : FALSE;
      }
   }
   return res;
}
