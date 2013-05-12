
/**HEADER********************************************************************
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
* $FileName: html_util.c$
* $Version : 3.8.7.1$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example of shell using RTCS.
*
*END************************************************************************/

#include "hvac.h"

#if DEMOCFG_ENABLE_WEBSERVER
#include "httpd.h"
#include "html.h"

void html_head(int s, char_ptr title) {
    httpd_sendstr(s, "<HTML>\n<HEAD>\n<TITLE>");
    httpd_sendstr(s, title);
    httpd_sendstr(s, "</TITLE>\n</HEAD>\n\n");
}
 
void html_heading(int s, char_ptr title) {
    httpd_sendstr(s, "<H1><EM>");
    httpd_sendstr(s, title);
    httpd_sendstr(s, "</H1></EM><BR>\n");
}

void html_link(int s, char_ptr text, char_ptr url) {
    httpd_sendstr(s, "<A HREF=\"");
    httpd_sendstr(s, url);
    httpd_sendstr(s, "\">");
    httpd_sendstr(s, text);
    httpd_sendstr(s, "</A>\n");
}


void html_radio_selection(int s, char_ptr group, char_ptr item, boolean selected) {
    httpd_sendstr(s, "<input type=\"radio\" name=\"");
    httpd_sendstr(s, group);
    httpd_sendstr(s, "\" value=\"");
    httpd_sendstr(s, item);
    httpd_sendstr(s, "\"");
    httpd_sendstr(s, selected ? "\"checked>" : "\">");
    httpd_sendstr(s, item);
    httpd_sendstr(s, "&nbsp\n");
}

#endif
