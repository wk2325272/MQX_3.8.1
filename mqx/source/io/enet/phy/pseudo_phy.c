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
* $FileName: pseudo_phy.c$
* $Version : 3.8.3.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the pseudo
* Ethernet Phy
*
*END************************************************************************/

#include "mqx.h"
#include "bsp.h"
#include "bsp_prv.h"
#include "enet.h"
#include "enetprv.h"
#include "pseudo_phy.h"


const ENET_PHY_IF_STRUCT pseudo_phy_IF = {
  pseudo_phy_discover_addr,
  pseudo_phy_init,
  pseudo_phy_get_speed,
  pseudo_phy_get_link_status,
  pseudo_phy_get_duplex
};


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : pseudo_phy_discover_addr
*  Returned Value : none
*  Comments       :
*    Scan possible PHY addresses looking for a valid device
*
*END*-----------------------------------------------------------------*/

boolean pseudo_phy_discover_addr(ENET_CONTEXT_STRUCT_PTR    enet_ptr)
{
   enet_ptr->PHY_ADDRESS = 1;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : pseudo_phy_init
*  Returned Value : boolean
*  Comments       :
*    Wait for PHY to automatically negotiate its configuration
*
*END*-----------------------------------------------------------------*/

boolean pseudo_phy_init(ENET_CONTEXT_STRUCT_PTR enet_ptr)
{
   return TRUE;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : pseudo_phy_get_speed
*  Returned Value : uint_32 - connection speed
*  Comments       :
*    Determine if connection speed is 10, 100 or 1000 Mbit
*
*END*-----------------------------------------------------------------*/

uint_32 pseudo_phy_get_speed(ENET_CONTEXT_STRUCT_PTR    enet_ptr)
{
#ifdef BSP_PSEUDO_PHY_SPEED
   return BSP_PSEUDO_PHY_SPEED;
#else
   return 1000;
#endif
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : pseudo_phy_get_link_status
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       :
*    Get actual link status.
*
*END*-----------------------------------------------------------------*/

boolean pseudo_phy_get_link_status(ENET_CONTEXT_STRUCT_PTR  enet_ptr)
{
   return TRUE;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : pseudo_phy_get_duplex
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       :
*    Get actual link status.
*
*END*-----------------------------------------------------------------*/

boolean pseudo_phy_get_duplex(ENET_CONTEXT_STRUCT_PTR   enet_ptr)
{
   return TRUE;
}


/*FUNCTION*-------------------------------------------------------------
*
*  Function Name  : pseudo_phy_dump
*  Returned Value : TRUE if link active, FALSE otherwise
*  Comments       :
*    Dump the phy registers.
*
*END*-----------------------------------------------------------------*/

void pseudo_phy_dump( ENET_CONTEXT_STRUCT_PTR enet_ptr )
{

}
