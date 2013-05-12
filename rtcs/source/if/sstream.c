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
* $FileName: sstream.c$
* $Version : 3.6.6.0$
* $Date    : Dec-1-2010$
*
* Comments:
*
*   This file contains the socket interface functions for
*   (PF_INET,SOCK_STREAM) sockets.
*
*END************************************************************************/

#include <rtcs.h>
#include "rtcs_prv.h"
#include "tcpip.h"
#include "socket.h"

#if RTCSCFG_ENABLE_TCP

#define RTCS_ENTER(f,a)    RTCSLOG_API(RTCSLOG_TYPE_FNENTRY, RTCSLOG_FN_STREAM_ ## f, a)

#define RTCS_EXIT(f,a)     RTCSLOG_API(RTCSLOG_TYPE_FNEXIT,  RTCSLOG_FN_STREAM_ ## f, a); \
                           return a

#define RTCS_EXIT2(f,a,e)  RTCSLOG_API(RTCSLOG_TYPE_FNEXIT,  RTCSLOG_FN_STREAM_ ## f, a); \
                           return e


uint_32 SOCK_STREAM_socket      (uint_32);
uint_32 SOCK_STREAM_bind        (uint_32, const sockaddr_in _PTR_, uint_16);
uint_32 SOCK_STREAM_connect     (uint_32, const sockaddr_in _PTR_, uint_16);
uint_32 SOCK_STREAM_listen      (uint_32, uint_32);
uint_32 SOCK_STREAM_accept      (uint_32, sockaddr_in _PTR_, uint_16 _PTR_);
uint_32 SOCK_STREAM_getsockname (uint_32, sockaddr_in _PTR_, uint_16 _PTR_);
uint_32 SOCK_STREAM_getpeername (uint_32, sockaddr_in _PTR_, uint_16 _PTR_);
 int_32 SOCK_STREAM_recv        (uint_32, pointer, uint_32, uint_32);
 int_32 SOCK_STREAM_send        (uint_32, pointer, uint_32, uint_32);
uint_32 SOCK_STREAM_shutdown    (uint_32, uint_32);

