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
* $FileName: cgi.h$
* $Version : 3.8.10.0$
* $Date    : Sep-26-2011$
*
* Comments:
*
*   
*
*END************************************************************************/

#ifndef _cgi_h_
#define _cgi_h_


_mqx_int cgi_index(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_ipstat(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_icmpstat(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_udpstat(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_tcpstat(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_hvac_data(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_hvac_input(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_hvac_output(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_usbstat(HTTPD_SESSION_STRUCT *session);

_mqx_int cgi_analog_data(HTTPD_SESSION_STRUCT *session);
_mqx_int cgi_rtc_data(HTTPD_SESSION_STRUCT *session);

#define CGI_SEND_NUM(val)               \
{                                       \
    char str[20], *pstr;                        \
    pstr = &str[0]; \
        sprintf(pstr, "%ld\n", val);        \
    httpd_sendstr(session->sock, str);  \
}

#define CGI_SEND_STR(val)               \
{                                       \
    httpd_sendstr(session->sock, val);  \
    httpd_sendstr(session->sock, "\n"); \
}


#endif
