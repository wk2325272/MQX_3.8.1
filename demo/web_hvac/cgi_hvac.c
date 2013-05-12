
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
* $FileName: cgi_hvac.c$
* $Version : 3.8.16.3$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example of shell using RTCS.
*
*END************************************************************************/

#include "hvac.h"
#include "hvac_public.h"

#if DEMOCFG_ENABLE_WEBSERVER
#include "httpd.h"
#include "html.h"
#include "cgi.h"

#include <string.h>

_mqx_int cgi_hvac_data(HTTPD_SESSION_STRUCT *session) {
	uint_32 Td = HVAC_GetDesiredTemperature();
    uint_32 Ta = HVAC_GetActualTemperature();
    char str[50];

    session->response.contenttype = CONTENT_TYPE_PLAIN;
    httpd_sendhdr(session, 0, 0);

    CGI_SEND_STR(HVAC_HVACModeName(HVAC_GetHVACMode()));

    sprintf(str, "%d.%d &deg;%c\n%d.%d &deg;%c\n", Td/10, Td%10, HVAC_GetTemperatureSymbol(), Ta/10, Ta%10, HVAC_GetTemperatureSymbol());
    httpd_sendstr(session->sock, str);
    
    CGI_SEND_STR(HVAC_GetFanMode() == Fan_Automatic ? "auto" : "on");

//  CGI_SEND_STR(HVAC_GetOutputName(0));
//  CGI_SEND_STR(HVAC_GetOutputName(1));
//  CGI_SEND_STR(HVAC_GetOutputName(2));
            
    CGI_SEND_STR(HVAC_GetOutput(HVAC_FAN_OUTPUT) ? "on" : "off");
    CGI_SEND_STR(HVAC_GetOutput(HVAC_HEAT_OUTPUT) ? "on" : "off");
    CGI_SEND_STR(HVAC_GetOutput(HVAC_COOL_OUTPUT) ? "on" : "off");      

    return session->request.content_len;
}

_mqx_int cgi_hvac_output(HTTPD_SESSION_STRUCT *session) {
    uint_32  len = 0;
    char     hvac[10], unit[10], fan[10], t[40];
    uint_32  temp,temp_fract=0;
    boolean  bParams = FALSE; 
    char buffer[100];

    session->response.contenttype = CONTENT_TYPE_HTML;
    httpd_sendhdr(session, 0, 0);

    httpd_sendstr(session->sock, "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//EN\">"
        "<html><head><title>HVAC Settings response</title>"
        "<meta http-equiv=\"REFRESH\" content=\"0;url=hvac.html\"></head>\n<body>\n");

//  html_link(session->sock,"Home","index.cgi");
//  html_link(session->sock,"HVAC Status","hvac.cgi");
//  html_link(session->sock,"HVAC Settings","hvac_input.cgi");
    //html_link(session->sock,"HVAC Log","hvac_log.txt");
    
    httpd_sendstr(session->sock, "<br><br>\n");
    
    if (session->request.content_len) {
        len = session->request.content_len;
        len = httpd_read(session, buffer, (int)((len > sizeof(buffer)) ? sizeof(buffer) : len));
        buffer[len] = 0;
        session->request.content_len -= len;
        len = 0;
        
        if (httpd_get_varval(session, buffer, "hvac", hvac, sizeof(hvac)) &&
            httpd_get_varval(session, buffer, "unit", unit, sizeof(unit)) &&
            httpd_get_varval(session, buffer, "temp", t, sizeof(t)) &&
            httpd_get_varval(session, buffer, "fan", fan, sizeof(fan))) {
            
            bParams =  TRUE;
         
            if (strcmp(hvac,"heat") == 0) {
                HVAC_SetHVACMode(HVAC_Heat);
            }
            else if (strcmp(hvac,"cool") == 0) {
                HVAC_SetHVACMode(HVAC_Cool);
            }
            else {
                HVAC_SetHVACMode(HVAC_Off);
            }
            
            if (strcmp(unit,"f") == 0)
                HVAC_SetTemperatureScale(Fahrenheit);
            else
                HVAC_SetTemperatureScale(Celsius);
         

            if (strcmp(fan,"auto") == 0) {
                HVAC_SetFanMode(Fan_Automatic);
            }
            else {
                HVAC_SetFanMode(Fan_On);
            }
        

        
            if (sscanf(t, "%d.%d", &temp, &temp_fract) >= 1) {
                if (temp_fract<10) {
                    HVAC_SetDesiredTemperature(temp * 10 + temp_fract);
                }
            }
        }
    }
  
   
    if (!bParams) {
        httpd_sendstr(session->sock, "No parameters received.<br>\n");
    }
    
    httpd_sendstr(session->sock, "</body></html>");
    return session->request.content_len;
}

#endif
