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
* $FileName: pseudo_phy.h$
* $Version : 3.8.2.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*  This file contains definitions that belongs to the pseudo phy.
*
*END************************************************************************/
#ifndef _pseudo_phy_h_
#define _pseudo_phy_h_ 1

#ifdef __cplusplus
extern "C" {
#endif




extern const ENET_PHY_IF_STRUCT pseudo_phy_IF;

extern boolean pseudo_phy_discover_addr(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern boolean pseudo_phy_init(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern uint_32 pseudo_phy_get_speed(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern boolean pseudo_phy_get_link_status(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern boolean pseudo_phy_get_duplex(ENET_CONTEXT_STRUCT_PTR enet_ptr);
extern void pseudo_phy_dump( ENET_CONTEXT_STRUCT_PTR enet_ptr );


#ifdef __cplusplus
}
#endif

#endif

/* EOF */
