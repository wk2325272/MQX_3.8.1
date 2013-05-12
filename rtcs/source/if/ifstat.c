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
* $FileName: ifstat.c$
* $Version : 3.0.5.0$
* $Date    : Nov-21-2008$
*
* Comments:
*
*   This file has functions to retrieve statistics structures
*   for each protocol in RTCS.
*
*END************************************************************************/

#include <rtcs.h>
#include "rtcs_prv.h"
#include "tcpip.h"
#include "tcp.h"
#include "udp_prv.h"
#include "icmp_prv.h"
#include "ip_prv.h"

/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  IP_stats
* Returned Value  :  IP statistics
* Comments        :
*     Returns a pointer to the IP layer statistics.
*
*END*-----------------------------------------------------------------*/

#if RTCSCFG_ENABLE_IP_STATS
IP_STATS_PTR IP_stats
   (
      void
   )
{ /* Body */
   IP_CFG_STRUCT_PTR   ip_cfg_ptr;

   ip_cfg_ptr = RTCS_getcfg(IP);

   if (ip_cfg_ptr != NULL) {
      return &ip_cfg_ptr->STATS;
   } /* Endif */

   return NULL;
} /* Endbody */

#endif


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  ICMP_stats
* Returned Value  :  ICMP statistics
* Comments        :
*     Returns a pointer to the ICMP layer statistics.
*
*END*-----------------------------------------------------------------*/

#if RTCSCFG_ENABLE_ICMP_STATS
ICMP_STATS_PTR ICMP_stats
   (
      void
   )
{ /* Body */
   ICMP_CFG_STRUCT_PTR   icmp_cfg_ptr;

   icmp_cfg_ptr = RTCS_getcfg(ICMP);

   if (icmp_cfg_ptr != NULL) {
      return &icmp_cfg_ptr->STATS;
   } /* Endif */

   return NULL;
} /* Endbody */

#endif

/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  UDP_stats
* Returned Value  :  UDP statistics
* Comments        :
*     Returns a pointer to the UDP layer statistics.
*
*END*-----------------------------------------------------------------*/

#if RTCSCFG_ENABLE_UDP_STATS
UDP_STATS_PTR UDP_stats
   (
      void
   )
{ /* Body */
   UDP_CFG_STRUCT_PTR   udp_cfg_ptr;

   udp_cfg_ptr = RTCS_getcfg(UDP);

   if (udp_cfg_ptr != NULL) {
      return &udp_cfg_ptr->STATS;
   } /* Endif */

   return NULL;
} /* Endbody */
#endif

/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  TCP_stats
* Returned Value  :  TCP statistics
* Comments        :
*     Returns a pointer to the TCP layer statistics.
*
*END*-----------------------------------------------------------------*/

#if RTCSCFG_ENABLE_TCP_STATS
TCP_STATS_PTR TCP_stats
   (
      void
   )
{ /* Body */
   TCP_CFG_STRUCT_PTR   tcp_cfg_ptr;

   tcp_cfg_ptr = RTCS_getcfg(TCP);

   if (tcp_cfg_ptr != NULL) {
      return &tcp_cfg_ptr->STATS;
   } /* Endif */

   return NULL;
} /* Endbody */
#endif


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  IPIF_stats
* Returned Value  :  IPIF statistics
* Comments        :
*     Returns a pointer to an interface's statistics.
*
*END*-----------------------------------------------------------------*/

#if RTCSCFG_ENABLE_IPIF_STATS
IPIF_STATS_PTR IPIF_stats
   (
      _rtcs_if_handle   handle
         /* [IN] the RTCS interface state structure */
   )
{ /* Body */
   IP_IF_PTR if_ptr = (IP_IF_PTR)handle;
   return &if_ptr->STATS;
} /* Endbody */
#endif


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   :  ARP_stats
* Returned Value  :  ARP statistics
* Comments        :
*     Returns a pointer to an interface's ARP statistics.
*
*END*-----------------------------------------------------------------*/

#if RTCSCFG_ENABLE_ARP_STATS
ARP_STATS_PTR ARP_stats
   (
      _rtcs_if_handle   handle
         /* [IN] the RTCS interface state structure */
   )
{ /* Body */
   IP_IF_PTR if_ptr = (IP_IF_PTR)handle;
   return (ARP_STATS_PTR)if_ptr->ARP;
} /* Endbody */
#endif


/* EOF */
