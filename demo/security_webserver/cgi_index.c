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
* $Version : 3.8.16.2$
* $Date    : Feb-13-2012$
*
* Comments:
*
*   Example of shell using RTCS.
*
*END************************************************************************/

#include "httpd.h"
#include "cgi.h"
#include "security_public.h"
#include "security_private.h"
#include <string.h>

static _mqx_int cgi_sec_data(HTTPD_SESSION_STRUCT *session);

const HTTPD_FN_LINK_STRUCT fn_lnk_tbl[] = {
    { 0, 0 }
};

const HTTPD_CGI_LINK_STRUCT cgi_lnk_tbl[] = {
   { "secdata",   cgi_sec_data},
    { 0, 0 }    // DO NOT REMOVE - last item - end of table
};


/*
 *  Function to send out input data so that the webpage can grab it
 *  These values can be seen at <ip_address_webserver>/secdata.cgi
 */   
static _mqx_int cgi_sec_data(HTTPD_SESSION_STRUCT *session) 
{
    TIME_STRUCT time;
    DATE_STRUCT date;
    short  i,hist;
    char buffer[36];
    
    /* Get elapsed time since bootup */
   _time_get(&time);
   _time_to_date(&time,&date);  
    sprintf(buffer,"%02d:%02d:%02d",date.HOUR,date.MINUTE,date.SECOND);

    session->response.contenttype = CONTENT_TYPE_PLAIN;
    httpd_sendhdr(session, 0, 0);
    /* Send out door status */      
    if(SEC_GetDoorStatus()==OPENED)
    {
        CGI_SEND_STR("Open");
    }
    else
    {
        CGI_SEND_STR("Closed");
    }
    
    /* Send out window status */
    if(SEC_GetWindowStatus()==OPENED)
    {
        CGI_SEND_STR("Open");
    }
    else
    {
        CGI_SEND_STR("Closed");
    }   
    
    /* Send out movement status */
    if(SEC_GetMovementStatus()==MOVING) 
    {
      CGI_SEND_STR("Movement");
    } 
    else 
    {
      CGI_SEND_STR("No Movement");
    }
        
    /* Send out current elapsed time */
    CGI_SEND_STR(buffer);
    
    /* Send out log queue */
    for(i = (MAX_QUEUE - 1);i >= 0;i--)
    {
          hist = (i+SEC_Params.HistoryIndex)%MAX_QUEUE;
          _time_to_date(&SEC_Params.History[hist].time,&date);
          if(SEC_Params.History[hist].eventptr != NULL)
          {
              sprintf(buffer,"%02d:%02d:%02d %s",
              date.HOUR,date.MINUTE,date.SECOND,SEC_Params.History[hist].eventptr);
          }
          else
          {
            buffer[0] = '\0';
          }
        CGI_SEND_STR(buffer);
    }
    
    /* Send out potentiometer in 10% increments */
    sprintf(buffer,"%d",(SEC_Params.GarageStatus / 409)*10);
    CGI_SEND_STR(buffer);


    return session->request.content_len;
}