const RTCS_SOCKET_CALL_STRUCT SOCK_STREAM_CALL = {
   SOCK_STREAM_socket,
   SOCK_STREAM_bind,
   SOCK_STREAM_connect,
   SOCK_STREAM_listen,
   SOCK_STREAM_accept,
   SOCK_STREAM_getsockname,
   SOCK_STREAM_getpeername,
   SOCK_STREAM_recv,
   NULL,
   NULL,
   SOCK_STREAM_send,
   NULL,
   NULL,
   NULL,
   SOCK_STREAM_shutdown
};


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_socket
* Returned Value  : error code
* Comments  :  Creates an unbound socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_socket
   (
      uint_32     sock
         /* [IN] socket handle */
   )
{ /* Body */
   TCP_PARM    parms;
   uint_32     error;

   parms.SOCKET = sock;

   error = RTCSCMD_issue(parms, TCP_Process_create);
   if (error) return error;

   ((SOCKET_STRUCT_PTR)sock)->STATE   = SOCKSTATE_STREAM_GROUND;
   ((SOCKET_STRUCT_PTR)sock)->TCB_PTR = parms.TCB_PTR;
   return RTCS_OK;

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_bind
* Returned Value  : error code
* Comments  :  Binds the local endpoint of a socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_bind
   (
      uint_32                    sock,
         /* [IN] socket handle */
      const sockaddr_in _PTR_    localaddr,
         /* [IN] local address to which to bind the socket */
      uint_16                    addrlen
         /* [IN] length of the address, in bytes */
   )
{ /* Body */
   TCP_PARM    parms;
   uint_32     error;

   RTCS_ENTER(BIND, sock);

#if RTCSCFG_CHECK_ERRORS
   if (((SOCKET_STRUCT_PTR)sock)->STATE != SOCKSTATE_STREAM_GROUND) {
      RTCS_EXIT(BIND, RTCSERR_SOCK_IS_BOUND);
   } /* Endif */
#endif

#if RTCSCFG_CHECK_ADDRSIZE
   if (addrlen < sizeof(sockaddr_in)) {
      RTCS_EXIT(BIND, RTCSERR_SOCK_SHORT_ADDRESS);
   } /* Endif */
#endif

#if RTCSCFG_CHECK_ERRORS
   if (localaddr->sin_family != AF_INET) {
      RTCS_EXIT(BIND, RTCSERR_SOCK_INVALID_AF);
   } /* Endif */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_EXIT(BIND, RTCSERR_TCP_CONN_RLSD);
   } /* Endif */

   parms.TCB_PTR  = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR;
   parms.IP_ADDR  = localaddr->sin_addr.s_addr;
   parms.TCP_PORT = localaddr->sin_port;

   error = RTCSCMD_issue(parms, TCP_Process_bind);
   if (error) {
      RTCS_EXIT(BIND, error);
   } /* Endif */

   ((SOCKET_STRUCT_PTR)sock)->LOCAL_PORT = localaddr->sin_port;

   ((SOCKET_STRUCT_PTR)sock)->STATE = SOCKSTATE_STREAM_BOUND;
   RTCS_EXIT(BIND, RTCS_OK);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_connect
* Returned Value  : error code
* Comments  :  Binds the remote endpoint of a socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_connect
   (
      uint_32                    sock,
         /* [IN] socket handle */
      const sockaddr_in _PTR_    peeraddr,
         /* [IN] remote address to which to bind the socket */
      uint_16                    addrlen
         /* [IN] length of the address, in bytes */
   )
{ /* Body */
            TCP_PARM             parms;
            TCP_OPTIONS_STRUCT   options;
   register SOCKET_STRUCT_PTR    socket_ptr = (SOCKET_STRUCT_PTR)sock;
            uint_32              error;

   RTCS_ENTER(CONNECT, sock);

#if RTCSCFG_CHECK_ERRORS
   switch (((SOCKET_STRUCT_PTR)sock)->STATE) {
   case SOCKSTATE_STREAM_LISTENING: RTCS_EXIT(CONNECT, RTCSERR_SOCK_IS_LISTENING);
   case SOCKSTATE_STREAM_CONNECTED: RTCS_EXIT(CONNECT, RTCSERR_SOCK_IS_CONNECTED);
   } /* Endswitch */
#endif

#if RTCSCFG_CHECK_ADDRSIZE
   if (addrlen < sizeof(sockaddr_in)) {
      RTCS_EXIT(CONNECT, RTCSERR_SOCK_SHORT_ADDRESS);
   } /* Endif */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_EXIT(CONNECT, RTCSERR_TCP_CONN_RLSD);
   } /* Endif */

   if (((SOCKET_STRUCT_PTR)sock)->STATE == SOCKSTATE_STREAM_GROUND) {
      sockaddr_in localaddr;
      localaddr.sin_family      = AF_INET;
      localaddr.sin_port        = 0;
      localaddr.sin_addr.s_addr = INADDR_ANY;
      error = SOCK_STREAM_bind(sock, &localaddr, sizeof(localaddr));
      if (error) {
         RTCS_EXIT(CONNECT, error);
      } /* Endif */
   } /* Endif */

   /*
   **  Setup the known 'connect' options
   */
   options.remhost    = peeraddr->sin_addr.s_addr;
   options.remport    = peeraddr->sin_port;
   options.utimeout   = socket_ptr->SEND_TIMEOUT;
   options.timeout    = socket_ptr->CONNECT_TIMEOUT;
   options.rto        = socket_ptr->RETRANSMISSION_TIMEOUT;
   options.active     = socket_ptr->ACTIVE;
   options.tbsize     = socket_ptr->TBSIZE;
   options.rbsize     = socket_ptr->RBSIZE;
   options.maxrto     = socket_ptr->MAXRTO;
   options.maxrcvwnd  = socket_ptr->MAXRCV_WND;
   options.keepalive  = socket_ptr->KEEPALIVE;
   options.nowait     = socket_ptr->NOWAIT;
   options.nonagle    = socket_ptr->NO_NAGLE_ALGORITHM;
   options.noswrbuf   = socket_ptr->NOSWRBUF;
   options.timewaitto = socket_ptr->TIMEWAIT_TIMEOUT;
   /* Start CR 1576 */
   options.tcpsecuredraft0 = socket_ptr->TCPSECUREDRAFT_0;
   /* End CR 1576 */
   options.delayackto = socket_ptr->DELAY_ACK;

   /*
   **  Setup the default 'connect' options
   */
   options.tos        = 0;
   options.precede    = 0xFF;
   options.ttl        = 0;

   parms.TCB_PTR     = socket_ptr->TCB_PTR;
   parms.OPTIONS_PTR = &options;

   error = RTCSCMD_issue(parms, TCP_Process_open);
   if (error) {
      RTCS_EXIT(CONNECT, error);
   } /* Endif */

   ((SOCKET_STRUCT_PTR)sock)->STATE = SOCKSTATE_STREAM_CONNECTED;
   RTCS_EXIT(CONNECT, RTCS_OK);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_listen
* Returned Value  : error code
* Comments  :  Enables remote connections on a socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_listen
   (
      uint_32     sock,
         /* [IN] socket handle */
      uint_32     backlog
         /* [IN] not implemented */
   )
{ /* Body */
            TCP_PARM             parms;
            TCP_OPTIONS_STRUCT   options;
   register SOCKET_STRUCT_PTR    socket_ptr = (SOCKET_STRUCT_PTR)sock;
            uint_32              error;

   RTCS_ENTER(LISTEN, sock);

#if RTCSCFG_CHECK_ERRORS
   switch (((SOCKET_STRUCT_PTR)sock)->STATE) {
   case SOCKSTATE_STREAM_GROUND:    RTCS_EXIT(LISTEN, RTCSERR_SOCK_NOT_BOUND);
   case SOCKSTATE_STREAM_LISTENING: RTCS_EXIT(LISTEN, RTCSERR_SOCK_IS_LISTENING);
   case SOCKSTATE_STREAM_CONNECTED: RTCS_EXIT(LISTEN, RTCSERR_SOCK_IS_CONNECTED);
   } /* Endswitch */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_EXIT(LISTEN, RTCSERR_TCP_CONN_RLSD);
   } /* Endif */

   /*
   **  Setup the known 'connect' options
   */
   options.remhost    = INADDR_ANY;
   options.remport    = 0;
   options.utimeout   = socket_ptr->SEND_TIMEOUT;
   options.timeout    = socket_ptr->CONNECT_TIMEOUT;
   options.rto        = socket_ptr->RETRANSMISSION_TIMEOUT;
   options.active     = FALSE;
   options.tbsize     = socket_ptr->TBSIZE;
   options.rbsize     = socket_ptr->RBSIZE;
   options.maxrto     = socket_ptr->MAXRTO;
   options.maxrcvwnd  = socket_ptr->MAXRCV_WND;
   options.keepalive  = socket_ptr->KEEPALIVE;
   options.nowait     = socket_ptr->NOWAIT;
   options.nonagle    = socket_ptr->NO_NAGLE_ALGORITHM;
   options.noswrbuf   = socket_ptr->NOSWRBUF;
   options.timewaitto = socket_ptr->TIMEWAIT_TIMEOUT;
   options.delayackto = socket_ptr->DELAY_ACK;

   /*
   **  Setup the default 'connect' options
   */
   options.tos        = 0;
   options.precede    = 0xFF;
   options.ttl        = 0;

   parms.TCB_PTR     = socket_ptr->TCB_PTR;
   parms.OPTIONS_PTR = &options;
   parms.SOCKET      = sock;

   error = RTCSCMD_issue(parms, TCP_Process_open);
   if (error) {
      RTCS_EXIT(LISTEN, error);
   } /* Endif */

   ((SOCKET_STRUCT_PTR)sock)->STATE = SOCKSTATE_STREAM_LISTENING;
   RTCS_EXIT(LISTEN, RTCS_OK);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_accept
* Returned Value  : socket
* Comments  :  Wait for a connection on a socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_accept
   (
      uint_32              sock,
         /* [IN] socket handle */
      sockaddr_in _PTR_    peeraddr,
         /* [OUT] remote address to which the socket is connected */
      uint_16     _PTR_    addrlen
         /* [IN/OUT] length of the address, in bytes */
   )
{ /* Body */
   TCP_PARM             parms;
   SOCKET_STRUCT_PTR    socket_ptr = (SOCKET_STRUCT_PTR)sock;
   SOCKET_STRUCT_PTR    new_socket_ptr;
   uint_32              error;
   uint_32              newsock;

   RTCS_ENTER(ACCEPT, sock);

#if RTCSCFG_CHECK_ERRORS
   switch (((SOCKET_STRUCT_PTR)sock)->STATE) {
   case SOCKSTATE_STREAM_GROUND:
            RTCS_setsockerror(sock, RTCSERR_SOCK_NOT_LISTENING);
            RTCS_EXIT2(ACCEPT, RTCSERR_SOCK_NOT_LISTENING, RTCS_SOCKET_ERROR);
   case SOCKSTATE_STREAM_BOUND:
            RTCS_setsockerror(sock, RTCSERR_SOCK_NOT_LISTENING);
            RTCS_EXIT2(ACCEPT, RTCSERR_SOCK_NOT_LISTENING, RTCS_SOCKET_ERROR);
   case SOCKSTATE_STREAM_CONNECTED:
            RTCS_setsockerror(sock, RTCSERR_SOCK_IS_CONNECTED);
            RTCS_EXIT2(ACCEPT, RTCSERR_SOCK_IS_CONNECTED, RTCS_SOCKET_ERROR);
   } /* Endswitch */
#endif

   /*
   ** Allocate a new socket structure for the established TCB
   */
   new_socket_ptr = SOCK_Get_sock_struct((RTCS_SOCKET_CALL_STRUCT_PTR)SOCK_STREAM,
                                         RTCS_task_getid());
   newsock = (uint_32)new_socket_ptr;

   if (new_socket_ptr == NULL) {
      RTCS_EXIT2(ACCEPT, RTCSERR_OUT_OF_SOCKETS, RTCS_SOCKET_ERROR);
   } /* Endif */

   parms.TCB_PTR = socket_ptr->TCB_PTR;
   parms.SOCKET = newsock;

   error = RTCSCMD_issue(parms, TCP_Process_accept);
   if (error) {
      SOCK_Free_sock_struct(new_socket_ptr);
      RTCS_EXIT2(ACCEPT, error, RTCS_SOCKET_ERROR);
   } /* Endif */

   new_socket_ptr->STATE = SOCKSTATE_STREAM_CONNECTED;
   
   error = SOCK_STREAM_getpeername(newsock, peeraddr, addrlen);

   /*
   ** If there is an error, it is because the TCB was freed (by a RST).
   ** We will free the socket and return a socket error
   */
   if (error) {
      SOCK_Free_sock_struct(new_socket_ptr);
      RTCS_EXIT2(ACCEPT, error, RTCS_SOCKET_ERROR);
   } /* Endif */

   /*
   ** inherit the socket options from listen socket
   */
   new_socket_ptr->CONNECT_TIMEOUT        = socket_ptr->CONNECT_TIMEOUT;
   new_socket_ptr->RETRANSMISSION_TIMEOUT = socket_ptr->RETRANSMISSION_TIMEOUT;
   new_socket_ptr->SEND_TIMEOUT           = socket_ptr->SEND_TIMEOUT;
   new_socket_ptr->RECEIVE_TIMEOUT        = socket_ptr->RECEIVE_TIMEOUT;
   new_socket_ptr->RECEIVE_PUSH           = socket_ptr->RECEIVE_PUSH;
   new_socket_ptr->SEND_NOWAIT            = socket_ptr->SEND_NOWAIT;
   new_socket_ptr->SEND_WAIT              = socket_ptr->SEND_WAIT;
   new_socket_ptr->SEND_PUSH              = socket_ptr->SEND_PUSH;
   new_socket_ptr->RECEIVE_NOWAIT         = socket_ptr->RECEIVE_NOWAIT;
   new_socket_ptr->CHECKSUM_BYPASS        = socket_ptr->CHECKSUM_BYPASS;
   new_socket_ptr->ACTIVE                 = socket_ptr->ACTIVE;
   new_socket_ptr->TBSIZE                 = socket_ptr->TBSIZE;
   new_socket_ptr->RBSIZE                 = socket_ptr->RBSIZE;
   new_socket_ptr->MAXRTO                 = socket_ptr->MAXRTO;
   new_socket_ptr->MAXRCV_WND             = socket_ptr->MAXRCV_WND;
   new_socket_ptr->KEEPALIVE              = socket_ptr->KEEPALIVE;
   new_socket_ptr->NOWAIT                 = socket_ptr->NOWAIT;
   new_socket_ptr->NO_NAGLE_ALGORITHM     = socket_ptr->NO_NAGLE_ALGORITHM;
   new_socket_ptr->NOSWRBUF               = socket_ptr->NOSWRBUF;
   new_socket_ptr->CALL_BACK              = socket_ptr->CALL_BACK;
   new_socket_ptr->TYPE_MIRROR            = socket_ptr->TYPE_MIRROR;
   new_socket_ptr->TIMEWAIT_TIMEOUT       = socket_ptr->TIMEWAIT_TIMEOUT;
   new_socket_ptr->LOCAL_PORT             = socket_ptr->LOCAL_PORT;
   /* Start CR 1144 */
   new_socket_ptr->LINK_OPTIONS           = socket_ptr->LINK_OPTIONS;
   /* End CR 1144 */

   RTCS_EXIT2(ACCEPT, RTCS_OK, newsock);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_send
* Returned Value  : number of bytes sent or RTCS_ERROR
* Comments  :  Send data to a connected socket.
*
*END*-----------------------------------------------------------------*/

int_32  SOCK_STREAM_send
   (
      uint_32        sock,
         /* [IN] socket handle */
      pointer        buffer,
         /* [IN] data to transmit */
      uint_32        buflen,
         /* [IN] length of the buffer, in bytes */
      uint_32        flags
         /* [IN] flags to underlying protocols */
   )
{ /* Body */
            TCP_PARM             parms;
   register SOCKET_STRUCT_PTR    socket_ptr = (SOCKET_STRUCT_PTR)sock;
            uint_32              error;

   RTCS_ENTER(SEND, sock);

#if RTCSCFG_CHECK_ERRORS
   if (((SOCKET_STRUCT_PTR)sock)->STATE != SOCKSTATE_STREAM_CONNECTED) {
      RTCS_setsockerror(sock, RTCSERR_SOCK_NOT_CONNECTED);
      RTCS_EXIT2(SEND, RTCSERR_SOCK_NOT_CONNECTED, RTCS_ERROR);
   } /* Endif */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_setsockerror(sock, RTCSERR_TCP_CONN_RLSD);
      RTCS_EXIT2(SEND, RTCSERR_TCP_CONN_RLSD, RTCS_ERROR);
   } /* Endif */

   if (flags == 0) {
      if (socket_ptr->SEND_NOWAIT) flags |= TCPO_NOWAIT;
      if (socket_ptr->SEND_WAIT)   flags |= TCPO_WAIT;
      if (socket_ptr->SEND_PUSH)   flags |= TCPO_PUSH;
   } /* Endif */

   parms.TCB_PTR    = socket_ptr->TCB_PTR;
   parms.SOCKET     = sock;
   parms.BUFFER_PTR = buffer;
   parms.BUFF_LENG  = buflen;
   parms.TIMEOUT    = socket_ptr->SEND_TIMEOUT;
   parms.OPTIONS    = flags;

   error = RTCSCMD_issue(parms, TCP_Process_send);
   if (error) {
      RTCS_setsockerror(sock, error);
      RTCS_EXIT2(SEND, error, RTCS_ERROR);
   } /* Endif */

   RTCS_EXIT2(SEND, RTCS_OK, parms.BUFF_LENG);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_recv
* Returned Value  : number of bytes received or RTCS_ERROR
* Comments  :  Receive data from a connected socket.
*
*END*-----------------------------------------------------------------*/

int_32  SOCK_STREAM_recv
   (
      uint_32        sock,
         /* [IN] socket handle */
      pointer        buffer,
         /* [IN] buffer for received data */
      uint_32        buflen,
         /* [IN] length of the buffer, in bytes */
      uint_32        flags
         /* [IN] flags to underlying protocols */
   )
{ /* Body */
            TCP_PARM             parms;
   register SOCKET_STRUCT_PTR    socket_ptr = (SOCKET_STRUCT_PTR)sock;
            uint_32              error;

   RTCS_ENTER(RECV, sock);

#if RTCSCFG_CHECK_ERRORS
   if (((SOCKET_STRUCT_PTR)sock)->STATE != SOCKSTATE_STREAM_CONNECTED) {
      RTCS_setsockerror(sock, RTCSERR_SOCK_NOT_CONNECTED);
      RTCS_EXIT2(RECV, RTCSERR_SOCK_NOT_CONNECTED, RTCS_ERROR);
   } /* Endif */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_setsockerror(sock, RTCSERR_TCP_CONN_RLSD);
      RTCS_EXIT2(RECV, RTCSERR_TCP_CONN_RLSD, RTCS_ERROR);
   } /* Endif */

   if (flags == 0) {
      if (socket_ptr->RECEIVE_NOWAIT) flags |= TCPO_NOWAIT;
                                 else flags |= TCPO_WAIT;
      if (socket_ptr->RECEIVE_PUSH)   flags |= TCPO_PUSH;
   } /* Endif */

   parms.TCB_PTR    = socket_ptr->TCB_PTR;
   parms.SOCKET     = sock;
   parms.BUFFER_PTR = buffer;
   parms.BUFF_LENG  = buflen;
   parms.TIMEOUT    = socket_ptr->RECEIVE_TIMEOUT;
   parms.OPTIONS    = flags;

   error = RTCSCMD_issue(parms, TCP_Process_receive);
   if (error) {
      RTCS_setsockerror(sock, error);
  
      /* Start CR 2340 */
      /* If data was copied to the userbuf, but not all that
         the recv() asked for, and a timer was started that has
         now timed out, we need to return with the count, and not
         RTCS_ERROR */
      if (error == RTCSERR_TCP_TIMED_OUT) {
         /* rcvusernxt is the number of data bytes in rcvuserbuf.
            TCB comments say this is undefined in the special case 
            when rcvuserbuf == rcvbuf.  However, TCP_Setup_receive() 
            does not use the rcvuserbuf when there is a timeout.  So
            this test is not needed, but do it anyway to be safe. */
         if (parms.TCB_PTR->rcvuserbuf != parms.TCB_PTR->rcvbuf) {
            if (parms.TCB_PTR->rcvusernxt > 0) {
               RTCS_EXIT2(RECV, RTCS_OK, parms.TCB_PTR->rcvusernxt);
            }
         }
      }
      /* End CR 2340 */
      
      RTCS_EXIT2(RECV, error, RTCS_ERROR);
   } /* Endif */

   RTCS_EXIT2(RECV, RTCS_OK, parms.BUFF_LENG);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_getsockname
* Returned Value  : error code
* Comments  :  Retrieve the address of the local endpoint of a
*              bound socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_getsockname
   (
      uint_32              sock,
         /* [IN] socket handle */
      sockaddr_in _PTR_    name,
         /* [OUT] address of local endpoint */
      uint_16     _PTR_    namelen
         /* [IN/OUT] length of the address, in bytes */
   )
{ /* Body */

   RTCS_ENTER(GETSOCKNAME, sock);

#if RTCSCFG_CHECK_ERRORS
   if (((SOCKET_STRUCT_PTR)sock)->STATE == SOCKSTATE_STREAM_GROUND) {
      RTCS_EXIT(GETSOCKNAME, RTCSERR_SOCK_NOT_BOUND);
   } /* Endif */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_EXIT(GETSOCKNAME, RTCSERR_TCP_CONN_RLSD);
   } /* Endif */

   if (!namelen) {

#if RTCSCFG_CHECK_ADDRSIZE
   } else if (*namelen < sizeof(sockaddr_in)) {
      sockaddr_in fullname;
      fullname.sin_family      = AF_INET;
      fullname.sin_port        = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->local_port;
      fullname.sin_addr.s_addr = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->local_host;
      _mem_copy(&fullname, name, *namelen);
      *namelen = sizeof(sockaddr_in);
#endif

   } else {
      name->sin_family      = AF_INET;
      name->sin_port        = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->local_port;
      name->sin_addr.s_addr = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->local_host;
      *namelen = sizeof(sockaddr_in);

   } /* Endif */

   RTCS_EXIT(GETSOCKNAME, RTCS_OK);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_getpeername
* Returned Value  : error code
* Comments  :  Retrieve the address of the remote endpoint of a
*              connected socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_getpeername
   (
      uint_32              sock,
         /* [IN] socket handle */
      sockaddr_in _PTR_    name,
         /* [IN] address of remote endpoint */
      uint_16     _PTR_    namelen
         /* [IN/OUT] length of the address, in bytes */
   )
{ /* Body */

   RTCS_ENTER(GETPEERNAME, sock);

#if RTCSCFG_CHECK_ERRORS
   if (((SOCKET_STRUCT_PTR)sock)->STATE != SOCKSTATE_STREAM_CONNECTED) {
      RTCS_EXIT(GETPEERNAME, RTCSERR_SOCK_NOT_CONNECTED);
   } /* Endif */
#endif

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_EXIT(GETPEERNAME, RTCSERR_TCP_CONN_RLSD);
   } /* Endif */

   if (!namelen) {

#if RTCSCFG_CHECK_ADDRSIZE
   } else if (*namelen < sizeof(sockaddr_in)) {
      sockaddr_in fullname;
      fullname.sin_family      = AF_INET;
      fullname.sin_port        = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->remote_port;
      fullname.sin_addr.s_addr = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->remote_host;
      _mem_copy(&fullname, name, *namelen);
      *namelen = sizeof(sockaddr_in);
#endif

   } else {
      name->sin_family      = AF_INET;
      name->sin_port        = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->remote_port;
      name->sin_addr.s_addr = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR->remote_host;
      *namelen = sizeof(sockaddr_in);

   } /* Endif */

   RTCS_EXIT(GETPEERNAME, RTCS_OK);

} /* Endbody */


/*FUNCTION*-------------------------------------------------------------
*
* Function Name   : SOCK_STREAM_shutdown
* Returned Value  : error code
* Comments  :  Close a socket.
*
*END*-----------------------------------------------------------------*/

uint_32  SOCK_STREAM_shutdown
   (
      uint_32        sock,
         /* [IN] socket handle */
      uint_32        how
         /* [IN] how to terminate the socket */
   )
{ /* Body */
   TCP_PARM    parms;
   uint_32     error;

   RTCS_ENTER(SHUTDOWN, sock);

   if (!((SOCKET_STRUCT_PTR)sock)->TCB_PTR) {
      RTCS_EXIT(SHUTDOWN, RTCS_OK);
   } /* Endif */

   parms.TCB_PTR = ((SOCKET_STRUCT_PTR)sock)->TCB_PTR;

   if (how == FLAG_ABORT_CONNECTION) {
      error = RTCSCMD_issue(parms, TCP_Process_abort);
   } else {
      error = RTCSCMD_issue(parms, TCP_Process_close);
   } /* Endif */

   if (error) {
      RTCS_EXIT(SHUTDOWN, error);
   } /* Endif */

   RTCS_EXIT(SHUTDOWN, RTCS_OK);

} /* Endbody */

#endif

/* EOF */
