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
* $FileName: lw_telnet.c$
* $Version : 3.6.3.0$
* $Date    : Jun-4-2010$
*
* Comments:
*
*   This file contains the light weight Telnet server implementation.
*
*END************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>
#include "lw_telnet.h"

#define return_error_if(c) { if (c) return IO_ERROR; }

/*TASK*-----------------------------------------------------------------
*
* Function Name    : lw_telnet_server
* Returned Value   : none
* Comments  :  A light weight TCP/IP Telnet Server
*
*END*-----------------------------------------------------------------*/

_mqx_int lw_telnet_server(void (_CODE_PTR_ user_fn)(void))
{
   uint_32        listensock, sock;
   sockaddr_in    addr;
   uint_32        error, option;
   MQX_FILE_PTR   sockfd, telnetfd;
   _mqx_uint      echoflag = IO_SERIAL_ECHO;

 
   /* Install device drivers for socket and telnet I/O */
   _io_socket_install("socket:");
   _io_telnet_install("telnet:");

   listensock = socket(PF_INET, SOCK_STREAM, 0);
   return_error_if(listensock == RTCS_SOCKET_ERROR);

   option = TELNETDCFG_BUFFER_SIZE;   
   error = setsockopt(listensock, SOL_TCP, OPT_TBSIZE, &option, sizeof(option));
   return_error_if(error != RTCS_OK);

   option = TELNETDCFG_BUFFER_SIZE;   
   error = setsockopt(listensock, SOL_TCP, OPT_RBSIZE, &option, sizeof(option));
   return_error_if(error != RTCS_OK);

   option = TELENETDCFG_TIMEWAIT_TIMEOUT;   
   error = setsockopt(listensock, SOL_TCP, OPT_TIMEWAIT_TIMEOUT, &option, sizeof(option));
   return_error_if(error != RTCS_OK);


   /* Bind the socket to the Telnet port */
   addr.sin_family      = AF_INET;
   addr.sin_port        = IPPORT_TELNET;
   addr.sin_addr.s_addr = INADDR_ANY;

   error = bind(listensock, &addr, sizeof(addr));
   return_error_if(error != RTCS_OK);

   /* Put the socket into listening mode */
   error = listen(listensock, 0);
   return_error_if(error != RTCS_OK);

   do {
      /* Wait for a connection */
      sock= accept(listensock, NULL, NULL);
      if (sock != RTCS_SOCKET_ERROR) {
         sockfd = fopen("socket:", (char_ptr)sock);
         if (sockfd != NULL) {
            telnetfd = fopen("telnet:", (char_ptr)sockfd);
            if (telnetfd != NULL) {

               ioctl(telnetfd, IO_IOCTL_SERIAL_SET_FLAGS, &echoflag);

               _io_set_handle(IO_STDIN, telnetfd);
               _io_set_handle(IO_STDOUT, telnetfd);

               (*user_fn)();
               /*
               ** Allow some time for queued data to go out.
               */
               RTCS_time_delay(100);
               fclose(telnetfd);
            }
            fclose(sockfd);
         }
         shutdown(sock, FLAG_CLOSE_TX);
      } 
   }  while (sock != RTCS_SOCKET_ERROR);
   shutdown(listensock, FLAG_CLOSE_TX);
   return RTCS_OK;
} 


/* EOF */
