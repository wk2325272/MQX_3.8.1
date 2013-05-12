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
* $FileName: config.h$
* $Version : 3.8.1.0$
* $Date    : Jul-19-2011$
*
* Comments:
*
*   Configurable information for the RTCS dual webserver demo.
*
*END************************************************************************/
#ifndef _config_h_
#define _config_h_

#ifndef ENET_IPADDR
	#define ENET_IPADDR_0  IPADDR(169,254,3,3) 
    #define ENET_IPADDR_1  IPADDR(169,254,3,2) 
#endif

#ifndef ENET_IPMASK
	#define ENET_IPMASK  IPADDR(255,255,255,0) 
#endif


#ifndef DEMOCFG_DEFAULT_DEVICE
#define DEMOCFG_DEFAULT_DEVICE_0 0
#define DEMOCFG_DEFAULT_DEVICE_1 1
#endif

#endif /* _config_h_ */
