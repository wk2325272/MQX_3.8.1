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
* $FileName: sh_rtcs.h$
* $Version : 3.7.6.0$
* $Date    : Feb-24-2011$
*
* Comments:
*
*   This file contains the RTCS shell.
*
*END************************************************************************/

#ifndef __sh_rtcs_h__
#define __sh_rtcs_h__

#define MAX_HOSTNAMESIZE     64


#define eaddrassign(p,x)   ((p)[0] = (x)[0], \
                           (p)[1] = (x)[1], \
                           (p)[2] = (x)[2], \
                           (p)[3] = (x)[3], \
                           (p)[4] = (x)[4], \
                           (p)[5] = (x)[5]  \
                          )

#define eaddriszero(p)   ( ((p)[0] == 0) && \
                           ((p)[1] == 0) && \
                           ((p)[2] == 0) && \
                           ((p)[3] == 0) && \
                           ((p)[4] == 0) && \
                           ((p)[5] == 0)    \
                          )

/*
** Function prototypes 
*/
#ifdef __cplusplus
extern "C" {
#endif

extern boolean Shell_parse_ip_address( char_ptr arg, _ip_address _PTR_ ipaddr_ptr);
extern boolean Shell_parse_netmask( char_ptr arg, _ip_address _PTR_ ipaddr_ptr);

extern int_32 Shell_netstat(int_32 argc, char_ptr argv[] );
extern int_32 Shell_EDS(int_32 argc, char_ptr argv[] ); 
extern int_32 Shell_ping(int_32 argc, char_ptr argv[] ); 
extern int_32 Shell_Telnet_client(int_32 argc, char_ptr argv[] );
extern int_32 Shell_FTP_client(int_32 argc, char_ptr argv[] );
extern int_32 Shell_TFTP_client(int_32 argc, char_ptr argv[] );
extern int_32 Shell_get_host_by_name(int_32 argc, char_ptr argv[] ); 
extern int_32 Shell_Telnetd(int_32 argc, char_ptr argv[] );
extern int_32 Shell_Echod(int_32 argc, char_ptr argv[] );
extern int_32 Shell_SNMPd(int_32 argc, char_ptr argv[] );
extern int_32 Shell_FTPd(int_32 argc, char_ptr argv[] );
extern int_32 Shell_TFTPd(int_32 argc, char_ptr argv[] );
extern int_32 Shell_type(int_32 argc, char_ptr argv[] );
extern int_32 Shell_exec(int_32 argc, char_ptr argv[] );
extern int_32 Shell_load(int_32 argc, char_ptr argv[] );
extern int_32 Shell_walkroute(int_32 argc, char_ptr argv[] );
extern int_32 Shell_gate(int_32 argc, char_ptr argv[] ); 
extern int_32 Shell_getroute(int_32 argc, char_ptr argv[] );


extern int_32 Shell_arpdisp(int_32 argc, char_ptr argv[] );
extern int_32 Shell_arpadd(int_32 argc, char_ptr argv[] );
extern int_32 Shell_arpdel(int_32 argc, char_ptr argv[] );
extern int_32 Shell_sendto(int_32 argc, char_ptr argv[] );


extern int_32 Shell_nat(int_32 argc, char_ptr argv[] );
extern int_32 Shell_dnat(int_32 argc, char_ptr argv[] );
extern int_32 Shell_natinfo(int_32 argc, char_ptr argv[] );
extern int_32 Shell_natinit(int_32 argc, char_ptr argv[] );
extern int_32 Shell_dhcptest(int_32 argc, char_ptr argv[] );

extern int_32 Shell_Clock_server_start(int_32 argc, char_ptr argv[] );

extern int_32 Shell_ipconfig(int_32 argc, char_ptr argv[] );
extern int_32 Shell_iwconfig(int_32 argc, char_ptr argv[] );

#ifdef __cplusplus
}
#endif

#endif
/*EOF*/
