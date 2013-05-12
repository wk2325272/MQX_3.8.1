#ifndef _security_h_
#define _security_h_
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
* $FileName: Security.h$
* $Version : 3.8.11.0$
* $Date    : Aug-1-2011$
*
* Comments:
*   The main configuration file for the email security demo
*
*END************************************************************************/

#define DEMOCFG_ENABLE_DHCP     1   
#define DEMOCFG_ENABLE_SNTP     1
#define DEMOCFG_AUTH_REQUIRED   0

#ifndef ENET_IPADDR
  #define ENET_IPADDR      IPADDR(169,254,3,3)
#endif

#ifndef ENET_IPMASK
   #define ENET_IPMASK     IPADDR(255,255,0,0)
#endif

#ifndef ENET_IPGATEWAY
   #define ENET_IPGATEWAY  IPADDR(0,0,0,0) 
#endif

#ifndef ENET_IPDNS
   #define ENET_IPDNS      IPADDR(0,0,0,0) 
#endif

#define  SNTP_SERVER        "time.nist.gov" 

#define  EMAIL_SERVER   "smtp.server.com"
#define  EMAIL_TO       "to@email.com"
#define  EMAIL_FROM     "from@email.com"
    
#define  AUTH_USERNAME  "username"
#define  AUTH_PASSWORD  "password"

#endif
