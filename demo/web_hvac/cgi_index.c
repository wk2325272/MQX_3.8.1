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
* $FileName: cgi_index.c$
* $Version : 3.8.16.4$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example of shell using RTCS.
*
*END************************************************************************/

#include "hvac.h"
#if DEMOCFG_ENABLE_WEBSERVER
#include "html.h"
#include "cgi.h"

#include <string.h>
#include <stdlib.h>


extern LWSEM_STRUCT      USB_Stick;

static void usb_status_fn(HTTPD_SESSION_STRUCT *session);

const HTTPD_FN_LINK_STRUCT fn_lnk_tbl[] = { { "usb_status_fn", usb_status_fn }, { 0, 0 } };

const HTTPD_CGI_LINK_STRUCT cgi_lnk_tbl[] = {
//  { "index",          cgi_index},
    { "ipstat",         cgi_ipstat},
    { "icmpstat",       cgi_icmpstat},
    { "udpstat",        cgi_udpstat},
    { "tcpstat",        cgi_tcpstat},
    { "hvacdata",       cgi_hvac_data},
    { "analog",         cgi_analog_data},
    { "rtcdata",        cgi_rtc_data},

//  { "hvacinput",      cgi_hvac_input},
    { "hvacoutput",     cgi_hvac_output},
    { "usbstat", 		cgi_usbstat},
    { 0, 0 }    // DO NOT REMOVE - last item - end of table
};

static boolean usbstick_attached() {
#if DEMOCFG_ENABLE_USB_FILESYSTEM
    if (_lwsem_poll(&USB_Stick)) {
        _lwsem_post(&USB_Stick);
        return TRUE;
    } else
#endif      
        return FALSE;
}

static void usb_status_fn(HTTPD_SESSION_STRUCT *session) {
    if (usbstick_attached()) {
        httpd_sendstr(session->sock, "visible");
    } else {
        httpd_sendstr(session->sock, "hidden");
    }
}

static _mqx_int cgi_usbstat(HTTPD_SESSION_STRUCT *session) {
    session->response.contenttype = CONTENT_TYPE_PLAIN;
    httpd_sendhdr(session, 0, 0);
    if (usbstick_attached()) {
        CGI_SEND_STR("visible");
    } else {
        CGI_SEND_STR("hidden");
    }
    return session->request.content_len;
}

static _mqx_int cgi_analog_data(HTTPD_SESSION_STRUCT *session) {
    int i;
    static int aval = 0;
    
    session->response.contenttype = CONTENT_TYPE_PLAIN;
    httpd_sendhdr(session, 0, 0);

    for (i = 0; i < 8; i++) {
        CGI_SEND_NUM(aval += 50);
    }

    if (aval > 3500)
        aval = 0;

    return session->request.content_len;
}

static _mqx_int cgi_rtc_data(HTTPD_SESSION_STRUCT *session) {
    TIME_STRUCT time;
    int min, hour;
    
    _time_get(&time);
    
    session->response.contenttype = CONTENT_TYPE_PLAIN;
    httpd_sendhdr(session, 0, 0);

    min = time.SECONDS / 60;
    hour = min / 60;
    min %= 60;
    
    CGI_SEND_NUM(hour);
    CGI_SEND_NUM(min);
    CGI_SEND_NUM(time.SECONDS % 60);

    return session->request.content_len;
}
   
#if 0
static _mqx_int cgi_index(HTTPD_SESSION_STRUCT *session) {
    session->response.contenttype = CONTENT_TYPE_HTML;
    httpd_sendhdr(session, 0, 0);

    html_head(session->sock, "MQX Demo");
     
    httpd_sendstr(session->sock,
        "<BODY><table width=\"600\" border=\"0\">\n"
        "<tr align=\"center\">\n"
        "<td height=\"85\" colspan=\"3\"><p align=\"center\"> \n"
        "<strong><font size=6 font color=#0000CC><u>MQX Demo Home Page</u></font> </p></td>\n"
        "</tr>\n"
        "<tr>\n"
        "<td></td>\n"
        "<td align=\"center\" height=\"35\" bgcolor=\"#FFDD00\"><strong><A HREF=\"ipstat.cgi\">Network Stats</A></strong></td>\n"
        "<td></td>\n"
        "</tr>\n"
        "<tr>\n"
        "<td></td>\n"
        "<td align=\"center\" height=\"35\" bgcolor=\"#D0D88E\"><strong><A HREF=\"hvac.cgi\">HVAC Info</A></strong></td>\n"
        "<td></td>\n"
        "</tr>\n");

    /*tries = 0;
    while ((!USB_Stick.VALUE) && (tries<8)){
        _time_delay(250);
        tries++;
    }*/
   
    if (USB_Stick.VALUE) {      
        httpd_sendstr(session->sock,
            "<tr>\n"
            "<td></td>\n"
            "<td align=\"center\" height=\"35\" bgcolor=\"#B8D2E4\"><strong><A HREF=\"index.htm\">Browse USB</A></strong></td>\n"
            "<td></td>\n"
            "</tr>\n"
            "<td></td>\n"
            "<td><div align=\"center\"><img src=\"Logo1.jpg\"></div></td>\n"
            "<td></td>\n"
            "</table>\n</BODY>\n");
            
    }
    
    httpd_sendstr(session->sock, "</html>");
    return session->request.content_len;
}

#endif

#endif
